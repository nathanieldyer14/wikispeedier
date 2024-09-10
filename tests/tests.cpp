#include "../src/graph.h"
#include "../src/wikinode.h"
#include "../src/algorithms.h"
#include <cassert>
#include <map>
#include <iostream>

#define NUM_ARTICLES 4604
#define NUM_LINKS 119882
#define TEST_ARTICLES "../dataset/test_articles.tsv"
#define TEST_LINKS "../dataset/test_links.tsv"

/* DATA PARSING TESTS */

void testGraphSize(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);   //creating a graph using the dummy data set
    assert(graph->getMap().size() == 10);                           //check if the size of the graph map is 10 (no. of articles)
    delete graph;
}

void testLinks(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false,TEST_ARTICLES, TEST_LINKS);   //creating a graph using the dummy data set
    vector<WikiNode*> links = graph->getPage("A")->getLinks();      //get all the pages linked to A
    assert(links.size() == 3);      //confirming there are three linked articles
    assert(!links[0]->getName().compare("B"));  //comparing the function returned value against the expected values
    assert(!links[1]->getName().compare("C"));
    assert(!links[2]->getName().compare("D"));
    delete graph;
}

void testAddNode(){
    Graph* graph = new Graph();
    WikiNode* node = new WikiNode("random_name!@#$%^&*(");  //making a dummy node to add to the graph
    graph->addNode(node);
    assert(!graph->getPage("random_name!@#$%^&*(")->getName().compare(node->getName()));    //checking if the node was added
    delete graph;
}


/* WIKINODE TESTS */

void testIsLinkedTo(){
    WikiNode* A = new WikiNode("A");    //creating a pair of dummy wikinodes
    WikiNode* B = new WikiNode("B");

    A->addConnection(B);        //using the addConnection function to connect A and B

    assert(A->isLinkedTo(B));   //checking if the addConnection function worked
    assert(A->isLinkedTo("B"));
    delete A; delete B;
}

void testGetLinks(){
    WikiNode* A = new WikiNode("A");    //creating dummy nodes
    WikiNode* B = new WikiNode("B");
    WikiNode* C = new WikiNode("C");

    A->addConnection(B);    //adding connections
    A->addConnection(C);
    vector<WikiNode*> links = A->getLinks();    //checking if getLinks() function works as intended

    assert(links[0] == B);
    assert(links[1] == C);
    delete A; delete B; delete C;
}

/* BFS TESTS */

void testValidPathBFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false,TEST_ARTICLES, TEST_LINKS);   //creating a graph from the dummy data set
    WikiNode* start = graph->getPage("H");  //selecting an article to start BFS from
    WikiNode* end = graph->getPage("E");    //selecting an article to end BFS on

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getBFSPath(start, end);   //calling the BFS function to find the shortest path between H and E nodes

    assert(!path[0] ->getName().compare("H"));  //checking if the path actually does start from H
    assert(!path[path.size()-1] ->getName().compare("E"));  //checking if the path actually does end on E
    delete graph; delete alg; 
}

void testImpossiblePathBFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("F"); //initiating two nodes that are not connected from the dummy data set
    WikiNode* end = graph->getPage("I");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getBFSPath(start, end); //running BFS for the two disconnected nodes
    assert(path.size() == 0);   //checking if we got an invalid path output which is what we expect
    delete graph; delete alg;
}

void testEqualPathsBFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("J");  //initiating two connected nodes from the dummy data set
    WikiNode* end = graph->getPage("B");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getBFSPath(start, end);   //running the BFS algorithm on them

    assert(!path[1]->getName().compare("A"));   //checking if the returned path is indeed J->A->B
    delete graph; delete alg;
}

/* DIJKSTRA'S ALGORITHM TESTS */

void testValidPathDijkstra(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);   //creating a graph from the dummy data set
    WikiNode* start = graph->getPage("H");  //selecting an article to start Dijkstra's from
    WikiNode* end = graph->getPage("E");    //selecting an article to end Dijkstra's on

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getDijkstraPath(start, end);   //calling the Dijkstra's function to find the shortest path between H and E nodes

    assert(!path[0] ->getName().compare("H"));  //checking if the path actually does start from H
    assert(!path[path.size()-1] ->getName().compare("E"));  //checking if the path actually does end on E
    delete graph; delete alg; 
}

