/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 07:44:16 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/05 18:35:24 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <fcntl.h>
# include <unistd.h>
# include "libft.h"
# include <dirent.h>
# include <wchar.h>
# include <locale.h>
# include <limits.h>

#define PROMPT				"$>"
#define COTE_PROMPT			"dquote>"
#define BACKSLASH_PROMPT	">"
#define LINE_BUF	12

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
}					t_strbuf;
