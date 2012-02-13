#include "qgraphsettings.h"
#include <QtCore>

QString QGraphSettings::getOutputDirectory()
{
    QSettings c("graph.ini", QSettings::IniFormat);
    const QString outputDirectory = c.value("Location/OutputDir", "./Out").toString() + "/" + globalDBManager->getProjectName();
    if (!QDir(outputDirectory).exists())
        QDir().mkpath(outputDirectory);
    return QFileInfo(outputDirectory).canonicalFilePath();
}

QString QGraphSettings::getBaseDirectory()
{
    QSettings c("graph.ini", QSettings::IniFormat);
    const QString baseDirectory = c.value("Location/BaseDir", "./BaseDir").toString() + "/" + globalDBManager->getProjectName();
    if (!QDir(baseDirectory).exists())
        QDir().mkpath(baseDirectory);
    return QFileInfo(baseDirectory).canonicalFilePath();
}

QString QGraphSettings::getTemplateDirectory()
{
    QSettings c("graph.ini", QSettings::IniFormat);
    return QFileInfo(c.value("Location/TemplateDir", "./Templates").toString()).canonicalFilePath();
}

bool QGraphSettings::isParallel()
{
    QSettings c("graph.ini", QSettings::IniFormat);
    return c.value("Compiler/Parallel", false).toBool();
}

