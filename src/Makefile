#
# Makefile for non-Microsoft compilers
#

include Makefile.config

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%,$(SRCS))
all: $(OBJS)

$(OBJS): %:%.cpp Snap.o
	$(CC) $(CXXFLAGS) -o $@ $< Snap.o -I$(GLIB) -I$(SNAP) $(LDFLAGS) $(LIBS)


Snap.o: snap-core/*.h snap-core/*.cpp glib-core/*.h glib-core/*.cpp
	$(CC) -c $(CXXFLAGS) snap-core/Snap.cpp -I$(GLIB)

lib: Snap.o
	rm -f libsnap.a
	ar -cvq libsnap.a Snap.o

libinstall: lib
	cp libsnap.a /usr/local/lib/libsnap.a

clean:
	rm -f *.a *.exe *.o *.plt *.tab *.png

