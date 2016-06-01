PLATFORM = PC_PLATFORM
#PLATFORM = IMAX6_PLATFORM

EXEC = ethernet

#CROSS = ~/bin/arm-none-linux-gnueabi-
CROSS = arm-linux-gnueabi-
EXTRA_FLAGS = -mcpu=cortex-a9 -march=armv7-a -mfpu=neon -mfloat-abi=softfp -DIMAX6_PLATFORM
EXTRA_LIBS =	lib/libipu_sf.a \
				lib/libvpu_sf.a

ifeq ($(PLATFORM), IMAX6_PLATFORM)
	CC = $(CROSS)g++
	CC_FLAGS = -c -O3 -std=c99 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -pthread $(EXTRA_FLAGS)
	LINK_FLAGS = -o $(EXEC) -pthread -lm $(EXTRA_FLAGS)

	StaticLIBS = $(EXTRA_LIBS)

else
	CC = gcc
	CC_FLAGS = -c -O3 -std=c99 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -pthread
	#LINK_FLAGS = -o $(EXEC) -pthread -lm
	LINK_FLAGS = -o $(EXEC)  -lrt 
endif


DEL = rm -rf

OBJS =	\
		TCPclient.c \
		


all:	$(OBJS)
	$(CC) $(OBJS) $(LINK_FLAGS)

clean: 	
	$(DEL) *.o *.out *~
	$(DEL) $(EXEC)

rebuild:
	make clean
	make all

.SUFFIXES: .o .c

.c.o:
	$(CC) $(CC_FLAGS) $<

