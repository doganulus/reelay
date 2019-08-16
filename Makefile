CXX=g++ # C compiler
CXXFLAGS=-std=c++17 -fPIC -O2 -g -pthread -fno-new-ttp-matching # -Wall -Wextra  C flags
LDFLAGS=-shared  # linking flags

LIB_FLAGS=-lreelay -lantlr4-runtime
INCLUDE_FLAGS=-I./include -I/usr/local/include/antlr4-runtime

NAME = reelay
VERSION = 1.2
TARGET = libreelay.so # target lib

PROJECT_INCLUDE = include/reelay

PREFIX = /usr/local
INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
WORKDIR = ./build

ANTLR4_EXE = $(LIBDIR)/antlr-4.7.1-complete.jar
ANTLR4_FLAGS = -Dlanguage=Cpp -visitor -Xexact-output-dir
ANTLR4_DIR = antlr4
ANTLR4_GRAMMAR_DIR = $(ANTLR4_DIR)/grammar

ANTLR4_TL_GRAMMAR = TemporalLogic
ANTLR4_TL_SOURCES = $(ANTLR4_DIR)/temporal_logic
ANTLR4_TL_HEADERS = include/reelay/parser/antlr4/temporal_logic

SOURCES = $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Lexer.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Parser.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Listener.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)BaseListener.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)Visitor.cpp $(ANTLR4_TL_SOURCES)/$(ANTLR4_TL_GRAMMAR)BaseVisitor.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean develop test install uninstall antlr4 apps

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -f $(ANTLR4_TL_SOURCES)/*.o
	rm -f $(TARGET)

install:
	mkdir -p $(DESTDIR)$(INCDIR)/$(NAME)

	cp -a $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)
	cp -p $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET)

uninstall:
	rm -rf $(DESTDIR)$(INCDIR)/$(NAME) 
	rm -rf $(DESTDIR)$(LIBDIR)/$(TARGET)

develop:
	ln -s $(PROJECT_INCLUDE) $(DESTDIR)$(INCDIR)
	ln -s $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET)

antlr4:
	mkdir -p $(ANTLR4_TL_SOURCES)	
	java -jar $(ANTLR4_EXE) $(ANTLR4_FLAGS) -o $(ANTLR4_TL_SOURCES) $(ANTLR4_GRAMMAR_DIR)/$(ANTLR4_TL_GRAMMAR).g4 
	mkdir -p $(ANTLR4_TL_HEADERS)
	cp -a $(ANTLR4_TL_SOURCES)/*.h $(ANTLR4_TL_HEADERS)
	make all

antlr4-clean:
	rm -rf $(ANTLR4_TL_SOURCES)
	rm -rf $(ANTLR4_TL_HEADERS)

apps:
	$(CXX) $(CXXFLAGS) apps/mtl/rymtl.cpp -o bin/rymtl $(INCLUDE_FLAGS) $(LIB_FLAGS)

test: test_main

test_main:
	$(CXX) $(CXXFLAGS) -c test/test_main.cpp -o test/main.o $(INCLUDE_FLAGS) $(LIB_FLAGS)

test_untimed:
	$(CXX) $(CXXFLAGS) test/main.o test/test_setting_untimed.cpp -o test/test_setting_untimed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./test/test_setting_untimed -r compact

test_robustness:
	$(CXX) $(CXXFLAGS) test/main.o test/test_setting_robustness.cpp -o test/test_setting_robustness $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./test/test_setting_robustness -r compact

test_discrete_timed:
	$(CXX) $(CXXFLAGS) test/main.o test/test_setting_discrete_timed.cpp -o test/test_setting_discrete_timed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./test/test_setting_discrete_timed -r compact

test_discrete_timed_robustness:
	$(CXX) $(CXXFLAGS) test/main.o test/test_setting_discrete_timed_robustness.cpp -o test/test_setting_discrete_timed_robustness $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./test/test_discrete_timed_robustness -r compact

test_dense_timed:
	$(CXX) $(CXXFLAGS) test/main.o test/test_setting_dense_timed.cpp -o test/test_setting_dense_timed $(INCLUDE_FLAGS) $(LIB_FLAGS)
	./test/test_setting_dense_timed -r compact