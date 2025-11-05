WORKSPACE := ${PWD}
BUILD_DIRECTORY := /tmp/build/$(basename $(notdir ${WORKSPACE}))

.PHONY: default configure build test cbuild cryjson

default: build

configure:
	cmake -S $(WORKSPACE) -B $(BUILD_DIRECTORY) -DBUILD_TESTS=ON

build: configure
	cmake --build $(BUILD_DIRECTORY)

test: build
	ctest --test-dir $(BUILD_DIRECTORY) --output-on-failure

install: build
	cmake --build $(BUILD_DIRECTORY) --target install

cdevel:
	docker build -t ghcr.io/doganulus/reelay:devel docker/devel

cryjson:
	docker build -t ghcr.io/doganulus/reelay:ryjson docker/ryjson

cbenchmark:
	docker build -t ghcr.io/doganulus/reelay-benchmark:latest docker/benchmark

clean:
	rm -rf $(BUILD_DIRECTORY)
