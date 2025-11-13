CC = g++

CCFLAGS = -Wall -g -lm

TRIDIR = ../triangle_renderer

all: face2faceindex faceindex2directedge

face2faceindex: face2faceindex.o $(TRIDIR)/Cartesian3.o $(TRIDIR)/vertex.o $(TRIDIR)/face.o 
	$(CC) $(CCFLAGS) $^ -o $@

faceindex2directedge: faceindex2directedge.o
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm *.o

