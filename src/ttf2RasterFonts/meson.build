font_files = files(
  '../../res/fonts/sortingOrder.csv',
  '../../res/fonts/C47__NumericFont.ttf',
  '../../res/fonts/C47__StandardFont.ttf')
ttf2RasterFonts = executable(
  'ttf2RasterFonts',
  build_by_default : false,
  sources          : files('ttf2RasterFonts.c'),
  dependencies     : ft_dep,
  native           : true)
rasterFontsData_c = custom_target(
  'rasterFontsData.c',
  build_by_default : false,
  input            : font_files,
  output           : 'rasterFontsData.c',
  command          : [ttf2RasterFonts, '@CURRENT_SOURCE_DIR@/../../res/fonts', '@OUTPUT@'])

testTtf2RasterFonts = executable(
  'testTtf2RasterFonts',
  build_by_default    : false,
  sources             : files('testTtf2RasterFonts.c') + [rasterFontsData_c],
  include_directories : [decNumber_inc, wp43_inc],
  dependencies        : [gtk_dep],
  native              : true)
