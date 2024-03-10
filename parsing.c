/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 16:12:56 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/03/10 21:13:32 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Extra-Library/libft.h"
#include "pipex.h"

static int	issep(char a, char b)
{
	return (a == '\\' && b == '\'');
}

static void	searchandreplaice(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != '\0')
	{
		if (issep(ptr[i], ptr[i + 1]))
		{
			ft_memmove(&ptr[i], &ptr[i + 1], ft_strlen(ptr) - i);
			return ;
		}
		i++;
	}
}

static void	handlquote(char *str, int *i, int *start, t_list **list)
{
	(*i)++;
	(*start) = *i; 
	while (str[*i] && (str[*i] != 39 || issep(str[*i], str[*i + 1])))
	{
		if (issep(str[*i], str[*i + 1]))
			searchandreplaice(str + *i);
		(*i)++;
	}
	ft_lstadd_back(list, ft_lstnew(ft_substr(str, *start, *i - *start)));
	*start = *i + 1;
}

static t_list	*extrasplit(char *str)
{
	t_list	*list;
	int		i;
	int		start;

	i = 0;
	start = 0;
	list = NULL;
	while (str[i])
	{
		while (str[i] && issep(str[i], str[i + 1]))
			searchandreplaice(str + i++);
		if (str[i] == ' ' || str[i + 1] == '\0')
		{
			if (str[i + 1] == '\0')
				i++;
			ft_lstadd_back(&list, ft_lstnew(ft_substr(str, start, i - start)));
			if (str[i] == '\0')
				return (list);
			start = i + 1;
		}
		else if (str[i] && str[i] == 39)
			handlquote(str, &i, &start, &list);
		i++;
	}
	return (list);
}


char	**cmdsplit(char *cmd)
{
	t_list	*list = NULL;
	t_list	*tmp;
	char	*str;
	char	**arr;
	int		i;

	list = NULL;
	str = ft_strdup(cmd);
	list = extrasplit(cmd);
	free(str);
	tmp = list;
	int siz = ft_lstsize(tmp);
	arr = malloc((ft_lstsize(tmp) + 1) * sizeof(char *));
	i = 0;
	while (tmp)
	{
		arr[i] = ft_strdup(tmp->content);
		tmp = (tmp)->next;
		i++;
	}
	arr[i] = NULL;
	ft_lstclear(&list,free);
	return (arr);
}

// int main(int argc, char *argv[])
// {
// 	char **arr;
// 	arr = cmdsplit(argv[1]);
// 	char **arr2;
// 	arr2 = cmdsplit(argv[2]);
// 	char **arr3;
// 	arr3 = cmdsplit(argv[3]);
// 	int i = 0;
// 	while (arr[i])
// 	{
// 		printf("%s\n", arr[i]);
// 		i++;
// 	}

// 	 i = 0;
// 	while (arr2[i])
// 	{
// 		printf("%s\n", arr2[i]);
// 		i++;
// 	}

// 	 i = 0;
// 	while (arr3[i])
// 	{
// 		printf("%s\n", arr3[i]);
// 		i++;
// 	}
// 	// system("leaks a.out");
// }