#include "Cartesian3.h"

class Vertex
{
 public:
  int id = -1;

  Cartesian3 point;

  // ID of the first directed edge
  int fdeID = -1;

  // current vertex degree (good for pinch point test)
  int degree = 0;

  bool isVisited = false;

  Vertex();
  Vertex(int vertexID, float x, float y, float z);
  Vertex(float x, float y, float z);
};
