/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 17:05:20 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/08 18:20:47 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			insert_to_line(t_strbuf *line, char c)
{
	if (line->str_len + 1 >= line->len)
		line->str = ft_realloc((void *)line->str, &line->len, LINE_BUF);
	ft_rstrcpy(&line->str[line->i + 1], &line->str[line->i]);
	line->str[line->i] = c;
	line->i++;
	line->str_len++;
	line->str[line->str_len] = '\0';
}

static void			insert_line_to_term(t_strbuf *line)
{
	tputs("\033[1B", 1, ft_iputchar);
	tputs(tgetstr("cr", NULL), 1, ft_iputchar);
	tputs(tgetstr("al", NULL), 1, ft_iputchar);
}

int					line_addchar(t_list *envp, t_strbuf *line, char c)
{
	static char		in_cote = 0;
	static char		back_slash = 0;

	insert_to_line(line, c);
	tputs(tgetstr("im", NULL), 1, ft_iputchar);
	back_slash = (c == '\\' && !back_slash ? 1 : 0);
	if (c == '\"' && back_slash == 0)
		in_cote ^= 1;
	if (c == '\n')
	{
		if (back_slash || in_cote)
			tputs(tgetstr("ce", NULL), 1, ft_iputchar);
		ft_putchar('\n');
		//insert_line_to_term(line);
		if (!in_cote && !back_slash)
		{
			tputs(tgetstr("ei", NULL), 1, ft_iputchar);
			return (1);
		}
		ft_putstr(COTE_PROMPT);
		move_backward(ft_putstrto(&line->str[line->i], '\n'));
	}
	else
		ft_putchar(c);
	tputs(tgetstr("ei", NULL), 1, ft_iputchar);
	return (0);
}
