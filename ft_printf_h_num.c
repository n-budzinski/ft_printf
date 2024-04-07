#include "ft_printf.h"

static char	*ft_uitoa(unsigned int value, t_list **lst)
{
	unsigned int	tmp;
	unsigned int	mem;
	t_list		*node;
	char		*str;

	mem = 1;
	tmp = value;
	while (tmp)
	{
		tmp /= 10;
		mem++;
	}
	str = malloc(mem + 1);
	if (str == NULL)
		return (NULL);
	mem--;
	str[mem] = '\0';
	while (value > 9)
	{
		str[--mem] = value % 10 + '0';
		value /= 10;
	}
	str[0] = value % 10 + '0';
	return (str);
}

void	handle_uint(t_fields *fields, unsigned int value, t_list **lst)
{
	char	*ptr;
	int		len;

	ptr = ft_uitoa(value, lst);
	if (ptr == NULL)
		return ;
	len = ft_strlen(ptr);
	if (fields->width > len)
		apply_padding((void **)(&ptr), len, fields->width, fields->flg_minus);
	ft_lst_memblock_append(lst, ptr, fields->width);
}

void	handle_dec(t_fields *fields, int value, t_list **lst)
{
	char	*ptr;
	int		len;

    ptr = NULL;
	if (value == -2147483648)
		ptr = ft_strdup("-2147483648");
	else
	{
		if (value < 0)
		{
			ptr = ft_strdup("-");
			value *= -1;
		}
		ptr = join_and_free(ptr, ft_uitoa(value, lst));
	}
    if (ptr == NULL)
        return ;
	len = ft_strlen(ptr);
	if (fields->width > len)
		apply_padding((void **)(&ptr), len, fields->width, fields->flg_minus);
	ft_lst_memblock_append(lst, ptr, fields->width);
}