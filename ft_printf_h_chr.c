#include "ft_printf.h"

void	handle_chr(t_fields *fields, int c, t_list **lst)
{
	unsigned char 	*ptr;

	ptr = malloc(1);
	if (ptr == NULL)
		return ;
	*ptr = c;
	if (fields->width && !fields->flg_minus)
		ft_lst_memblock_append(lst, ft_pad(fields->width - 1, ' '), 1);
	ft_lst_memblock_append(lst, ptr, 1);
	if (fields->width && fields->flg_minus)
		ft_lst_memblock_append(lst, ft_pad(fields->width - 1, ' '), 1);
}

void    handle_str(t_fields *fields, char *str, t_list **lst)
{
	char				*ptr;
	unsigned long long	len;

	if (str == NULL)
		return ;
	len = ft_strlen(str);
	if (fields->precision && fields->precision < len)
		len = fields->precision;
	if (fields->width > len)
	{
		ptr = ft_pad(fields->width, ' ');
		if (fields->flg_minus)
			ft_memcpy(ptr, str, len);
		else
			ft_memcpy(ptr + fields->width - len, str, len);
	}
	else
	{

		ptr = ft_strndup(str, len);
		fields->width = len;
	}
	ft_lst_memblock_append(lst, ptr, fields->width);
}

void	handle_esc(t_list **lst)
{
	ft_lst_memblock_append(lst, ft_strdup("%"), 1);
}