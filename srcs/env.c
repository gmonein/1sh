/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 18:07:22 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/08 18:07:47 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envnode	*get_env_node(char *str, t_list *envp)
{
	while ((envp = envp->next))
		if (!ft_strcmp(str, ((t_envnode *)envp->content)->name))
			return (envp->content);
	return (NULL);
}

void		print_envline(t_list *lst)
{
	t_envnode	*node;

	node = lst->content;
	if (node)
	{
		ft_putstr(node->name);
		ft_putstr("=");
		ft_putstr(node->info);
		ft_putstr("\n");
	}
}

void		env(t_list *envp, char **args)
{
	if (*args == NULL)
		ft_lstiter(envp, print_envline);
}
