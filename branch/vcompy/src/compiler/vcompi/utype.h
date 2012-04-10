#ifndef UTYPE_H
#define UTYPE_H

#include "defines.h"
#include "stype.h"

PROJECT_BEGIN_NAMESPACE
//Пользовательские типы
typedef struct _Graph
{int FromTop;
    int ToTop;
    int Pred;
    int ArcType;
} COMPH;
typedef char* CharS;
typedef COMPH* COMPHs;
typedef struct _ListP
{int Pred;
    char Name[9];
    char NameProt[9];
} COMPRE;
typedef struct _ListGraf
{char Name[9];
    int NambPred;
    int NambTop;
    int ArcType_;
    int F;
    int I;
    int NTop;
    int Fl;
    char CodeTr[200];
} DEFGRF;



typedef struct {
int TekErr;
int MaxErr;
int CodErr[25];
} GRHERR;
typedef char NAMAGR[20];
typedef COMPRE* COMPREs;
typedef struct _ListT {
    int Top;
    char Name[9];
    int FirstDef;
    int LastDef;
    char NameProt[9];
    char SPName[9];
    char SMName[9];
    int F;
    char CodeTr[200];
    int Faz;
    int rankT;
    int back;
} COMPTOP;

typedef char Char200[200];
typedef char GRFTEXT[30][80];
typedef COMPTOP* COMPTOPs;
typedef DEFGRF* DEFGRAFs;

PROJECT_END_NAMESPACE

//Часто включаемые файлы
#include <stdio.h>
#include <strings.h>
#include <memory.h>

#endif
