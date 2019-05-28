# $@ target
# $< 1st dependency
# $^ dependency list
# $? recent dependency list
# $* target without extension

GENERATECONSTANTS_APP = generateConstants$(EXE)
TTF2RASTERFONTS_APP = ttf2RasterFonts$(EXE)
TESTTTF2RASTERFONTS_APP = testTtf2RasterFonts$(EXE)
TESTSUITE_APP = testSuite$(EXE)
WP43S_APP = wp43s$(EXE)
EXE =


ifeq '$(findstring ;,$(PATH))' ';'
    detected_OS := Windows
else
    detected_OS := $(shell uname 2>/dev/null || echo Unknown)
    detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
    detected_OS := $(patsubst MSYS%,MSYS,$(detected_OS))
    detected_OS := $(patsubst MINGW%,MSYS,$(detected_OS))
endif

ifeq ($(detected_OS),Windows)
    EXE = .exe
    CFLAGS += -D WIN32
else ifeq ($(detected_OS),Darwin)        # Mac OS X
    CFLAGS += -D OSX
    LDFLAGS += -L/opt/local/libs
else ifeq ($(detected_OS),Linux)
    CFLAGS += -D LINUX
endif


 
CC = gcc
INC = -IdecNumberICU -Isrc/wp43s -Isrc/testSuite

ifeq ($(DEST), gitlab)
  CFLAGS += -march=x86-64
else
  CFLAGS += -march=native
endif

ifdef DEBUG_WP43S
	CFLAGS += -g
else
	CFLAGS += -O2 -s
endif

CFLAGS += -Wextra -Wall -std=c11 -m64 -fshort-enums -fomit-frame-pointer -DPC_BUILD -DTFM_NO_ASM -DTFM_X86_64 -MMD

LDFLAGS += -m64

SRC_DECIMAL              = \
	$(addprefix decNumberICU/, \
		decContext.c decDouble.c decimal128.c decimal64.c decNumber.c \
		decQuad.c)
OBJ_DECIMAL              = $(SRC_DECIMAL:.c=.o)
DEPS_DECIMAL             = $(SRC_DECIMAL:.c=.d)

SRC_WP43S                = \
	$(addprefix src/wp43s/, \
		assign.c bufferize.c charString.c config.c constantPointers.c \
		constants.c conversionAngles.c conversionUnits.c \
		curveFitting.c dateTime.c debug.c display.c error.c flags.c \
		fonts.c fractions.c gui.c integers.c items.c keyboard.c \
		rasterFontsData.c registerValueConversions.c registers.c \
		saveRestoreCalcState.c screen.c softmenus.c sort.c stack.c \
		stats.c statusBar.c testFunctions.c timer.c tomsfastmath.c \
		wp43s.c) \
	$(addprefix src/wp43s/mathematics/, \
		10pow.c 2pow.c addition.c arccos.c arcsin.c arctan.c \
		changeSign.c comparisonReals.c conjugate.c cos.c cube.c \
		division.c exp.c factorial.c fractionalPart.c gamma.c gcd.c \
		imaginaryPart.c integerPart.c invert.c lcm.c ln.c log10.c \
		log2.c magnitude.c minusOnePow.c multiplication.c power.c \
		realPart.c sign.c sin.c sqrt.c square.c subtraction.c \
		swapRealImaginary.c tan.c toPolar.c toRect.c unitVector.c \
		slvq.c \
		wp34s.c) \
	$(addprefix src/wp43s/logicalOps/, \
		not.c) \
	$(addprefix src/wp43s/browsers/, \
		flagBrowser.c fontBrowser.c registerBrowser.c)
OBJ_WP43S                = $(SRC_WP43S:.c=.o) $(OBJ_DECIMAL)
DEPS_WP43S               = $(SRC_WP43S:.c=.d)

SRC_TESTSUITE            = \
	$(addprefix src/testSuite/, \
		testSuite.c)
