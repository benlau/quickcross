.import QuickCross 1.0 as QC

var EasingCurve = function(type) {
    this.value = QC.EasingCurve._createValue(type);
}

EasingCurve.prototype.valueForProgress = function(progress) {
    return QC.EasingCurve.valueForProgress(this.value, progress);
}

function create(type) {
    return new EasingCurve(type);
}
