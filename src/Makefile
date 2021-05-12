IDIR = ../include
CC = gcc
CFLAGS= -I$(IDIR)

ODIR=obj
LDIR = ../lib

LIBS=-lm

_DEPS = baseconversions.h hex.h octal.h binary.h
DEPS = $(patsubst %, $(IDIR)/%,$(_DEPS))

_OBJ = baseconversions.o hex.o octal.o binary.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)  	

convert: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
