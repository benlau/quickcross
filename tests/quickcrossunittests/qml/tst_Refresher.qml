import QtQuick 2.0
import QtTest 1.0
import QuickCross 1.0

TestCase {
    name: "RefresherTests"

    Item {
        id: item1
        property int count : 0;

        function refresh() {
            count++;
        }
    }

    function test_markDirty() {
        compare(Refresher.markDirty !== undefined, true);
        wait(10);
        Refresher.markDirty(item1);
        compare(item1.count, 0);
        wait(100);
        compare(item1.count, 1);

        Refresher.markDirty(item1);
        Refresher.markDirty(item1);

        wait(100);
        compare(item1.count, 2);

        wait(100);
        compare(item1.count, 2);
    }

    Component {
        id: factory
        Item {
            property int count : 0;

            function refresh() {
                count++;
            }
        }
    }

    function test_destroy() {
        var item = factory.createObject();
        Refresher.markDirty(item);
        compare(item.count, 0);
        item.destroy(0);
        gc();
        wait(100);
    }

}
