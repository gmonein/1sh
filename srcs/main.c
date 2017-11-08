/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmonein <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 16:06:47 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/08 15:11:47 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int		read_loop(t_list *envp)
{
	t_strbuf	line;
	char		in_cote;
	size_t		off_line;
	char		key;

	line.str = (char *)malloc(sizeof(char) * LINE_BUF);
	line.i = 0;
	line.str_len = 0;
	line.len = LINE_BUF;
	off_line = 0;
	ft_bzero(line.str, sizeof(char) * line.len);
	in_cote = 0;
	while (42)
	{
		key = get_key(&line);
		if (key)
		{
			if (line_addchar(envp, &line, key))
			{
				line.str[line.i - 1] = '\0';
				launch_cmd(envp, line.str);
				ft_putstr(PROMPT);
				ft_bzero(line.str, sizeof(char) * line.len);
				line.str_len = 0;
				line.i = 0;
			}
		}
	}
	return (0);
}

int		main(int argc, char **argv, char **envp)
{
	int				readed;
	t_list			*env;
	struct termios	term;
	t_envnode		*shell_name;

	tputs(tgetstr("im", NULL), 1, ft_iputchar);
	if (!(env = dup_env(envp)))
		return (EXIT_FAILURE);
	shell_name = get_env_node("TERM", env);
	if (!shell_name)
		return (EXIT_FAILURE);
//	ft_strdel(&shell_name->info);
//	shell_name->info = ft_strdup("minishell");
	if (tcgetattr(0, &term) == -1)
		return (EXIT_FAILURE);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (EXIT_FAILURE);
	tgetent(0, shell_name->info);
	write (1, PROMPT, 2);
	read_loop(env);
}
