#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include <sys/stat.h>
#include<unistd.h>
#include<string.h>


#define _OPTAB_ "optab.txt"
#define _SYMTAB_ "symtab.txt"

int optcount,symcount;
int searchsymtab(char*);
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
int makeob(int opcode,int ind,int oprend)
{
int obj=0;
obj=opcode;
obj=obj<<1;
obj=obj+ind;
obj=obj<<15;
obj=obj+oprend;
//printf("%d\n",obj);
return obj;
}

int getbyte(char *p){
int d;
sscanf(p,"%x",&d);

return d;

}

int vget(char* line,char* label,char* opc,char* opr)
{int i,j=0;
label[0]='\0';
opc[0]='\0';
opr[0]='\0';
for(i=0,j=0;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
label[j]=line[i];
i++;
label[j]='\0';
for(;((i<=strlen(line))&&(line[i]==' '));i++);
for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
opc[j]=line[i];
i++;
opc[j]='\0';
for(j=0;(i<=strlen(line)&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
opr[j]=line[i];
opr[j]='\0';
i++;

}

void sepcoma(char *str,char* r,char*p)
{
int i=0,j=0;
r[0]='\0';
p[0]='\0';
for(;(str[i]!='\0'&&str[i]!=',');i++)
{
r[j]=str[i];
j++;
}
r[j]='\0';
if(str[i]!='\0'){
i++;

j=0;
for(;(str[i]!='\0');i++)
{
p[j]=str[i];
j++;
}
p[j]='\0';
}
}



int vget2(char* line,char* loc,char* label,char* opc,char* opr)
{int i,j=0;
label[0]='\0';
opc[0]='\0';
opr[0]='\0';
opr[1]='\0';
for(;((i<=strlen(line))&&(line[i]==' '));i++);
for(i=0,j=0;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
loc[j]=line[i];
i++;
loc[j]='\0';
for(;((i<=strlen(line))&&(line[i]==' '));i++);
for(j=0;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]!=' '));i++,j++)
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
for(;((i<=strlen(line))&&(line[i]!='\n')&&(line[i]!='\r')&&(line[i]==' '));i++);
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
do{
fgets(li,100,p);
vget(li,opc,opvc,opp);
strcpy(op[i].opcode,opc);
sscanf(opvc,"%x",&op[i].val);
i++;
}while(!feof(p));
optcount=i;

}

int searchop(char* o){

for(int i=0;i<optcount;i++)
if(strcmp(op[i].opcode,o)==0){
return op[i].val;
}

return -1;
}
int rstart2(const char* filein){
char li[100],opc[100],opr[100],label[100],loc[100];
int r=0;
FILE *p=fopen(filein,"r");
fgets(li,100,p);
if(feof(p)){fclose(p);return r;}
vget2(li,loc,label,opc,opr);
if(strcmp(opc,"START")==0){

sscanf(opr,"%d",&r);

}
fclose(p);
return r;
}

int rend2(const char* filein){
char li[100],opc[100],opr[100],label[100],loc[100];
int r=0,fl=0;
FILE *p=fopen(filein,"r");
if(!p)printf("\nUnable to open file");
printf("\nLooking for end");
do{
fgets(li,100,p);
vget2(li,loc,label,opc,opr);
if(strcmp(label,"END")==0){
printf("\nEnd found at %s\n",loc);
sscanf(loc,"%x",&r);
fl=1;
}

}while(!fl);



fclose(p);
return r;
}



int secondpass(const char* interfile,const char* objfile)
{

FILE *p=fopen(interfile,"r");
FILE *r=fopen(objfile,"w+r");
int start=rstart2(interfile),obj;
int end=rend2(interfile);
int count=0,flag=0,temp=0,opcode,operand,ind,line=0,tadd=0,tsize=0,wordb=0,fl=0,jj;
char label[100]="PROG",opc[100],opr[100],li[100],loc[15],nonindex[10],index[10],tline[100]="",objs[10],byt[3],temps[20];
printf("%s","Second Pass started");
if(r==NULL)printf("Unable to create a file");
do{
obj=0;
fgets(li,100,p);
if(feof(p))break;
vget2(li,loc,opc,opr,temps);
sscanf(loc,"%x",&count);
if(strcmp(opc,"START")==0){    
start=count;                     //ignore
fprintf(r,"H^%-6s^%06x^%06x\n",label,count,end-start);                    //label does not contain real program name
printf("\nStart found");
}

else if(strcmp(opc,"RESW")==0){
if(tsize>0)fprintf(r,"T^%06x^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;
}
else if(strcmp(opc,"RESB")==0){
if(tsize>0)fprintf(r,"T^%06x^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;

}
else if(strcmp(opc,"BYTE")==0){
if(opr[0]=='C'){
			
			fl=0;
		for(int ii=2;opr[ii]!='\''&&fl!=1;){
		wordb=0;
		for(jj=0;jj<3&&fl!=1;jj++,ii++){
		if(opr[ii]=='\'')fl=1;
		wordb=wordb<<8;
		if(fl!=1)wordb+=opr[ii];
		
		}
		count+=jj;
if(tsize>27){
fprintf(r,"T^%06x^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;
}
if(tsize==0)
{
tadd=count-jj;
}
if(jj==1)sprintf(objs,"%02x",wordb);    
else if(jj==2)sprintf(objs,"%04x",wordb); 
else sprintf(objs,"%06x",wordb);
strcat(tline,"^");
strcat(tline,objs);
tsize+=jj;
		
		
		}	
			}
			
			else if(opr[0]=='X'){
			temp=strlen(opr)-3;
			temp=temp/2+temp%2;
			
			fl=0;
		for(int ii=2;opr[ii]!='\''&&fl!=1;){
		wordb=0;
		for(jj=0;jj<3&&fl==0;jj++){
		
		byt[0]=0;
		byt[1]=0;
		byt[2]=0;
		for(int pp=0;opr[ii]!='\''&&pp<2;pp++){
		byt[pp]=opr[ii];
		ii++;                                       //byte to eord
				}  
				  
				                                        //herer
		if(opr[ii]=='\'')fl=1;
		wordb=wordb<<8;
		wordb+=getbyte(byt);
		
		}
		
		count+=jj;
		
		if(tsize>27){                                            //TODO edit here further
fprintf(r,"T^%06x^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;
}
if(tsize==0)
{
tadd=count-jj;
}

if(jj==1)sprintf(objs,"%02x",wordb);    
else if(jj==2)sprintf(objs,"%04x",wordb);                                     //does not occupy all three bytes 
else sprintf(objs,"%06x",wordb);
strcat(tline,"^");
strcat(tline,objs);
tsize+=jj;
		
		
		}
			
			
			}

}

else if(strcmp(opc,"WORD")==0){

wordb=0;
sscanf(opr,"%d",&wordb);

if(tsize>27){
fprintf(r,"T^%06x^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;
}
if(tsize==0)
{
tadd=count;
}
sprintf(objs,"%06x",wordb);
strcat(tline,"^");
strcat(tline,objs);
tsize+=3;


}

else if(strcmp(opc,"END")==0){				//ignore
if(tsize>0)fprintf(r,"T^%06x^%02x%s\n",tadd,tsize,tline);
fprintf(r,"E^%06x",start);
printf("\nEnd  found");
}
else if((opcode=searchop(opc))>=0){


if(strcmp(opr,"")!=0){
sepcoma(opr,nonindex,index);
if((operand=searchsymtab(nonindex))>0){

}
else {
printf("%s not found insymtab",nonindex);
return -2;
}
}
else {
operand=0;
}
if(strcmp(index,"")==0)ind=0;       //no index register
else {ind=1;
}
obj=makeob(opcode,ind,operand);
objs[0]='\0';
sprintf(objs,"%06x",obj);
//printf("   1:%s\n",objs);
if(tsize>27){
fprintf(r,"T^%06x^%02x%s\n",tadd,tsize,tline);
tsize=0;
tline[0]='\0';
tadd=0;
}
if(tsize==0)
{
tadd=count;
}
strcat(tline,"^");
strcat(tline,objs);
tsize+=3;

}
else return -1;


}while((strcmp("END",opc)!=0));
fclose(p);
fclose(r);
}

void prnsymtab(){
printf("\nSYMTAB\n");
for(int i=0;i<symcount;i++)
printf("%s * %06x\n",sym[i].symbol,sym[i].val);
}

int searchsymtab(char *p){
for(int i=0;i<symcount;i++){
if(strcmp(p,sym[i].symbol)==0){

return sym[i].val;
}
}
return -1;
}



void loadsymtab(const char* filein){
FILE *p=fopen(filein,"r");
char li[300],symboln[100],symboladd[100],s[100];
int i=0;
if(p==NULL) printf("cant open file");
fgets(li,300,p);
while(!feof(p)){

vget(li,symboln,symboladd,s);
sscanf(symboladd,"%x",&sym[i].val);
strcpy(sym[i].symbol,symboln);
i++;
fgets(li,100,p);
}
symcount=i;
}


int main()

{
int d;
loadsymtab(_SYMTAB_);
printf("%s%d","loading optab",'.');
loadop();
printf("%s","\nloaded optab");
printf("\n");
secondpass("inter.txt","obj.txt");
printf("\ncompleted");
scanf(" %d",&d);
sleep(5);
}
