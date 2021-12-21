#include<iostream>
#include<cstring>
#include<cstdio>

char buf[1024];
int val;

int main(int argc, char** argv){
    FILE *fin = fopen(argv[1],"r");
    FILE *fout = fopen("token.cpp","w");
    fprintf(fout, "#include \"token.h\"\n");
    fprintf(fout, "std::string token_name[2048];\nint init_token(){\n");
    while(fscanf(fin,"%s",buf)!=EOF){
        if(strcmp(buf,"yytokentype"))continue;
        else break;
    }
    fscanf(fin, "%s",buf);
    while(buf[0]!='}' && fscanf(fin, "%s = %d", buf, &val)!=EOF){
        fprintf(fout, "    token_name[%d]=std::string(\"%s\");\n", val, buf);
        fscanf(fin,"%s", buf);

    }
    fprintf(fout, "return 0;\n}\n");
    fprintf(fout, "std::string getTokenName(int tok){return token_name[tok];}\n");
}