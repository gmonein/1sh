/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 07:44:16 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/08 14:45:53 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <fcntl.h>
# include <unistd.h>
# include "libft.h"
# include <dirent.h>
# include <wchar.h>
# include <locale.h>
# include <limits.h>
# include <errno.h>
# include <term.h>
# include <termios.h>
# include <termcap.h>
# include <curses.h>
# include <stdio.h>
# include <stdlib.h>

#define PROMPT				"$>"
#define COTE_PROMPT			">"
#define LINE_BUF	12

# define BACK_WHITE			"\033[47m"

# define END_EFFECT			"\033[m"

typedef struct		s_cmd
{
	char			*name;
	void			(*fct)(char **);
}					t_cmd;

typedef struct		s_envnode
{
	char			*name;
	char			*info;
}					t_envnode;

typedef struct		s_argsnode
{
	char			*str;
	size_t			len;
}					t_argsnode;

typedef struct		s_strbuf
{
	char			*str;
	size_t			len;
	size_t			i;
	size_t			str_len;
}					t_strbuf;

typedef struct		s_env
{
	t_strbuf		line;
	size_t			line_i;
	size_t			cursor;
}					t_env;

typedef struct		s_echo_args
{
	char			n:1;
	char			e:1;
}					t_echo_args;

void	pwd(t_list *envp, char **args);
void	sh_unsetenv(t_list *envp, char **args);
void	sh_setenv(t_list *envp, char **args);
void	sh_echo(t_list *envp, char **args);
void	remplace_echo_char(char **str);
int		get_echo_args(char **args, t_echo_args *res);
char	*get_cd_path(char *path);
void	del_dtab_first_node(char **str);
char	*purify_path(char *path);
char	*remplace_first_occurence(char *str, char *remove, char *new);
void	cd(t_list *envp, char **args);
t_envnode	*get_env_node(char *str, t_list *envp);
void	print_envline(t_list *lst);
void	*ft_realloc(void *str, size_t *len, size_t add);
void	env(t_list *envp, char **args);
char	*ft_strjoinwithchar(char **to_join, char c);
char	*search_and_remplace(char *str, char *remove, char *new);
t_list	*dup_env(char **envp);
int		ft_iputchar(int c);
char	*ft_getchar(int *len);
char	is_printable(char c);
void	ft_rstrcpy(char *dest, char *src);
void	ft_putstrto(char *str, char c);
size_t	ft_strlento(char *str, char c);
void	launch_cmd(t_list *envp, char *line);
void	get_delete_character(t_strbuf *line, char *input);
void	move_backward(size_t count);
void	move_forward(size_t count);
void	move_to_upper_line(t_strbuf *line);
void	get_arrow(t_strbuf *line, char *input);
char	get_key(t_strbuf *line);
int		line_addchar(t_list *envp, t_strbuf *line, char c);