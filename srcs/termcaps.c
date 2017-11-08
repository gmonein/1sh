#include "minishell.h"

void		get_delete_character(t_strbuf *line, char *input)
{
	char		*buf;

	if (*((long *)input) == 0x7F && line->i != 0)
	{
		ft_strcpy(&line->str[line->i - 1], &line->str[line->i]);
		line->i--;
		line->str_len--;
		buf = tgetstr("dc", NULL);
		tputs("\033[1D", 1, ft_iputchar);
		tputs(buf, 1, ft_iputchar);
	}
}

void	move_backward(size_t count)
{
	while (count != -1)
	{
		tputs("\033[1D", 1, ft_iputchar);
		count--;
	}
}

void	move_forward(size_t count)
{
	while (count != -1)
	{
		tputs("\033[1C", 1, ft_iputchar);
		count--;
	}
}

void		move_to_upper_line(t_strbuf *line)
{
	size_t		i;

	tputs("\033[1A", 1, ft_iputchar);
	move_forward(ft_strlen(COTE_PROMPT));
	i = line->i;
	while (line->str[i] != '\n' && i != -1)
	{
		move_forward(1);
		i--;
	}
}

void		get_arrow(t_strbuf *line, char *input)
{
	if (input[1] == '[')
	{
		if (input[2] == 'D' && line->i != 0)
		{
			if (line->str[line->i - 1] != '\n')
				tputs(input, 1, ft_iputchar);
			else
				move_to_upper_line(line);
			line->i--;
		}
		else if (input[2] == 'C' && line->str[line->i]
								&& line->str[line->i] != '\n')
		{
			line->i++;
			ft_putstr(input);
		}
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

int		line_addchar(t_list *envp, t_strbuf *line, char c)
{
	static char		in_cote = 0;
	static char		back_slash = 0;

	if (line->str_len + 1 >= line->len)
		line->str = ft_realloc((void *)line->str, &line->len, LINE_BUF);
	ft_rstrcpy(&line->str[line->i + 1], &line->str[line->i]);
	line->str[line->i] = c;
	line->i++;
	line->str_len++;
	line->str[line->str_len] = '\0';

	back_slash = (c == '\\' && !back_slash ? 1 : 0);
	if (c == '\"' && back_slash == 0)
		in_cote ^= 1;
	if (c == '\n')
	{
		if (back_slash || in_cote)
			tputs(tgetstr("ce", NULL), 1, ft_iputchar);
		ft_putchar(c);
		if (!in_cote && !back_slash)
			return (1);
		ft_putstr(back_slash ? BACKSLASH_PROMPT : COTE_PROMPT);
		ft_putstrto(&line->str[line->i], '\n');
//		tputs(tgetstr("kE", NULL), 1, ft_iputchar);
		move_backward(ft_strlento(&line->str[line->i], '\n') - 1);
	}
	else
		ft_putchar(c);
	return (0);
}