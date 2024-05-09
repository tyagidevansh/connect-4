CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Werror
LDFLAGS=-lws2_32

SRCS=server.cpp
OBJS=$(SRCS:.cpp=.o)

all: connect4_server

connect4_server: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

server.o: server.cpp position.hpp solver.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o connect4_server


# CXX=g++
# CXXFLAGS=--std=c++11 -Wall -Wextra -Werror

# SRCS=solver.cpp
# OBJS=$(SRCS:.cpp=.o)

# all: solver

# solver: $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o solver $(OBJS)

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f *.o solver

# run: solver
# 	./solver

# .PHONY: all clean run