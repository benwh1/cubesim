QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    colorsettingwidget.cpp \
    cube.cpp \
    cubegraphicsobject.cpp \
    cubegraphicsview.cpp \
    cubestate.cpp \
    cubewidget.cpp \
    ffmpegprocess.cpp \
    main.cpp \
    mainwindow.cpp \
    move.cpp \
    projection.cpp \
    reconstruction.cpp \
    reconstructionwindow.cpp \
    replayrecorder.cpp \
    replayrecordersettings.cpp \
    replayrecordersettingswidget.cpp \
    replayrecorderwindow.cpp \
    settings.cpp \
    settingswindow.cpp \
    statistics.cpp \
    statisticswidget.cpp \
    sticker.cpp

HEADERS += \
    colorsettingwidget.h \
    cube.h \
    cubegraphicsobject.h \
    cubegraphicsview.h \
    cubestate.h \
    cubewidget.h \
    enums.h \
    ffmpegprocess.h \
    formatting.h \
    global.h \
    mainwindow.h \
    move.h \
    projection.h \
    reconstruction.h \
    reconstructionwindow.h \
    replayrecorder.h \
    replayrecordersettings.h \
    replayrecordersettingswidget.h \
    replayrecorderwindow.h \
    saveconverter.h \
    settings.h \
    settingswindow.h \
    statistics.h \
    statisticswidget.h \
    sticker.h

FORMS += \
    colorsettingwidget.ui \
    cubewidget.ui \
    mainwindow.ui \
    reconstructionwindow.ui \
    replayrecordersettingswidget.ui \
    replayrecorderwindow.ui \
    settingswindow.ui \
    statisticswidget.ui

TRANSLATIONS += \
    cubesim_en_GB.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
