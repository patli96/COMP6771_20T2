# TESTING RATIONALE

## Testing files
The structure of testing files is as below:
/ test
    |_ test_main.cpp
    |_ CMakeLists.txt
    |_ README.md
    |_ graph
        |_ accessors_test.cpp
        |_ comparisons_test.cpp
        |_ constructors_test.cpp
        |_ extractor_test.cpp
        |_ iterator_test.cpp
        |_ modifiers_test.cpp
        |_ graph_test1.cpp
        |_ CMakeLists.txt
Each test.cpp file under test/graph/ directory is named as the task secitons listed in the specs.

## Test Case
Each test.cpp file contains one test case for the functionality and several sections. By default, my test cases will test the order of nodes and edges in the graph and handling duplicated node/edge.

### accessors_test.cpp
This file is for testing the iterator [gdwg.accessors].
Each section inside is corresponding to each functions listed in accessors part in the specs.
I tested those functions with legal and illegal node/edge.

### comparisons_test.cpp
This file is for testing comparisons [gdwg.cmp].
I created two graphs with the same nodes and edges to test the comparison operator.

### constructors_test.cpp
This file is for testing constructors [gdwg.ctor].
Each section inside is corresponding to each constructor listed in the specs.

### extractor_test.cpp
This file is for testing extractor [gdwg.io].
I used the example given in the specs. The format of my code's result. like spaces, might be differ from the given expected output, but the nodes and edges are exactly the same as expected.

### iterator_test.cpp
This file is for testing the iterator [gdwg.iterator].
Each section inside is corresponding to each functions in iterator class, including constructor, source, traversal and comparison.

To test constructor, I created an empty iterator with the graph::iterator type. If no exception raised the the test case is passed. For source section, I just compared the result with an expected comman_tuple. I implemented a for loop to test traversal section. And a begin() and end() of an empty graph are compared to test comparison section.

### modifiers_test.cpp
This file is for testing the iterator [gdwg.modifiers].
Each section inside is corresponding to each functions listed in modifiers part in the specs.

To test insert_node and insert_edge, the idea is to check successfully inserting and unsuccessfully one. Unsuccessfully inserting I tried are inserting node or edge already exist, inserting edge with non-existing node which would throw an exception. Also, I tested inserted edges from the same src and dst with different weights as well.

The idea for testing replace_node is the same as above.

In the merge_replace_node section, I created a graph given in the example in specs to test the result is as expected or not.
I added an edge (C, A, 2) in the original graph on the left hand side in another test, since this new edge would be duplicated after merge_replace_node(B, A) with the original edge (C, B, 2).

For erase_node and erase_edge, the idea is also testing erase legal node/edge, illegal node/edge and case with exception.
