import QtQuick 2.5
import QtQuick.Window 2.2
import QtQml.Models 2.1

Window {
    id: component
    visible: true
    width: 640
    height: 480

    ObjectModel {
        id: objectModel

        ListItem {
            width: component.width
            source: "qrc:///img/Lenna.png"
            sourceWidth: 512
            sourceHeight: 512
        }

        ListItem {
            width: component.width;
            source: "image://arts/Lenna.png"
            sourceWidth: 512
            sourceHeight: 512
        }

        BorderImageItem {
            width: component.width;

            source: "qrc:///img/button.png"
            expectedWidth: 300
            expectedHeight: 200
        }

        BorderImageItem {
            width: component.width;

            source: "image://arts/button"
            expectedWidth: 300
            expectedHeight: 200
        }

    }


    ListView {
        anchors.fill: parent
        model: objectModel
    }



}
