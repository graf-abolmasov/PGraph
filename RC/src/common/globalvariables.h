#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "databasemanager.h"
#include "qgraphsettings.h"
#include "logger.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
QT_END_NAMESPACE

extern Graph            *globalClipboard;
extern Logger           *globalLogger;
extern QLabel           *globalInfoLabel;
extern QStringList      globalPredicateList;
extern DataBaseManager  *globalDBManager;
extern QGraphSettings   *globalSettings;

#endif // GLOBALVARIABLES_H
