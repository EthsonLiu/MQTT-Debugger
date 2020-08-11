#include "main_window.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QString windowTitle = "MQTT Debugger";
    windowTitle += " ";
    windowTitle += Project::applicationVersion();

    setStyleSheet("QMainWindow::separator{"
    "background: red;"
    "width: 1px;}");

    setWindowTitle(windowTitle);
    setWindowState(Qt::WindowMaximized);
    createMenuBar();
    createToolBar();
    createDockWidget();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();

    event->accept();
}

void MainWindow::createMenuBar()
{
    setStyleSheet("QMenu{font-size:12px;}"
                  "QMenuBar{font-size:12px;background-color:rgb(220, 220, 220);}"
                  "QMenuBar::item:selected {color:white;background-color:rgb(122, 122, 122);}");

    createFileMenu();
    createWindowMenu();
    createLanguagesMenu();
    createHelpMenu();
}

void MainWindow::createDockWidget()
{
    setDockNestingEnabled(true);
    setStyleSheet("QDockWidget > QWidget {"
                  "border:1px solid red;");

    m_profilesListTreeDockWidget = new QDockWidget(tr("Profiles"), this);
    m_subscribeDockWidget        = new QDockWidget(tr("Subscribe"), this);
    m_publishDockWidget          = new QDockWidget(tr("Publish"), this);
    m_logDockWidget              = new QDockWidget(tr("Log"), this);
    m_brokerStatusDockWidget     = new QDockWidget(tr("Broker Status"), this);

    m_profilesListTreeDockWidget->setWidget(createProfilesListWidget());
    m_profilesListTreeDockWidget->setMaximumWidth(400);
    m_subscribeDockWidget->setWidget(createProfilesListWidget());
    m_publishDockWidget->setWidget(createProfilesListWidget());
    m_logDockWidget->setWidget(createProfilesListWidget());
    m_brokerStatusDockWidget->setWidget(createProfilesListWidget());

    m_profilesListTreeDockWidget->setFeatures(QDockWidget::DockWidgetClosable);
    m_subscribeDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_publishDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_logDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_brokerStatusDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);

    m_profilesListTreeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_subscribeDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    m_publishDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    m_logDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    m_brokerStatusDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

    addDockWidget(Qt::LeftDockWidgetArea, m_profilesListTreeDockWidget);
    splitDockWidget(m_profilesListTreeDockWidget, m_subscribeDockWidget, Qt::Horizontal);
    splitDockWidget(m_subscribeDockWidget, m_logDockWidget, Qt::Horizontal);
    splitDockWidget(m_subscribeDockWidget, m_publishDockWidget, Qt::Vertical);
    tabifyDockWidget(m_logDockWidget, m_brokerStatusDockWidget);
}

void MainWindow::createFileMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    fileMenu->addAction(tr("Open Logs Directory..."), this, &MainWindow::openLogsDirectory);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Options..."), this, &MainWindow::showOptionsWidget);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), this, &MainWindow::exitApplication, QKeySequence(Qt::CTRL + Qt::Key_Q));
}

void MainWindow::createWindowMenu()
{
    QMenu *windowMenu = menuBar()->addMenu(tr("Window"));

    QAction *treeAction   = windowMenu->addAction(tr("Show Profiles Window"));
    QAction *separator1   = windowMenu->addSeparator();
    QAction *subAction    = windowMenu->addAction(tr("Show Subscribe Window"));
    QAction *pubAction    = windowMenu->addAction(tr("Show Publish Window"));
    QAction *separator2   = windowMenu->addSeparator();
    QAction *logAction    = windowMenu->addAction(tr("Show Log Window"));
    QAction *separator3   = windowMenu->addSeparator();
    QAction *brokerAction = windowMenu->addAction(tr("Show Broker Status Window"));

    Q_UNUSED(separator1);
    Q_UNUSED(separator2);
    Q_UNUSED(separator3);

    treeAction->setIcon(QIcon(":/list_tree.svg"));
    logAction->setIcon(QIcon(":/log.svg"));
    brokerAction->setIcon(QIcon(":/statistic.svg"));

    treeAction->setCheckable(true);
    subAction->setCheckable(true);
    pubAction->setCheckable(true);
    logAction->setCheckable(true);
    brokerAction->setCheckable(true);

    connect(treeAction,   &QAction::triggered, this, &MainWindow::showProfilesListTreeWindow);
    connect(subAction,    &QAction::triggered, this, &MainWindow::showSubscribeWindow);
    connect(pubAction,    &QAction::triggered, this, &MainWindow::showPublishWindow);
    connect(logAction,    &QAction::triggered, this, &MainWindow::showLogWindow);
    connect(brokerAction, &QAction::triggered, this, &MainWindow::showBrokerStatusWindow);
}

void MainWindow::createLanguagesMenu()
{
    QMenu *langMenu = menuBar()->addMenu(tr("Languages"));

    QAction *systemLanguageAction    = langMenu->addAction("<System Language>");
    QAction *englishAction           = langMenu->addAction("English");
    QAction *simplifiedChineseAction = langMenu->addAction(QString::fromLocal8Bit("简体中文"));

    systemLanguageAction->setCheckable(true);
    systemLanguageAction->setChecked(true);

    m_langActionsVec.push_back(systemLanguageAction);
    m_langActionsVec.push_back(englishAction);
    m_langActionsVec.push_back(simplifiedChineseAction);

    QActionGroup* actionsGroup = new QActionGroup(langMenu);
    actionsGroup->setExclusive(true);
    actionsGroup->addAction(systemLanguageAction);
    actionsGroup->addAction(englishAction);
    actionsGroup->addAction(simplifiedChineseAction);

    connect(actionsGroup, &QActionGroup::triggered, this, &MainWindow::changeLanguage);
}

