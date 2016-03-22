.pragma library
.import QuickCross 1.0 as QC

var Loader = function() {
    this.v1 = "1";
}

// To verify can it access singleton member in loading.
var Device = QC.Device;

function create() {
    return new Loader();
}
