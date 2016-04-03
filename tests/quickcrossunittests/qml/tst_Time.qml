import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "TimeTests"

    function test_function() {
        var time = Time.create();

        compare(time.elapsed(), 0);
        time.start();

        wait(0);
        compare(time.elapsed() > 8, true);
    }
}
