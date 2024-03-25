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
/*   Updated: 2024/03/24 22:22:58 by nbudzins                                 */
/*                                               .                 .          */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include "./libft/libft.h"

typedef struct s_fields
{
	char			flg_minus;
	char			flg_plus;
	char			flg_space;
	char			flg_zero;
	char			flg_alt;
	unsigned int	width;
	unsigned int	precision;
}	t_fields;

static t_fields	*new_fields()
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
	fields->precision = 0;
	return (fields);
} 

typedef struct s_memblock
{
	void				*data;
	size_t				size;
}	t_memblock;

void	*ft_memjoin(void *m1, size_t m1_size, void *m2, size_t m2_size)
{
	void	*ptr;

	ptr = (void *)malloc(m1_size + m2_size);
	if (ptr != NULL)
	{
		ft_memcpy(ptr, m1, m1_size);
		ft_memcpy(ptr + m1_size, m2, m2_size);
	}
	return (ptr);
}

char    *ft_create_padding(size_t n, int c)
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
	void	*ptr;

	ptr = (char *)malloc(n + 1);
	if (ptr == NULL)
		return (NULL);
	ft_strlcpy(ptr, s, n + 1);
	return (ptr);
}

void	ft_lst_memblock_append(t_list **lst, void *mem, size_t size)
{
	t_list		*node;
	t_memblock	*memblock;

	memblock = malloc(sizeof(t_memblock));
	if (memblock == NULL)
		return ;
	memblock->data = mem;
	memblock->size = size;
	node = ft_lstnew(memblock);
	if (node)
		ft_lstadd_back(lst, node);
}

unsigned long long	ft_lst_memblock_getttlsize(t_list *lst)
{
	unsigned long long	size;

	size = 0;
	while (lst)
	{
		size += ((t_memblock *)(lst->content))->size;
		lst = lst->next;
	}
	return (size);
}

void	*ft_lst_memblock_join(t_list *lst)
{
	unsigned long long	size;
	unsigned long long	pos;
	void	*ptr;
	t_memblock	*tmp;

	size = ft_lst_memblock_getttlsize(lst);
	pos = 0;
	ptr = (void *)malloc(size);
	if (ptr == NULL)
		return (NULL);
	while (lst)
	{
		tmp = (t_memblock *)(lst->content);
		ft_memcpy(ptr + pos, tmp->data, tmp->size);
		pos += tmp->size;
		lst = lst->next;
	}
	return (ptr);
}

void	ft_lst_memblock_clear(void *memblock)
{
	if (memblock)
	{
		free(((t_memblock *)memblock)->data);
		free(memblock);
	}
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

char	*ft_uitoa(unsigned int value, t_list **lst)
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

char	*ft_ulltohexa(unsigned long long value, char *set)
{
	char	shift;
	char	start;
	char	*str;
	int	len;

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
	return (str);
}

static void	handle_chr(t_fields *fields, int c, t_list **lst)
{
	unsigned char 	*ptr;

	ptr = malloc(1);
	if (ptr == NULL)
		return ;
	*ptr = c;
	if (fields->width && !fields->flg_minus)
		ft_lst_memblock_append(lst, ft_create_padding(fields->width - 1, ' '), 1);
	ft_lst_memblock_append(lst, ptr, 1);
	if (fields->width && fields->flg_minus)
		ft_lst_memblock_append(lst, ft_create_padding(fields->width - 1, ' '), 1);
}

static void	handle_str(t_fields *fields, char *str, t_list **lst)
{
	void				*ptr;
	unsigned long long	nmem;
	unsigned long long	len;

	if (str == NULL)
		return ;
	len = ft_strlen(str);
	if (fields->precision && fields->precision < len)
		nmem = fields->precision;
	else
		nmem = len;
	ptr = (void *)malloc(nmem);
	if (str == NULL)
		return ;
	ft_memcpy(ptr, str, nmem);
	if (fields->width && !fields->flg_minus)
		ft_lst_memblock_append(lst, ft_create_padding(fields->width - len, ' '), fields->width - len);
	ft_lst_memblock_append(lst, ptr, nmem);
	if (fields->width && fields->flg_minus)
		ft_lst_memblock_append(lst, ft_create_padding(fields->width - len, ' '), fields->width - len);
}

// static void handle_ptr(t_fields *fields, unsigned long long value, t_list **lst)
// {
// 	if (!value)
// 		ft_lststr_append(lst, ft_strdup("(nil)"));
// 	else
// 	{
// 		ft_lststr_append(lst, ft_strdup("0x"));
// 		ft_lststr_append(lst, ft_ulltohexa(value, "0123456789abcdef"));
// 	}
// }

// static void	handle_dec(t_fields *fields, int value, t_list **lst)
// {
// 	return ;
// }

// static void	handle_uint(t_fields *fields, unsigned int value, t_list **lst)
// {
// 	uitoa(value, lst);
// }

// static void	handle_int(t_fields *fields, int value, t_list **lst)
// {
// 	if (value == -2147483648)
// 		pad_and_append(fields, ft_strdup("-2147483648"), lst);
// 	else if (value < 0)
// 	{
// 		pad_and_append(fields, ft_strdup("-"), lst);
// 		value *= -1;
// 	} 
// 	uitoa(value, lst);
// }

// static void	handle_lhex(t_fields *fields, unsigned int value, t_list **lst)
// {
// 	if (fields->flg_alt)
// 		pad_and_append(fields, ft_strdup("0x"), lst);
// 	pad_and_append(lst, ft_ulltohexa(value, "0123456789abcdef"));
// }

// static void	handle_uhex(t_fields *fields, unsigned int value, t_list **lst)
// {
// 	if (fields->flg_alt)
// 		ft_lststr_append(lst, ft_strdup("0x"));
// 	ft_lststr_append(lst, ft_ulltohexa(value, "0123456789ABCDEF"));
// }

static void	handle_esc(t_list **lst)
{
	ft_lst_memblock_append(lst, ft_strdup("%"), 1);
}

void	fmt_parse(char **fstring, t_list **lst, va_list ap)	
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
	// else if (**fstring == 'p')
	// 	handle_ptr(fields, va_arg(ap, unsigned long long), lst);
	// else if (**fstring == 'd' || **fstring == 'i')
	// 	handle_dec(fields, va_arg(ap, int), lst);
	// else if (**fstring == 'u')
	// 	handle_uint(fields, va_arg(ap, unsigned int), lst);
	// else if (**fstring == 'x')
	// 	handle_lhex(fields, va_arg(ap, unsigned int), lst);
	// else if (**fstring == 'X')
	// 	handle_uhex(fields, va_arg(ap, unsigned int), lst);
	else if	(**fstring == '%')
		handle_esc(lst);
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

int	main()
{
	char *word = "MY";
	char *word2 = "CRUEL";
	char c = '!';
	unsigned int uint = 94121;
	ft_printf("HELLO %c %c %10.3s\n", 0, '0', word);
	//printf("HELLO %c %10.3s\n", 0, word);
	//ft_printf("HELLO, %s %s WORLD%c %#X%p\n", word, word2, c, uint, NULL);
	//printf("HELLO, %s %s WORLD%c %#X%p\n", word, word2, c, uint, NULL);
	return (0);
}