#include "vertex.h"

Vertex::Vertex() : point(Cartesian3())
{}

Vertex::Vertex(float x, float y, float z){
  point.x = x;
  point.y = y;
  point.z = z;
}
