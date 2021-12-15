# for linux
EXE = wenyan
CPP = ChineseConvert.cpp ErrorLog.cpp AST.cpp token.cpp

LEX = wenyan.l
LEXC = lex.yy.c

YACC = wenyan.y
YACCX = wenyan.x.y
YACCC = wenyan.x.tab.c

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

$(TOKENCPP):
	g++ parsetoken.cpp -o parsetoken
	./parsetoken wenyan.x.tab.h
	rm parsetoken

clean:
	rm $(EXE)  $(YACCC) wenyan.x.tab.h $(LEXC) $(TOKENCPP)