#include "ft_nm.h"
#include "libft.h"
#include <stddef.h>

void	print_addr(size_t addr)
{
	size_t	i;
	size_t	bit_4;
	size_t	count;

	count = sizeof(size_t) * 2;
	i = 0;
	while (i < count)
	{
		bit_4 = (addr >> ((count - i - 1) * 4)) & 0xf;
		if (bit_4 < 10)
			ft_putnbr(bit_4);
		else
			ft_putchar('a' + bit_4 - 10);
		i++;
	}
}

void	print_addr_32(size_t addr)
{
	size_t	i;
	size_t	bit_4;
	size_t	count;

	count = sizeof(size_t);
	i = 0;
	while (i < count)
	{
		bit_4 = (addr >> ((count - i - 1) * 4)) & 0xf;
		if (bit_4 < 10)
			ft_putnbr(bit_4);
		else
			ft_putchar('a' + bit_4 - 10);
		i++;
	}
}