CXX = g++
CXXFLAGS = -Wall -g

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

# Name of the final binary
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) -lm

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -lm

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
