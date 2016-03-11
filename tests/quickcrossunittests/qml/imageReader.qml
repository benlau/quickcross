import QtQuick 2.0
import QtQuick.Window 2.0
import QuickCross 1.0

Window {
    visible: true
    width: 640
    height: 480

    Item { // Dirty hack for a bug in Automator

        ImageReader {
            id: reader1
            objectName: "reader1"
            source: ":/unittests/img/qt-logo-medium.png"
        }

    }

}