void testImpossiblePathDijkstra(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("F"); //initiating two nodes that are not connected from the dummy data set
    WikiNode* end = graph->getPage("I");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getDijkstraPath(start, end); //running Dijkstra's for the two disconnected nodes
    assert(path.size() == 0);   //checking if we got an invalid path output which is what we expect
    delete graph; delete alg;
}

void testEqualPathsDijkstraWeighted(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("J");  //initiating two connected nodes from the dummy data set
    WikiNode* end = graph->getPage("B");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getDijkstraPath(start, end, 3.0);   //running the Dijkstra's algorithm on them with weight factor 3
    //there are two ways to get from J to B, J->A->B and J->E->B but A has three outgoing links while E only has 2 so Dijkstra should prefer E
    assert(!path[1]->getName().compare("E"));   //checking if the returned path is indeed J->E->B
    
    delete graph; delete alg;
}

void testEqualPathsDijkstraBFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false,TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("J");  //initiating two connected nodes from the dummy data set
    WikiNode* end = graph->getPage("B");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getDijkstraPath(start, end, 500.0);   //running the Dijkstra's algorithm on them with weight factor 500
    //Since the weight factor is 500 the number of outgoing links shouldnt affect the output and we should get the same result as BFS
    vector<WikiNode*> BFSpath = alg->getBFSPath(start, end); 
    assert(path == BFSpath);   //checking if the returned path is the same as BFS
    
    delete graph; delete alg;
}

/* IDFS TESTS */

void testOutofDepthIDDFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);   //creating a graph from the dummy data set
    WikiNode* start = graph->getPage("H");  //selecting an article to start IDFS from
    WikiNode* end = graph->getPage("E");    //selecting an article to end IDFS on

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getIDDFSPath(start, end);   //calling the IDFS function to find the shortest path between H and E nodes
   
   assert(path.size() == 0);   //checking if we got an invalid path output which is what we expect since H and E have a depth > 5
    delete graph; delete alg; 
}

void testImpossiblePathIDDFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("F"); //initiating two nodes that are not connected from the dummy data set
    WikiNode* end = graph->getPage("I");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getIDDFSPath(start, end); //running IDFS for the two disconnected nodes
    assert(path.size() == 0);   //checking if we got an invalid path output which is what we expect
    delete graph; delete alg;
}

void testValidPathsIDDFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false,TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("J");  //initiating two connected nodes from the dummy data set
    WikiNode* end = graph->getPage("B");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getIDDFSPath(start, end);   //running the IDFS algorithm on them
    
    assert(!path[0]->getName().compare("J"));   //checking if the returned path starts with J like it should
    assert(!path[path.size()-1]->getName().compare("B"));   //checking if the returned path starts with B like it should
    delete graph; delete alg;
}


void testEqualPathsIDDFS(){
    Graph* graph = new Graph();
    graph->createGraphFromFile(false, TEST_ARTICLES, TEST_LINKS);
    WikiNode* start = graph->getPage("J");  //initiating two connected nodes from the dummy data set
    WikiNode* end = graph->getPage("B");

    Algorithm* alg = new Algorithm(graph);
    vector<WikiNode*> path = alg->getIDDFSPath(start, end);   //running the IDFS algorithm on them
    
    assert(!path[1]->getName().compare("A"));   //checking if the returned path is indeed J->A->B
    delete graph; delete alg;
}

/* MAIN TEST FUNCTION */

int main(){
    /* Graph */
    testGraphSize();
    testLinks();
    testAddNode();
    std::cout << "Graph tests passed!" << std::endl;

    /* WikiNode */
    testIsLinkedTo();
    testGetLinks();
    std::cout << "WikiNode tests passed!" << std::endl;

    /* BFS */
    testValidPathBFS();
    testImpossiblePathBFS();
    testEqualPathsBFS();
    std::cout << "BFS tests passed" << std::endl;

    /* Dijkstra */
    testValidPathDijkstra();
    testImpossiblePathDijkstra();
    testEqualPathsDijkstraWeighted();
    testEqualPathsDijkstraBFS();
    std::cout << "Dijkstra tests passed" << std::endl;

    /* IDDFS */
    testOutofDepthIDDFS();
    testValidPathsIDDFS();
    testImpossiblePathIDDFS();
    testEqualPathsIDDFS();
    std::cout << "IDDFS tests passed" << std::endl;

    std::cout << "\n#################################" << std::endl;
    std::cout <<   "##### ALL TEST CASES PASSED #####" << std::endl;
    std::cout <<   "#################################" << std::endl;
    return 0;
}