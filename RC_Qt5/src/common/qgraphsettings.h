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

    bool myProjectsDirectorySet;
    QString myProjectsDirectory;

    QString myConfigPath;
public:
    QString getOutputDirectory();
    QString getBaseDirectory();
    QString getTemplateDirectory();
    QString getProjectsDirectory();

    bool isParallel();
    QString getConfigPath() { return myConfigPath; }
    QGraphSettings(const QString &configPath);
};

#endif // QGRAPHSETTINGS_H
