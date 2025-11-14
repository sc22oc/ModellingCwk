#include "Vertex.h"

Vertex::Vertex() : point(Cartesian3())
{}

Vertex::Vertex(int vertexID, float x, float y, float z){
  id = vertexID;
  point.x = x;
  point.y = y;
  point.z = z;
}

Vertex::Vertex(float x, float y, float z){
  point.x = x;
  point.y = y;
  point.z = z;
}
