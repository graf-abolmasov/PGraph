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
    return QFileInfo(c.value("Location/BaseDir", "./BaseDir").toString()).canonicalFilePath();
}

QString QGraphSettings::getTemplateDirectory()
{
    QSettings c("graph.ini", QSettings::IniFormat);
    return QFileInfo(c.value("Location/TemplateDir", "./Templates").toString()).canonicalFilePath();
}
