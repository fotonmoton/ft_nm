/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtertysh <gtertysh@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 22:37:16 by foton             #+#    #+#             */
/*   Updated: 2019/07/27 18:10:09 by gtertysh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
#define FT_NM_H

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <stddef.h>

typedef struct symtab_command		t_symtab_command;
typedef struct load_command			t_load_command;

typedef struct mach_header_64		t_mach_header_64;
typedef struct mach_header			t_mach_header_32;
typedef struct segment_command_64	t_segment_command_64;
typedef struct segment_command		t_segment_command_32;
typedef struct nlist_64				t_nlist_64;
typedef struct nlist				t_nlist_32;
typedef struct section_64			t_section_64;
typedef struct section				t_section_32;


typedef struct						s_nm_file
{
	void							*file;
	uint32_t						size;
	int								fd;
	const char						*filename;
}									t_nm_file;

typedef struct						s_nm_mach_64
{
	t_mach_header_64				*header;
	t_load_command					*commands;
	t_symtab_command				*symbol_table_command;
	t_nlist_64						*symbol_table;
	char							*string_table;
}									t_nm_mach_64;

typedef struct						s_nm_mach_32
{
	t_mach_header_32				*header;
	t_load_command					*commands;
	t_symtab_command				*symbol_table_command;
	t_nlist_32						*symbol_table;
	char							*string_table;
}									t_nm_mach_32;

void								init_file
(
	t_nm_file *file
);


void								open_file
(
	const char *filename,
	t_nm_file *file
);

void								close_file
(
	t_nm_file *file
);


void								macho64
(
	t_nm_file *file
);

void								macho32
(
	t_nm_file *file
);

t_symtab_command					*find_symbol_table_command
(
	t_load_command *lc,
	uint32_t count
);

void								print_addr
(
	size_t addr
);

void								print_addr_32
(
	size_t addr
);




#endif
