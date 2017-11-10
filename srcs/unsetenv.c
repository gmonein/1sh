#include "minishell.h"

void	sh_unsetenv(t_list *envp, char **args)
{
	t_list		*res;
	t_list		*past;

	past = envp;
	if (args && args[0])
		while ((envp = envp->next))
		{
			if (envp->content)
			{
				if (!ft_strcmp(args[0], ((t_envnode *)envp->content)->name))
				{
					ft_strdel(&((t_envnode *)envp->content)->name);
					ft_strdel(&((t_envnode *)envp->content)->info);
					ft_memdel(&envp->content);
					past->next = envp->next;
					ft_memdel((void **)&envp);
					return ;
				}
			}
			past = past->next;
		}
}
