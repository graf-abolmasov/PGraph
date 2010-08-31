#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#include "databasemanager.h"
#include "logger.h"

extern DataBaseManager* globalDBManager;
extern Logger *globalLogger;
extern QStatusBar *globalStatusBar;
extern QStringList globalPredicateList;

#endif // GLOBALVARIABLES_H
