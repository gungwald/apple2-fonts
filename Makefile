# Builds circ demo

# Requirements:
# 1. Gmake must be used.
# 2. The cc65 compiler must be properly setup.

ifndef CC65_TARGET
	CC65_TARGET:=apple2enh
endif

CC=cl65
CFLAGS=-O -t $(CC65_TARGET) -DTRACE
# The -S $6000 makes the start address $6000 so that both hi-res
# pages are available.
LDFLAGS=-t $(CC65_TARGET) -u __EXEHDR__ --start-addr 0x6000
DISK_VOL=fonts
DISK=$(DISK_VOL).dsk
PGM=fonts
BASIC_AUX_TYPE=0x0801
READ_TIME_LOAD_ADDR=0x0260
AC=java -jar AppleCommander-ac-1.4.0.jar
SYS_LOAD_ADDR=0x2000
BIN_LOAD_ADDR=0x0803
FONT_HDRS=ibmpclike8.h ibmpclike12.h runes-english.h runes-anglo-saxon.h yolo.h salzalon.h

########################################

all: $(DISK)

$(DISK): $(PGM) 
	$(RM) $(DISK)
	$(AC) -pro140 $(DISK) $(DISK_VOL)
	$(AC) -as $(DISK) $(PGM) BIN < $(PGM)

$(PGM): $(PGM).o
	$(CC) $(LDFLAGS) -o $@ $^

$(PGM).o: $(PGM).c $(FONT_HDRS)
	$(CC) $(CFLAGS) $(PGM).c

salzalon.h: salzalon.txt mkfonthdr
	./mkfonthdr salzalon.txt > salzalon.h

yolo.h: yolo.txt mkfonthdr
	./mkfonthdr yolo.txt > yolo.h

ibmpclike8.h: ibmpclike8.txt mkfonthdr
	./mkfonthdr ibmpclike8.txt > ibmpclike8.h

ibmpclike12.h: ibmpclike12.txt mkfonthdr
	./mkfonthdr ibmpclike12.txt > ibmpclike12.h

runes-english.h: runes-english.txt mkfonthdr
	./mkfonthdr runes-english.txt > runes-english.h

runes-anglo-saxon.h: runes-anglo-saxon.txt mkfonthdr
	./mkfonthdr runes-anglo-saxon.txt > runes-anglo-saxon.h

mkfonthdr: mkfonthdr.c
	gcc -g -o mkfonthdr mkfonthdr.c -lm

clean:
	$(RM) *.o $(PGM) $(DISK) $(GRAF_DRVR).s $(FONT_HDRS) mkfonthdr

