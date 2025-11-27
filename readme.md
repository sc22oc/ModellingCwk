
# Table of Contents

1.  [New Additions](#org05f66e0)
    1.  [triangle\_renderer](#orgb285849)
    2.  [handout\_models](#org292ee4f)
    3.  [assignment\_files](#orgdbe238e)
        1.  [dirEdgeFiles](#orge52e34c)
        2.  [faceFiles](#org1b5e805)
2.  [Usage](#orgd325770)
    1.  [face2faceindex](#orgd79a5ce)
    2.  [faceindex2directedge](#org9bdb27f)
    3.  [manifoldTest](#org5f7ce0e)
    4.  [meshRepair](#org5c9a82e)
    5.  [meshSimplify](#org69ee2b2)



<a id="org05f66e0"></a>

# New Additions

-   New files have been added to the triangle\_renderer and handout\_models
-   assignment\_files has also been added, containing the required solutions to the coursework
-   new content for triangle\_renderer and handout\_models is specified below


<a id="orgb285849"></a>

## triangle\_renderer

-   DirectedEdge class (a "to" half edge)
-   Face class
-   Vertex class
-   The Cartesian3 class has also been changed to include the dot product, this was needed for the mesh simplification
-   should still compile as usual, under:
    
        ./compile.sh


<a id="org292ee4f"></a>

## handout\_models

-   cube\_bad.tri: a cube missing one of its faces (to test mesh repair)
-   tetrahedron\_hole.tri: a tetrahedron missing one of its faces (to also test repair)
-   pinchtetra.tri: two tetrahedra joined by a single vertex (to test pinch point)


<a id="orgdbe238e"></a>

## assignment\_files

-   To compile the files:

    make


<a id="orge52e34c"></a>

### dirEdgeFiles

-   contains all the provided meshes, exported to the required ".diredge" format


<a id="org1b5e805"></a>

### faceFiles

-   contains all the provided meshes, exported to the required ".face" format


<a id="orgd325770"></a>

# Usage


<a id="orgd79a5ce"></a>

## face2faceindex

    ./face2faceindex <file path>

-   **file path** path to file to convert to ".face"


<a id="org9bdb27f"></a>

## faceindex2directedge

    ./faceindex2directedge <file path>

-   **file path** path to file to convert to ".diredge"


<a id="org5f7ce0e"></a>

## manifoldTest

    ./manifoldTest <directory_path>

-   **directory path** path to the target directory to test each file as manifold
-   each file within this directory should be of ".diredge" format

    ./manifoldTest dirEdgeFiles/

-   should test all ".diredge" files required in the coursework

> The output of the file "manifold\_results.txt" will be in the same directory the script is ran


<a id="org5c9a82e"></a>

## meshRepair

    ./meshRepair <file path>

-   **file path** path to file to repair
-   expects input to be the ".diredge" format

> The output of the file will be prefixed with "\_fixed", in the same directory the script is ran


<a id="org69ee2b2"></a>

## meshSimplify

    ./meshSimplify <file path> <face percentage>

-   **file path**: path to file to simplify (must be of file type ".diredge")
-   **face percentage**: the percentage of the original mesh edge count to simplify to
    -   must be in range between 0.1 and 1
    -   e.g. 0.5 will perform mesh simplification until 50% of the original edges remain

> The output of the file will be prefixed with "\_simplified", in the same directory the script is ran

