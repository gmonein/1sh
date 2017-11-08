#include "minishell.h"

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

	tputs(tgetstr("im", NULL), 1, ft_iputchar);
	back_slash = (c == '\\' && !back_slash ? 1 : 0);
	if (c == '\"' && back_slash == 0)
		in_cote ^= 1;
	if (c == '\n')
	{
		if (back_slash || in_cote)
			tputs(tgetstr("ce", NULL), 1, ft_iputchar);
		ft_putchar(c);
		if (!in_cote && !back_slash)
		{
			tputs(tgetstr("ei", NULL), 1, ft_iputchar);
			return (1);
		}
		ft_putstr(COTE_PROMPT);
		ft_putstrto(&line->str[line->i], '\n');
//		tputs(tgetstr("kE", NULL), 1, ft_iputchar);
		move_backward(ft_strlento(&line->str[line->i], '\n'));
	}
	else
		ft_putchar(c);
	tputs(tgetstr("ei", NULL), 1, ft_iputchar);
	return (0);
}

