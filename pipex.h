/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:55:04 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/07 19:10:14 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_var{
	char	**argvcmd;
	char	*path;
	char	**fullpath;
	int		pip;
	int		io[2];
	int		i;
	int		pid;
	int		(*fd)[2];
}	t_var;

char	**get_env_path(char *envp[]);
char	**cmdsplait(char *cmd);

#endif