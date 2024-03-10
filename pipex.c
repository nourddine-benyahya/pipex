/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:36:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/10 21:53:28 by nbenyahy         ###   ########.fr       */
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

char	*check_cmd_access(char *cmd, char **fullpath)
{
	char	*path;
	int		i;

	i = 0;
	while (fullpath[i] != NULL)
	{
		path = ft_strjoin(fullpath[i], cmd);
		if (access(path, F_OK) == 0)
			return (path);
		free (path);
		i++;
	}
	perror("command not found");
	return (NULL);
}

void close_all(t_var *var,int argc)
{
	argc -= 4;
	while (argc--)
	{
		close(var->fd[argc][0]);
		close(var->fd[argc][1]);
	}
	close(var->io[0]);
	close(var->io[1]);
}

void open_pipes(t_var *var, int argc)
{
	int i = 0;

	var->fd = malloc((argc - 4) * sizeof(int[2]));

	while (i < argc - 4)
		pipe(var->fd[i++]);
}


int	duping(t_var *var, int argc, char *argv[])
{
	var->io[1] = open(argv[argc - 1], O_RDWR | O_TRUNC);
	var->io[0] = open(argv[1], O_RDWR);
	if (var->i == 2)
	{
		dup2(var->io[0], STDIN_FILENO);
		dup2(var->fd[var->i - 2][1], STDOUT_FILENO);
	}
	else if (var->i == argc - 2)
	{
		dup2(var->fd[var->i - 3][0], STDIN_FILENO);
		dup2(var->io[1], STDOUT_FILENO);
	}
	else if (var->i != argc - 2)
	{
		dup2(var->fd[var->i - 3][0], STDIN_FILENO);
		dup2(var->fd[var->i - 2][1], STDOUT_FILENO);
	}
	close_all(var,argc);
	return (0);
}


void execute_cmds(t_var *var, int argc, char *argv[], char *envp[])
{
	var->fullpath = get_env_path(envp);
	var->pids = malloc((argc - 3) * sizeof(int));

	while (var->i < argc - 1)
	{
		var->argvcmd = cmdsplit(argv[var->i]);  
		var->path = check_cmd_access(var->argvcmd[0], var->fullpath);
		var->pids[var->i - 2] = fork();
		if (var->pids[var->i - 2] == 0)
		{
			duping(var, argc, argv);
			if (execve(var->path, var->argvcmd, envp) == -1){
				perror("err in execve");
				exit(1);
			}
		}
		free(var->path);
		free_2d_arr(var->argvcmd);
		var->i++;
	}
	free_2d_arr(var->fullpath);
	// free(var->fd);
}

void	wait_childes(t_var *var, int argc)
{
	int status;

	status = 0;
	var->i = 2;
	while(var->i < argc - 1)
		waitpid(var->pids[var->i++ - 2], &status, 0);
	free(var->pids);
}

void	command_loop(t_var *var, int argc, char *argv[], char *envp[])
{
	open_pipes(var, argc);
	execute_cmds(var, argc, argv, envp);
	close_all(var ,argc);
	wait_childes(var, argc);
}

int	execute_command(int argc, char *argv[], char *envp[])
{
	t_var	var;

	var.i = 2; 
	command_loop(&var, argc, argv, envp);
	free(var.fd);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc < 5)
		return (ft_printf("args number less then 5\n"));
	if (access(argv[1], F_OK) == -1 || access(argv[argc - 1], F_OK) == -1)
	{
		perror("io_file not Exist");
		return (127);
	}
	execute_command(argc, argv, envp);
	// while (1);
	// system("leaks a.out");
	return (0);
}
