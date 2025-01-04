CXX = g++
CXXFLAGS = -Wall -g

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

# Name of the final binary
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
