/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:29:12 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/11 11:40:51 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_failure(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
	exit(EXIT_FAILURE);
}

int	pipex(int fd1, int fd2, char *cmd1, char *cmd2, char **env)
{
	int	end[2];//end[0] = read; end[1] = write
	pid_t	pid;

	if (pipe(end) < 0)
		perror("Pipe failed");
		close_failure(fd1, fd2);
	pid = fork();//fork split in two processes: one parent and maybe multiple children
	if (pid < 0)//not working
		perror("Fork failed");
		close_failure(fd1, fd2);
	if (!pid)//0 = child
		child_process(fd1, end, cmd1, env);
	else//0+ is parent
	{
		close(end[1]);//close here to be more secure, and deal with timing issues
		if (waitpid(pid, NULL, 0) == -1)
		{
			close(end[0]);
			perror("Waitpid failed");
			close_failure(fd1, fd2);
		}
		parent_process(fd2, end, cmd2, env);
	}
	return (EXIT_SUCCESS);
}

int	main(int ac, char *av[], char **env)
{
	int	fd1;
	int	fd2;

	if (ac != 5)//fd1, cmd1, cmd2, fd2
		ft_putstr_fd("Expected : infile \"cmd1\" \"cmd2\" outfile", 2);
		exit(EXIT_FAILURE);
	fd1 = open(av[1], O_RDONLY);//open infile
	if (fd1 < 0)
	{
		ft_putstr_fd("Error opening infile", 2);
		exit(EXIT_FAILURE);
	}
	fd2 = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);//create (if not exist) and open an outfile
	//0644 : Owner 6 = rw, Group & Others 4 = r
	if (fd2 < 0)
	{
		ft_putstr_fd("Error opening outfile", 2);
		close(fd1);//because exit() will leave, so no double
		exit(EXIT_FAILURE);
	}
	if (pipex(fd1, fd2, av[2], av[3], env) == EXIT_FAILURE)
		perror("pipex failed");
	close(fd1);
	close(fd2);
	return (EXIT_SUCCESS);//it's like return 1/0 except it works with exit() too, so, more clear
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