CXX=g++# C compiler
CXXFLAGS=-std=c++17 -fPIC -O2 -pthread -fno-new-ttp-matching# -Wall -Wextra C flags
LDFLAGS=-shared# linking flags

LIB_FLAGS=
INCLUDE_FLAGS=-I. -I./include

NAME = reelay
VERSION = 1.6

PROJECT_INCLUDE = include/reelay

PREFIX = /usr/local
INCDIR = $(PREFIX)/include
WORKDIR = ./build

.PHONY: all develop test install uninstall apps

all: install

install:
	mkdir -p $(DESTDIR)$(INCDIR)/$(NAME)
	cp -a $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)

uninstall:
	rm -rf $(DESTDIR)$(INCDIR)/$(NAME)

develop:
	ln -s $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)
	ln -s $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET)

timescales:
	mkdir -p build
	cd build && rm -rf timescales && git clone https://github.com/doganulus/timescales.git
	cd build/timescales && make full

timescales-clean:
	rm -rf build/timescales

apps: rymtl rystl

rymtl:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) apps/mtl/main.cpp -o bin/rymtl $(INCLUDE_FLAGS) $(LIB_FLAGS)

rystl:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) apps/stl/main.cpp -o bin/rystl $(INCLUDE_FLAGS) $(LIB_FLAGS)

apps-install:
	cp -p bin/rymtl /usr/local/bin/rymtl
	cp -p bin/rystl /usr/local/bin/rystl

apps-uninstall:
	rm /usr/local/bin/rymtl
	rm /usr/local/bin/rystl

test_csvparser:
	mkdir -p bin/csvparser
	$(CXX) $(CXXFLAGS) apps/csvparser/basic.cpp -o bin/csvparser/csvparser_basic $(INCLUDE_FLAGS)
	$(CXX) $(CXXFLAGS) apps/csvparser/fast.cpp -o bin/csvparser/csvparser_fast $(INCLUDE_FLAGS)
	$(CXX) $(CXXFLAGS) apps/csvparser/modern.cpp -o bin/csvparser/csvparser_modern $(INCLUDE_FLAGS)
	multitime -n 10 bin/csvparser/csvparser_basic build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv
	multitime -n 10 bin/csvparser/csvparser_fast build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv
	multitime -n 10 bin/csvparser/csvparser_modern build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv

test: test_main test_untimed test_discrete_timed test_dense_timed test_untimed_robustness test_discrete_timed_robustness

test_main:
	mkdir -p build/test
	$(CXX) $(CXXFLAGS) -c test/test_main.cpp -o build/test/main.o $(INCLUDE_FLAGS) $(LIB_FLAGS)

test_untimed:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_untimed.cpp -o build/test/test_setting_untimed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_untimed -r compact

test_untimed_robustness:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_untimed_robustness.cpp -o build/test/test_setting_untimed_robustness $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_untimed_robustness -r compact

test_discrete_timed:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_discrete_timed.cpp -o build/test/test_setting_discrete_timed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_discrete_timed -r compact

test_discrete_timed_robustness:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_discrete_timed_robustness.cpp -o build/test/test_setting_discrete_timed_robustness $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_discrete_timed_robustness -r compact

test_dense_timed:
	$(CXX) $(CXXFLAGS) build/test/main.o test/test_setting_dense_timed.cpp -o build/test/test_setting_dense_timed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./build/test/test_setting_dense_timed -r compact

test_mtl_performance_discrete: test/timescales/discrete/multitime/*.txt
	for batchfile in $^ ; do \
        multitime -n 10 -b $${batchfile} ; \
    done

python: 
	cd python && pip install .

python-develop: 
	sudo make uninstall && sudo make install
	cd python && pip install .

main: 
	$(CXX) $(CXXFLAGS) $(FILE) -o bin/main $(INCLUDE_FLAGS) $(LIB_FLAGS) && bin/main $(EXTRA)