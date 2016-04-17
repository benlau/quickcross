import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    visible: true
    width: 640
    height: 480

    Image {
        id: image;
        objectName: "image"
        source: "image://custom/qt-logo-medium"
    }

    Image {
        id: image2
        objectName: "image2"
        source: "image://custom/button3?scaleToFitDpi=true"
    }

}
