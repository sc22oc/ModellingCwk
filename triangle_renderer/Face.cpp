#include "Face.h"

Face::Face(){}

Face::Face(int faceID, std::vector<int> inputIDs){
  id = faceID;
  vertexIDs = inputIDs;
}
