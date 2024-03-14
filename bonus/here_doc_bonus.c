/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 08:45:56 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/14 00:44:50 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	read_stdin(t_var *var, char *argv[])
{
	char	*str;
	char	*tmp;

	str = NULL;
	tmp = str;
	close(var->fd[0][0]);
	while (1)
	{
		ft_printf("heredoc>");
		str = get_next_line(STDIN_FILENO);
		tmp = ft_strjoin(argv[2], "\n");
		if (ft_strncmp(str, tmp, ft_strlen(str)) == 0)
		{
			free(tmp);
			free(str);
			break ;
		}
		free(tmp);
		write(var->fd[0][1], str, strlen(str));
		free(str);
	}
	close(var->fd[0][1]);
	exit(EXIT_SUCCESS);
}

void	here_doc(t_var *var, char *argv[])
{
	var->pids[var->i - 2] = fork();
	if (var->pids[var->i - 2] == 0)
		read_stdin(var, argv);
	else
	{
		waitpid(var->pids[var->i - 2], &var->last_exit_status, 0);
		var->i++;
	}
}
