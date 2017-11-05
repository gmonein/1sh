/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 16:06:47 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/05 19:28:35 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include <stdio.h>

t_envnode	*get_env_node(char *str, t_list *envp)
{
	while ((envp = envp->next))
		if (!ft_strcmp(str, ((t_envnode *)envp->content)->name))
			return (envp->content);
	return (NULL);
}

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

void	print_envline(t_list *lst)
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

char	*ft_strjoinwithchar(char **to_join, char c)
{
	int		i;
	size_t	len;
	char	*res;

	len = 0;
	i = -1;
	while (to_join[++i])
		len += ft_strlen(to_join[i]) + (to_join[i + 1] ? 1 : 0);
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = -1;
	len = 0;
	while (to_join[++i])
	{
		ft_strcpy(res + len, to_join[i]);
		len += ft_strlen(to_join[i]);
		if (to_join[i + 1])
		{
			ft_strcpy(res + len, &c);
			len += 1;
		}
	}
	res[len] = '\0';
	return (res);
}

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

	pdirent = opendir(path);
	if (pdirent)
	{
		closedir(pdirent);
		return (purify_path(path));
	}
	else
		ft_multiputstr_fd((char *[4])
					{"cd: no such file or directory: ", path, "\n", NULL}, 2);
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
	ft_strcpy(res + offset, to_remplace + ft_strlen(new));
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
		pwd->info = ft_strdup("/");
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

void	env(t_list *envp, char **args)
{
	if (*args == NULL)
		ft_lstiter(envp, print_envline);
}

typedef struct		s_echo_args
{
	char			n:1;
	char			e:1;
}					t_echo_args;

int		get_echo_args(char **args, t_echo_args *res)
{
	int				i;
	int				j;
	int				is_arg;
	t_echo_args		buf;

	i = -1;
	buf = (t_echo_args){0, 0};
	while (args[++i] && (is_arg || i == 0))
	{
		is_arg = 0;
		buf = (t_echo_args){0, 0};
		if (!(j = 0) && args[i][0] == '-')
			while (args[i][++j] && (args[i][j] == 'e' || args[i][j] == 'n'))
			{
				buf.e |= (args[i][j] == 'e' ? 1 : 0);
				buf.n |= (args[i][j] == 'n' ? 1 : 0);
			}
		if (!args[i][j] && j != 1)
		{
			res->e |= buf.e;
			res->n |= buf.n;
			is_arg = 1;
		}
	}
	return (i - !is_arg);
}

char	*search_and_remplace(char *str, char *remove, char *new)
{
	char		*res;
	int			i;
	int			j;

	j = 0;
	i = -1;
	while (str[++i])
		if (!ft_strncmp(&str[i], remove, ft_strlen(remove)))
			j++;
	res = (char *)malloc(sizeof(char) *
			(i - j * ft_strlen(remove) + j * ft_strlen(new) + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (!ft_strncmp(&str[i], remove, ft_strlen(remove)))
		{
			ft_strcpy(&res[j], new);
			j += ft_strlen(new);
			i += ft_strlen(remove);
		}
		else
		{
			res[j] = str[i];
			j++;
			i++;
		}
	}
	res[j] = '\0';
	return (res);
}

void	remplace_echo_char(char **str)
{
	char	*res;

	res = search_and_remplace(*str, "\\n", "\n");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\a", "\a");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\b", "\b");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\f", "\f");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\r", "\r");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\t", "\t");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\v", "\v");
	free(*str);
	*str = res;
}

void	echo(t_list *envp, char **args)
{
	int			bracket;
	int			i;
	int			j;
	t_echo_args	echo_args;

	bracket = 0;
	j = get_echo_args(args, &echo_args);
	if (j != -1)
		while (args[j])
		{
			if (echo_args.e)
				remplace_echo_char(&args[j]);
			i = -1;
			while (args[j][++i])
				write(1, &args[j][i], (args[j][i] == '\"' ? 0 : 1));
			j++;
			if (args[j])
				ft_putchar(' ');
		}
	if (!echo_args.n)
		ft_putchar('\n');
}

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
			echo(envp, &splited[1]);
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

char	ft_getchar(void)
{
	char		buf;

	read(0, &buf, 1);
	return (buf);
}

void	*ft_realloc(void *str, size_t *len, size_t add)
{
	char	*res;

	res = (void *)malloc(*len + add);
	if (!res)
		return (NULL);
	ft_memcpy(res, str, *len);
	*len += add;
	free(str);
	return (res);
}

int		read_loop(t_list *envp)
{
	t_strbuf	line;
	size_t		i;
	char		in_cote;
	size_t		off_line;

	line.str = (char *)malloc(sizeof(char) * LINE_BUF);
	line.len = LINE_BUF;
	off_line = 0;
	ft_bzero(line.str, sizeof(char) * line.len);
	i = 0;
	in_cote = 0;
	while (42)
	{
		if ((line.str[i] = ft_getchar()))
		{
			if (line.str[i] == '\"' && (i == 0 || line.str[i - 1] != '\\'))
				in_cote ^= 1;
			if (line.str[i] == '\n')
			{
				if (in_cote)
					ft_putstr(COTE_PROMPT);
				else if (i == 0 || line.str[i - 1] != '\\')
				{
					line.str[i] = '\0';
					launch_cmd(envp, &line.str[off_line]);
					off_line = i + 1;
					ft_putstr(PROMPT);
				}
				else
				{
					i--;
					line.str[i] = '\n';
					ft_putstr(BACKSLASH_PROMPT);
				}
			}
			i++;
			if (i == line.len)
				line.str = ft_realloc((void *)line.str, &line.len, LINE_BUF);
		}
	}
	return (0);
}

# include <stdio.h>

t_list	*dup_env(char **envp)
{
	t_list		*begin;
	t_list		*tmp;
	t_envnode	node;

	begin = ft_lstnew(NULL, 0);
	tmp = begin;
	while (*envp)
	{
		node.name = ft_strduptochar(*envp, '=');
		node.info = ft_strdup(ft_strchr(*envp, '=') + 1);
		tmp->next = ft_lstnew(&node, sizeof(node));
		tmp = tmp->next;
		envp++;
	}
	return (begin);
}

int		main(int argc, char **argv, char **envp)
{
	int			readed;
	t_list		*env;
	
	if (!(env = dup_env(envp)))
		return (EXIT_FAILURE);
	write (1, PROMPT, 2);
	read_loop(env);
}
