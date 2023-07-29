##**************************************
## Makefile
##
## Build routine for pascalish compiler
##
## Author: Phil Howard 
## phil.howard@oit.edu
##

COPTS=-Wall -g -c  -O0 -std=c++11
OBJS=main.o \
	 pascalishlex.o \
	 pascalishparse.o \
	 cVisitor.o \
	 cSymbol.o \
	 initSymbolTable.o \
	 cOpNode.o \
	 cSemantics.o \
	 cComputeSize.o \
	 cCodeGen.o \
	 emit.o \

all: pascalish

clean:
	rm -f $(OBJS)
	rm -f pascalishlex.c
	rm -f pascalishparse.c
	rm -f pascalishparse.h
	rm -f pascalish
	rm -f out.xml
	rm -f out2.xml
	rm -f out

.cpp.o:
	g++ $(COPTS) $? -o $@

.c.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp pascalishparse.c pascalishlex.c 
	g++ $(COPTS) main.cpp -o $@

pascalishlex.o: pascalishlex.c
	g++ $(COPTS) -Wno-sign-compare $? -o $@

pascalishlex.c: pascalish.l pascalishparse.c
	flex -o pascalishlex.c pascalish.l

pascalishparse.c: pascalish.y
	bison --defines=pascalishparse.h pascalish.y -o pascalishparse.c

pascalish: $(OBJS)
	g++ $(OBJS) -o pascalish

