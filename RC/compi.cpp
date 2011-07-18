#include <QtCore>
#include <QtGui>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shab.h"
#include "compi.h"
#include "globalvariables.h"

/////////////////////////////// Base modules /////////////////////////////
/*Построение текста с-программы граф-агрегата*/

int GraphCompiler::B_compL__compi(GraphPaths *GrfNav,NAMAGR *NameFilePRG,COMPTOPs *ListTop,
                                  DEFGRAFs *LstGraf,COMPREs *ListP,GRHERR *Gerror,int *MaxGH,int *MaxLT,int *MaxLP)
{
    int i,j,FL ;
    char Tip;
    FILE *fw;
    char Stmp1[10];
    char Stmp2[10];

    QString nameFilePRG = (*GrfNav).outDir + (*GrfNav).namepr + ".cpp";
    strcpy((*NameFilePRG), nameFilePRG.toStdString().c_str());

    if((fw = fopen((*NameFilePRG),"wt"))  == NULL) {
        ((*Gerror).TekErr) = 1;
        ((*Gerror).CodErr)[(*Gerror).MaxErr] = 1;
        (*Gerror).MaxErr++;
        return(1);
    }

    // strcpy(NameP,(*GrfNav).PathPO);
    // strcat(NameP,"GSP_SHAB.dbf");
    // qDebug("NamPRG %s NamSh %s\n",NameFilePRG,NameP);
    // if((d4use(NameP)) < 0)
    //  {
    //   ((*Gerror).TekErr) = 2;
    //   ((*Gerror).CodErr)[(*Gerror).MaxErr] = 2;
    //   (*Gerror).MaxErr++;
    //   return(1);
    //  }

    if (!globalDBManager->Compi_get_GSP_Shab_List())
        QMessageBox::critical(NULL,
                              QObject::tr("Ошибка"),
                              QObject::tr("Не удалось прочитать из БД шаблон "
                                          "для генерации исходного файла (GSP_SHAB).\n")
                              + globalDBManager->lastError().databaseText(),
                              QMessageBox::Ok);

    QString tempQString;

    foreach (Shab* shab, globalDBManager->myShabList){

        // for( d4top(); !d4eof(); d4skip(1L) )
        //  {
        //   Tip = f4char(f4ref("TIPLEX"));
        // ...

        Tip = shab->TIPLEX.toStdString().c_str()[0];
        switch(Tip)
        {
        case 'T':
            tempQString = shab->LEXEM.left(shab->LEXEM.indexOf("@"));
            fprintf(fw,"%s",tempQString.toStdString().c_str());

            break;
        case 'N':
            fprintf(fw,"\n");
            break;

        case '1':
            fprintf(fw,"\"%s.h\"",(*GrfNav).NamePO);
            break;

        case '2':
            for(i=0; i < *MaxLT; i++)
            { FL=1;
                for(j=i-1; j >= 0; j--)
                {
                    if(strcmp((*(*ListTop + i)).NameProt,(*(*ListTop + j)).NameProt) == 0)
                    { FL = 0 ;
                        break;
                    }
                }
                if(FL == 1) fprintf(fw,"\n  int %s(TPOData *); ",(*(*ListTop + i)).NameProt );
            }

            for(i=0; i < *MaxLP; i++)
            { FL=1;
                for(j=i-1; j >= 0; j--)
                {
                    if(strcmp((*(*ListP + i)).NameProt,(*(*ListP + j)).NameProt) == 0)
                    { FL = 0 ;
                        break;
                    }
                }
                if(FL == 1) fprintf(fw,"\n  int %s(TPOData *); ",(*(*ListP + i)).NameProt );
            }
            break;

        case '3':
            fprintf(fw,"%d",*MaxLP );
            break;

        case '4':
            for(i=0; i < ((*MaxLP) - 1); i++)
                fprintf(fw,"       {\"%s\", %s},\n ",(*((*ListP) + i)).Name,(*(*ListP + i)).NameProt );
            fprintf(fw,"       {\"%s\", %s}\n ",(*((*ListP) + i)).Name,(*(*ListP + i)).NameProt );
            break;

        case '5':
            fprintf(fw,"%d",*MaxLT );
            break;

        case '6':
            for(i=0; i <= ((*MaxLT) - 1); i++) {

                if ( !strcmp((*(*ListTop + i)).SPred, "NULL") )
                {  strcpy(Stmp1, "NULL"); }
                else { sprintf(Stmp1, "&%s", (*(*ListTop + i)).SPred ); }

                if ( !strcmp((*(*ListTop + i)).SMsg, "NULL") )
                {  strcpy(Stmp2, "NULL"); }
                else { sprintf(Stmp2, "&%s", (*(*ListTop + i)).SMsg ); }


                if ( i < ((*MaxLT) - 1) ) {
                    fprintf(fw,"       {\"%s\",%d, %d, &%s, %s, %s},\n ",(*(*ListTop + i)).Name,(*(*ListTop + i)).FirstDef,(*(*ListTop + i)).LastDef,(*(*ListTop + i)).NameProt,Stmp1,Stmp2 );
                }
                else {
                    fprintf(fw,"       {\"%s\",%d, %d, &%s, %s, %s}\n " ,(*(*ListTop + i)).Name,(*(*ListTop + i)).FirstDef,(*(*ListTop + i)).LastDef,(*(*ListTop + i)).NameProt,Stmp1,Stmp2 );
                }

            }
            break;

        case '7':
            fprintf(fw,"%d",*MaxGH );
            break;

        case '8':
            for(i=0; i < ((*MaxGH) - 1); i++)
                fprintf(fw,"       {%d, %d, %d },\n ",(*(*LstGraf + i)).NambPred,(*(*LstGraf + i)).NambTop,(*(*LstGraf + i)).ArcType);
            fprintf(fw,"       {%d, %d, %d }\n ",(*(*LstGraf + i)).NambPred,(*(*LstGraf + i)).NambTop,(*(*LstGraf + i)).ArcType);
            break;

        case '9':
            fprintf(fw," %s",(*GrfNav).namepr);
            break;

        case 'A':
            for(i=0; i < *MaxLT; i++)
            {
                if((*(*ListTop + i)).Top ==  (*GrfNav).root)
                {
                    fprintf(fw," %d;\n ",i);
                    break;
                }
            }
            break;
        }

    }  // foreach

    globalDBManager->myShabList.clear();
    fclose(fw);

    return(1);
}


/*З крытие ф йл  текст  гр ф-прогр ммы*/

int GraphCompiler::B_free__compi(COMPTOPs *ListTop,DEFGRAFs *LstGraf,COMPREs *ListP,COMPHs *Graph)
{
    free(*ListTop);
    free(*LstGraf);
    free(*ListP);
    free(*Graph);
    return(1);
}

/* Обработка ошибок */

int GraphCompiler::BERR2M__compi(GraphPaths *GrfNav, GRHERR *error)
{
    int errCode = error->TekErr;
    QMessageBox::critical(NULL, QObject::tr("Ошибка"), QObject::tr("Ошибка номер: ") + QString::number(errCode) + "\n"
                          +  globalDBManager->lastError().databaseText(), QMessageBox::Ok);
}

