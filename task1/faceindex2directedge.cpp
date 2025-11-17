#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>

#include "../triangle_renderer/Vertex.h"
#include "../triangle_renderer/Face.h"
#include "../triangle_renderer/DirectedEdge.h"

int main(int argc, char* argv[]){
  if(argc != 2){
    std::cout << "Usage: ./faceindex2directedge <filepath>" << std::endl;
    return 0;
  }

  // PHASE 1: Read the file and store the input

  std::vector<Vertex> vertexInput;
  std::vector<Face> faceInput;
  std::vector<DirectedEdge> dirEdgeInput;

  std::filesystem::path filePath(argv[1]);
  std::ifstream inputFile(filePath, std::ios::in);
  std::string inputType;

  // marking as i as we are checking for input of vertices and faces
  int i1, i2, i3;
  int id;
  int currentLine = 0;

  std::string strLine;

  if(inputFile.is_open()){
    while(std::getline(inputFile, strLine)){
      if(strLine[0] == '#') continue;

      // cast as a sstream so that we can parse the input
      std::stringstream ss(strLine);
      ss >> inputType >> id >> i1 >> i2 >> i3;

      if(inputType.compare("Vertex") == 0){
	vertexInput.push_back(Vertex(id, (float)i1, (float)i2, (float)i3));
      }
      else if(inputType.compare("Face") == 0){
	faceInput.push_back(Face(id, (std::vector<int>){i1, i2, i3}));
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
  
  // calculate the directed edges now from the stored face vertices
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

  std::cout << "------------------------" << std::endl;

  for(auto d : dirEdgeInput){
    std::cout << "DirEdge " << d.id << " | from: " << dirEdgeInput[d.prev()].vertexID << " | to: " << d.vertexID << std::endl;
  }

  std::cout << "------------------------" << std::endl;

  std::vector<Vertex> fdeInput;

  // first directed edge for each vertex
  for(auto &v : vertexInput){
    for(auto d : dirEdgeInput){
      if(dirEdgeInput[d.prev()].vertexID == v.id){
	v.fdeID = d.id;
	std::cout << "FirstDirectedEdge " << v.id << " " << v.fdeID << std::endl;
	fdeInput.push_back(v);
	break;
      }
    }
  }

  std::cout << "------------------------" << std::endl;

  // find the opposing / twin vertex
  for(auto &d1 : dirEdgeInput){
    for(auto d2 : dirEdgeInput){
      if(dirEdgeInput[d1.prev()].vertexID == d2.vertexID
	 &&
	 dirEdgeInput[d2.prev()].vertexID == d1.vertexID
	 ){
	d1.twinID = d2.id;
	std::cout << "OtherHalf " << d1.id << " " << d1.twinID << std::endl;
	break;
      }
    }
  }

  std::cout << "------------------------" << std::endl;

  // PHASE 2: take the stored data as file output
  std::string objectName = (std::string)filePath.stem();
  std::string outputFileName = objectName + ".diredge";
  std::ofstream outputFile(outputFileName, std::ios::out);

  if(outputFile.is_open()){
    outputFile << "# University of Leeds 2022-2023" << std::endl;
    outputFile << "# COMP 5812 Assignment 1" << std::endl;
    outputFile << "# Oliver Cheung " << std::endl;
    outputFile << "# 201597566" << std::endl;
    outputFile << "#" << std::endl;
    outputFile << "# Object Name: " << objectName << std::endl;
    outputFile << "# Vertices=" << vertexInput.size() << " Faces=" << faceInput.size() << std::endl; 
    outputFile << "#" << std::endl;

    for(auto v : vertexInput){
      outputFile << "Vertex " << v.id << "\t" << v.point << std::endl;
    }

    for(auto fde : fdeInput){
      outputFile << "FirstDirectedEdge " << fde.id << "\t" << fde.fdeID << std::endl;
    }

    for(auto f : faceInput){
      outputFile << "Face " << f.id << "\t"; 
      for(int i = 0; i < 3; i++){
	outputFile << f.vertexIDs[i] << " ";
      }
      outputFile << std::endl;
    }

    for(auto de : dirEdgeInput){
      outputFile << "OtherHalf " << de.id << "\t" << de.twinID << std::endl;
    }

    outputFile.close();

    std::cout << "File <" << outputFileName << "> written to successfully!" << std::endl;
  }
  else{
    std::cout << "Error: failed to write to a file: " << outputFileName << std::endl;
    return 1;
  }
    
  return 0;
}
