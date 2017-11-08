#include "minishell.h"

void	sh_unsetenv(t_list *envp, char **args)
{
	t_list		*res;
	t_list		*past;
	int			find;

	find = 0;
	past = envp;
	while ((envp = envp->next))
	{
		if (!ft_strcmp(args[0], ((t_envnode *)envp->content)->name))
		{
			find = 1;
			break;
		}
		past = past->next;
	}
	if (find)
	{
		ft_strdel(&((t_envnode *)envp->content)->name);
		ft_strdel(&((t_envnode *)envp->content)->info);
		ft_memdel(&envp->content);
		past->next = envp->next;
		ft_memdel((void **)&envp);
	}
}