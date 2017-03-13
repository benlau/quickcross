CONFIG += c++11

QT += concurrent

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qcdevice.h \
    $$PWD/qcstandardpaths.h \
    $$PWD/qcrect.h \
    $$PWD/qcline.h \
    $$PWD/qcimageloader.h \
    $$PWD/qcmainthreadrunner.h \
    $$PWD/qcimagereader.h \
    $$PWD/priv/qcutils.h \
    $$PWD/qcjsonreader.h \
    $$PWD/priv/qcreader.h \
    $$PWD/qceasingcurve.h \
    $$PWD/qcrefresher.h \
    $$PWD/priv/qcimageloader_p.h \
    $$PWD/qctime.h \
    $$PWD/qcimagepool.h \
    $$PWD/QCImageLoader \
    $$PWD/QCImagePool \
    $$PWD/QCMainThreadRunner \
    $$PWD/QCRefresher \
    $$PWD/qcimagepoolprovider.h \
    $$PWD/QCImagePoolProvider \
    $$PWD/qcfuture.h

SOURCES += \
    $$PWD/qcdevice.cpp \
    $$PWD/qcqmltypes.cpp \
    $$PWD/qcstandardpaths.cpp \
    $$PWD/qcrect.cpp \
    $$PWD/qcline.cpp \
    $$PWD/qcimageloader.cpp \
    $$PWD/qcimagereader.cpp \
    $$PWD/priv/qcutils.cpp \
    $$PWD/qcjsonreader.cpp \
    $$PWD/priv/qcreader.cpp \
    $$PWD/qceasingcurve.cpp \
    $$PWD/qcrefresher.cpp \
    $$PWD/qctime.cpp \
    $$PWD/qcimagepool.cpp \
    $$PWD/qcmainthreadrunner.cpp \
    $$PWD/qcimagepoolprovider.cpp \
    $$PWD/qcfuture.cpp

DISTFILES +=

RESOURCES += \
    $$PWD/quickcross.qrc

android {
    QT += androidextras
}
