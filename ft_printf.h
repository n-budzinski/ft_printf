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
/*   ft_printf.h        ▪             .                                       */
/*   .                                                                        */
/*   By: nbudzins <nbudzins@student.42warsaw.pl>            ▪                 */
/*                                                                   .        */
/*   Created: 2024/03/14 08:59:12 by nbudzins                                 */
/*   Updated: 2024/03/14 09:00:05 by nbudzins                                 */
/*                                               .                 .          */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
#define FT_PRINTF_H

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include "./libft/libft.h"

typedef struct s_memblock
{
	void				*data;
	size_t				size;
}	t_memblock;

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

void	ft_lst_memblock_prepend(t_list **lst, void *mem, size_t size);
void	ft_lst_memblock_append(t_list **lst, void *mem, size_t size);
unsigned long long  ft_lst_memblock_getttlsize(t_list *lst);
void	*ft_lst_memblock_join(t_list *lst);
void ft_lst_memblock_clear(void *memblock);
void	handle_chr(t_fields *fields, int c, t_list **lst);
void    handle_str(t_fields *fields, char *str, t_list **lst);
void	handle_esc(t_list **lst);
static char	*ft_uitoa(unsigned int value);
static char	*ft_ulltohexa(unsigned long long value, char *set);
void	handle_uint(t_fields *fields, unsigned int value, t_list **lst);
void	handle_dec(t_fields *fields, int value, t_list **lst);
void handle_ptr(t_fields *fields, unsigned long long value, t_list **lst);
static void	handle_hex(t_fields *fields, t_list **lst, char *str);
void	handle_lhex(t_fields *fields, unsigned int value, t_list **lst);
void	handle_uhex(t_fields *fields, unsigned int value, t_list **lst);
char	*join_and_free(char *s1, char *s2);
void	*ft_memjoin(void *m1, size_t m1_size, void *m2, size_t m2_size);
char    *ft_pad(size_t n, int c);
char	*ft_strndup(const char *s, size_t n);
void apply_padding(void **str, size_t len, size_t width, char flg_minus);
static t_fields	*new_fields(void);
static void set_fields(t_fields *fields, char **fstring);
static void	fmt_parse(char **fstring, t_list **lst, va_list ap)	;
static void	striter(va_list ap, const char *fstring, char *str, t_list **lst);
int	ft_printf(const char *fstring, ...);
#endif