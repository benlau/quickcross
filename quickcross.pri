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
    $$PWD/qcimagereader.h

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
    $$PWD/qcimagereader.cpp

DISTFILES +=

android {
    QT += androidextras
}
