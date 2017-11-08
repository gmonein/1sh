#include "minishell.h"

void	del_dtab_first_node(char **str)
{
	int		i;

	if (!str)
		return ;
	ft_strdel(&(str[0]));
	i = 0;
	while (str[++i])
		str[i - 1] = str[i];
	str[i - 1] = NULL;
}

char	*purify_path(char *path)
{
	char	**splited;
	char	*res;
	int		i;

	splited = ft_strsplit(path, '/');
	i = 0;
	while (splited[i])
	{
		if (!ft_strcmp(splited[i], "."))
			del_dtab_first_node(&splited[i]);
		else if (!ft_strcmp(splited[i], ".."))
		{
			i -= (i != 0 ? 1 : 0);
			del_dtab_first_node(&splited[i]);
			del_dtab_first_node(&splited[i]);
		}
		else
			i++;
	}
	if (!splited[0])
		return (ft_strdup("/"));
	res = ft_strjoinwithchar(splited, '/');
	ft_deldoubletab((void ***)&splited);
	return (res);
}

char	*get_cd_path(char *path)
{
	DIR		*pdirent;
	char	*res;

	pdirent = opendir(path);
	if (pdirent)
	{
		closedir(pdirent);
		return (purify_path(path));
	}
	else
		ft_multiputstr_fd((char *[6])
				{"cd: ", strerror(errno), ": ", path, "\n", NULL}, 2);
	return (NULL);
}

char	*remplace_first_occurence(char *str, char *remove, char *new)
{
	char	*res;
	char	*to_remplace;
	size_t	offset;

	to_remplace = ft_strchrstr(str, remove);
	if (!to_remplace)
	{
		ft_putstr_fd("cd: string not in pwd: ", 2);
		ft_putendl_fd(remove, 2);
		return (NULL);
	}
	res = (char *)malloc(sizeof(char) *
	((size_t)(to_remplace - str)
	 + ft_strlen(new)
	 + ft_strlen((char *)((size_t)(to_remplace + ft_strlen(to_remplace))))
	));
	offset = 0;
	ft_strncpy(res + offset, str, (size_t)(to_remplace - str));
	offset += (size_t)(to_remplace - str);
	ft_strcpy(res + offset, new);
	offset += ft_strlen(new);
	ft_strcpy(res + offset, to_remplace + ft_strlen(to_remplace));
	return (res);
}

void	cd(t_list *envp, char **args)
{
	t_envnode		*pwd;
	char			*res;
	char			*new_line;

	pwd = get_env_node("PWD", envp);
	if (!pwd)
		return ;
	new_line = NULL;
	if (!args[0])
	{
		ft_strdel(&pwd->info);
		res = get_env_node("HOME", envp)->info;
		if (!res)
			res = "/";
		pwd->info = ft_strdup(res);
	}
	else if (args[0] && !args[1])
		new_line = ft_multijoin((char *[4]){pwd->info, "/", args[0], NULL});
	else if (args[0] && args[1] && !args[2])
		new_line = remplace_first_occurence(pwd->info, args[0], args[1]);
	else
		ft_putstr_fd("cd: Too many arguments\n", 2);
	if (args[0] && (!args[1] || !args[2]) && new_line)
	{
		res = get_cd_path(new_line);
		if (res)
		{
			ft_strdel(&pwd->info);
			pwd->info = res;
		}
		ft_strdel(&new_line);
	}
}