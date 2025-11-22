///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	GeometricSurfaceFaceDS.cpp
//	------------------------
//
//	Base code for geometric assignments.
//
//	This is the minimalistic Face-based D/S for storing
//	surfaces, to be used as the basis for fuller versions
//
//	It will include object load / save code & render code
//
///////////////////////////////////////////////////

#include "GeometricSurfaceFaceDS.h"
#include <fstream>
#include <iostream>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>

// added myself for rendering files in the .diredge format
#include <filesystem>
#include "Face.h"
#include "Vertex.h"

#endif

// constructor will initialise to safe values
GeometricSurfaceFaceDS::
    GeometricSurfaceFaceDS() { // GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()
  // force the size to nil (should not be necessary, but . . .)
  vertices.resize(0);

  // set this to something reasonable
  boundingSphereSize = 1.0;

  // set the midpoint to the origin
  midPoint = Cartesian3(0.0, 0.0, 0.0);
} // GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()

// read routine returns true on success, failure otherwise
bool GeometricSurfaceFaceDS::ReadFileTriangleSoup(
    char *fileName) { // GeometricSurfaceFaceDS::ReadFileTriangleSoup()

  // these are for accumulating a bounding box for the object
  Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
  Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

  // open the input file
  // std::ifstream inFile(fileName);
  std::filesystem::path filePath(fileName);
  std::ifstream inFile(filePath, std::ios::in);

  if (inFile.bad())
    return false;

  // set the number of vertices and faces
  long nTriangles = 0, nVertices = 0;

  // set the midpoint to the origin
  midPoint = Cartesian3(0.0, 0.0, 0.0);

  // check file extension (to do tri and diredge)
  std::string fileType = filePath.extension();
  // std::cout << "file type: " << fileType << std::endl;

  // extension for diredge rendering, otherwise we can just do the .tri like
  // usual
  if (fileType.compare(".diredge") == 0) {
	std::string strLine;
	std::vector<Vertex> vertexInput;
	std::vector<Face> faceInput;

	std::string inputType;
	float i1, i2, i3;
	int id;

	// read in the file and store its data
	while(std::getline(inFile, strLine)){
	  // skip comment boilerplate and empty lines
      if (strLine[0] == '#')
        continue;

	  std::stringstream ss(strLine);
      ss >> inputType >> id >> i1 >> i2 >> i3;

	  // we only need to read the vertices and faces here
	  if (inputType.compare("Vertex") == 0) {

		// std::cout << "V: " << id << " | " << i1 << " " << i2 << " " << i3 << std::endl;
        vertexInput.push_back(Vertex(id, i1, i2, i3));

		// we can also set the min and max coords here for the bounding box
		if(i1 < minCoords.x) minCoords.x = i1;
		if(i2 < minCoords.y) minCoords.y = i2;
		if(i3 < minCoords.z) minCoords.z = i3;

		if(i1 > maxCoords.x) maxCoords.x = i1;
		if(i2 > maxCoords.y) maxCoords.y = i2;
		if(i3 > maxCoords.z) maxCoords.z = i3;

	  } // for each vertex
      else if (inputType.compare("Face") == 0) {
        faceInput.push_back(Face(id, (std::vector<int>){(int)i1, (int)i2, (int)i3}));
	  }
	}

	// store the vertices of the mesh with the faces
	for(auto f : faceInput){
	  for(int i : f.vertexIDs){
		Cartesian3 currentVertex = vertexInput[i].point;

		vertices.push_back(currentVertex);
		midPoint = midPoint + currentVertex;
	  }
	}

	nVertices = faceInput.size() * 3;
	midPoint = midPoint / vertices.size();
  }
  // otherwise, treat it like a .tri file
  else {
    // read in the number of vertices
    inFile >> nTriangles;
    nVertices = nTriangles * 3;

    // now allocate space for them all
    vertices.resize(nVertices);

    // now loop to read the vertices in, and hope nothing goes wrong
    for (int vertex = 0; vertex < nVertices; vertex++) { // for each vertex
      inFile >> vertices[vertex].x >> vertices[vertex].y >> vertices[vertex].z;

      // keep running track of midpoint, &c.
      midPoint = midPoint + vertices[vertex];
      if (vertices[vertex].x < minCoords.x)
        minCoords.x = vertices[vertex].x;
      if (vertices[vertex].y < minCoords.y)
        minCoords.y = vertices[vertex].y;
      if (vertices[vertex].z < minCoords.z)
        minCoords.z = vertices[vertex].z;

      if (vertices[vertex].x > maxCoords.x)
        maxCoords.x = vertices[vertex].x;
      if (vertices[vertex].y > maxCoords.y)
        maxCoords.y = vertices[vertex].y;
      if (vertices[vertex].z > maxCoords.z)
        maxCoords.z = vertices[vertex].z;

    } // for each vertex

	// now sort out the size of a bounding sphere for viewing
	// and also set the midpoint's location
	midPoint = midPoint / vertices.size();
  }

  // now go back through the vertices, subtracting the mid point
  for (int vertex = 0; vertex < nVertices; vertex++) { // per vertex
    vertices[vertex] = vertices[vertex] - midPoint;

	// std::cout << vertices[vertex] << std::endl;
  } // per vertex

  // the bounding sphere radius is just half the distance between these
  boundingSphereSize = sqrt((maxCoords - minCoords).length()) * 1.0;

  return true;
} // GeometricSurfaceFaceDS::ReadFileTriangleSoup()

// routine to render
void GeometricSurfaceFaceDS::Render() { // GeometricSurfaceFaceDS::Render()
  // walk through the faces rendering each one
  glBegin(GL_TRIANGLES);

  // we will loop in 3's, assuming CCW order
  for (unsigned int vertex = 0; vertex < vertices.size();) { // per triangle
    // use increment to step through them
    Cartesian3 *v0 = &(vertices[vertex++]);
    Cartesian3 *v1 = &(vertices[vertex++]);
    Cartesian3 *v2 = &(vertices[vertex++]);
    // now compute the normal vector
    Cartesian3 uVec = *v1 - *v0;
    Cartesian3 vVec = *v2 - *v0;
    Cartesian3 normal = uVec.cross(vVec).normalise();

    glNormal3fv(&normal.x);
    glVertex3fv(&v0->x);
    glVertex3fv(&v1->x);
    glVertex3fv(&v2->x);
  } // per triangle
  glEnd();
} // GeometricSurfaceFaceDS::Render()
