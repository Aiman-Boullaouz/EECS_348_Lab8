CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

TARGET = matrix
SRC = matrix.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run