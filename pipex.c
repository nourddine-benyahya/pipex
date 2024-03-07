/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:36:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/07 19:26:07 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Extra-Library/libft.h"
#include <stdio.h>
#include <fcntl.h>
#include "pipex.h"

void	free_2d_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
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

int	command_loop(t_var *var, int argc, char *argv[], char *envp[])
{
	while (var->i < argc - 1)
	{
		var->argvcmd = cmdsplait(argv[var->i]);
		var->path = check_cmd_access(var->argvcmd[0], var->fullpath);
		if (var->path == NULL)
			return (1);
		var->pip = pipe(var->fd[var->i - 2]);
		if (var->pip == -1)
			perror("pipe error");
		var->pid = fork();
		if (var->pid == 0)
		{
			duping(var->i, var->fd, var->io, argc);
			if (execve(var->path, var->argvcmd, envp) == -1)
				perror("err in execve");
		}
		else
			wait(NULL);
		close(var->fd[var->i - 2][1]);
		var->i++;
		free(var->path);
		free_2d_arr(var->argvcmd);
	}
	return (1);
}

int	execute_command(int argc, char *argv[], char *envp[])
{
	t_var	var;

	var.io[1] = open(argv[argc - 1], O_RDWR | O_TRUNC);
	var.io[0] = open(argv[1], O_RDWR);
	var.i = 2;
	var.fd = malloc((argc - 2) * sizeof(int [2]));
	var.fullpath = get_env_path(envp);
	command_loop(&var, argc, argv, envp);
	free_2d_arr(var.fullpath);
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
	// system("leaks a.out");
	return (0);
}
