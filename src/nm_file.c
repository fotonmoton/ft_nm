
#include "ft_nm.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/mman.h>
#include <stdio.h>

void	init_file(t_nm_file *file)
{
	file->fd = -1;
	file->file = NULL;
	file->size = 0;
}

void	open_file(const char *filename, t_nm_file *file)
{
	struct stat stat_buff;

	file->fd = open(filename, O_RDONLY);
	if (file->fd == -1)
	{
		ft_putstr("can't open file\n");
		exit(1);
	}
	stat(filename, &stat_buff);
	if (!S_ISREG(stat_buff.st_mode))
	{
		ft_putstr("not a regular file\n");
		close(file->fd);
		exit(1);
	}
	file->size = stat_buff.st_size;
	file->file = mmap(0, file->size, PROT_READ, MAP_PRIVATE, file->fd, 0);
	if (file->file == MAP_FAILED)
	{
		perror("can't mmap file");
		close(file->fd);
		exit(1);
	}
}

void	close_file(t_nm_file *file)
{
	close(file->fd);
	munmap(file->file, file->size);
}