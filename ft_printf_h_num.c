#include "ft_printf.h"

static int	calcsize(long value, int padding)
{
	int	buffsize;
	long	val;

	val = value;
	buffsize = 0;
	if (val == 0)
		buffsize++;
	else
	{
		while (val)
		{
			buffsize++;
			val /= 10;
		}
	}
	if (padding > buffsize)
		buffsize += padding - buffsize;
	if (value < 0)
		buffsize++;
	return (buffsize);
}

static char	*ft_padded_itoa(long value, int padding)
{
	unsigned int	mem;
	char			*str;

	mem = calcsize(value, padding);
	str = malloc(mem + 1);
	if (str == NULL)
		return (NULL);
	ft_memset(str, '0', mem);
	if (value < 0)
	{
		value = -value;
		str[0] = '-';
	}
	str[mem--] = '\0';
	while (value > 9)
	{
		str[mem] = value % 10 + '0';
		value /= 10;
		mem--;
	}
	str[mem] = value % 10 + '0';
	return (str);
}

void	handle_uint(t_fields *fields, unsigned int value, t_list **lst)
{
	char	*ptr;
	int		len;

	if (fields->precision == 0 && value == 0)
		ptr = ft_strdup("");
	else
		ptr = ft_padded_itoa(value, fields->precision);
	len = ft_strlen(ptr);
	if (fields->width > len)
	{
		apply_padding(fields, (void **)(&ptr), len);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, ptr, len);
}

void	handle_dec(t_fields *fields, int value, t_list **lst)
{
	char	*ptr;
	int		len;

	if (fields->precision == 0 && value == 0)
		ptr = ft_strdup("");
	else
		ptr = ft_padded_itoa(value, fields->precision);
	len = ft_strlen(ptr);
	if (fields->width > len)
	{
		apply_padding(fields, (void **)(&ptr), len);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, ptr, len);
}