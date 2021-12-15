#ifndef ASTH
#define ASTH

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#define YYSTYPE Node*

// Class for record token position in source file
struct TextPosition{
    int line;
    int row;
    TextPosition(int _line, int _row):line(_line), row(_row){};
};

// Class of a AST node
// for coding convience, all types of token use this class.
class Node{
    TextPosition m_pos;
    Node * m_parent;// parent
    std::vector<Node*> m_childList;// list of child
    std::string m_name;// name of token
    int m_val;
    int m_type;
protected:
    void printDfs(Node * node, int depth); // print this node and all its children recursively

public:    
    Node(int _type, const TextPosition & pos = TextPosition(0,0));
    Node(const std::string &_name, const TextPosition &pos = TextPosition(0,0));

    int line();
    int row();
    Node* parent();
    void setParent(Node *);
    Node* child(int index);
    void insertChild(Node *);

    std::string name();
    void setName(std::string _name);
    int val();
    void setVal(int _val);

    void print();// print the information of this node
    void printAll();// print the subtree
};


void __add_child(Node* father, Node* child);

template <typename... Args>
void __add_child(Node* father, Node* child, Args... args){
    __add_child(father, child);
    __add_child(father, args...);
}

// use to create a parent node
// param:   parent node name, and all its children
// return:  return pointer to the parent node created
template <typename... Args>
Node* makeNode(const std::string  &_name, Args... args){
    Node *node = new Node(_name);
    __add_child(node, args...);
    return node;
}

#endif