/*Чтение_запись в структуре ListT*/
int GraphCompiler::B_TRW__compi(int *HeadTop, COMPTOPs *ListTop, char *WR,int *Top,CharS *Name,int *FirstDef,int *LastDef )
{

    if (*Top >= 0)
    {switch (*WR)
        { case 'R':
	    *Top = (*((*ListTop)+(*HeadTop))).Top;
	    break;
        case 'W':
	    (*((*ListTop)+(*HeadTop))).Top = *Top;
	    break;
        }
    }
    if (Name!=NULL)
    {
        if (strlen(*Name) > 0)
        { switch (*WR)
            { case 'R':

                strcpy(*Name, (*((*ListTop)+(*HeadTop))).Name );
                break;

            case 'W':
                strcpy( (*((*ListTop)+(*HeadTop))).Name,*Name);
                break;
            }
        }
    }

    if (*FirstDef >= 0)
    { switch (*WR)
        { case 'R':
	    *FirstDef = (*((*ListTop)+(*HeadTop))).FirstDef;
	    break;

        case 'W':
	    (*((*ListTop)+(*HeadTop))).FirstDef = *FirstDef;
            //	qDebug("FirstDef  %d  \n",*FirstDef);
	    break;
        }
    }

    if (*LastDef >= 0)
    { switch (*WR)
        { case 'R':
	    *LastDef = (*((*ListTop)+(*HeadTop))).LastDef;
	    break;

        case 'W':
            //	 qDebug("LastDef  %d  \n",*LastDef);
	    (*((*ListTop)+(*HeadTop))).LastDef = *LastDef;
	    break;
        }
    }

    //qDebug("TWR HeadT=%d WR=%c Top=%d Name=%s Fdef=%d LDef=%d\n",
    //	   *HeadTop,*WR,*Top,Name,*FirstDef,*LastDef, *Top);getch();
    return(1);
}

/*

void main()
{

 static int HeadTop=0;
 static char WR='R';
 static int Top = 11 ;
 static int FirstDef = 22 ;
 static int LastDef = 33 ;
 static char *Name  ;
 static COMPTOP *ListTop;
 int maxgraf = 2;

ListTop = (COMPTOP *)calloc(maxgraf,sizeof(COMPTOP));
Name	= (char *)calloc(9,sizeof(char));
strcpy(Name,"");
(*ListTop).Top = 1;
(*ListTop).FirstDef = 2;
(*ListTop).LastDef  = 3;
strcpy((*ListTop).Name,"ttttt");
(*(ListTop + 1)).Top = 3;
(*(ListTop + 1)).FirstDef = 777;

B_TRW__compi(&HeadTop, &ListTop, &WR, &Top,&Name,&FirstDef,&LastDef );

 }
*/

