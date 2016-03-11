QT       += testlib qml qmltest

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
    qml/loader.qml \
    qml/tst_Rect.qml \
    qml/tst_Line.qml \
    qml/tst_ImageLoader.qml \
    qml/imageProvider.qml \
    qml/imageReader.qml

include(../../quickcross.pri)
include(vendor/vendor.pri)

HEADERS += \
    quickcrossunittests.h

QMAKE_CXXFLAGS += -Wall -Werror

RESOURCES += \
    quickcrossunittests.qrc
