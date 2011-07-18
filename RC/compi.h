#ifndef COMPI_H
#define COMPI_H

#include <QtCore/QString>
#include "compiler.h"

#define MAXDIR 250

/*! Хранить настройки путей для компиятора */
class GraphPaths
{
public:
    GraphPaths(const QString &poDir, const QString &iconDir, const QString &outDir, const QString &NamePO, const QString &namepr, int root);
    QString poDir;
    QString iconDir;
    QString outDir;
    QString NamePO;
    QString namepr;
    int root;
};

class DefineGraph
{
public:
    DefineGraph(int NambPred, int NambTop);
    DefineGraph(int NambPred, int NambTop, int ArcType);
    int NambPred;
    int NambTop;
    int ArcType;
};

class DefineTop
{
public:
    DefineTop(const QString &NameTop, int FirstDef, int  LastDef, int (*Top)(void*[]));
    QString NameTop;
    int  FirstDef;
    int  LastDef;
    int (*Top)(void*[]);
};

class DefinePredicate
{
    DefinePredicate(const QString &name, int (*predicate)(void*[]));
    QString name[10]; /*Внутреннее название предиката*/
    int (*predicate)(void*[]);
};

typedef struct _Graph {
    int FromTop;
    int ToTop;
    int Pred;
    int ArcType;
} COMPH;

typedef struct {
    int TekErr;
    int MaxErr;
    int CodErr[25];
} GRHERR;

typedef char GRFTEXT[30][80];

typedef char* CharS;

typedef char NAMAGR[80];

typedef struct _ListP {
    int Pred;
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


typedef COMPH* COMPHs;

typedef DefineGraph* DEFGRAFs;

typedef COMPRE* COMPREs;

typedef COMPTOP* COMPTOPs;

class GraphCompiler;

typedef struct {
    char name[33];
    void *addr;
} VAR;

class GraphCompiler : public ICompiler
{
public:
    GraphCompiler(const QString &graphName);
    virtual void compile();
private:

    QString myGraphName;
    virtual void init(){}
    virtual void finalize(){}

    static int B_compL__compi(GraphPaths *GrfNav,NAMAGR *NameFilePRG,COMPTOPs *ListTop,
                       DEFGRAFs *LstGraf,COMPREs *ListP,GRHERR *Gerror,int *MaxGH,int *MaxLT,int *MaxLP);
    static int B_free__compi(COMPTOPs *ListTop,DEFGRAFs *LstGraf,COMPREs *ListP,COMPHs *Graph);
    static int BERR2M__compi(GraphPaths *GrfNav, GRHERR *error);
    static int B_TRW__compi(int *HeadTop, COMPTOPs *ListTop, char *WR,int *Top,CharS *Name,int *FirstDef,int *LastDef );
    static int BallocM__compi(GraphPaths *GrfNav,COMPTOPs *ListTop,DEFGRAFs *LstGraf,COMPREs *ListP,COMPHs *Graph,int *MaxGH,int *MaxLT,int *MaxLP,GRHERR *Gerror);
    static int B_PHRWP__compi(int *HeadGPH, COMPHs *ListGPH, char *WR,int *FromTop,int *ToTop,int *Pred, int *ArcType );
    static int B_GRWP__compi(int *HeadGR, DEFGRAFs *LstGraf, char *WR,int *NambPred,int *NambTop,int  *ArcType);
    static int B_PRW__compi(int *HeadPre, COMPREs *ListPre, char *WR,int *Pred,CharS *Name );
    static int P_TERM__compi(int *HeadTop, COMPHs *ListGPH,int *MaxGH );

    static int paaaaaaj__compi(void *p[]);
    static int paaaaaaf__compi(void *p[]);
    static int paaaaaal__compi(void *p[]);
    static int paaaaaad__compi(void *p[]);
    static int paaaaaac__compi(void *p[]);
    static int paaaaaab__compi(void *p[]);
    static int paaaaaaa__compi(void *p[]);
    static int paaaaaai__compi(void *p[]);
    static int paaaaaah__compi(void *p[]);
    static int saaaaaai__compi(void *p[]);
    static int saaaaaam__compi(void *p[]);
    static int saaaaaae__compi(void *p[]);
    static int saaaaaak__compi(void *p[]);
    static int saaaaaab__compi(void *p[]);
    static int saaaaaad__compi(void *p[]);
    static int saaaaaaj__compi(void *p[]);
    static int saaaaaal__compi(void *p[]);
    static int saaaaaaa__compi(void *p[]);
    static int aaaaaaap__compi(void *p[]);
    static int aaaaaaak__compi(void *p[]);
    static int aaaaaaaj__compi(void *p[]);
    static int aaaaaaai__compi(void *p[]);
    static int aaaaaaal__compi(void *p[]);
    static int aaaaaabe__compi(void *p[]);
    static int aaaaaaaf__compi(void *p[]);
    static int aaaaaaab__compi(void *p[]);
    static int aaaaaaag__compi(void *p[]);
    static int aaaaaaae__compi(void *p[]);
    static int aaaaaaba__compi(void *p[]);
    static int aaaaaaah__compi(void *p[]);
    static int aaaaaaad__compi(void *p[]);
    static int aaaaaaaa__compi(void *p[]);

    static int GraphMV(void* params[], int jroot, int Ntop, DefinePredicate *ListPred, DefineTop *ListTop, DefineGraph *ListGraf);
    static int gaaaaaaa__compi(void *p[]);
    static int gaaaaaab__compi(void *p[]);
    static int gaaaaaac__compi(void *p[]);
    static int gaaaaaad__compi(void *p[]);
    static int gaaaaaae__compi(void *p[]);
    static int gaaaaaag__compi(void *p[]);
    static int gaaaaaaf__compi(void *p[]);
};

#endif // COMPI_H
