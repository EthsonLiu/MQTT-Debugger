#include "src/config.h"
#include "src/main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Project::init();
    Log::init();

    //qInstallMessageHandler(Log::messageOutput);

    MainWindow w;
    w.show();

    return a.exec();
}
