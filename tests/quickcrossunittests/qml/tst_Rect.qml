import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "RectTests"

    function test_type() {
        var rect = Qt.rect(3,4,5,6);

        compare(Rect.intersects(rect,rect), true);
    }
}
