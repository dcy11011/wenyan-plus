#ifndef ASTH
#define ASTH

#include <iostream>
#include <cstdio>
#include <string>
#include <stack>
#include <vector>

#define YYSTYPE Node*

// Save temp values in code
extern std::stack<std::string> temp_value_stack;
extern int temp_value_cnt;

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
    std::vector<Node*> m_child_list;// list of child
    std::string m_name;// name of token
    long long m_val;
    int m_type;
    std::string m_str;
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
    int findChildIndexByTokenName(const std::string name, int start_index = 0);
    void insertChild(Node *);

    std::string name();
    void setName(std::string _name);
    long long val();
    void setVal(long long _val);
    std::string& str();
    void setStr(const std::string &_str);

    void print();// print the information of this node
    void printAll();// print the subtree

    std::string codeGenerate();//generate target code from this AST
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

