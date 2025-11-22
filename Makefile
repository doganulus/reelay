WORKSPACE := ${PWD}
BUILD_DIRECTORY := /tmp/build/$(basename $(notdir ${WORKSPACE}))

.PHONY: default configure build test

default: build

configure:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY)

build: configure
	cmake --build $(BUILD_DIRECTORY)

test:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DREELAY_BUILD_TESTS=ON
	cmake --build $(BUILD_DIRECTORY)
	ctest --test-dir $(BUILD_DIRECTORY) --output-on-failure

install: build
	cmake --build $(BUILD_DIRECTORY) --target install

clean:
	rm -rf $(BUILD_DIRECTORY)
