LDFLAGS=-lncurses

all: clock

clock: clock.c
	cc -o clock clock.c $(LDFLAGS)

run: clock 
	./clock

clean: 
	rm clock 
