#include "libft.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>

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
	Elf64_Ehdr elf_header;
	read(fd, &elf_header, sizeof(Elf64_Ehdr));
	if (elf_header.e_ident[EI_MAG0] != ELFMAG0 || 
		elf_header.e_ident[EI_MAG1] != ELFMAG1 ||
		elf_header.e_ident[EI_MAG2] != ELFMAG2)
	{
		ft_putstr("not a valid magic number for elf binary file\n");
		return (1);
	}
	if (elf_header.e_ident[EI_CLASS] != ELFCLASS64)
	{
		ft_putstr("sorry, only 64bit elf binaries for now");
		return (1);
	}
	ft_putstr("symbol table\n");
	return(0);
}
