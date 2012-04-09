#include "graph.h"

PROJECT_BEGIN_NAMESPACE
int GraphMV(TPOData *D, int jroot, int Ntop, DefinePredicate *ListPred, DefineTop *ListTop, DefineGraph *ListGraf)
{
    int jfirst, jlast, j, jPred, jTop, Fpred;
    int (*Tp)(TPOData *);
    Tp =  ListTop[jroot].top;
    (*Tp)(D); //Do actor

    jfirst = ListTop[jroot].FirstDef;
    jlast  = ListTop[jroot].LastDef;
BEGIN:
    for(j = jfirst; j <= jlast; j++) {
	jPred = ListGraf[j].NambPred;

        Tp =  ListPred[jPred].predicate;
        Fpred = (*Tp)(D);

        if(Fpred == 1) {
            jTop = ListGraf[j].NambTop;
            Tp  =  ListTop[jTop].top;
            (*Tp)(D);

            jfirst = ListTop[jTop].FirstDef;
            jlast  = ListTop[jTop].LastDef;

            if(jlast == -77)
                goto ENDGRAF;
            goto BEGIN;
        }
    }
ENDGRAF:
    return 1;
}

PROJECT_END_NAMESPACE