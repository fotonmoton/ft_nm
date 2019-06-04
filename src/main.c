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
	Elf64_Shdr *section_array;
	Elf64_Shdr *strings_table;
	
	section_array = (Elf64_Shdr *)(file + elf_header->e_shoff);
	strings_table = section_array + elf_header->e_shstrndx;
	char *string;
	int i = 1;
	while (i < elf_header->e_shnum)
	{
		section_array = section_array + i;
		string = (char *)(file + strings_table->sh_offset + section_array->sh_name);
		printf("entity name: %s\n", string);
		i++;
	}
	ft_putstr("symbol table\n");
	return(0);
}
