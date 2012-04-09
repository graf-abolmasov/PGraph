#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int BcodeTr(Char200 *Cod,char *WR,char *Tip,int *Nurov,int *Kurov)
{ 
    //Программа читает последний уровень иерархии или записывает код по номеру места
    Char200 C,Cw;
    char Curov[6],TTip[2];
    char Np[6];
    int i,j,TNurov;
    char *p,*pi;

    strncpy((char*)C,(char*)Cod,199);
    if((*WR)=='R')
    {
	p = strtok(C,". ");
	strcpy((char*)Curov,p);
	TNurov = atoi(Curov);
	if((*Nurov)>TNurov)
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
	//(*WR)='R';
	return(1);
    }
    //if (strcmp(WR,"W")==0)
    if ((*WR)=='W')
    {
	p = strtok(C,". ");
	strcpy((char*)Curov,p);
	TNurov = atoi(Curov);			//определение количества уровней кода
	if(TNurov==(*Nurov))			//Если количество уровней совпадают в коде и параметре
        {
            strcpy(Cw,Curov);			// то сохранется общее значение
            strncat(Cw,".",1);
        }
	else
        {sprintf(Np,"%d",(*Nurov));	// иначе записывается новое значение уровня
            strcpy(Cw,Np);
            strncat(Cw,".",1);
        }
	p = strtok(NULL, ". ");
	strcpy(TTip,p);

	if (strcmp(TTip,Tip)==0)
            strncat(Cw,TTip,1);
	else
            strncat(Cw,Tip,1);


	if (TNurov==(*Nurov))				//Если количество уровней совпадают в коде и параметре
        {
            if((*Nurov)==0)
                j=0;
            else
            {strncat(Cw,".",1);
                for(i=0;i<(*Nurov)-1;i++)
                {
                    p = strtok(NULL, ". ");		// то разметка производится в соответствии с кодом
                    strcat(Cw,p);
                    strncat(Cw,".",1);
                }

                sprintf(Np,"%d",(*Kurov));
                strcat(Cw,Np);
            }
        }
	else
        {
            if((*Nurov)!=0)
            {
                if((*Nurov)==1)					// иначе записывается новое значение уровня добавляется
                {
                    strncat(Cw,".",1);
                    sprintf(Np,"%d",(*Kurov));
                    strcat(Cw,Np);
                    j=0;
                }
                else
                {
                    strncat(Cw,".",1);
                    for(i=0;i<(*Nurov)-1;i++)
                    {
                        p = strtok(NULL, ". ");
                        strcat(Cw,p);
                        strncat(Cw,".",1);
                    }
                    sprintf(Np,"%d",(*Kurov));
                    strcat(Cw,Np);
                    //strncat(Cw,".",1);
                }
            }
        }
	strncpy((char*)Cod,(char*)Cw,199);
	return(1);
    }

    printf("Неправильный код использования функции BcodeTr\n");
    return 1;
}
PROJECT_END_NAMESPACE

/*

int main()
{
 static COMPTOPs ListTop;
 static DEFGRAFs LstGraf;
 int MaxGH=23,MaxLT=17;
 static Char200 st1="3.V.1.2.3";
 static char Tip='V';
 static char WR='W';
 int Nurov=4,Kurov=34;
 int nl;
 char *p;


   BcodeTr(&st1,&WR,&Tip,&Nurov,&Kurov);
//	mdeftest(&ListTop,&LstGraf);
 return 0;
}
*/
