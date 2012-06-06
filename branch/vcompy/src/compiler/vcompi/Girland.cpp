/*G-нумерация преобразование в направленный граф*/
#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int Girland(COMPTOPs *ListTop,DEFGRAFs *LstGraf,int *NumTop,int *MaxLT,int *MaxLG)
{ 

    int I,J,Iroot,K,FistD,LastD,ITop,Ind;
    char Name[NAMEPR_SIZE+1]="true_one\0";
    DEFGRF GRF;



    //Просмотр параллельных фрагментов графа
    //for(I=*NumTop;I<(*MaxLT);I++)
    for(I=0;I<(*MaxLT);I++)
    {
	//strncpy(NameGr,(*(*ListTop + I)).SPName,8);
	if((*(*ListTop + I)).rankT>0 && (*(*ListTop + I)).F==0)
        {
            //Концевая вершина
            ITop=(*(*LstGraf+(*(*ListTop+I)).FirstDef)).NambTop;
            for(J=0;J<(*MaxLG);J++)
            {
                if((*(*LstGraf+J)).NambTop==I)
                {
                    //Поиск исходящей вершины
                    for(K=0;K<(*MaxLT);K++)
                    {
                        if(J>=(*(*ListTop + K)).FirstDef && J<=(*(*ListTop + K)).LastDef)
                        {Iroot=K;
                            break;}
                    }
                    FistD=(*(*ListTop+Iroot)).FirstDef; //Номер ссылки root-> концевая вершина
                    LastD=(*(*ListTop+Iroot)).LastDef;
                    Ind=LastD+1;

                    //Цикл на массиве ListTop, реализующей циклический сдвиг ссылок
                    for(K=0;K<(*MaxLT);K++)
                    {
                        if((*(*ListTop+K)).FirstDef>=LastD+1)
                            (*(*ListTop+K)).FirstDef=(*(*ListTop+K)).FirstDef+1;
                        if((*(*ListTop+K)).LastDef>=LastD+1)
                            (*(*ListTop+K)).LastDef=(*(*ListTop+K)).LastDef+1;
                    }
                    //Отбрасывание концевых дуг для параллельных вершин
                    for(K=FistD;K<=LastD;K++)
                    {
                        (*(*ListTop+(*(*LstGraf+K)).NambTop)).FirstDef=-77;
                        (*(*ListTop+(*(*LstGraf+K)).NambTop)).LastDef =-77;
                        (*(*ListTop+(*(*LstGraf+K)).NambTop)).F =1;
                    }
                    //Сдвиг массива LstGraf
                    for(K=(*MaxLG);K>=Ind;K--)
                    {
                        GRF=(*(*LstGraf+K));
                        (*(*LstGraf+(K+1)))=GRF;
                    }
                    (*(*ListTop + Iroot)).LastDef=LastD+1;
                    //Добавление новой дуги - связки корневой и концевой вершин (для синхропредиката
                    strncpy((*(*LstGraf + Ind)).Name,Name,NAMEPR_SIZE);
                    (*(*LstGraf+Ind)).NambPred=0;
                    //забить 0 номер предиката за тождественно-истинным предикатом!!!!!
                    (*(*LstGraf+Ind)).NambTop =ITop;
                    (*(*LstGraf+Ind)).ArcType_ =4;
                    break;
                }

            }
        }
    }

    return 1;
}
PROJECT_END_NAMESPACE
