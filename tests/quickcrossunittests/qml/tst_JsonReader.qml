import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "JsonReaderTests"

    JsonReader {
        id: reader1
    }

    function test_blocked_read() {
        compare(reader1.isReady, false);
        compare(reader1.isCompleted, false);
        compare(reader1.isError, false);
        reader1.autoRead = true;
        reader1.source = "qrc:///unittests/json/test.json"
        compare(reader1.isReady, true);
        compare(reader1.isCompleted, true);
        compare(reader1.isError, false);
        var object = reader1.object;
        compare(object.v1 , 1);
        compare(object.v2 , "2");
        compare(object.v3 , 3.0);
    }


}
