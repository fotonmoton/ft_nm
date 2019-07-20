#include "ft_nm.h"
#include "libft.h"
#include <stddef.h>

t_section_64			*find_section
(
	t_nm_mach_64 *mach64,
	uint32_t index
)
{
	uint32_t				i;
	uint32_t				acc;
	t_load_command			*lc;
	t_section_64			*sections;
	t_segment_command_64	*segment;

	lc = mach64->commands;
	acc = 0;
	i = 0;
	while(i < mach64->header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			segment = (t_segment_command_64 *)lc;
			if (segment->nsects + acc >= index)
			{
				sections = (t_section_64 *)((uintptr_t)segment +
					sizeof(t_segment_command_64));
				return (sections + index - acc - 1);
			}
			acc += segment->nsects;
		}
		lc = (t_load_command *)((uintptr_t)lc + lc->cmdsize);
		i++;
	}
	return NULL;
}

void				print_symbol_table(t_nm_mach_64 *mach64)
{
	uint32_t		j;
	t_nlist_64		symbol;
	t_section_64	*section;

	j = 0;

	while(j < mach64->symbol_table_command->nsyms)
	{
		symbol = mach64->symbol_table[j];
		int type = symbol.n_type & N_TYPE;
		int external = symbol.n_type & N_EXT;
		int debug = (symbol.n_type & N_STAB) != 0;
		int offset = external ? 0 : 32;
		char *name = mach64->string_table + symbol.n_un.n_strx;

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
			section = find_section(mach64, symbol.n_sect);
			if(ft_strcmp(SECT_TEXT, section->sectname) == 0)
				ft_putchar('T' + offset);
			else if(ft_strcmp(SECT_DATA, section->sectname) == 0)
				ft_putchar('D' + offset);
			else if(ft_strcmp(SECT_BSS, section->sectname) == 0)
				ft_putchar('B' + offset);
			else
				ft_putchar('S' + offset);
		}
		ft_putchar(' ');
		ft_putstr(name);
		ft_putstr("\n");
		j++;
	}
}

void				macho64(t_nm_file *file)
{
	t_nm_mach_64	mach64;

	mach64.header = (t_mach_header_64 *)file->file;
	mach64.commands = (t_load_command *)(file->file + sizeof(t_mach_header_64));
	mach64.symbol_table_command = find_symbol_table_command(mach64.commands,
		mach64.header->ncmds);
	mach64.symbol_table = (t_nlist_64 *)
		(mach64.symbol_table_command->symoff + file->file);
	mach64.string_table = (char *)
		(mach64.symbol_table_command->stroff + file->file);
	ft_putstr("\n");
	ft_putstr(file->filename);
		ft_putstr(":\n");
	print_symbol_table(&mach64);
}