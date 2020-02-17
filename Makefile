CC=gcc#
CXX=g++#
CXXFLAGS=-std=c++11 -fPIC -O2 -pthread
CXXFLAGS_APPS=-std=c++17 -fPIC -O2 -pthread -fno-new-ttp-matching -Wall -Wextra
CXXFLAGS_TEST=-g -std=c++11 -fPIC -O0 -pthread --coverage -fno-inline -fno-inline-small-functions -fno-default-inline#-Wall -Wextra

LIB_FLAGS=-lcudd
INCLUDE_FLAGS=-I. -I./include

NAME=reelay

ROOT_DIR=$(shell pwd)
PROJECT_INCLUDE=include/reelay

PREFIX = /usr/local
INCDIR = $(PREFIX)/include

.PHONY: all apps python develop test install uninstall 

all: install

install:
	mkdir -p $(DESTDIR)$(INCDIR)/$(NAME)
	cp -a $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)

uninstall:
	rm -rf $(DESTDIR)$(INCDIR)/$(NAME)

develop:
	ln -s $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)
	ln -s $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET)

cudd:
	mkdir -p build
	cd build && rm -rf cudd && git clone https://github.com/doganulus/cudd.git
	cd build/cudd && aclocal && autoconf &&./configure CC=gcc CXX=g++ --enable-silent-rules --enable-shared --enable-obj
	cd build/cudd && make

cudd-install:
	cd build/cudd && make install

cudd-uninstall:
	cd build/cudd && make uninstall

benchmark: timescales rvbc2018

timescales:
	mkdir -p test/build
	cd test/build && rm -rf timescales && git clone https://github.com/doganulus/timescales.git
	cd test/build/timescales && make full

timescales-clean:
	rm -rf test/build/timescales

rvbc2018:
	mkdir -p test/build
	cd test/build && rm -rf benchmark-challenge-2018 && git clone https://github.com/runtime-verification/benchmark-challenge-2018.git

rvbc2018-clean:
	rm -rf test/build/benchmark-challenge-2018

apps: rymtl rystl ryjavu

rymtl:
	mkdir -p bin
	$(CXX) $(CXXFLAGS_APPS) apps/rymtl/main.cpp -o bin/rymtl $(INCLUDE_FLAGS)

rystl:
	mkdir -p bin
	$(CXX) $(CXXFLAGS_APPS) apps/rystl/main.cpp -o bin/rystl $(INCLUDE_FLAGS)

ryjavu:
	mkdir -p bin
	$(CXX) $(CXXFLAGS_APPS) apps/ryjavu/main.cpp /usr/local/lib/libcudd.a -o bin/ryjavu $(INCLUDE_FLAGS)

apps-install:
	cp -p bin/rymtl /usr/local/bin/rymtl
	cp -p bin/rystl /usr/local/bin/rystl
	cp -p bin/rystl /usr/local/bin/ryjavu

apps-uninstall:
	rm /usr/local/bin/rymtl
	rm /usr/local/bin/rystl
	rm /usr/local/bin/ryjavu

test_csvparser:
	mkdir -p bin/csvparser
	$(CXX) $(CXXFLAGS) apps/csvparser/basic.cpp -o bin/csvparser/csvparser_basic $(INCLUDE_FLAGS)
	$(CXX) $(CXXFLAGS) apps/csvparser/fast.cpp -o bin/csvparser/csvparser_fast $(INCLUDE_FLAGS)
	$(CXX) $(CXXFLAGS) apps/csvparser/modern.cpp -o bin/csvparser/csvparser_modern $(INCLUDE_FLAGS)
	multitime -n 10 bin/csvparser/csvparser_basic build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv
	multitime -n 10 bin/csvparser/csvparser_fast build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv
	multitime -n 10 bin/csvparser/csvparser_modern build/timescales/fullsuite/AlwaysBQR/Discrete/1M/AlwaysBQR1000.csv

test: test_main test_untimed test_discrete_timed test_dense_timed test_untimed_robustness test_discrete_timed_robustness test_dense_timed_robustness_0 test_untimed_data test_discrete_timed_data test_dense_timed_data coverage

