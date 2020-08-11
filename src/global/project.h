#ifndef MQTT_DEBUGGER_SRC_PROJECT_H
#define MQTT_DEBUGGER_SRC_PROJECT_H

#include <QString>

class Project
{
public:
    static void init();
    static QString applicationDirPath();
    static QString applicationVersion();

private:
    static QString m_applicationDirPath;
    static QString m_applicationVersion;
};

#endif /** MQTT_DEBUGGER_SRC_PROJECT_H */
