QT += network

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xonlinetools.h \
    $$PWD/xonlinetoolsdialogprocess.h \
    $$PWD/xonlinetoolsoptionswidget.h \
    $$PWD/xvirustotal.h \
    $$PWD/xvirustotaldialog.h \
    $$PWD/xvirustotalwidget.h

SOURCES += \
    $$PWD/xonlinetools.cpp \
    $$PWD/xonlinetoolsdialogprocess.cpp \
    $$PWD/xonlinetoolsoptionswidget.cpp \
    $$PWD/xvirustotal.cpp \
    $$PWD/xvirustotaldialog.cpp \
    $$PWD/xvirustotalwidget.cpp

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include($$PWD/../Formats/xformats.pri)
}

FORMS += \
    $$PWD/xonlinetoolsoptionswidget.ui \
    $$PWD/xvirustotaldialog.ui \
    $$PWD/xvirustotalwidget.ui

DISTFILES += \
    $$PWD/xvirustotalwidget.cmake
