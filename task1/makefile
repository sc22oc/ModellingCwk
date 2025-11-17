CC = g++

CCFLAGS = -Wall -g -lm

TRIDIR = ../triangle_renderer

all: face2faceindex faceindex2directedge manifoldTest

face2faceindex: face2faceindex.o $(TRIDIR)/Cartesian3.o $(TRIDIR)/Vertex.o $(TRIDIR)/Face.o 
	$(CC) $(CCFLAGS) $^ -o $@

faceindex2directedge: faceindex2directedge.o $(TRIDIR)/Cartesian3.o $(TRIDIR)/Vertex.o $(TRIDIR)/Face.o $(TRIDIR)/DirectedEdge.o
	$(CC) $(CCFLAGS) $^ -o $@

manifoldTest: manifoldTest.o $(TRIDIR)/Cartesian3.o $(TRIDIR)/Vertex.o $(TRIDIR)/Face.o $(TRIDIR)/DirectedEdge.o
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm *.o

