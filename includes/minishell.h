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
