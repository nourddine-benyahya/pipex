/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:36:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/02 19:48:28 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Extra-Library/libft.h"
#include <stdio.h>
#include <fcntl.h>

typedef struct t_list{
	char	**argvcmd;
	char	*path;
	char	**fullpath;
	int		pip;
	int		io[2];
	int		i;
	int		pid;
	int		(*fd)[2];
} v_list;

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
	free(helper);
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

void	free_2d_arr(char ***arr)
{
	while (**arr != NULL)
	{
		free(**arr);
		**arr = NULL;
		(*arr)++;
	}
	arr = NULL;
}

int	duping(int i, int (*fd)[2], int io[2], int argc)
{
	if (i == 2)
	{
		dup2(io[0], STDIN_FILENO);
		dup2(fd[i - 2][1], STDOUT_FILENO);
		close(fd[i - 2][0]);
	}
	else if (i == argc - 2 && argc >= 5)
	{
		dup2(fd[i - 3][0], STDIN_FILENO);
		dup2(io[1], STDOUT_FILENO);
		close(fd[i - 2][1]);
		close(fd[i - 2][0]);
	}
	else if (i != argc - 2)
	{
		dup2(fd[i - 3][0], STDIN_FILENO);
		dup2(fd[i - 2][1], STDOUT_FILENO);
		close(fd[i - 2][0]);
	}
	return (0);
}

char	**ft_speacialsplit(char *input)
{
	char	**arr;
	char	*str;
	char	*str2;

	if (ft_strchr(input, 39) != NULL)
	{
		arr = malloc(sizeof(char *) * 3);
		if (!arr)
			return (NULL);
		str = ft_substr(strchr(input, ' ') + 2, 0, \
			ft_strlen(strchr(input, ' ') + 2) - 1);
		if (!str)
			return (free(arr),NULL);
		str2 = ft_substr(input,0 , strlen(input) - strlen(str) - 3);
		if (!str2)
			return (free(arr), free(str2), NULL); 
		arr[0] = str2;
		arr[1] = str;
		arr[2] = NULL;
	}
	else
		arr = ft_split(input, ' ');
	return (arr);
}

int execute_command(int argc, char *argv[], char *envp[])
{
	v_list	var;

	var.io[1] = open(argv[argc - 1], O_RDWR | O_TRUNC);
	var.io[0] = open(argv[1], O_RDWR);
	var.i = 2;
	var.fd = malloc((argc - 2) * sizeof(int [2]));
	var.fullpath = get_env_path(envp);
	while (var.i < argc - 1)
	{
		var.argvcmd = ft_speacialsplit(argv[var.i]);
		var.path = check_cmd_access(var.argvcmd[0], var.fullpath);
		if (var.path == NULL)
			return (1);
		var.pip = pipe(var.fd[var.i - 2]);
		if (var.pip == -1)
			perror("pipe error");
		var.pid = fork();
		if (var.pid == 0)
		{
			duping(var.i, var.fd, var.io, argc);
			if (execve(var.path, var.argvcmd, envp) == -1)
				perror("err in execve");
		}
		else
			wait(NULL);
		close(var.fd[var.i - 2][1]);
		var.i++;
		free(var.path);
		var.path = NULL;
		free_2d_arr(&var.argvcmd);
		var.argvcmd = NULL;
	}
	free_2d_arr(&var.fullpath);
	free(var.fd);
	close(var.io[0]);
	close(var.io[1]);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc < 4)
		return (ft_printf("arr in nb of args"));
	if (access(argv[1], F_OK) == -1 || access(argv[argc - 1], F_OK) == -1)
	{
		perror("error in io_files (io_file not exist)");
		return (1);
	}
	execute_command(argc, argv, envp);
	return (0);
}
