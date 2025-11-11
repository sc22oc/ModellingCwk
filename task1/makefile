CC = g++

CCFLAGS = -Wall -Werror -g -lm

all: face2faceindex faceindex2directedge

face2faceindex: face2faceindex.o
	$(CC) $(CCFLAGS) $^ -o $@

faceindex2directedge: faceindex2directedge.o
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm *.o

