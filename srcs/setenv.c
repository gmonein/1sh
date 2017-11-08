#include "minishell.h"

void	sh_setenv(t_list *envp, char **args)
{
	char		*to_find;
	int			find;
	t_list		*begin;
	t_envnode	buf;

	find = 0;
	to_find = ft_strchr(args[0], '=');
	if (to_find)
		to_find = ft_strndup(args[0], (size_t)(to_find - args[0]));
	else
		to_find = ft_strdup(args[0]);
	begin = envp;
	while ((envp = envp->next))
		if (!ft_strcmp(to_find, ((t_envnode *)envp->content)->name))
		{
			find = 1;
			break;
		}
	if (find)
	{
		ft_strdel(&to_find);
		ft_strdel(&((t_envnode *)envp->content)->info);
		if ((to_find = ft_strchr(args[0], '=')))
			((t_envnode *)envp->content)->info = ft_strdup(to_find + 1);
		else
			((t_envnode *)envp->content)->info = ft_strdup(args[1]);
	}
	else
	{
		while (begin->next)
			begin = begin->next;
		buf.name = ft_strdup(to_find);
		ft_strdel(&to_find);
		if ((to_find = ft_strchr(args[0], '=')))
			buf.info = ft_strdup(to_find + 1);
		else
			buf.info = ft_strdup(args[1]);
		begin->next = ft_lstnew(&buf, sizeof(buf));
	}
}