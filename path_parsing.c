/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-brie <mle-brie@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:28:44 by mle-brie          #+#    #+#             */
/*   Updated: 2025/03/12 15:06:00 by mle-brie         ###   ########.fr       */
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

char	*find_path(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, ft_strlen(name)) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**all_paths;
	char	**s_cmd;
	char	*path_part;
	char	*executable;

	executable = NULL;
	all_paths = ft_split(find_path("PATH", env), ':');
	if (!all_paths)
		return (free_tab(all_paths), NULL);
	if (!cmd || !(*cmd))
		return (free_tab(all_paths), NULL);
	s_cmd = ft_split(cmd, ' ');
	i = -1;
	while (all_paths[++i])
	{
		path_part = ft_strjoin(all_paths[i], "/");
		executable = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(executable, F_OK | X_OK) == 0)
			return (free_tab(s_cmd), free_tab(all_paths), executable);
		free(executable);
	}
	return (free_tab(s_cmd), free_tab(all_paths), NULL);
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