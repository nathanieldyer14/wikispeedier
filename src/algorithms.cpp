#include "algorithms.h"
#include "graph.h"
#include "wikinode.h"

#include <chrono>
#include <queue>
#include <iostream>
#include <algorithm>

#define NUM_ARTICLES 4604
#define NUM_LINKS 119882

using namespace std;
using namespace std::chrono;

Algorithm::Algorithm(Graph* graph) : graph(graph) {}

/// @brief Compares the three search algorithms in terms of path size and runtime.
/// @param start Starting page
/// @param end Destination page
/// @return Vector of pairs where the first element is a path vector of WikiNode pointers and the second is the algorithm's runtime in seconds.
vector<pair<vector<WikiNode*>, double>> Algorithm::compareAlgs(WikiNode* start, WikiNode* end){
    vector<pair<vector<WikiNode*>, double>> paths;
    vector<WikiNode*> path;
    duration<double, std::milli> time;

    auto startTime = high_resolution_clock::now();
    path = getDijkstraPath(start, end);
    auto endTime = high_resolution_clock::now();
    time = endTime - startTime;
    paths.push_back(pair<vector<WikiNode*>, double>(path, time.count()));

    startTime = high_resolution_clock::now();
    path = getBFSPath(start, end);
    endTime = high_resolution_clock::now();
    time = endTime - startTime;
    paths.push_back(pair<vector<WikiNode*>, double>(path, time.count()));

    startTime = high_resolution_clock::now();
    path = getIDDFSPath(start, end);
    endTime = high_resolution_clock::now();
    time = endTime - startTime;
    paths.push_back(pair<vector<WikiNode*>, double>(path, time.count()));

    startTime = high_resolution_clock::now();
    path = getIDDFSPathVisited(start, end);
    endTime = high_resolution_clock::now();
    time = endTime - startTime;
    paths.push_back(pair<vector<WikiNode*>, double>(path, time.count()));
    return paths;
}

/// @brief This function will print the article names in order of the given path
/// @param path Vector of pointers to the path's WikiNodes
void Algorithm::printPath(vector<WikiNode*> path){
    WikiNode* node;

    for(int i = 0; i < (int)path.size(); i++){
        node = path[i];
        if(i != (int)path.size()-1)
            cout << node->getNameParsed() << " -> ";
        else
            cout << node->getNameParsed() << endl;
    }
}

/// @brief This function determines the shortest path between two WikiNodes
/// using Dijkstra's algorithm.
/// @param start start Starting page
/// @param end end Destination page
/// @return Vector containing the order of pages to visit for the shortest path
vector<WikiNode *> Algorithm::getDijkstraPath(WikiNode *start, WikiNode *end, float weight) {
  typedef pair<float, WikiNode *> distPair;

  /* Min heap to shave time off lookup for the node with minimum distance */
  priority_queue<distPair, vector<distPair>, greater<distPair>> heap; 

  /* Maps WikiNode:pair<parent, distance to parent> */
  map<WikiNode *, pair<float,WikiNode *>> childParent;
  vector<WikiNode *> path; 

  /* Initializing the heap with the source WikiNode pointer */
  heap.push(make_pair(0, start));
  childParent[start] = make_pair(0, nullptr);

  /* Execute Dijkstra's until you reach the destination node */
  while (heap.top().second != end && !heap.empty()) {
    distPair top = heap.top();
    heap.pop();
    WikiNode *thisNode = top.second;
    int distance = top.first;
    for (auto &adjNode : thisNode->getLinks()) {

    /* If adjacent node hasn't already been visited or if the page rank leading up to the adjacent node is less than the page rank that we have via the current path, replace the adjacent node's parent in the heap and the dictionary */
      if (childParent.find(adjNode) == childParent.end() || distance < childParent[adjNode].first) {

        /* Update the neighbors' parents */
        childParent[adjNode] = top; 
        heap.push(

            /* Push current node to heap */
            make_pair(distance + 1 + (adjNode->getLinks().size())/weight, adjNode));
      }
    }
  }
  if (heap.empty()) return vector<WikiNode *>();

  /* Backtracking parents to obtain path */
  WikiNode *currNode = heap.top().second;
  path.push_back(currNode);
  while (childParent[currNode].second != NULL) {
    path.push_back(childParent[currNode].second);
    currNode = childParent[currNode].second;
  }
  reverse(path.begin(), path.end());
  return path;
}


