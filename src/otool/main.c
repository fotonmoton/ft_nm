#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <ar.h>
#include "ft_file.h"
#include "ft_nm.h"
#include "libft.h"

t_segment_command_64	*find_text_command_64
(
	t_load_command *lc,
	uint32_t count
)
{
	uint32_t			i;
	t_segment_command_64	*text_command;

	text_command = NULL;
	i = 0;
	while(i < count)
	{
		if (lc->cmd == LC_SEGMENT_64 && 
            ft_strcmp(((t_segment_command_64 *)lc)->segname, SEG_TEXT) == 0)
		{
			text_command = (t_segment_command_64 *)lc;
			break;
		}
		lc = (t_load_command *)((uintptr_t)lc + lc->cmdsize);
		i++;
	}
	return text_command;
}

t_section_64       *find_text_section_64
(
    t_segment_command_64 *text_segment
)
{
    uint32_t        i;
    t_section_64    *text_section;
    t_section_64    *runner;

    i = 0;
    text_section = NULL;
    runner = (t_section_64 *)((uintptr_t)text_segment + 
        sizeof(t_segment_command_64));

    while(i < text_segment->nsects)
    {
        if (ft_strcmp(runner->sectname, SECT_TEXT) == 0)
        {
            text_section = runner;
            break;
        }
        runner = (t_section_64 *)((uintptr_t)runner + sizeof(t_section_64));
        i++;
    }
    return (text_section);
}

void                print_text_64
(
    t_nm_mach_64    *mach64
)
{
    ft_putstr(mach64->text_section->sectname);
    ft_putstr("\n section size: ");
    ft_putnbr(mach64->text_section->size);
    ft_putstr("\n");
}

void                otool64(t_file *file)
{
    t_nm_mach_64	mach64;

	mach64.header = (t_mach_header_64 *)file->file;
	mach64.commands = (t_load_command *)(file->file + sizeof(t_mach_header_64));
	mach64.text_command = find_text_command_64(mach64.commands,
		mach64.header->ncmds);
    mach64.text_section = find_text_section_64(mach64.text_command);
	print_text_64(&mach64);
}

void		hanldle_file(const char *filename)
{
	t_file	file;
	uint32_t	magic;

	// create dispatch table for this bullshit
	init_file(&file);
	open_file(filename, &file);
	magic = *(uint32_t *)file.file;
	if (magic == MH_MAGIC_64)
		otool64(&file);
	// else if (magic == MH_MAGIC)
	// 	macho32(&file);
	// else if (magic == FAT_CIGAM)
	// 	fat(&file);
	// else if (ft_strncmp(file.file, ARMAG, SARMAG) == 0)
	// 	ar(&file);
	else
		ft_putstr("invalid magic number.");
	close_file(&file);
}

int     main(int argc, char **argv)
{
    int     i;

    if (argc == 1)
    {
        ft_putstr_fd("at least one file must be specified\n", STDERR_FILENO);
        return (1);
    }

    i = 1;
    while(i < argc)
    {
        ft_putstr(argv[i]);
        ft_putstr(":\n");
        hanldle_file(argv[i]);
        i++;
    }
    return (0);
}