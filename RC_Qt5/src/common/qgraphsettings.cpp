#include "qgraphsettings.h"
#include "globalvariables.h"
#include <QtCore>


QGraphSettings *globalSettings;

QGraphSettings::QGraphSettings(const QString &configPath)
    : ini(configPath, QSettings::IniFormat)
{
    myIsParallelSet = false;
    myIsParallel = false;

    myOutputDirectorySet = false;
    myOutputDirectory = "out";

    myBaseDirectorySet = false;
    myBaseDirectory = "src";

    myTemplateDirectorySet = false;
    myTemplateDirectory = "./templates/";

    myProjectsDirectorySet = false;
    myProjectsDirectory = "./projects/";
}

QString QGraphSettings::getOutputDirectory()
{
    if (myOutputDirectorySet == false) {
        const QString outputDirectory = getProjectsDirectory()
                + "/" + globalDBManager->getProjectName()
                + "/" + ini.value("Folders/projects", myOutputDirectory).toString();
        QDir dir(outputDirectory);
        if (!dir.exists())
            dir.mkpath(".");
        myOutputDirectory = dir.canonicalPath();
        myOutputDirectorySet = true;
    }
    return myOutputDirectory;
}

QString QGraphSettings::getBaseDirectory()
{
    if (myBaseDirectorySet == false) {
        const QString baseDirectory = getProjectsDirectory()
                + "/" + globalDBManager->getProjectName()
                + "/" + ini.value("Folders/source", myBaseDirectory).toString();
        QDir dir(baseDirectory);
        if (!dir.exists())
            dir.mkpath(".");
        myBaseDirectory = dir.canonicalPath();
        myBaseDirectorySet = true;
    }
    return myBaseDirectory;
}

QString QGraphSettings::getTemplateDirectory()
{
    if (myTemplateDirectorySet == false) {
        myTemplateDirectory = ini.value("Folders/templates", myTemplateDirectory).toString();
        QDir dir(myTemplateDirectory);
//        Q_ASSERT(dir.exists());
        myTemplateDirectory = dir.canonicalPath();
        myTemplateDirectorySet = true;
    }
    return myTemplateDirectory;
}

QString QGraphSettings::getProjectsDirectory()
{
    if (myProjectsDirectorySet == false) {
        myProjectsDirectory = ini.value("Folders/projects", myProjectsDirectory).toString();
        QDir dir(myProjectsDirectory);
        if (!dir.exists())
            dir.mkpath(".");
        myProjectsDirectory = dir.canonicalPath();
        myProjectsDirectorySet = true;
    }
    return myProjectsDirectory;
}

bool QGraphSettings::isParallel()
{
    if (myIsParallelSet == false) {
        myIsParallel = ini.value("Compiler/Parallel", false).toBool();
        myIsParallelSet = true;
    }
    return myIsParallel;
}

