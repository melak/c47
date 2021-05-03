project('wp43s', 'c',
  meson_version   : '>=0.53.2',
  license         : 'GPL-3.0-only',
  default_options : ['warning_level=2']
)

# Simulator and generate tools arguments
add_project_arguments('-DPC_BUILD',   native : true, language : 'c')
if build_machine.system() == 'windows'
  add_project_arguments('-DWIN32',    native : true, language : 'c')
elif build_machine.system() == 'darwin'
  add_project_arguments('-DOSX',      native : true, language : 'c')
elif build_machine.system() == 'linux'
  add_project_arguments('-DLINUX',    native : true, language : 'c')
else
  error('Unsupported platform')
endif

if build_machine.cpu_family() in ['aarch64', 'x86_64']
  add_project_arguments('-DOS64BIT',    native : true, language : 'c')
elif build_machine.cpu_family() in ['x86']
  add_project_arguments('-DOS32BIT',    native : true, language : 'c')
else
  error('Unsupported platform')
endif

if get_option('RASPBERRY')
  add_project_arguments('-DRASPBERRY',  native : true, language : 'c')
endif

# DMCP arguments
add_project_arguments('-DDMCP_BUILD', native : false, language : 'c')
add_project_arguments('-DOS32BIT',    native : false, language : 'c')

git = find_program('git')
run_command(git, 'submodule', 'update', '--init', '--recursive', meson.source_root())

cc = meson.get_compiler('c')
ft_dep  = dependency('freetype2', native : true)
gtk_dep = dependency('gtk+-3.0',  native : true)
gmp_dep = dependency('gmp', native : true)
m_dep   = cc.find_library('m', required : false)

conf_data = configuration_data()
commit_tag = get_option('CI_COMMIT_TAG')
if commit_tag != ''
  today = run_command('date', '+%Y-%m-%d')
  conf_data.set('CI_USE_TAG',    true)
  conf_data.set('CI_COMMIT_TAG', commit_tag)
  conf_data.set('TODAY',         today.stdout().strip())
else
  conf_data.set('CI_COMMIT_TAG', '')
  conf_data.set('TODAY',         '')
endif

subdir('docs/code')
subdir('dep')
subdir('src/wp43s')
subdir('src/ttf2RasterFonts')
subdir('src/generateConstants')
subdir('src/generateCatalogs')
subdir('src/generateTestPgms')
subdir('src/testSuite')
subdir('src/wp43s-gtk')

# Only attempt to include the DMCP target if we have set-up the cross compiler
if meson.is_cross_build()
  add_pgm_chsum = find_program('add_pgm_chsum', dirs : meson.source_root() / 'tools')
  modify_crc    = find_program('modify_crc', dirs : meson.source_root() / 'tools')
  gen_qspi_crc  = find_program('gen_qspi_crc', dirs : meson.source_root() / 'tools')
  arm_objcopy   = find_program('arm-none-eabi-objcopy')
  subproject('gmp-6.2.0')
  arm_gmp_dep   = dependency('arm-gmp-6.2.0', native : false)
  subdir('src/wp43s-dmcp')
endif
