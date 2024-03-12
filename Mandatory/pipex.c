/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:36:06 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/12 06:24:05 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_cmds(t_var *var, int argc, char *argv[], char *envp[])
{
	var->fullpath = get_env_path(envp);
	var->pids = malloc((argc - 3) * sizeof(int));
	var->i = 2;
	while (var->i < argc - 1)
	{
		var->argvcmd = cmdsplit(argv[var->i]);
		var->path = check_cmd_access(var->argvcmd[0], var->fullpath);
		var->pids[var->i - 2] = fork();
		if (var->pids[var->i - 2] == 0)
		{
			if (var->path == NULL)
				exit(127);
			duping(var, argc, argv);
			if (execve(var->path, var->argvcmd, envp) == -1)
			{
				perror("err in execve");
				exit(1);
			}
		}
		free(var->path);
		free_2d_arr(var->argvcmd);
		var->i++;
	}
	free_2d_arr(var->fullpath);
}

int	helper(int argc, char *argv[], char *envp[])
{
	t_var	var;

	if (open_pipes(&var, argc) == -1)
	{
		ft_printf("pipeng error");
		exit(175);
	}
	execute_cmds(&var, argc, argv, envp);
	close_all(&var, argc);
	if (wait_childes(&var, argc) == -1)
	{
		ft_printf("waiting error");
		exit(176);
	}
	return (var.last_exit_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
		return (ft_printf("args number less or more then 5\n"));
	if (access(argv[1], F_OK) == -1)
	{
		perror("input not Exist");
		return (127);
	}
	return (helper(argc, argv, envp));
}
