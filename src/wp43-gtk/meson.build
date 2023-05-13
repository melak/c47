wp43_gtk_src = files(
  'wp43-gtk.c',
  'gtkGui.c',
  'hal/audio.c',
  'hal/gui.c')

override_opts = ['b_coverage=false']
cflags        = []
linkargs      = []
wp43_gtk_rc   = []
if get_option('buildtype').startswith('release')
  override_opts += ['b_lto=true']
else
  cflags        += ['-fstack-protector-strong']
  linkargs      += ['-fstack-protector-strong']
  # Sanitizers not supported on MinGW
  if (host_machine.system() == 'linux' or host_machine.system() == 'darwin')
    override_opts += ['b_sanitize=address,undefined']
  endif
endif

if (target_machine.system() == 'windows')
  windows = import('windows')
  wp43_gtk_rc += windows.compile_resources('wp43-gtk.rc', depend_files: [version_h], depends: [vcs_h], include_directories: [wp43_inc])
endif

sim = executable(
  'wp43',
  sources             : decNumber_src + wp43_src + wp43_gtk_src + wp43_gtk_rc +
                        [rasterFontsData_c, softmenuCatalogs_h,
                          constantPointers_ch, vcs_h, version_h],
  c_pch               : 'pch/wp43-gtk_pch.h',
  include_directories : [decNumber_inc, wp43_inc],
  dependencies        : [gtk_dep, gmp_dep, m_dep, pulse_dep],
  native              : true,
  override_options    : ['b_lto=true'],
  win_subsystem       : 'windows')
alias_target('sim', sim)
