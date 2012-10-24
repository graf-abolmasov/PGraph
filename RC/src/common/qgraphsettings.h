#ifndef QGRAPHSETTINGS_H
#define QGRAPHSETTINGS_H

#include <QSettings>
#include <QString>

class QGraphSettings
{
private:
    bool myIsParallel;
    bool myIsParallelSet;

    bool myOutputDirectorySet;
    QString myOutputDirectory;

    bool myBaseDirectorySet;
    QString myBaseDirectory;

    bool myTemplateDirectorySet;
    QString myTemplateDirectory;

    QString myConfigPath;
public:
    QString getOutputDirectory();
    QString getBaseDirectory();
    QString getTemplateDirectory();

    bool isParallel();
    QString getConfigPath() { return myConfigPath; }
    QGraphSettings(const QString &configPath);
};

#endif // QGRAPHSETTINGS_H
