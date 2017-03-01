#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include <sys/stat.h>
#include<unistd.h>
#include<string.h>
//#include"vin.h"

#define _OPTAB_ "optab.txt"
#define _SYMTAB_ "symtab.txt"
int optcount,symcount;
struct optab
{
	char opcode[10];
	int val;
}op[100];

struct symtab
{
	char symbol[10];
	int val;
}sym[100];

char* vgetfile(char* p,long int* sizet)
{
	FILE *a;
	char *dat;
	struct stat s;
	int filen;
	long int size;
	a=fopen(p,"r");
	if(!a){
		printf("\nError:Unable to open input file");
		return 0;
	}
	filen=fileno(a);
  	if (fstat(filen, &s) == -1) {
  	        printf("\nError:Unable to get file size from input file");
        	return 0;
   		}
	size=s.st_size; 
	*sizet=size;
	dat=(char*)malloc(size);  
	read(fileno(a),dat,size);
	return dat;
	}
int skipcomment(char* p){
	int k=strlen(p);
	for(int i=0;i<k;i++)
		{
		if(p[i]==46){
		p[i]='\0';
		return 1;}
		}
	return 0;
	}
int isnulls(char* p){
	for(int i=0;i<strlen(p);i++)
		{
		if((p[i]!=' ')&&(p[i]!='\t')&&(p[i]!='\n')&&(p[i]!='\r'))
			return 1;
		}
	return 0;
	}

int vget(char* line,char* label,char* opc,char* opr)
{
	int i=0,j=0;
	label[0]='\0';
	opc[0]='\0';
	opr[0]='\0';

	for(i=0,j=0;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
		label[j]=line[i];
	i++;
	for(;((i<=strlen(line))&&(line[i]==' '));i++);
	label[j]='\0';
	for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
		opc[j]=line[i];
	i++;
	for(;((i<=strlen(line))&&(line[i]==' '));i++);
	opc[j]='\0';
	for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
		opr[j]=line[i];
	opr[j]='\0';
}

void vgetline(char* dat,int tsize,char* to,int sis,int lineno){
	int i,st,cnt=0;
	for(st=0;((cnt<lineno)&&(st<tsize));st++)
		if(dat[st]=='\n'||dat[st]=='\r')cnt++;

	for(i=0;((i<sis-1)&&(st+i<tsize));i++){
		if(dat[st+i]=='\n'||dat[st+i]=='\r')break;
		to[i]=dat[st+i];
		}
	to[i]='\0';
}


void loadop(){
	FILE *p=fopen(_OPTAB_,"r");
	int opv;
	char li[50],opc[10],opvc[10],opp[10];
	int i=0;
	if(!p)printf("\nUnable to open file");
	do{
		fgets(li,100,p);
		vget(li,opc,opvc,opp);
		strcpy(op[i].opcode,opc);
		sscanf(opvc,"%x",&op[i].val);
		//printf("\n%s + %d",op[i].opcode,op[i].val);
		//scanf("%d",&j);
		i++;
	}while(!feof(p));
	optcount=i;
	/*for(i=0;i<optcount;i++)
	printf("\n%s + %d",op[i].opcode,op[i].val);*/

}

int searchop(char* o){

	for(int i=0;i<optcount;i++)
		if(strcmp(op[i].opcode,o)==0){
			return op[i].val;
		}

	return -1;
}
	int rstart(const char* filein){
	char li[100],opc[100],opr[100],label[100];
	int r=0;
	FILE *p=fopen(filein,"r");
	fgets(li,100,p);
	if(feof(p)){
		fclose(p);
		return r;
		}
	vget(li,label,opc,opr);
	if(strcmp(opc,"START")==0){
	
	sscanf(opr,"%x",&r);

	}
	fclose(p);
	return r;
	}
int traverse(int start,const char* filein,const char* fileout){
	FILE *p=fopen(filein,"r");
	FILE *q=fopen(fileout,"w");
	start=rstart(filein);
	
	int count =start,flag=0,temp=0,cmnt=0;
	char label[100],opc[100],opr[100],li[100],startadd[10];
	sprintf(startadd,"%06x",start);
	if(!p){printf("\nFile not found");return -1;}
	if(!q){printf("\nFile not found");return -1;}

	do{
		fgets(li,100,p);
		cmnt=skipcomment(li);
		//printf("%s",li);
		if(!isnulls(li))continue;
		if(feof(p))break;
		vget(li,label,opc,opr);
		//printf("\n%s * %s * %s",label,opc,opr);
		if(strcmp(opc,"END")==0){
		strcpy(opr,startadd);
		}
		fprintf(q,"%06x %10s %10s\n",count,opc,opr);
		if(strcmp(label,"")!=0)
			{
			strcpy(sym[symcount].symbol,label);
			sym[symcount++].val=count;
			}
		if(searchop(opc)>=0){
			count+=3;
			}
		else if(strcmp(opc,"RESW")==0){
		
		
			sscanf(opr,"%d",&temp);
			count+=3*temp;
			}
		else if(strcmp(opc,"RESB")==0){
		
		
			sscanf(opr,"%d",&temp);
			count+=1*temp;
			}
		else if(strcmp(opc,"BYTE")==0){
			
			if(opr[0]=='C'){
			temp=strlen(opr)-3;
			if((temp%3)>0)temp+=3-(temp%3);
			}
			
			else if(opr[0]=='X'){
			temp=strlen(opr)-3;
			temp=temp/2+temp%2;
			
			}
			count+=temp;
			}

		else if(strcmp(opc,"WORD")==0){
			count+=3;
			}
		else if(strcmp(opc,"START")==0){                               ///ignore
				printf("\nStart found");
			}
		else if(strcmp(opc,"END")==0){					//ignore
			printf("\nEnd found");
			}
		else {
		printf("invalid opcode %s",opc);
		return -1;}

		//printf("%s %s %s",label,opc,opr);
		//scanf("%d",&j);

		}while(!feof(p));
	fclose(p);
	fclose(q);
}


void prnsymtab(){
	printf("\nSYMTAB\n");
	for(int i=0;i<symcount;i++)
		printf("%s * %06x\n",sym[i].symbol,sym[i].val);
	}

void writesymtab(const char* fileout){
	//printf("\nSYMTAB\n");
	FILE *p=fopen(fileout,"w");

	for(int i=0;i<symcount;i++)
		fprintf(p,"%-10s %06x\n",sym[i].symbol,sym[i].val);
		fclose(p);
	}


int main(int argn,char** args)
{
	int q,j;

	long int size;
	printf("Loading optab");
	loadop();
	printf("\nloading complete");
	j=traverse(0000,"ttt.txt","inter.txt");
	
	writesymtab(_SYMTAB_);
	scanf("%d",&q);
	return 0;
}
