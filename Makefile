CC=gcc#
CXX=g++#
CXXFLAGS=-std=c++11 -fPIC -O2 -pthread
CXXFLAGS_APPS=-std=c++17 -fPIC -pthread -O2 -s -DNDEBUG#-Wall -Wextra
CXXFLAGS_TEST=-g -std=c++11 -fPIC -O0 -pthread --coverage -fno-inline -fno-inline-small-functions -fno-default-inline -fvisibility=hidden#-Wall -Wextra

LIB_FLAGS=-lcudd
INCLUDE_FLAGS=-I$(ROOT_DIR) -I$(ROOT_DIR)/include

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
	cd build/cudd && ./configure
	cd build/cudd && aclocal
	cd build/cudd && autoconf
	cd build/cudd && ./configure --enable-silent-rules --enable-shared --enable-obj
	cd build/cudd && make

cudd-install:
	cd build/cudd && make install

cudd-uninstall:
	cd build/cudd && make uninstall

benchmark: timescales rvbc2018

timescales:
	mkdir -p test/build
	cd test/build && rm -rf timescales && git clone https://github.com/doganulus/timescales.git
	cd test/build/timescales && make full FLAGS="--format=json"

timescales-clean:
	rm -rf test/build/timescales

rvbc2018:
	mkdir -p test/build
	cd test/build && rm -rf benchmark-challenge-2018 && git clone https://github.com/runtime-verification/benchmark-challenge-2018.git

rvbc2018-clean:
	rm -rf test/build/benchmark-challenge-2018

app: 
	mkdir -p bin
	$(CXX) $(CXXFLAGS_APPS) apps/$(name)/*.cpp -o bin/$(name) $(INCLUDE_FLAGS) -Iapps/$(name) $(LIB_FLAGS)

ryjson1:
	mkdir -p bin
	$(CXX) $(CXXFLAGS_APPS) apps/ryjson1/*.cpp -o bin/ryjson1 $(INCLUDE_FLAGS) $(LIB_FLAGS)

apps: ryjson1

apps-install:
	cp -a ./bin /usr/local/bin

test: test_main test_discrete_timed test_dense_timed test_discrete_timed_robustness test_dense_timed_robustness_0 test_discrete_timed_data test_dense_timed_data test_monitors coverage

test_main:
	mkdir -p test/build
	$(CXX) $(CXXFLAGS) -c test/test_main.cpp -o test/build/main.o

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

test_monitors:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) -std=c++11 main.o $(ROOT_DIR)/test/test_monitors.cpp -o test_monitors $(INCLUDE_FLAGS) -L/home/dogan/anaconda3/lib -lcudd 
	cd test/build && ./test_monitors -r compact

test_random:
	cd test/build && $(CXX) $(CXXFLAGS_TEST) -std=c++11 main.o $(ROOT_DIR)/test/test_random.cpp -o test_random $(INCLUDE_FLAGS) -L/home/dogan/anaconda3/lib -lcudd -lpython3.7m
	cd test/build && ./test_random -r compact

test_performance_discrete: test/timescales/discrete/multitime/*.txt
	for batchfile in $^ ; do \
        multitime -n 10 -b $${batchfile} >/dev/null; \
    done
		
test_performance_dense10: test/timescales/dense10/multitime/*.txt
	for batchfile in $^ ; do \
        multitime -n 10 -b $${batchfile} >/dev/null; \
    done

test_performance_dense100: test/timescales/dense100/multitime/*.txt
	for batchfile in $^ ; do \
        multitime -n 10 -b $${batchfile} >/dev/null; \
    done

tutorial:
	$(CXX) $(CXXFLAGS) apps/tutorial/door_open_warning/cpp/dense_tutorial_main.cpp -o apps/tutorial/door_open_warning/cpp/dense_tutorial $(INCLUDE_FLAGS) $(LIB_FLAGS)
	$(CXX) $(CXXFLAGS) apps/tutorial/door_open_warning/cpp/discrete_tutorial_main.cpp -o apps/tutorial/door_open_warning/cpp/discrete_tutorial $(INCLUDE_FLAGS) $(LIB_FLAGS)

python: 
	pip install .

python-pypi-upload: 
	python setup.py sdist bdist_wheel
	twine check dist/*
	twine upload dist/*

coverage:
	cd test/build && gcov -p -s .. -o . test_setting_discrete_timed.cpp test_setting_dense_timed.cpp test_setting_discrete_timed_robustness.cpp test_setting_dense_timed_robustness.cpp test_setting_discrete_timed_data.cpp test_setting_dense_timed_data.cpp test_monitors.cpp
	cd test/build && lcov --capture --quiet --directory . --output-file lcov.info  
	cd test/build && lcov --remove lcov.info "/usr/*" "$(ROOT_DIR)/third_party/*" "$(ROOT_DIR)/include/reelay/third_party/*" "$(ROOT_DIR)/test/*" --directory . --output-file lcov.info
	cd test/build && genhtml --ignore-errors source lcov.info --legend --title "commit SHA1"