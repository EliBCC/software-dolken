CMAKE_GENERATOR = Unix Makefiles
BUILD_DIR = ./build

ECHO = cmake -E echo
RMDIR = cmake -E remove_directory
TOUCH = cmake -E touch

.DEFAULT_GOAL := help

help:
	@$(ECHO) "DanSTAR RICHARD build system"
	@$(ECHO) "Current shell: $(SHELL)"
	@$(ECHO) "Usage: make <target>"
	@$(ECHO) "Target can be any of the following:"
	@$(ECHO) "modm"
	@$(ECHO) "    Generates modm code from repository."
	@$(ECHO) "    Dependencies:"
	@$(ECHO) "        File project.xml"
	@$(ECHO) ""
	@$(ECHO) "modm-docs"
	@$(ECHO) "    Generates HTML of modm documentation, can be found in docs directory."
	@$(ECHO) "    Dependencies:"
	@$(ECHO) "        File project.xml"
	@$(ECHO) ""
	@$(ECHO) "build-<boardname>-debug"
	@$(ECHO) "    Builds firmware with the code in src/<boardname>"
	@$(ECHO) "    Dependencies:"
	@$(ECHO) "        Target modm"
	@$(ECHO) "        Files in src/common/*"
	@$(ECHO) "        Files in src/<boardname>/*"
	@$(ECHO) ""
	@$(ECHO) "build-<boardname>-release"
	@$(ECHO) "    Builds firmware with the code in src/<boardname> with optimizations enabled"
	@$(ECHO) "    Dependencies:"
	@$(ECHO) "        Target modm"
	@$(ECHO) "        Files in src/common/*"
	@$(ECHO) "        Files in src/<boardname>/*"
	@$(ECHO) ""
	@$(ECHO) "upload-<boardname>-debug"
	@$(ECHO) "    Uploads debug firmware to microcontroller"
	@$(ECHO) "    Dependencies:"
	@$(ECHO) "        Target build-<boardname>-debug"
	@$(ECHO) ""
	@$(ECHO) "upload-<boardname>-debug"
	@$(ECHO) "    Uploads release firmware to microcontroller"
	@$(ECHO) "    Dependencies:"
	@$(ECHO) "        Target build-<boardname>-release"
	@$(ECHO) ""
	@$(ECHO) "clean"
	@$(ECHO) "    Deletes the build folder"
	@$(ECHO) ""
	@$(ECHO) "Dependencies are automatically built or rebuilt as necessary. Example:"
	@$(ECHO) "Running 'make upload-mainboard-debug' from a clean repository will build modm, build the firmware, and upload it."


clean:
	@$(RMDIR) build

modm/stamp: project.xml
	@$(RMDIR) modm
	@lbuild build
	@$(TOUCH) modm/stamp

modm: modm/stamp

modm/docs/stamp: modm/stamp
	@lbuild build -m ":docs"
	@$(TOUCH) modm/docs/stamp
	@cmake -E create_symlink ../modm/docs/html/index.html docs/modm.html

modm-docs: modm/docs/stamp

$(BUILD_DIR)/%-debug/stamp: modm/stamp
	@cmake -E make_directory $(BUILD_DIR)/$*-debug
	@cd $(BUILD_DIR)/$*-debug && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Debug -DBOARD_NAME=$* -G "$(CMAKE_GENERATOR)" ../..

build-%-debug: $(BUILD_DIR)/%-debug/stamp
	@cmake --build $(BUILD_DIR)/$*-debug --parallel $J

upload-%-debug: build-%-debug
	@openocd -f scripts/openocd.cfg -c "program_file build/$*-debug/$*.elf"

$(BUILD_DIR)/%/stamp: modm/stamp
	@cmake -E make_directory $(BUILD_DIR)/$*-release
	@cd $(BUILD_DIR)/$*-release && cmake $(CMAKE_FLAGS) -DCMAKE_BUILD_TYPE=Release -DBOARD_NAME=$* -G "$(CMAKE_GENERATOR)" ../..

build-%-release: $(BUILD_DIR)/%/stamp
	@cmake --build $(BUILD_DIR)/$*-release --parallel $J

upload-%-release: build-%-release
	@openocd -f scripts/openocd.cfg -c "program_file build/$*-release/$*.elf"
