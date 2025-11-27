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

// goal of simplification
// - keep a hold of how many faces we have to start
// - allow the user to determine what percentage of faces will remain
// - repeat simplification until done

std::vector<Vertex> oneRing(std::vector<DirectedEdge> dirEdgeInput, std::vector<Vertex> vertexInput, int startID){
  std::vector<Vertex> res;
  res.clear();

  // std::cout << "starting on vertex: " << startID << std::endl;

  DirectedEdge currentEdge = dirEdgeInput[startID];
  int currentID = -1;

  std::cout << "--------------------------------" << std::endl;
  while(currentID != startID){

    DirectedEdge prevEdge = dirEdgeInput[currentEdge.prev()];

    // assuming that we have a manifold mesh, won't need to check for boundaries

    /*
    std::cout << "current id: " << currentID << std::endl;
    std::cout << "points to: " << currentEdge.vertexID << std::endl;
    std::cout << "twin: " << currentEdge.twinID << std::endl;
    */

    currentEdge = dirEdgeInput[prevEdge.twinID];
    currentID = currentEdge.id;
    
    // push back the vertex to get neighbouring vertices
    res.push_back(vertexInput[currentEdge.vertexID]);
  }

  return res;
}

int main(int argc, char* argv[]){
  if (argc != 2) {
    std::cout << "Usage: ./meshSimplify <filepath> <face_percentage>" << std::endl;
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


  // this should be set by a user argument
  int numFaceReq = faceInput.size() / 2;

  // BEGIN COLLAPSE 
  while((int)faceInput.size() > numFaceReq){
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
      v1 = v1.normalise();
      v2 = v2.normalise();

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

  int collapseID = -1;

  std::cout << "size of gauss: " << ringFromGauss.size() << std::endl;

  // check for a vertex that is safe to delete
  // if the one rings of both intersect at two points, we can delete the vertex!
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
      collapseID = v1.id;
      break;
    }
  }

  std::cout << "Vertex to collapse to: " << collapseID << std::endl;
  std::cout << "------------------------" << std::endl;

  // STEP 1: replace every face that contains the vertex that we will delete with the new vertex
  std::cout << "faces before: " << std::endl;
  for(auto &f : faceInput){
    std::cout << " [ ";
    for(auto &vid: f.vertexIDs){
      std::cout << vid << " ";
    }
    std::cout << "] " << std::endl;
  }

  // vertex to replace is minCurveID
  // vertex to replace WITH is the collapseID
  for(auto &f : faceInput){
    for(auto &vid: f.vertexIDs){
      if(vid == minCurveID){
	vid = collapseID;
      }
    }
  }

  std::cout << "faces after: " << std::endl;
  for(auto &f : faceInput){
    std::cout << " [ ";
    for(auto &vid: f.vertexIDs){
      std::cout << vid << " ";
    }
    std::cout << "] " << std::endl;
  }
  
  // STEP 2: delete the faces that lies on the collapsed edge (if it exists)
  
  // as the face order will be switched with swap to end, have a seperate iterator to keep track of the current face

  std::cout << "face size: " << faceInput.size() << std::endl;
  int currentFace = 0;
  std::vector<int> dupeFaceIDs;

  for(auto &f : faceInput){
    for(auto vid1 : f.vertexIDs){

      int dupe = 0;

      for(auto vid2 : f.vertexIDs){
	// std::cout << "checking: " << vid1 << " against " << vid2 << std::endl;
	if(vid1 == vid2) dupe++;
      }

      if(dupe > 1){
	std::cout << "dupe face at: " << currentFace << std::endl;

	// delete the face when the dupe is found
	/*
	std::swap(faceInput[currentFace], faceInput.back());
	faceInput.pop_back();
	*/

	dupeFaceIDs.push_back(currentFace);

	break;
      }
    }
    currentFace++;
  }

  // for a good mesh, the collapsed edge will be paired with two faces
  // start with the largest ID to remove
  if(dupeFaceIDs[0] < dupeFaceIDs[1])
    std::swap(dupeFaceIDs[0], dupeFaceIDs[1]);

  for(int id : dupeFaceIDs){
    std::cout << "delete: " << id << std::endl;
    std::swap(faceInput[id], faceInput.back());
    faceInput.pop_back();
  }

  std::cout << "faces after (deletion): " << std::endl;
  for(auto &f : faceInput){
    std::cout << " [ ";
    for(auto &vid: f.vertexIDs){
      std::cout << vid << " ";
    }
    std::cout << "] " << std::endl;
  }

  // STEP 3: delete the vertex

  std::cout << "------------------------" << std::endl;

  std::cout << "vertex before: [ ";
  for(auto &v : vertexInput){
    std::cout << v.id << " ";
  }
  std::cout << "] " << std::endl;

  // perform a swap to end and delete the vertex
  std::swap( vertexInput[minCurveID], vertexInput.back() );
  vertexInput.pop_back();

  std::cout << "vertex after: [ ";
  for(auto &v : vertexInput){
    std::cout << v.id << " ";
  }
  std::cout << "] " << std::endl;

  // STEP 4: re-assign

  int faceID = 0;
  int vertexID = 0;
  for(auto &f : faceInput){
    f.id = faceID;
    faceID++;
  }

  for(auto &v : vertexInput){
    for(auto &f : faceInput){
      for(auto &vid : f.vertexIDs){
	if(vid == v.id){
	  vid = vertexID;
	}
      }
    }
    // set the new id
    v.id = vertexID;
    vertexID++;
  }
  
  std::cout << "vertex after: [ ";
  for(auto &v : vertexInput){
    std::cout << v.id << " ";
  }
  std::cout << "] " << std::endl;

  std::cout << "faces after: " << std::endl;
  for(auto &f : faceInput){
    std::cout << " [ ";
    for(auto &vid: f.vertexIDs){
      std::cout << vid << " ";
    }
    std::cout << "] " << std::endl;
  }

  // as we have new FDEs, refresh
  dirEdgeInput.clear();

  int j = 0;
  for(auto f : faceInput){
    std::vector<int> v = f.vertexIDs;

    // number is respect the current face
    DirectedEdge e0(j, v[0], f.id);
    DirectedEdge e1(j+1, v[1], f.id);
    DirectedEdge e2(j+2, v[2], f.id);

    std::cout << "dirEdge: " << v[2] << " " << v[0] << std::endl;
    std::cout << "dirEdge: " << v[0] << " " << v[1] << std::endl;
    std::cout << "dirEdge: " << v[1] << " " << v[2] << std::endl;

    dirEdgeInput.push_back(e0);
    dirEdgeInput.push_back(e1);
    dirEdgeInput.push_back(e2);

    j += 3;
  }

  std::cout << "dir edges done" << std::endl;

  // find the opposing / twin vertex
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

	std::cout << d1.id << " twin " << d1.twinID << std::endl;
        break;
      }
    }
  }

  std::cout << "twin assigned" << std::endl;

  fdeInput.clear();
  for (auto &v : vertexInput) {
    for (auto d : dirEdgeInput) {
      if (dirEdgeInput[d.prev()].vertexID == v.id) {
        v.fdeID = d.id;
        fdeInput.push_back(d.id);
        break;
      }
    }
  }

  std::cout << "fde done" << std::endl;

  }
  // END COLLAPSE

  // EXTRA: re-arrange
  // once edge collapse has been completed, re-index and re-draw the directed edges
  // as swap to end has been done multiple times, we need it identified 0 to edgeNum


  // write to file output
  std::string objectName = (std::string)filePath.stem();
  std::string outputFileName = objectName + "_simplified.diredge";
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
      outputFile << "Vertex " << v.id << "\t" << v.point.x << " " << v.point.y << " " << v.point.z << std::endl;
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

    std::cout << "File <" << outputFileName << "> simplified successfully!"
              << std::endl;
  } else {
    std::cout << "Error: failed to write to a file: " << outputFileName
              << std::endl;
    return 1;
  }

  return 0;
}
