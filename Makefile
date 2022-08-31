.PHONY: all clean rpiwsmd sim test dmcp docs testPgms dist_windows dist_macos dist_dm42

all: sim

EXE =
ifeq ($(OS),Windows_NT)
  EXE = .exe
endif

clean:
	rm -f wp43s$(EXE) wp43s-smd
	rm -rf wp43s-windows* wp43s-macos* wp43s-dm42*
	rm -rf build build.sim build.rpiwsmd build.dmcp build.rel

build.rpiwsmd:
	meson setup build.rpiwsmd --buildtype=debug -DRASPBERRY=`tools/onARaspberry` -DRPIWSMD=true

build.sim:
	meson setup build.sim --buildtype=debug -DRASPBERRY=`tools/onARaspberry`

build.rel:
	meson setup build.rel --buildtype=release -DCI_COMMIT_TAG=$(CI_COMMIT_TAG)

build.dmcp:
	meson setup build.dmcp --cross-file=cross_arm_gcc.build -DCI_COMMIT_TAG=$(CI_COMMIT_TAG)

rpiwsmd: build.rpiwsmd
	cd build.rpiwsmd && ninja rpiwsmd
	cp build.rpiwsmd/src/wp43s-gtk/wp43s-smd ./
	cp build.rpiwsmd/src/generateCatalogs/softmenuCatalogs.h src/generated/
	cp build.rpiwsmd/src/generateConstants/constantPointers.* src/generated/
	cp build.rpiwsmd/src/ttf2RasterFonts/rasterFontsData.c src/generated/

sim: build.sim
	cd build.sim && ninja sim
	cp build.sim/src/wp43s-gtk/wp43s$(EXE) ./
	cp build.sim/src/generateCatalogs/softmenuCatalogs.h src/generated/
	cp build.sim/src/generateConstants/constantPointers.* src/generated/
	cp build.sim/src/ttf2RasterFonts/rasterFontsData.c src/generated/

test: build.sim
	cd build.sim && ninja test

dmcp: build.dmcp
	cd build.dmcp && ninja dmcp

docs: build.sim
	cd build.sim && ninja docs

testPgms: build.sim
	cd build.sim && ninja testPgms
	cp build.sim/src/generateTestPgms/testPgms.bin res/dmcp/

build.rel/wiki: build.rel
	git clone https://gitlab.com/Over_score/wp43s.wiki.git build.rel/wiki

ifeq ($(CI_COMMIT_TAG),)
  WIN_DIST_DIR = wp43s-windows
  MAC_DIST_DIR = wp43s-macos
  DM_DIST_DIR = wp43s-dm42
else
  WIN_DIST_DIR = wp43s-windows-$(CI_COMMIT_TAG)
  MAC_DIST_DIR = wp43s-macos-$(CI_COMMIT_TAG)
  DM_DIST_DIR = wp43s-dm42-$(CI_COMMIT_TAG)
endif

dist_windows: testPgms build.rel/wiki
	cd build.rel && ninja sim
	mkdir -p $(WIN_DIST_DIR)/res/artwork $(WIN_DIST_DIR)/res/dmcp $(WIN_DIST_DIR)/res/tone
	cp build.rel/src/wp43s-gtk/wp43s.exe $(WIN_DIST_DIR)/
	cp res/artwork/*.cfg $(WIN_DIST_DIR)/res/artwork/
	cp -r res/artwork/standardSkin $(WIN_DIST_DIR)/res/artwork/
	cp res/tone/*.wav $(WIN_DIST_DIR)/res/tone/
	cp res/dmcp/testPgms.bin $(WIN_DIST_DIR)/res/dmcp/
	cp res/wp43s_pre.css $(WIN_DIST_DIR)/res/
	cp res/fonts/WP43S_StandardFont.ttf $(WIN_DIST_DIR)/
	cp build.rel/wiki/Installation-on-Windows.md $(WIN_DIST_DIR)/readme.txt
	zip -r wp43s-windows.zip $(WIN_DIST_DIR)
	rm -rf $(WIN_DIST_DIR)

dist_macos: testPgms build.rel
	cd build.rel && ninja sim
	mkdir -p $(MAC_DIST_DIR)/res/artwork $(MAC_DIST_DIR)/res/dmcp
	cp build.rel/src/wp43s-gtk/wp43s $(MAC_DIST_DIR)/
	cp res/artwork/*.cfg $(MAC_DIST_DIR)/res/artwork/
	cp -r res/artwork/standardSkin $(MAC_DIST_DIR)/res/artwork/
	cp res/dmcp/testPgms.bin $(MAC_DIST_DIR)/res/dmcp/
	cp res/wp43s_pre.css $(MAC_DIST_DIR)/res/
	cp res/fonts/WP43S_StandardFont.ttf $(MAC_DIST_DIR)/
	zip -r wp43s-macos.zip $(MAC_DIST_DIR)
	rm -rf $(MAC_DIST_DIR)

dist_dm42: dmcp testPgms build.rel/wiki
	mkdir -p $(DM_DIST_DIR)
	cp build.dmcp/src/wp43s-dmcp/WP43S.pgm build.dmcp/src/wp43s-dmcp/WP43S_qspi.bin $(DM_DIST_DIR)
	cp -r res/offimg $(DM_DIST_DIR)
	cp res/dmcp/keymap.bin res/dmcp/original_DM42_keymap.bin res/dmcp/testPgms.bin $(DM_DIST_DIR)
	cp res/artwork/WP43S_layout.svg $(DM_DIST_DIR)/WP43S_layout.svg
	cp build.rel/wiki/Installation-on-a-DM42.md $(DM_DIST_DIR)/readme.txt
	zip -r wp43s-dm42.zip $(DM_DIST_DIR)
	rm -rf $(DM_DIST_DIR)
