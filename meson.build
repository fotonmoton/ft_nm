project(
	'malloc',
	'c',
	default_options: [
		'buildtype=plain',
		'strip=true',
	],
)

add_global_arguments(
	'-O0',
	'-Wall',
	'-Wextra',
	'-Werror',
	'-Wwrite-strings',
	'-Wunreachable-code',
	'-Winit-self',
	'-Wmissing-field-initializers',
	'-Wno-unknown-pragmas',
	'-Wstrict-prototypes',
	'-Wundef',
	'-Wold-style-definition',
	language: 'c',
)

inc = include_directories('inc')

install_headers('inc/ft_nm.h')

sources = [
  'src/nm/main.c',
  'src/nm/nm_file.c',
  'src/nm/macho64.c',
  'src/nm/macho32.c',
  'src/nm/fat.c',
  'src/nm/ar.c',
  'src/nm/print_address.c',
  'src/nm/find_symbol_table_command.c',
]

libft = subproject('libft')

libft_dep = libft.get_variable('libft_dep')


ft_nm = executable(
	'ft_nm',
	sources,
	include_directories: inc,
	dependencies: libft_dep,
	install: true,
)

#subdir('t')
