#ifndef TPODATA_H
#define TPODATA_H
#include "utype.h"

PROJECT_BEGIN_NAMESPACE
class TPOData
{
    public:

    TPOData(int rank);
    ~TPOData();
    int myRank;

    //Указатели на память с данными
    int *ArcType;
    Char200 *CodPU;
    Char200 *CodT;
    int *F1go;
    int *FL;
    int *FLH;
    int *Faza;
    int *FistD;
    int *FlGr;
    int *FromLst;
    int *FromTop;
    COMPTOPs *GPcomp;
    DEFGRAFs *GPgraf;
    int *HeadTop;
    int *I;
    int *KurC;
    int *LastD;
    int *MaxGf;
    int *MaxLT;
    int *NPred;
    int *NTop;
    NAMAGR *NameGr;
    int *Nobj;
    int *NumTop;
    int *NurC;
    int *ToTop;
    char *TurC;
    NAMAGR *WSP;
    char *cE;
    char *cH;
    char *cR;
    char *cV;
    char *cW;
    Char200 *final_graph;
    Char200 *initial_graph;
    int *rank;
    int *root;
    int *root0;


    private:


    //Initialize memory
    void initMemory();
};
PROJECT_END_NAMESPACE
#endif