/// @brief This function determines the shortest path between two WikiNodes using a Breadth First Search
/// @param start Starting page
/// @param end Destination page
/// @return Vector containing the order of pages to visit for the shortest path, empty if path cannot be found
vector<WikiNode*> Algorithm::getBFSPath(WikiNode* start, WikiNode* end){
    queue<WikiNode*> queue;
    map<WikiNode*,WikiNode*> parents;
    vector<WikiNode*> curLinks;
    vector<WikiNode*> path;

    WikiNode* curr = start;

    /* Initialize queue with starting node */
    parents.insert(std::pair<WikiNode*,WikiNode*>(curr, end));
    queue.push(curr);

    while(!queue.empty()){
        /* Get current depth and pop curr */
        curr = queue.front();
        queue.pop();
        if(curr == end){                     //we have found the destination and assigned depths along the way
            break;
        }

        /* For each unvisited link from the current node, visit it and assign depth of current + 1 */
        curLinks = curr->getLinks();
        for(auto& link : curLinks) {
            if(parents.find(link) == parents.end()) {
                parents.insert(std::pair<WikiNode*,WikiNode*>(link, curr));
                queue.push(link);
            }
        }
    }

    /* Check to see if the end node was actually reached */
    if(curr != end)
        return vector<WikiNode*>();

    /* Backwards traversal via depth minimization */
    path.insert(path.begin(), curr);
    do {
        curr = parents[curr];
        path.insert(path.begin(), curr);
    } while (curr != start);
    return path;
}

/// @brief This function determines the shortest path between two WikiNodes using Iterative Deepening Depth-First Search; no visited
/// @param start Starting page
/// @param end Destination page
/// @param max_depth Deepest depth it will iterate to (exclusive)
/// @return Vector containing the order of pages to visit for the shortest path
vector<WikiNode*> Algorithm::getIDDFSPath(WikiNode* start, WikiNode* end, int max_depth) {
    int currDepth = 0;
    vector<WikiNode*> result;
    do {
        result = getDLSPath(start, end, currDepth);     // Call helper function based on current depth
        ++currDepth;                                    // Increment current depth
    } while (result.empty() && currDepth < max_depth);  // Repeat until path found or max_depth is reached
    return result;
}

/// @brief Helper function for IDDFS. This function determines the shortest path between two WikiNodes using a depth-limited Depth-First Search.
/// @param start Starting page
/// @param end Destination page
/// @param depth Depth to check for end from
/// @param visited Pointer to vector of all nodes visited from most recently visited to least recently visited
/// @return Pair containing vector of steps and bool if done (found end or none remaining)
vector<WikiNode*> Algorithm::getDLSPath(WikiNode* start, WikiNode* end, int depth) {
    if (depth == 0) {
        return (start == end) ? vector<WikiNode*>(1, start) : vector<WikiNode*>();      // If at zero depth, return destination node (if reached) or empty vector (no possible path)
    } else {   
        for (WikiNode* child : start->getLinks()) {                                     // For each linked article from start, recursively call getDLSPath with decremented depth
            vector<WikiNode*> result = getDLSPath(child, end, depth -1);
            if (!result.empty()) {                                                      // If result is not empty (path exists)...
                result.insert(result.begin(), start);                                   // ... add the start node to the path and return
                return result;
            }
        }
        return vector<WikiNode*>();                                                     // Case where no path can be found: return empty vector.
    }

}

/*
The following two functions are versions of IDDFS
the utilize a visited map to keep track of visited nodes.
This can be used to minimize the number of visited nodes
but may not be beneficial since there is additional time
needed to store these in the map.
*/

/// @brief This function determines the shortest path between two WikiNodes using Iterative Deepening Depth-First Search; no visited
/// @param start Starting page
/// @param end Destination page
/// @param max_depth deepest depth it will iterate to (exclusive)
/// @return Vector containing the order of pages to visit for the shortest path
vector<WikiNode*> Algorithm::getIDDFSPathVisited(WikiNode* start, WikiNode* end, int max_depth) {
    int currDepth = 0;
    vector<WikiNode*> result;
    map<WikiNode*, int>* visited = new map<WikiNode*, int>();
    do {
        result = getDLSPathVisited(start, end, currDepth, visited);
        visited->clear();
        ++currDepth;
    } while (result.empty() && currDepth < max_depth); // while not done
    return result;
}

/// @brief Helper function for IDDFS. This function determines the shortest path between two WikiNodes using a depth-limited Depth-First Search.
/// @param start Starting page
/// @param end Destination page
/// @param depth depth to check for end from
/// @param visited pointer to vector of all nodes visited from most recently visited to least recently visited
/// @return Pair containing vector of steps and bool if done (found end or none remaining)
vector<WikiNode*> Algorithm::getDLSPathVisited(WikiNode* start, WikiNode* end, int depth, map<WikiNode*, int>* visited) {
    if (depth == 0) {
        return (start == end) ? vector<WikiNode*>(1, start) : vector<WikiNode*>();
    } else {
        for (WikiNode* child : start->getLinks()) {
            if (!visited->count(child) || visited->at(child) <= depth) { // if have not visited or have not visited at current or lower depth
                visited->emplace(child, depth);
                vector<WikiNode*> result = getDLSPathVisited(child, end, depth -1, visited);
                if (!result.empty()) { // if result not empty (path exists)
                    result.insert(result.begin(), start);
                    return result;
                }
            }
        }
        return vector<WikiNode*>();
    }

}
