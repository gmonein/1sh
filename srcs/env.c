/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 18:07:22 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/12 19:48:56 by jamerlin         ###   ########.fr       */
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

int			nb_elem(char **str)
{
	int i;
	
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int			equal_is(char *str)
{
	int i;

	i = 0;
	while (*str)
	{
		i = (*str == '=') ? 1 : 0;
		str++;
	}
	return (i);
}

void		env_i(char **args)
{
	t_list *env_cpy;
	int i;

	i = 0;
	env_cpy = (t_list *)malloc(sizeof(t_list));
	if (nb_elem(args) == 1)
		ft_putchar('\n');
	else
	{
		while (args[i])
		{
			if (equal_is(args[i]))
			{
				sh_setenv(env_cpy, &args[i]);
			}//else if (launch_cmd(env_cpy,
			i++;
		}
		ft_lstiter(env_cpy, print_envline);
	}
}

void		env(t_list *envp, char **args)
{
	if (*args == NULL)
		ft_lstiter(envp, print_envline);
	else
	{
		if (!ft_strcmp(*args, "-i"))
			env_i(args);
		/*else
			env_with_exec(args);*/
	}
}
