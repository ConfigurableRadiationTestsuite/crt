QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE = -O2

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS DEBUG
#DEFINES += DUMMY_DATA

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:LIBS += -L/usr/local/lib -lLabJackM -llxi -ltirpc

win32:INCLUDEPATH += 'C:/Program Files (x86)/LabJack/Drivers'
win32:LIBS += -L'C:/Program Files (x86)/LabJack/Drivers/64bit/' -lLabJackM
win32:DEPENDPATH += 'C:/Program Files (x86)/LabJack/Drivers/64bit'

SOURCES += \
    main.cpp \
    src/Components/Component.cpp \
    src/Components/DIOM.cpp \
    src/Components/Ethernet.cpp \
    src/Components/EthernetClient.cpp \
    src/Components/Fieldfox.cpp \
    src/Components/IQSample.cpp \
    src/Components/IQVector.cpp \
    src/Components/IndicatorBox.cpp \
    src/Components/IndicatorIcon.cpp \
    src/Components/LXIClient.cpp \
    src/Components/Labjack.cpp \
    src/Components/LabjackChannel.cpp \
    src/Components/OSC.cpp \
    src/Components/OSCChannel.cpp \
    src/Components/PSU.cpp \
    src/Components/PSUChannel.cpp \
    src/Components/ProgrammStarter.cpp \
    src/Components/RFIO.cpp \
    src/Components/RFIOChannel.cpp \
    src/Components/Sequencer.cpp \
    src/Dialog/SpecAddDialog.cpp \
    src/Dialog/SpecSignalDialog.cpp \
    src/Dialog/TaskDialog.cpp \
    src/Manager/EventManager.cpp \
    src/Manager/FileManager.cpp \
    src/Manager/RunManager.cpp \
    src/Plot/FFOXPlot.cpp \
    src/Plot/LBJPlot.cpp \
    src/Plot/OSCPlot.cpp \
    src/Plot/PSUPlot.cpp \
    src/Plot/Plot.cpp \
    src/Plot/RFPlot.cpp \
    src/Plot/qcustomplot.cpp \
    src/SubWindow/DIOW.cpp \
    src/SubWindow/ETHW.cpp \
    src/SubWindow/FFOXW.cpp \
    src/SubWindow/LBJW.cpp \
    src/SubWindow/OSCW.cpp \
    src/SubWindow/PROGW.cpp \
    src/SubWindow/PSUW.cpp \
    src/SubWindow/RFW.cpp \
    src/SubWindow/SEQW.cpp \
    src/Tabs/ETHTab.cpp \
    src/Tabs/FFOXTab.cpp \
    src/Tabs/LBJTab.cpp \
    src/Tabs/OSCTab.cpp \
    src/Tabs/PROGTab.cpp \
    src/Tabs/PSUTab.cpp \
    src/Tabs/RFTab.cpp \
    src/Tabs/SEQTab.cpp \
    src/mainwindow.cpp \
    src/detachedwindow.cpp \
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
    src/Components/Fieldfox.h \
    src/Components/IQSample.h \
    src/Components/IQVector.h \
    src/Components/IndicatorBox.h \
    src/Components/IndicatorIcon.h \
    src/Components/LXIClient.h \
    src/Components/Labjack.h \
    src/Components/LabjackChannel.h \
    src/Components/OSC.h \
    src/Components/OSCChannel.h \
    src/Components/PSU.h \
    src/Components/PSUChannel.h \
    src/Components/ProgrammStarter.h \
    src/Components/RFIO.h \
    src/Components/RFIOChannel.h \
    src/Components/Sequencer.h \
    src/Dialog/SpecAddDialog.h \
    src/Dialog/SpecSignalDialog.h \
    src/Dialog/TaskDialog.h \
    src/Manager/EventManager.h \
    src/Manager/FileManager.h \
    src/Manager/RunManager.h \
    src/Plot/FFOXPlot.h \
    src/Plot/LBJPlot.h \
    src/Plot/OSCPlot.h \
    src/Plot/PSUPlot.h \
    src/Plot/Plot.h \
    src/Plot/RFPlot.h \
    src/Plot/qcustomplot.h \
    src/SubWindow/DIOW.h \
    src/SubWindow/ETHW.h \
    src/SubWindow/FFOXW.h \
    src/SubWindow/LBJW.h \
    src/SubWindow/OSCW.h \
    src/SubWindow/PROGW.h \
    src/SubWindow/PSUW.h \
    src/SubWindow/RFW.h \
    src/SubWindow/SEQW.h \
    src/Tabs/ETHTab.h \
    src/Tabs/FFOXTab.h \
    src/Tabs/LBJTab.h \
    src/Tabs/OSCTab.h \
    src/Tabs/PROGTab.h \
    src/Tabs/PSUTab.h \
    src/Tabs/RFTab.h \
    src/Tabs/SEQTab.h \
    src/mainwindow.h \
    src/detachedwindow.h \
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

DISTFILES += \
    src/Components/PSU.cpp.old \
    src/Components/PSU.h.old \
    src/Components/PSUChannel.cpp.old \
    src/Components/PSUChannel.h.old