int GraphCompiler::BallocM__compi(GraphPaths *GrfNav,COMPTOPs *ListTop,DEFGRAFs *LstGraf,COMPREs *ListP,COMPHs *Graph,int *MaxGH,int *MaxLT,int *MaxLP,GRHERR *Gerror)
{
    int FiltGH__compi();
    int FiltGH2();
    int FiltGH3();
    int uniq = 0;
    int safety = 0;
    int i,IndGH;
    char Name2[80];
    char NameAKT[80];

    int ref_GH,ref_LT,ref_LP,ref_AKTOR;
    int ref_index;

    char   WhereGH[65]  = "NAMEPR = ";
    char   WhereGH2[65] = "NAMEPR = ";
    char   WhereGH3[65] = "NAMEPR = ";

    strcat(WhereGH,"\"");
    strcat(WhereGH,(*GrfNav).namepr.toStdString().c_str());
    strcat(WhereGH,"\"");

    strcat(WhereGH3,"\"");
    strcat(WhereGH3,(*GrfNav).namepr.toStdString().c_str());
    strcat(WhereGH3,"\"");
    strcat(WhereGH3," .AND. PRIOR # 0");

    strcat(WhereGH2,"\"");
    strcat(WhereGH2,(*GrfNav).namepr.toStdString().c_str());
    strcat(WhereGH2,"\"");
    strcat(WhereGH2," .AND. PRIOR = 0");

    /* Построение индекса для БД AKTOR*/

    // strcpy(NameAKT,(*GrfNav).PathPO);
    // strcat(NameAKT,"actor.dbf");

    //qDebug("Whrer:1, %s 2 %s 3 %s\n",WhereGH,WhereGH2, WhereGH3);

    /*ref_AKTOR = d4use(NameAKT);
 if(ref_AKTOR < 0)
 {
     ((*Gerror).TekErr) = 11;
     ((*Gerror).CodErr)[(*Gerror).MaxErr] = 11;
     (*Gerror).MaxErr++;
     return(1);
 }
 ref_index=i4index("akt_name.ndx", "NAMEPR", 1, 0);
 if(ref_index < 0)
 {
     ((*Gerror).TekErr) = 12;
     ((*Gerror).CodErr)[(*Gerror).MaxErr] = 12;
     (*Gerror).MaxErr++;
     return(1);
 }
*/
    /* Определение размера структуры описания графа*/

    (*MaxGH) = globalDBManager->Compi_count_MaxGH((*GrfNav).namepr);
    /*
        strcpy(Name2,(*GrfNav).PathPO);
        strcat(Name2,"graph.dbf");

        ref_GH = d4use(Name2);
        if(ref_GH < 0)
         {
          ((*Gerror).TekErr) = 4;
          ((*Gerror).CodErr)[(*Gerror).MaxErr] = 4;
          (*Gerror).MaxErr++;
          return(1);
         }

        e4parse((char *) WhereGH,&compile_ptr);
        if( (IndGH = i4index_filter("IndGH.ndx","NAMEPR+str(NFROM)+str(PRIOR)",uniq,safety,FiltGH__compi) ) < 0 )
        {
            ((*Gerror).TekErr) = 5;
            ((*Gerror).CodErr)[(*Gerror).MaxErr] = 5;
            (*Gerror).MaxErr++;
            return(1);
        }

        d4top();
        (*MaxGH) = 0;
        do
            {  (*MaxGH)++; }
        while( !(d4skip(1L) == 3) );
        (*MaxGH)--;
 */

    // Определение размера структуры описания графа
    // (кол-во строк в GRAPH для данного агрегата)

    // (*MaxGH) = 0;
    // QSqlQuery query;
    // query.prepare("SELECT * FROM graph"
    //               " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID;");
    // query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    // query.bindValue(":EXTNAME", (*GrfNav).NamePR);
    // bool queryres = query.exec();

    // // int fieldNo = query.record().indexOf("country");
    // if (queryres)
    // {
    //     while (query.next()) {
    //         (*MaxGH)++;
    //     }
    // }
    // else
    // {
    //     QMessageBox::critical(NULL,
    //                           QObject::tr("Ошибка"),
    //                           QObject::tr("GRAPH doesn't contain ")
    //                           + (*GrfNav).NamePR,
    //                           QMessageBox::Ok);
    // }


    /*Выделение корневой вершины*/
    QString tempQString = (*GrfNav).namepr;
    (*GrfNav).root = globalDBManager->Compi_get_root_top(tempQString);

    // x4filter_pop();
    // e4parse((char *) WhereGH2,&compile_ptr2);
    // x4filter(FiltGH2);
    // if(x4top() != 0)
    //  {
    //   ((*Gerror).TekErr) = 6;
    //   ((*Gerror).CodErr)[(*Gerror).MaxErr] = 6;
    //   (*Gerror).MaxErr++;
    //   return(1);
    //  }
    // (*GrfNav).root = f4int(f4ref("NFROM"));


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///* Определение размера структуры описания вершин*/
    //(*MaxLT) = 0;
    (*MaxLT) = globalDBManager->Compi_count_MaxLT((*GrfNav).namepr);

    // if (!globalDBManager->db.open())
    // {
    //     QMessageBox::critical(NULL,
    //                           QObject::tr("Ошибка"),
    //                           QObject::tr("Can't open database!"),
    //                           QMessageBox::Ok);
    //     return 1;
    // }

    //(*MaxLT) = 0;
    //QSqlQuery query;
    //query.prepare("SELECT * FROM graphtop"
    //              " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID;");
    //query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    //tempQString = (*GrfNav).NamePR;
    //query.bindValue(":EXTNAME", tempQString);
    //bool queryres = query.exec();

    //// int fieldNo = query.record().indexOf("country");
    //if (queryres)
    //{
    //    while (query.next()) {
    //        (*MaxLT)++;
    //    }
    //}
    //else
    //{
    //    QMessageBox::critical(NULL,
    //                          QObject::tr("Ошибка"),
    //                          QObject::tr("graphtop doesn't contain ")
    //                          + tempQString,
    //                          QMessageBox::Ok);
    //}

    // strcpy(Name2,(*GrfNav).PathPO);
    // strcat(Name2,"graphtop.dbf");

    // ref_LT = d4use(Name2);
    // e4parse((char *) WhereGH,&compile_ptr);
    // if( ( i4index_filter("IndLT.ndx","NAMEPR+str(NTOP)",uniq,safety,FiltGH__compi) ) < 0 )
    // {
    //  ((*Gerror).TekErr) = 7;
    //  ((*Gerror).CodErr)[(*Gerror).MaxErr] = 7;
    //  (*Gerror).MaxErr++;
    //  return(1);
    // }

    // d4top();
    // (*MaxLT) = 0;
    // do
    // {  (*MaxLT)++; }
    // while( !(d4skip(1L) == 3) );


    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///* Определение размера структуры описания дуг*/

    (*MaxLP) = globalDBManager->Compi_count_MaxLP((*GrfNav).namepr);

    ////QSqlQuery query;
    //query.prepare("SELECT * FROM graphpre"
    //              " WHERE NAMEPR = :EXTNAME AND PROJECT_ID = :PROJECT_ID;");
    //query.bindValue(":PROJECT_ID", globalDBManager->getProjectId());
    //query.bindValue(":EXTNAME", (*GrfNav).NamePR);
    //queryres = query.exec();

    //// int fieldNo = query.record().indexOf("country");
    //if (queryres)
    //{
    //    while (query.next()) {
    //        (*MaxLP)++;
    //    }
    //}
    //else
    //{
    //    QMessageBox::critical(NULL,
    //                          QObject::tr("Ошибка"),
    //                          QObject::tr("GRAPH doesn't contain ")
    //                          + (*GrfNav).NamePR,
    //                          QMessageBox::Ok);
    //}

    // strcpy(Name2,(*GrfNav).PathPO);
    // strcat(Name2,"graphpre.dbf");

    // ref_LP = d4use(Name2);
    // e4parse((char *) WhereGH,&compile_ptr);
    // if( ( i4index_filter("IndLP.ndx","NAMEPR+str(NPRED)",uniq,safety,FiltGH__compi) ) < 0 )
    //  {
    //   ((*Gerror).TekErr) = 8;
    //   ((*Gerror).CodErr)[(*Gerror).MaxErr] = 8;
    //   (*Gerror).MaxErr++;
    //   return(1);
    //  }
    // d4top();
    // (*MaxLP) = 0;
    // do
    // {  (*MaxLP)++; }
    // while( !(d4skip(1L) == 3) );



    *ListTop = (COMPTOP *)calloc((*MaxLT),sizeof(COMPTOP));
    *LstGraf = (DefineGraph *)calloc((*MaxGH),sizeof(DefineGraph));
    *ListP   = (COMPRE  *)calloc((*MaxLP),sizeof(COMPRE ));
    *Graph   = (COMPH   *)calloc((*MaxGH),sizeof(COMPH  ));

    qDebug("MGH=%d MT=%d MP=%d\n",*MaxGH,*MaxLT,*MaxLP);

    //  d4select(ref_GH);
    //  x4filter_pop();
    //  e4parse((char *) WhereGH3,&compile_ptr3);
    //  x4filter(FiltGH3);
    //  i4select(IndGH);
    //  x4top();


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Заполнение структуры Graph

    globalDBManager->Compi_fill_Graph_struct((*GrfNav).namepr, (*MaxGH), Graph);

    // for(i=0; i < (*MaxGH); i++)
    //  {
    //   (*(*Graph+i)).FromTop = f4int(f4ref("NFROM"));
    //   (*(*Graph+i)).ToTop   = f4int(f4ref("NTO"));
    //   (*(*Graph+i)).Pred    = f4int(f4ref("NPRED"));
    //   if ( f4ref("ARCTYPE") >= 0 ) {
    //     (*(*Graph+i)).ArcType = f4int(f4ref("ARCTYPE"));
    //   }
    //   else { (*(*Graph+i)).ArcType = 1; }

    //qDebug("from %d to %d Pred %d, ArcType %d\n",(*(*Graph+i)).FromTop,(*(*Graph+i)).ToTop,(*(*Graph+i)).Pred,(*(*Graph+i)).ArcType);

    //   x4skip(1L);
    //  }


    //  d4select(ref_LT);
    //  x4filter_pop();
    //  e4parse((char *) WhereGH,&compile_ptr);
    //  x4filter(FiltGH__compi);

    //  if(x4relate("NAME",ref_AKTOR, ref_index,-1L) > 0)
    //   {
    //    ((*Gerror).TekErr) = 13;
    //    ((*Gerror).CodErr)[(*Gerror).MaxErr] = 13;
    //    (*Gerror).MaxErr++;
    //    return(1);
    //   }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Заполнение структуры ListT

    globalDBManager->Compi_fill_ListT_struct((*GrfNav).namepr, (*MaxLT), ListTop);

    // x4top();
    // for(i=0; i < *MaxLT; i++)
    // {
    //  strncpy((*(*ListTop + i)).Name,f4str(f4ref("NAME")),8);
    //  (*(*ListTop+i)).Top = f4int(f4ref("NTOP"));
    //  (*(*ListTop+i)).FirstDef =-77;
    //  (*(*ListTop+i)).LastDef  =-77;

    //  if ( (f4ref("SPred") >= 0) && (strncmp( f4str(f4ref("SPred")), "        ",8 )) ) {
    //    strncpy((*(*ListTop + i)).SPred,f4str(f4ref("SPred")),8);
    //  }
    //  else {  strcpy((*(*ListTop + i)).SPred,"NULL");  }

    //  if ( (f4ref("SMsg") >= 0) && (strncmp( f4str(f4ref("SMsg")), "        ",8 )) ) {
    //    strncpy((*(*ListTop + i)).SMsg,f4str(f4ref("SMsg")),8);
    //  }
    //  else {  strcpy((*(*ListTop + i)).SMsg,"NULL");  }

    //  d4select(ref_AKTOR);
    //  strncpy((*(*ListTop + i)).NameProt,f4str(f4ref("PROTOTIP")),8);
    //  d4select(ref_LT);
    //  qDebug("Name %s Top=%d  FistDef=%d LastDef=%d NameProt=%s\n",(*(*ListTop + i)).Name,
    //             (*(*ListTop+i)).Top,(*(*ListTop+i)).FirstDef,(*(*ListTop+i)).LastDef,(*(*ListTop + i)).NameProt);
    //  x4skip(1L);
    // }


    // x4relate_reset();

    // d4select(ref_LP);
    // x4filter_pop();
    // e4parse((char *) WhereGH,&compile_ptr);
    // x4filter(FiltGH__compi);
    // if(x4relate("NAME",ref_AKTOR, ref_index,-1L) > 0)
    // {
    //  ((*Gerror).TekErr) = 14;
    //  ((*Gerror).CodErr)[(*Gerror).MaxErr] = 14;
    //  (*Gerror).MaxErr++;
    //  return(1);
    // }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //// Заполнение структуры ListP

    globalDBManager->Compi_fill_ListP_struct((*GrfNav).namepr, (*MaxLP), ListP);

    // x4top();
    // for(i=0; i < (*MaxLP); i++)
    // {
    //  strncpy((*(*ListP + i)).Name,f4str(f4ref("NAME")),8);
    //  (*(*ListP+i)).Pred = f4int(f4ref("NPRED"));
    //  d4select(ref_AKTOR);
    //  strncpy((*(*ListP + i)).NameProt,f4str(f4ref("PROTOTIP")),8);
    //  qDebug("Name=%s  Pred=%d NameProt=%s\n",(*(*ListP + i)).Name,(*(*ListP+i)).Pred,(*(*ListP + i)).NameProt);
    //  d4select(ref_LP);
    //  x4skip(1L);
    // }


    // d4close_all();
    return 1;
}

