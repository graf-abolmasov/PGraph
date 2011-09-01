#ifndef QGRAPHSETTINGS_H
#define QGRAPHSETTINGS_H

#include <QSettings>
#include <QString>

class QGraphSettings
{
public:
    static QString getOutputDirectory();
    static QString getBaseDirectory();
    static QString getTemplateDirectory();
};

#endif // QGRAPHSETTINGS_H
