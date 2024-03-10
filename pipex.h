/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:55:04 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/10 21:13:47 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_var{
	char	**argvcmd;
	char	*path;
	char	**fullpath;
	int		*pids;
	int		(*fd)[2];
	int		io[2];
	int		i;
}	t_var;

char	**get_env_path(char *envp[]);
char	**cmdsplit(char *cmd);

#endif