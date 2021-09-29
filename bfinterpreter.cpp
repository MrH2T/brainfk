#include<iostream>
#include<cstring>
#include<conio.h>
using namespace std;
bool equals(const char* str1, const char * str2){
	int _len=strlen(str1);
	if(_len!=(int)strlen(str2))return 0;
	for(int i=0;i<_len;i++)if(str1[i]!=str2[i])return 0;
	return 1;
}

const int MAX_BUFFER_SIZE = 1048576;

char buffer[MAX_BUFFER_SIZE];
char buf_run[MAX_BUFFER_SIZE];
int bracket[MAX_BUFFER_SIZE];
int stk[MAX_BUFFER_SIZE],top=0;
void runBFFile(){
	int buf_len = strlen(buffer);
	memset(buf_run,0,sizeof(buf_run));
	
	//prework []
	top=0;
	memset(stk,0,sizeof(stk));
	memset(bracket,0,sizeof(bracket));
	for(int r_ptr=0;r_ptr<buf_len;r_ptr++){
		if(buffer[r_ptr]=='[')stk[++top]=r_ptr;
		if(buffer[r_ptr]==']'){
			if(!top){
				printf("BF Interpreter: Brackets not matching at %d.",r_ptr);
				exit(-1);
			}
			bracket[stk[top]]=r_ptr,bracket[r_ptr]=stk[top--];
		}
	}
	if(top){
		printf("BF Interpreter: Brackets not matching.");
		exit(-1);
	}
	
	int ptr=0;
	for(int r_ptr=0;r_ptr<buf_len;){
		switch(buffer[r_ptr]){
			case '.':{
				putchar(buf_run[ptr]);
				r_ptr++;
				break;
			}
			case ',':{
				buf_run[ptr]=_getch();
				if(buf_run[ptr]=='\r')buf_run[ptr]='\n';
				r_ptr++;
				break;
			}
			case '+':{
				buf_run[ptr]++;
				r_ptr++;
				break;
			}
			case '-':{
				buf_run[ptr]--;
				r_ptr++;
				break;
			}
			case '>':{
				ptr++;
				if(ptr>=MAX_BUFFER_SIZE){
					printf("BF Interpreter: Pointer out of bound.");
					exit(-1);
				}
				r_ptr++;
				break;
			}
			case '<':{
				ptr--;
				if(ptr<0){
					printf("BF Interpreter: Pointer out of bound.");
					exit(-1);
				}
				r_ptr++;
				break;
			}
			case '[':{
				if(!buf_run[ptr])r_ptr=bracket[r_ptr]+1;
				else r_ptr++;
				break;
			}
			case ']':{
				if(buf_run[ptr])r_ptr=bracket[r_ptr]+1;
				else r_ptr++;
				break;
			}
			case ' ':break;
			case '\n':break;
			default:{
				printf("BF Interpreter: Invalid Character \'%c\' at column %d.",buffer[r_ptr],r_ptr+1);
				exit(-1);
				break;
			}
		}
	}
}

#define _add 1
#define _minus 2
#define _lsh 3
#define _rsh 4
#define _else 0
void solveNow(FILE* fl,int tbs,int op,int tm){
	if(op==_else)return;
	for(int tb=0;tb<tbs-1;tb++)fprintf(fl,"\t");
	if(op==_add){
		if(tm==1)fprintf(fl,"\tbuf[ptr]++;\n");
		else fprintf(fl,"\tbuf[ptr]+=%d;\n",tm);
	}
	
	if(op==_minus){
		if(tm==1)fprintf(fl,"\tbuf[ptr]--;\n");
		else fprintf(fl,"\tbuf[ptr]-=%d;\n",tm);
	}
	
	if(op==_lsh){
		if(tm==1)fprintf(fl,"\tptr--;\n");
		else fprintf(fl,"\tptr-=%d;\n",tm);
	}
	
	if(op==_rsh){
		if(tm==1)fprintf(fl,"\tptr++;\n");
		else fprintf(fl,"\tptr+=%d;\n",tm);
	}
}

