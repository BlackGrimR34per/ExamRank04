/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bad_picoshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yosherau <yosherau@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:10:52 by yosherau          #+#    #+#             */
/*   Updated: 2025/10/08 13:58:20 by yosherau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void	create_pipes(int command_count, int pipes[][2])
{
	int	index;

	index = -1;
	while (++index < command_count)
		if (pipe(pipes[index]) == -1)
			exit(EXIT_FAILURE);
}

int	count_commands(char **cmds[])
{
	int	count;

	count = 0;
	while (cmds[count])
		count++;
	return (count);
}

int	picoshell(char **cmds[])
{
	int	index;
	int	jindex;
	int	command_count = count_commands(cmds);
	int	pids[command_count];
	int	pipes[command_count][2];

	index = -1;
	create_pipes(command_count, pipes);
	while (++index < command_count)
	{
		pids[index] = fork();
		if (pids[index] == -1)
		exit(EXIT_FAILURE);
		if (pids[index] == 0)
		{
			jindex = -1;
			if (index > 0)
				dup2(pipes[index - 1][0], STDIN_FILENO);
			if (index < command_count - 1)
				dup2(pipes[index - 1][1], STDOUT_FILENO);
			while (++jindex < command_count - 1)
			{
				close(pipes[jindex][0]);
				close(pipes[jindex][1]);
			}
			execvp(cmds[index][0], cmds[index]);
		}
	}
	index = -1;
	while (++index < command_count - 1)
	{
		close(pipes[index][0]);
		close(pipes[index][1]);
	}
	index = -1;
	while (++index < command_count)
		waitpid(pids[index], NULL, 0);
	return (EXIT_SUCCESS);
}

int	main()
{
	char *cmd1[] = {"/bin/ls", NULL};
	char *cmd2[] = {"/usr/bin/sleep", "3", NULL};

	char **inputs[] = {cmd1, cmd2, NULL};
	picoshell(inputs);
}