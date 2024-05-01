CXX=g++
CXXFLAGS=--std=c++11 -Wall -Wextra -Werror

SRCS=solver.cpp
OBJS=$(SRCS:.cpp=.o)

all: solver

solver: $(OBJS)
	$(CXX) $(CXXFLAGS) -o solver $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o solver

run: solver
	./solver

.PHONY: all clean run
