QT       += testlib qml

QT       -= gui

TARGET = tst_quickcrossunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    quickcrossunittests.cpp \
    main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

DISTFILES += \
    ../../README.md \
    loader.qml

include(../../quickcross.pri)
include(vendor/vendor.pri)

HEADERS += \
    quickcrossunittests.h