//int FiltGH__compi()
//{char *rez_ptr;
// if (d4deleted()) return 1;
// rez_ptr = (char*)e4exec(compile_ptr);
// if (*((int *) rez_ptr) == 1 ) return(0);
//                          else return(1);
//}

//int FiltGH2()
//{char *rez_ptr;
// if (d4deleted()) return 1;
// rez_ptr = (char*)e4exec(compile_ptr2);
// if (*((int *) rez_ptr) == 1 ) return(0);
//                          else return(1);
//}

//int FiltGH3()
//{char *rez_ptr;
// if (d4deleted()) return 1;
// rez_ptr = (char*)e4exec(compile_ptr3);
// if (*((int *) rez_ptr) == 1 ) return(0);
//                          else return(1);
//}

/*Чтение_з пись в структуре ListT*/

int GraphCompiler::B_PHRWP__compi(int *HeadGPH, COMPHs *ListGPH, char *WR,int *FromTop,int *ToTop,int *Pred, int *ArcType )
{

    int i;
    //for (i=0; i < 6; i++)
    //qDebug("Graph: %d %d %d \n", (*(*ListGPH + i)).FromTop,(*(*ListGPH + i)).ToTop,(*(*ListGPH + i)).Pred);

    if (*FromTop >= 0)
    {switch (*WR)
        { case 'R':
            *FromTop = (*((*ListGPH)+(*HeadGPH))).FromTop;
            break;
        case 'W':
            (*((*ListGPH)+(*HeadGPH))).FromTop = *FromTop;
            break;
        }
    }


    if (*ToTop >= 0)
    { switch (*WR)
        { case 'R':
            *ToTop = (*((*ListGPH)+(*HeadGPH))).ToTop;
            break;

        case 'W':
            (*((*ListGPH)+(*HeadGPH))).ToTop = *ToTop;
            break;
        }
    }

    if (*Pred >= 0)
    { switch (*WR)
        { case 'R':
            *Pred = (*((*ListGPH)+(*HeadGPH))).Pred;
            break;

        case 'W':
            (*((*ListGPH)+(*HeadGPH))).Pred = *Pred;
            break;
        }
    }

    if (*ArcType >= 0)
    { switch (*WR)
        { case 'R':
            *ArcType = (*((*ListGPH)+(*HeadGPH))).ArcType;
            break;

        case 'W':
            (*((*ListGPH)+(*HeadGPH))).ArcType = *ArcType;
            break;
        }
    }

    //qDebug("FROM TO PRED : %d  %d  %d\n",*FromTop,*ToTop,*Pred);
    //getch();
    return(1);
}

/*

void main()
{

 static int HeadGPH=0;
 static char WR='R';
 static int FromTop = 0 ;
 static int ToTop = 0 ;
 static int Pred = 0 ;
 static COMPH *ListGPH;
 int maxgraf = 2;

ListGPH = (COMPH *)calloc(maxgraf,sizeof(COMPH));
(*ListGPH).FromTop = 1;
(*ListGPH).ToTop = 2;
(*ListGPH).Pred  = 3;
(*(ListGPH + 1)).FromTop = 3;
(*(ListGPH + 1)).ToTop = 777;

B_PHRW(&HeadGPH, &ListGPH, &WR, &FromTop,&ToTop,&Pred );

 }

*/
int GraphCompiler::B_GRWP__compi(int *HeadGR, DEFGRAFs *LstGraf, char *WR,int *NambPred,int *NambTop,int  *ArcType)
{

    if (*NambPred >= 0)
    {switch (*WR)
        { case 'R':
            *NambPred = (*((*LstGraf)+(*HeadGR))).NambPred;
            break;
        case 'W':
            // qDebug("HeadGR = %d NambPred %d\n", *HeadGR,*NambPred);
            (*((*LstGraf)+(*HeadGR))).NambPred = *NambPred;
            break;
        }
    }

    if (*NambTop >= 0)
    { switch (*WR)
        { case 'R':
            *NambTop = (*((*LstGraf)+(*HeadGR))).NambTop;
            break;

        case 'W':
            //qDebug("HeadGR = %d NambTop %d\n", *HeadGR,*NambTop);
            (*((*LstGraf)+(*HeadGR))).NambTop = *NambTop;
            break;
        }
    }

    if (*ArcType >= 0)
    { switch (*WR)
        { case 'R':
            *ArcType = (*((*LstGraf)+(*HeadGR))).ArcType;
            break;

        case 'W':
            //qDebug("HeadGR = %d NambTop %d\n", *HeadGR,*NambTop);
            (*((*LstGraf)+(*HeadGR))).ArcType = (*((*LstGraf)+(*HeadGR))).ArcType;
            break;
        }
    }


    return(1);
}

