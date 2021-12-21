# for linux
EXE = wenyan
CPP = ChineseConvert.cpp ErrorLog.cpp AST.cpp token.cpp

LEX = wenyan.l
LEXC = lex.yy.c

YACC = wenyan.y
YACCX = wenyan.x.y
YACCC = wenyan.x.tab.c
YACCH = wenyan.x.tab.h

TOKENCPP = token.cpp
all: wenyan

wenyan: $(YACCC) $(TOKENCPP) $(CPP)
	g++ -std=c++11 $(YACCC) $(CPP)  -o   $(EXE)

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
	rm $(EXE) $(YACCX) $(YACCC) $(YACCH) $(LEXC) $(TOKENCPP)