/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:28:44 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/11 11:28:45 by mle-brie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/*
I first need to find the PATH line, so, for this, use a substr and a strcmp
Then, I will be able to use a ft_split inside the path with ':'
and a strjoin to add another / and identify a path
=> ie. check if it's a valid command 
*/

char	*find_path(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')//move through every line until you find the =
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, ft_strlen(name)) == 0)
		{
			free(sub);
			return (env[i] + j + 1);//+ 1 skips =
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**possible_paths;
	char	**splitted_cmd;//in case the cmd include multiple args (like ls -la)
	char	*path_part;
	char	*executable;

	if(!(possible_paths = ft_split(find_path("PATH", env), ':')))
		return(cmd);
	splitted_cmd = ft_split(cmd, ' ');
	i = -1;
	while (possible_paths[++i])
	{
		path_part = ft_strjoin(possible_paths[i], "/");//make it a full path
		executable = ft_strjoin(path_part, splitted_cmd[0]);
		free(path_part);
		if (access(executable, F_OK | X_OK) == 0)//if it exists and is executable
		{
			free_tab(splitted_cmd);
			return (executable);//return the path if it was found
		}
		free(executable);//if not found, so that it does it again
	}
	free_tab(possible_paths);
	free_tab(splitted_cmd);
	return(cmd);//if nothing was found, return cmd as it is
}

// int main(int ac, char *av[], char **env)
// {
//     if (ac < 2)
//     {
//         printf("Usage: %s <command>\n", av[0]);
//         return (1);
//     }
//     char *cmd = av[1];
//     char *path = get_path(cmd, env);

//     printf("Command: %s\n", cmd);
//     printf("Full path: %s\n", path);

//     // If path is different from cmd, we need to free it
//     if (path != cmd)
//         free(path);
//     return (0);
// }