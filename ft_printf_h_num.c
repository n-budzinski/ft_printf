#include "ft_printf.h"

static char	*ft_uitoa(unsigned int value)
{
	unsigned int	tmp;
	unsigned int	mem;
	char		*str;

	if (value == 0)
		return (ft_strdup("0"));
	mem = 0;
	tmp = value;
	while (tmp)
	{
		tmp /= 10;
		mem++;
	}
	str = malloc(mem + 1);
	if (str == NULL)
		return (NULL);
	str[mem--] = '\0';
	while (value > 9)
	{
		str[mem] = value % 10 + '0';
		value /= 10;
		mem--;
	}
	str[0] = value % 10 + '0';
	return (str);
}

void	handle_uint(t_fields *fields, unsigned int value, t_list **lst)
{
	char	*ptr;
	int		len;

	ptr = ft_uitoa(value);
	if (ptr == NULL)
		return ;
	len = ft_strlen(ptr);
	if (fields->width > len)
	{
		apply_padding((void **)(&ptr), len, fields->width, fields->flg_minus);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, ptr, len);
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
			value *= -1;
			ptr = join_and_free(ft_strdup("-"), ft_uitoa(value));
		}
		else
			ptr = ft_uitoa(value);
	}
    if (ptr == NULL)
        return ;
	len = ft_strlen(ptr);
	if (fields->width > len)
	{
		apply_padding((void **)(&ptr), len, fields->width, fields->flg_minus);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, ptr, len);
}