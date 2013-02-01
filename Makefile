CXX = g++
CXXFLAGS = -Wall

all: comp
	

comp: pl0
	$(CXX) $(CXXFLAGS) -o bin/pl0 src/pl0.cpp lib/grammar.o

grammar.o:
	$(CXX) -o  $(CXXFLAGS) -I./lib/

morphology.o:
	$(CXX) -c #(CXXFLAGS) morphology.c -I./lib/

clean:
	rm *.o
