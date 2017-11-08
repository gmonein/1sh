#include "minishell.h"

void	launch_cmd(t_list *envp, char *line)
{
	char		**splited;
	char		*new_line;

	new_line = ft_strdup(line);
	splited = ft_strsplit(new_line, ' ');
	if (splited && splited[0])
	{
		if (!ft_strcmp(splited[0], "pwd"))
			pwd(envp, &splited[1]);
		else if (!ft_strcmp(splited[0], "env"))
			env(envp, &splited[1]);
		else if (!ft_strcmp(splited[0], "cd"))
			cd(envp, &splited[1]);
		else if (!ft_strcmp(splited[0], "exit"))
			exit(0);
		else if (!ft_strcmp(splited[0], "echo"))
			sh_echo(envp, &splited[1]);
		else if (!ft_strcmp(splited[0], "unsetenv"))
			sh_unsetenv(envp, &splited[1]);
		else if (!ft_strcmp(splited[0], "setenv"))
			sh_setenv(envp, &splited[1]);
		else
			ft_multiputstr_fd((char *[4])
					{"1sh: command not found: ", splited[0], "\n", NULL}, 2);
	}
	free(new_line);
	ft_deldoubletab((void ***)&splited);
}