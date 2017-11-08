#include "minishell.h"

void	*ft_realloc(void *str, size_t *len, size_t add)
{
	char	*res;

	res = (void *)malloc(*len + add);
	if (!res)
		return (NULL);
	ft_memcpy(res, str, *len);
	*len += add;
	free(str);
	return (res);
}