void MainWindow::createHelpMenu()
{
    QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("Report Bug"), this, &MainWindow::reportBug);
    helpMenu->addSeparator();
    helpMenu->addAction(QIcon(":/update.svg"), tr("Check for New Version..."), this, &MainWindow::showUpdateWidget);
    helpMenu->addSeparator();
    helpMenu->addAction(tr("About MQTT Debugger"), this, &MainWindow::showAboutWidget);
}

void MainWindow::createToolBar()
{
    QToolBar* toolBar = addToolBar(tr("Tool"));
    toolBar->setMovable(false);

    QWidget *toolBarCentralWidget = new QWidget(toolBar);
    QHBoxLayout *toolBarCentralWidgetHLayout = new QHBoxLayout(toolBarCentralWidget);

    QWidget *subWidget = new QWidget(toolBar);
    QHBoxLayout *subWidgetHLayout = new QHBoxLayout(subWidget);

    m_profileNameLabel = new QLabel(toolBarCentralWidget);
    m_profileSettingToolButton = new QToolButton(subWidget);
    m_profileSettingToolButton->setIcon(QIcon(":/setting.svg"));
    m_profileSettingToolButton->setToolTip("Configure the selected connection profile");
    m_connectPushButton = new QPushButton(subWidget);
    m_connectPushButton->setText(tr("Connect"));
    m_connectPushButton->setToolTip("Connect to the selected broker");
    m_connectPushButton->setStyleSheet("");
    m_connectPushButton->setFixedWidth(70);
    m_loadingLabel = new QLabel(subWidget);
    m_loadingMovie = new QMovie(":/loading.gif", QByteArray(), m_loadingLabel);
    m_loadingMovie->setScaledSize(QSize(40, 40));
    m_loadingLabel->setMovie(m_loadingMovie);
    m_loadingLabel->setFixedSize(40, 40);
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    m_loadingMovie->start();
    m_resultLabel = new QLabel(subWidget);

    subWidgetHLayout->addWidget(m_profileSettingToolButton);
    subWidgetHLayout->addWidget(m_connectPushButton);
    subWidgetHLayout->addWidget(m_loadingLabel);
    subWidgetHLayout->addWidget(m_resultLabel);
    subWidgetHLayout->setMargin(0);

    toolBarCentralWidgetHLayout->addWidget(m_profileNameLabel, 1);
    toolBarCentralWidgetHLayout->addWidget(subWidget, 4);
    toolBarCentralWidgetHLayout->setMargin(0);

    toolBar->addWidget(toolBarCentralWidget);
}

QWidget *MainWindow::createProfilesListWidget()
{
    QWidget *widget = new QWidget(this);

    m_profilesListTreeWidget = new QTreeWidget(widget);
    m_profilesListTreeWidget->setHeaderHidden(true);
    m_profilesListTreeWidget->setStyleSheet("QTreeWidget{show-decoration-selected:1;}");

    QTreeWidgetItem* searchItem = new QTreeWidgetItem(m_profilesListTreeWidget, QStringList(tr("Searching")));

    QPushButton *addPushButton    = new QPushButton(widget);
    QPushButton *removePushButton = new QPushButton(widget);
    addPushButton->setIcon(QIcon(":/add.svg"));
    removePushButton->setIcon(QIcon(":/remove.svg"));

    QVBoxLayout *vLayout = new QVBoxLayout(widget);
    QHBoxLayout *hLayout = new QHBoxLayout;

    hLayout->addStretch();
    hLayout->addWidget(addPushButton);
    hLayout->addWidget(removePushButton);
    vLayout->addWidget(m_profilesListTreeWidget);
    vLayout->addLayout(hLayout);

    return widget;
}

QWidget *MainWindow::createSubscribeWidget()
{
    QWidget *widget = new QWidget(this);
    return widget;
}

QWidget *MainWindow::createPublishWidget()
{
    QWidget *widget = new QWidget(this);
    return widget;
}

QWidget *MainWindow::createLogWidget()
{
    QWidget *widget = new QWidget(this);
    return widget;
}

QWidget *MainWindow::createBrokerStatusWidget()
{
    QWidget *widget = new QWidget(this);
    return widget;
}

void MainWindow::exitApplication()
{
    close();
}

void MainWindow::changeLanguage(QAction *action)
{
    action->setCheckable(true);
    action->setChecked(true);

    Language language = static_cast<Language>(m_langActionsVec.indexOf(action));

    if (m_currentLanguage != language)
    {
        m_currentLanguage = language;

        QMessageBox box(QMessageBox::Information, tr("Restart Required"), tr("The language change will take effect after restart."));
        box.setStandardButtons(QMessageBox::Ok);
        box.setButtonText(QMessageBox::Ok, tr("Ok"));
        box.exec();
    }
}

void MainWindow::showOptionsWidget()
{

}

void MainWindow::reportBug()
{

}

void MainWindow::showAboutWidget()
{

}

void MainWindow::showUpdateWidget()
{

}

void MainWindow::changeProfileNameLabelText(const QString &profileName)
{

}

void MainWindow::setProfileSettingToolButtonEnabled(bool enabled)
{

}

void MainWindow::setConnectPushButtonEnabled(bool enabled)
{

}

void MainWindow::changeConnectPushButtonStatus()
{

}

void MainWindow::changeResultLabelText(bool isSuccessResult, const QString &resultText)
{

}

void MainWindow::showSubscribeWindow(bool checked)
{

}

void MainWindow::showPublishWindow(bool checked)
{
}

void MainWindow::showLogWindow(bool checked)
{

}

void MainWindow::showBrokerStatusWindow(bool checked)
{

}

void MainWindow::showProfilesListTreeWindow(bool checked)
{

}

void MainWindow::openLogsDirectory()
{

}
