
# Table of Contents

1.  [New Additions](#org6a48e0b)
    1.  [triangle\_renderer](#org6e409e3)
    2.  [handout\_models](#orga29a67f)
    3.  [assignment\_files](#org3bb8c37)
        1.  [dirEdgeFiles](#org104163c)
        2.  [faceFiles](#org83774f2)
2.  [Usage](#org3b463ba)
    1.  [face2faceindex](#org15e7826)
    2.  [faceindex2directedge](#orgc6870cb)
    3.  [manifoldTest](#org685c0e0)
    4.  [meshRepair](#org6058d64)
    5.  [meshSimplify](#org5337591)



<a id="org6a48e0b"></a>

# New Additions

-   new files have been added to the triangle\_renderer and handout\_models
-   assignment\_files has also been added, containing the required solutions to the coursework
-   new content for triangle\_renderer and handout\_models is specified below


<a id="org6e409e3"></a>

## triangle\_renderer

-   DirectedEdge class (a "to" half edge)
-   Face class
-   Vertex class
-   The Cartesian3 class has also been changed to include the dot product, this was needed for the mesh simplification
-   should still compile as usual, under:
    
        ./compile.sh


<a id="orga29a67f"></a>

## handout\_models

-   cube\_bad.tri: a cube missing one of its faces (to test mesh repair)
-   tetrahedron\_hole.tri: a tetrahedron missing one of its faces (to also test repair)
-   pinchtetra.tri: two tetrahedra joined by a single vertex (to test pinch point)


<a id="org3bb8c37"></a>

## assignment\_files

-   To compile the files:

    make


<a id="org104163c"></a>

### dirEdgeFiles

-   contains all the provided meshes, exported to the required ".diredge" format


<a id="org83774f2"></a>

### faceFiles

-   contains all the provided meshes, exported to the required ".face" format


<a id="org3b463ba"></a>

# Usage


<a id="org15e7826"></a>

## face2faceindex

    ./face2faceindex <file path>

-   **file path:** path to file to convert to ".face"


<a id="orgc6870cb"></a>

## faceindex2directedge

    ./faceindex2directedge <file path>

-   **file path:** path to file to convert to ".diredge"


<a id="org685c0e0"></a>

## manifoldTest

    ./manifoldTest <directory_path>

-   **directory path:** path to the target directory to test each file as manifold
-   each file within this directory should be of ".diredge" format

    ./manifoldTest dirEdgeFiles/

-   should test all ".diredge" files required in the coursework

> The output of the file "manifold\_results.txt" will be in the same directory the script is ran


<a id="org6058d64"></a>

## meshRepair

    ./meshRepair <file path>

-   **file path:** path to file to repair
-   expects input to be the ".diredge" format

> The output of the file will be appended with "\_fixed", in the same directory the script is ran


<a id="org5337591"></a>

## meshSimplify

    ./meshSimplify <file path> <face percentage>

-   **file path:**: path to file to simplify (must be of file type ".diredge")
-   **face percentage:** the percentage of the original mesh edge count to simplify to
    -   must be in range between 0.1 and 1
    -   e.g. 0.5 will perform mesh simplification until 50% of the original edges remain

> The output of the file will be appended with "\_simplified", in the same directory the script is ran

