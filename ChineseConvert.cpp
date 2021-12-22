#include "ChineseConvert.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <set>
#include "ErrorLog.h"
using namespace std;

TrieNode::TrieNode():
     is_end(false)
{
}

TrieNode::~TrieNode(){
    for(auto i : next)if(i!=nullptr)
        delete i;
}

TrieNode * TrieNode::getNext(unsigned char c) const{
    for(auto i : next)
        if(i!=nullptr && i->value == c) return i;
    return nullptr;
}


Trie::Trie():
     root(nullptr)
    ,pos(nullptr)
{
}

Trie::~Trie(){
    if(root!=nullptr)delete root;
}

bool Trie::build(const string &pattern, const string &end_string){
    if(root == nullptr) {
        root = new TrieNode;
        if(pos == nullptr) pos = root;
    }
    TrieNode *p = root;
    for(auto c : pattern){
        TrieNode *next = p->getNext(c);
        if(next == nullptr){
            next = new TrieNode;
            next->value = c;
            p->next.push_back(next);
        }
        p = next;
    }
    if(p->is_end) return false;
    p->is_end = true;
    if (isdigit(end_string.at(0)) ||
        end_string == "s" || end_string == "b" || end_string == "k" ||
        end_string == "w" || end_string == "k" )
        p->end_value = end_string;
    else
        p->end_value = std::string(" ") + end_string + " ";
    return true;
}

void Trie::beginWalk(){
    pos = root;
}

bool Trie::nextChar(unsigned char c){
    if(pos == nullptr) return false;
    TrieNode *next = pos->getNext(c);
    pos = next;
    return next != nullptr;
}

bool Trie::isEnd() const{
    if(pos!=nullptr) return pos->is_end;
    return false;
}

std::string Trie::endString() const{
    if(pos ==  nullptr) return std::string("");
    if(isEnd()) return pos->end_value;
    return std::string("");
}

TrieNode* Trie::find(const std::string &pattern){
    auto last_pos = pos;
    beginWalk();
    for(auto c:pattern)
        if(!nextChar(c))return nullptr;
    auto ret = pos;
    pos = last_pos;
    return ret;
}

ChineseConverter::ChineseConverter(){
    
}

ChineseConverter::~ChineseConverter(){

}

bool ChineseConverter::loadConfig(const char *file_name){
    FILE* conf_file;
    if((conf_file = fopen(file_name,"r"))==NULL){
        wyLog(log_error, "Can't open Chinese parse config");
        return false;
    }
    char c = EOF;
    vector<unsigned char> tok_buf, oringin_tok_buf;
    enum token_type{
        token_target,
        token_oringin
    }stat;
    while((c=fgetc(conf_file)) != EOF){
        if(c == ' '){
            if(tok_buf.size()&&oringin_tok_buf.size()){
                tok_buf.push_back(0);
                oringin_tok_buf.push_back(0);
                if(!trie.build((char*)oringin_tok_buf.data(), (char*)tok_buf.data())){
                    wyLog(log_warning)<<"Chinese parsing file containing confilicting rules "<<((char*)oringin_tok_buf.data())<<std::endl;
                }
            }
            oringin_tok_buf.clear();
            stat = token_oringin;
            continue;
        }
        if(c == '\n' || c == '\r'){
            if(tok_buf.size()&&oringin_tok_buf.size()){
                tok_buf.push_back(0);
                oringin_tok_buf.push_back(0);
                if(!trie.build((char*)oringin_tok_buf.data(), (char*)tok_buf.data())){
                    wyLog(log_warning, "chinese parsing file containing confilicting rules");
                }
            }
            tok_buf.clear();
            oringin_tok_buf.clear();
            stat = token_target;
            continue;
        }
        if(stat == token_target)
            tok_buf.push_back(c);
        else if(stat == token_oringin)
            oringin_tok_buf.push_back(c);
    }
    return true;
}

std::string ChineseConverter::convertString(const std::string &str){
    trie.beginWalk();
    
    std::vector<char> buf;
    std::vector<char> output;

    int match_index = -1;
    std::string last_match = "";

    string s = str + std::string("$");// this '$' makes all buffer fall dismatch at last, so that after this loop all char in oringin string will be handled
    int depth = 0; // how many '(' unmatched now. this function ignore all string in "「 ... 」"
    for(int i=0;i<s.size();i++){
        //cout<<"i="<<i<<" lastmatch="<<last_match<<" match_index="<<match_index<<" buf=";for(int i=0;i<buf.size();i++)printf("%02x ",(unsigned char)buf[i]);cout<<endl;
        if(i+2<s.size()&&(unsigned char)s[i]==0xe3&&(unsigned char)s[i+1]==0x80&&(unsigned char)s[i+2]==0x82){
            // ignore "。"
            i+=2;
            continue;
        }
        unsigned char c = s[i];
        buf.push_back(c);
        if(!trie.nextChar(c)){
            if(match_index < 0){
                // if no match in this buf, just push the oringin string and go on matching following
                output.insert(output.end(), buf.begin(), buf.end());
                buf.clear();
                trie.beginWalk();
            }
            else{
                // if there is mathc in buf, push the matched string and restart matching from last matched position
                if(last_match == "(" || "\""){
                    depth++;
                }
                if(last_match == ")" || "\""){
                    depth--;
                }
                output.insert(output.end(), last_match.begin(), last_match.end());
                buf.clear();
                trie.beginWalk();
                i = match_index;
                match_index = -1;
            }
            continue;
        }
        if( !depth && trie.isEnd() ){
            match_index = i;
            last_match = trie.endString();
            continue;
        }
    }
    output.pop_back();
    output.push_back(0);
    return convertNames(std::string(output.data()));
}

std::string int2string(int x){
    std::string ret ="";
    if(!x){
        return "0";
    }
    while(x){
        ret.insert(ret.begin(), '0'+(x%10));
        x/=10;
    }
    return ret;
}

std::string ChineseConverter::convertNames(const std::string &s){
    vector<char> output;
    vector<char> buf;
    int depth = 0;// how many '(' unmatched now
    Trie names;
    int id = 0;
    for(int i=0;i<s.size();i++){
        unsigned char c = s[i];
        if(c == ' '){
            if(output.size() && output[output.size()-1]!=' ')output.push_back(c);
            continue;
        }
        if(c == '(') {
            depth++;
            if(depth > 1){
                wyLog(log_error, "Too much left quote");
                return "";
            }
        }

        if(depth == 0)output.push_back(c);
        else buf.push_back(c);

        if(c == ')') {
            depth --;
            if(buf.size()&&depth==0){
                buf.push_back(0);
                auto match = names.find(std::string(buf.data()));
                if(match != nullptr){
                    output.insert(output.end(), match->end_value.begin(), match->end_value.end());
                }
                else{
                    std::string var_name = std::string("$name_")+int2string(id++);
                    names.build(std::string(buf.data()), var_name);
                    output.insert(output.end(), var_name.begin(), var_name.end());
                }
                buf.clear();
            }
        }
    }
    output.push_back(0);
    return std::string(output.data());
}