class DirectedEdge
{
 public:
  int id = -1;
  // the vertex the edge points towards
  int vertexID = -1;
  // the opposing half edge (i.e. the edges twin)
  int twinID = -1;

  bool isVisited = false;
  bool paired = false;

  DirectedEdge();
  DirectedEdge(int edgeID, int toVertex, int currentFaceID);

  // access the next and prev through module arithmetic
  int next();
  int prev();
  int face();
};
