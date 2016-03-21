import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "EasingCurveTests"

    function test_createValue() {
        var easingCurve = EasingCurve.createValue();
        compare(EasingCurve.valueForProgress(easingCurve,0.5), 0.5);
    }


}
