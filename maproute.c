#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Graph data structure

#define MaxVertexNum 3950
#define INFINITY 65535
#define NotAVertex -1

typedef int Vertex;         
typedef double WeightType;        
typedef int DataType;
typedef double LonValue;
typedef double LatValue;        

typedef struct ENode{
   	Vertex V1, V2;      
	WeightType Weight;  
}ENode,*PtrToENode;

typedef PtrToENode Edge;
typedef struct AdjVNode *PtrToAdjVNode; 

struct AdjVNode{
   	Vertex AdjV;        
	WeightType Weight;  
	PtrToAdjVNode Next;    
};

typedef struct Vnode{
	PtrToAdjVNode FirstEdge;
	DataType Data;
	LonValue lon;
	LatValue lat;
	Vertex newid;            
}AdjList[MaxVertexNum];    

typedef struct GNode{  
	int Nv;     
    	int Ne;     
    	AdjList G;  
}GNode,*PtrToGNode;

typedef PtrToGNode LGraph; 

struct LinkInfo{
	int node1;
	int node2;
	double length;
}LinkInfo;

typedef struct LinkInfo ids[3000];

struct NewID{
	int newid;
	int oldid;
	double lon;
	double lat;
}NewID;

typedef struct NewID new[3950];

struct TableEntry{
	int Known;
	double Dist;
	int Path;
};

typedef struct TableEntry Table[3950];

// Declare the functions

void ShowMenu();
void Exit();
void Info();
char *find_next_string(char *source, char *x);
int split_the_string_int(char source[], char x[], char y[]);
double split_the_string_double(char source[], char x[], char y[]);
int find_the_first_place(char source[],char ch[]);
int find_the_second_place(char source[],char ch[]);
int take_int(char source[], int index1, int index2);
int take_next_int(char source[], int index, char s[]);
LGraph CreateGraph(int Nodenum);
void read_file();
void InsertEdge(LGraph Graph, Edge E);
void load(ids s);
void oldnew(new n);
int findid(new n,int oldid);
double get_lon(new n,int id);
double get_lat(new n,int id);
LGraph BuildGraph(new n);
void DrawMap(new n,ids s,LGraph Graph);
void InitTable(Vertex Start,LGraph Graph,Table T);
Vertex FindMin(LGraph Graph,Table T);
void Dijkstra(LGraph Graph,Table T);
void PrintOut(new n,Vertex E,Table T,FILE *fp);
void ViewGraph(LGraph Graph);

// Main function

int main(int argc,char *argv[]){
	LGraph Graph;
	Table T;
	ids s;
	new n;
	read_file();
	oldnew(n);
	load(s);
	Graph = BuildGraph(n);
	DrawMap(n,s,Graph);
	int choice = 0;
	ShowMenu();
	while(1){
		printf("Please select(From 1 to 3): ");
		scanf("%i",&choice);
		if(choice == 1){
			ViewGraph(Graph);
			Info();
		}
		else if(choice == 2){
			int start,end;
			printf("\nPlease input the start point (0-2118): ");
			scanf("%i",&start);
			if(start >= 0 && start <= 2118){
				InitTable(start,Graph,T);
				Dijkstra(Graph,T);
				printf("Please input the end point (0-2118): ");
				scanf("%i",&end);
				if(end >= 0 && end <= 2118){
					FILE *fp;
					fp = fopen("short.out","w");
					PrintOut(n,end,T,fp);
					printf("\nThe total length is %lf.\n",T[end].Dist);
					fclose(fp);
				}
				else
					printf("End ERROR!!!\n");
			}
			else
				printf("Start ERROR!!!\n");
		}
		else if(choice == 3){
			Exit();
			break;
		}
		else 
			printf("Please input again!\n");
	}
	return 0;
}

// Contents of other functions

void ShowMenu(){
	printf("\n\n");
	printf("=========Welcome to Leeds University=========\n");
	printf("||      1.View the whole university        ||\n");
	printf("||     2.Find GPS system to find road      ||\n");
	printf("||        3.Exit the system                ||\n");
	printf("=============================================\n");
	printf("\n\n");
}

void Exit(){
	printf("\nThank you for using the system!\n");
	printf("Press 'Enter' to exit...\n\n");
	getchar();
	exit(0);
}

void Info(){
	printf("\nPlease input 'load 'map.plt'' in Gnuplot.\n\n");
}

char *find_next_string(char *source, char *x){
	char *result;
	result = (char *)malloc(sizeof(char)*1000000);
	char *str = strstr(source,x);
	int length = strlen(x);
	int j = 0;
	for(int i = length; str[i] != '\0'; i++){
		result[j] = str[i];
		j++;
	}
	result[j] = '\0';
	return result;
}

