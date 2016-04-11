INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qcdevice.h \
    $$PWD/qcstandardpaths.h \
    $$PWD/qcrect.h \
    $$PWD/qcline.h \
    $$PWD/qcimageloader.h \
    $$PWD/qcimageloaderwrapper.h \
    $$PWD/qcimageprovider.h \
    $$PWD/priv/qcmainthreadrunner.h \
    $$PWD/qcimagereader.h \
    $$PWD/priv/qcutils.h \
    $$PWD/qcjsonreader.h \
    $$PWD/priv/qcreader.h \
    $$PWD/qceasingcurve.h \
    $$PWD/qcrefresher.h \
    $$PWD/priv/qcimageloader_p.h \
    $$PWD/qctime.h \
    $$PWD/QCImageLoader \
    $$PWD/QCImageProvider

SOURCES += \
    $$PWD/qcdevice.cpp \
    $$PWD/qcqmltypes.cpp \
    $$PWD/qcstandardpaths.cpp \
    $$PWD/qcrect.cpp \
    $$PWD/qcline.cpp \
    $$PWD/qcimageloader.cpp \
    $$PWD/qcimageloaderwrapper.cpp \
    $$PWD/qcimageprovider.cpp \
    $$PWD/priv/qcmainthreadrunner.cpp \
    $$PWD/qcimagereader.cpp \
    $$PWD/priv/qcutils.cpp \
    $$PWD/qcjsonreader.cpp \
    $$PWD/priv/qcreader.cpp \
    $$PWD/qceasingcurve.cpp \
    $$PWD/qcrefresher.cpp \
    $$PWD/qctime.cpp

DISTFILES +=

RESOURCES += \
    $$PWD/quickcross.qrc

android {
    QT += androidextras
}
