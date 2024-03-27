/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_funs_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 06:06:43 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/24 01:57:36 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*check_cmd_access(char *cmd, char **fullpath)
{
	char	*path;
	int		i;

	i = 0;
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
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

void	close_all(t_var *var, int argc)
{
	argc -= 4;
	while (argc--)
	{
		close(var->fd[argc][0]);
		close(var->fd[argc][1]);
	}
	close(var->io[0]);
	close(var->io[1]);
	free(var->fd);
}

int	open_pipes(t_var *var, int argc)
{
	int	i;

	i = 0;
	var->fd = malloc((argc - 4) * sizeof(int [2]));
	if (!var->fd)
		return (-1);
	while (i < argc - 4)
	{
		if (pipe(var->fd[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	duping(t_var *var, int argc, char *argv[])
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) != 0)
	{
		var->io[1] = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
		var->io[0] = open(argv[1], O_RDONLY);
	}
	else
		var->io[1] = open(argv[argc - 1], O_WRONLY | O_APPEND | O_CREAT, 0777);
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
	close_all(var, argc);
	return (0);
}

int	wait_childes(t_var *var, int argc, char *argv[])
{
	int	status;

	status = 0;
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		var->i = 3;
	else
		var->i = 2;
	while (var->i < argc - 1)
	{
		if (waitpid(var->pids[var->i++ - 2], &status, 0) == -1)
			return (-1);
	}
	var->last_exit_status = WEXITSTATUS(status);
	free(var->pids);
	return (0);
}
