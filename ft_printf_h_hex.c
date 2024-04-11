#include "ft_printf.h"
#include <stdio.h>
#include "libft/libft.h"

static char	*ft_ulltohexa(unsigned long long value, char *set)
{
	char	shift;
	char	start;
	char	*str;
	int		len;

	start = 0;
	shift = 64;
	str = NULL;
	while (--shift >= 0)
	{
		if ((!start && value >> shift & 0x0F) || shift == 0)
			start = 1;
		if (start)
		{
			if (str == NULL)
			{
				len = shift / 4 + 1;
				str = ft_calloc(1, len + 1);
				if (str == NULL)
					break ;
			}
			str[len - 1 - (shift / 4)] = set[value >> shift & 0x0F];
		}
	}
	str[len] = '\0';
	return (str);
}

void handle_ptr(t_fields *fields, unsigned long long value, t_list **lst)
{
	int		len;
	char	*str;

	str = ft_ulltohexa(value, "0123456789abcdef");
	len = ft_strlen(str) + 2;
	if (fields->precision > len)
		str = join_and_free(ft_pad(fields->precision, '0'), str);
	str = join_and_free(ft_strdup("0x"), str);
	if (fields->width > len)
	{
		apply_padding(fields, (void **)(&str), len);
		len = fields->width;
	}
	if (fields->precision >= 0 && fields->precision > len)
		len = fields->precision;
	ft_lst_memblock_append(lst, str, len);
}

static void	handle_hex(t_fields *fields, t_list **lst, char *str)
{
	int		len;
	void	*ptr;

	len = ft_strlen(str);
	if (fields->precision > len)
	{
		ptr = ft_pad(fields->precision - len, '0');
		str = ft_memjoin(ptr, fields->precision - len, str, len);
		len = fields->precision;
	}
	if (fields->flg_alt)
	{
		str = ft_memjoin(ft_strdup("0x"), 2, str, ft_strlen(str));
		len += 2;
	}
	if (fields->width > len)
	{
		apply_padding(fields, (void **)(&str), len);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, str, len);
}

void	handle_lhex(t_fields *fields, unsigned int value, t_list **lst)
{
	if (fields->precision == 0 && value == 0)
		handle_hex(fields, lst, ft_strdup(""));
	else
		handle_hex(fields, lst, ft_ulltohexa(value, "0123456789abcdef"));
}

void	handle_uhex(t_fields *fields, unsigned int value, t_list **lst)
{
	if (fields->precision == 0 && value == 0)
		handle_hex(fields, lst, ft_strdup(""));
	else
		handle_hex(fields, lst, ft_ulltohexa(value, "0123456789ABCDEF"));
}