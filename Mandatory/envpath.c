/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envpath.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 19:09:27 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/12 07:41:30 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		path[i] = ft_strjoin(path[i], "/");
		free(backup);
		i++;
	}
	return (path);
}
