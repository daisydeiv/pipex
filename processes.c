/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:29:20 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/12 14:26:17 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(int fd1, int end[], char *cmd1, char **env)
{
	char	**s_cmd1;
	char	*path;

	s_cmd1 = ft_split(cmd1, ' ');
	dup2(fd1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(fd1);
	close(end[1]);
	if (!(path = get_path(cmd1, env)))
	{
		free_tab(s_cmd1);
		exit(EXIT_FAILURE);
	}
	if (execve(path, s_cmd1, env) == -1)
	{
		// close (end[0]) as it's the only one not closed?
		free_tab(s_cmd1);
		ft_putstr_fd("execve() failed for cmd1", 2);
		exit(EXIT_FAILURE);
	}
}

void	parent_process(int fd2, int end[], char *cmd2, char **env)
{
	char	**s_cmd2;
	char	*path;

	s_cmd2 = ft_split(cmd2, ' ');
	dup2(end[0], STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
	close(fd2);
	// close(end[0]);
	if (!(path = get_path(cmd2, env)))
	{
		free_tab(s_cmd2);
		exit(EXIT_FAILURE);
	}
	if (execve(path, s_cmd2, env) == -1)
	{
		free_tab(s_cmd2);
		ft_putstr_fd("execve() failed for cmd2", 2);
		exit(EXIT_FAILURE);
	}
}