int split_the_string_int(char source[], char x[], char y[]){
	char *result,*s0;
	result = (char *)malloc(sizeof(char)*1000000);
	s0 = (char *)malloc(sizeof(char)*1000000);
	s0 = find_next_string(source,x);
	int L0 = strlen(s0);
	char *newc = strstr(source,y);
	int L1 = strlen(newc);
	int index = L0 - L1;
	s0[index] = '\0';
	strcpy(result,s0);
	free(s0);
	return atoi(result);
}

double split_the_string_double(char source[], char x[], char y[]){
	char *result,*s0;
	result = (char *)malloc(sizeof(char)*1000000);
	s0 = (char *)malloc(sizeof(char)*1000000);
	s0 = find_next_string(source,x);
	int L0 = strlen(s0);
	char *newc = strstr(source,y);
	int L1 = strlen(newc);
	int index = L0 - L1;
	s0[index] = '\0';
	strcpy(result,s0);
	free(s0);
	return atof(result);
}

int find_the_first_place(char source[],char ch[]){
	int l = strlen(source);
	int position = 0;
	for(int i = 0; i < l; i++){
		if(ch[0] == source[i]){
			position = i;
			break;
		}
	}
	return position;
}

int find_the_second_place(char source[],char ch[]){
	int l = find_the_first_place(source,ch);
	int position = 0;
	for(int i = l + 1; source[i] != '\0'; i++){
		if(ch[0] == source[i]){
			position = i ;
			break;
		}
	}
	return position;
}

int take_int(char source[], int index1, int index2){
	char *result;
	result = (char *)malloc(sizeof(char)*1000000);
	int k = 0;
	for(int i = index1 + 1; i < index2; i++){
		result[k] = source[i];
		k++;
	}
	result[k] = '\0';
	return atoi(result);
}

int take_next_int(char source[], int index, char s[]){
	int k = 0;
	int index0 = 0;
	for(int i = index + 1; source[i] != '\0'; i++){
		if(s[0] == source[i]){
			index0 = i;
			break;
		}
	}
	int r = take_int(source,index,index0);
	return r;
}

LGraph CreateGraph(int Nodenum){ 
	Vertex V;
	LGraph Graph;
     	Graph = (LGraph)malloc(sizeof(struct GNode)); 
	Graph->Nv = Nodenum;
	Graph->Ne = 0;
        for (V=0; V<Graph->Nv; V++)
       		Graph->G[V].FirstEdge = NULL;
        return Graph; 
}

