/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 14:16:18 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/10 15:10:18 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char		*get_node_name(char **args)
{
	char		*to_find;

	if ((to_find = ft_strchr(args[0], '=')))
		to_find = ft_strndup(args[0], (size_t)(to_find - args[0]));
	else
		to_find = ft_strdup(args[0]);
	return (to_find);
}

static void		set_env_info(t_envnode *node, char **args)
{
	char		*buf;

	ft_strdel(&node->info);
	if ((buf = ft_strchr(args[0], '=')))
		node->info = ft_strdup(buf + 1);
	else if (args[1])
		node->info = ft_strdup(args[1]);
	else
		node->info = NULL;
}

void	sh_setenv(t_list *envp, char **args)
{
	char		*to_find;
	t_envnode	*node;
	t_envnode	buf;

	if (!args || !args[0])
		return ;
	to_find = get_node_name(args);
	node = get_env_node(to_find, envp);
	if (node)
	{
		ft_strdel(&to_find);
		set_env_info(node, args);
	}
	else
	{
		while (envp->next)
			envp = envp->next;
		buf.name = ft_strdup(to_find);
		ft_strdel(&to_find);
		set_env_info(&buf, args);
		envp->next = ft_lstnew(&buf, sizeof(buf));
	}
}
