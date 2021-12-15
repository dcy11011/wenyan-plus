#ifndef FIRSTCONVERT
#define FIRSTCONVERT

#include <string>
#include <vector>

#define MAXTOKENLEN 64
#define MAXCHINESETKENLEN 256

struct TrieNode{
    unsigned char value;
    std::vector<TrieNode*> next;
    bool is_end;
    std::string end_value;

    explicit TrieNode();
    virtual ~TrieNode();

    TrieNode * getNext(unsigned char c)const;
};

struct Trie{
    TrieNode * root;
    TrieNode * pos;

    explicit Trie();
    virtual ~Trie();
    bool build(const std::string &pattern, const std::string &end_string);
    void beginWalk();
    bool nextChar(unsigned char c);
    bool isEnd() const;
    std::string endString() const;
    TrieNode* find(const std::string &pattern);
};

class ChineseConverter{
public:
    explicit ChineseConverter();
    ~ChineseConverter();

private:
    Trie trie;

public:
    bool loadConfig(const char *file_name);
    std::string convertString(const std::string &s);
    std::string convertNames(const std::string &s);
};

#endif