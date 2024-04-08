#include "ft_printf.h"
#include <stdio.h>

int	main()
{
    char *str = "hello";
	printf("%s\n", str);
	ft_printf("%s\n", str);
	return (0);
}