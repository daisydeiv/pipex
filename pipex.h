/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:57:55 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/13 11:02:09 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
// # include <sys/stat.h>

//functions
void	free_tab(char **tab);
char	*get_path(char *cmd, char **env);
void	child_process(int fd1, int end[], char *cmd1, char **env);
void	parent_process(int fd2, int end[], char *cmd2, char **env);

#endif