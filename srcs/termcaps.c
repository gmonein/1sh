/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 16:57:31 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/08 17:05:40 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t		get_prompt_len(t_strbuf *line)
{
	size_t		i;

	i = line->i - 1;
	while (--i != -1 && line->str[i] && line->str[i] != '\n')
		;
	if (i != -1)
		return (ft_strlen(COTE_PROMPT));
	else
		return (ft_strlen(PROMPT));
}

void		get_arrow(t_strbuf *line, char *input)
{
	if (input[1] == '[')
	{
		if (input[2] == 'D' && line->i != 0)
		{
			if (line->str[line->i - 1] != '\n')
				move_backward(1);
			else
				move_to_upper_line(line);
			line->i--;
		}
		else if (input[2] == 'C' && line->str[line->i])
		{
			if (line->str[line->i] != '\n')
				move_forward(1);
			else
				move_to_lower_line(line);
			line->i++;
		}
	}
}

void		get_delete_character(t_strbuf *line, char *input)
{
	char		*buf;

	if (*((long *)input) == 0x7F && line->i != 0)
	{
		if (line->str[line->i - 1] != '\n')
		{
			ft_putstr("\033[1D");
			buf = tgetstr("dc", NULL);
			tputs(buf, 1, ft_iputchar);
		}
		else
		{
			tputs(tgetstr("dl", NULL), 1, ft_iputchar);
			move_to_upper_line(line);
			move_backward(ft_putstrto(&line->str[line->i], '\n'));
		}
		ft_strcpy(&line->str[line->i - 1], &line->str[line->i]);
		line->i--;
		line->str_len--;
	}
}

char		get_key(t_strbuf *line)
{
	char		*input;
	int			len;

	input = ft_getchar(&len);
	if (len == 1 && is_printable(*input))
		return (*input);
	get_delete_character(line, input);
	get_arrow(line, input);
	return (0);
}
