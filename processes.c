/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:29:20 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/11 11:30:38 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//use dup2() to transfer the infile and outfile
// for every dup2(), check if it really worked
// => if < 0 then close all corresponding fds and end[]

void	child_process(int fd1, int end[], char *cmd1, char **env)
{
	char	**s_cmd1;

	s_cmd1 = ft_split(cmd1, ' ');
	dup2(fd1, STDIN_FILENO);//you could also write 0 technically
	dup2(end[1], STDOUT_FILENO);//and here 1 is valid
	// here, the infile (cmd1) should be stdin (fd1, 0), and end[1] should be stdout (end[1], 1)
	//=> read from the infile, write the output of cmd 1 
	//	inside the child-end of the pipe
	// okay, so, one dup2() to copy infile inside the stdin, so dup2(av[1], 0)
	// and another one to copy end[1] inside stdout
	// don't forget to close infile and end[1] to avoid leaks too
	close(fd1);
	close(end[1]);
	// but I thought dup2() closed the old file anready??
	//	==> nah, it closes the new file if it already exists, my  b a d
	// and then, execute (execve) cmd1
	if (execve(get_path(cmd1, env), s_cmd1, env) == -1)
	{
		// close (end[0]) as it's the only one not closed?
		free_tab(s_cmd1);
		ft_putstr_fd("execve() failed for cmd1", 2);
		exit(EXIT_FAILURE);
	}
	// if execve == -1 (< 0), failed, so free_tab s_cmd1
	// + perror("execve() failed for cmd1")
}

void	parent_process(int fd2, int end[], char *cmd2, char **env)
{
	char	**s_cmd2;

	s_cmd2 = ft_split(cmd2, ' ');
	dup2(end[0], STDIN_FILENO);
	dup2(fd2, STDOUT_FILENO);
	// start with a waitîd() for the child to finish their process
	// here, cmd2 read from the parent-end of the pipe (that reads from the child-end)
	// and write the output inside the stdout
	// so, dup2() from end[0] as the stdin, and dup2() from outfile (av[4]) as stdout
	// here, close end[0] even tho it's read-only ; and end[1] should already be closed
	close(fd2);
	close(end[1]);
	close(end[0]);
	// and then, execute (execve) cmd2
	if (execve(get_path(cmd2, env), s_cmd2, env) == -1)
	{
		// no need to close anything else here?
		free_tab(s_cmd2);
		ft_putstr_fd("execve() failed for cmd2", 2);
		exit(EXIT_FAILURE);
	}
	//=>check if it worked?
	// if execve == -1 (< 0), failed, so free_tab s_cmd2
	// + perror("execve() failed for cmd2")
}

/*
Next step :
if cmd1/cmd2 not found, print ("%s: command not found", cmd1/2) on the stderr (2)
so not a ft_printf but almost, like, ft_printf ft_str_fd(2)?

tips: perror() prints the message + errno (the specific error)
*/