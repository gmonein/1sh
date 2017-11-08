#include "minishell.h"

int		get_echo_args(char **args, t_echo_args *res)
{
	int				i;
	int				j;
	int				is_arg;
	t_echo_args		buf;

	i = -1;
	buf = (t_echo_args){0, 0};
	while (args[++i] && (is_arg || i == 0))
	{
		is_arg = 0;
		buf = (t_echo_args){0, 0};
		if (!(j = 0) && args[i][0] == '-')
			while (args[i][++j] && (args[i][j] == 'e' || args[i][j] == 'n'))
			{
				buf.e |= (args[i][j] == 'e' ? 1 : 0);
				buf.n |= (args[i][j] == 'n' ? 1 : 0);
			}
		if (!args[i][j] && j != 1)
		{
			res->e |= buf.e;
			res->n |= buf.n;
			is_arg = 1;
		}
	}
	return (i - !is_arg);
}

void	remplace_echo_char(char **str)
{
	char	*res;

	res = search_and_remplace(*str, "\\n", "\n");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\a", "\a");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\b", "\b");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\f", "\f");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\r", "\r");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\t", "\t");
	free(*str);
	*str = res;
	res = search_and_remplace(*str, "\\v", "\v");
	free(*str);
	*str = res;
}

void	sh_echo(t_list *envp, char **args)
{
	int			bracket;
	int			i;
	int			j;
	t_echo_args	echo_args;

	bracket = 0;
	j = get_echo_args(args, &echo_args);
	if (j != -1)
		while (args[j])
		{
			if (echo_args.e)
				remplace_echo_char(&args[j]);
			i = -1;
			while (args[j][++i])
				write(1, &args[j][i], (args[j][i] == '\"' ? 0 : 1));
			j++;
			if (args[j])
				ft_putchar(' ');
		}
	if (!echo_args.n)
		ft_putchar('\n');
}