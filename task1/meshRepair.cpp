#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../triangle_renderer/DirectedEdge.h"
#include "../triangle_renderer/Face.h"
#include "../triangle_renderer/Vertex.h"

int oneBoundary(std::vector<DirectedEdge> dirEdgeInput, int startID) {

  // traverse the one ring until we reach a boundary
  DirectedEdge currentEdge = dirEdgeInput[startID];
  int currentID = -1;

  while (currentID != startID) {
    DirectedEdge prevEdge = dirEdgeInput[currentEdge.prev()];

    if (prevEdge.twinID == -1) {
      // store the directed edge who has the boundary of its pair
      return prevEdge.id;
    }

    currentEdge = dirEdgeInput[prevEdge.twinID];
    currentID = currentEdge.id;
  }

  // if there is no boundary, return 0
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./meshRepair <filepath>" << std::endl;
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

  int i1, i2, i3;
  int id;
  int currentLine = 0;

  std::string strLine;

  // PHASE 1: Parse the file
  if (inputFile.is_open()) {
    while (std::getline(inputFile, strLine)) {

      if (strLine[0] == '#')
        continue;

      std::stringstream ss(strLine);
      ss >> inputType >> id >> i1 >> i2 >> i3;

      if (inputType.compare("Vertex") == 0) {
        vertexInput.push_back(Vertex(id, (float)i1, (float)i2, (float)i3));
      } else if (inputType.compare("FirstDirectedEdge") == 0) {
        fdeInput.push_back(i1);
      } else if (inputType.compare("Face") == 0) {
        faceInput.push_back(Face(id, (std::vector<int>){i1, i2, i3}));
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

  // PHASE 2: DATA CONSTRUCTION
  // making sure things are nice and tidy to do testing
  for (auto f : faceInput) {
    for (auto vID : f.vertexIDs) {
      vertexInput[vID].degree++;
    }
  }

  // construct the directed edges
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

  std::vector<std::vector<int>> holes;

  for (auto &d : dirEdgeInput) {
    std::vector<int> boundaryEdgeIDs;

    // we've found another half
    if (d.twinID == -1 && !d.isVisited) {

      int startID = d.id;
      int nextStartID = -1;

      while (nextStartID != startID) {
        if (nextStartID != -1)
          nextStartID = oneBoundary(dirEdgeInput, nextStartID);
        else
          nextStartID = oneBoundary(dirEdgeInput, startID);

        boundaryEdgeIDs.push_back(nextStartID);
      }

      std::cout << "found hole: [ ";
      for (auto e : boundaryEdgeIDs) {
        dirEdgeInput[e].isVisited = true;
        std::cout << e << " ";
      }
      std::cout << "]" << std::endl;

      holes.push_back(boundaryEdgeIDs);
    }
  }

  // if there are holes, find the central vertex and perform insertion
  if (!holes.empty()) {

    for (auto h : holes) {

      // take the position of each vertex to calculate the center
      int holeDegree = 0;
      Cartesian3 vertexTotal{};

      for (auto e : h) {
        vertexTotal = vertexTotal + vertexInput[dirEdgeInput[e].vertexID].point;
        std::cout << "Boundary point: "
                  << vertexInput[dirEdgeInput[e].vertexID].point << std::endl;
        holeDegree++;
      }

      Cartesian3 centreVertex = vertexTotal / holeDegree;

      int centreID = vertexInput.size();

      vertexInput.push_back(
          Vertex(centreID, centreVertex.x, centreVertex.y, centreVertex.z));

      std::cout << "Average center: " << centreVertex << std::endl;

	  // check the start point of the new directed edges
	  // we'll need this to set the FDE of the new vertex
      int startEdgeID = dirEdgeInput.size();

      // construct # faces equal to the whole degree
      for (int i = 0; i < holeDegree; i++) {

        // form the vertices of the triangle (question: how do we access the new vertices?)
        int v0 = dirEdgeInput[dirEdgeInput[h[i]].prev()].vertexID;
        int v1 = centreID;
        int v2 = dirEdgeInput[h[i]].vertexID;

        // note, we can't push this back yet as we still need the halfEdgeID?
        int faceID = faceInput.size();
        int edgeID = dirEdgeInput.size();

        // after finding the average center, compute faces
        // generate a face with its respective directed edges
        Face insertFace(faceID, std::vector<int>{v0, v1, v2});
        faceInput.push_back(insertFace);

        DirectedEdge e0(edgeID, v0, faceID);
        DirectedEdge e1(edgeID + 1, v1, faceID);
        DirectedEdge e2(edgeID + 2, v2, faceID);

        dirEdgeInput.push_back(e0);
        dirEdgeInput.push_back(e1);
        dirEdgeInput.push_back(e2);
      }

	  // set the twins for each of the new edges
      for (auto &d1 : dirEdgeInput) {

        if (d1.twinID != -1)
          continue;

        for (auto &d2 : dirEdgeInput) {

          if (dirEdgeInput[d1.prev()].vertexID == d2.vertexID &&
              dirEdgeInput[d2.prev()].vertexID == d1.vertexID) {

            if (d2.twinID != -1) {
              continue;
            }

            d1.twinID = d2.id;
            d2.twinID = d1.id;

            std::cout << "OtherHalf " << d1.id << " " << d1.twinID << std::endl;
            break;
          }
        }
      }

	  // set the new FDE for the added vertex
	  // vertexInput.back().fdeID 
	  for(size_t i = startEdgeID; i < dirEdgeInput.size(); i++){
		int currentVertexTo = dirEdgeInput[dirEdgeInput[i].prev()].vertexID;
		if(currentVertexTo == centreID){
		  vertexInput.back().fdeID = dirEdgeInput[i].id;
		  break;
		}
	  }
    }
  }

  std::string objectName = (std::string)filePath.stem();
  std::string outputFileName = objectName + "_fixed.diredge";
  std::ofstream outputFile(outputFileName, std::ios::out);

  if (outputFile.is_open()) {
    outputFile << "# University of Leeds 2022-2023" << std::endl;
    outputFile << "# COMP 5812 Assignment 1" << std::endl;
    outputFile << "# Oliver Cheung " << std::endl;
    outputFile << "# 201597566" << std::endl;
    outputFile << "#" << std::endl;
    outputFile << "# Object Name: " << objectName << std::endl;
    outputFile << "# Vertices=" << vertexInput.size()
               << " Faces=" << faceInput.size() << std::endl;
    outputFile << "#" << std::endl;

    for (auto v : vertexInput) {
      outputFile << "Vertex " << v.id << "\t" << v.point << std::endl;
    }

	for (auto v : vertexInput) {
	  outputFile << "FirstDirectedEdge " << v.id << "\t" << v.fdeID
				 << std::endl;
	}

    for (auto f : faceInput) {
      outputFile << "Face " << f.id << "\t";
      for (int i = 0; i < 3; i++) {
        outputFile << f.vertexIDs[i] << " ";
      }
      outputFile << std::endl;
    }

    for (auto de : dirEdgeInput) {
      outputFile << "OtherHalf " << de.id << "\t" << de.twinID << std::endl;
    }

    outputFile.close();

    std::cout << "File <" << outputFileName << "> written to successfully!"
              << std::endl;
  } else {
    std::cout << "Error: failed to write to a file: " << outputFileName
              << std::endl;
    return 1;
  }

  return 0;
}
