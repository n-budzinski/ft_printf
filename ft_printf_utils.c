#include "ft_printf.h"

char	*join_and_free(char *s1, char *s2)
{
	char	*ptr;

	ptr = NULL;
	if (s1 && s2)
	{
		ptr = ft_strjoin(s1, s2);
		if (ptr != NULL)
		{
			free(s1);
			free(s2);
		}
	}
	else
	{
		if (s1)
			ptr = s1;
		if (s2)
			ptr = s2;
	}
	return (ptr);
}

void	*ft_memjoin(void *m1, size_t m1_size, void *m2, size_t m2_size)
{
	void	*ptr;

	ptr = (void *)malloc(m1_size + m2_size);
	if (ptr != NULL)
	{
		ft_memcpy(ptr, m1, m1_size);
		ft_memcpy(ptr + m1_size, m2, m2_size);
		free(m1);
		free(m2);
	}
	return (ptr);
}

char    *ft_pad(size_t n, int c)
{
	char		*str;

	if (n <= 0)
		return (ft_strdup(""));
	str = malloc(n);
	if (str == NULL)
		return (NULL);
	ft_memset(str, c, n);
    return (str);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*ptr;

	ptr = (char *)malloc(n + 1);
	if (ptr == NULL)
		return (NULL);
	ft_strlcpy(ptr, s, n + 1);
	return (ptr);
}


void apply_padding(void **str, size_t len, size_t width, char flg_minus)
{
	void	*ptr;

	ptr = ft_pad(width, ' ');
	if (flg_minus)
		ft_memcpy(ptr, *str, len);
	else
		ft_memcpy(ptr + width - len, *str, len);
	free(*str);
	*str = ptr;
}