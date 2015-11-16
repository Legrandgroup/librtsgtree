#          Makefile for the librtsgtree project
#
# ----------------------------------------------------------------------------
#
# Note that this is a GNU Makefile.
# nmake and other abominations are not supported.
#
# ----------------------------------------------------------------------------

define HELP

 libsgtree project makefile help
 ------------------------------------

 In order to test with cpputest, run the following command:
 CFLAGS="-DUSE_CPPUTEST `pkg-config --cflags cpputest`" LIBS="`pkg-config --libs cpputest`" CPPUTEST_STATIC_LIB=/path/to/libCppUTest.a make CC=g++ <target>
 Please use `make <target>' where <target> is one of:


  --- build commands
  all              build lib and its tests
  test             build tests 
  clean            remove binaries (lib and tests)
  clean-all        remove binaries and object files
  rebuild          clean all and build

endef
export HELP

CFLAGS += -DIPV6_SUPPORT -DIPV4_SUPPORT

GIT_HASH = $(shell git log -1 --format=%h)

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

# Project name
PROJECT_NAME = librtsgtree

# Directories definition
HOME_DIR = .

INCLUDES += -I $(HOME_DIR)/src/

# keep this rule first
help:
	@echo "$$HELP"

# ----------------------------------------------------------------------------
#
# Documentation related tasks

.PHONY: all clean

# ----------------------------------------------------------------------------
#
# Build related tasks

# get rid of built-in rules
.SUFFIXES:

SRC_PATH = src
SRCS +=     $(SRC_PATH)/node.c \
            $(SRC_PATH)/uint128.c

TEST_SRCS +=$(SRC_PATH)/node_tests.c \
            $(SRC_PATH)/uint128_arithm_tests.c \
            $(SRC_PATH)/test_librtsgtree.c \

all: $(PROJECT_NAME) test Makefile

OBJS = $(patsubst %.c, %.o, $(SRCS))
TEST_OBJS = $(patsubst %.c, %.o, $(TEST_SRCS)) $(CPPUTEST_STATIC_LIB)

%.o: %.c
	@echo Compiling $<
	$(SILENCE)$(CC) -c  $(CFLAGS) $< -o $@


$(PROJECT_NAME) : $(OBJS)
	@echo Linking $@
	$(SILENCE)$(AR) cr $(PROJECT_NAME).a $^

rebuild: clean-all all

clean:
	@rm -f $(PROJECT_NAME).a
	@rm -f test

clean-all: clean
	@find $(HOME_DIR)/src -name "*.o" -exec rm -f {} \;

test: $(OBJS) $(TEST_OBJS)
	@echo Linking $@
	$(SILENCE)$(CC) $(LIBS) $(CFLAGS) $(LINKER_FLAGS) -o $@ $^
	./$@
