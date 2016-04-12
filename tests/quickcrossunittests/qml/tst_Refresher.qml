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
        // Destory a dirty item before the next refresh trigger
        var item = factory.createObject();
        Refresher.markDirty(item);
        compare(item.count, 0);
        item.destroy(0);
        gc();
        wait(100);
    }

    Item {
        id: item2

        property int count : 0

        function refresh() {
            count++;
            if (count === 1) {
                Refresher.markDirty(item2);
            }
        }
    }

    function test_refresh_immediately() {

        item2.count = 0;
        compare(item2.count, 0);
        Refresher.markDirty(item2);
        wait(100);
        compare(item2.count, 2);

    }

}
