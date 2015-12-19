QT       += testlib qml

QT       -= gui

TARGET = tst_quickcrossunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_quickcrossunittests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

DISTFILES += \
    ../../README.md \
    loader.qml

include(../../quickcross.pri)
