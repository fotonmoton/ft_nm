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

t_segment_command_32	*find_text_command_32
(
	t_load_command *lc,
	uint32_t count
)
{
	uint32_t				i;
	t_segment_command_32	*text_command;

	text_command = NULL;
	i = 0;
	while(i < count)
	{
		if (lc->cmd == LC_SEGMENT &&
            ft_strcmp(((t_segment_command_32 *)lc)->segname, SEG_TEXT) == 0)
		{
			text_command = (t_segment_command_32 *)lc;
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

t_section_32       *find_text_section_32
(
    t_segment_command_32 *text_segment
)
{
    uint32_t        i;
    t_section_32    *text_section;
    t_section_32    *runner;

    i = 0;
    text_section = NULL;
    runner = (t_section_32 *)((uintptr_t)text_segment +
        sizeof(t_segment_command_32));

    while(i < text_segment->nsects)
    {
        if (ft_strcmp(runner->sectname, SECT_TEXT) == 0)
        {
            text_section = runner;
            break;
        }
        runner = (t_section_32 *)((uintptr_t)runner + sizeof(t_section_32));
        i++;
    }
    return (text_section);
}

void                print_text_64
(
    t_nm_mach_64    *mach64,
	t_file *file
)
{
	size_t		text;
	size_t		addr;
    uint64_t	i;
    uint64_t	j;

	text = (size_t)file->file + mach64->text_section->offset;
	addr = mach64->text_section->addr;
	i = 0;
	while(i < mach64->text_section->size)
	{
		print_addr(addr + i, 16);
		ft_putstr("\t");
		j = 0;
		while(i < mach64->text_section->size && j < 16)
		{
			print_addr(*(char *)(text + i), 2);
			ft_putstr(" ");
			j++;
			i++;
		}
		ft_putstr("\n");
	}
}

void                print_text_32
(
    t_nm_mach_32    *mach32,
	t_file *file
)
{
	size_t		text;
	size_t		addr;
    uint64_t	i;
    uint64_t	j;

	text = (size_t)file->file + mach32->text_section->offset;
	addr = mach32->text_section->addr;
	i = 0;
	while(i < mach32->text_section->size)
	{
		print_addr(addr + i, 8);
		ft_putstr("\t");
		j = 0;
		while(i < mach32->text_section->size && j < 8)
		{
			print_addr(*(char *)(text + i), 2);
			ft_putstr(" ");
			j++;
			i++;
		}
		ft_putstr("\n");
	}
}

void                otool64(t_file *file)
{
    t_nm_mach_64	mach64;

	mach64.header = (t_mach_header_64 *)file->file;
	mach64.commands = (t_load_command *)(file->file + sizeof(t_mach_header_64));
	mach64.text_command = find_text_command_64(mach64.commands,
		mach64.header->ncmds);
    mach64.text_section = find_text_section_64(mach64.text_command);
	print_text_64(&mach64, file);
}

void                otool32(t_file *file)
{
    t_nm_mach_32	mach32;

	mach32.header = (t_mach_header_32 *)file->file;
	mach32.commands = (t_load_command *)(file->file + sizeof(t_mach_header_32));
	mach32.text_command = find_text_command_32(mach32.commands,
		mach32.header->ncmds);
    mach32.text_section = find_text_section_32(mach32.text_command);
	print_text_32(&mach32, file);
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
	else if (magic == MH_MAGIC)
		otool32(&file);
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
		ft_putstr("Contents of (__TEXT,__text) section\n");
        hanldle_file(argv[i]);
        i++;
    }
    return (0);
}