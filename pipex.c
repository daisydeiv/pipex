/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:29:12 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/12 15:23:08 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_failure(int fd1, int fd2, int type)
{
	close(fd1);
	close(fd2);
	if (type == 1)
		perror("Pipe failed");
	else if (type == 2)
		perror("Fork failed");
	else if (type == 3)
		perror("Waitpid failed");
	exit(EXIT_FAILURE);
}

int	pipex(int fd[2], char *cmd1, char *cmd2, char **env)
{
	int		end[2];
	pid_t	pid;

	if (pipe(end) < 0)
		close_failure(fd[0], fd[1], 1);
	pid = fork();
	if (pid < 0)
		close_failure(fd[0], fd[1], 2);
	if (!pid)
		child_process(fd[0], end, cmd1, env);
	else
	{
		close(end[1]);
		if (waitpid(pid, NULL, 0) == -1)
			close_failure(fd[0], fd[1], 3);
		parent_process(fd[1], end, cmd2, env);
	}
	return (EXIT_SUCCESS);
}

int	main(int ac, char *av[], char **env)
{
	int	fd[2];

	if (ac != 5)
	{
		ft_putstr_fd("Expected : infile \"cmd1\" \"cmd2\" outfile", 2);
		exit(EXIT_FAILURE);
	}
	fd[0] = open(av[1], O_RDONLY);
	if (fd[0] < 0)
	{
		ft_putstr_fd("Error opening infile", 2);
		exit(EXIT_FAILURE);
	}
	fd[1] = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd[1] < 0)
	{
		ft_putstr_fd("Error opening outfile", 2);
		close(fd[1]);
		exit(EXIT_FAILURE);
	}
	if (pipex(fd, av[2], av[3], env) == EXIT_FAILURE)
		perror("pipex failed");
	return (close(fd[0]), close(fd[1]), EXIT_SUCCESS);
}
