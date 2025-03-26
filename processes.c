/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:29:20 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/26 16:02:42 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_pipes(int *end)
{
	close(end[1]);
	close(end[0]);
}

void	check_files(int fd, int *end, int type)
{
	if (fd < 0)
	{
		close_pipes(end);
		if (type == 1)
			ft_putstr_fd("Error opening infile\n", 2);
		else if (type == 2)
			ft_putstr_fd("Error opening outfile\n", 2);
		exit(EXIT_FAILURE);
	}
}

void	first_child(char *av[], int end[], char *cmd1, char **env)
{
	char	**s_cmd1;
	char	*path;
	int		fd1;

	fd1 = open(av[1], O_RDONLY);
	check_files(fd1, end, 1);
	s_cmd1 = ft_split(cmd1, ' ');
	dup2(fd1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(fd1);
	close_pipes(end);
	path = get_path(cmd1, env);
	if (!path)
	{
		free_tab(s_cmd1);
		ft_putstr_fd("Cmd1 not found\n", 2);
		exit(EXIT_FAILURE);
	}
	if (execve(path, s_cmd1, env) == -1)
	{
		free_tab(s_cmd1);
		ft_putstr_fd("execve() failed for cmd1\n", 2);
		exit(EXIT_FAILURE);
	}
	exit(0);
}

void	second_child(char *av[], int end[], char *cmd2, char **env)
{
	char	**s_cmd2;
	char	*path;
	int		fd2;

	fd2 = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	check_files(fd2, end, 2);
	s_cmd2 = ft_split(cmd2, ' ');
	dup2(end[0], STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
	close(fd2);
	close(end[0]);
	path = get_path(cmd2, env);
	if (!path)
	{
		free_tab(s_cmd2);
		exit(127);
	}
	if (execve(path, s_cmd2, env) == -1)
	{
		free_tab(s_cmd2);
		ft_putstr_fd("execve() failed for cmd2\n", 2);
		exit(EXIT_FAILURE);
	}
	exit(0);
}
