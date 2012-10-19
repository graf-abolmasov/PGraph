#include "qgraphsettings.h"
#include <QtCore>

bool QGraphSettings::myIsParallelSet = false;
bool QGraphSettings::myIsParallel = false;

bool QGraphSettings::myOutputDirectorySet = false;
QString QGraphSettings::myOutputDirectory = "./Out";

bool QGraphSettings::myBaseDirectorySet = false;
QString QGraphSettings::myBaseDirectory = "./C";

bool QGraphSettings::myTemplateDirectorySet = false;
QString QGraphSettings::myTemplateDirectory = "./Templates";

QString QGraphSettings::getOutputDirectory()
{
    if (myOutputDirectorySet == false) {
        QSettings c("graph.ini", QSettings::IniFormat);
        const QString outputDirectory = "projects/" + globalDBManager->getProjectName() + "/" + c.value("Location/OutputDir", myOutputDirectory).toString();
        if (!QDir(outputDirectory).exists())
            QDir().mkpath(outputDirectory);
        myOutputDirectory = QFileInfo(outputDirectory).canonicalFilePath();
        myOutputDirectorySet = true;
    }
    return myOutputDirectory;
}

QString QGraphSettings::getBaseDirectory()
{
    if (myBaseDirectorySet == false) {
        QSettings c("graph.ini", QSettings::IniFormat);
        const QString baseDirectory = "projects/" + globalDBManager->getProjectName() + "/" + c.value("Location/BaseDir", myBaseDirectory).toString();
        if (!QDir(baseDirectory).exists())
            QDir().mkpath(baseDirectory);
        myBaseDirectory = QFileInfo(baseDirectory).canonicalFilePath();
        myBaseDirectorySet = true;
    }
    return myBaseDirectory;
}

QString QGraphSettings::getTemplateDirectory()
{
    if (myTemplateDirectorySet == false) {
        QSettings c("graph.ini", QSettings::IniFormat);
        myTemplateDirectory = QFileInfo(c.value("Location/TemplateDir", myTemplateDirectory).toString()).canonicalFilePath();
        myTemplateDirectorySet = true;
    }
    return myTemplateDirectory;
}

bool QGraphSettings::isParallel()
{
    if (myIsParallelSet == false) {
        QSettings c("graph.ini", QSettings::IniFormat);
        myIsParallel = c.value("Compiler/Parallel", false).toBool();
        myIsParallelSet = true;
    }
    return myIsParallel;
}

