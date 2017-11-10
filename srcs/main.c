/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamerlin <jamerlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 16:06:47 by gmonein           #+#    #+#             */
/*   Updated: 2017/11/10 15:14:35 by gmonein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int signl)
{
	if (signl == SIGINT)
	{
			ft_putstr("\n");
			signal(SIGINT, handle_signal);
	}
}

void	handle_signal2(int sinl)
{
		char *tmp;
		int i;
		
		ft_putstr("\n");
		tmp = ft_strdup(getcwd(NULL,0));
		i = ft_strlen(tmp);
		ft_putstr("\033[33m");
		if (i == 1)
			ft_putstr(tmp);
		else
			while (i >= 0 && tmp[i] != '/')
				i--;
		ft_putstr(&tmp[i + 1]);
		free(tmp);
		ft_putstr(" \033[36m");
		ft_putstr(PROMPT);
		ft_putstr(" \033[0m");
}

void	print_current(t_list *envp)
{
	char *tmp;
	int i;

	tmp = ft_strdup(get_env_node("PWD", envp)->info);
	i = ft_strlen(tmp);
	ft_putstr("\033[33m");
	if (i == 1)
		ft_putstr(tmp);
	else
		while (i >= 0 && tmp[i] != '/')
			i--;
	ft_putstr(&tmp[i + 1]);
	free(tmp);
	ft_putstr(" \033[36m");
	ft_putstr(PROMPT);
	ft_putstr(" \033[0m");
}

int		read_loop(t_list *envp)
{
	t_strbuf	line;
	char		key;

	line = (t_strbuf){(char *)malloc(sizeof(char) * LINE_BUF), LINE_BUF, 0, 0};
	ft_bzero(line.str, sizeof(char) * line.len);
	while (42)
	{
		signal(SIGINT, SIG_IGN);
		if (signal(SIGINT, handle_signal2) == SIG_ERR)
			return (0);
		key = get_key(&line);
		if (key)
			if (line_addchar(envp, &line, key))
			{
				line.str[line.i - 1] = '\0';
				launch_cmd(envp, line.str);
				print_current(envp);
				ft_bzero(line.str, sizeof(char) * line.len);
				line.str_len = 0;
				line.i = 0;
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
	if (tcgetattr(0, &term) == -1)
		return (EXIT_FAILURE);
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (EXIT_FAILURE);
	tgetent(0, shell_name->info);
	print_current(env);
	read_loop(env);
}
