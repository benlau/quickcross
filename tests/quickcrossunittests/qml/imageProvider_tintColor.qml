import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    visible: true
    width: 640
    height: 480

    Grid {
        spacing: 8

        Image {
            id: image;
            objectName: "image"
            source: "image://custom/qt-logo-medium"
            width: 100
            fillMode: Image.PreserveAspectFit
        }

        Image {
            objectName: "image"
            source: "image://custom/qt-logo-medium?tintColor=" + escape("#000000");
            width: 100
            fillMode: Image.PreserveAspectFit
        }

        Image {
            objectName: "image"
            source: "image://custom/qt-logo-medium?tintColor=" + escape("#FF0000");
            width: 100
            fillMode: Image.PreserveAspectFit
        }

        Image {
            objectName: "image"
            source: "image://custom/qt-logo-medium?tintColor=" + escape("#7FFF0000");
            width: 100
            fillMode: Image.PreserveAspectFit
        }

    }

}
