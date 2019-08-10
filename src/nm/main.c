/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtertysh <gtertysh@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/13 14:52:27 by gtertysh          #+#    #+#             */
/*   Updated: 2019/08/10 16:05:21 by gtertysh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nm.h"


void		hanldle_file(const char *filename)
{
	t_file	file;
	uint32_t	magic;

	// create dispatch table for this bullshit
	init_file(&file);
	open_file(filename, &file);
	magic = *(uint32_t *)file.file;
	if (magic == MH_MAGIC_64)
		macho64(&file);
	else if (magic == MH_MAGIC)
		macho32(&file);
	else if (magic == FAT_CIGAM)
		fat(&file);
	else if (ft_strncmp(file.file, ARMAG, SARMAG) == 0)
		ar(&file);
	else
		ft_putstr("invalid magic number.");
	close_file(&file);
}

void		print_name(const char *name)
{
	ft_putstr("\n");
	ft_putstr(name);
	ft_putstr(":\n");
}

int			main(int argc, char **argv)
{
	int		i;

	if (argc == 1)
		hanldle_file("a.out");
	else if (argc == 2)
		hanldle_file(argv[1]);
	else
	{
		i = 1;
		while (i < argc)
		{
			print_name(argv[i]);
			hanldle_file(argv[i]);
			i++;
		}

	}
	return (0);
}
