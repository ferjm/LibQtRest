#-------------------------------------------------
#
# Project created by QtCreator 2011-08-04T16:42:21
#
#-------------------------------------------------

QT       -= gui
QT       += network xml

TARGET = qtrest
TEMPLATE = lib

DEFINES += LIBQTREST_LIBRARY QTREST_DEBUG

SOURCES += core/qtrest.cpp \
    core/qtrestientity.cpp \
    core/qtrestxmlparser.cpp \
    core/qtrestxmlserializer.cpp \
    core/qtresthttprequest.cpp \
    core/qtresthttpconnector.cpp \
    core/qtrestoauthrequest.cpp \
    core/qtrestoauthdata.cpp \
    core/qtrestoauth.cpp \
    core/qtrestclient.cpp

HEADERS += core/qtrest.h\
        core/libqtrest_global.h \
    core/qtrestdefs.h \
    core/qtrestientity.h \
    core/qtrestiserializer.h \
    core/qtrestiparser.h \
    core/qtrestxmlparser.h \
    core/qtrestxmlserializer.h \
    core/qtresthttprequest.h \
    core/qtresthttpconnector.h \
    core/qtrestiauthentication.h \
    core/qtrestoauthrequest.h \
    core/qtrestoauthdata.h \
    core/qtrestoauth.h \
    core/qtrestclient.h

INCLUDEPATH += core

header_files.files = $$HEADERS
header_files.path = /usr/local/include/libqtrest
INSTALLS += header_files

target.path = /usr/local/lib
INSTALLS += target
