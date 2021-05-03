sim = executable(
  'wp43s',
  sources             : decNumber_src + wp43s_src + files('wp43s-gtk.c') +
                        [rasterFontsData_c, softmenuCatalogs_h,
                          constantPointers_ch, vcs_h, version_h],
  c_pch               : 'pch/wp43s-gtk_pch.h',
  include_directories : [decNumber_inc, wp43s_inc],
  dependencies        : [gtk_dep, gmp_dep, m_dep],
  native              : true,
  override_options    : ['b_lto=true'])
alias_target('sim', sim)
