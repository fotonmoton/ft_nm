#include "libft.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>
#include <stdio.h>

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
	Elf64_Ehdr *elf_header;

	elf_header = (Elf64_Ehdr *)file;
	if (elf_header->e_ident[EI_MAG0] != ELFMAG0 || 
		elf_header->e_ident[EI_MAG1] != ELFMAG1 ||
		elf_header->e_ident[EI_MAG2] != ELFMAG2)
	{
		ft_putstr("not a valid magic number for elf binary file\n");
		return (1);
	}
	if (elf_header->e_ident[EI_CLASS] != ELFCLASS64)
	{
		ft_putstr("sorry, only 64bit elf binaries for now");
		return (1);
	}
	Elf64_Shdr	*section_table;
	Elf64_Shdr	*section_names;
	Elf64_Shdr	*section_entity;
	Elf64_Sym	*symbol_table;
	char		*symbol_names;
	int			symbol_table_size;
	int			symbol_table_entity_size;
	
	section_table = (Elf64_Shdr *)(file + elf_header->e_shoff);
	section_names = section_table + elf_header->e_shstrndx;
	char *string;
	int i = 0;
	while (i < elf_header->e_shnum)
	{
		section_entity = section_table + i;
		string = file + section_names->sh_offset + section_entity->sh_name;
		printf("%d entity name: %s type: %d\n", i, string, section_entity->sh_type);
		if (section_entity->sh_type == SHT_SYMTAB)
		{
			printf("found symbol table! index: %d\n", i);
			symbol_table = (Elf64_Sym *)(file + section_entity->sh_offset);
			symbol_table_size = section_entity->sh_size;
			symbol_table_entity_size = section_entity->sh_entsize;
		}
		if (section_entity->sh_type == SHT_STRTAB && i != elf_header->e_shstrndx)
		{
			printf("found symbol names! index: %d\n", i);
			symbol_names = file + section_entity->sh_offset;
		}
		i++;
	}
	int j = 0;
	Elf64_Sym *symbol;
	while (symbol_table_size > 0)
	{
		symbol = symbol_table + j;
		string = symbol_names + symbol->st_name;
		printf("%d: %s\n", j, string);
		symbol_table_size -= symbol_table_entity_size;
		j++; 
	}
	return(0);
}
