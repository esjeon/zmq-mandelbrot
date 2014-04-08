
CXX ?= c++
CXXFLAGS = -ggdb -std=c++11
LDFLAGS = -lzmq

SRC = vent.cpp work.cpp sink.cpp
BIN = ${SRC:.cpp=}

all: ${BIN}

.PHONY: clean
clean:
	rm -f ${BIN}
	rm -f *.o
