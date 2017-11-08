/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 17:00:59 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/08 17:11:43 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	move_backward(size_t count)
{
	size_t		i;

	i = 0;
	while (i < count)
	{
		tputs("\033[1D", 1, ft_iputchar);
		i++;
	}
}

void	move_forward(size_t count)
{
	size_t		i;

	i = 0;
	while (i < count)
	{
		tputs("\033[1C", 1, ft_iputchar);
		i++;
	}
}

void		move_to_upper_line(t_strbuf *line)
{
	size_t		i;

	tputs("\033[1A", 1, ft_iputchar);
	tputs(tgetstr("cr", NULL), 1, ft_iputchar);
	move_forward(get_prompt_len(line));
	i = line->i - 2;
	while (i != -1 && line->str[i] != '\n')
	{
		move_forward(1);
		i--;
	}
}

void		move_to_lower_line(t_strbuf *line)
{
	tputs("\033[1B", 1, ft_iputchar);
	tputs(tgetstr("cr", NULL), 1, ft_iputchar);
	move_forward(get_prompt_len(line));
}
