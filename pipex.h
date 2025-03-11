#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h> //open()
# include <sys/wait.h> //wait()
# include <sys/types.h> //pid_t
// # include <sys/stat.h> //not so sure for now

//functions
void	free_tab(char **tab);
char	*get_path(char *cmd, char **env);
void	child_process(int fd1, int end[], char *cmd1, char **env);
void	parent_process(int fd2, int end[], char *cmd2, char **env);

#endif