#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = example_nor_memory_flash_mockup

#--- Inputs ----#
PROJECT_HOME_DIR = ../
ifeq "$(CPPUTEST_HOME)" ""
$(error The environment variable CPPUTEST_HOME is not set. \
Set it to where cpputest is installed)
endif

# --- SRC_FILES and SRC_DIRS ---
# Production code files are compiled and put into
# a library to link with the test runner.
#
# Test code of the same name overrides
# production code at link time.
#
# SRC_FILES specifies individual production
# SRC_FILES += ../src/XXX.c

#
# SRC_DIRS specifies directories containing
# production code C and CPP files.
#
SRC_DIRS += ../src
SRC_DIRS += ../src/lfs

# --- TEST_SRC_FILES and TEST_SRC_DIRS ---
# Test files are always included in the build.
# Production code is pulled into the build unless
# it is overriden by code of the same name in the
# test code.
#
# TEST_SRC_FILES specifies individual test files to build.
TEST_SRC_FILES += ./all_tests.cpp
TEST_SRC_FILES += ./file_system.test.cpp
TEST_SRC_FILES += ./fake_memory_io.c

# TEST_SRC_DIRS, builds everything in the directory
# TEST_SRC_DIRS += tests/printf-spy

#	tests/example-fff \
#	tests/fff \

# --- MOCKS_SRC_DIRS ---
# MOCKS_SRC_DIRS specifies a directories where you can put your
# mocks, stubs and fakes.  You can also just put them
# in TEST_SRC_DIRS
# MOCKS_SRC_DIRS += ./Mocks/XXX

# Turn on CppUMock
CPPUTEST_USE_EXTENSIONS = Y

# INCLUDE_DIRS are searched in order after the included file's
# containing directory
INCLUDE_DIRS += $(CPPUTEST_HOME)/include
INCLUDE_DIRS += $(CPPUTEST_HOME)/include/Platforms/Gcc
INCLUDE_DIRS += ../src
INCLUDE_DIRS += .

# --- CPPUTEST_OBJS_DIR ---
# CPPUTEST_OBJS_DIR lets you control where the
# build artifact (.o and .d) files are stored.
#
# If you have to use "../" to get to your source path
# the makefile will put the .o and .d files in surprising
# places.
#
# To make up for each level of "../"in the source path,
# add place holder subdirectories to CPPUTEST_OBJS_DIR
# each.
# e.g. if you have "../../src", set to "test-objs/1/2"
#
# This is kind of a kludge, but it causes the
# .o and .d files to be put under objs.
CPPUTEST_OBJS_DIR = ./build/objects

CPPUTEST_LIB_DIR = ./build/libraries

# You may have to tweak these compiler flags
#    CPPUTEST_WARNINGFLAGS - apply to C and C++
#    CPPUTEST_CFLAGS - apply to C files only
#    CPPUTEST_CXXFLAGS - apply to C++ files only
#    CPPUTEST_CPPFLAGS - apply to C and C++ Pre-Processor
#
# If you get an error like this
#     TestPlugin.h:93:59: error: 'override' keyword is incompatible
#        with C++98 [-Werror,-Wc++98-compat] ...
# The compiler is basically telling you how to fix the
# build problem.  You would add this flag setting
#     CPPUTEST_CXXFLAGS += -Wno-c++14-compat

# Some flags to quiet clang
ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang"), 1)
CPPUTEST_WARNINGFLAGS += -Wno-unknown-warning-option
CPPUTEST_WARNINGFLAGS += -Wno-covered-switch-default
CPPUTEST_WARNINGFLAGS += -Wno-reserved-id-macro
CPPUTEST_WARNINGFLAGS += -Wno-keyword-macro
CPPUTEST_WARNINGFLAGS += -Wno-documentation
CPPUTEST_WARNINGFLAGS += -Wno-missing-noreturn
endif

CPPUTEST_WARNINGFLAGS += -Wall
CPPUTEST_WARNINGFLAGS += -Werror
CPPUTEST_WARNINGFLAGS += -Wfatal-errors
CPPUTEST_WARNINGFLAGS += -Wswitch-default
CPPUTEST_WARNINGFLAGS += -Wno-format-nonliteral
CPPUTEST_WARNINGFLAGS += -Wno-sign-conversion
CPPUTEST_WARNINGFLAGS += -Wno-pedantic
CPPUTEST_WARNINGFLAGS += -Wno-shadow
CPPUTEST_WARNINGFLAGS += -Wno-missing-field-initializers
CPPUTEST_WARNINGFLAGS += -Wno-unused-parameter
CPPUTEST_CFLAGS += -pedantic
CPPUTEST_CFLAGS += -Wno-missing-prototypes
CPPUTEST_CFLAGS += -Wno-strict-prototypes
CPPUTEST_CXXFLAGS += -Wno-c++14-compat
CPPUTEST_CXXFLAGS += --std=c++11
CPPUTEST_CXXFLAGS += -Wno-c++98-compat-pedantic
CPPUTEST_CXXFLAGS += -Wno-c++98-compat
CPPUTEST_CFLAGS += -g

CPPUTEST_CFLAGS += -DLFS_NO_ERROR

# Coloroze output
CPPUTEST_EXE_FLAGS += -c
CPPUTEST_EXE_FLAGS += -v

# --- LD_LIBRARIES -- Additional needed libraries can be added here.
# commented out example specifies math library
#LD_LIBRARIES += -lm

# Look at $(CPPUTEST_HOME)/build/MakefileWorker.mk for more controls

# Code coverage
CPPUTEST_USE_GCOV=Y
GCOV_ARGS += -b
GCOV_ARGS += -c

include $(CPPUTEST_HOME)/build/MakefileWorker.mk