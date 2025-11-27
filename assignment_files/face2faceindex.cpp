#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// libraries for data structure
#include "../triangle_renderer/Face.h"
#include "../triangle_renderer/Vertex.h"

int main(int argc, char *argv[]) {

  // no arguments provided
  if (argc != 2) {
    std::cout << "Usage: ./face2faceindex <file_path>" << std::endl;
    return 0;
  }

  // PHASE 1: Read the file and store the input

  // keep track of vertex and face count to write to file output
  int vertices = 0;
  int faces = 0;

  std::filesystem::path filePath(argv[1]);
  std::ifstream inputFile(filePath, std::ios::in);

  if(inputFile.fail()){
    std::cout << "Error: invalid file" << std::endl;
  }
  // check for .tri extension, as that should be the only file type we need to convert
  if(filepath.extension().compare(".tri") != 0){
    std::cout << "Error: .tri file type required for conversion" << std::endl;
    return 1;
  }

  std::vector<Vertex> vertexInput;
  std::vector<Face> faceInput;

  if (inputFile.is_open()) {
    // take the first line to be the number of faces, we can just read the
    // first input on the stream
    if (!(inputFile >> faces)) {
      std::cout << "invalid start line!" << std::endl;
    }

    // we can expect for each vertex to be given as 3 points
    int currentLine = 1;
    float v1, v2, v3;

    while (!inputFile.eof()) {
      inputFile >> v1 >> v2 >> v3;

      // for error checking
      currentLine++;

      // map the input onto a Cartesian3 to represent the vertices' point
      Cartesian3 currentVertex(v1, v2, v3);

      // as faces share a vertex, we only want to keep track of the unique vertices in the mesh
      bool vertexUnique = true;
      for (auto v : vertexInput) {
        if (v.point == currentVertex)
          vertexUnique = false;
      }
      if (vertexUnique) {
        Vertex vertexBuffer;

        vertexBuffer.point = currentVertex;
        vertexBuffer.id = vertices;
        vertices++;

        vertexInput.push_back(vertexBuffer);
      }
    }

    // reset to beginning of the ifstream
    // now we have an id for each vertex, we can assign them to faces
    inputFile.clear();
    inputFile.seekg(0);

    int currentFace = 1;

    // skip first line (we don't need the face count this time)
    inputFile >> faces;
    faces = 0;

    Face faceBuffer;

    // second pass to generate the required faces
    while (!inputFile.eof()) {
      inputFile >> v1 >> v2 >> v3;

      Cartesian3 currentVertex(v1, v2, v3);

      for (auto v : vertexInput) {
        if (v.point == currentVertex) {
          faceBuffer.vertexIDs.push_back(v.id);
        }
      }

      // as we can assume the mesh has been triangulated, we can now move onto the next face
      if (currentFace % 3 == 0) {
        faceBuffer.id = faces;
        faceInput.push_back(faceBuffer);

        faces++;
        faceBuffer.vertexIDs.clear();
      }

      currentFace++;
    }

    // close file stream afterwards
    inputFile.close();
  } else {
    std::cout << "Error: failed to read file <"
              << (std::string)filePath.filename() << ">" << std::endl;
    return 1;
  }

  // PHASE 2: take the stored data as file output

  std::string objectName = (std::string)filePath.stem();
  std::string outputFileName = objectName + ".face";
  std::ofstream outputFile(outputFileName, std::ios::out);

  if (outputFile.is_open()) {
    // output file header
    outputFile << "# University of Leeds 2022-2023" << std::endl;
    outputFile << "# COMP 5812 Assignment 1" << std::endl;
    outputFile << "# Oliver Cheung " << std::endl;
    outputFile << "# 201597566" << std::endl;
    outputFile << "#" << std::endl;
    outputFile << "# Object Name: " << objectName << std::endl;
    outputFile << "# Vertices=" << vertices << " Faces=" << faces << std::endl;
    outputFile << "#" << std::endl;

    // for loop for vertices
    for (auto v : vertexInput) {
      outputFile << "Vertex " << v.id << "\t" << v.point << std::endl;
    }

    // for loop for faces
    for (auto f : faceInput) {
      outputFile << "Face " << f.id << "\t";
      for (int i = 0; i < 3; i++) {
        outputFile << f.vertexIDs[i] << " ";
      }
      outputFile << std::endl;
    }

    // close file when we're done writing
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
