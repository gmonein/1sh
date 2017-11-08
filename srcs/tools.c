#include "minishell.h"

char	*ft_strjoinwithchar(char **to_join, char c)
{
	int		i;
	size_t	len;
	char	*res;

	len = 0;
	i = -1;
	while (to_join[++i])
		len += ft_strlen(to_join[i]) + 1;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = -1;
	len = 0;
	while (to_join[++i])
	{
		ft_strcpy(res + len, &c);
		len += 1;
		ft_strcpy(res + len, to_join[i]);
		len += ft_strlen(to_join[i]);
	}
	res[len] = '\0';
	return (res);
}

char	*search_and_remplace(char *str, char *remove, char *new)
{
	char		*res;
	int			i;
	int			j;

	j = 0;
	i = -1;
	while (str[++i])
		if (!ft_strncmp(&str[i], remove, ft_strlen(remove)))
			j++;
	res = (char *)malloc(sizeof(char) *
			(i - j * ft_strlen(remove) + j * ft_strlen(new) + 1));
	i = 0;
	j = 0;
	while (str[i])
	{
		if (!ft_strncmp(&str[i], remove, ft_strlen(remove)))
		{
			ft_strcpy(&res[j], new);
			j += ft_strlen(new);
			i += ft_strlen(remove);
		}
		else
		{
			res[j] = str[i];
			j++;
			i++;
		}
	}
	res[j] = '\0';
	return (res);
}

t_list	*dup_env(char **envp)
{
	t_list		*begin;
	t_list		*tmp;
	t_envnode	node;

	begin = ft_lstnew(NULL, 0);
	tmp = begin;
	while (*envp)
	{
		node.name = ft_strduptochar(*envp, '=');
		node.info = ft_strdup(ft_strchr(*envp, '=') + 1);
		tmp->next = ft_lstnew(&node, sizeof(node));
		tmp = tmp->next;
		envp++;
	}
	return (begin);
}

char		is_printable(char c)
{
	if (c >= ' ' && c <= '~')
		return (1);
	else if (c == '\n' || c == '\t')
		return (1);
	return (0);
}

int			ft_iputchar(int c)
{
	write (1, &c, 1);
	return (0);
}

char	*ft_getchar(int *len)
{
	static char		buf[8];

	ft_bzero(buf, 8);
	*len = read(0, buf, 8);
	return (buf);
}


void	ft_rstrcpy(char *dest, char *src)
{
	int		i;

	i = ft_strlen(src);
	while (i != -1)
	{
		dest[i] = src[i];
		i--;
	}
}

size_t		ft_putstrto(char *str, char c)
{
	size_t		i;

	i = 0;
	while (str[i] && str[i] != c)
	{
		ft_putchar(str[i]);
		i++;
	}
	return (i);
}

size_t	ft_strlento(char *str, char c)
{
	size_t		i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}
