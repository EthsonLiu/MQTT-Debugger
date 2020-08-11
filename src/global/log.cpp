#include "src/global/log.h"
#include "src/global/project.h"

#include <QMutexLocker>
#include <QDateTime>
#include <QTextStream>

QFile Log::m_logFile;
QMutex Log::m_mutex;

void Log::init()
{
    m_logFile.setFileName(Project::applicationDirPath() + "/log/log.txt");
    m_logFile.open(QIODevice::WriteOnly | QIODevice::Append);
}

void Log::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (m_logFile.isOpen() == false)
        return;

    QMutexLocker locker(&m_mutex);

    if (needSplitFile())
        splitFile();

    QString logText;
    logText.reserve(256);

    logText += QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss zzz] ");

    switch (type)
    {
    case QtWarningMsg:
    {
        logText += "[Warning] ";
        break;
    }
    case QtCriticalMsg:
    {
        logText += "[Critical] ";
        break;
    }
    default:
    {
        logText += "[Debug] ";
        break;
    }
    }

    QString fileName = context.file;
    for (int i = fileName.length() - 1; i >= 0; --i)
    {
        if (fileName[i] == '\\' || fileName[i] == '/')
            fileName = fileName.mid(i + 1);
    }

    logText += QString("[%1:%2] ").arg(fileName).arg(context.line);
    logText += msg;

    QTextStream stream(&m_logFile);
    stream << logText << "\n";
    m_logFile.flush();
}

bool Log::needSplitFile()
{
    return false;
}

void Log::splitFile()
{

}
