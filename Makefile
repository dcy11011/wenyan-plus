# for linux
EXE = wenyan
CPP = ChineseConvert.cpp ErrorLog.cpp AST.cpp token.cpp

LEX = wenyan.l
LEXC = lex.yy.c

YACC = wenyan.y
YACCC = wenyan.tab.c

TOKENCPP = token.cpp
all: wenyan

wenyan: $(YACCC) $(TOKENCPP)
	g++ -std=c++11 $(YACCC) $(CPP)  -o   $(EXE)

$(LEXC): $(LEX)
	flex  $(LEX)

$(YACCC): $(LEXC)
	bison -d $(YACC)

$(TOKENCPP):
	g++ parsetoken.cpp -o parsetoken
	./parsetoken wenyan.tab.h
	rm parsetoken

clean:
	rm $(EXE)  $(YACCC) wenyan.tab.h $(LEXC) $(TOKENCPP)