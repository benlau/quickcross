import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "EasingCurveTests"

    function test_createValue() {
        var easingCurve = EasingCurve._createValue(EasingCurve.InBounce);
        compare(EasingCurve.valueForProgress(easingCurve,0.5), 0.234375);
    }

    function test_create() {
        var curve = EasingCurve.create(EasingCurve.InCubic);

        compare(curve.valueForProgress(0.5), 0.125);
        compare(EasingCurve.valueForProgress(curve, 0.5), 0.125);
    }

}
