#pragma once

#include <string>
#include <utility>
#include "wikinode.h"
#include "graph.h"

using namespace std;

class Algorithm {
    public:
        Algorithm(Graph* graph);
        vector<pair<vector<WikiNode*>, double>> compareAlgs(WikiNode* start, WikiNode* end);
        vector<WikiNode*> getDijkstraPath(WikiNode* start, WikiNode* end, float weight = 500.0);
        vector<WikiNode*> getBFSPath(WikiNode* start, WikiNode* end);
        vector<WikiNode*> getIDDFSPath(WikiNode* start, WikiNode* end, int max_depth = 5);
        vector<WikiNode*> getIDDFSPathVisited(WikiNode* start, WikiNode* end, int max_depth = 5);
        void printPath(vector<WikiNode*> path);
    private:
        vector<WikiNode*> getDLSPath(WikiNode* start, WikiNode* end, int depth);
        vector<WikiNode*> getDLSPathVisited(WikiNode* start, WikiNode* end, int depth, map<WikiNode*, int>* visited);
        Graph* graph;
};