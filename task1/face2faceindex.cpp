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
    std::cout << "Usage: ./face2faceindex <filepath>" << std::endl;
    return 0;
  }

  // PHASE 1: Reading the file and storing the data, we'll want these as their
  // own structs / classes most likely Error checks: not a .tri file

  // Required data:
  // - Vertex
  // - Face
  int vertices = 0;
  int faces = 0;

  std::filesystem::path filePath(argv[1]);
  std::ifstream inputFile(filePath, std::ios::in);

  std::vector<Vertex> vertexOutput;
  std::vector<Face> faceOutput;

  if (inputFile.is_open()) {
    // taken the first line to be the number of faces, we can just read the
    // first input on the stream
    if (!(inputFile >> faces)) {
      std::cout << "invalid start line!" << std::endl;
    }

    // we can expect for each vertex to be given as 3 points
    int currentLine = 1;
    float v1, v2, v3;

    while (!inputFile.eof()) {
      inputFile >> v1 >> v2 >> v3;

      // error checking for bad stuff in the ifstream

      /*
      if(inputFile.fail()){
        std::cout << v1 << " " << v2 << " " << v3 << std::endl;
        std::cerr << "Error: invalid vertex found on raw vertex: " <<
      currentLine << std::endl; return 1;
      }
      */

      // for error checking
      currentLine++;

      Cartesian3 currentVertex(v1, v2, v3);

      bool vertexUnique = true;
      for (auto v : vertexOutput) {
        if (v.point == currentVertex)
          vertexUnique = false;
      }

      if (vertexUnique) {
        Vertex vertexBuffer;

        vertexBuffer.point = currentVertex;
        vertexBuffer.id = vertices;
        vertices++;

        vertexOutput.push_back(vertexBuffer);
      }
    }

    // reset to beginning of the ifstream
    inputFile.clear();
    inputFile.seekg(0);

    int currentFace = 1;

    // skip first line
    inputFile >> faces;
    faces = 0;

    Face faceBuffer;

    while (!inputFile.eof()) {
      inputFile >> v1 >> v2 >> v3;
      Cartesian3 currentVertex(v1, v2, v3);

      for (auto v : vertexOutput) {
        if (v.point == currentVertex) {
          faceBuffer.vertexIDs.push_back(v.id);
        }
      }

      if (currentFace % 3 == 0) {
        faceBuffer.id = faces;
        faceOutput.push_back(faceBuffer);

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

  // PHASE 2: Writing to the file using the custom data parameters
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
    for (auto v : vertexOutput) {
      outputFile << "Vertex " << v.id << "\t" << v.point << std::endl;
    }

    // for loop for faces
    for (auto f : faceOutput) {
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