void compileFile(const char* filename){
	int buf_len=strlen(buffer);
	
	top=0;
	memset(stk,0,sizeof(stk));
	memset(bracket,0,sizeof(bracket));
	for(int r_ptr=0;r_ptr<buf_len;r_ptr++){
		if(buffer[r_ptr]=='[')stk[++top]=r_ptr;
		if(buffer[r_ptr]==']'){
			if(!top){
				printf("BF Interpreter: Brackets not matching at %d.",r_ptr);
				exit(-1);
			}
			bracket[stk[top]]=r_ptr,bracket[r_ptr]=stk[top--];
		}
	}
	if(top){
		printf("BF Interpreter: Brackets not matching.");
		exit(-1);
	}
	
	FILE *outfile=fopen("output.cpp","w");
	fprintf(outfile,
"\
//you are not expected to understand this.\n\
#include<iostream>\n\
#include<conio.h>\n\
using namespace std;\n\
const int MAX_BUFFER_SIZE = 1048576;\n\
char buf[MAX_BUFFER_SIZE];\n\
int ptr=0;\n\
int main()\n\
{\n");
	int tabs=1;
	
	
	int lastop=_else,tms=0;
	
	for(int r_ptr=0;r_ptr<buf_len;r_ptr++){
		switch(buffer[r_ptr]){
			case '.':{
				if(lastop!=_else)solveNow(outfile,tabs,lastop,tms),lastop=_else,tms=1;
				for(int tb=0;tb<tabs-1;tb++)fprintf(outfile,"\t");
				fprintf(outfile,"\tputchar(buf[ptr]);\n");
				break;
			}
			case ',':{
				if(lastop!=_else)solveNow(outfile,tabs,lastop,tms),lastop=_else,tms=1;
				for(int tb=0;tb<tabs-1;tb++)fprintf(outfile,"\t");
				fprintf(outfile,"\tbuf[ptr]=_getch();\n");
				break;
			}
			case '+':{
				if(lastop!=_add)solveNow(outfile,tabs,lastop,tms),lastop=_add,tms=1;
				else tms++;
				break;
			}
			case '-':{
				if(lastop!=_minus)solveNow(outfile,tabs,lastop,tms),lastop=_minus,tms=1;
				else tms++;
				break;
			}
			case '>':{
				if(lastop!=_rsh)solveNow(outfile,tabs,lastop,tms),lastop=_rsh,tms=1;
				else tms++;
				break;
			}
			case '<':{
				if(lastop!=_lsh)solveNow(outfile,tabs,lastop,tms),lastop=_lsh,tms=1;
				else tms++;
				break;
			}
			case '[':{
				if(lastop!=_else)solveNow(outfile,tabs,lastop,tms),lastop=_else,tms=1;
				for(int tb=0;tb<tabs-1;tb++)fprintf(outfile,"\t");
				fprintf(outfile,"\twhile(buf[ptr]){\n");
				tabs++;
				break;
			}
			case ']':{
				if(lastop!=_else)solveNow(outfile,tabs,lastop,tms),lastop=_else,tms=1;
				for(int tb=0;tb<tabs-1;tb++)fprintf(outfile,"\t");
				fprintf(outfile,"}\n");
				tabs--;
				break;
			}
			case ' ':break;
			case '\n':break;
			default:{
				printf("BF Interpreter: Invalid Character \'%c\' at column %d.",buffer[r_ptr],r_ptr+1);
				exit(-1);
				break;
			}
		}
	}
	solveNow(outfile,tabs,lastop,tms);
	fprintf(outfile,"}\n");
	fclose(outfile);
}

int main(int args, char** argc){
	if(args!=3){
		printf("BF Interpreter: Incorrect format.");
		return -1;
	}
	
	if(equals(argc[1],"-r")){
		FILE* file=fopen(argc[2],"r");
		if(file==NULL){
			printf("BF Interpreter: File not found.");
			return -1;
		}
		fread(buffer,sizeof(char),MAX_BUFFER_SIZE,file);
		fclose(file);
		runBFFile();
		return 0;
	}
	else if(equals(argc[1],"-c")){
		FILE* file=fopen(argc[2],"r");
		if(file==NULL){
			printf("BF Interpreter: File not found.");
			return -1;
		}
		fread(buffer,sizeof(char),MAX_BUFFER_SIZE,file);
		fclose(file);
		compileFile(argc[2]);
		return 0;
	}
	else{
		printf("BF Interpreter: Illegal switch \"%s\".",argc[1]);
		return -1;
	}
	return 0;
}
