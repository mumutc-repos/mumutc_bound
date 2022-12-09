CXX = g++

CFLAGS= -std=c++0x

#ROOT_CFLAGS=$(shell root-config --cflags)
#ROOT_LDFLAGS=$(shell root-config --ldflags)
#ROOT_LIBS=$(shell root-config --libs)

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))

EXEC=main

all:$(OBJS) $(EXEC)

$(EXEC): $(OBJS)
#	$(CXX) $^ -o $@ $(ROOT_LDFLAGS) $(ROOT_LIBS)
	$(CXX) $^ -o $@

$(OBJS): $(SRCS)
#	$(CXX) $(ROOT_CFLAGS) -c $^
	$(CXX) $(CFLAGS) -c $^

.PHONY:clean
clean:
	rm *.o $(EXEC)
