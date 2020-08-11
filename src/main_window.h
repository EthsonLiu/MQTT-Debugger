#ifndef MQTT_DEBUGGER_SRC_MAIN_WINDOW_H
#define MQTT_DEBUGGER_SRC_MAIN_WINDOW_H

#include "src/config.h"

#include <QMainWindow>
#include <QVector>
#include <QAction>
#include <QCloseEvent>
#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QMovie>
#include <QTreeWidget>
#include <QDockWidget>

enum class Language
{
    english,
    simplifiedChinese
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();
    virtual void closeEvent(QCloseEvent* event);

private:
    void createMenuBar();
    void createDockWidget();
    void createFileMenu();
    void createWindowMenu();
    void createLanguagesMenu();
    void createHelpMenu();
    void createToolBar();
    QWidget *createProfilesListWidget();
    QWidget *createSubscribeWidget();
    QWidget *createPublishWidget();
    QWidget *createLogWidget();
    QWidget *createBrokerStatusWidget();

private slots:
    void exitApplication();
    void changeLanguage(QAction *action);
    void showOptionsWidget();
    void reportBug();
    void showAboutWidget();
    void showUpdateWidget();
    void changeProfileNameLabelText(const QString &profileName);
    void setProfileSettingToolButtonEnabled(bool enabled);
    void setConnectPushButtonEnabled(bool enabled);
    void changeConnectPushButtonStatus();
    void changeResultLabelText(bool isSuccessResult, const QString &resultText);
    void showSubscribeWindow(bool checked);
    void showPublishWindow(bool checked);
    void showLogWindow(bool checked);
    void showBrokerStatusWindow(bool checked);
    void showProfilesListTreeWindow(bool checked);
    void openLogsDirectory();

private:
    QVector<QAction *> m_langActionsVec;
    Language           m_currentLanguage;

    QLabel            *m_profileNameLabel;
    QToolButton       *m_profileSettingToolButton;
    QPushButton       *m_connectPushButton;
    QLabel            *m_loadingLabel;
    QMovie            *m_loadingMovie;
    QLabel            *m_resultLabel;

    QDockWidget       *m_profilesListTreeDockWidget;
    QDockWidget       *m_subscribeDockWidget;
    QDockWidget       *m_publishDockWidget;
    QDockWidget       *m_logDockWidget;
    QDockWidget       *m_brokerStatusDockWidget;
    QTreeWidget       *m_profilesListTreeWidget;
};

#endif /** MQTT_DEBUGGER_SRC_MAIN_WINDOW_H */