test_main:
	mkdir -p test/build
	$(CXX) $(CXXFLAGS) -c test/test_main.cpp -o test/build/main.o

test_untimed:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_untimed.cpp -o test_setting_untimed -I$(ROOT_DIR)/include
	cd test/build && ./test_setting_untimed -r compact

test_untimed_data:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_untimed_data.cpp -o test_setting_untimed_data -I$(ROOT_DIR)/include -lcudd
	cd test/build && ./test_setting_untimed_data -r compact

test_untimed_robustness:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_untimed_robustness.cpp -o test_setting_untimed_robustness -I$(ROOT_DIR)/include
	cd test/build && ./test_setting_untimed_robustness -r compact

test_discrete_timed:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_discrete_timed.cpp -o test_setting_discrete_timed -I$(ROOT_DIR)/include
	cd test/build && ./test_setting_discrete_timed -r compact

test_discrete_timed_data:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_discrete_timed_data.cpp -o test_setting_discrete_timed_data -I$(ROOT_DIR)/include -lcudd
	cd test/build && ./test_setting_discrete_timed_data -r compact

test_discrete_timed_robustness:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_discrete_timed_robustness.cpp -o test_setting_discrete_timed_robustness -I$(ROOT_DIR)/include
	cd test/build && ./test_setting_discrete_timed_robustness -r compact

test_dense_timed:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_dense_timed.cpp -o test_setting_dense_timed -I$(ROOT_DIR)/include
	cd test/build && ./test_setting_dense_timed -r compact

test_dense_timed_data:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_dense_timed_data.cpp -o test_setting_dense_timed_data -I$(ROOT_DIR)/include -lcudd
	cd test/build && ./test_setting_dense_timed_data -r compact

test_dense_timed_robustness_0:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_dense_timed_robustness_0.cpp -o test_setting_dense_timed_robustness_0 -I$(ROOT_DIR)/include
	cd test/build && ./test_setting_dense_timed_robustness_0 -r compact

test_untimed_regular:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_setting_untimed_regular.cpp -o test_setting_untimed_regular -I$(ROOT_DIR)/include
	cd test/build && ./test_setting_untimed_regular -r compact

test_recipes:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_recipes.cpp -o test_recipes -I$(ROOT_DIR)/include -lcudd
	cd test/build && ./test_recipes -r compact

test_conversions:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/test/test_conversions.cpp -o test_conversions -I$(ROOT_DIR)/include -lcudd
	cd test/build && ./test_conversions -r compact

test_mtl_performance_discrete: test/timescales/discrete/multitime/*.txt
	for batchfile in $^ ; do \
        multitime -n 10 -b $${batchfile} ; \
    done

test_qtl_performance_discrete: test/dejavu/*.txt
	for batchfile in $^ ; do \
        multitime -n 10 -b $${batchfile} ; \
    done

python: 
	pip install .

python-pypi-upload: 
	python setup.py sdist bdist_wheel
	twine check dist/*
	twine upload dist/*

python-develop: 
	sudo make uninstall && sudo make install
	cd python && pip install .

main: 
	$(CXX) $(CXXFLAGS) $(FILE) -o bin/main $(INCLUDE_FLAGS) $(LIB_FLAGS) && bin/main $(EXTRA)

coverage:
	cd test/build && gcov -p -s .. -o . test_setting_untimed.cpp test_setting_discrete_timed.cpp test_setting_dense_timed.cpp test_setting_untimed_robustness.cpp test_setting_discrete_timed_robustness.cpp test_setting_dense_timed_robustness.cpp test_setting_untimed_data.cpp test_setting_discrete_timed_data.cpp test_setting_dense_timed_data.cpp
	cd test/build && lcov --capture --quiet --directory . --output-file lcov.info  
	cd test/build && lcov --remove lcov.info "/usr/*" "$(ROOT_DIR)/third_party/*" "$(ROOT_DIR)/include/reelay/third_party/*" "$(ROOT_DIR)/test/*" --directory . --output-file lcov.info
	cd test/build && genhtml --ignore-errors source lcov.info --legend --title "commit SHA1"