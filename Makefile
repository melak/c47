# $@ target
# $< 1st dependency
# $^ dependency list with path
# $+ like $^ without duplicates
# $? out of date dependency list
# $* target without extension

# Path to aux build scripts
TOOLS_DIR = tools
BUILD_DIR = build
GENERATECONSTANTS_APP = $(BUILD_DIR)/generateConstants/generateConstants$(EXE)
GENERATECATALOGS_APP = $(BUILD_DIR)/generateCatalogs/generateCatalogs$(EXE)
GENERATETESTPGMS_APP = $(BUILD_DIR)/generateTestPgms/generateTestPgms$(EXE)
TTF2RASTERFONTS_APP = $(BUILD_DIR)/ttf2RasterFonts/ttf2RasterFonts$(EXE)
TESTTTF2RASTERFONTS_APP = testTtf2RasterFonts$(EXE)
TESTSUITE_APP = testSuite$(EXE)
WP43S_APP = wp43s$(EXE)
WP43S_DMCP = WP43S
EXE =

CFLAGS += -Wextra -Wall -MMD
SIM_CFLAGS += -std=c11
DMCP_CFLAGS += -Wno-unused-parameter

C_INCLUDES = -Idep/decNumberICU -I$(BUILD_DIR)/generated -Isrc/wp43s -Isrc/testSuite
DMCP_INCLUDES = -Idep/DMCP_SDK/dmcp -Idep/gmp-6.2.0 -Isrc/wp43s-dmcp

# Simulator CFLAGS and binaries configuration

CC = gcc

ifeq ($(OS),Windows_NT)
  EXE = .exe
  SIM_CFLAGS += -DWIN32
  detected_OS := Windows
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    detected_OS := Linux
    SIM_CFLAGS += -DLINUX
  endif
  ifeq ($(UNAME_S),Darwin)
    detected_OS := Darwin
    CC = clang
    SIM_CFLAGS += -DOSX
    ifneq ($(wildcard /opt/homebrew/.),)
      # Homebrew on Arm Macs installs into /opt/homebrew
      SIM_CFLAGS += -I/opt/homebrew/include
      SIM_LDFLAGS += -L/opt/homebrew/lib
    else
      SIM_CFLAGS += -I/usr/local/include/
      SIM_LDFLAGS += -L/usr/local/lib
    endif
  endif
endif

RASPBERRY = $(shell $(TOOLS_DIR)/onARaspberry)

ifeq ($(DEST), gitlab)
	SIM_CFLAGS += -march=x86-64
else ifeq ($(detected_OS),Darwin)
	# Don't add -march=native on macOS as it isn't supported in clang on Arm
else
	SIM_CFLAGS += -march=native
endif

ifdef DEBUG_WP43S
	SIM_CFLAGS += -g
else
	SIM_CFLAGS += -O2
endif

SIM_CFLAGS += -DPC_BUILD

ifeq ($(RASPBERRY),YES32)
        SIM_CFLAGS += -DOS32BIT -DRASPBERRY
endif

ifeq ($(RASPBERRY),YES64)
        SIM_CFLAGS += -DOS64BIT -DRASPBERRY -fshort-enums
endif

ifeq ($(RASPBERRY),NO32)
        SIM_CFLAGS += -DOS32BIT
endif

ifeq ($(RASPBERRY),NO64)
        SIM_CFLAGS += -DOS64BIT -fshort-enums -m64
        SIM_LDFLAGS += -m64
endif

SIM_CFLAGS += `pkg-config --cflags freetype2` `pkg-config --cflags gtk+-3.0`
SIM_LDFLAGS += -lgmp -lm `pkg-config --libs freetype2` `pkg-config --libs gtk+-3.0`

# DMCP CFLAGS and binaries configuration

# WP43S related defines
DMCP_CFLAGS += -DDMCP_BUILD -DOS32BIT -DUSE_HAL_DRIVER -DSTM32L476xx -DARM

# Libraries
GMPLIB = dep/gmp-6.2.0/.libs/libgmp.a
LIBS += $(GMPLIB)

#######################################
# binaries
#######################################
DMCP_CC = arm-none-eabi-gcc
DMCP_AS = arm-none-eabi-gcc -x assembler-with-cpp
DMCP_OBJCOPY = arm-none-eabi-objcopy
DMCP_SIZE = arm-none-eabi-size

