WORKSPACE := ${PWD}
BUILD_DIRECTORY := /tmp/$(basename $(notdir ${WORKSPACE}))/build
CMAKE_INSTALL_PREFIX ?= /usr/local

.PHONY: all configure build test cbuild cryjson

configure:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)

build: configure
	cmake --build $(BUILD_DIRECTORY)

test: build
	ctest --test-dir $(BUILD_DIRECTORY) --output-on-failure

install: build
	cmake --install $(BUILD_DIRECTORY) --prefix $(CMAKE_INSTALL_PREFIX)

cdevel:
	docker build -t ghcr.io/doganulus/reelay:devel docker/devel

cryjson:
	docker build -t ghcr.io/doganulus/reelay:ryjson docker/ryjson

cbenchmark:
	docker build -t ghcr.io/doganulus/reelay-benchmark:latest docker/benchmark

