import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "ImageLoaderTests"

    ImageLoader {
        id: loader
    }

    function test_interface() {
        compare(loader.running, false);
        compare(loader.isLoaded, false);
    }

}
