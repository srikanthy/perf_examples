
#
# Makefile - perf examples
#
# author: srikanth yalavarthi
# last modified: 2019-10-06
# url: https://github.com/srikanthy/perf_examples.git
#

TOOLCHAIN		= gcc

ifeq ($(TOOLCHAIN), gcc)
CPP         = /lib/cpp -P
CC          = gcc
ARCH_FLAGS  =
OMP_FLAGS   =
DEBUG_FLAGS = -Wno-pedantic -g -Wall -Wextra --save-temps
OPT_FLAGS   = $(OMP_FLAGS) -O0 -fno-tree-vectorize
endif

ifeq ($(TOOLCHAIN), llvm)
CPP         = clang -E
CC          = clang
ARCH_FLAGS  =
OMP_FLAGS   =
DEBUG_FLAGS = -Wno-pedantic -g -Wall -Wextra --save-temps
OPT_FLAGS   = $(OMP_FLAGS) -O0 -fno-vectorize
endif

INCDIR			=
LIBDIR			=
LIBS        =
CPPFLAGS    = $(ARCH_FLAGS) $(INCDIR)
CFLAGS      = $(DEBUG_FLAGS) $(OPT_FLAGS)
LDFLAGS     = $(LIBDIR) $(LIBS) $(OMP_FLAGS)
RM          = rm -rf
LN          = ln -sf

# source files
LIB_HDR =
LIB_SRC =
EXE_SRC = pe_hw_cpu_cycles.c \
					pe_hw_instructions.c \
					pe_hw_cache_misses.c \
					pe_llc_stats.c

# derived files
EXE = $(EXE_SRC:.c=.x)
PPS = $(EXE_SRC:.c=.i) $(LIB_SRC:.c=.i)
ASM = $(EXE_SRC:.c=.s) $(LIB_SRC:.c=.s)
OBJ = $(EXE_SRC:.c=.o) $(LIB_SRC:.c=.o)

# build targets
all: $(EXE)

# build rules
%.x : %.c
	$(CPP) $(CPPFLAGS) $< > $*.i
	$(CC) $(CFLAGS) -c $*.i -o $*.o
	$(CC) $(LDFLAGS) -o $@ $*.o

# additional rules
clean:
	$(RM) $(OBJ)
	$(RM) $(ASM)
	$(RM) $(PPS)

distclean: clean
	$(RM) $(EXE)
