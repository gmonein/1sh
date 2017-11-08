
#include "minishell.h"

void	pwd(t_list *envp, char **args)
{
	t_envnode	*res;

	if (*args == NULL)
	{
		res = get_env_node("PWD", envp);
		if (res)
		{
			write (1, res->info, ft_strlen(res->info));
			write (1, "\n", 1);
		}
	}
}