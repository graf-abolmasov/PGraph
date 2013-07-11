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

    QSettings ini;

    QString getProjectsDirectory();

public:
    QString getOutputDirectory();
    QString getBaseDirectory();
    QString getTemplateDirectory();

    bool isParallel();
    QString getConfigPath() {
        return ini.fileName();
    }
    QGraphSettings(const QString &configPath);
};

#endif // QGRAPHSETTINGS_H
