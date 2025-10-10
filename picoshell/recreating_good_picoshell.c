/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recreating_good_picoshell.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yosherau <yosherau@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 21:47:15 by yosherau          #+#    #+#             */
/*   Updated: 2025/10/08 23:21:28 by yosherau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int	count_commands(char **cmds[])
{
	int	index;

	index = 0;
	while (cmds[index])
		index++;
	return (index);
}

int	picoshell(char **cmds[])
{
	int		cmd_count = count_commands(cmds);
	pid_t	pids[cmd_count];
	int		curr_fd[2];
	int		prev_fd[2] = {-1, -1};
	int		index;

	index = -1;
	while (++index < cmd_count)
	{
		if (index < cmd_count - 1 && pipe(curr_fd) == -1)
			exit(EXIT_FAILURE);
		pids[index] = fork();
		if (pids[index] == -1)
			exit(EXIT_FAILURE);
		if (pids[index] == 0)
		{
			if (index > 0)
			{
				dup2(prev_fd[0], STDIN_FILENO);
				close(prev_fd[0]);
				close(prev_fd[1]);
			}
			if (index < cmd_count - 1)
			{
				dup2(curr_fd[1], STDOUT_FILENO);
				close(curr_fd[0]);
				close(curr_fd[1]);
			}
			execvp(cmds[index][0], cmds[index]);
		}
		if (index > 0)
		{
			close(prev_fd[0]);
			close(prev_fd[1]);
		}
		if (index < cmd_count - 1)
		{
			prev_fd[0] = curr_fd[0];
			prev_fd[1] = curr_fd[1];
		}
	}
	if (cmd_count > 1)
	{
		close(prev_fd[0]);
		close(prev_fd[1]);
	}
	index = -1;
	while (++index < cmd_count)
		wait(NULL);
	return (EXIT_SUCCESS);
}

static int count_pipes(char **argv)
{
	int count = 1; // At least one command
	for (int i = 1; argv[i]; i++)
		if (strcmp(argv[i], "|") == 0)
			count++;
	return count;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s cmd1 [args...] '|' cmd2 [args...] ...\n", argv[0]);
		return EXIT_FAILURE;
	}

	int command_count = count_pipes(argv);
	char ***cmds = calloc(command_count + 1, sizeof(char **));
	if (!cmds)
		return EXIT_FAILURE;

	int cmd_index = 0;
	int arg_start = 1;
	for (int i = 1; i <= argc; i++)
	{
		// Either end of input or found a "|"
		if (argv[i] == NULL || strcmp(argv[i], "|") == 0)
		{
			int arg_count = i - arg_start;
			cmds[cmd_index] = calloc(arg_count + 1, sizeof(char *));
			if (!cmds[cmd_index])
				return EXIT_FAILURE;

			for (int j = 0; j < arg_count; j++)
				cmds[cmd_index][j] = argv[arg_start + j];
			cmds[cmd_index][arg_count] = NULL;

			cmd_index++;
			arg_start = i + 1;
		}
	}

	cmds[command_count] = NULL;

	int result = picoshell(cmds);

	// Cleanup (though argv[] are stack-allocated, cmds[] is heap)
	for (int i = 0; i < command_count; i++)
		free(cmds[i]);
	free(cmds);

	return result;
}