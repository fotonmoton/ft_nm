/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtertysh <gtertysh@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 22:37:16 by foton             #+#    #+#             */
/*   Updated: 2019/07/20 15:38:47 by gtertysh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
#define FT_NM_H

#include <mach-o/loader.h>
#include <mach-o/nlist.h>

typedef struct load_command			t_load_command;
typedef struct mach_header_64		t_mach_header_64;
typedef struct symtab_command		t_symtab_command;
typedef struct segment_command_64	t_segment_command_64;
typedef struct nlist_64				t_nlist_64;
typedef struct section_64			t_section_64;

typedef struct						s_nm_file
{
	void							*file;
	uint32_t						size;
	int								fd;
}									t_nm_file;

typedef struct						s_nm_mach_64
{
	t_mach_header_64				*header;
	t_load_command					*commands;
	t_symtab_command				*symbol_table_command;
	t_nlist_64						*symbol_table;
	char							*string_table;
}									t_nm_mach_64;

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

t_symtab_command					*find_symbol_table_command
(
	t_load_command *lc,
	uint32_t count
);

void								print_addr
(
	void *addr
);



#endif
