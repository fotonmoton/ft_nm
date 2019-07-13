/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtertysh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/13 14:52:27 by gtertysh          #+#    #+#             */
/*   Updated: 2019/07/13 14:52:29 by gtertysh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

static void	print_addr(void *addr)
{
	size_t	i;
	size_t	bit_4;
	size_t	count;

	count = sizeof(addr) * 2;
	i = 0;
	while (i < count)
	{
		bit_4 = ((size_t)addr >> ((count - i - 1) * 4)) & 0xf;
		if (bit_4 < 10)
			ft_putnbr(bit_4);
		else
			ft_putchar('a' + bit_4 - 10);
		i++;
	}
}

void		open_file(const char *filename, int *fd, char **file)
{
	struct stat stat_buff;

	*fd = open(filename, O_RDONLY);
	if (*fd == -1)
	{
		ft_putstr("can't open file\n");
		exit(1);
	}
	stat(filename, &stat_buff);
	if (!S_ISREG(stat_buff.st_mode))
	{
		ft_putstr("not a regular file\n");
		close(*fd);
		exit(1);
	}
	*file = NULL;
	*file = malloc(stat_buff.st_size);
	if (!*file)
	{
		ft_putstr("can't allocate memory\n");
		close(*fd);
		exit(1);
	}
	read(*fd, *file, stat_buff.st_size);
}

void		close_file(int fd, char *file)
{
	close(fd);
	free(file);
}

void		handle_64(char *file)
{
	struct mach_header_64		*hdr;
	struct load_command			*load_cmd;
	struct symtab_command		*symtab_cmd;
	struct segment_command_64	*segment_cmd;
	
	hdr = (struct mach_header_64 *)file;
	ft_putstr("64 bit mach-o file\n");
	ft_putstr("# of cmds: ");
	ft_putnbr(hdr->ncmds);
	ft_putstr("\n");
	ft_putstr("total cmds memory: ");
	ft_putnbr(hdr->sizeofcmds);
	ft_putstr("\n");
	
	uint32_t load_cmd_offset = sizeof(struct mach_header_64);
	uint32_t i = 0;
	while (i < hdr->ncmds)
	{
		load_cmd = (struct load_command *)(file + load_cmd_offset);
		if (load_cmd->cmd == LC_SEGMENT_64)
		{
			segment_cmd = (struct segment_command_64 *)load_cmd;
			ft_putnbr(i);
			ft_putchar(' ');
			ft_putnbr(segment_cmd->nsects);
			ft_putchar(' ');
			ft_putstr(segment_cmd->segname);
			ft_putchar('\n');
			load_cmd_offset += load_cmd->cmdsize;
			i++;
			continue;
		}
		if (load_cmd->cmd == LC_SYMTAB)
		{
			symtab_cmd = (struct symtab_command *)load_cmd;
			ft_putstr("found symbol table load command\n");
			ft_putstr("# of symbols: ");
			ft_putnbr(symtab_cmd->nsyms);
			ft_putstr("\n");

			uint32_t j = 0;
			struct nlist_64 *symbol_table = (struct nlist_64 *)
				(file + symtab_cmd->symoff);
			struct nlist_64 symbol;
			char *str_table = file + symtab_cmd->stroff;
			while(j < symtab_cmd->nsyms)
			{
				symbol = symbol_table[j];
				int type = symbol.n_type & N_TYPE;
				int external = symbol.n_type & N_EXT;
				int debug = (symbol.n_type & N_STAB) != 0;
				int offset = external ? 0 : 32;
				char *name = str_table + symbol.n_un.n_strx;

				if (debug)
				{
					j++;
					continue;
				}

				// some shit herustic should be used 
				// to determine to print address or not
				if (symbol.n_value)
					print_addr((void *)symbol.n_value);
				else
					ft_putstr("                ");
				ft_putchar(' ');
				if (type == N_UNDF)
					ft_putchar('U' + offset);
				if (type == N_ABS)
					ft_putchar('A' + offset);
				if (type == N_SECT)
				{
					// lookup in which section symbol is located

					ft_putchar('T' + offset);
					ft_putchar(' ');
					ft_putnbr(symbol.n_sect);
				}
				ft_putchar(' ');
				ft_putstr(name);
				ft_putstr("\n");
				j++;
			}
		}
		load_cmd_offset += load_cmd->cmdsize;
		i++;
	}
}

void		handle_32(char *file)
{
	(void)file;
	ft_putstr("TODO\n");
}

void		hanldle_file(const char *filename)
{
	char	*file;
	int		fd;

	open_file(filename, &fd, &file);
	uint32_t magic;

	magic = *(uint32_t *)file;

	if (magic == MH_MAGIC_64)
		handle_64(file);
	else if (magic == MH_MAGIC)
		handle_64(file);
	else if (magic == MH_CIGAM || magic == MH_CIGAM_64)
		ft_putstr("do not support big endian binaries.");
	else
		ft_putstr("invalid magic number.");
	close_file(fd, file);
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
