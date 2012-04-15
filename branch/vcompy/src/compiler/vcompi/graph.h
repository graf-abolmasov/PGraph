#ifndef GRAPH_H
#define GRAPH_H

#include "tpodata.h"

PROJECT_BEGIN_NAMESPACE
class DefineGraph
{
public:
    DefineGraph(int NambPred, int NambTop);
    DefineGraph(int NambPred, int NambTop, int Type);
    int NambPred;
    int NambTop;
	int Type;
};

class DefineTop
{
public:
    DefineTop(const char *NameTop, int FirstDef, int LastDef, int (*top)(TPOData *));
	const char *NameTop;
    int  FirstDef;
    int  LastDef;
    int (*top)(TPOData *);
};

class DefinePredicate
{
public:
    DefinePredicate(const char *name, int (*predicate)(TPOData *));
    const char *name; /*Внутреннее название предиката*/
    int (*predicate)(TPOData *);
};

int GraphMV(TPOData *D, int jroot, int Ntop, DefinePredicate *ListPred, DefineTop *ListTop, DefineGraph *ListGraf);

PROJECT_END_NAMESPACE

#endif // GRAPH_H
