#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]){
  // no arguments provided
  if(argc != 2){
    std::cout << "Usage: ./face2faceindex <directory>" << std::endl;
    return 0;
  }

  // PHASE 1: Reading the file and storing the data, we'll want these as their own structs / classes most likely
  // Error checks: not a .tri file 
  
  // Required data:
  // - Vertex
  // - Face
  int vertices = 0;
  int faces = 0;

  std::filesystem::path filePath(argv[1]);
  std::ifstream inputFile(filePath, std::ios::in);
  if(inputFile.is_open()){
    // read file contents
    std::string strLine;
    while(std::getline(inputFile, strLine)){
      // if an empty line is found, continue
      if(strLine.empty()) continue;

      std::cout << strLine << std::endl;
    }

    // close file stream afterwards
    inputFile.close();
  }
  else{
    std::cout << "Error: failed to read file <" << (std::string)filePath.filename() << ">" << std::endl;
    return 1;
  }

  // PHASE 2: Writing to the file using the custom data parameters
  std::string objectName = (std::string)filePath.stem();
  std::string outputFileName = objectName + ".face";
  std::ofstream outputFile(outputFileName, std::ios::out);

  if(outputFile.is_open()){
    // file header
    outputFile << "# University of Leeds 2022-2023" << std::endl;
    outputFile << "# COMP 5812 Assignment 1" << std::endl;
    outputFile << "# Oliver Cheung " << std::endl;
    outputFile << "# 201597566" << std::endl;
    outputFile << "#" << std::endl;
    outputFile << "# Object Name: " << objectName << std::endl;
    outputFile << "# Vertices=" << vertices << " Faces=" << faces << std::endl; 
    outputFile << "#" << std::endl;

    // file contents

    // close file when we're done writing
    outputFile.close();

    std::cout << "File <" << outputFileName << "> written to successfully!" << std::endl;
  }
  else{
    std::cout << "Error: failed to write to a file: " << outputFileName << std::endl;
    return 1;
  }

  return 0;
}
