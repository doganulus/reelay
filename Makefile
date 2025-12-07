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
	cmake --build $(BUILD_DIRECTORY)
	cmake --install $(BUILD_DIRECTORY)

install: build
	cmake --install $(BUILD_DIRECTORY)

clean:
	rm -rf $(BUILD_DIRECTORY)

benchmark:
	pip install git+https://github.com/doganulus/timescales.git --break-system-packages
	timescales-generate-large --failing-end --output-dir /tmp/benchmarks/data

benchmark-binary: benchmark
	git clone https://github.com/doganulus/timescales.git /tmp/timescales
	cd /tmp/benchmarks/data && python /tmp/timescales/scripts/to_binary_row.py

docs:
	mkdocs serve
