#include "ft_printf.h"

void	handle_chr(t_fields *fields, int c, t_list **lst)
{
	char				*str;
	unsigned long long	len;

	str = malloc(1);
	if (str == NULL)
		return ;
	str[0] = c;
	len = 1;
	if (fields->width > len)
	{
		apply_padding(fields, (void **)(&str), len);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, str, len);
}

void    handle_str(t_fields *fields, char *str, t_list **lst)
{
	unsigned long long	len;

	if (str == NULL)
		str = ft_strdup("(null)");
	else
		str = ft_strdup(str);
	len = ft_strlen(str);
	if (fields->precision >= 0 && fields->precision < len)
		len = fields->precision;
	if (fields->width > len)
	{
		apply_padding(fields, (void **)(&str), len);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, str, len);
}

void	handle_esc(t_fields *fields, t_list **lst)
{
	char				*str;
	unsigned long long	len;

	str = ft_strdup("%");
	len = 1;
	if (fields->width > len)
	{
		apply_padding(fields, (void **)(&str), len);
		len = fields->width;
	}
	ft_lst_memblock_append(lst, str, len);
}