/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yosherau <yosherau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 17:37:18 by yosherau          #+#    #+#             */
/*   Updated: 2025/12/30 18:06:03 by yosherau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		exit_status;

	pid = fork();
	if (pid == 0)
	{
		alarm(timeout);
		f();
		
	}
	waitpid(pid, &exit_status, NULL);
	
}