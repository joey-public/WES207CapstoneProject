CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic
LDFLAGS=-lboost_thread -lboost_atomic -lpthread

SRC_FILES=../main.cpp
OBJ_FILES=$(SRC_FILES:.cpp=.o)
EXECUTABLE=main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

