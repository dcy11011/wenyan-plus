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

## code: 添加新语法

添加新语法分为几个步骤：

1. 在parse.config中添加中文到ASICC标识符的翻译规则。该文件的格式为，每行表示一个转换规则，每行包含若干空格分隔的字符串，其中第一个是中文对应的ASICC串，之后的都是要翻译成第一个串的中文串。例如，“name_as 名之曰 曰”会把文言代码中所有“名之曰”和“曰”替换成“name_as”（并自动插入空格分隔）。翻译时会优先匹配最长串，例如同时有“index 之”规则和“func_end 之术也”规则时，转换时遇到“之术也”时会优先匹配后者。这部分由ChineseConvert.h/.cpp实现。

2. **在wenyan.l中添加词法（添加新的token）。在.l文件中添加正则表达式和匹配后执行的代码，在.y文件中添加token定义。**.l文件是lex文件，会被flex编译成可供其他程序调用的.c/.h文件。其整体格式分为三段，第一段是c/c++代码，会被插入到生成的代码首部，可以放一些include语句、定义一些函数；第二部分是词法部分，是识别token的规则。可以参照已有的格式，一行第一段是一个正则表达式，第二段是大括号包裹的c/c++代码，这段代码会在词法器匹配到这行前面的正则表达式时执行。如果这段代码有返回值，则这个token会被识别并被传递给语法分析器。匹配后，匹配的字符串会被flex保存在全局变量yytext中。可以把token的一些信息放在另一个全局变量yylval中。这里我把yylval定义成了Node *类型，用来保存一个AST节点，并在每次匹配时都生成一个AST节点保存在yylval中。这里AST节点Node的构造函数接受两个参数，分别token的编号和这个token的位置（参见代码）。token的其他信息可以保存在生成的Node中，例如Node::setStr(...)可以保存一个字符串（用Node::str()访问）,Node::setVal(...)可以保存一个int64值（Node::val()访问）。另外，可以看到，wenyan.l文件include了wenyan.x.tab.h文件，这个文件定义了由语法分析器生成的token编码枚举量。这个.h文件是wenyan.y根据其中%token xxxx的语句生成的。添加新的token时，添加一句%token xxx即可。

3. **在wenyan.y中添加语法规则。这个文件的结构和.l文件类似。在里面看起来像是文法规则的地方加上新的规则即可。** 每行文法规则的格式为，第一个字符串为生成式左侧的非终结符，然后紧跟一个分号，接下来是这个非终结符生成的表达式，每个表达式是终结符和非终结符用空格分隔的序列，表达式之间用竖线分隔。每个表达式的末尾也可以添加用大括号包裹的代码段，也是在匹配成功时会被执行。代码段中，可以用\$1，\$2，\$3,...,$n表示生成式右侧的第n个终结符/非终结符的yylval，也就是它们的AST节点指针；用$$表示这个节点自己的指针。**.y文件中的每个生成式会被parseyy.cpp编译的脚本自动插入一句\$\$=makeNode("name",\$1,...)来创建一个名字为生成式左侧非终结符的节点，并连接当前节点和子节点。**所以不需要手动写这一步，可以直接在大括号中写生成AST节点过后的处理。

4. **在AST.cpp中添加目标代码生成。**在codeGenerate函数中添加token对应的生成代码。codeGenerate函数里面是一堆if，根据m_name(保存节点对应的token名，上面.y中自动插入的代码负责填入的)选择对应的方法生成。codeGenerate的思路是递归调用子树的codeGenerate来拼合字符串。添加新的文法在这里添加if分支和对应的代码即可。

5.**测试。**目前，make后运行程序，会读入test.wy中的文言代码（**需utf-8编码**），然后输出到test.js中，并会输出中间过程转换的结果。


