CXX = gcc
CXXFLAGS = -O3 -march=native -funroll-all-loops -ftree-vectorize -funsafe-math-optimizations -march=native -flto -mtune=native -fopenmp -DNDEBUG $(shell pkg-config --cflags opencv4)
LDFLAGS = 

SRC = main.c

all: stltodesmos

stltodesmos: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f stltodesmos

