#ifndef COMPI_H
#define COMPI_H

#define MAXDIR 250
enum Boolean {False,True};
typedef FILE* FILE_;
typedef struct _ListGraf
             {int NambPred;
              int NambTop;
              int ArcType;
              } DEFGRAF;
typedef struct _Graph
             {int FromTop;
              int ToTop;
              int Pred;
              int ArcType;
              } COMPH;
typedef struct
             {int TekErr;
              int MaxErr;
              int CodErr[25];
              } GRHERR;
typedef char GRFTEXT[30][80];
typedef char* CharS;
typedef char NAMAGR[80];
typedef struct _ListP
             {int Pred;
              char Name[10];
              char NameProt[10];
              } COMPRE;
typedef struct _ListT {
  int Top;
  char Name[10];
  int FirstDef;
  int LastDef;
  char NameProt[10];
  char SPred[10];
  char SMsg[10];
} COMPTOP;
typedef struct tagGrfNavi
       {
         char PathPO[MAXDIR];
         char IconDir[MAXDIR];
         char COutDir[MAXDIR];
         char NamePO[10];
         char NamePR[10];
         int root;	/*╩юЁэхтр  тхЁ°шэр уЁрЇр*/
       } GRFNAVI;
typedef COMPH* COMPHs;
typedef DEFGRAF* DEFGRAFs;
typedef COMPRE* COMPREs;
typedef COMPTOP* COMPTOPs;

int Compi(QString myGraphName);

#endif // COMPI_H
