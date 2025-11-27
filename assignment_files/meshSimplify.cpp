#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <math.h>
#include <cfloat>
#include <limits.h>

#include "../triangle_renderer/DirectedEdge.h"
#include "../triangle_renderer/Face.h"
#include "../triangle_renderer/Vertex.h"

std::vector<Vertex> oneRing(std::vector<DirectedEdge> dirEdgeInput, std::vector<Vertex> vertexInput, int startID){
  std::vector<Vertex> res;

  DirectedEdge currentEdge = dirEdgeInput[startID];
  int currentID = -1;

  while(currentID != startID){
    DirectedEdge prevEdge = dirEdgeInput[currentEdge.prev()];

    // assuming that we have a manifold mesh, won't need to check for boundaries

    currentEdge = dirEdgeInput[prevEdge.twinID];
    currentID = currentEdge.id;

    // push back the vertex to get neighbouring vertices
    res.push_back(vertexInput[currentEdge.vertexID]);
  }

  return res;
}

int main(int argc, char* argv[]){
  if (argc != 2) {
    std::cout << "Usage: ./meshSimplify <filepath>" << std::endl;
    return 0;
  }

  std::vector<Vertex> vertexInput;
  std::vector<Face> faceInput;
  std::vector<DirectedEdge> dirEdgeInput;
  std::vector<int> fdeInput;
  std::vector<int> halfInput;

  std::filesystem::path filePath(argv[1]);

  if (filePath.extension().compare(".diredge") != 0) {
    std::cout << "Error: .diredge file type required for manifold test"
              << std::endl;
    return 1;
  }

  std::ifstream inputFile(filePath, std::ios::in);
  std::string inputType;

  float i1, i2, i3;
  int id;
  int currentLine = 0;

  std::string strLine;

  if (inputFile.is_open()) {
    while (std::getline(inputFile, strLine)) {

      if (strLine[0] == '#')
        continue;

      std::stringstream ss(strLine);
      ss >> inputType >> id >> i1 >> i2 >> i3;

      if (inputType.compare("Vertex") == 0) {
        vertexInput.push_back(Vertex(id, i1, i2, i3));
      } else if (inputType.compare("FirstDirectedEdge") == 0) {
        fdeInput.push_back(i1);
      } else if (inputType.compare("Face") == 0) {
        faceInput.push_back(Face(id, (std::vector<int>){(int)i1, (int)i2, (int)i3}));
      } else if (inputType.compare("OtherHalf") == 0) {
        halfInput.push_back(i1);
      } else {
        std::cout << "Error: invalid line format on line" << currentLine
                  << std::endl;

        return 1;
      }

      currentLine++;
    }

    inputFile.close();
  } else {
    std::cout << "Error: failed to read file <"
              << (std::string)filePath.filename() << ">" << std::endl;
    return 1;
  }

  // OPTIONAL: vertex degree?

  int j = 0;
  for (size_t i = 0; i < faceInput.size(); i++) {
    std::vector<int> v = faceInput[i].vertexIDs;

    // number is respect the current face
    DirectedEdge e0(j + 0, v[0], i);
    DirectedEdge e1(j + 1, v[1], i);
    DirectedEdge e2(j + 2, v[2], i);

    dirEdgeInput.push_back(e0);
    dirEdgeInput.push_back(e1);
    dirEdgeInput.push_back(e2);
    j += 3;
  }

  if (dirEdgeInput.size() != halfInput.size()) {
    std::cout << "Error: insufficient number of edge pairings specified"
              << std::endl;
    return 1;
  }

  if (fdeInput.size() != vertexInput.size()) {
    std::cout << "Error: insufficient number of vertices or FDEs specified"
              << std::endl;
    return 1;
  }

  int e = 0;
  for (auto &de : dirEdgeInput) {
    de.twinID = halfInput[e];
    e++;
  }
  e = 0;
  for (auto &v : vertexInput) {
    v.fdeID = fdeInput[e];
    e++;
  }

  // test the one ring of each vertex, we'll calculate curvature here
  int minCurveID = INT_MAX;
  float minCurvature = FLT_MAX;

  for(auto v : vertexInput){

    // calculate mean gaussian curvature
    // get the one ring of the vertex and store it
    float totalArea = 0;
    float totalAngle = 0;
    std::vector<Vertex> ringVertices = oneRing(dirEdgeInput, vertexInput, v.fdeID);

    if(ringVertices.size() == 0) continue;

    std::cout << "------------------------" << std::endl;

    std::cout << "Vertex: " << v.id << " " << std::endl << "[ ";
    for(auto v2 : ringVertices){
      std::cout << v2.id << " ";
    }
    std::cout << "]" << std::endl;

    // construct vectors
    for(size_t i = 0; i < ringVertices.size(); i++){
      Cartesian3 v1 = vertexInput[ringVertices[i].id].point - v.point;
      Cartesian3 v2 = vertexInput[ringVertices[(i+1) % ringVertices.size()].id].point - v.point;

      // append the area
      totalArea += (std::sqrt(v1.cross(v2).length())) / 2.f;

      // append the angle
      totalAngle += std::acos( v1.dot(v2) / ( std::sqrt(v1.length()) * std::sqrt(v2.length()) ) );

      /*
      std::cout << "Current vertex " << vertexInput[ringVertices[i].id].id << " | " << vertexInput[ringVertices[i].id].point << std::endl;
      std::cout << "------------------------" << std::endl;
      std::cout << "Vector: " << v.id << ", " << vertexInput[ringVertices[i].id].id << std::endl;
      std::cout << "Vector: " << v.id << ", " << vertexInput[ringVertices[(i+1) % ringVertices.size()].id].id << std::endl;
      std::cout << "Centre point: " << v.point << std::endl;
      std::cout << "Point 1: " << vertexInput[ringVertices[i].id].point << std::endl;
      std::cout << "Point 2: " << vertexInput[ringVertices[(i+1) % ringVertices.size()].id].point << std::endl;
      std::cout << "dot: " << v1.dot(v2) << std::endl;
      std::cout << "v1: " << v1 << std::endl;
      std::cout << "v2: " << v2 << std::endl;
      std::cout << "v1 length: " << std::sqrt(v1.length()) << std::endl;
      std::cout << "v2 length: " << std::sqrt(v2.length()) << std::endl;
      std::cout << "angle: " <<  std::acos(v1.dot(v2) / (std::sqrt(v1.length()) * std::sqrt(v2.length()))) << std::endl; 
      */
    }

    float gaussCurvature = std::abs((2 * M_PI - totalAngle) / totalArea);

    std::cout << "total area: " << totalArea << std::endl;
    std::cout << "total angle: " << totalAngle << std::endl;
    std::cout << "calc curvature vertex " << v.id << ": " << gaussCurvature << std::endl;

    if(gaussCurvature < minCurvature){
      minCurveID = v.id;
      minCurvature = gaussCurvature;
    }
  }

  std::cout << "------------------------" << std::endl;
  std::cout << "lowest curvature is vertex: " << minCurveID << " curvature (" << minCurvature << ")" << std::endl;

  // now that we've found the vertex with least curvature, choose the vertex to delete
  std::vector<Vertex> ringFromGauss = oneRing(dirEdgeInput, vertexInput, vertexInput[minCurveID].fdeID);

  int deletionID = -1;

  std::cout << "size of gauss: " << ringFromGauss.size() << std::endl;

  for(auto v1 : ringFromGauss){

    std::vector<Vertex> ringVertices = oneRing(dirEdgeInput, vertexInput, v1.fdeID);
    int sharedValues = 0;

    for(auto v2 : ringFromGauss){

      if(v2.id == minCurveID) continue;

      for(auto v3 : ringVertices){

	if(v3.id == v1.id) continue;

	if(v2.id == v3.id) {
	  sharedValues++;
	}

      }
    }

    std::cout << "Vertex: " << v1.id << " | " << "sharedValues: " << sharedValues << std::endl;

    if(sharedValues == 2){
      deletionID = v1.id;
      break;
    }
  }

  std::cout << "Vertex to delete: " << deletionID << std::endl;

  // re-arrange the entire mesh (please i do not want to re-arrange the edge to collapse)

  return 0;
}
