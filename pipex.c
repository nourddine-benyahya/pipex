/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:36:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/01 17:23:26 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Extra-Library/libft.h"
#include <stdio.h>
#include <fcntl.h>

// typedef struct t_list{
// 	char	**argvcmd;
// 	char	*path;
// 	int		pip;
// 	int		io[2];
// 	int		i;
// 	int		pid;
// 	int		(*fd)[2];
// } v_list

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

int execute_command(int argc, char *argv[], char *envp[])
{
	char	**argvcmd;
	char	*path;
	int		pip;
	int		io[2];
	int		i;
	int		pid;
	int		(*fd)[2];

	io[1] = open(argv[argc - 1], O_RDWR | O_TRUNC);
	io[0] = open(argv[1], O_RDWR);
	i = 2;
	fd = malloc((argc - 2) * sizeof(int[2]));
	while (i < argc - 1)
	{
		argvcmd = ft_split(argv[i], ' ');
		path = check_cmd_access(argvcmd[0], get_env_path(envp));
		if (path == NULL)
			return (1);
		pip = pipe(fd[i - 2]);
		if (pip == -1)
			perror("pipe error");
		pid = fork();
		if (pid == 0) {
			duping(i, fd, io, argc);
			if (execve(path, argvcmd, envp) == -1)
				perror("err in execve");
		}
		else
			wait(NULL);
        close(fd[i - 2][1]);
		i++;
		free(path);
		path = NULL;
		free_2d_arr(&argvcmd);
	}
	close(io[0]);
	close(io[1]);
	return(0);
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
	return (execute_command(argc, argv, envp));
}
