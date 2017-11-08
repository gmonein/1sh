#include "minishell.h"

static int     run_cmd(char *path, char **args, char **newenv)
{
        pid_t   pid;
        int             cmd_status;

        cmd_status = 0;
        //signal(SIGINT, handle_signal);
		pid = fork();
		if (pid == 0)
		{
			printf("%s\n", path);
            execve(path, args, newenv);
		}
		if (pid > 0)
            wait(&cmd_status);
        ft_memdel((void **)&path);
        return (1);
}

static int      is_executable(char *bin_path, struct stat f, char **ag, char **env)
{
    if (f.st_mode & S_IFREG)
	{
        if (f.st_mode & S_IXUSR)
            return (run_cmd(bin_path, ag, env));
        else
        {
            ft_putstr("minishell: permission denied: ");
            ft_putendl(bin_path);
        }
        free(bin_path);
        return (1);
    }
    free(bin_path);
    return (0);
}

int		nb_node(t_list *envp)
{
	int i;

	i = 0;
	while (envp && envp->next)
	{
		envp = envp->next;
		i++;
	}
	return (i);
}

char	**list_to_2dtab(t_list *envp)
{
	char **envi;
	t_list *tmp;
	t_envnode *content;
	int i;

	i = 0;
	tmp = envp;
	envi = (char **)malloc(sizeof(char *) * (nb_node(envp) + 1));
	while(envp && envp->next)
	{
		content = envp->content;
		if (content)
			envi[i++] = ft_multijoin((char *[4]){content->name, "=", content->info, NULL});
		envp = envp->next;
	}
	envi[i] = NULL;
	envp = tmp;
	return (envi);
}

static int      check_if_is_binary(char **line, t_list *envp)
{
        int                             i;
        char                    *bin_path;
        char                    **path;
        struct stat             f;
		char                    *tmp;
		char					**envi;

        path = NULL;
        tmp = NULL;
		bin_path = NULL;
		envi = list_to_2dtab(envp);
        if (lstat(line[0], &f) != -1)
                return (is_executable((bin_path = ft_strdup(line[0])), f, line, envi));
        path = ft_strsplit(get_env_node("PATH", envp)->info , ':');
		while (path && path[++i] && (bin_path = ft_multijoin((char *[4]){path[i],"/", line[0], NULL})))
        {
            if (lstat(bin_path, &f) != -1)
            {
                ft_deldoubletab((void ***)&path);
                //if (line[1] && line[1][0] == '~')
                //      line[1] = ft_strdup(ft_get_elem(envi, "HOME"));
                return (is_executable(bin_path, f, line, envi));
            }
            ft_memdel((void **)&bin_path);
        }
        ft_deldoubletab((void ***)&path);
        return (0);
}

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
		//else if (!ft_strncmp(splited[0], "./", 2))
		else if (!check_if_is_binary(splited, envp))
			ft_multiputstr_fd((char *[4])
					{"1sh: command not found: ", splited[0], "\n", NULL}, 2);
	}
	free(new_line);
	ft_deldoubletab((void ***)&splited);
}