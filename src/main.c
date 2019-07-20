/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtertysh <gtertysh@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/13 14:52:27 by gtertysh          #+#    #+#             */
/*   Updated: 2019/07/20 15:21:14 by gtertysh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nm.h"


void		hanldle_file(const char *filename)
{
	t_nm_file	file;
	uint32_t	magic;

	init_file(&file);
	open_file(filename, &file);
	magic = *(uint32_t *)file.file;
	if (magic == MH_MAGIC_64)
		macho64(&file);
	// else if (magic == MH_MAGIC)
	// 	handle_32(file);
	// else if (magic == MH_CIGAM || magic == MH_CIGAM_64)
	// 	ft_putstr("do not support big endian binaries.");
	else
		ft_putstr("invalid magic number.");
	close_file(&file);
}

int			main(int argc, char **argv)
{
	int		i;

	if (argc == 1)
		hanldle_file("a.out");
	else
	{
		i = 1;
		while (i < argc)
			 hanldle_file(argv[i++]);
	}
	return (0);
}
