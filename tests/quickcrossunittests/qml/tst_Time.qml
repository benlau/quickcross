import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "TimeTests"

    function test_function() {
        var time = Time.create();

        compare(time.elapsed(), 0);
        time.start();

        wait(8);
        compare(time.elapsed() > 8, true);

        var ret = time.restart();
        compare(ret > 8, true);
        compare(time.elapsed() < 8 , true);

    }
}
