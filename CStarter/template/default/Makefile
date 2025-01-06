flags=-02 -Wall -std=c2x
ldflags=-lbu

.PHONY: all clean

all: clean prjName

prjName : prjName.o
	cc $(flags) $^ -o $@ $(ldflags)

prjName.o: prjName.c prjName.h
	cc $(flags) -c $<

clean:
	rm -f *.o prjName