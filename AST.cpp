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
std::string getTempVariableName(int no_new = 0){
    if (!no_new) tmp_var_cnt ++;
    return std::string("temp_") + std::to_string(tmp_var_cnt);
}

std::string Node::indentGenerate(int indent, int indented) {
    if (indented) return "";
    std::string s;
    while (indent-- > 0) s += "  ";
    return s;
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
std::string Node::codeGenerate(int indent, int indented) {
    std::string ret_str = "";
    std::string indent_str = indentGenerate(indent, indented);
    if(m_name == "func_def"){
        std::string func_name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str += indent_str + "var " + func_name + " = _ => {};\n";
        ret_str += indent_str + func_name +" = ";
        ret_str += child(findChildIndexByTokenName("func_params"))->codeGenerate(indent, 1);
        ret_str += " {\n";
    }
    else if(m_name == "func_param_pack"){
        int x = 0;
        if ((x=findChildIndexByTokenName("name_defs",x)) != -1){
            Node *name_def = child(x);
            ret_str = name_def->codeGenerate(indent, indented);
        }
    }
    else if(m_name == "name_def"){
        std::string name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str += name + " => ";
    }
    else if(m_name == "func_end"){
        ret_str += indent_str + "};\n";
    }
    else if(m_name == "RETURN_IT") {
        ret_str += indent_str + "return " + getTempVariableName(1) + ";\n";
    }
    else if(m_name == "return_sentence"){
        ret_str += indent_str + "return " + child(1)->codeGenerate(indent, 1) + ";\n";
    }
    else if(m_name == "function_sentence"){
        for(auto i : m_child_list) ret_str += i->codeGenerate(indent, 1);
        ret_str = indent_str + "var " + getTempVariableName() + " = " + ret_str;
        ret_str += ";\n";
    }
    else if(m_name == "func_use"){
        std::string name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str += indent_str + name + " ";
        Node * params = child(findChildIndexByTokenName("params"));
        if(params != nullptr){
            ret_str += params->codeGenerate(indent, 1);
        }
    }
    else if(m_name == "USE_TO"){
    }
    else if(m_name == "param"){
        for(auto i : m_child_list) ret_str += "(" + i->codeGenerate(indent, 1) + ")";
    }
    else if(m_name == "PRINT_IT") {
        ret_str += indent_str + "console.log(" + getTempVariableName(1) + ");\n";
    }
    else if(m_name == "print_sentence") {
        if(m_child_list.size()) {
            ret_str += indent_str + "console.log(";
            std::string temp_str = "";
            for(auto i : m_child_list)
                if (i->name()=="value" || i->name()=="expression_0") {
                    temp_str += i->codeGenerate(indent, 1);
                    temp_str.push_back(',');
                }
            temp_str.pop_back();
            ret_str += temp_str + ");\n";
        }   
    }
    else if(m_name == "loop_sentence"){
        ret_str += child(findChildIndexByTokenName("loop_statment"))->codeGenerate(indent, indented);
        ret_str += indent_str + "{\n";
        ret_str += child(findChildIndexByTokenName("sentences"))->codeGenerate(indent, 0);
        ret_str += indent_str + "}\n";
    }
    else if(m_name == "value"){
        ret_str = indent_str + child(0)->codeGenerate(indent, 1);
    }
    else if(m_name == "do_times_loop"){
        auto loop_times = getTempVariableName(), loop_temp = getTempVariableName();
        ret_str += indent_str + "var " + loop_times + " = " + child(1)->codeGenerate(indent, 1) + ";\n";// child[1] is loop time (value or NAME or ...)
        ret_str += indent_str + "for(var " + loop_temp + " = 0; " + loop_temp + " < " + loop_times + "; " + loop_temp + "++)\n";
    }
    else if(m_name == "for_in_loop"){
        std::string loop_temp = child(findChildIndexByTokenName("NAME"))->codeGenerate(indent, 1);
        std::string loop_range = child(findChildIndexByTokenName("expression_0"))->codeGenerate(indent, 1);
        ret_str += indent_str + "for(var " + loop_temp + " of " + loop_range + ")\n";
    }
    else if(m_name == "WHILE_TRUE"){
        ret_str += indent_str + "while(true)";
    }
    else if(m_name == "BREAK"){
        ret_str += indent_str + "break;\n";
    }
    else if(m_name == "NUMBER"){
        sprintf(buffer, "%lld", m_val);
        ret_str += indent_str + std::string(buffer);
    }
    else if(m_name == "NAME"){
        ret_str += indent_str + _convert_name(m_str);
    }
    else if(m_name == "IT"){
        ret_str += indent_str + getTempVariableName(1);
    }
    else if(m_name == "INF"){
        ret_str += indent_str + "Infinity";
    }
    else if(m_name == "LOGIC_EQUAL"){
        ret_str += indent_str + " == ";
    }
    else if(m_name == "LOGIC_LESS"){
        ret_str += indent_str + " < ";
    }
    else if(m_name == "LOGIC_GREATER"){
        ret_str += indent_str + " > ";
    }
    else if(m_name == "ARI_ADD"){
        ret_str += indent_str + " + ";
    }
    else if(m_name == "ARI_SUB"){
        ret_str += indent_str + " - ";
    }
    else if(m_name == "ARI_MUL"){
        ret_str += indent_str + " * ";
    }
    else if(m_name == "ARI_DIV"){
        ret_str += indent_str + " / ";
    }
    else if(m_name == "ARI_MOD"){
        ret_str += indent_str + " % ";
    }
    else if(m_name == "ROUND_IT") {
        ret_str = "Math.round(" + getTempVariableName(1) + ");\n";
        ret_str = indent_str + "var " + getTempVariableName() + " = " + ret_str;
    }
    else if(m_name == "FLOOR_IT") {
        ret_str = "Math.floor(" + getTempVariableName(1) + ");\n";
        ret_str = indent_str + "var " + getTempVariableName() + " = " + ret_str;
    }
    else if(m_name == "LIST_E"){
        if (!m_parent || m_parent->m_name != "type")
            ret_str += indent_str + "[]";
    }
    else if(m_name == "concat_sentence"){
        int x1 = findChildIndexByTokenName("valueref");
        int x2 = findChildIndexByTokenName("expression_0", x1 + 1);
        std::string s1 = child(x1)->codeGenerate(indent, 1);
        std::string s2 = child(x2)->codeGenerate(indent, 1);
        ret_str = indent_str + "var " + getTempVariableName() + " = (" + s1 + " = Array.from(" + s1 + ").concat(" + s2 + "));\n";
    }
    else if(m_name == "slice_sentence"){
        int x1 = findChildIndexByTokenName("value");
        int x2 = findChildIndexByTokenName("value", x1 + 1);
        std::string s1 = child(x1)->codeGenerate(indent, 1) + "-1";
        std::string s2 = child(x2)->codeGenerate(indent, 1);
        ret_str = getTempVariableName(1);
        ret_str = indent_str + "var " + getTempVariableName() + " = " + ret_str + ".slice(" + s1 + ", " + s2 + ");\n";
    }
    else if(m_name == "index_sentence"){
        int x1 = findChildIndexByTokenName("expression_0");
        int x2 = findChildIndexByTokenName("value", x1 + 1);
        std::string s1 = child(x1)->codeGenerate(indent, 1);
        std::string s2 = child(x2)->codeGenerate(indent, 1) + "-1";
        ret_str = indent_str + "var " + getTempVariableName() + " = " + s1 + "[" + s2 + "];\n";
    }
    else if(m_name == "IF_BEGIN"){
        ret_str += indent_str + "if (";
    }
    else if(m_name == "IF_STAT"){
        // ret_str += indent_str + ")\n";
        ret_str += ")\n" + indent_str + "{\n";
    }
    else if (m_name == "sentences") {
        if (m_parent && m_parent->m_name != m_name) indent++;
        for(auto i : m_child_list) ret_str += i->codeGenerate(indent, 0);
    }
    else if(m_name == "IF_END" || m_name == "END"){
        ret_str = indent_str + "}\n";
    }
    else if(m_name == "IF_ELSE"){
        ret_str = indent_str +  "else\n" + indent_str + "{\n";
    }
    else if(m_name == "eval_sentence"){
        ret_str += child(findChildIndexByTokenName("expression_0"))->codeGenerate(indent, 1) + ";\n";
        ret_str = indent_str + "var " + getTempVariableName() + " = " + ret_str;
    }
    else if(m_name == "expression_3"){
        int x = findChildIndexByTokenName("single_operator");
        ret_str = child(findChildIndexByTokenName("valuei"))->codeGenerate(indent, 1);
        if (x != -1) {
            if (child(x)->findChildIndexByTokenName("LENGTH") != -1)
                ret_str = "(" + ret_str + ").length";
        }
    }
    else if(m_name == "assign_sentence"){
        std::string name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str += indent_str + name + " = " + getTempVariableName(1) + ";\n";
    }
    else if(m_name == "define_sentence"){
        std::string name = _convert_name(child(findChildIndexByTokenName("NAME"))->str());
        ret_str += indent_str + "var " + name + " = " + getTempVariableName(1) + ";\n";
    }
    else{
        if (m_parent && m_parent->m_name == "if_statment") indented = 1;
        for(auto i : m_child_list) ret_str += i->codeGenerate(indent, indented);
        // ret_str = "<" + m_name + ret_str + ">";
    }
    return ret_str;
}

void __add_child(Node* father, Node* child){
    father->insertChild(child);
}




