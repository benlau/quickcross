QT       += testlib qml qmltest concurrent

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
    qml/imageReader.qml \
    qml/tst_JsonReader.qml \
    qml/tst_EasingCurve.qml \
    qml/tst_Refresher.qml \
    qml/tst_Time.qml \
    qml/imageProvider_tintColor.qml \
    qml/imageProvider_clip.qml

include(../../quickcross.pri)
include(vendor/vendor.pri)

HEADERS += \
    quickcrossunittests.h

QMAKE_CXXFLAGS += -Wall -Werror

RESOURCES += \
    quickcrossunittests.qrc
