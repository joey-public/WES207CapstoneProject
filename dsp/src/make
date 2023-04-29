CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic
LDFLAGS=-lboost_program_options -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -luhd

SRC_FILES=main.cpp
OBJ_FILES=$(SRC_FILES:.cpp=.o)
EXECUTABLE=program_name

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

