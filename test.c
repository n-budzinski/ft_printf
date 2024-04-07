#include "ft_printf.h"
#include <stdio.h>

int	main()
{
    int ptr = 22222;
	printf("%x\n", ptr);
	ft_printf("%x\n", ptr);
	return (0);
}