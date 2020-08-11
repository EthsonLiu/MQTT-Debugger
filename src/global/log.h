#ifndef MQTT_DEBUGGER_SRC_LOG_H
#define MQTT_DEBUGGER_SRC_LOG_H

#include <QString>
#include <QMutex>
#include <QFile>

class Log
{
public:
    static void init();
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static bool needSplitFile();
    static void splitFile();

private:
    static QMutex m_mutex;
    static QFile  m_logFile;
};

#endif /** MQTT_DEBUGGER_SRC_LOG_H */
