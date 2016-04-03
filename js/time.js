.pragma library
.import QuickCross 1.0 as QC

var Time = function() {
    this.value = QC.Time._createValue();
}

Time.prototype.start = function() {
    QC.Time.start(this);
}

Time.prototype.elapsed = function() {
    return QC.Time.elapsed(this);
}

function create() {
    return new Time();
}
