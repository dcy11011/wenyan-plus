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
    return m_parent;
}

void Node::setParent(Node * _parent){
    m_parent = _parent;
}

Node* Node::child(int index){
    if(index>=m_child_list.size()||index <0)return nullptr;
    return m_child_list[index];
}
void Node::insertChild(Node * _child){
    _child->setParent(this);
    m_child_list.push_back(_child);
}

std::string Node::name(){
    return m_name;
}

void Node::setName(std::string _name){
    m_name = _name;
}

long long Node::val(){
    return m_val;
}

void Node::setVal(long long _val){
    m_val = _val;
}

int Node::findChildIndexByTokenName(const std::string name, int start_index){
    for(int i=start_index;i<m_child_list.size();i++)
        if(m_child_list[i]!=nullptr && m_child_list[i]->name()==name)return i;
    return -1;
}

void Node::printDfs(Node *node, int depth){
    for(int i=0;i<depth;i++)printf("  ");
    node->print();
    puts("");
    for(auto i : node->m_child_list)
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

void Node::setStr(const std::string &_str){
    m_str = _str;
}

std::string& Node::str(){
    return m_str;
}

std::string __parse_name(const std::string _name){
    std::string ret = "";
    for(auto i : _name)if(isdigit(i)||isalpha(i)||i=='_') ret.push_back(i);
    return ret;
}

std::stack<std::string> temp_value_stack;
int temp_value_cnt = 0;

char buffer[30];

std::string Node::code_gen(){
    std::string ret_str = "";
    if(m_name == "func_def"){
        std::string func_name = __parse_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str = std::string("var ")+func_name+" = _ => {};\n"+func_name+" = ";
        ret_str += child(findChildIndexByTokenName("func_params"))->code_gen();
        ret_str += "{\n";
    }
    else if(m_name == "func_param_pack"){
        int x = 0;
        while((x=findChildIndexByTokenName("name_defs",x))!=-1){
            Node* name_def = child(x);
            std::string name = __parse_name(name_def->child(name_def->findChildIndexByTokenName("NAME"))->str());
            ret_str += name + " => ";
            x++;
        }
    }
    else if(m_name == "func_end"){
        ret_str = "};\n";
    }
    else if(m_name == "return_sentence"){
        std::string name = __parse_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str = "return "+name+";\n";
    }
    else if(m_name == "func_use"){
        std::string name = __parse_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str = name + " ";
        Node * params = child(findChildIndexByTokenName("params"));
        if(params != nullptr){
            ret_str += params->code_gen();
        }
    }
    else if(m_name == "params"){
        for(auto i : m_child_list) ret_str += "(" + i->code_gen() + ")";
    }
    else if(m_name == "print_sentence"){
        if(m_child_list.size()){
            ret_str = "console.log(";
            ret_str += m_child_list[0]->code_gen();
            for(auto i : m_child_list) if(i->name()=="value"){
                if( i == *m_child_list.begin()) continue;
                ret_str.push_back(',');ret_str.push_back(' ');
                ret_str += i->code_gen();
            }
            ret_str += ");\n";
        }   
    }
    else if(m_name == "value"){
        ret_str = child(0)->code_gen();
    }
    else if(m_name == "NUMBER"){
        sprintf(buffer, "%lld", m_val);
        ret_str = std::string(buffer);
    }
    else if(m_name == "NAME"){
        ret_str = __parse_name(m_name);
    }
    else{
        for(auto i : m_child_list) ret_str += i->code_gen();
    }
    return ret_str;
}

void __add_child(Node* father, Node* child){
    father->insertChild(child);
}




