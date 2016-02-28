import QtQuick 2.0
import QuickCross 1.0

Item {

    Component.onCompleted: {
        var i,num;
        var properties = ["os", "isAndroid", "isLinux", "isIOS", "isMac", "isWindows", "dp"];

        for (i in properties) {
            console.log("Device." + properties[i] + " =", Device[properties[i]]);
        }

        var standardLocations = [
             "DesktopLocation",
             "DocumentsLocation",
             "FontsLocation",
             "ApplicationsLocation",
             "MusicLocation",
             "MoviesLocation",
             "PicturesLocation",
             "TempLocation",
             "HomeLocation",
             "DataLocation",
             "CacheLocation",
             "GenericCacheLocation",
             "GenericDataLocation",
             "RuntimeLocation",
             "ConfigLocation",
             "DownloadLocation",
             "GenericConfigLocation",
             "AppDataLocation",
             "AppLocalDataLocation",
        ];

        console.log("standardLocations");

        for (i in standardLocations) {
            num = StandardPaths[standardLocations[i]];
            console.log("StandardPaths." + standardLocations[i] + " =",
                        StandardPaths.standardLocations(num));
        }

        console.log("writeableLocations");

        for (i in standardLocations) {
            num = StandardPaths[standardLocations[i]];
            console.log("StandardPaths." + standardLocations[i] + " =",
                        StandardPaths.writableLocation(num));
        }

        console.log("displayName");

        for (i in standardLocations) {
            num = StandardPaths[standardLocations[i]];
            console.log("StandardPaths." + standardLocations[i] + " =",
                        StandardPaths.displayName(num));
        }


    }
}

