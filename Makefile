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

 Please use `make <target>' where <target> is one of:


  --- build commands
  all              build lib and its tests
  test             build tests 
  clean            remove binaries (lib and tests)
  clean-all        remove binaries and object files
  rebuild          clean all and build

endef
export HELP

GIT_HASH = $(shell git log -1 --format=%h)

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

# Project name
PROJECT_NAME = librtsgt

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
SRCS +=     $(SRC_PATH)/ipv6_node.c \
            $(SRC_PATH)/uint128.c \
            $(SRC_PATH)/utils.c 

TEST_SRCS +=$(SRC_PATH)/ipv6_node_tests.c \
            $(SRC_PATH)/node_utest.c \
            $(SRC_PATH)/uint128_arithm_tests.c \
            $(SRC_PATH)/librtsgtree.c \

all: $(PROJECT_NAME) test Makefile

OBJS = $(patsubst %.c, %.o, $(SRCS))
TEST_OBJS = $(patsubst %.c, %.o, $(TEST_SRCS))

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
