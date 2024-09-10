#include "graph.h"
#include "wikinode.h"
#include "algorithms.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

//to build and run, go to /build and run `make && ./main`
int main(int argc, char* argv[]){//int argc, char** argv
    Graph* graph = new Graph();

    try{
        string article_path(argv[1]);
        string links_path(argv[2]);
        cout << "\nCustom Articles: " << article_path << " | Custom Links: " << links_path << endl;
        graph->createGraphFromFile(true,article_path, links_path, atoi(argv[3]), atoi(argv[4]));
    }catch(...){
        cout << "\nUsing default articles & links..." << endl;
        graph->createGraphFromFile(true);
    }

    Algorithm* alg = new Algorithm(graph);
    WikiNode* start = graph->getRandomPage();
    WikiNode* end = graph->getRandomPage();
    vector<string> algorithm_names = {"| Dijkstra        | ", "| BFS             | ", "| IDDFS           | ", "| IDDFS (Visited) | "};

    cout << "\nGOAL: " << start->getNameParsed() << " -> " << end->getNameParsed() << endl;
    cout << "\n    Algorithm     | # |   Time   | Path" << endl;
    cout << "----------------------------------------------------" << endl;
    vector<pair<vector<WikiNode*>, double>> paths = alg->compareAlgs(start, end);

    int alg_idx = 0;
    cout << fixed;
    for(auto& pair : paths){
        cout << algorithm_names[alg_idx++];
        if(pair.first.size() == 0)
            cout << "Path could not be found" << endl;
        else{
            cout << pair.first.size() << " | " << pair.second << " ms | ";
            alg->printPath(pair.first);
            cout << "----------------------------------------------------" << endl;
        }
    }

    delete graph; delete alg;
    return 0;
}