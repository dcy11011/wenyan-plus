# wenyan+

## 介绍

wenyan+是一个简化版魔改版的wenyan-lang，将文言代码编译成Javascript代码。

## 环境

开发环境：ubuntu 16.04
依赖：g++, gcc, flex, bison
```
sudo apt install g++ gcc flex bison
```

## 构建

使用Makefile构建，在项目文件夹下执行：
```
make
```

## code:编译器生成

wenyan+编译一个程序经过以下流程：
* 读取输入源文件，通过ChineseConverter::convertString()(ChineseConvert.h)将读取的utf-8编码的中文文件转换为ASCII编码代码，输出到.wytmp文件
* 词法分析， 通过flex完成(wenyan.l)。
* 语法分析，获得语法分析树，通过bison完成(wenyan.y)。
* 生成目标代码

wenyan+编译器通过以下流程生成：
* flex编译wenyan.l文件，生成lex.yy.c文件，提供yytext()语法分析函数供后续语法分析使用
* parseyy.cpp编译生成paeseyy脚本程序，为wenyan.y文件插入生成AST相关代码，生成wenyan.x.y文件
* bison编译wenyan.x.y文件生成wenyan.x.tab.c/wenyan.x.tab.h文件。编译器主函数就位于wenyan.x.tab.c中（主函数写在wenyan.y中）。
* parsetoken.cpp编译生成parsetoken脚本程序，读入wenyan.x.tab.h文件，分析该文件中token数值和名称对应关系，生成token数值到字符串名称的对应函数，以供后续使用（例如生成AST时，（为了方便）使用了字符串来标记节点类型）。
* g++编译上述生成的所有.c文件以及其他.cpp文件，生成编译器

## code: AST结构

AST(抽象语法树)定义在AST.h/AST.cpp中。

AST的基本单位是节点(Class Node)，每个节点保存了祖先节点指针和儿子节点指针列表。通过相关方法可以查询和修改父子关系，详见代码(AST.h)。

AST的叶子节点在wenyan.l中生成。这段代码被flex编译后执行时，在每个token被识别时都会生成一个对应的Node，并记录名称、位置。添加新的token请参考原有的token的格式。

AST的非叶子节点在wenyan.x.y中生成。这个文件是通过parseyy脚本，在make的时候自动生成的。parseyy会在每个产生式右侧表达式后，按照产生式的结构，添加一个生成非叶节点的代码。生成的非叶节点的名称即为wenyan.y文件中产生式左侧非终结符的名称。根节点被保存在全局变量root中(wenyan.y)。

打印子树可以调用Node::printAll()方法。