OBJ_TESTSUITE            = $(SRC_TESTSUITE:.c=.o) $(OBJ_WP43S:.o=.ts.o)
DEPS_TESTSUITE           = $(OBJ_TESTSUITE:.o=.d)

SRC_GENERATECONSTANTS    = \
	$(addprefix src/generateConstants/, \
		generateConstants.c)
OBJ_GENERATECONSTANTS    = $(SRC_GENERATECONSTANTS:.c=.o) $(OBJ_DECIMAL)
DEPS_GENERATECONSTANTS   = $(SRC_GENERATECONSTANTS:.c=.d)

SRC_TTF2RASTERFONTS      = \
	$(addprefix src/ttf2RasterFonts/, \
		ttf2RasterFonts.c)
OBJ_TTF2RASTERFONTS      = $(SRC_TTF2RASTERFONTS:.c=.o)
DEPS_TTF2RASTERFONTS     = $(SRC_TTF2RASTERFONTS:.c=.d)

SRC_TESTTTF2RASTERFONTS  = \
	$(addprefix src/ttf2RasterFonts/, \
		testTtf2RasterFonts.c)
OBJ_TESTTTF2RASTERFONTS  = $(SRC_TESTTTF2RASTERFONTS:.c=.o) src/wp43s/rasterFontsData.o
DEPS_TESTTTF2RASTERFONTS = $(SRC_TESTTTF2RASTERFONTS:.c=.d)

GEN_SRC_CONSTANTPOINTERS = \
	$(addprefix src/wp43s/, constantPointers.c constantPointers.h)

GEN_SRC_RASTERFONTSDATA   = \
	$(addprefix src/wp43s/, rasterFontsData.c)

GENERATED_SOURCES        = $(GEN_SRC_CONSTANTPOINTERS) $(GEN_SRC_RASTERFONTSDATA)

STAMP_FILES              = .stamp-constantPointers .stamp-rasterFontsData

all: wp43s

rebuild:
	$(MAKE) mrproper
	$(MAKE) all

.PHONY: clean_wp43s clean_generateConstants clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite all clean_all mrproper decNumberICU sources rebuild

ifneq ($(EXE),)
generateConstants: $(GENERATECONSTANTS_APP)
ttf2RasterFonts: $(TTF2RASTERFONTS_APP)
testTtf2RasterFonts: $(TESTTTF2RASTERFONTS_APP)
testSuite: $(TESTSUITE_APP)
wp43s: $(WP43S_APP)

.PHONY: generateConstants ttf2RasterFonts testTtf2RasterFonts testSuite wp43s
endif

sources: $(STAMP_FILES)

clean_all: clean_decNumberICU clean_wp43s clean_generateConstants clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite

mrproper: clean_all
	rm -f $(GENERATED_SOURCES)
	rm -f $(STAMP_FILES)
	rm -f $(GENERATECONSTANTS_APP)
	rm -f $(TTF2RASTERFONTS_APP)
	rm -f $(TESTTTF2RASTERFONTS_APP)
	rm -f $(WP43S_APP)
	rm -f $(TESTSUITE_APP)


clean_decNumberICU:
	rm -f $(OBJ_DECIMAL)
	rm -f $(DEPS_DECIMAL)

-include $(DEPS_DECIMAL)

decNumberICU: $(OBJ_DECIMAL)
	@echo -e "\n====> decNumberICU $@ <===="

decNumberICU/%.o: decNumberICU/%.c
	@echo -e "\n====> decNumberICU $@ <===="
	$(CC) $(CFLAGS) -c -o $@ $<



clean_generateConstants: 
	rm -f $(OBJ_GENERATECONSTANTS)
	rm -f $(DEPS_GENERATECONSTANTS)

-include $(DEPS_GENERATECONSTANTS)

$(GENERATECONSTANTS_APP): $(OBJ_GENERATECONSTANTS)
	@echo -e "\n====> generateConstants $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_GENERATECONSTANTS) -o $@