/*

void main()
{

 static int HeadGR=0;
 static DEFGRAF Graf[3] = {{1,2},{3,4},{5,6}};
 static char WR='W';
 static int NambPred = 11 ;
 static int NambTop  = 22;
 static DEFGRAF *LstGraf;
 int maxgraf = 2;

LstGraf = (DEFGRAF *)calloc(maxgraf,sizeof(DEFGRAF));
(*LstGraf).NambPred = 1;
(*LstGraf).NambTop = 2;
(*(LstGraf + 1)).NambPred = 3;
(*(LstGraf + 1)).NambTop = 4;

B_GRW(&HeadGR, &LstGraf, &WR, &NambPred,&NambTop );

 }
*/

/*Чтение_з пись в структуре ListP*/

int GraphCompiler::B_PRW__compi(int *HeadPre, COMPREs *ListPre, char *WR,int *Pred,CharS *Name )
{

    if (*Pred >= 0)
    {switch (*WR)
        { case 'R':
	    *Pred = (*((*ListPre)+(*HeadPre))).Pred;
	    break;
        case 'W':
	    (*((*ListPre)+(*HeadPre))).Pred = *Pred;
	    break;
        }
    }

    if (strlen(*Name) > 0)
    { switch (*WR)
        { case 'R':

	    strcpy(*Name, (*((*ListPre)+(*HeadPre))).Name );
	    break;

        case 'W':
	    strcpy( (*((*ListPre)+(*HeadPre))).Name,*Name);
	    break;
        }
    }


    return(1);
}


/*
void main()
{

 static int HeadPre=0;
 static char WR='R';
 static int Pre = 11 ;
 static char *Name  ;
 static COMPRE *ListPre;
 int maxgraf = 2;

ListPre = (COMPRE *)calloc(maxgraf,sizeof(COMPRE));
Name	= (char *)calloc(9,sizeof(char));
strcpy(Name,"""");
(*ListPre).Pred = 1;
strcpy((*ListPre).Name,"ttttt");
(*(ListPre + 1)).Pred = 3;

B_PRW__compi(&HeadPre, &ListPre, &WR, &Pre,&Name );

 }

*/
/*Предик т проверки тупиковости вершины*/

int GraphCompiler::P_TERM__compi(int *HeadTop, COMPHs *ListGPH,int *MaxGH )
{
    int i;

    if (*HeadTop < *MaxGH) {
        for (i=0; i < *MaxGH; i++)
        {
            if ( *HeadTop == (*((*ListGPH)+(i))).FromTop )
                return(0);
        }
        return(1);
    }
    return(0);

}


/*
void main()
{
 int r;
 static int HeadTop=1;
 static COMPH *ListGPH;
 int MaxGH = 2;

ListGPH = (COMPH *)calloc(MaxGH,sizeof(COMPH));
(*ListGPH).FromTop = 1;
(*ListGPH).ToTop = 2;
(*ListGPH).Pred  = 3;
(*(ListGPH + 1)).FromTop = 3;
(*(ListGPH + 1)).ToTop = 777;

r=P_TERM__compi(&HeadTop,&ListGPH,&MaxGH );

 }
*/

////////////////////////////// Inline modules ////////////////////////////

int GraphCompiler::paaaaaaj__compi(void *p[])
{
    return((*(GRHERR *)(p[0])).TekErr != 0);
}


int GraphCompiler::paaaaaaf__compi(void *p[])
{
    return(1);
}


int GraphCompiler::paaaaaal__compi(void *p[])
{
    return((*(int *)(p[0]))==(*(int *)(p[1])));
}


int GraphCompiler::saaaaaam__compi(void *p[])
{
    return P_TERM__compi((int *)p[0],
                         (COMPHs *)p[1],
                         (int *)p[2]);
}


int GraphCompiler::paaaaaad__compi(void *p[])
{
    return((*(int *)(p[0]))>=(*(int *)(p[1])));
}


int GraphCompiler::paaaaaac__compi(void *p[])
{
    return((*(int *)(p[0]))==(*(int *)(p[1])));
}


int GraphCompiler::paaaaaab__compi(void *p[])
{
    return((*(int *)(p[0]))==1);
}


int GraphCompiler::paaaaaaa__compi(void *p[])
{
    return((*(int *)(p[0]))==0);
}


int GraphCompiler::saaaaaaa__compi(void *p[])
{
    return B_TRW__compi((int *)p[0],
                        (COMPTOPs *)p[1],
                        (char *)p[2],
                        (int *)p[3],
                        (CharS *)p[4],
                        (int *)p[5],
                        (int *)p[6]);
}


int GraphCompiler::aaaaaaap__compi(void *p[])
{
    (*(int *)(p[0]))--;
    return 1;
}


int GraphCompiler::paaaaaai__compi(void *p[])
{
    return((*(int *)(p[0]))==(*(int *)(p[1])));
}


int GraphCompiler::saaaaaab__compi(void *p[])
{
    return B_PRW__compi((int *)p[0],
                        (COMPREs *)p[1],
                        (char *)p[2],
                        (int *)p[3],
                        (CharS *)p[4]);
}


int GraphCompiler::aaaaaaak__compi(void *p[])
{
    (*(int *)(p[0]))=(*(int *)(p[1]));
    return 1;
}


int GraphCompiler::aaaaaaaj__compi(void *p[])
{
    (*(int *)(p[0]))=0;
    return 1;
}


int GraphCompiler::aaaaaaai__compi(void *p[])
{
    (*(int *)(p[0]))++;
    return 1;
}


int GraphCompiler::paaaaaah__compi(void *p[])
{
    //qDebug("Nobj=%d ToTop=%d\n",(*(int *)(p[0])),(*(int *)(p[1])));getch();
    return((*(int *)(p[0]))==(*(int *)(p[1])));
}


int GraphCompiler::aaaaaaal__compi(void *p[])
{
    (*(int *)(p[0]))=(*(int *)(p[1]));
    return 1;
}


int GraphCompiler::aaaaaabe__compi(void *p[])
{
    (*((*(DEFGRAFs *)(p[0]))+(*(int *)(p[1])))).ArcType = (*((*(COMPHs *)(p[2]))+(*(int *)(p[1])))).ArcType;
    return 1;
}


int GraphCompiler::saaaaaal__compi(void *p[])
{
    return B_GRWP__compi((int *)p[0],
                         (DEFGRAFs *)p[1],
                         (char *)p[2],
                         (int *)p[3],
                         (int *)p[4],
                         (int *)p[5]);
}


int GraphCompiler::aaaaaaaf__compi(void *p[])
{
    (*(int *)(p[0]))++;
    return 1;
}


int GraphCompiler::aaaaaaab__compi(void *p[])
{
    (*(int *)(p[0]))=(*(int *)(p[1]));
    return 1;
}


int GraphCompiler::aaaaaaag__compi(void *p[])
{
    (*(int *)(p[0]))++;
    return 1;
}


int GraphCompiler::aaaaaaae__compi(void *p[])
{
    (*(int *)(p[0]))=1;
    return 1;
}


int GraphCompiler::saaaaaak__compi(void *p[])
{
    return B_PHRWP__compi((int *)p[0],
                          (COMPHs *)p[1],
                          (char *)p[2],
                          (int *)p[3],
                          (int *)p[4],
                          (int *)p[5],
                          (int *)p[6]);
}


