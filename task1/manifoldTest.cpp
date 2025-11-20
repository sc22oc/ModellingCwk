#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

#include "../triangle_renderer/DirectedEdge.h"
#include "../triangle_renderer/Face.h"
#include "../triangle_renderer/Vertex.h"

struct TestOutput {
  std::string meshName;
  int pinchID = -1;
  int edgeID = -1;
  int twinID = -1;
  int genus = 0;
  bool manifold = false;
  bool readSuccessful = true;
};

int oneRing(std::vector<DirectedEdge> dirEdgeInput, int startID) {
  // to store the degree of the one ring
  int degree = 0;

  // return -1 if the test finds a boundary
  // check if each pair has a twin
  // if the twinID is -1, that implies that the edge does not have a twin
  // so, the test will fail
  DirectedEdge currentEdge = dirEdgeInput[startID];

  int currentID = -1;

  /*
  std::cout << "start id: " << startID << std::endl;
  std::cout << "current id: " << currentID << std::endl;
  */

  while (currentID != startID) {

    // std::cout << "----------------------------" << std::endl;
    // std::cout << "current id: " << currentID << std::endl;

    DirectedEdge prevEdge = dirEdgeInput[currentEdge.prev()];

    // std::cout << "prev id: " << prevEdge.id << std::endl;

    currentEdge = dirEdgeInput[prevEdge.twinID];
    currentID = currentEdge.id;

    // std::cout << "next (twin) id: " << currentID << std::endl;

    degree++;
  }

  return degree;
}

int pinchTest(std::vector<Vertex> vertexInput,
              std::vector<DirectedEdge> dirEdgeInput) {

  for (auto v : vertexInput) {
    // count one ring
    int ringDegree = oneRing(dirEdgeInput, v.fdeID);

    /*
    std::cout << "----------------------------" << std::endl;
    std::cout << "Vertex: " << v.id << std::endl;
    std::cout << "Ring degree: " << ringDegree << std::endl;
    std::cout << "Vertex degree: " << v.degree << std::endl;
    */

    // compare ring degree to vertex degree
    // if they are not equal, test fails
    if (ringDegree != v.degree) {
      // std::cout << "Error: bad vertex at Vertex " << v.id << std::endl;
      return v.id;
    }
  }

  return -1;
}

// TASK 3
int genusTest(std::vector<DirectedEdge> dirEdgeInput, std::vector<Vertex> vertexInput, std::vector<Face> faceInput) {

  std::vector<int> vertexCounts;
  std::vector<int> faceCounts;

  for(auto &v : vertexInput){
    if(!v.isVisited){

      int vertexCount = 0;
      int faceCount = 0;

      std::queue<Vertex> vertexQueue;
      vertexQueue.push(v);

      v.isVisited = true;
      vertexCount++;

      while(!vertexQueue.empty()){

	// unqueue the current vertex (it has been visited)
	Vertex startVertex = vertexQueue.front();
	vertexQueue.pop();

	// loop through vertices
	DirectedEdge currentEdge = dirEdgeInput[startVertex.fdeID];
	int currentID = -1;

	while(currentID != startVertex.fdeID){

	  DirectedEdge prevEdge = dirEdgeInput[currentEdge.prev()];
	  currentEdge = dirEdgeInput[prevEdge.twinID];
	  currentID = currentEdge.id;

	  // check face
	  Face currentFace = faceInput[prevEdge.face()];

	  if(!currentFace.isVisited){
	    currentFace.isVisited = true;
	    faceInput[prevEdge.face()] = currentFace;
	    faceCount++;
	  }

	  // add the neighbour vertices
	  Vertex neighbourVertex = vertexInput[currentEdge.vertexID];
	  if(!neighbourVertex.isVisited){
	    neighbourVertex.isVisited = true;

	    vertexQueue.push(neighbourVertex);
	    vertexInput[currentEdge.vertexID] = neighbourVertex;
	    vertexCount++;

	    // std::cout << "Visited vertex: " << neighbourVertex.id << std::endl;
	  }
	}
      }

      vertexCounts.push_back(vertexCount);
      faceCounts.push_back(faceCount);
    }
  }

  int genus = 0;

  // for each mesh, calculate the genus using Euler's formula (slide 22, meshes_euler_formula.pdf)
  for(int i = 0; i < vertexCounts.size(); i++){
    // 3f = 2e, so we can find the # of edges from the # of faces
    genus += 1 - 0.5f * vertexCounts[i] + 0.25f * faceCounts[i];

    /*
    std::cout << "------------------------------" << std::endl;
    std::cout << "mesh: " << i << std::endl;
    std::cout << "vertex count: " << vertexCounts[i] << std::endl;
    std::cout << "face count: " << faceCounts[i] << std::endl;
    std::cout << "genus: " << genus << std::endl;
    */
  }

  return genus;
}