src/generateConstants/%.o: src/generateConstants/%.c
	@echo -e "\n====> generateConstants $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.stamp-constantPointers: $(GENERATECONSTANTS_APP)
	@echo -e "\n====> running generateConstants <===="
	./$(GENERATECONSTANTS_APP)
	touch $@

$(GEN_SRC_CONSTANTPOINTERS): .stamp-constantPointers


clean_ttf2RasterFonts:
	rm -f $(OBJ_TTF2RASTERFONTS)
	rm -f $(DEPS_TTF2RASTERFONTS)

-include $(DEPS_TTF2RASTERFONTS)

$(TTF2RASTERFONTS_APP): $(OBJ_TTF2RASTERFONTS)
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_TTF2RASTERFONTS) -o $@ `pkg-config --libs freetype2`

src/ttf2RasterFonts/%.o: src/ttf2RasterFonts/%.c
	@echo -e "\n====> ttf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) $(INC) `pkg-config --cflags freetype2` `pkg-config --cflags gtk+-3.0` -c -o $@ $<

.stamp-rasterFontsData: $(TTF2RASTERFONTS_APP) fonts/WP43S_NumericFont.ttf fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> running ttf2RasterFonts <===="
	./$(TTF2RASTERFONTS_APP) > /dev/null
	touch $@

$(GEN_SRC_RASTERFONTSDATA): .stamp-rasterFontsData

clean_testTtf2RasterFonts:
	rm -f $(OBJ_TESTTTF2RASTERFONTS)
	rm -f $(DEPS_TESTTTF2RASTERFONTS)

-include $(DEPS_TESTTTF2RASTERFONTS)

$(TESTTTF2RASTERFONTS_APP): $(OBJ_TESTTTF2RASTERFONTS)
	@echo -e "\n====> testTtf2RasterFonts $@ <===="
	$(CC) $(CFLAGS) -m64 `pkg-config --libs freetype2` $(OBJ_TESTTTF2RASTERFONTS) -o $@

src/ttf2RasterFonts/testTtf2RasterFonts.o: .stamp-constantPointers

clean_testSuite:
	rm -f $(OBJ_TESTSUITE)
	rm -f $(DEPS_TESTSUITE) $(DEPS_WP43S)

-include $(DEPS_TESTSUITE)

$(TESTSUITE_APP): CFLAGS += -DTESTSUITE_BUILD
$(TESTSUITE_APP): $(OBJ_TESTSUITE)
	@echo -e "\n====> testSuite $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_TESTSUITE) -o $(TESTSUITE_APP)

src/testSuite/%.o: src/testSuite/%.c .stamp-constantPointers
	@echo -e "\n====> testSuite $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

src/wp43s/%.ts.o: src/wp43s/%.c .stamp-constantPointers
	@echo -e "\n====> wp43s (testSuite) $@ <===="
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

decNumberICU/%.ts.o: decNumberICU/%.c
	@echo -e "\n====> decNumberICU (testSuite) $@ <===="
	$(CC) $(CFLAGS) -c -o $@ $<



clean_wp43s: 
	rm -f $(OBJ_WP43S)
	rm -f $(DEPS_WP43S)

-include $(DEPS_WP43S)

$(WP43S_APP): GTK_LIBS   = `pkg-config --libs   gtk+-3.0`
$(WP43S_APP): GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
$(WP43S_APP): $(OBJ_WP43S)
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) -m64 $(OBJ_WP43S) -o $(WP43S_APP) $(GTK_LIBS)

src/wp43s/%.o: src/wp43s/%.c .stamp-constantPointers
	@echo -e "\n====> wp43s $@ <===="
	$(CC) $(CFLAGS) $(INC) $(GTK_CFLAGS) -c -o $@ $<

src/wp43s/rasterFontsData.o: .stamp-rasterFontsData

