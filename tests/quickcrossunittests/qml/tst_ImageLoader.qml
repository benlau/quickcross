import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "ImageLoaderTests"

    function test_interface() {
        compare(ImageLoader.running, false);
        compare(ImageLoader.isLoaded !== undefined, true);

    }

}