TestOutput manifoldTest(std::filesystem::path filePath) {

  std::vector<Vertex> vertexInput;
  std::vector<Face> faceInput;
  std::vector<DirectedEdge> dirEdgeInput;
  std::vector<int> fdeInput;
  std::vector<int> halfInput;

  std::ifstream inputFile(filePath, std::ios::in);
  std::string inputType;

  TestOutput results;
  results.meshName = (std::string)filePath.stem();

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
        // std::cout << "Vertex " << id << " " << i1 << " " << i2 << " " << i3
        // << std::endl;
      }
	  else if (inputType.compare("FirstDirectedEdge") == 0) {
        fdeInput.push_back(i1);
        // std::cout << "FirstDirectedEdge " << id << " " << i1 << std::endl;
      }
	  else if (inputType.compare("Face") == 0) {
        faceInput.push_back(Face(id, (std::vector<int>){i1, i2, i3}));
        // std::cout << "Face " << id << " " << i1 << " " << i2 << " " << i3 <<
        // std::endl;
      }
	  else if (inputType.compare("OtherHalf") == 0) {
        halfInput.push_back(i1);
        // std::cout << "OtherHalf " << id << " " << i1 << std::endl;
      }
	  else {
        std::cout << "Error: invalid line format on line" << currentLine
                  << std::endl;
        results.readSuccessful = false;
        return results;
      }
      currentLine++;
    }

    inputFile.close();
  }
  else {
    std::cout << "Error: failed to read file <"
              << (std::string)filePath.filename() << ">" << std::endl;
    results.readSuccessful = false;
    return results;
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
  for (int i = 0; i < faceInput.size(); i++) {
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
    results.readSuccessful = false;
    return results;
  }

  if (fdeInput.size() != vertexInput.size()) {
    std::cout << "Error: insufficient number of vertices or FDEs specified"
              << std::endl;
    results.readSuccessful = false;
    return results;
  }

  // PHASE 3: Perform each manifold test and return the result
  int e = 0;
  for (auto &de : dirEdgeInput) {
    de.twinID = halfInput[e];

    /*
    if(halfInput[e] == -1){
      std::cout << "Error: boundary found at edge " << e << std::endl;
      return results;
    }
    */

    e++;
  }

  e = 0;
  for (auto de : dirEdgeInput) {
    // std::cout << "de: " << de.id << " | twin: " <<
    // dirEdgeInput[de.twinID].twinID << std::endl;

    // EDGE TEST: twin is -1, implying that a half edge lies at the boundary
    if (de.twinID == -1) {
      // std::cout << "Error: boundary found at edge " << e << std::endl;
      results.edgeID = e;
      return results;
    }
	else if (de.id != dirEdgeInput[de.twinID].twinID) {
      std::cout << "Error: half edges point to different twins!" << std::endl;
      std::cout << "de: " << de.id << " | twin: " << de.twinID << std::endl;
      std::cout << "de: " << de.twinID
                << " | twin: " << dirEdgeInput[de.twinID].twinID << std::endl;
      results.twinID = e;
      return results;
    }

    e++;
  }

  // assign the FDEs based on file input
  e = 0;
  for (auto &v : vertexInput) {
    v.fdeID = fdeInput[e];
    e++;
  }

  results.pinchID = pinchTest(vertexInput, dirEdgeInput);
  results.genus = genusTest(dirEdgeInput, vertexInput, faceInput);

  // if a pinch point has been found, then the result is not manifold
  if (results.pinchID != -1)
    return results;

  results.manifold = true;
  return results;
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "Usage: ./manifoldTest <directory_path>" << std::endl;
    return 0;
  }

  std::vector<TestOutput> testResults;

  // PHASE 1: Read the file and store the input
  for (auto testFile : std::filesystem::directory_iterator(argv[1])) {

    std::string fileName = (std::string)testFile.path().filename();

    if (testFile.path().extension().compare(".diredge") != 0) {
      std::cout << "Error: .diredge file type required for manifold test"
                << std::endl;
      std::cout << "File: <" << fileName << "> does not fit this criteria"
                << std::endl;
      return 1;
    }

    // std::cout << testFile.path() << std::endl;

    // execute test on each of them and store the result
    TestOutput result = manifoldTest(testFile.path());
    if (!result.readSuccessful) {
      std::cout << "Error: read failed on file: <" << fileName << ">"
                << std::endl;
      return 1;
    }

    testResults.push_back(result);
  }

  // PHASE 2: take the stored data as file output
  std::string outputFileName = "manifold_results.txt";
  std::ofstream outputFile(outputFileName, std::ios::out);

  if (outputFile.is_open()) {

    for (auto t : testResults) {
      outputFile << "--------------------------" << std::endl;
      outputFile << "File: " << t.meshName << std::endl;

      if (t.manifold) {
        outputFile << "Manifold: YES" << std::endl;
	outputFile << "Genus: " << t.genus << std::endl;
      }
      else {
        outputFile << "Manifold: NO" << std::endl;
        if (t.pinchID != -1)
          outputFile << "<PINCH TEST FAILED> on Vertex: " << t.pinchID
                     << std::endl;
        if (t.edgeID != -1)
          outputFile << "<BOUNDARY TEST FAILED> on Edge: " << t.edgeID
                     << std::endl;
        if (t.twinID != -1)
          outputFile << "<TWIN TEST FAILED> on Edge: " << t.twinID << std::endl;
      }

    }

    outputFile << "--------------------------" << std::endl;
  }

  return 0;
}
