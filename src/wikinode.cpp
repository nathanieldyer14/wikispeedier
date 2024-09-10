#include "wikinode.h"
#include <string>
#include <vector>
#include <iostream>
#define NUM_ARTICLES 4604

using namespace std;


WikiNode::WikiNode() : page_name("") {}

WikiNode::WikiNode(string name) : page_name(name) {}

WikiNode::~WikiNode(){
    /* TODO bc i don't like dealing with memory leaks */
}

string WikiNode::getName(){
    return page_name;
}

/// @brief Helper function to replace encoded characters with their unicode equivalent
/// @param value String to be decoded
/// @return Decoded string
string UrlDecode(const string& value)
{
    string result;
    result.reserve(value.size());
    
    for (size_t i = 0; i < value.size(); ++i){
        auto ch = value[i];
        
        if (ch == '%' && (i + 2) < value.size()){
            auto hex = value.substr(i + 1, 2);
            auto dec = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
            result.push_back(dec);
            i += 2;
        }
        else if (ch == '_')
            result.push_back(' ');
        else
            result.push_back(ch);
    }
    return result;
}

/// @brief Getter for an article's decoded name
/// @return Name of the article after being parsed
string WikiNode::getNameParsed(){
   return UrlDecode(page_name);
}

/// @brief Getter for an article's links
/// @return Vector containing pointers to linked articles
vector<WikiNode*> WikiNode::getLinks(){ return links; }


/// @brief Util function to print the names of the linked articles
void WikiNode::printLinks(){
    for(auto& link : links)
        cout << "   " << link->getNameParsed() << endl;
}

/// @brief Adds a directed link to another article
/// @param other Node to establish connection to
void WikiNode::addConnection(WikiNode* other){
    links.push_back(other);
}

/// @brief Checks whether or not this article is linked to another one
/// @param name Name of the potentially linked article
/// @return true if there is a link, false if there isn't
bool WikiNode::isLinkedTo(string name){
    for(auto& link : links)
        if(!link->getName().compare(name))
            return true;
    return false;
}

/// @brief Checks whether or not this article is linked to another one
/// @param other Potentially linked WikiNode
/// @return true if there is a link, false if there isn't
bool WikiNode::isLinkedTo(WikiNode* other){
    for(auto& link : links)
        if(link == other)
            return true;
    return false;
}