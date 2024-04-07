NAME = libftprintf.a
LIBFT_SRC = libft
AR = ar rcs $(NAME)
CC = gcc
CCFLAGS =
SRC =\
ft_printf_memblock.c \
ft_printf_utils.c \
ft_printf_h_chr.c \
ft_printf_h_hex.c \
ft_printf.c \
ft_printf_h_num.c

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CCFLAGS) -g -I. -c $< -o $@

$(NAME): $(OBJ)
	@make -C $(LIBFT_SRC) $(MAKECMDGOALS) bonus
	cp libft/libft.a .
	mv libft.a $(NAME)
	$(AR) $(OBJ)

all: $(NAME)

clean:
	@make -C $(LIBFT_SRC) $(MAKECMDGOALS)
	$(RM) $(OBJ)

fclean: clean
	@make -C $(LIBFT_SRC) $(MAKECMDGOALS)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re