#include "AST.h"
#include "token.h"
#include <cstdlib>

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

std::string _convert_name(const std::string _name){
    std::string ret = "";
    for(auto i : _name)if(isdigit(i)||isalpha(i)||i=='_') ret.push_back(i);
    return ret;
}

// use for convert int/longlong to string
char buffer[30];

// Use this to get a temperaory new variable's name
int tmp_var_cnt = 0;
std::string getTempVariableName(){
    tmp_var_cnt ++;
    return std::string("temp_") + std::to_string(tmp_var_cnt);
}

// Generate code from AST
// note:
//   this function runs recuresively
//   each AST node gose into its 'if' code block,
//   and call its child's code_generate() if needed
//   I choose do it with if-else instead of class inheritance that each 
//   token use its own class, because we don't have much time for 
//   coding. if-else is time saving.
//    
std::string Node::codeGenerate(){
    std::string ret_str = "";
    if(m_name == "func_def"){
        std::string func_name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str = std::string("var ")+func_name+" = _ => {};\n"+func_name+" = ";
        ret_str += child(findChildIndexByTokenName("func_params"))->codeGenerate();
        ret_str += "{\n";
    }
    else if(m_name == "func_param_pack"){
        int x = 0;
        while((x=findChildIndexByTokenName("name_defs",x))!=-1){
            Node* name_def = child(x);
            std::string name = _convert_name(name_def->child(name_def->findChildIndexByTokenName("NAME"))->str());
            ret_str += name + " => ";
            x++;
        }
    }
    else if(m_name == "func_end"){
        ret_str = "};\n";
    }
    else if(m_name == "return_sentence"){
        std::string name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str = "return "+name+";\n";
    }
    else if(m_name == "func_use"){
        std::string name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str = name + " ";
        Node * params = child(findChildIndexByTokenName("params"));
        if(params != nullptr){
            ret_str += params->codeGenerate();
        }
    }
    else if(m_name == "params"){
        for(auto i : m_child_list) ret_str += "(" + i->codeGenerate() + ")";
    }
    else if(m_name == "print_sentence"){
        if(m_child_list.size()){
            ret_str = "console.log(";
            std::string temp_str = "";
            for(auto i : m_child_list) if(i->name()=="value"){
                temp_str += i->codeGenerate();
                temp_str.push_back(',');
            }
            temp_str.pop_back();
            ret_str += temp_str + ");\n";
        }   
    }
    else if(m_name == "loop_sentence"){
        ret_str =  child(findChildIndexByTokenName("loop_statment"))->codeGenerate();
        ret_str += "{\n";
        ret_str += child(findChildIndexByTokenName("sentences"))->codeGenerate();
        ret_str += "}\n";
    }
    else if(m_name == "value"){
        ret_str = child(0)->codeGenerate();
    }
    else if(m_name == "do_times_loop"){
        auto loop_times = getTempVariableName(), loop_temp = getTempVariableName();
        ret_str = std::string("var ") + loop_times + " = " + child(1)->codeGenerate() + ";\n";// child[1] is loop time (value or NAME or ...)
        ret_str += "for(var " + loop_temp + " = 0; " + loop_temp + " < " + loop_times + "; " + loop_temp + "++)";
    }
    else if(m_name == "WHILE_TRUE"){
        ret_str = "while(true)";
    }
    else if(m_name == "BREAK"){
        ret_str = "break;\n";
    }
    else if(m_name == "NUMBER"){
        sprintf(buffer, "%lld", m_val);
        ret_str = std::string(buffer);
    }
    else if(m_name == "NAME"){
        ret_str = _convert_name(m_str);
    }
    else if(m_name == "LOGIC_EQUAL"){
        ret_str = "==";
    }
    else if(m_name == "LOGIC_LESS"){
        ret_str = "<";
    }
    else if(m_name == "LOGIC_GREATER"){
        ret_str = ">";
    }
    else{
        for(auto i : m_child_list) ret_str += i->codeGenerate();
    }
    return ret_str;
}

void __add_child(Node* father, Node* child){
    father->insertChild(child);
}




