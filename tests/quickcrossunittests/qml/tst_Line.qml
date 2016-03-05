import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "LineTests"

    function test_create() {
        var line1 = Line.create(Qt.point(5,5), Qt.point(12,13));
        var line2 = Line.create(5,5,12,13);
        var line3 = Line.create(Qt.point(4,5));
        var line4 = Line.create(line3);

        compare(Line.equals(line1,line2), true);
        compare(line3.x1, 0);
        compare(line3.y1, 0);
        compare(line3.x2, 4);
        compare(line3.y2, 5);

        compare(Line.equals(line3,line4), true);
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

    function test_translateToOrigin() {
        var line1 = Line.create(4,5,7,10);
        Line.translateToOrigin(line1);
        compare(line1.x1, 0);
        compare(line1.y1, 0);
        compare(line1.x2, 3);
        compare(line1.y2, 5);
    }

    function test_dxdy() {
        var line1 = Line.create(3,5,4,10);
        compare(Line.dx(line1) , 1);
        compare(Line.dy(line1) , 5);
    }

    function test_p1p2() {
        var line1 = Line.create(3,5,4,10);

        compare(Line.p1(line1), Qt.point(3,5));
        compare(Line.p2(line1), Qt.point(4,10));
    }

}
