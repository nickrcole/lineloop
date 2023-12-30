IDIR =../include
CC=gcc
CFLAGS=-fcommon -Wall -I$(IDIR)

ODIR=../obj
LDIR =../lib

# LIBS=-lm

_DEPS = rpi_dma_utils.h rpi_smi_defs.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = rpi_dma_utils.o rpi_pixleds.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rpi_pixleds: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

# CC=gcc
# CFLAGS=-Wall -fcommon -I.
# DEPS = rpi_dma_utils.h
# OBJ = rpi_dma_utils.o rpi_pixleds.o 

# %.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# rpi_pixleds: $(OBJ)
# 	$(CC) -o $@ $^ $(CFLAGS)