int GraphCompiler::aaaaaaba__compi(void *p[])
{
    int k1;
    for (k1=0;k1<(*(int *)(p[0]));k1++)
        qDebug("Top=%d, Name=%s, FDef=%d, LDef=%d, NPr=%s\n",(*((*(COMPTOPs *)(p[1]))+k1)).Top,
               (*((*(COMPTOPs *)(p[1]))+k1)).Name,(*((*(COMPTOPs *)(p[1]))+k1)).FirstDef,(*((*(COMPTOPs *)(p[1]))+k1)).LastDef,(*((*(COMPTOPs *)(p[1]))+k1)).NameProt);

    return 1;
}


int GraphCompiler::saaaaaaj__compi(void *p[])
{
    return BERR2M__compi((GraphPaths *)p[0],
                         (GRHERR *)p[1]);
}


int GraphCompiler::saaaaaai__compi(void *p[])
{
    return BallocM__compi((GraphPaths *)p[0],
                          (COMPTOPs *)p[1],
                          (DEFGRAFs *)p[2],
                          (COMPREs *)p[3],
                          (COMPHs *)p[4],
                          (int *)p[5],
                          (int *)p[6],
                          (int *)p[7],
                          (GRHERR *)p[8]);
}


int GraphCompiler::aaaaaaah__compi(void *p[])
{
    (*(int *)(p[0]))++;
    return 1;
}


int GraphCompiler::aaaaaaad__compi(void *p[])
{
    // ═рўрыю
    return 1;
}


int GraphCompiler::aaaaaaaa__compi(void *p[])
{
    //  DO
    return 1;
}


int GraphCompiler::saaaaaae__compi(void *p[])
{
    return B_free__compi((COMPTOPs *)p[0],
                         (DEFGRAFs *)p[1],
                         (COMPREs *)p[2],
                         (COMPHs *)p[3]);
}


int GraphCompiler::saaaaaad__compi(void *p[])
{
    return B_compL__compi((GraphPaths *)p[0],
                          (NAMAGR *)p[1],
                          (COMPTOPs *)p[2],
                          (DEFGRAFs *)p[3],
                          (COMPREs *)p[4],
                          (GRHERR *)p[5],
                          (int *)p[6],
                          (int *)p[7],
                          (int *)p[8]);
}

////////////////////////////// graphmv ///////////////////////////

int GraphCompiler::GraphMV(void* params[], int jroot, int Ntop, DefinePredicate *ListPred, DefineTop *ListTop, DefineGraph *ListGraf)
{
    int jfirst, jlast, j, jPred, jTop, Fpred;
    int (*Tp)(void*[]);
    /*Вызов корневой вершины*/

    qDebug("Начало\n");
    Tp =  ListTop[jroot].Top;
    (*Tp)((void **)(params[jroot]));
    qDebug("Вершин : %s \n", ListTop[jroot].NameTop);

    jfirst = ListTop[jroot].FirstDef;
    jlast  = ListTop[jroot].LastDef;
BEGIN:
    for (j = jfirst; j <= jlast; j++) {
	jPred = ListGraf[j].NambPred;
        Tp =  ListPred[jPred].predicate;
        Fpred = (*Tp)((void **)(params[jPred+Ntop]));
        qDebug("Пред: %s\n", ListPred[jPred].name);
        if (Fpred == 1) {
            jTop = ListGraf[j].NambTop;
            qDebug("Текущая вершина: %s \n", ListTop[jTop].NameTop);
            Tp  =  ListTop[jTop].Top;
            (*Tp)((void **)(params[jTop]));

            jfirst = ListTop[jTop].FirstDef;
            jlast  = ListTop[jTop].LastDef;

            if (jlast == -77)
                goto ENDGRAF;
            goto BEGIN;
        }
    }
    qDebug("Из вершины %s нет разрешенного выхода\n",ListTop[jTop].NameTop);
ENDGRAF:
    return 1;
}

/////////////////////////////// Aggregates /////////////////////////////
int GraphCompiler::gaaaaaaa__compi(void *p[])
{
    static DefinePredicate ListPred[2] = {
        DefinePredicate("paaaaaah", paaaaaah__compi),
        DefinePredicate("paaaaaaf", paaaaaaf__compi)
    };
    static DefineTop ListTop[4] = {
        DefineTop("aaaaaaaj",0, 0, &aaaaaaaj__compi),
        DefineTop("aaaaaaam",1, 2, &saaaaaaa__compi),
        DefineTop("aaaaaaai",3, 3, &aaaaaaai__compi),
        DefineTop("aaaaaaal",-77, -77, &aaaaaaal__compi)
    };
    static DefineGraph ListGraf[4] = {
        DefineGraph(1, 1, 0),
        DefineGraph(0, 3, 0),
        DefineGraph(1, 2, 0),
        DefineGraph(1, 1, 0),
    };

    int Ntop = 4;
    int jroot = 0;

    GraphMV(p, jroot, Ntop, ListPred, ListTop, ListGraf);
    return(1);
}

int GraphCompiler::gaaaaaab__compi(void *p[])
{
    static DefinePredicate ListPred[2] = {
        DefinePredicate("paaaaaai", paaaaaai__compi),
        DefinePredicate("paaaaaaf", paaaaaaf__compi)
    };
    static DefineTop ListTop[4] = {
        DefineTop("aaaaaaaj",0, 0, &aaaaaaaj__compi),
        DefineTop("aaaaaaan",1, 2, &saaaaaab__compi),
        DefineTop("aaaaaaai",3, 3, &aaaaaaai__compi),
        DefineTop("aaaaaaak",-77, -77, &aaaaaaak__compi)
    };
    static DefineGraph ListGraf[4] = {
        DefineGraph(1, 1),
        DefineGraph(0, 3),
        DefineGraph(1, 2),
        DefineGraph(1, 1)
    };

    int Ntop = 4;
    int jroot = 0;

    GraphMV(p, jroot, Ntop, ListPred, ListTop, ListGraf);
    return(1);
}

int GraphCompiler::gaaaaaac__compi(void *p[])
{
    static DefinePredicate ListPred[1] = {
        DefinePredicate("paaaaaaf", paaaaaaf__compi)
    };
    static DefineTop ListTop[6] = {
        DefineTop("gaaaaaab",0, 0, &gaaaaaab__compi),
        DefineTop("gaaaaaaa",1, 1, &gaaaaaaa__compi),
        DefineTop("aaaaaabc",2, 2, &saaaaaal__compi),
        DefineTop("aaaaaaaf",3, 3, &aaaaaaaf__compi),
        DefineTop("aaaaaaab",-77, -77, &aaaaaaab__compi),
        DefineTop("aaaaaabe",4, 4, &aaaaaabe__compi)
    };
    static DefineGraph ListGraf[5] = {
        DefineGraph(0, 5 ),
        DefineGraph(0, 0 ),
        DefineGraph(0, 3 ),
        DefineGraph(0, 4 ),
        DefineGraph(0, 2 )
    };
    int Ntop = 6;
    int jroot = 1;

    GraphMV(p,jroot,Ntop,ListPred,ListTop,ListGraf);
    return(1);
}

