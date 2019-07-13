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
#include "ft_nm.h"
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

t_symtab_command *find_symbol_table_command(t_load_command *lc, uint32_t count)
{
	uint32_t			i;
	t_symtab_command	*sym_tab;

	sym_tab = NULL;
	i = 0;
	while(i < count)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym_tab = (t_symtab_command *)lc;
			break;
		}
		lc = (t_load_command *)((uintptr_t)lc + lc->cmdsize);
		i++;
	}
	return sym_tab;
}

t_section_64	*find_section(t_load_command *lc, uint32_t cmd_num, uint32_t section)
{
	uint32_t				i;
	uint32_t				acc;
	t_section_64			*sections;
	t_segment_command_64	*segment;

	acc = 0;
	i = 0;
	while(i < cmd_num)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			segment = (t_segment_command_64 *)lc;
			if (segment->nsects + acc >= section)
			{
				sections = (t_section_64 *)((uintptr_t)segment + sizeof(t_segment_command_64));
				return (sections + section - acc - 1);
			}
			acc += segment->nsects;
		}
		lc = (t_load_command *)((uintptr_t)lc + lc->cmdsize);
		i++;
	}
	return NULL;
}

void		print_symbol_table(t_symtab_command *symtab_cmd, char *file, t_load_command *load_commands, uint32_t cmd_num)
{
	uint32_t		j;
	t_nlist_64		*symbol_table;
	t_nlist_64		symbol;
	t_section_64	*section;
	char			*string_table;

	string_table = file + symtab_cmd->stroff;
	symbol_table = (t_nlist_64 *)(file + symtab_cmd->symoff);
	j = 0;
	while(j < symtab_cmd->nsyms)
	{
		symbol = symbol_table[j];
		int type = symbol.n_type & N_TYPE;
		int external = symbol.n_type & N_EXT;
		int debug = (symbol.n_type & N_STAB) != 0;
		int offset = external ? 0 : 32;
		char *name = string_table + symbol.n_un.n_strx;

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
			section = find_section(load_commands, cmd_num, symbol.n_sect);
			if(ft_strcmp(SECT_TEXT, section->sectname) == 0)
				ft_putchar('T' + offset);
			else if(ft_strcmp(SECT_DATA, section->sectname) == 0)
				ft_putchar('D' + offset);
			else if(ft_strcmp(SECT_BSS, section->sectname) == 0)
				ft_putchar('B' + offset);
			else
				ft_putchar('S' + offset);
			ft_putchar(' ');
			ft_putnbr(symbol.n_sect);
		}
		ft_putchar(' ');
		ft_putstr(name);
		ft_putstr("\n");
		j++;
	}
}

void		handle_64(char *file)
{
	t_mach_header_64		*hdr;
	t_load_command			*load_commands;
	t_symtab_command		*symtab_cmd;
	// t_section_64			*sections;
	// t_segment_command_64	*segment_cmd;

	hdr = (t_mach_header_64 *)file;
	load_commands = (t_load_command *)(file + sizeof(t_mach_header_64));
	// sections = (t_section_64 *)(file + hdr->sizeofcmds + sizeof(t_mach_header_64));
	symtab_cmd = find_symbol_table_command(load_commands, hdr->ncmds);
	print_symbol_table(symtab_cmd, file, load_commands, hdr->ncmds);
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
