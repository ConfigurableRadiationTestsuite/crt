QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE = -O2

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS DEBUG DUMMY_DATA

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L/usr/local/lib -lLabJackM

SOURCES += \
    main.cpp \
    src/Components/Component.cpp \
    src/Components/DIOM.cpp \
    src/Components/Ethernet.cpp \
    src/Components/EthernetClient.cpp \
    src/Components/Indicator.cpp \
    src/Components/Labjack.cpp \
    src/Components/LabjackChannel.cpp \
    src/Components/PSU.cpp \
    src/Components/PSUChannel.cpp \
    src/Components/ProgrammStarter.cpp \
    src/Components/RFIO.cpp \
    src/Components/RFIOChannel.cpp \
    src/Components/RFIOUpdater.cpp \
    src/Dialog/SpecAddDialog.cpp \
    src/Dialog/SpecSignalDialog.cpp \
    src/Manager/EventManager.cpp \
    src/Manager/FileManager.cpp \
    src/Manager/RunManager.cpp \
    src/Plot/LBJPlot.cpp \
    src/Plot/PSUPlot.cpp \
    src/Plot/Plot.cpp \
    src/Plot/RFPlot.cpp \
    src/Plot/qcustomplot.cpp \
    src/SubWindow/DIOW.cpp \
    src/SubWindow/ETHW.cpp \
    src/SubWindow/LBJW.cpp \
    src/SubWindow/PROGW.cpp \
    src/SubWindow/PSUW.cpp \
    src/SubWindow/RFW.cpp \
    src/Tabs/ETHTab.cpp \
    src/Tabs/LBJTab.cpp \
    src/Tabs/PROGTab.cpp \
    src/Tabs/PSUTab.cpp \
    src/Tabs/RFTab.cpp \
    src/mainwindow.cpp \
    src/Configuration/ConfigElement.cpp \
    src/Configuration/ConfigManager.cpp \
    src/SubWindow/SubWindow.cpp \
    src/Tabs/WindowTab.cpp \
    src/mainlayout.cpp \

HEADERS += \
    LJM_Utilities.h \
    src/Components/Component.h \
    src/Components/DIOM.h \
    src/Components/Ethernet.h \
    src/Components/EthernetClient.h \
    src/Components/Indicator.h \
    src/Components/Labjack.h \
    src/Components/LabjackChannel.h \
    src/Components/PSU.h \
    src/Components/PSUChannel.h \
    src/Components/ProgrammStarter.h \
    src/Components/RFIO.h \
    src/Components/RFIOChannel.h \
    src/Components/RFIOUpdater.h \
    src/Dialog/SpecAddDialog.h \
    src/Dialog/SpecSignalDialog.h \
    src/Manager/EventManager.h \
    src/Manager/FileManager.h \
    src/Manager/RunManager.h \
    src/Plot/LBJPlot.h \
    src/Plot/PSUPlot.h \
    src/Plot/Plot.h \
    src/Plot/RFPlot.h \
    src/Plot/qcustomplot.h \
    src/SubWindow/DIOW.h \
    src/SubWindow/ETHW.h \
    src/SubWindow/LBJW.h \
    src/SubWindow/PROGW.h \
    src/SubWindow/PSUW.h \
    src/SubWindow/RFW.h \
    src/Tabs/ETHTab.h \
    src/Tabs/LBJTab.h \
    src/Tabs/PROGTab.h \
    src/Tabs/PSUTab.h \
    src/Tabs/RFTab.h \
    src/mainwindow.h \
    src/Configuration/ConfigElement.h \
    src/Configuration/ConfigManager.h \
    src/SubWindow/SubWindow.h \
    src/Tabs/WindowTab.h \
    src/mainlayout.h \
    src/mainwindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
