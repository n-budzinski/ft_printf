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
/*   Updated: 2024/03/24 02:30:55 by nbudzins                                 */
/*                                               .                 .          */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include "./libft/libft.h"

typedef struct fields_s
{
	char flg_minus;
	char flg_plus;
	char flg_space;
	char flg_zero;
	char flg_alt;
	unsigned int flg_width;
}		fields_t;

static fields_t *new_fields()
{
	fields_t *fields;
	fields = malloc(sizeof (fields_t));
	if (!fields)
		return (NULL);
	fields->flg_minus = 0;
	fields->flg_plus = 0;
	fields->flg_space = 0;
	fields->flg_zero = 0;
	fields->flg_alt = 0;
	fields->flg_width = 0;
	return (fields);
} 

static void	ft_lststr_append(t_list **lst, char *str)
{
	t_list	*node;

	node = ft_lstnew(ft_strdup(str));
	if (node)
		ft_lstadd_back(lst, node);

}
static void set_fields(fields_t *fields, char **fstring)
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
	//TODO WIDTH HANDLE
	//fields->flg_width = ft_atoi(*fstring);
}

static void	uitoa(unsigned int value, t_list **lst)
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
	mem--;
	str[mem] = '\0';
	while (value > 9)
	{
		str[--mem] = value % 10 + '0';
		value /= 10;
	}
	str[0] = value % 10 + '0';
	ft_lststr_append(lst, str);
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
		if (!start && value >> shift & 0x0F || shift == 0)
			start = 1;
		if (start)
		{
			if (str == NULL)
			{
				len = shift / 4 + 1;
				str = (char *)ft_calloc(len + 1, sizeof(char));
				if (str == NULL)
					break ;
			}
			str[len - 1 - (shift / 4)] = set[value >> shift & 0x0F];
		}
	}
	return (str);
}

static void	handle_chr(fields_t *fields, int c, t_list **lst)
{
	t_list	*node;
	char 	str[2];

	str[0] = c;
	str[1] = '\0';
	ft_lststr_append(lst, str);
}

static void	handle_str(fields_t *fields, char *str, t_list **lst)
{
	ft_lststr_append(lst, str);
}
static void handle_ptr(fields_t *fields, unsigned long long value, t_list **lst)
{
	if (!value)
		ft_lststr_append(lst, "(nil)");
	else
	{
		ft_lststr_append(lst, "0x");
		ft_lststr_append(lst, ft_ulltohexa(value, "0123456789abcdef"));
	}
}

static void	handle_dec(fields_t *fields, int value, t_list **lst)
{
	return ;
}

static void	handle_uint(fields_t *fields, unsigned int value, t_list **lst)
{
	uitoa(value, lst);
}

static void	handle_int(fields_t *fields, int value, t_list **lst)
{
	if (value == -2147483648)
		ft_lststr_append(lst, "-2147483648");
	else if (value < 0)
	{
		write(1, "-", 1);
		value *= -1;
	} 
	uitoa(value, lst);
}

static void	handle_lhex(fields_t *fields, unsigned int value, t_list **lst)
{
	if (fields->flg_alt)
		ft_lststr_append(lst, "0x");
	ft_lststr_append(lst, ft_ulltohexa(value, "0123456789abcdef"));
}

static void	handle_uhex(fields_t *fields, unsigned int value, t_list **lst)
{
	if (fields->flg_alt)
		ft_lststr_append(lst, "0x");
	ft_lststr_append(lst, ft_ulltohexa(value, "0123456789ABCDEF"));
}

static void	handle_esc(t_list **lst)
{
	ft_lststr_append(lst, "%");
}


static void	format_handler(char **fstring, t_list **lst, va_list ap)	
{
	fields_t	*fields;

	fields = new_fields();
	if (!fields)
		return ;
	set_fields(fields, fstring);
	if (**fstring == 'c')
		handle_chr(fields, va_arg(ap, int), lst);
	else if (**fstring == 's')
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
		handle_esc(lst);
	free(fields);
	(*fstring)++;
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

char	*ft_lststr_join(t_list *lst)
{
	char	*str;
	char	*tmp;

	str = ft_strdup("");
	while (lst)
	{
		tmp = ft_strjoin(str, (const char *)(lst->content));
		free(str);
		str = tmp;
		lst = lst->next;
	}
	return (str);
}

int	ft_printf(const char *fstring, ...)
{

	va_list		ap;
	char		*str;
	t_list		*lst;
	char		*sym;
	char		*sub;
	t_list		*new;
	size_t		count;
	lst = NULL;

	str = (char *)fstring;
	va_start(ap, fstring);
	while(*str != '\0')
	{
		sym = ft_strchr(str, '%');
		if (sym == NULL)
		{
			sub = ft_strdup(str);
			new = ft_lstnew(sub);
			ft_lstadd_back(&lst, new);
			break;
		}
		else
		{
			if (fstring != sym)
			{
				sub = ft_strndup(str, (size_t)(sym - str));
				new = ft_lstnew(sub);
				ft_lstadd_back(&lst, new);
				str = sym;
			}
			str++;
			format_handler(&str, &lst, ap);
		}
	}
	str = ft_lststr_join(lst);
	count = write(1, str, ft_strlen(str));
	ft_lstclear(&lst, &free);
	free(str);
	va_end(ap);
	return (count);
}

int	main()
{
	char *word = "MY";
	char *word2 = "CRUEL";
	char c = '!';
	unsigned int uint = 94121;
	ft_printf("HELLO, %s %s WORLD%c%p\n", word, word2, c, word);
	printf("HELLO, %s %s WORLD%c%p\n", word, word2, c, word);
	//ft_printf("HELLO, %s %s WORLD%c %#X%p\n", word, word2, c, uint, NULL);
	//printf("HELLO, %s %s WORLD%c %#X%p\n", word, word2, c, uint, NULL);
	return (0);
}
