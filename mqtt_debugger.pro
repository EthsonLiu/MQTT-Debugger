QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 'MQTT Debugger'
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH = third_party/include

LIBS += \
    -L$$PWD/third_party/lib -lmosquitto

SOURCES += \
    src/main.cpp \
    src/main_window.cpp \
    src/global/project.cpp \
    src/global/log.cpp

HEADERS += \
    src/main_window.h \
    src/global/project.h \
    src/config.h \
    src/global/log.h

RESOURCES += \
    image/image.qrc
