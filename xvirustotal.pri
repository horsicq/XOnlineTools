QT += network

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/xonlinetools.h \
    $$PWD/xvirustotal.h

SOURCES += \
    $$PWD/xonlinetools.cpp \
    $$PWD/xvirustotal.cpp

!contains(XCONFIG, xformats) {
    XCONFIG += xformats
    include($$PWD/../Formats/xformats.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md 
