# for linux

WY_PROG = test.wy $(wildcard Examples/*.wy)
JS_PROG = $(WY_PROG:.wy=.js)


EXE = wenyan
CPP = ChineseConvert.cpp ErrorLog.cpp AST.cpp token.cpp

LEX = wenyan.l
LEXC = lex.yy.c

YACC = wenyan.y
YACCX = wenyan.x.y
YACCC = wenyan.x.tab.c
YACCH = wenyan.x.tab.h

TOKENCPP = token.cpp

all: examples wenyan
	@echo "Usage: "
	@echo "Compile & Build  | make "
	@echo "Compile Examples | make examples"
	@echo "Build Compiler   | make wenyan"
	@echo "Run JS Programs  | nodejs ./<filename>.js"
	@echo "Run test.js      | make run"

examples: $(JS_PROG)

run: $(JS_PROG)
	nodejs ./test.js

%.js: %.wy wenyan 
	./wenyan $<

wenyan: $(YACCC) $(TOKENCPP) $(CPP)
	g++ -std=c++11 $(YACCC) $(CPP) -o $(EXE) -g

$(LEXC): $(LEX)
	flex  $(LEX)

$(YACCC): $(LEXC) $(YACCX)
	bison -d -W $(YACCX)

$(YACCX): $(YACC) parseyy.cpp parse.config
	g++ -std=c++11 parseyy.cpp -o parseyy
	./parseyy $(YACC) $(YACCX)
	rm parseyy

$(TOKENCPP): $(YACCC) parsetoken.cpp
	g++ parsetoken.cpp -o parsetoken
	./parsetoken $(YACCH)
	rm parsetoken

clean:
	rm $(EXE) $(YACCX) $(YACCC) $(YACCH) $(LEXC) $(TOKENCPP) $(JS_PROG)

.PHONY: clean run all