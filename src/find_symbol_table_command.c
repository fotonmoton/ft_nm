#include "ft_nm.h"
#include "stddef.h"

t_symtab_command	*find_symbol_table_command
(
	t_load_command *lc,
	uint32_t count
)
{
	uint32_t			i;
	t_symtab_command	*sym_tab;

	sym_tab = NULL;
	i = 0;
	while(i < count)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym_tab = (t_symtab_command *)lc;
			break;
		}
		lc = (t_load_command *)((uintptr_t)lc + lc->cmdsize);
		i++;
	}
	return sym_tab;
}