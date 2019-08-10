#include "ft_nm.h"
#include "libft.h"
#include <ar.h>


void	ar(t_file *file)
{
	struct ar_hdr	*runner;
	char			*long_name;
	size_t			offset;
	t_file		obj;


	runner = file->file + SARMAG;
	while ((void *)runner < file->file + file->size)
	{
		if (ft_strncmp(runner->ar_name, AR_EFMT1, 3) == 0)
		{
			offset = ft_atoi(&runner->ar_name[3]);
			long_name = (char *)runner + sizeof(struct ar_hdr);
			ft_putstr("\n");
			ft_putstr(long_name);
			ft_putstr("\n");
			obj.file = (void *)((size_t)runner + sizeof(struct ar_hdr) + offset);
			obj.size = ft_atoi(runner->ar_size) - offset;
			if (*(uint32_t *)obj.file == MH_MAGIC_64)
				macho64(&obj);
		}
		runner = (struct ar_hdr *)((size_t)runner + ft_atoi(runner->ar_size) + sizeof(struct ar_hdr));
	}
}