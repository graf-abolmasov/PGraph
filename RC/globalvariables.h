#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "databasemanager.h"
#include "logger.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

extern DataBaseManager *globalDBManager;
extern Logger *globalLogger;
extern QLabel *globalInfoLabel;
extern QStringList globalPredicateList;
extern Graph *globalClipboard;

#endif // GLOBALVARIABLES_H