int GraphCompiler::gaaaaaad__compi(void *p[])
{
    static DefinePredicate ListPred[1] = {
        DefinePredicate("paaaaaaf", paaaaaaf__compi)
    };
    static DefineTop ListTop[4] = {
        DefineTop("aaaaaaab",0, 0, &aaaaaaab__compi),
        DefineTop("gaaaaaac",1, 1, &gaaaaaac__compi),
        DefineTop("aaaaaaag",2, 2, &aaaaaaag__compi),
        DefineTop("aaaaaaae",-77, -77, &aaaaaaae__compi)
    };
    static DefineGraph ListGraf[3] = {
        DefineGraph(0, 1),
        DefineGraph(0, 2),
        DefineGraph(0, 3)
    };

    int Ntop =4;
    int jroot = 0;

    GraphMV(p,jroot,Ntop,ListPred,ListTop,ListGraf);
    return(1);
}

int GraphCompiler::gaaaaaae__compi(void *p[])
{
    static DefinePredicate ListPred[1] = {
        DefinePredicate("paaaaaaf", paaaaaaf__compi)
    };
    static DefineTop ListTop[2] = {
        DefineTop("aaaaaaap",0, 0, &aaaaaaap__compi),
        DefineTop("aaaaaaaq",-77, -77, &saaaaaaa__compi)
    };
    static DefineGraph ListGraf[1] = {
        DefineGraph(0, 1)
    };
    int Ntop =2;
    int jroot = 0;

    GraphMV(p,jroot,Ntop,ListPred,ListTop,ListGraf);
    return(1);
}

int GraphCompiler::gaaaaaag__compi(void *p[])
{
    static DefinePredicate ListPred[8] = {
        DefinePredicate("paaaaaaj", paaaaaaj__compi),
        DefinePredicate("paaaaaaf", paaaaaaf__compi),
        DefinePredicate("paaaaaab", paaaaaab__compi),
        DefinePredicate("paaaaaaa", paaaaaaa__compi),
        DefinePredicate("paaaaaad", paaaaaad__compi),
        DefinePredicate("paaaaaac", paaaaaac__compi),
        DefinePredicate("paaaaaak", saaaaaam__compi),
        DefinePredicate("paaaaaal", paaaaaal__compi)
    };
    static DefineTop ListTop[15] = {
        DefineTop("aaaaaaay",0, 1, &saaaaaai__compi),
         DefineTop("aaaaaaaz",-77, -77, &saaaaaaj__compi),
         DefineTop("aaaaaaad",2, 3, &aaaaaaad__compi),
         DefineTop("aaaaaaag",4, 5, &aaaaaaag__compi),
         DefineTop("aaaaaaap",6, 6, &aaaaaaap__compi),
         DefineTop("aaaaaaaq",7, 7, &saaaaaaa__compi),
         DefineTop("aaaaaaaf",8, 8, &aaaaaaaf__compi),
         DefineTop("aaaaaaah",9, 11, &aaaaaaah__compi),
         DefineTop("aaaaaaaa",12, 12, &aaaaaaaa__compi),
         DefineTop("aaaaaaaw",13, 13, &saaaaaaa__compi),
         DefineTop("gaaaaaac",14, 14, &gaaaaaac__compi),
         DefineTop("gaaaaaad",15, 15, &gaaaaaad__compi),
         DefineTop("gaaaaaae",16, 16, &gaaaaaae__compi),
         DefineTop("aaaaaabb",17, 18, &saaaaaak__compi),
         DefineTop("aaaaaaba",-77, -77, &aaaaaaba__compi)
    };
    static DefineGraph ListGraf[19] = {
        DefineGraph(0, 1),
        DefineGraph(1, 13),
        DefineGraph(5, 10),
        DefineGraph(1, 4),
        DefineGraph(4, 12),
        DefineGraph(1, 13),
        DefineGraph(1, 5),
        DefineGraph(1, 6),
        DefineGraph(1, 7),
        DefineGraph(6, 8),
        DefineGraph(7, 14),
        DefineGraph(1, 9),
        DefineGraph(1, 7),
        DefineGraph(1, 11),
        DefineGraph(1, 3),
        DefineGraph(1, 13),
        DefineGraph(1, 14),
        DefineGraph(3, 9),
        DefineGraph(2, 2)
    };
    int Ntop =15;
    int jroot = 0;

    GraphMV(p,jroot,Ntop,ListPred,ListTop,ListGraf);
    return(1);
}

int GraphCompiler::gaaaaaaf__compi(void *p[])
{
    static DefinePredicate ListPred[2] = {
        DefinePredicate("paaaaaaf", paaaaaaf__compi),
        DefinePredicate("paaaaaaj", paaaaaaj__compi)
    };
    static DefineTop ListTop[4] = {
        DefineTop("aaaaaaar",0, 1, &saaaaaad__compi),
        DefineTop("aaaaaaas",-77, -77, &saaaaaae__compi),
        DefineTop("aaaaaaaz",-77, -77, &saaaaaaj__compi),
        DefineTop("gaaaaaag",2, 3, &gaaaaaag__compi)
    };
    static DefineGraph ListGraf[4] = {
        DefineGraph(1, 2),
        DefineGraph(0, 1),
        DefineGraph(1, 2),
        DefineGraph(0, 0)
    };
    int Ntop =4;
    int jroot = 3;

    GraphMV(p,jroot,Ntop,ListPred,ListTop,ListGraf);
    return(1);
}

