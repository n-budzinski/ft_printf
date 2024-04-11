#include "ft_printf.h"
#include <stdio.h>

int	main()
{
    int str = -1337;
	// printf("%d\n", str);
	//ft_printf("%.13d\n", (int)-2147483648);
	ft_printf("%.4i %.2i %.20i %.0i %.0i %.i %.i %.i", 127, 0, 1023, 0, (int)-2147483648, 0, 1, (int)-2147483648);
	return (0);
}