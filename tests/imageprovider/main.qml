import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: component
    visible: true
    width: 640
    height: 480

    ListModel {
        id: listModel

        ListElement {
            source: "qrc:///img/Lenna.png"
            sourceWidth: 512
            sourceHeight: 512
        }

        ListElement {
            source: "image://arts/Lenna.png"
            sourceWidth: 512
            sourceHeight: 512
        }

    }

    ListView {
        anchors.fill: parent
        model: listModel

        delegate: ListItem {
            source: model.source
            sourceWidth: model.sourceWidth
            sourceHeight: model.sourceHeight
            width: component.width
        }
    }



}
