/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:55:04 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/24 01:34:16 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <stdio.h>
# include <fcntl.h>

typedef struct s_var{
	char	**argvcmd;
	char	*path;
	char	**fullpath;
	int		*pids;
	int		(*fd)[2];
	int		io[2];
	int		last_exit_status;
	int		i;
}	t_var;

int		issep(char a, char b);
void	free_2d_arr(char **arr);
char	**get_env_path(char *envp[]);
char	**cmdsplit(char *cmd);
char	*check_cmd_access(char *cmd, char **fullpath);
void	close_all(t_var *var, int argc);
int		open_pipes(t_var *var, int argc);
int		duping(t_var *var, int argc, char *argv[]);
int		wait_childes(t_var *var, int argc);
#endif