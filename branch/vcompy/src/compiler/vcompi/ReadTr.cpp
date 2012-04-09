/*Чистое чтение кода вершины дерева*/
#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int ReadTr(Char200 *Cod,char *Tip,int *Nurov,int *Kurov)
{ 
//Программа читает последний уровень иерархии или записывает код по номеру места
	Char200 C,Cw;
	char Curov[6],TTip[2];
	char Np[6];
	int i,j,TNurov;
	char *p,*pi;

strncpy((char*)C,(char*)Cod,199);
p = strtok(C,". ");
strcpy((char*)Curov,p);
TNurov = atoi(Curov);
(*Nurov)=TNurov;
p = strtok(NULL, ". ");
strncpy(Tip,p,1);
for(i=0;i<(*Nurov);i++)
	{
	p = strtok(NULL, ". ");
	strcpy(Np,p);
	}
if((*Nurov)==0)				//Если нулевой уровень
	(*Kurov)=0;
else
	(*Kurov) = atoi(Np);
return(1);
	
}
PROJECT_END_NAMESPACE
