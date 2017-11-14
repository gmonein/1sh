/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamerlin <jamerlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 18:07:22 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/14 19:55:09 by jamerlin         ###   ########.fr       */
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
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	total_size(char **str)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i])
	{
		j += ft_strlen(str[i]);
		i++;
	}
	return (j);
}

char *d2tab_to_d1_tab(char **args)
{
	char *line;
	int i;

	i = 0;
	if (!(line = (char *)malloc(sizeof(char) * (total_size(args) + nb_elem(args)))))
		return (NULL);
	while (args[i])
	{
		if (i == 0)
			line = ft_strdup(args[i]);
		else
			line = ft_strjoin(ft_strjoin(line, " "), args[i]);
		i++;
	}
	line[total_size(args) + nb_elem(args) - 1] = 0;
	return (line);
}

t_list  *dup_list(t_list *envp)
{
	t_list          *begin;
	t_list			*tmp;
	t_list          *tmp2;
	t_envnode       node;

	begin = ft_lstnew(NULL, 0);
	tmp = begin;
	tmp2 = envp;
	while ((envp = envp->next))
	{
		node.name = ft_strdup(((t_envnode *)envp->content)->name);
		node.info = ft_strdup(((t_envnode *)envp->content)->info);
		tmp->next = ft_lstnew(&node, sizeof(node));
		tmp = tmp->next;
	}
	envp = tmp2;
	return (begin);
}

void		env_arg(char **args, t_list *envp)
{
	char *tmp;
	int i;

	i = 0;
	if (nb_elem(args) == 0)
		return ;
	else
	{
		while (args[i] && equal_is(args[i]))
		{
			sh_setenv(envp, &args[i]);
			i++;
		}
		if (args[i])
			tmp = d2tab_to_d1_tab(&args[i]);
		else
			ft_lstiter(envp, print_envline);
		launch_cmd(envp, tmp);
	}
}

void		env(t_list *envp, char **args)
{
	t_list		*env_cpy;
	t_envnode	node;

	env_cpy = ft_lstnew(NULL, 0);
	node.name = NULL;
	node.info = NULL;
	env_cpy->next = ft_lstnew(&node, sizeof(node));
	if (*args == NULL)
		ft_lstiter(envp, print_envline);
	else
	{
		if (!ft_strcmp(*args, "-i"))
			env_arg(&args[1], env_cpy);
		else
			env_arg(args, (env_cpy = dup_list(envp)));
	}
}
