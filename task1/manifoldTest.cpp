#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>

#include "../triangle_renderer/Vertex.h"
#include "../triangle_renderer/Face.h"
#include "../triangle_renderer/DirectedEdge.h"

bool manifoldTest(){
  // this will take the filePath

  // PHASE 1: Parse the file
  // PHASE 2: Sort the input
  // PHASE 3: Perform each manifold test and return the result

  // 1) edge pairing
  // 2) pinch point

  return true;
}

bool edgePairTest(){
  // check if each pair has a twin
  // if the twinID is -1, that implies that the edge does not have a twin
  // so, the test will fail
  return true;
}

bool pinchTest(){
  // count one ring
  // count the vertex degree
  // if they are not equal, the test will fail
  return true;
}

// TASK 3
int genusTest(){
  // set the return type of a failed genus to -1
  return 0;
}


int main(int argc, char* argv[]){

  if(argc != 2){
    std::cout << "Usage: ./manifoldTest <filepath>" << std::endl;
    return 0;
  }

  // PHASE 1: Read the file and store the input

  // NOTE: the FDE is implied by the vertex, no need to store that explicitly
  std::vector<Vertex> vertexInput;
  std::vector<Face> faceInput;
  std::vector<DirectedEdge> dirEdgeInput;

  std::vector<int> fdeInput;
  std::vector<int> halfInput;
  
  std::filesystem::path filePath(argv[1]);
  std::ifstream inputFile(filePath, std::ios::in);
  std::string inputType;

  // the .diredge filetype is required for testing, so we should check it
  if(filePath.extension().compare(".diredge") != 0){
    std::cout << "Error: .diredge file type required for manifold test" << std::endl;
    return 1;
  }

  int i1, i2, i3;
  int id;
  int currentLine = 0;

  std::string strLine;

  if(inputFile.is_open()){
    while(std::getline(inputFile, strLine)){
      if(strLine[0] == '#') continue;

      std::stringstream ss(strLine);
      ss >> inputType >> id >> i1 >> i2 >> i3;

      if(inputType.compare("Vertex") == 0){
	vertexInput.push_back(Vertex(id, (float)i1, (float)i2, (float)i3));
	// std::cout << "Vertex " << id << " " << i1 << " " << i2 << " " << i3 << std::endl;
      }
      else if(inputType.compare("FirstDirectedEdge") == 0){
	fdeInput.push_back(i1);
	// std::cout << "FirstDirectedEdge " << id << " " << i1 << std::endl;
      }
      else if(inputType.compare("Face") == 0){
	faceInput.push_back(Face(id, (std::vector<int>){i1, i2, i3}));
	// std::cout << "Face " << id << " " << i1 << " " << i2 << " " << i3 << std::endl;
      }
      else if(inputType.compare("OtherHalf") == 0){
	halfInput.push_back(i1);
	// std::cout << "OtherHalf " << id << " " << i1 << std::endl;
      }
      else{
	std::cout << "Error: invalid line format on line" << currentLine << std::endl;
      }
      currentLine++;
    }

    inputFile.close();
  }
  else{
    std::cout << "Error: failed to read file <" << (std::string)filePath.filename() << ">" << std::endl;
    return 1;
  }

  // PHASE 2: DATA CONSTRUCTION
  // making sure things are nice and tidy to do testing

  // construct the directed edges
  int j = 0;
  for(int i = 0; i < faceInput.size(); i++){
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

  if(dirEdgeInput.size() != halfInput.size()){
    std::cout << "Error: insufficient number of edge pairings specified" << std::endl;
    return 1;
  }

  if(fdeInput.size() != vertexInput.size()){
    std::cout << "Error: insufficient number of vertices or FDEs specified" << std::endl;
    return 1;
  }

  // assign the half edge pairs based on file input
  int e = 0;
  for(auto &de : dirEdgeInput){
    de.twinID = halfInput[e];
    e++;
  }

  // assign the FDEs based on file input
  e = 0;
  for(auto &v : vertexInput){
    v.fdeID = fdeInput[e];
    e++;
  }

  /*
  for(auto v : vertexInput){
    std::cout << "Vertex: " << v.id << " | FDE: " << v.fdeID << std::endl;
  }
  for(auto de : dirEdgeInput){
    std::cout << "DirEdge: " << de.id << " | Twin: " << de.twinID << std::endl;
  }
  */

  // PHASE 3: MANIFOLD TESTING

  // Edge pair : boundary test

  // One ring : pinch point test


  // PHASE 4: take the stored data as file output

  return 0;
}
