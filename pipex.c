/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:29:12 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/12 14:23:31 by mle-brie         ###   ########.fr       */
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

int	pipex(int fd1, int fd2, char *cmd1, char *cmd2, char **env)
{
	int	end[2];
	pid_t	pid;

	if (pipe(end) < 0)
		close_failure(fd1, fd2, 1);
	pid = fork();
	if (pid < 0)
		close_failure(fd1, fd2, 2);
	if (!pid)
		child_process(fd1, end, cmd1, env);
	else
	{
		close(end[1]);
		if (waitpid(pid, NULL, 0) == -1)
			close_failure(fd1, fd2, 3);
		parent_process(fd2, end, cmd2, env);
	}
	return (EXIT_SUCCESS);
}

int	main(int ac, char *av[], char **env)
{
	int	fd1;
	int	fd2;

	// ac = 5;
	// av[1] = "lorem50.txt";
	// av[2] = "ls -l";
	// av[3] = "";
	// av[4] = "test50.txt";

	if (ac != 5)
	{
		ft_putstr_fd("Expected : infile \"cmd1\" \"cmd2\" outfile", 2);
		exit(EXIT_FAILURE);
	}
	fd1 = open(av[1], O_RDONLY);
	if (fd1 < 0)
	{
		ft_putstr_fd("Error opening infile", 2);
		exit(EXIT_FAILURE);
	}
	fd2 = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd2 < 0)
	{
		ft_putstr_fd("Error opening outfile", 2);
		close(fd1);
		exit(EXIT_FAILURE);
	}
	if (pipex(fd1, fd2, av[2], av[3], env) == EXIT_FAILURE)
		perror("pipex failed");
	close(fd1);
	close(fd2);
	return (EXIT_SUCCESS);
}

/*
char **envp
environmental variable 
contains a line PATH (type env in terminal) for all possible paths to command binaries
you can/should split the paths with :
=> get_path or path_parsing or path_finder function

exec as a function tries every possible path until it finds the right one
if nothing, does nothing and return -1
if found, execute the cmd, delete ongoing processes (variables too, so no leaks) and exit
*/