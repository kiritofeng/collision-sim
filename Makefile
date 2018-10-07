CC=gcc
CXX=g++
RM=rm -f
CXXFLAGS=-O2

SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main $(OBJS)

main.o: main.cpp debug.h physics_h/physics.h

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) main

