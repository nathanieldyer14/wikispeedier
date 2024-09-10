#pragma once

#include <string>
#include <vector>
#include <map>
#include "wikinode.h"

#define ARTICLES "../dataset/articles.tsv"
#define LINKS "../dataset/links.tsv"
#define NUM_ARTICLES 4604
#define NUM_LINKS 119882
using namespace std;

class Graph{
    public:
        Graph();
        ~Graph();
        WikiNode* getPage(string page_name);
        WikiNode* getRandomPage();
        void createGraphFromFile(bool print_progress, string articles_path = ARTICLES, string links_path = LINKS, 
                                 int n_articles = NUM_ARTICLES, int n_links = NUM_LINKS);
        void addNode(WikiNode* node);
        map<string, WikiNode*>& getMap();

    private:
        map<string, WikiNode*> name_node_map;
        int num_articles;
        int num_links;
};