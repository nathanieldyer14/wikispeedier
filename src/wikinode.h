#pragma once

#include <string>
#include <vector>

using namespace std;

class WikiNode{
    public:
        WikiNode();
        ~WikiNode();
        WikiNode(string name);

        string getName();
        string getNameParsed();
        vector<WikiNode*> getLinks();

        void addConnection(WikiNode* other);

        bool isLinkedTo(string name);
        bool isLinkedTo(WikiNode* other);

        void printLinks();

    private:
        string page_name;
        vector<WikiNode*> links;

};