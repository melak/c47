testSuite_src = files(
  'testSuite.c',
  'hal/audio.c',
  'hal/gui.c',
  'hal/io.c')
testSuite = executable(
  'testSuite',
  build_by_default    : false,
  sources             : decNumber_src + wp43_src + testSuite_src +
                        [rasterFontsData_c, softmenuCatalogs_h,
                          constantPointers_ch],
  c_args              : '-DTESTSUITE_BUILD',
  include_directories : [decNumber_inc, wp43_inc],
  dependencies        : [gtk_dep, gmp_dep, m_dep],
  native              : true)
test(
  'testSuite',
  testSuite,
  args    : [files('tests/testSuiteList.txt')],
  workdir : meson.project_source_root(),
  timeout : 120)
