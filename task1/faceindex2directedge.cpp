#include <iostream>

int main(int argc, char* argv[]){
  // no arguments provided
  if(argc == 1){
    std::cout << "Usage: ./face2faceindex <directory>" << std::endl;
    return 0;
  }

  // Phase 1: Reading the file and storing the data, we'll want these as their own structs / classes most likely

  // Required data:
  // - Vertex
  // - FirstDirectedEdge
  // - Face
  // - OtherHalf

  // Phase 2: Writing to the file using the custom data parameters

  return 0;
}
