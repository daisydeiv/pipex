/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:29:12 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/26 16:02:00 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_failure(int *end, int type)
{
	if (type == 1)
		perror("Pipe failed");
	else if (type == 2)
	{
		close(end[0]);
		close(end[1]);
		perror("Fork failed");
	}
	exit(EXIT_FAILURE);
}

int	pipex(pid_t pid[2], char **env, char *av[])
{
	int		end[2];
	char	*cmd1;
	char	*cmd2;
	int		status;

	status = 0;
	cmd1 = av[2];
	cmd2 = av[3];
	if (pipe(end) < 0)
		close_failure(end, 1);
	pid[0] = fork();
	if (pid[0] < 0)
		close_failure(end, 2);
	if (pid[0] == 0)
		first_child(av, end, cmd1, env);
	close(end[1]);
	pid[1] = fork();
	if (pid[1] < 0)
		close_failure(end, 2);
	if (pid[1] == 0)
		second_child(av, end, cmd2, env);
	close(end[0]);
	waitpid(pid[0], &status, 0);
	waitpid(pid[1], &status, 0);
	return (status);
}

int	main(int ac, char *av[], char **env)
{
	pid_t	pid[2];
	int		status;

	if (ac != 5)
	{
		ft_putstr_fd("Expected : infile \"cmd1\" \"cmd2\" outfile\n", 2);
		exit(EXIT_FAILURE);
	}
	status = pipex(pid, env, av);
	return (WEXITSTATUS(status));
}

/*
"big_text.txt" "cat" "head -2" "outfile.txt"
"big_text.txt" "sleep 3" "ls" "outfile.txt"
*/