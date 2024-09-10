# Wikispeedier: Speedier than Wikispeedia
A project by Ethan Greenwald, Nathaniel Dyer, Ayush Garg, and Nitya Sunkad.
UIUC CS 225 | Fall 2022.

## Overview
This project is based on the game Wikispeedia, a web game where players attempt to get from one Wikipedia article to another exclusively through clicking links. Our project is designed to determine the shortest path between any two articles. This utilizes the Wikispeedia navigation paths dataset provided by SNAP. We have implemented three different algorithms to achieve this, each of which is described in detail below. Once all three algorithms are done compiling, we represent their runtimes in a table as a means of comparing them.

## Algorithms
##### **Breadth First Search (BFS)**
This algorithm works like a standard BFS with the added feature of maintaining a history of parents to recall the path used to find the destination. This is implemented using a map with WikiNode pointers as keys and values. This maps a WikiNode to its "parent", the previously visited node that linked to it. During traversal, this map is used to keep track of visited nodes as well. If the map does not contain the current article, it is visited and added to the map. Then, once the destination node is reached, the map is traversed beginning with the value linked to the destination node. In this way, the parents can be added until we reach the starting node, at which point the path is finished and returned.
##### **Dijkstra's Algorithm**
Dijkstra's Algorithm is one of the most common algorithms used in finding the shortest path between any two nodes on a given graph. What's special about Dijsktra's is that it takes into account the weight of the edges in the process of finding the shortest path. In our project, we consider the number of outgoing links normalized by a weight factor to be the weight of an edge between two linked articles. We believe this weight serves as a rough approximation of the size of the wikipedia article and allows the player to save time in navigating an article looking for links and also save time in loading an article. The algorithm we implemented is pretty standard, in addition to a mapping scheme similar to BFS where we keep track of a node's parent in order to be able to retrace the path, once we reach the destination. 
##### **Iterative Deepening Depth First Search (IDDFS)**
IDDFS combines the speed efficiency of BFS and the space efficiency of DFS. In IDDFS we implement a depth first search for a controlled depth, and we recursively call the DFS for incremented depth till you find the end node where you return the path or reach a maximum depth where you return an empty vector. We implement two different types of IDDFS, one where we keep track of visited nodes for a particular depth and one where we don't. Although, the algorithm where we keep track of the visited node tends to be relatively slower, it helps us avoid cycles in the given graph and from exploring repeated paths if they exist. The idea of maintainig a visited map to avoid cycles was something we discovered on our own during the development stage.
## Location of deliverables
* All of our source code is present in the folder titled "src". Within src, algorithms.cpp and algorithms.h consist of the implementation for all of the three data traversing algorithms; graph.cpp and graph.h contain our data parsing code; main.cpp is where we put together our whole project and call all of our functions and display the output on the screen; wikinode.h and wikinode.cpp includes implementation for the nodes on the graph.
* Our test cases are stored in the folder titled "tests" within the tests.cpp file.
* Our dataset is stored within the folder titled "dataset".
* Our makefile is stored in the foler titled "build".
* Our final presentation is the video titled "final_presentation.mp4"
* Our final written report is the file titled "results.md"
* An image of the results is stored in the folder titled "Output_PNG"
* Our team contract and team proposal are the files "team-contract.md" and "team-proposal.md"
## Running the Program
##### Main
To run this program, install make and g++, then clone the repository and navigate to `./wikispeedier/build/`. From here, running `make main` will create `main.exe`. In the same directoy, call `./main` to run the program on the default dataset. If you wish to use custom articles and links, create a tsv for each. The articles tsv should just have the article names on each line. The links tsv should have the article and linked article separated by a tab on each line. Once these are created, run `./main <articles_path> <links_path> <article_count> <links_count>` to run the program. Article_count and links_count are the total lines in each tsv respectively.
##### Testing
Test cases have also been written for this program and can be executed by first running `make test` from `../build`. Then, `./test` will run all test cases. These can be viewed in `./wikispeedier/tests/tests.cpp`. The test cases verify that the data parsing algortihm (the wikinode and the graph functions) works as expected by construtcting an accurate graph based on a dummy dataset. We also test for the accuracy of all our algorithms, by testing for valid paths, impossible paths, and expected paths between nodes from the dummy dataset. Furthermore, for Dijkstra's and IDDFS we have unique test cases for a weighted graph and out of depth graph respectively.
