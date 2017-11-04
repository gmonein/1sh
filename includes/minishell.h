/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 07:44:16 by gmonein           #+#    #+#             */
/*   Updated: 2017/10/31 16:12:47 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <fcntl.h>
# include <unistd.h>
# include "libft.h"
# include <dirent.h>

#define PROMPT "$>"

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
