#
# Soubor vychazi z makefilu knihovny rscode
#
#

RANLIB = ranlib
AR = ar


VERSION = 1.0
DIRNAME= rscode-$(VERSION)


CC = gcc
# OPTIMIZE_FLAGS = -O69
DEBUG_FLAGS = -g
CFLAGS = -std=c99 -Wall -Wstrict-prototypes  $(OPTIMIZE_FLAGS) $(DEBUG_FLAGS) -I..
LDFLAGS = $(OPTIMIZE_FLAGS) $(DEBUG_FLAGS)

LIB_CSRC = rs.c galois.c berlekamp.c crcgen.c
LIB_HSRC = ecc.h
LIB_OBJS = rs.o galois.o berlekamp.o crcgen.o

TARGET_LIB = libecc.a

#koder a dekoder
TEST_PROGS = bms1A bms1B


TARGETS = $(TARGET_LIB) $(TEST_PROGS)

all: $(TARGETS)

$(TARGET_LIB): $(LIB_OBJS)
	$(RM) $@
	$(AR) cq $@ $(LIB_OBJS)
	if [ "$(RANLIB)" ]; then $(RANLIB) $@; fi

bms1A: bms1A.o galois.o berlekamp.o crcgen.o rs.o
	gcc -o bms1A bms1A.o -L. -lm -lecc

bms1B: bms1B.o galois.o berlekamp.o crcgen.o rs.o
	gcc -o bms1B bms1B.o -L. -lm -lecc

clean:
	rm -f *.o bms1A bms1B libecc.a
	rm -f *~

dist:
	(cd ..; tar -cvf rscode-$(VERSION).tar $(DIRNAME))

depend:
	makedepend $(SRCS)



