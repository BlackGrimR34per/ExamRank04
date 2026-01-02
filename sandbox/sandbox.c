/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yosherau <yosherau@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 17:37:18 by yosherau          #+#    #+#             */
/*   Updated: 2026/01/02 01:01:44 by yosherau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status = 0;
	int		exit_status = 0;
	int		signal_status = 0;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		alarm(timeout);
		f();
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		signal_status = WTERMSIG(status);

	if (verbose)
	{
		if (exit_status)
		{
			printf("Bad function: exited with code %d\n", exit_status);
			return (EXIT_SUCCESS);
		}
		else if (signal_status == SIGALRM)
		{
			printf("Bad function: timed out after %d seconds\n", timeout);
			return (EXIT_SUCCESS);
		}
		else if (signal_status)
		{
			printf("Bad function: %s\n", strsignal(signal_status));
			return (EXIT_SUCCESS);
		}
		else if (!exit_status)
			printf("Nice function!\n");
	}
	return (EXIT_FAILURE);
}