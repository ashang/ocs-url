.pragma library

function convertByteToHumanReadable(bytes) {
    bytes = parseFloat(bytes);
    var kb = 1024;
    var mb = 1024 * kb;
    var gb = 1024 * mb;
    var tb = 1024 * gb;
    var pb = 1024 * tb;
    var eb = 1024 * pb;
    var zb = 1024 * eb;
    var yb = 1024 * zb;

    var text = '';
    if (bytes < kb) {
        text = bytes.toFixed(0) + ' B';
    }
    else if (bytes < mb) {
        text = (bytes / kb).toFixed(2) + ' KB';
    }
    else if (bytes < gb) {
        text = (bytes / mb).toFixed(2) + ' MB';
    }
    else if (bytes < tb) {
        text = (bytes / gb).toFixed(2) + ' GB';
    }
    else if (bytes < pb) {
        text = (bytes / tb).toFixed(2) + ' TB';
    }
    else if (bytes < eb) {
        text = (bytes / pb).toFixed(2) + ' PB';
    }
    else if (bytes < zb) {
        text = (bytes / eb).toFixed(2) + ' EB';
    }
    else if (bytes < yb) {
        text = (bytes / zb).toFixed(2) + ' ZB';
    }
    else if (bytes >= yb) {
        text = (bytes / yb).toFixed(2) + ' YB';
    }
    return text;
}
