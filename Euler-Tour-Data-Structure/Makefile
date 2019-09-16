name = main
src = $(wildcard src/*.cpp)
incl = hdr
obj = $(src:.c=.o)

LEDA = '/usr/local/LEDA/incl'
LEDALIBS = '/usr/local/LEDA'

CC = g++
CFLAGS = -std=c++0x -O3
LIBFLAGS = -lleda -lm

DFLAGS = -DEXAMPLE

all: $(name)

$(name): $(obj)
	$(CC) -I$(incl) -o $@ $^ $(CFLAGS) -I$(LEDA) -L$(LEDALIBS) $(LIBFLAGS) $(DFLAGS)

run:
	./$(name)

clean:
	rm -f $(name)
