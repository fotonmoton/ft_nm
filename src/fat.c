#include "ft_nm.h"
#include "libft.h"
#include <stdio.h>

void	fat(t_nm_file *file)
{
	struct fat_arch		*arch_runner;
	struct fat_header	*header;
	t_nm_file			arch_file;
	uint32_t			i;

	header = (struct fat_header *)file->file;

	if (header->magic != FAT_CIGAM)
	{
		ft_putstr_fd("only 32bit big endian fat binaries supported\n",
			STDERR_FILENO);
		exit(1);
	}
	arch_runner = file->file + sizeof(struct fat_header);
	i = 0;
	while (i < OSSwapInt32(header->nfat_arch))
	{
		if (OSSwapInt32(arch_runner->cputype) == CPU_TYPE_X86_64)
		{
			arch_file.file = file->file + OSSwapInt32(arch_runner->offset);
			arch_file.size = OSSwapInt32(arch_runner->size);
			macho64(&arch_file);
		}
		arch_runner = arch_runner + sizeof(struct fat_arch);
		i++;
	}
}