#ifndef QGRAPHSETTINGS_H
#define QGRAPHSETTINGS_H

#include <QSettings>
#include <QString>
#include "globalvariables.h"

class QGraphSettings
{
public:
    static QString getOutputDirectory();
    static QString getBaseDirectory();
    static QString getTemplateDirectory();

    static bool isParallel();
};

#endif // QGRAPHSETTINGS_H
