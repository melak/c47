generateTestPgms = executable(
  'generateTestPgms',
  build_by_default    : false,
  sources             : files('generateTestPgms.c'),
  include_directories : [decNumber_inc, wp43s_inc],
  dependencies        : gtk_dep,
  native              : true)
testPgms_bin = custom_target(
  'testPgms.bin',
  build_by_default : false,
  output           : 'testPgms.bin',
  command          : [generateTestPgms, '@OUTPUT@'])
alias_target('testPgms', testPgms_bin)
