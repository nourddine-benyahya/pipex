/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:36:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/02/28 00:40:15 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Extra-Library/libft.h"
#include <stdio.h>

char	**get_env_path(char *envp[])
{
	char	*helper;
	char	**path;
	char	*backup;
	int		i;

	while (!ft_strnstr(*envp, "PATH=", 5))
		envp++;
	helper = ft_substr(*envp, 5, ft_strlen(*envp) - 5);
	if (!helper)
		return (NULL);
	path = ft_split(helper, ':');
	if (!path)
		return (NULL);
	i = 0;
	while (path[i] != NULL)
	{
		backup = path[i];
		free(path[i]);
		path[i] = ft_strjoin(backup, "/");
		i++;
	}
	return (path);
}

char	*check_cmd_access(char *cmd, char **path)
{
	char	*fullpath;
	int		i;

	i = 0;
	while (path[i] != NULL)
	{
		fullpath = ft_strjoin(path[i], cmd);
		if (access(fullpath, F_OK) == 0)
			return (fullpath);
		free (fullpath);
		i++;
	}
	perror("command not faund");
	return (NULL);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		fd[2];
	int		i;
	int		pid;
	char	**argvcmd;
	char	*path;

	if (argc < 4)
		return (ft_printf("arr in nb of args"));
	if (access(argv[1], F_OK) == -1 || access(argv[argc - 1], F_OK) == -1)
	{
		perror("error in io_files (io_file not exist)");
		return (1);
	}
	i = 2;
	while (i < argc)
	{
		argvcmd = ft_split(argv[i], ' ');
		path = check_cmd_access(argvcmd[0], get_env_path(envp));
		if (path == NULL)
			return (1);
		pid = fork();
		if (execve(path, argvcmd, NULL) == -1)
			perror("err in execve");
		i++;
	}
}
