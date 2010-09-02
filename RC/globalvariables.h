#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#include "databasemanager.h"
#include "logger.h"

extern DataBaseManager* globalDBManager;
extern Logger *globalLogger;
extern QLabel* globalInfoLabel;
extern QStringList globalPredicateList;

#endif // GLOBALVARIABLES_H
