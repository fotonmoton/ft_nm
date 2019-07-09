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

int		main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr("usage\n");
		return (1);
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_putstr("can't open file\n");
		return (1);
	}
	struct stat stat_buff;
	stat(argv[1], &stat_buff);
	if (!S_ISREG(stat_buff.st_mode))
	{
		ft_putstr("not a regular file\n");
		return (1);
	}
	char *file;
	file = malloc(stat_buff.st_size);
	read(fd, file, stat_buff.st_size);

	uint32_t magic = *(uint32_t *)file;

	if (magic != MH_MAGIC && magic != MH_MAGIC_64)
	{
		ft_putstr("not a mach-o file\n");
		return (1);
	}

	if (magic == MH_MAGIC)
		ft_putstr("32 bit mach-o file\n");
	
	if (magic == MH_MAGIC_64)
	{
		struct mach_header_64 *hdr = (struct mach_header_64 *)file;
		// ft_putstr("64 bit mach-o file\n");
		// ft_putstr("# of cmds: ");
		// ft_putnbr(hdr->ncmds);
		// ft_putstr("\n");
		// ft_putstr("total cmds memory: ");
		// ft_putnbr(hdr->sizeofcmds);
		// ft_putstr("\n");
		struct load_command *load_cmd;
		struct symtab_command *symtab_cmd;
		uint32_t load_cmd_offset = sizeof(struct mach_header_64);
		uint32_t i = 0;
		while (i < hdr->ncmds)
		{
			load_cmd = (struct load_command *)(file + load_cmd_offset);
			if (load_cmd->cmd == LC_SYMTAB)
			{
				symtab_cmd = (struct symtab_command *)load_cmd;
				// ft_putstr("found symbol table load command\n");
				// ft_putstr("# of symbols: ");
				// ft_putnbr(symtab_cmd->nsyms);
				// ft_putstr("\n");

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
						// lookup in which section symbol is located
						ft_putchar('T' + offset);
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
}
