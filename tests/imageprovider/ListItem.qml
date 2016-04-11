import QtQuick 2.0

Item {
    id: component

    property string source

    property int sourceWidth: 0

    property int sourceHeight: 0

    height: 48 + content.height

    Rectangle {
        id: header
        height: 48
        width: parent.width
        color : "#000000"

        Text {
            anchors.fill: parent
            font.pixelSize: 16
            color: "#FFFFFF"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: component.source
        }
    }

    Item {
        id: content
        width: parent.width
        height: Math.max(150 , sourceHeight + 100)

        Image {
            id: image
            anchors.centerIn: parent
            source: component.source
        }

        Rectangle {
            anchors.centerIn: parent
            width: component.sourceWidth
            height: component.sourceHeight
            color: "blue"
            opacity: 0.1
            border.color: "red"
            border.width: 10
        }
    }

    Component.onCompleted: {
        console.log("Source:", source);
        console.log("Image Size: ", image.width, image.height);
        console.log("Source Size: ", image.sourceSize.width, image.sourceSize.height);
    }


}
