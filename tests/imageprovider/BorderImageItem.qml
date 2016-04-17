import QtQuick 2.0

Item {

    id: component

    property string source

    property int expectedWidth: 0

    property int expectedHeight: 0

    property int borderMargin: 42

    height: 48 + expectedHeight + 50

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

    BorderImage {
        id: image
        y: 10 + 48
        anchors.horizontalCenter: parent.horizontalCenter
        border { left: borderMargin; right: borderMargin; top: borderMargin; bottom: borderMargin}
        source: component.source
        width: expectedWidth
        height: expectedHeight
    }

    PinchArea {
        anchors.fill: parent

        onPinchUpdated: {
            if (pinch.previousScale > pinch.scale) {

                if (image.width <= borderMargin * 2 ||
                    image.height <= borderMargin * 2) {
                    return;
                }

                image.width -=10;
                image.height -=10;
            } else {
                image.width += 10;
                image.height +=10;
            }
        }

        MouseArea {
            anchors.fill: parent
        }
    }

}
