/*Наполнение  структур параллельного графа тестовым примером*/
//#include "stdafx.h"
#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE

int deftest(COMPTOPs *ListTop,DEFGRAFs *LstGraf)
{
// int i,IndGH,NTgraf=33,NTtop=30;
// char Name2[80];
// char NameAKT[80];
////Вариант 2
//// DEFGRF TGRAF[27]={{"gaaaaaae",0,1,2,0,"0.I"},{"gaaaaaae",0,2,2,0,"0.I"},{"gaaaaaae",0,3,2,0,"0.I"},{"gaaaaaae",0,5,1,0,"0.I"},{"gaaaaaae",0,4,1,0,"0.I"},{"gaaaaaae",2,7,1,0,"0.I"},{"gaaaaaae",2,6,1,0,"0.I"},{"gaaaaaae",0,8,2,0,"0.I"},{"gaaaaaae",0,9,2,0,"0.I"},{"gaaaaaae",1,10,1,0,"0.I"},{"gaaaaaae",1,10,1,0,"0.I"},{"gaaaaaae",2,7,1,0,"0.I"},{"gaaaaaae",0,11,2,0,"0.I"},{"gaaaaaae",0,12,2,0,"0.I"},{"gaaaaaae",1,13,3,0,"0.I"},{"gaaaaaae",1,13,3,0,"0.I"},{"gaaaaaae",1,15,3,0,"0.I"},{"gaaaaaae",1,14,3,0,"0.I"},{"gaaaaaae",1,14,3,0,"0.I"},{"gaaaaaae",1,15,3,0,"0.I"},{"gaaaaaae",1,15,3,0,"0.I"},{"gaaaaaae",2,16,1,0,"0.I"},{"gaaaaaae",2,17,1,0,"0.I"},{"gaaaaaae",2,18,2,0,"0.I"},{"gaaaaaae",2,19,2,0,"0.I"},{"gaaaaaae",2,20,3,0,"0.I"},{"gaaaaaae",2,20,3,0,"0.I"}};
////Вариант 3`
////DEFGRF TGRAF[27]={{"gaaaaaae",0,1,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,2,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,3,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,5,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,4,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,6,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,8,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,9,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,10,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,10,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,11,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,12,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,13,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,13,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,14,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,14,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,16,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,17,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,18,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,19,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"}};
////Вариант 6
////DEFGRF TGRAF[27]={{"gaaaaaae",0,1,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,2,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,3,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,5,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,4,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,6,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,8,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,9,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,10,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,10,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,11,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,12,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,13,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,13,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,14,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,14,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,16,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,17,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,18,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,19,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"}};
////Вариант 7
////DEFGRF TGRAF[27]={{"gaaaaaae",0,1,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,2,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,3,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,5,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,4,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,6,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,8,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,9,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,10,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,10,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,11,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",0,12,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,13,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,13,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,14,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,14,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",1,15,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,16,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,17,1,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,18,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,19,2,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"}, {"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"}};
////Вариант 1
//DEFGRF TGRAF[33]={{"gaaaaaae",0,1,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,2,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,3,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,5,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,4,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,6,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,8,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,9,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,10,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,10,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,7,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,11,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,12,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,13,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,13,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,14,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,14,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,15,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,16,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,17,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,18,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,19,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,20,3,0,-1,-1,-1,"0.I"}};
// //Вариант 5
////DEFGRF TGRAF[33]={{"gaaaaaae",0,1,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,6,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,11,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,16,2,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,2,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,5,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,3,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,4,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,2,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,2,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,5,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,19,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,7,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",0,10,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,8,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,9,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,7,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,19,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,12,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,15,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,13,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,14,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,12,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,19,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,17,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,18,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,19,3,0,-1,-1,-1,"0.I"},{"gaaaaaae",1,0,1,0,-1,-1,-1,"0.I"},{"gaaaaaae",2,20,1,0,-1,-1,-1,"0.I"}};
////Вариант 1, 3, 3`, 6, 7
//COMPTOP TTOP[30]= {{0,"aaaaaaad",0,2,"aaaaaaad","","",0,"0.I",-1}, {1,"aaaaaaaa",3,4,"aaaaaaaa","","",0,"0.I",-1}, {2,"aaaaaaaa",5,6,"aaaaaaaa","","",0,"0.I",-1}, {3,"aaaaaaaa",7,8,"aaaaaaaa","","",0,"0.I",-1}, {4,"aaaaaaaa",9,9,"aaaaaaaa","","",0,"0.I",-1}, {5,"aaaaaaaa",10,10,"aaaaaaaa","","",0,"0.I",-1}, {6,"aaaaaaad",11,11,"aaaaaaad","","",0,"0.I",-1}, {7,"aaaaaaac",12,13,"aaaaaaac","","",0,"0.I",-1}, {8,"aaaaaaaa",14,14,"aaaaaaaa","","",0,"0.I",-1}, {9,"aaaaaaaa",15,15,"aaaaaaaa","","",0,"0.I",-1}, {10,"aaaaaaac",16,16,"aaaaaaac","","",0,"0.I",-1}, {11,"aaaaaaaa",17,17,"aaaaaaaa","","",0,"0.I",-1}, {12,"aaaaaaaa",18,18,"aaaaaaaa","","",0,"0.I",-1}, {13,"aaaaaaac",19,19,"aaaaaaac","","",0,"0.I",-1}, {14,"aaaaaaac",20,20,"aaaaaaac","","",0,"0.I",-1}, {15,"aaaaaaac",21,21,"aaaaaaac","","",0,"0.I",-1}, {16,"aaaaaaae",22,22,"aaaaaaae","","",0,"0.I",-1}, {17,"aaaaaaae",23,24,"aaaaaaae","","",0,"0.I",-1}, {18,"aaaaaaae",25,25,"aaaaaaae","","",0,"0.I",-1}, {19,"aaaaaaae",26,26,"aaaaaaae","","",0,"0.I",-1}, {20,"aaaaaaae",-77,-77,"aaaaaaae","","",0,"0.I",-1}};
////Вариант 5
////COMPTOP TTOP[25]={{0,"aaaaaaad",0,3,"aaaaaaad","","",0,"0.I",-1},{1,"aaaaaaaa",4,5,"aaaaaaaa","","",0,"0.I",-1},{2,"aaaaaaaa",6,7,"aaaaaaaa","","",0,"0.I",-1},{3,"aaaaaaaa",8,8,"aaaaaaaa","","",0,"0.I",-1},{4,"aaaaaaaa",9,10,"aaaaaaaa","","",0,"0.I",-1},{5,"aaaaaaaa",11,11,"aaaaaaaa","","",0,"0.I",-1},{6,"aaaaaaad",12,12,"aaaaaaad","","",0,"0.I",-1},{7,"aaaaaaac",13,14,"aaaaaaac","","",0,"0.I",-1},{8,"aaaaaaaa",15,15,"aaaaaaaa","","",0,"0.I",-1},{9,"aaaaaaaa",16,16,"aaaaaaaa","","",0,"0.I",-1},{10,"aaaaaaac",17,17,"aaaaaaac","","",0,"0.I",-1},{11,"aaaaaaaa",18,18,"aaaaaaaa","","",0,"0.I",-1},{12,"aaaaaaaa",19,20,"aaaaaaaa","","",0,"0.I",-1},{13,"aaaaaaac",21,21,"aaaaaaac","","",0,"0.I",-1},{14,"aaaaaaac",22,22,"aaaaaaac","","",0,"0.I",-1},{15,"aaaaaaac",23,23,"aaaaaaac","","",0,"0.I",-1},{16,"aaaaaaae",24,24,"aaaaaaae","","",0,"0.I",-1},{17,"aaaaaaae",25,25,"aaaaaaae","","",0,"0.I",-1},{18,"aaaaaaae",26,26,"aaaaaaae","","",0,"0.I",-1},{19,"aaaaaaae",27,28,"aaaaaaae","","",0,"0.I",-1},{20,"aaaaaaae",-77,-77,"aaaaaaae","","",0,"0.I",-1}};

// int ref_GH,ref_LT,ref_LP,ref_AKTOR;
// int ref_index;
  
// *ListTop = (COMPTOP *)calloc(NTtop,sizeof(COMPTOP));
// *LstGraf = (DEFGRF *)calloc(NTgraf,sizeof(DEFGRF));
 
//// Наполнение тестовых массивов

// for(i=0; i < NTtop; i++)
// {
//  strncpy((*(*ListTop + i)).Name,TTOP[i].Name,8);
//  //(*(*ListTop+i)).Top =TTOP[i].Top;
//  (*(*ListTop+i)).Top =-1;
//  (*(*ListTop+i)).FirstDef =TTOP[i].FirstDef;
//  (*(*ListTop+i)).LastDef  =TTOP[i].LastDef ;
//  strncpy((*(*ListTop+i)).NameProt,TTOP[i].NameProt,8);
//  (*(*ListTop+i)).F  =TTOP[i].F;
//  strncpy((*(*ListTop+i)).CodeTr,TTOP[i].CodeTr,3);
//  (*(*ListTop+i)).Faz  =TTOP[i].Faz;
// }
// for(i=0; i < NTgraf; i++)
// {
//  strncpy((*(*LstGraf + i)).Name,TGRAF[i].Name,8);
//  (*(*LstGraf+i)).NambPred =TGRAF[i].NambPred;
//  (*(*LstGraf+i)).NambTop =TGRAF[i].NambTop;
//  (*(*LstGraf+i)).ArcType_ =TGRAF[i].ArcType_;
//  (*(*LstGraf+i)).F =TGRAF[i].F;
//  (*(*LstGraf+i)).I   =-1;
//  (*(*LstGraf+i)).Fl  =0;
//  (*(*LstGraf+i)).NTop=-1;
//  strncpy((*(*LstGraf + i)).CodeTr,TGRAF[i].CodeTr,3);
// }
 
 return 1;
}
PROJECT_END_NAMESPACE
