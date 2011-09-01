#include "qgraphsettings.h"
#include <QtCore>

QString QGraphSettings::getOutputDirectory()
{
    QSettings c("graph.ini", QSettings::IniFormat);
    return QFileInfo(c.value("Location/OutputDir", "./Out").toString()).canonicalFilePath();
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
