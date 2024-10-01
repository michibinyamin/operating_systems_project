# Compiler and flags
CXX = g++
CXXFLAGS = -pedantic -pthread #-fprofile-arcs -ftest-coverage 
LDFLAGS = -lgcov

# Executable name
TARGET = main

# Source files
SOURCES = main.cpp Graph.cpp Proactor.cpp KruskalAlgorithm.cpp PrimAlgorithm.cpp MSTFactory.cpp Functions.cpp #Pipeline.cpp LF.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Build target
all: $(TARGET)

# Link the target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run gcov to generate coverage
coverage: $(TARGET)
	./$(TARGET)
	gcov main.cpp

# Clean up build files and coverage files
clean:
	rm -f $(TARGET) $(OBJECTS) *.gcda *.gcno *.gcov

# Phony targets
.PHONY: all clean coverage
