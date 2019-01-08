CMAKE_GENERATOR = Unix Makefiles
BUILD_DIR = ./build
J = 1

.DEFAULT_GOAL := help

help:
	@echo "Current shell: $(SHELL)"
	@echo "Look at the Makefile"

modm/stamp: project.xml
	@lbuild build
	@touch modm/stamp

modm: modm/stamp

modm/docs/stamp: modm/stamp
	@lbuild build -m ":docs"
	@touch modm/docs/stamp

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

clean-%:
	@cmake --build $(BUILD_DIR)/$*-debug --target clean