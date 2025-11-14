#include <vector>

class Face
{
 public:
  int id = -1;
  int halfEdgeID = -1; 
  std::vector<int> vertexIDs;
  Face();
  Face(int faceID, std::vector<int> inputIDs);
};
