# CS225 Final Project Report
## Project: wikispeedier
## Names: Nathaniel Dyer, Ayush Garg, Ethan Greenwald, Nitya Sunkad


### Output
![output](/Output_PNG/Results.png)
![output](/Output_PNG/Results2.png)

### Overview of tests
The test cases we have written verify that the data parsing algortihm (the wikinode and the graph functions) works as expected by construtcting an accurate graph based on a dummy dataset. We also test for the accuracy of all our algorithms, by testing for valid paths, impossible paths, and expected paths between nodes from the dummy dataset. Furthermore, for Dijkstra's and IDDFS we have unique test cases for a weighted graph and out of depth graph respectively.

#### BFS - O(V + E)
This algorithm works like a standard BFS with the added feature of maintaining a history of parents to recall the path used to find the destination. This is implemented using a map with WikiNode pointers as keys and values. This maps a WikiNode to its "parent", the previously visited node that linked to it. During traversal, this map is used to keep track of visited nodes as well. If the map does not contain the current article, it is visited and added to the map. Then, once the destination node is reached, the map is traversed beginning with the value linked to the destination node. In this way, the parents can be added until we reach the starting node, at which point the path is finished and returned.

For BFS we are testing three situations:
* No valid path exists in the data set between the two given nodes from the dummy dataset
* If a valid path exists, it starts and ends where its supposed to
* We get the exact path that we expect for two nodes on our dummy data set

#### Dijkstra’s Algorithm - O(V<sup>2</sup>)
Dijkstra’s Algorithm is a popular algorithm for finding shortest paths in a weighted, directed graph. The algorithm works by starting at a source node in the graph and exploring all of its neighbors, and adding the lowest cost node to the path. Similar to BFS, which constructs a path using a FIFO queue, Dijkstra’s constructs a path using a min-heap (priority queue) to prioritize adding nodes with shorter distances from source to the path.

In our implementation, we find the shortest path from our source article to our destination article by running Dijkstra’s on the graph of WikiNodes, using a min-heap implemented with a priority queue. Weights are assigned to nodes based on how many outgoing links they have, which is a rough estimate of the page size and load time of each article.

For Dijkstra’s we are testing four situations:
* No valid path exists in the data set between the two given nodes from the dummy dataset
* If a valid path exists, it starts and ends where its supposed to
* If the weight factor (the dividing factor for the number of outgoing links before they are added to the weight of an edge) is comparable to the outgoing links in a given path, it chooses the path with the less number of outgoing links
* If the weight factor is not comparable and the number of outgoing links don’t matter, then we get the same result as BFS

#### Iterative Deepening Depth-First Search (IDDFS) - O(V+E)
IDDFS combines the speed efficiency of BFS and the space efficiency of DFS. In IDDFS we implement a depth first search for a controlled depth, and we recursively call the DFS for incremented depth till you find the end node where you return the path or reach a maximum depth where you return an empty vector. We implement two different types of IDDFS, one where we keep track of visited nodes for a particular depth and one where we don't. Although, the algorithm where we keep track of the visited node tends to be relatively slower, it helps us avoid cycles in the given graph and from exploring repeated paths if they exist. The idea of maintainig a visited map to avoid cycles was something we discovered on our own during the development stage.

For IDDFS we are testing four situations:
* No valid path exists in the data set between the two given nodes from the dummy dataset
* If a valid path exists, it starts and ends where its supposed to
* A valid path exists between two given nodes but the distance between the nodes is greater than the maximum depth specified
* A valid path, within the depth threshold, exists and is returned as expected

### Answer to Leading Question
Our leading objective was finding the shortest path between any two given wikipedia articles from the SNAP dataset. We were able to do so using three different algorithms: BFS, Dijkstra's, IDDFS. We discovered that IDDFS was by far the fastest algorithm to discover the most efficient path due to its optimization of time and space complexity. Furthermore, the occurrence of a path not found between source and destination Wikipedia articles was more common than anticipated

