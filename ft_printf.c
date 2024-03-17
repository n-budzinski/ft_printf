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
/*   main.c             ▪             .                                       */
/*   .                                                                        */
/*   By: nbudzins <nbudzins@student.42warsaw.pl>            ▪                 */
/*                                                                   .        */
/*   Created: 2024/03/14 08:47:16 by nbudzins                                 */
/*   Updated: 2024/03/17 09:03:14 by nbudzins                                 */
/*                                               .                 .          */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

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

static void	uitoa(unsigned int value)
{
	char c;

	c = value % 10 + '0';
	if (value > 9)
	{
		value /= 10;
		uitoa(value);
	}
	write(1, &c, 1);
}

static size_t	print_hex(unsigned long long value, char *set)
{
	char c;
	char bitshift;
	char start;

	start = 0;
	bitshift = 64;
	while (bitshift)
	{
		bitshift -= 4;
		c = value >> bitshift & 0x0F;
		if (!start && c)
			start = 1;
		if (start)
			write(1, &(set[c]), 1);
	}
	return (0);
}

size_t	handle_chr(fields_t *fields, int value)
{
	write(1, &value, 1);
	return (1);
}

size_t	handle_str(fields_t *fields, char *value)
{
	int len;
	len = strlen(value);
	write(1, value, len);
	return (0);
}

size_t	handle_ptr(fields_t *fields, unsigned long long value)
{
	write(1, "0x", 2);
	print_hex(value, "0123456789abcdef");
	return (0);
}

size_t	handle_dec(fields_t *fields, int value)
{
	//TODO
	return (0);
}

size_t	handle_uint(fields_t *fields, unsigned int value)
{
	uitoa(value);
	return (0);

}
size_t	handle_int(fields_t *fields, int value)
{
	if (value == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	else if (value < 0)
	{
		write(1, "-", 1);
		value *= -1;
	} 
	uitoa(value);
	return (0);
}

size_t	handle_lhex(fields_t *fields, unsigned int value)
{
	if (fields->flg_alt)
		write(1, "0x", 2);
	return (print_hex(value, "0123456789abcdef"));
}

size_t	handle_uhex(fields_t *fields, unsigned int value)
{
	if (fields->flg_alt)
		write(1, "0x", 2);
	return (print_hex(value, "0123456789ABCDEF"));
}

size_t	handle_esc()
{
	return (write(1, "%", 1));
}

static void	format_handler(char **fstring, va_list ap, size_t *nchr)	
{
	fields_t *fields;

	fields = new_fields();
	if (!fields)
		return ;
	set_fields(fields, fstring);
	if (**fstring == 'c')
		*nchr += handle_chr(fields, va_arg(ap, int));
	else if (**fstring == 's')
		*nchr += handle_str(fields, va_arg(ap, char*));
	else if (**fstring == 'p')
		*nchr += handle_ptr(fields, va_arg(ap, unsigned long long));
	else if (**fstring == 'd' || **fstring == 'i')
		*nchr += handle_dec(fields, va_arg(ap, int));
	else if (**fstring == 'u')
		*nchr += handle_uint(fields, va_arg(ap, unsigned int));
	else if (**fstring == 'x')
		*nchr += handle_lhex(fields, va_arg(ap, unsigned int));
	else if (**fstring == 'X')
		*nchr += handle_uhex(fields, va_arg(ap, unsigned int));
	else if	(**fstring == '%')
		*nchr += handle_esc();
	(*fstring)++;
}

int	ft_printf(const char *fstring, ...)
{

	int		nchars;
	va_list		ap;
	char		*ptr;
	size_t		nchr;

	ptr = (char *)fstring;
	va_start(ap, fstring);

	while (*ptr != '\0')
		if (*ptr == '%')
		{
			ptr++;
			format_handler(&ptr, ap, &nchr);
		}
		else
			write(1, ptr++, 1);
	va_end(ap);
	return (nchr);
}

int	main()
{
	char *word = "MY";
	char *word2 = "CRUEL";
	char c = '!';
	unsigned int uint = 94121;
	ft_printf("HELLO, %s %s WORLD%c %#X\n", word, word2, c, uint);
	printf("HELLO, %s %s WORLD%c %#X\n", word, word2, c, uint);
	return (0);
}
