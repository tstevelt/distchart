PROG = distchart
LIB = distchart.a
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)

FILES = \
	$(LIB)(distchart.o)	\
	$(LIB)(getargs.o)	\
	$(LIB)(getcfg.o)	\
	$(LIB)(shslib.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): distchart.h shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

