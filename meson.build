project(
	'malloc',
	'c',
	default_options: [
		'buildtype=plain',
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
  'src/main.c'
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