#######################################
# CFLAGS
#######################################
# macros for gcc
DMCP_CFLAGS += -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))"
DMCP_CPUFLAGS += -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nosys.specs

# compile gcc flags
DMCP_ASFLAGS = $(DMCP_CPUFLAGS) -Wall -fdata-sections -ffunction-sections
DMCP_CFLAGS += $(DMCP_CPUFLAGS) $(C_INCLUDES) $(DMCP_INCLUDES) -fdata-sections -ffunction-sections

ifdef DEBUG_WP43S
DMCP_CFLAGS += -O0 -DDEBUG
else
DMCP_CFLAGS += -Os -fmerge-all-constants -s -fomit-frame-pointer
endif

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = src/wp43s-dmcp/stm32_program.ld
DMCP_LDFLAGS = $(DMCP_CPUFLAGS) -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/dmcp/$(WP43S_DMCP).map,--cref -Wl,--gc-sections -Wl,--wrap=_malloc_r

# Common set-up

SRC_DECIMAL              = $(addprefix dep/decNumberICU/, decContext.c decDouble.c decimal128.c decimal64.c decNumber.c decQuad.c)

GEN_SRC_CONSTANTPOINTERS = $(addprefix $(BUILD_DIR)/generated/, constantPointers.c constantPointers.h)
GEN_SRC_RASTERFONTSDATA  = $(addprefix $(BUILD_DIR)/generated/, rasterFontsData.c)
GEN_SRC_SOFTMENUCATALOGS = $(addprefix $(BUILD_DIR)/generated/, softmenuCatalogs.h)
GEN_BIN_TESTPGMS         = $(addprefix res/dmcp/, testPgms.bin)

GENERATED_SOURCES        = $(GEN_SRC_CONSTANTPOINTERS) $(GEN_SRC_RASTERFONTSDATA) $(GEN_SRC_SOFTMENUCATALOGS) $(GEN_BIN_TESTPGMS)
GENERATED_SRC_NOT_H      = $(addprefix $(BUILD_DIR)/generated/, constantPointers.c rasterFontsData.c)

