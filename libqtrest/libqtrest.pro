#-------------------------------------------------
#
# Project created by QtCreator 2011-08-04T16:42:21
#
#-------------------------------------------------

QT       -= gui

TARGET = libqtrest
TEMPLATE = lib

DEFINES += LIBQTREST_LIBRARY

SOURCES += qtrest.cpp

HEADERS += qtrest.h\
        libqtrest_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE57F7AD3
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = libqtrest.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
