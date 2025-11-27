
# Table of Contents

1.  [New Additions](#org178f08a)
    1.  [triangle\_renderer](#orga70ea8d)
    2.  [handout\_models](#orgf06d03d)
    3.  [assignment\_files](#orgf67a574)
        1.  [dirEdgeFiles](#org413e0af)
        2.  [faceFiles](#org5cd8c93)
2.  [Usage](#org9dd17d7)
    1.  [face2faceindex](#orgae69462)
    2.  [faceindex2directedge](#orgcd2129d)
    3.  [manifoldTest](#org5c67964)
    4.  [meshRepair](#org998bd83)
    5.  [meshSimplify](#org66b9ead)



<a id="org178f08a"></a>

# New Additions

-   new files have been added to the triangle\_renderer and handout\_models
-   assignment\_files has also been added, containing the required solutions to the coursework
-   new content for triangle\_renderer and handout\_models is specified below


<a id="orga70ea8d"></a>

## triangle\_renderer

> The triangle renderer has been updated to work with ".diredge" files as to test mesh repair and mesh simplification

-   DirectedEdge class (a "to" half edge)
-   Face class
-   Vertex class
-   The Cartesian3 class has also been changed to include the dot product, this was needed for the mesh simplification
-   should still compile as usual, under:
    
        ./compile.sh


<a id="orgf06d03d"></a>

## handout\_models

-   cube\_bad.tri: a cube missing one of its faces (to test mesh repair)
-   tetrahedron\_hole.tri: a tetrahedron missing one of its faces (to also test repair)
-   pinchtetra.tri: two tetrahedra joined by a single vertex (to test pinch point)


<a id="orgf67a574"></a>

## assignment\_files

-   To compile the files:

    make


<a id="org413e0af"></a>

### dirEdgeFiles

-   contains all the provided meshes, exported to the required ".diredge" format


<a id="org5cd8c93"></a>

### faceFiles

-   contains all the provided meshes, exported to the required ".face" format


<a id="org9dd17d7"></a>

# Usage


<a id="orgae69462"></a>

## face2faceindex

    ./face2faceindex <file path>

-   **file path:** path to file to convert to ".face"


<a id="orgcd2129d"></a>

## faceindex2directedge

    ./faceindex2directedge <file path>

-   **file path:** path to file to convert to ".diredge"


<a id="org5c67964"></a>

## manifoldTest

    ./manifoldTest <directory_path>

-   **directory path:** path to the target directory to test each file as manifold
-   each file within this directory should be of ".diredge" format

    ./manifoldTest dirEdgeFiles/

-   should test all ".diredge" files required in the coursework

> The output of the file "manifold\_results.txt" will be in the same directory the script is ran


<a id="org998bd83"></a>

## meshRepair

    ./meshRepair <file path>

-   **file path:** path to file to repair
-   expects input to be the ".diredge" format

> The output of the file will be appended with "\_fixed", in the same directory the script is ran


<a id="org66b9ead"></a>

## meshSimplify

    ./meshSimplify <file path> <face percentage>

-   **file path:**: path to file to simplify (must be of file type ".diredge")
-   **face percentage:** the percentage of the original mesh edge count to simplify to
    -   must be in range between 0.1 and 1
    -   e.g. 0.5 will perform mesh simplification until 50% of the original edges remain

> The output of the file will be appended with "\_simplified", in the same directory the script is ran. The file output has also been left in to show the process in which the mesh is simplified.

