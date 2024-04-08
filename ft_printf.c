/* ************************************************************************** */
/*                                                                            */
/*                                    .                            .          */
/*      .    ▄█▀· ·▄▄▄▄•   ▄▄▌ ▐ ▄▌ ▄▄▄  ▄▄▄  .▄▄ ·  ▄▄▄· ▄▄▌ ▐ ▄▌            */
/*          ▐█  ▄ ▪▀·.█▌   ██· █▌▐█▐█ ▀█ ▀▄ █·▐█ ▀. ▐█ ▀█ ██  █▌▐█      .     */
/*          .▀▀▀█ ▄█▀▀▀•   ██ ▐█▐▐▌▄█▀▀█ ▐▀▀▄ ▄▀▀▀█▄▄█▀▀█ ██▪▐█▐▐▌            */
/*          ·  ▪▐▌█▌▪▄█▀   ▐█▌██▐█▌▐█ ▪▐▌▐█•█▌▐█▄▪▐█▐█  ▐▌▐█▌██▐█▌            */
/*    ▪        ·▀ ·▀▀▀ •    ▀▀▀▀ ▀▪ ▀  ▀ .▀  ▀ ▀▀▀▀  ▀  ▀  ▀▀▀▀ ▀             */
/*                                                          .           ▪     */
/*            .                                    .                          */
/*   ft_printf.c        ▪             .                                       */
/*   .                                                                        */
/*   By: nbudzins <nbudzins@student.42warsaw.pl>            ▪                 */
/*                                                                   .        */
/*   Created: 2024/03/23 00:11:36 by nbudzins                                 */
/*   Updated: 2024/04/05 14:53:35 by nbudzins                                 */
/*                                               .                 .          */
/* ************************************************************************** */

#include "ft_printf.h"

static t_fields	*new_fields(void)
{
	t_fields	*fields;

	fields = malloc(sizeof (t_fields));
	if (!fields)
		return (NULL);
	fields->flg_minus = 0;
	fields->flg_plus = 0;
	fields->flg_space = 0;
	fields->flg_zero = 0;
	fields->flg_alt = 0;
	fields->width = 0;
	fields->precision = -1;
	return (fields);
} 

static void set_fields(t_fields *fields, char **fstring)
{
	while (**fstring)
	{
		if (**fstring == '-')
			fields->flg_minus = 1;
		else if(**fstring == '+')
			fields->flg_minus = 1;
		else if(**fstring == ' ')
			fields->flg_space = 1;
		else if(**fstring == '0')
			fields->flg_zero = 1;
		else if(**fstring == '#')
			fields->flg_alt = 1;
		else
			break;
		(*fstring)++;
	}	
	if (ft_isdigit(**fstring))
		fields->width = ft_atoi(*fstring);
	while (ft_isdigit(**fstring))
		(*fstring)++;
	if (**fstring == '.')
		fields->precision = ft_atoi(++*fstring);
	while (ft_isdigit(**fstring))
		(*fstring)++;
}

static void	fmt_parse(char **fstring, t_list **lst, va_list ap)	
{
	t_fields	*fields;

	fields = new_fields();
	if (!fields)
		return ;
	set_fields(fields, fstring);
	if (**fstring == 'c')
		handle_chr(fields, va_arg(ap, int), lst);
	if (**fstring == 's')
		handle_str(fields, va_arg(ap, char*), lst);
	else if (**fstring == 'p')
		handle_ptr(fields, va_arg(ap, unsigned long long), lst);
	else if (**fstring == 'd' || **fstring == 'i')
		handle_dec(fields, va_arg(ap, int), lst);
	else if (**fstring == 'u')
		handle_uint(fields, va_arg(ap, unsigned int), lst);
	else if (**fstring == 'x')
		handle_lhex(fields, va_arg(ap, unsigned int), lst);
	else if (**fstring == 'X')
		handle_uhex(fields, va_arg(ap, unsigned int), lst);
	else if	(**fstring == '%')
		handle_esc(fields, lst);
	free(fields);
	(*fstring)++;
}

static void	striter(va_list ap, const char *fstring, char *str, t_list **lst)
{
	char	*sym;
	char	*sub;
	t_list	*new;

	while(*str != '\0')
	{
		sym = ft_strchr(str, '%');
		if (sym == NULL)
		{
			sub = ft_strdup(str);
			ft_lst_memblock_append(lst, sub, ft_strlen(str));
			break ;
		}
		if (fstring != sym)
		{
			sub = ft_strndup(str, (size_t)(sym - str));
			ft_lst_memblock_append(lst, sub, (size_t)(sym - str));
			str = sym;
		}
		str++;
		fmt_parse(&str, lst, ap);
	}
}

int	ft_printf(const char *fstring, ...)
{

	va_list		ap;
	char		*str;
	t_list		*lst;
	size_t		ttl;
	
	lst = NULL;
	str = (char *)fstring;
	va_start(ap, fstring);
	striter(ap, fstring, str, &lst);
	str = ft_lst_memblock_join(lst);
	ttl = ft_lst_memblock_getttlsize(lst);
	write(1, str, ttl);
	ft_lstclear(&lst, &ft_lst_memblock_clear);
	free(str);
	va_end(ap);
	return (ttl);
}
