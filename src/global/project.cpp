#include "src/global/project.h"

#include <QCoreApplication>

QString Project::m_applicationDirPath = "";
QString Project::m_applicationVersion = "";


void Project::init()
{
    m_applicationDirPath = QCoreApplication::applicationDirPath();
    m_applicationVersion = "0.1";
}

QString Project::applicationDirPath()
{
    return m_applicationDirPath;
}

QString Project::applicationVersion()
{
    return m_applicationVersion;
}
