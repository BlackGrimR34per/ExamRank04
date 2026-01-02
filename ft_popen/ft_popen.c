/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yosherau <yosherau@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 14:47:57 by yosherau          #+#    #+#             */
/*   Updated: 2026/01/02 15:37:05 by yosherau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int		fds[2];
	pid_t	pid;
	int		exit_status;

	if (pipe(fds) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		if (type == 'r')
			dup2(fds[1], STDOUT_FILENO);
		else if (type == 'w')
			dup2(fds[1], STDIN_FILENO);
		execvp(file, argv);
		// This line may not be needed?
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &exit_status, 0);
	if (WIFEXITED(exit_status))
		return (EXIT_FAILURE);
	if (type == 'r')
		return (fds[0]);
	else
		return (fds[1]);
}