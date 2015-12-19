import QtQuick 2.0
import QuickCross 1.0

Item {

    Component.onCompleted: {
        var properties = ["os", "isAndroid", "isLinux", "isIOS", "isMac", "isWindows"];

        for (var i in properties) {
            console.log("Device." + properties[i] + " =", Device[properties[i]]);
        }

    }
}

