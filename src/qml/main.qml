import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2

Window {
    id: root
    title: Qt.application.name
    width: 400
    height: 200
    minimumWidth: 400
    minimumHeight: 200
    maximumWidth: 800
    maximumHeight: 400

    MessageDialog {
        id: confirmDialog
        title: root.title
        icon: StandardIcon.Question
        text: ''
        informativeText: ''
        detailedText: ''
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: xdgUrlHandler.process()
        onRejected: Qt.quit()
    }

    MessageDialog {
        id: infoDialog
        title: root.title
        icon: StandardIcon.Information
        text: ''
        informativeText: ''
        detailedText: ''
        standardButtons: StandardButton.Open | StandardButton.Close
        onAccepted: {
            xdgUrlHandler.openDestination();
            Qt.quit();
        }
        onRejected: Qt.quit()
    }

    MessageDialog {
        id: errorDialog
        title: root.title
        icon: StandardIcon.Warning
        text: ''
        informativeText: ''
        detailedText: ''
        standardButtons: StandardButton.Close
        onRejected: Qt.quit()
    }

    Dialog {
        id: progressDialog
        title: root.title
        contentItem: Item {
            implicitWidth: 400
            implicitHeight: 150
            Column {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                Label {
                    id: primaryLabel
                    text: ''
                    font.bold: true
                }
                Label {
                    id: informativeLabel
                    text: ''
                }
                ProgressBar {
                    id: progressBar
                    maximumValue: 1
                    minimumValue: 0
                    value: 0
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
                Label {
                    id: progressLabel
                    text: ''
                    anchors.right: parent.right
                }
                Button {
                    id: cancelButton
                    text: 'Cancel'
                    anchors.right: parent.right
                    onClicked: Qt.quit()
                }
            }
        }
        property alias primaryLabel: primaryLabel
        property alias informativeLabel: informativeLabel
        property alias progressBar: progressBar
        property alias progressLabel: progressLabel
    }

    Component.onCompleted: {
        var metadata = JSON.parse(xdgUrlHandler.getMetadata());
        var primaryMessages = {
            'success_download': 'Download successfull',
            'success_install': 'Installation successfull',
            'error_validation': 'Validation error',
            'error_network': 'Network error',
            'error_filetype': 'File type error',
            'error_save': 'Saving file failed',
            'error_install': 'Installation failed'
        };

        xdgUrlHandler.started.connect(function() {
            progressDialog.open();
        });

        xdgUrlHandler.finished.connect(function(result) {
            progressDialog.close();
            result = JSON.parse(result);
            infoDialog.text = primaryMessages[result.status];
            infoDialog.informativeText = metadata.filename;
            infoDialog.detailedText = result.message;
            infoDialog.open();
        });

        xdgUrlHandler.error.connect(function(result) {
            progressDialog.close();
            result = JSON.parse(result);
            errorDialog.text = primaryMessages[result.status];
            errorDialog.informativeText = metadata.filename;
            errorDialog.detailedText = result.message;
            errorDialog.open();
        });

        xdgUrlHandler.downloadProgress.connect(function(received, total) {
            progressDialog.primaryLabel.text = 'Downloading... ';
            progressDialog.informativeLabel.text = metadata.filename;
            progressDialog.progressBar.value = received / total;
            progressDialog.progressLabel.text = convertByteToHumanReadable(received)
                    + ' / ' + convertByteToHumanReadable(total)
        });

        if (xdgUrlHandler.isValid()) {
            confirmDialog.text = 'Do you want to ' + metadata.command + '?';
            confirmDialog.informativeText = metadata.filename;
            confirmDialog.detailedText = 'URL: ' + metadata.url + '\n\n'
                    + 'File: ' + metadata.filename + '\n\n'
                    + 'Type: ' + metadata.type;
            confirmDialog.open();
        }
        else {
            errorDialog.text = 'Validation error';
            errorDialog.detailedText = 'Invalid XDG-URL ' + xdgUrlHandler.getXdgUrl();
            errorDialog.open();
        }
    }

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
}
