import QtQuick 2.5
import QtQuick.Window 2.2
import QtMultimedia 5.5
import QuickCross 1.0

Window {
    visible: true

    Rectangle {
        anchors.fill: parent
        color : "black"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            camera.imageCapture.capture()
        }
    }

    Camera {
        id: camera

        imageCapture {
            onImageCaptured: {
                console.log(preview);
                previewModel.append({ source: preview});
            }

            onImageSaved: {
                console.log("Saved", requestId, path);
                globalImageReader.source = path;
                globalImageReader.fetch();
            }
        }
    }

    ImageReader {
        id: globalImageReader
        onFetched: {
            console.log(source , ": Size = ", size);
        }
    }

    VideoOutput {
        source: camera
        anchors.fill: parent
        anchors.bottomMargin: 56
        focus : visible // to receive focus and capture key events when visible
    }

    ListView {
        id: listView
        height: 56
        width: parent.width
        anchors.bottom: parent.bottom
        orientation: ListView.Horizontal

        model: ListModel {
            id: previewModel
        }

        delegate: Item {
            width:56
            height: 56

            Image {
                anchors.fill: parent

                source: model.source;
                fillMode: Image.PreserveAspectFit
            }

            ImageReader {
                id: reader
                source: model.source;
                onFetched: {
                    read();
                }

                onFinished: {
                    console.log("Size of preview image",size);
                    clear();
                }
            }

            Component.onCompleted: {
                reader.fetch();
            }
        }
    }
}
