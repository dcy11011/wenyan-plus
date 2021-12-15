#ifndef ASTH
#define ASTH

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#define YYSTYPE Node*

struct TextPosition{
    int line;
    int row;
    TextPosition(int _line, int _row):line(_line), row(_row){};
};

class Node{
    TextPosition m_pos;
    Node * p;
    std::vector<Node*> c;
    std::string m_name;
    int m_val;
    int m_type;

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

};


void __add_child(Node* father, Node* child);

template <typename... Args>
void __add_child(Node* father, Node* child, Args... args){
    __add_child(father, child);
    __add_child(father, args...);
}

template <typename... Args>
Node* makeNode(const std::string  &_name, Args... args){
    Node *node = new Node(_name);
    __add_child(node, args...);
    return node;
}

#endif

