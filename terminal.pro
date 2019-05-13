QT += widgets serialport charts
requires(qtConfig(combobox))

TARGET = terminal
TEMPLATE = app

SOURCES += \
    imetPlot.cpp \
    main.cpp \
    settingsdialog.cpp \
    console.cpp \
    mainwindow.cpp \
    chart.cpp

HEADERS += \
    imetPlot.h \
    settingsdialog.h \
    console.h \
    mainwindow.h \
    chart.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialport/terminal
INSTALLS += target
