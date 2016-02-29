import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "LineTests"

    function test_create() {
        var line1 = Line.create(Qt.point(5,5), Qt.point(12,13));
        var line2 = Line.create(5,5,12,13);

        compare(Line.equals(line1,line2), true);
    }

    function test_length() {
        var line1 = Line.create(0,5,0,10);
        compare(Line.length(line1), 5);

        Line.setLength(line1, 20);
        compare(line1.x2, 0);
        compare(line1.y2, 25);
    }

    function test_angle() {
        var line1 = Line.create(0,5,0,10);
        compare(Line.angle(line1), 270);

        Line.setAngle(line1, 0);
        compare(Line.angle(line1), 0);
    }

    function test_translate() {
        var line1 = Line.create(0,5,0,10);
        Line.translate(line1, 3,4);
        compare(line1.x1, 3);
        compare(line1.y1, 9);
        compare(line1.x2, 3);
        compare(line1.y2, 14);

    }
}