void read_file(){
	int i = 0;
	FILE *fp1,*fp2,*fp3;
	fp1 = fopen("Final_Map.map","r");
	fp2 = fopen("nodeinfo.txt","w");
	fp3 = fopen("linkinfo.txt","w");
	char buf[200];
	while(fgets(buf,sizeof(buf),fp1)){
        	if(buf[0] == '<' && buf[1] == 'n' && buf[2] == 'o'){
			double b = split_the_string_double(buf,"lat="," lon=");
			double c = split_the_string_double(buf,"lon="," /node>");
			int a = split_the_string_int(buf,"id="," lat=");
			fprintf(fp2,"%i     %i    %lf    %lf \n",i,a,c,b);
			i++;
		}
		if(buf[0] == '<' && buf[1] == 'l' && buf[2] == 'i'){
			double l = split_the_string_double(buf,"length="," veg=");
			int f0 = find_the_first_place(buf,"o");
			int f1 = find_the_second_place(buf,"o");
			int n1 = take_int(buf,f0+3,f1-2);
			int n2 = take_next_int(buf,f1+3," way=");
			fprintf(fp3,"%i     %i     %lf\n",n1,n2,l);
		}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

void InsertEdge(LGraph Graph, Edge E){
	PtrToAdjVNode NewNode;
     
    	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    	NewNode->AdjV = E->V2;
    	NewNode->Weight = E->Weight;
    	
    	NewNode->Next = Graph->G[E->V1].FirstEdge;
    	Graph->G[E->V1].FirstEdge = NewNode;
         
    	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
    	NewNode->AdjV = E->V1;
    	NewNode->Weight = E->Weight;
    	
    	NewNode->Next = Graph->G[E->V2].FirstEdge;
    	Graph->G[E->V2].FirstEdge = NewNode;
}

void load(ids s){
	int i = 0;
	FILE *fp;
	fp = fopen("linkinfo.txt","r");
	while( i < 2344 ){
		fscanf(fp,"%i%i%lf",&s[i].node1,&s[i].node2,&s[i].length);
		i++;
	}
	fclose(fp);
}

void oldnew(new n){
	int i = 0;
	FILE *fp;
	fp = fopen("nodeinfo.txt","r");
	while( i < 3941 ){
		fscanf(fp,"%i%i%lf%lf",&n[i].newid,&n[i].oldid,&n[i].lon,&n[i].lat);
		i++;
	}
}

int findid(new n,int oldid){
	int newid;
	for(int i = 0; i < 3941; i++){
		if( oldid == n[i].oldid ){
			newid = n[i].newid;
			break;
		}
	}
	return newid;
}

double get_lon(new n,int id){
	int i = 0;
	double lon;
	for(i = 0; i < 3941; i++){
		if( id == n[i].oldid || id == n[i].newid){
			lon = n[i].lon;
			break;
		}
	}
	return lon;
}

double get_lat(new n,int id){
	int i = 0;
	double lat;
	for(i = 0; i < 3941; i++){
		if( id == n[i].oldid || id == n[i].newid){
			lat = n[i].lat;
			break;
		}
	}
	return lat;
}

LGraph BuildGraph(new n){
	LGraph Graph;
	Edge E;
	Vertex V;
    	int Nv, i;
    	Nv = 3941;
     	Graph = CreateGraph(Nv);
    	FILE *fp1,*fp2; 
     	fp1 = fopen("nodeinfo.txt","r");
	fp2 = fopen("linkinfo.txt","r");
	Graph->Ne = 2344;
    	if ( Graph->Ne != 0 ) { 
        	E = (Edge)malloc(sizeof(struct ENode));  
        	for (i = 0; i < Graph->Ne; i ++) {
        	int a,b;
        	double c;
            		fscanf(fp2,"%i%i%lf",&a,&b,&c);
            		E->V1 = findid(n,a);
            		E->V2 = findid(n,b);
            		E->Weight = c;
            		InsertEdge(Graph,E);
        	}
    	} 
 
    	for (V = 0; V < Graph->Nv; V++) 
 		fscanf(fp1,"%i%i%lf%lf",&(Graph->G[V].newid),&(Graph->G[V].Data),&(Graph->G[V].lon),&(Graph->G[V].lat));
 	fclose(fp1);
 	fclose(fp2);
 	return Graph;
}

void DrawMap(new n,ids s,LGraph Graph){
	FILE *fp;
	int i = 0;
	fp = fopen("map.out","w");
	for(i = 0; i < 2344; i++){
		int m = s[i].node1;
		double m0 = get_lon(n,m);
		double m1 = get_lat(n,m);
		int x = s[i].node2;
		double n0 = get_lon(n,x);
		double n1 = get_lat(n,x);
		fprintf(fp,"%lf %lf \n",m0,m1);
		fprintf(fp,"%lf %lf \n\n\n\n",n0,n1);
	}
	fclose(fp);
}

void InitTable(Vertex Start,LGraph Graph,Table T){
	int i;
	for(i = 0; i < Graph->Nv; i++){
		T[i].Known = 0;
		T[i].Dist = INFINITY;
		T[i].Path = NotAVertex;
	}
	T[Start].Dist = 0;
}

Vertex FindMin(LGraph Graph,Table T){
	Vertex MinV,V;
	double minDist = INFINITY;
	for(V = 0; V < Graph->Nv; V++){
		if(T[V].Known == 0 && T[V].Dist < minDist){
			minDist = T[V].Dist;
			MinV = V;
		}
	}
	if(minDist < INFINITY)
		return MinV;
	else
		return NotAVertex;
}

void Dijkstra(LGraph Graph,Table T){
	Vertex V;
	PtrToAdjVNode Ptr;
	while(1){
		V = FindMin(Graph,T);
		if(V == NotAVertex)
			break;
		T[V].Known = 1;
		for(Ptr = Graph->G[V].FirstEdge; Ptr; Ptr = Ptr->Next){
			if(T[Ptr->AdjV].Known == 0 && T[V].Dist + Ptr->Weight < T[Ptr->AdjV].Dist){
				T[Ptr->AdjV].Dist = T[V].Dist + Ptr->Weight;
				T[Ptr->AdjV].Path = V;
			}
		}
	}
}

void PrintOut(new n,Vertex E,Table T,FILE *fp){
	if(T[E].Path != NotAVertex){
		PrintOut(n,T[E].Path,T,fp);
		printf("-> ");
	}
	printf("%i ",E);
	fprintf(fp,"%lf %lf \n",get_lon(n,E),get_lat(n,E));
}

void ViewGraph(LGraph Graph){
	PtrToAdjVNode p;
	printf("View the adjacent list: \n");
	for(int i = 0; i < 2119; i++){
		p = Graph->G[i].FirstEdge;
		while(p){
			printf(" ( %i -> %i ) ",Graph->G[i].newid,Graph->G[p->AdjV].newid);
			p = p->Next;
		}
		printf("\n");
	}
}
