#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "databasemanager.h"
#include "logger.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
QT_END_NAMESPACE

extern Graph            *globalClipboard;
extern Logger           *globalLogger;
extern QLabel           *globalInfoLabel;
extern QListWidget      *globalOutput;
extern QStringList      globalPredicateList;
extern DataBaseManager  *globalDBManager;

#endif // GLOBALVARIABLES_H
