
# Table of Contents

1.  [New Additions](#org553aee7)
    1.  [triangle\_renderer](#org3866250)
    2.  [handout\_models](#org05c733b)
2.  [Usage](#org364013c)
    1.  [face2faceindex](#orgb7dade6)
    2.  [faceindex2directedge](#org40d84e7)
    3.  [manifoldTest](#orgf5ccf3e)
    4.  [meshRepair](#org8acdb8a)
    5.  [meshSimplify](#org93a1401)



<a id="org553aee7"></a>

# New Additions

-   New files have been added to the triangle\_renderer and handout\_models
-   assignment\_files has also been added, containing the required solutions to the coursework
-   new content for triangle\_renderer and handout\_models is specified below


<a id="org3866250"></a>

## triangle\_renderer

-   DirectedEdge class (a "to" half edge)
-   Face class
-   Vertex class
-   The Cartesian3 class has also been changed to include the dot product, this was needed for the mesh simplification
-   should still compile as usual, under:
    
        ./compile.sh


<a id="org05c733b"></a>

## handout\_models

-   cube\_bad.tri: a cube missing one of its faces (to test mesh repair)
-   tetrahedron\_hole.tri: a tetrahedron missing one of its faces (to also test repair)
-   pinchtetra.tri: two tetrahedra joined by a single vertex (to test pinch point)


<a id="org364013c"></a>

# Usage


<a id="orgb7dade6"></a>

## face2faceindex

    ./face2faceindex <file path>


<a id="org40d84e7"></a>

## faceindex2directedge

    ./faceindex2directedge <file path>


<a id="orgf5ccf3e"></a>

## manifoldTest

    ./manifoldTest <directory_path>


<a id="org8acdb8a"></a>

## meshRepair

    ./meshRepair <file path>


<a id="org93a1401"></a>

## meshSimplify

    ./meshSimplify <file path> <face percentage>

-   **file path**: path to file to simplify
-   **face percentage**: the percentage of the original edge count to simplify to
    -   must be in range between 0.1 and 1
    -   e.g. 0.5 will perform mesh simplification until 50% of the original edges remain