SRC_WP43S                = $(SRC_DECIMAL) \
                           $(sort $(wildcard src/wp43s/*.c) \
                           $(wildcard src/wp43s/mathematics/*.c) \
                           $(wildcard src/wp43s/browsers/*.c) \
                           $(wildcard src/wp43s/logicalOps/*.c) \
                           $(wildcard src/wp43s/programming/*.c) \
                           $(wildcard src/wp43s/distributions/*.c) \
                           $(wildcard src/wp43s/ui/*.c) \
                           $(GENERATED_SRC_NOT_H) )

SRC_SIMULATOR            = $(SRC_WP43S) \
                           $(wildcard src/wp43s-gtk/*.c)
OBJ_SIMULATOR            = $(addprefix $(BUILD_DIR)/simulator/,$(notdir $(SRC_SIMULATOR:.c=.o)))
DEPS_SIMULATOR           = $(addprefix $(BUILD_DIR)/simulator/,$(notdir $(SRC_SIMULATOR:.c=.d)))

SRC_DMCP                 = $(SRC_WP43S) \
                           dep/DMCP_SDK/dmcp/sys/pgm_syscalls.c
ASM_DMCP                 = dep/DMCP_SDK/dmcp/startup_pgm.s
OBJ_DMCP                 = $(addprefix $(BUILD_DIR)/dmcp/,$(notdir $(SRC_DMCP:.c=.o))) \
                           $(addprefix $(BUILD_DIR)/dmcp/,$(notdir $(ASM_DMCP:.s=.o)))
DEPS_DMCP                = $(addprefix $(BUILD_DIR)/dmcp/,$(notdir $(SRC_DMCP:.c=.d)))

SRC_TESTSUITE            = $(SRC_WP43S) \
                           $(addprefix src/testSuite/, testSuite.c)
OBJ_TESTSUITE            = $(addprefix $(BUILD_DIR)/testSuite/,$(notdir $(SRC_TESTSUITE:.c=.o)))
DEPS_TESTSUITE           = $(addprefix $(BUILD_DIR)/testSuite/,$(notdir $(SRC_TESTSUITE:.c=.d)))

SRC_GENERATECONSTANTS    = $(SRC_DECIMAL) \
                           $(addprefix src/generateConstants/, generateConstants.c)
OBJ_GENERATECONSTANTS    = $(addprefix $(BUILD_DIR)/generateConstants/,$(notdir $(SRC_GENERATECONSTANTS:.c=.o)))
DEPS_GENERATECONSTANTS   = $(addprefix $(BUILD_DIR)/generateConstants/,$(notdir $(SRC_GENERATECONSTANTS:.c=.d)))

SRC_GENERATECATALOGS     = $(addprefix src/generateCatalogs/, generateCatalogs.c) \
                           $(addprefix src/wp43s/, charString.c fonts.c items.c sort.c) \
                           $(addprefix $(BUILD_DIR)/generated/, rasterFontsData.c)
OBJ_GENERATECATALOGS     = $(addprefix $(BUILD_DIR)/generateCatalogs/,$(notdir $(SRC_GENERATECATALOGS:.c=.o)))
DEPS_GENERATECATALOGS    = $(addprefix $(BUILD_DIR)/generateCatalogs/,$(notdir $(SRC_GENERATECATALOGS:.c=.d)))

SRC_GENERATETESTPGMS    = $(addprefix src/generateTestPgms/, generateTestPgms.c)
OBJ_GENERATETESTPGMS    = $(addprefix $(BUILD_DIR)/generateTestPgms/,$(notdir $(SRC_GENERATETESTPGMS:.c=.o)))
DEPS_GENERATETESTPGMS   = $(addprefix $(BUILD_DIR)/generateTestPgms/,$(notdir $(SRC_GENERATETESTPGMS:.c=.d)))

SRC_TTF2RASTERFONTS      = $(addprefix src/ttf2RasterFonts/, ttf2RasterFonts.c)
OBJ_TTF2RASTERFONTS      = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TTF2RASTERFONTS:.c=.o)))
DEPS_TTF2RASTERFONTS     = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TTF2RASTERFONTS:.c=.d)))

SRC_TESTTTF2RASTERFONTS  = $(addprefix src/ttf2RasterFonts/, testTtf2RasterFonts.c) \
                           $(addprefix $(BUILD_DIR)/generated/, rasterFontsData.c)
OBJ_TESTTTF2RASTERFONTS  = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TESTTTF2RASTERFONTS:.c=.o)))
DEPS_TESTTTF2RASTERFONTS = $(addprefix $(BUILD_DIR)/ttf2RasterFonts/,$(notdir $(SRC_TESTTTF2RASTERFONTS:.c=.d)))

STAMP_FILES = $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-rasterFontsData $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-testPgms

vpath %.c dep/decNumberICU src/testSuite src/generateConstants src/generateCatalogs src/generateTestPgms src/ttf2RasterFonts \
          dep/DMCP_SDK/dmcp/sys $(BUILD_DIR)/generated \
          src/wp43s src/wp43s/mathematics src/wp43s/browsers src/wp43s/logicalOps src/wp43s/programming src/wp43s/distributions \
          src/wp43s/ui src/wp43s-gtk
vpath %.s dep/DMCP_SDK/dmcp


all: 	wp43s

rebuild:
	$(MAKE) mrproper
	$(MAKE) all

ifeq ($(CI_COMMIT_TAG),)
  WIN_DIST_DIR = wp43s-windows
  MAC_DIST_DIR = wp43s-macos
  DM_DIST_DIR = wp43s-dm42
else
  WIN_DIST_DIR = wp43s-windows-$(CI_COMMIT_TAG)
  MAC_DIST_DIR = wp43s-macos-$(CI_COMMIT_TAG)
  DM_DIST_DIR = wp43s-dm42-$(CI_COMMIT_TAG)
endif

dmcp: version $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_qspi.bin $(BUILD_DIR)/dmcp/$(WP43S_DMCP).pgm

dist_windows:	wp43s.exe $(BUILD_DIR)/wiki
	mkdir -p $(WIN_DIST_DIR)/res/artwork $(WIN_DIST_DIR)/res/dmcp
	cp wp43s.exe $(WIN_DIST_DIR)/
	cp res/artwork/*.png $(WIN_DIST_DIR)/res/artwork/
	cp res/dmcp/testPgms.bin $(WIN_DIST_DIR)/res/dmcp/
	cp res/wp43s_pre.css $(WIN_DIST_DIR)/res/
	cp res/fonts/WP43S_StandardFont.ttf $(WIN_DIST_DIR)/
	cp $(BUILD_DIR)/wiki/Windows-installation.md $(WIN_DIST_DIR)/readme.txt
	zip -r wp43s-windows.zip $(WIN_DIST_DIR)

dist_macos:	wp43s
	mkdir -p $(MAC_DIST_DIR)/res/artwork $(MAC_DIST_DIR)/res/dmcp
	cp wp43s $(MAC_DIST_DIR)/
	cp res/artwork/*.png $(MAC_DIST_DIR)/res/artwork/
	cp res/dmcp/testPgms.bin $(MAC_DIST_DIR)/res/dmcp/
	cp res/wp43s_pre.css $(MAC_DIST_DIR)/res/
	cp res/fonts/WP43S_StandardFont.ttf $(MAC_DIST_DIR)/
	zip -r wp43s-macos.zip $(MAC_DIST_DIR)

dist_dm42:	dmcp $(BUILD_DIR)/wiki
	mkdir -p $(DM_DIST_DIR)
	cp $(BUILD_DIR)/dmcp/WP43S.pgm $(BUILD_DIR)/dmcp/WP43S_qspi.bin $(DM_DIST_DIR)
	cp -r res/offimg $(DM_DIST_DIR)
	cp res/dmcp/keymap.bin res/dmcp/original_DM42_keymap.bin res/dmcp/testPgms.bin $(DM_DIST_DIR)
	cp res/artwork/WP43S_layout.svg $(DM_DIST_DIR)/WP43S_layout.svg
	cp $(BUILD_DIR)/wiki/DM42-installation.md $(DM_DIST_DIR)/readme.txt
	zip -r wp43s-dm42.zip $(DM_DIST_DIR)

.PHONY: clean_wp43s clean_generateConstants clean_generateCatalogs clean_generateTestPgms clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite clean_dmcp all clean_all mrproper sources rebuild dmcp dist_macos dist_windows dist_dm42

ifneq ($(EXE),)
generateConstants: $(GENERATECONSTANTS_APP)
generateCatalogs: $(GENERATECATALOGS_APP)
generateTestPgms: $(GENERATETESTPGMS_APP)
ttf2RasterFonts: $(TTF2RASTERFONTS_APP)
testTtf2RasterFonts: $(TESTTTF2RASTERFONTS_APP)
testSuite: $(TESTSUITE_APP)
wp43s: $(WP43S_APP)

.PHONY: generateConstants generateCatalogs generateTestPgms ttf2RasterFonts testTtf2RasterFonts testSuite wp43s
endif

sources: $(STAMP_FILES)

clean_all: clean_wp43s clean_generateConstants clean_generateCatalogs clean_generateTestPgms clean_ttf2RasterFonts clean_testTtf2RasterFonts clean_testSuite

mrproper: clean_all
	#rm -f $(GENERATED_SOURCES)
	rm -f $(STAMP_FILES)
	rm -f $(GENERATECONSTANTS_APP)
	rm -f $(GENERATECATALOGS_APP)
	rm -f $(GENERATETESTPGMS_APP)
	rm -f $(TTF2RASTERFONTS_APP)
	rm -f $(TESTTTF2RASTERFONTS_APP)
	rm -f $(WP43S_APP)
	rm -f $(TESTSUITE_APP)
	rm -f gmon.out


$(BUILD_DIR)/simulator:
	mkdir -p $@

$(BUILD_DIR)/testSuite:
	mkdir -p $@

$(BUILD_DIR)/generateConstants:
	mkdir -p $@

$(BUILD_DIR)/generateCatalogs:
	mkdir -p $@

$(BUILD_DIR)/generateTestPgms:
	mkdir -p $@

$(BUILD_DIR)/ttf2RasterFonts:
	mkdir -p $@

$(BUILD_DIR)/dmcp:
	mkdir -p $@



version:
	mkdir -p $(BUILD_DIR)/generated
	tools/versionUpdate

docs:
	cd docs/code && make html

$(BUILD_DIR)/wiki:
	git clone https://gitlab.com/Over_score/wp43s.wiki.git $(BUILD_DIR)/wiki



clean_generateConstants:
	rm -rf $(BUILD_DIR)/generateConstants

-include $(DEPS_GENERATECONSTANTS)

$(GENERATECONSTANTS_APP): $(OBJ_GENERATECONSTANTS)
	@echo -e "\n====> $(GENERATECONSTANTS_APP): binary/exe $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(OBJ_GENERATECONSTANTS) -o $@ $(SIM_LDFLAGS)

$(BUILD_DIR)/generateConstants/%.o: %.c | $(BUILD_DIR)/generateConstants
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/.stamp-constantPointers: $(GENERATECONSTANTS_APP)
	@echo -e "\n====> running $(GENERATECONSTANTS_APP) <===="
	./$(GENERATECONSTANTS_APP) $(BUILD_DIR)/generated/constantPointers.c $(BUILD_DIR)/generated/constantPointers.h
	cp $(BUILD_DIR)/generated/constantPointers.h src/generated/constantPointers.h
	cp $(BUILD_DIR)/generated/constantPointers.c src/generated/constantPointers.c
	touch $@

$(GEN_SRC_CONSTANTPOINTERS): $(BUILD_DIR)/.stamp-constantPointers



clean_generateCatalogs:
	rm -rf $(BUILD_DIR)/generateCatalogs

-include $(DEPS_GENERATECATALOGS)

$(GENERATECATALOGS_APP): CFLAGS += -DGENERATE_CATALOGS
$(GENERATECATALOGS_APP): $(OBJ_GENERATECATALOGS)
	@echo -e "\n====> $(GENERATECATALOGS_APP): binary/exe $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(OBJ_GENERATECATALOGS) -o $@ $(SIM_LDFLAGS)

$(BUILD_DIR)/generateCatalogs/rasterFontsData.o: $(BUILD_DIR)/generated/rasterFontsData.c $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-rasterFontsData | $(BUILD_DIR)/generateCatalogs
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/generateCatalogs/%.o: %.c $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-rasterFontsData | $(BUILD_DIR)/generateCatalogs
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/.stamp-softmenuCatalog: $(GENERATECATALOGS_APP)
	@echo -e "\n====> running $(GENERATECATALOGS_APP) <===="
	./$(GENERATECATALOGS_APP) $(BUILD_DIR)/generated/softmenuCatalogs.h
	cp $(BUILD_DIR)/generated/softmenuCatalogs.h src/generated/softmenuCatalogs.h
	touch $@

$(GEN_SRC_SOFTMENUCATALOGS): $(BUILD_DIR)/.stamp-softmenuCatalog



clean_generateTestPgms:
	rm -rf $(BUILD_DIR)/generateTestPgms

-include $(DEPS_GENERATETESTPGMS)

$(GENERATETESTPGMS_APP): $(OBJ_GENERATETESTPGMS)
	@echo -e "\n====> $(GENERATETESTPGMS_APP): binary/exe $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(OBJ_GENERATETESTPGMS) -o $@ $(SIM_LDFLAGS)

$(BUILD_DIR)/generateTestPgms/%.o: %.c | $(BUILD_DIR)/generateTestPgms
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/.stamp-testPgms: $(GENERATETESTPGMS_APP)
	@echo -e "\n====> running $(GENERATETESTPGMS_APP) <===="
	./$(GENERATETESTPGMS_APP) res/dmcp/testPgms.bin
	touch $@

$(GEN_BIN_TESTPGMS): $(BUILD_DIR)/.stamp-testPgms



clean_ttf2RasterFonts:
	rm -rf $(BUILD_DIR)/ttf2RasterFonts

-include $(DEPS_TTF2RASTERFONTS)

$(TTF2RASTERFONTS_APP): $(OBJ_TTF2RASTERFONTS)
	@echo -e "\n====> $(TTF2RASTERFONTS_APP): binary/exe $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(OBJ_TTF2RASTERFONTS) -o $@ $(SIM_LDFLAGS)

$(BUILD_DIR)/ttf2RasterFonts/%.o: %.c | $(BUILD_DIR)/ttf2RasterFonts
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/.stamp-rasterFontsData: $(TTF2RASTERFONTS_APP) res/fonts/WP43S_NumericFont.ttf res/fonts/WP43S_StandardFont.ttf
	@echo -e "\n====> running $(TTF2RASTERFONTS_APP) <===="
	./$(TTF2RASTERFONTS_APP) res/fonts build/generated/rasterFontsData.c > /dev/null
	cp $(BUILD_DIR)/generated/rasterFontsData.c src/generated/rasterFontsData.c
	touch $@

$(BUILD_DIR)/ttf2RasterFonts/rasterFontsData.o: $(BUILD_DIR)/.stamp-rasterFontsData

$(GEN_SRC_RASTERFONTSDATA): $(BUILD_DIR)/.stamp-rasterFontsData


clean_testTtf2RasterFonts:
	rm -f $(BUILD_DIR)/ttf2RasterFonts/testTtf2RasterFonts.o

-include $(DEPS_TESTTTF2RASTERFONTS)

$(TESTTTF2RASTERFONTS_APP): $(OBJ_TESTTTF2RASTERFONTS)
	@echo -e "\n====> $(TESTTTF2RASTERFONTS_APP): binary/exe <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(OBJ_TESTTTF2RASTERFONTS) -o $@ $(SIM_LDFLAGS)

$(BUILD_DIR)/ttf2RasterFonts/testTtf2RasterFonts.o: $(BUILD_DIR)/.stamp-constantPointers


clean_testSuite:
	rm -rf $(BUILD_DIR)/testSuite

-include $(DEPS_TESTSUITE)

$(TESTSUITE_APP): CFLAGS += -DTESTSUITE_BUILD
$(TESTSUITE_APP): version $(OBJ_TESTSUITE)
	@echo -e "\n====> $(TESTSUITE_APP): binary/exe $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(OBJ_TESTSUITE) -o $(TESTSUITE_APP) $(SIM_LDFLAGS)

$(BUILD_DIR)/testSuite/rasterFontsData.o: $(BUILD_DIR)/generated/rasterFontsData.c $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-rasterFontsData | $(BUILD_DIR)/testSuite
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/testSuite/constantPointers.o: $(BUILD_DIR)/generated/constantPointers.c $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-rasterFontsData | $(BUILD_DIR)/testSuite
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/testSuite/%.o: %.c $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-rasterFontsData | $(BUILD_DIR)/testSuite
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<



clean_wp43s:
	rm -rf $(BUILD_DIR)/simulator

-include $(DEPS_SIMULATOR)

$(WP43S_APP): version $(OBJ_SIMULATOR)
	@echo -e "\n====> $(WP43S_APP): binary/exe $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(OBJ_SIMULATOR) -o $(WP43S_APP) $(SIM_LDFLAGS)

$(BUILD_DIR)/simulator/rasterFontsData.o: $(BUILD_DIR)/generated/rasterFontsData.c $(STAMP_FILES) | $(BUILD_DIR)/simulator
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/simulator/constantPointers.o: $(BUILD_DIR)/generated/constantPointers.c $(STAMP_FILES) | $(BUILD_DIR)/simulator
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

$(BUILD_DIR)/simulator/%.o: %.c $(STAMP_FILES) | $(BUILD_DIR)/simulator
	@echo -e "\n====> $<: $@ <===="
	$(CC) $(SIM_CFLAGS) $(CFLAGS) $(C_INCLUDES) -c -o $@ $<

# Check-out submodule when trying to build pgm_syscalls.c - we may need additional triggers for the submodule init
dmcp_sdk:
	git submodule update --init --recursive dep/DMCP_SDK

dep/DMCP_SDK/dmcp/dmcp.h: dmcp_sdk
dep/DMCP_SDK/dmcp/sys/pgm_syscalls.c: dmcp_sdk
dep/DMCP_SDK/dmcp/startup_pgm.s: dmcp_sdk

# forcecrc32 needed because QSPI update process in DMCP requires a specific CRC
$(BUILD_DIR)/dmcp/forcecrc32: dep/forcecrc32.c | $(BUILD_DIR)/dmcp
	gcc $< -o $@

dep/gmp-6.2.0:
	wget -qO- https://gmplib.org/download/gmp/gmp-6.2.0.tar.lz | tar --lzip -xv -C dep

$(GMPLIB): dep/gmp-6.2.0
	cd dep/gmp-6.2.0 && ./configure \
		--build=x86_64-pc-linux-gnu \
		--host=arm-none-eabi \
		--disable-assembly \
		--disable-shared \
		--disable-fft \
		--disable-werror \
		--disable-cxx \
		LDFLAGS='-Wl,--fix-cortex-a8 -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now' \
		CFLAGS="-nostartfiles --specs=nosys.specs -mcpu=cortex-m4 -Os -s -pedantic -fomit-frame-pointer -Wa,--noexecstack -ffunction-sections -funwind-tables -fno-strict-aliasing -fstack-protector -finline-limit=64 -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16"
	cd dep/gmp-6.2.0 && make -j8 V=1

clean_dmcp:
	-rm -rf $(BUILD_DIR)/dmcp

-include $(DEPS_DMCP)

$(BUILD_DIR)/dmcp/rasterFontsData.o: $(BUILD_DIR)/generated/rasterFontsData.c dep/DMCP_SDK/dmcp/dmcp.h $(GMPLIB) $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-testPgms | $(BUILD_DIR)/dmcp
	@echo -e "\n====> $<: $@ <===="
	$(DMCP_CC) $(DMCP_CFLAGS) $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/dmcp/$(notdir $(<:.c=.lst)) -c -o $@ $<

$(BUILD_DIR)/dmcp/constantPointers.o: $(BUILD_DIR)/generated/constantPointers.c dep/DMCP_SDK/dmcp/dmcp.h $(GMPLIB) $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-testPgms | $(BUILD_DIR)/dmcp
	@echo -e "\n====> $<: $@ <===="
	$(DMCP_CC) $(DMCP_CFLAGS) $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/dmcp/$(notdir $(<:.c=.lst)) -c -o $@ $<

$(BUILD_DIR)/dmcp/%.o: %.c dep/DMCP_SDK/dmcp/dmcp.h $(GMPLIB) $(BUILD_DIR)/.stamp-constantPointers $(BUILD_DIR)/.stamp-softmenuCatalog $(BUILD_DIR)/.stamp-testPgms | $(BUILD_DIR)/dmcp
	@echo -e "\n====> $<: $@ <===="
	$(DMCP_CC) $(DMCP_CFLAGS) $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/dmcp/$(notdir $(<:.c=.lst)) -c -o $@ $<

$(BUILD_DIR)/dmcp/%.o: %.s | $(BUILD_DIR)/dmcp
	@echo -e "\n====> $<: $@ <===="
	$(DMCP_AS) $(DMCP_CFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/dmcp/$(WP43S_DMCP).elf: $(BUILD_DIR)/dmcp/forcecrc32 $(GMPLIB) $(OBJ_DMCP)
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(DMCP_CC) $(OBJ_DMCP) $(DMCP_LDFLAGS) -o $@
	$(DMCP_SIZE) $@

$(BUILD_DIR)/dmcp/$(WP43S_DMCP)_flash.bin: $(BUILD_DIR)/dmcp/$(WP43S_DMCP).elf
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(DMCP_OBJCOPY) --remove-section .qspi -O binary  $<  $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_flash.bin

$(BUILD_DIR)/dmcp/$(WP43S_DMCP)_qspi.bin: $(BUILD_DIR)/dmcp/$(WP43S_DMCP).elf
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(DMCP_OBJCOPY) --only-section .qspi -O binary  $<  $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_qspi.bin
	$(TOOLS_DIR)/modify_crc $(WP43S_DMCP)
	$(TOOLS_DIR)/check_qspi_crc $(WP43S_DMCP) src/wp43s-dmcp/qspi_crc.h || ( $(MAKE) clean_dmcp && false )

$(BUILD_DIR)/dmcp/$(WP43S_DMCP).pgm: $(BUILD_DIR)/dmcp/$(WP43S_DMCP)_flash.bin
	@echo -e "\n====> $(WP43S_DMCP): binary/exe $@ <===="
	$(TOOLS_DIR)/add_pgm_chsum $< $@