void GraphCompiler::compile()
{
    static int FL=0;
    static int FromTop=0;
    static int FromLst=0;
    static int ArcType=0;
    static int ToTop=0;
    static int NTop=0;
    static int int1=1;
    static int Pred=0;
    static int NPred=0;
    static int Nobj=0;
    static char cR='R';
    static int I=0;
    static CharS c0=NULL;
    static int intM1=-1;
    static char cW='W';
    static int HeadTop=0;
    static int HeadGR=0;
    static int HeadGH=0;
    static COMPHs Graph=NULL;
    static int MaxLP=0;
    static int MaxLT=0;
    static int MaxGH=6;
    static GRHERR Gerror={0,0,{0}};
    static COMPREs ListP=NULL;
    static DEFGRAFs LstGraf=NULL;
    static COMPTOPs ListT=NULL;
    static NAMAGR NameAgr="gaaaaaaa";
    GraphPaths outputSetting = GraphPaths("D:\\TEMP\\", "D:\\TEMP\\", "D:\\TEMP\\", "COMPI", "gaaaaaae", 0);

    VAR listvars[] = {
        {"",(void*)28},
        {"FL",&FL},
        {"FromTop",&FromTop},
        {"FromLst",&FromLst},
        {"ArcType",&ArcType},
        {"ToTop",&ToTop},
        {"NTop",&NTop},
        {"int1",&int1},
        {"Pred",&Pred},
        {"NPred",&NPred},
        {"Nobj",&Nobj},
        {"cR",&cR},
        {"I",&I},
        {"c0",&c0},
        {"intM1",&intM1},
        {"cW",&cW},
        {"HeadTop",&HeadTop},
        {"HeadGR",&HeadGR},
        {"HeadGH",&HeadGH},
        {"Graph",&Graph},
        {"MaxLP",&MaxLP},
        {"MaxLT",&MaxLT},
        {"MaxGH",&MaxGH},
        {"Gerror",&Gerror},
        {"ListP",&ListP},
        {"LstGraf",&LstGraf},
        {"ListT",&ListT},
        {"NameAgr",&NameAgr},
        {"GrfNav",&outputSetting},
    };

    static void * _aaaaaaar[]={&outputSetting,&NameAgr,&ListT,&LstGraf,&ListP,&Gerror,&MaxGH,&MaxLT,&MaxLP};
    static void * _aaaaaaas[]={&ListT,&LstGraf,&ListP,&Graph};
    static void * _aaaaaaaz[]={&outputSetting,&Gerror};
    static void * _aaaaaaay[]={&outputSetting,&ListT,&LstGraf,&ListP,&Graph,&MaxGH,&MaxLT,&MaxLP,&Gerror};
    static void * _aaaaaaad[]={(void*)0};
    static void * _aaaaaaag[]={&HeadGH};
    static void * _aaaaaaap[]={&HeadGR};
    static void * _aaaaaaaq[]={&HeadTop,&ListT,&cW,&intM1,&c0,&intM1,&HeadGR};
    static void * _aaaaaaaf[]={&HeadGR};
    static void * _aaaaaaah[]={&HeadTop};
    static void * _aaaaaaaa[]={(void*)0};
    static void * _aaaaaaaw[]={&HeadTop,&ListT,&cW,&intM1,&c0,&HeadGR,&intM1};
    static void * _aaaaaaaj[]={&I};
    static void * _aaaaaaan[]={&I,&ListP,&cR,&Nobj,&c0};
    static void * _aaaaaaai[]={&I};
    static void * _aaaaaaak[]={&NPred,&I};
    static void * _paaaaaai[]={&Nobj,&Pred};
    static void * _paaaaaaf[]={(void*)0};
    static void * _gaaaaaab[]={_aaaaaaaj,_aaaaaaan,_aaaaaaai,_aaaaaaak,_paaaaaai,_paaaaaaf};
    static void * _aaaaaaam[]={&I,&ListT,&cR,&Nobj,&c0,&int1,&int1};
    static void * _aaaaaaal[]={&NTop,&I};
    static void * _paaaaaah[]={&Nobj,&ToTop};
    static void * _gaaaaaaa[]={_aaaaaaaj,_aaaaaaam,_aaaaaaai,_aaaaaaal,_paaaaaah,_paaaaaaf};
    static void * _aaaaaabc[]={&HeadGH,&LstGraf,&cW,&NPred,&NTop,&ArcType};
    static void * _aaaaaaab[]={&FromLst,&FromTop};
    static void * _aaaaaabe[]={&LstGraf,&HeadGH,&Graph,&Graph};
    static void * _gaaaaaac[]={_gaaaaaab,_gaaaaaaa,_aaaaaabc,_aaaaaaaf,_aaaaaaab,_aaaaaabe,_paaaaaaf};
    static void * _aaaaaaae[]={&FL};
    static void * _gaaaaaad[]={_aaaaaaab,_gaaaaaac,_aaaaaaag,_aaaaaaae,_paaaaaaf};
    static void * _gaaaaaae[]={_aaaaaaap,_aaaaaaaq,_paaaaaaf};
    static void * _aaaaaabb[]={&HeadGH,&Graph,&cR,&FromTop,&ToTop,&Pred,&ArcType};
    static void * _aaaaaaba[]={&MaxGH,&ListT};
    static void * _paaaaaaj[]={&Gerror};
    static void * _paaaaaab[]={&FL};
    static void * _paaaaaaa[]={&FL};
    static void * _paaaaaad[]={&HeadGH,&MaxGH};
    static void * _paaaaaac[]={&FromLst,&FromTop};
    static void * _paaaaaak[]={&HeadTop,&Graph,&MaxGH};
    static void * _paaaaaal[]={&HeadTop,&MaxGH};
    static void * _gaaaaaag[]={_aaaaaaay,_aaaaaaaz,_aaaaaaad,_aaaaaaag,_aaaaaaap,_aaaaaaaq,_aaaaaaaf,_aaaaaaah,_aaaaaaaa,_aaaaaaaw,_gaaaaaac,_gaaaaaad,_gaaaaaae,_aaaaaabb,_aaaaaaba,_paaaaaaj,_paaaaaaf,_paaaaaab,_paaaaaaa,_paaaaaad,_paaaaaac,_paaaaaak,_paaaaaal};
    static void * _gaaaaaaf[]={_aaaaaaar,_aaaaaaas,_aaaaaaaz,_gaaaaaag,_paaaaaaf,_paaaaaaj};

    //Инициализация переменных
    FL=0;
    FromTop=0;
    FromLst=0;
    ArcType=0;
    ToTop=0;
    NTop=0;
    int1=1;
    Pred=0;
    NPred=0;
    Nobj=0;
    cR='R';
    I=0;
    c0 = (char *)calloc(9,sizeof(char));    (*c0)='\0';
    intM1=-1;
    cW='W';
    HeadTop=0;
    HeadGR=0;
    HeadGH=0;
    Graph=NULL;
    MaxLP=0;
    MaxLT=0;
    MaxGH=6;
    Gerror.TekErr=0;
    Gerror.MaxErr=0;
    Gerror.CodErr[0]=0;
    ListP=NULL;
    LstGraf=NULL;
    ListT=NULL;


    strcpy(NameAgr, myGraphName.toStdString().c_str());
    QSettings settings("graph.ini", QSettings::IniFormat);
    outputSetting.outDir = settings.value("Location/OutputDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString();
    outputSetting.poDir = settings.value("Location/OutputDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString();
    outputSetting.iconDir = settings.value("Location/OutputDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString();
    outputSetting.NamePO = "COMPI";
    outputSetting.namepr = myGraphName;
    gaaaaaaf__compi(_gaaaaaaf);
}

GraphCompiler::GraphCompiler(const QString &graphName)
{
    myGraphName = graphName;
}

DefineGraph::DefineGraph(int NambPred, int NambTop)
{
    this->NambPred = NambPred;
    this->NambTop = NambTop;
    this->ArcType = 0;
}

DefineGraph::DefineGraph(int NambPred, int NambTop, int ArcType)
{
    this->NambPred = NambPred;
    this->NambTop = NambTop;
    this->ArcType = ArcType;
}

GraphPaths::GraphPaths(const QString &poDir, const QString &iconDir, const QString &outDir, const QString &NamePO, const QString &namepr, int root)
{
    this->poDir = poDir;
    this->iconDir = iconDir;
    this->outDir = outDir;
    this->NamePO = NamePO;
    this->namepr = namepr;
    this->root = root;
}

DefineTop::DefineTop(const QString &NameTop, int FirstDef, int LastDef, int (*Top)(void *[]))
{
    this->NameTop = NameTop;
    this->FirstDef = FirstDef;
    this->LastDef = LastDef;
    this->Top = Top;
}

DefinePredicate::DefinePredicate(const QString &name, int (*predicate)(void *[]))
{
    this->name = name;
    this->predicate = predicate;
}
