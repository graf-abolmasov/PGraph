#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#include "databasemanager.h"
#include "logger.h"

extern DataBaseManager* globalDBManager;
extern Logger *globalLogger;
extern QStringList globalPredicateList;
extern QStatusBar *globalStatusBar;

#endif // GLOBALVARIABLES_H
