WORKSPACE := ${PWD}
BUILD_DIRECTORY := /tmp/build/$(basename $(notdir ${WORKSPACE}))

.PHONY: default configure build test apps

default: build

configure:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY)

build: configure
	cmake --build $(BUILD_DIRECTORY)

test:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DREELAY_BUILD_TESTS=ON
	cmake --build $(BUILD_DIRECTORY)
	ctest --test-dir $(BUILD_DIRECTORY) --output-on-failure

apps:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DREELAY_BUILD_APPS=ON
	cmake --build $(BUILD_DIRECTORY) --target ryjson
	cmake --install $(BUILD_DIRECTORY)

install: build
	cmake --install $(BUILD_DIRECTORY)

clean:
	rm -rf $(BUILD_DIRECTORY)

docs:
	mkdocs serve
