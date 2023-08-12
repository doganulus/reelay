WORKSPACE = $(PWD)
BUILD_DIRECTORY = /tmp/$(basename $(notdir ${WORKSPACE}))/build

.PHONY: all configure build test

configure:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY)

build: configure
	cmake --build $(BUILD_DIRECTORY)

test: build
	ctest --test-dir $(BUILD_DIRECTORY) --output-on-failure

all: build
