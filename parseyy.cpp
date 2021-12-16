#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

char buf[1024], name[1024];

int main(int argc, char ** argv){
    string sfilename = argv[1], tfilename = "";
    if(argc==3)tfilename = argv[2];
    FILE *fin  = fopen(sfilename.c_str(), "r");
    if(tfilename == "") tfilename = "wenyan.x.y";
    FILE *fout = fopen(tfilename.c_str(), "w");
    
    char c,last=' ';
    while((c=fgetc(fin))!=EOF){
        fputc(c, fout);
        if(c == '%' && last == '%')break;
        last = c;
    }

    enum stages{
        none,
        read_left,
        read_right_token,
        read_right_code,
        end_right_code,
    };
    int stage = none, is_end = 0, cnt = 0;
    int name_cur = 0;
    int brace = 0;
    while((c = fgetc(fin))!=EOF){
        if(!stage&&isalpha(c)){
            name_cur = 0;
            stage = read_left;
        }
        else if(c ==':'){
            stage = read_right_token; 
            name[name_cur] = 0;
            cnt = 0;
        }
        else if(stage==read_right_token){
            if(isspace(c)&&isalpha(last)) cnt++;
            if(c=='|'){
                if(!isspace(last))cnt++;
                is_end = 1;
            }
            if(c=='{'){
                if(!isspace(last))cnt++;
                is_end = 2;
                stage = read_right_code;
                brace = 1;
            }
            if(c==';'){
                if(!isspace(last))cnt++;
                stage = none;
                is_end = 1;
            }
        }
        else if(stage == read_right_code){
            if(c=='{')brace++;
            if(c=='}')brace--;
            if(brace==0) stage = end_right_code;
        }
        else if(stage == end_right_code){
            if(c==';'){
                stage = none;
            }
            if(isalpha(c)||c=='|')stage = read_right_token;
        }

        
        if(is_end==1){
            fprintf(fout, " {$$=makeNode(\"%s\"",name);
            for(int i=1;i<=cnt;i++)fprintf(fout, ",$%d", i);
            fprintf(fout, ");}");
            is_end = 0;
            cnt = 0;
        }
        if(stage == read_left&&!isspace(c)) name[name_cur++] = c;
        fputc(c, fout);
        if(is_end==2){
            fprintf(fout, " $$=makeNode(\"%s\"",name);
            for(int i=1;i<=cnt;i++)fprintf(fout, ",$%d", i);
            fprintf(fout, ");");
            is_end = 0;
            cnt = 0;
        }
        if(c=='%'&&last=='%')break;
        last = c;       
    }
    while((c=fgetc(fin))!=EOF){
        fputc(c, fout);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}