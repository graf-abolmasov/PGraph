#ifndef QGRAPHSETTINGS_H
#define QGRAPHSETTINGS_H

#include <QSettings>
#include <QString>
#include "globalvariables.h"

class QGraphSettings
{
private:
    static bool myIsParallel;
    static bool myIsParallelSet;

    static bool myOutputDirectorySet;
    static QString myOutputDirectory;

    static bool myBaseDirectorySet;
    static QString myBaseDirectory;

    static bool myTemplateDirectorySet;
    static QString myTemplateDirectory;
public:
    static QString getOutputDirectory();
    static QString getBaseDirectory();
    static QString getTemplateDirectory();

    static bool isParallel();
};

#endif // QGRAPHSETTINGS_H
