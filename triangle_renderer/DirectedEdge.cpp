#include "DirectedEdge.h"

DirectedEdge::DirectedEdge(){};

DirectedEdge::DirectedEdge(int edgeID, int toVertex, int currentFaceID){
  id = edgeID;
  vertexID = toVertex;
}

int DirectedEdge::next(){
  return (id / 3) * 3 + (id + 1) % 3;
}

int DirectedEdge::prev(){
  return (id / 3) * 3 + (id + 2) % 3;
}

int DirectedEdge::face(){
  // this will get clamped (due to integer dvision)
  return id / 3;
}
