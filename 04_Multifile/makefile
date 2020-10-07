CC=cc
CFLAGS=-fPIC
LIBS_FILES=const.o fun.o
TRASH= *.o
GENERATES= prog prog-a prog-so liboutput_static.a liboutput.so


all: prog prog-a prog-so test

liboutput_static.a: $(LIBS_FILES)
	ar -rcs $@ $^

liboutput.so: $(LIBS_FILES)
	$(CC) -shared $(CFLAGS) $^ -o $@ 

prog: prog.o $(LIBS_FILES)

prog-a: prog.o liboutput_static.a
	$(CC) $(CFLAGS) -L. $<  -loutput_static -o $@ 

prog-so: prog.o liboutput.so
	$(CC) $(CFLAGS) -L. $< -loutput -o $@

test: prog prog-a prog-so
	export LD_LIBRARY_PATH="$LD_LIBRARY_PATH;`pwd`"
	./test.sh $^

clean:
	rm -f $(TRASH)

distclean: clean
	rm -f $(GENERATES)

