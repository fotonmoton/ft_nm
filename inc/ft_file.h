#ifndef FT_FILE
# define FT_FILE

typedef struct						s_file
{
	void							*file;
	uint32_t						size;
	int								fd;
}									t_file;

void								init_file
(
	t_file *file
);


void								open_file
(
	const char *filename,
	t_file *file
);

void								close_file
(
	t_file *file
);

#endif