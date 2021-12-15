#include "AST.h"
#include "token.h"

Node::Node(int _type, const TextPosition & _pos):
    m_type(_type),
    m_pos(_pos)
{
    m_name = getTokenName(m_type);
}

Node::Node(const std::string &_name, const TextPosition &_pos):
    m_name(_name),
    m_pos(_pos)
{
}

int Node::line(){
    return m_pos.line;
}

int Node::row(){
    return m_pos.row;
}

Node* Node::parent(){
    return p;
}

void Node::setParent(Node * _parent){
    p = _parent;
}

Node* Node::child(int index){
    if(index>=c.size())return nullptr;
    return c[index];
}
void Node::insertChild(Node * _child){
    _child->setParent(this);
    c.push_back(_child);
}

std::string Node::name(){
    return m_name;
}

void Node::setName(std::string _name){
    m_name = _name;
}

int Node::val(){
    return m_val;
}

void Node::setVal(int _val){
    m_val = _val;
}

void Node::printDfs(Node *node, int depth){
    for(int i=0;i<depth;i++)printf("  ");
    node->print();
    puts("");
    for(auto i : node->c)
        if(i!=nullptr) printDfs(i, depth+1);
    return;
}

void Node::print(){
    printf("[%s]",m_name.c_str());
    if(line()>=0)printf(" line %d, row %d ", line(), row());
}

void Node::printAll(){
    printDfs(this, 0);
}
void __add_child(Node* father, Node* child){
    father->insertChild(child);
}




