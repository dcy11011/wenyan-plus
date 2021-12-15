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

wenyan: $(YACCC) $(TOKENCPP)
	g++ -std=c++11 $(YACCC) $(CPP)  -o   $(EXE)

$(LEXC): $(LEX)
	flex  $(LEX)

$(YACCC): $(LEXC) $(YACCX)
	bison -d $(YACCX)

$(YACCX): 
	g++ -std=c++11 parseyy.cpp -o parseyy
	./parseyy $(YACC) $(YACCX)
	rm parseyy

$(TOKENCPP): $(YACCC)
	g++ parsetoken.cpp -o parsetoken
	./parsetoken $(YACCH)
	rm parsetoken

clean:
	rm $(EXE) $(YACCX) $(YACCC) $(YACCH) $(LEXC) $(TOKENCPP)