#include "ft_nm.h"
#include "libft.h"
#include <stddef.h>

static t_section_32			*find_section
(
	t_nm_mach_32 *mach32,
	uint32_t index
)
{
	uint32_t				i;
	uint32_t				acc;
	t_load_command			*lc;
	t_section_32			*sections;
	t_segment_command_32	*segment;

	lc = mach32->commands;
	acc = 0;
	i = 0;
	while(i < mach32->header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			segment = (t_segment_command_32 *)lc;
			if (segment->nsects + acc >= index)
			{
				sections = (t_section_32 *)((uintptr_t)segment +
					sizeof(t_segment_command_32));
				return (sections + index - acc - 1);
			}
			acc += segment->nsects;
		}
		lc = (t_load_command *)((uintptr_t)lc + lc->cmdsize);
		i++;
	}
	return NULL;
}

static void				print_symbol_table(t_nm_mach_32 *mach32)
{
	uint32_t		j;
	t_nlist_32		symbol;
	t_section_32	*section;

	j = 0;

	while(j < mach32->symbol_table_command->nsyms)
	{
		symbol = mach32->symbol_table[j];
		int type = symbol.n_type & N_TYPE;
		int external = symbol.n_type & N_EXT;
		int debug = (symbol.n_type & N_STAB) != 0;
		int offset = external ? 0 : 32;
		char *name = mach32->string_table + symbol.n_un.n_strx;

		if (debug)
		{
			j++;
			continue;
		}

		// some shit herustic should be used
		// to determine to print address or not
		if (symbol.n_value)
			print_addr(symbol.n_value, 8);
		else
			ft_putstr("        ");
		ft_putchar(' ');
		if (type == N_UNDF)
			ft_putchar('U' + offset);
		if (type == N_ABS)
			ft_putchar('A' + offset);
		if (type == N_SECT)
		{
			section = find_section(mach32, symbol.n_sect);
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

void				macho32(t_file *file)
{
	t_nm_mach_32	mach32;

	mach32.header = (t_mach_header_32 *)file->file;
	mach32.commands = (t_load_command *)(file->file + sizeof(t_mach_header_32));
	mach32.symbol_table_command = find_symbol_table_command(mach32.commands,
		mach32.header->ncmds);
	mach32.symbol_table = (t_nlist_32 *)
		(mach32.symbol_table_command->symoff + file->file);
	mach32.string_table = (char *)
		(mach32.symbol_table_command->stroff + file->file);
	print_symbol_table(&mach32);
}