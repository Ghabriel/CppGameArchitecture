project('CppGameArchitecture', 'cpp', default_options: ['cpp_std=c++17'])

incdir = include_directories('include')

thread_dep = dependency('threads')
sfml_graphics = dependency('sfml-graphics')
sfml_window = dependency('sfml-window')
sfml_system = dependency('sfml-system')
x11 = dependency('x11')

src = [
	'src/engine/game-loop/GameLoop.cpp',
	'src/engine/testing/testing-api.cpp',
	'src/load-resources.cpp',
	'src/main.cpp',
]

executable(
	'main',
	src,
	include_directories: incdir,
	dependencies: [thread_dep, sfml_graphics, sfml_window, sfml_system, x11],
	override_options: ['cpp_std=c++17'],
	cpp_args: '-std=c++17'
)
