import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

import "scripts/Utility.js" as Utility

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
        text: ""
        informativeText: ""
        detailedText: ""
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: xdgUrlHandler.process()
        onRejected: Qt.quit()
    }

    MessageDialog {
        id: infoDialog
        title: root.title
        icon: StandardIcon.Information
        text: ""
        informativeText: ""
        detailedText: ""
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
        text: ""
        informativeText: ""
        detailedText: ""
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
                    text: ""
                    font.bold: true
                }
                Label {
                    id: informativeLabel
                    text: ""
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
                    text: ""
                    anchors.right: parent.right
                }
                Button {
                    id: cancelButton
                    text: "Cancel"
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
        var metadata = xdgUrlHandler.metadata();
        var primaryMessages = {
            "success_download": "Download successfull",
            "success_install": "Installation successfull",
            "error_validation": "Validation error",
            "error_network": "Network error",
            "error_save": "Saving file failed",
            "error_install": "Installation failed"
        };

        xdgUrlHandler.started.connect(function() {
            progressDialog.open();
        });

        xdgUrlHandler.finishedWithSuccess.connect(function(result) {
            progressDialog.close();
            infoDialog.text = primaryMessages[result.status];
            infoDialog.informativeText = metadata.filename;
            infoDialog.detailedText = result.message;
            infoDialog.open();
        });

        xdgUrlHandler.finishedWithError.connect(function(result) {
            progressDialog.close();
            errorDialog.text = primaryMessages[result.status];
            errorDialog.informativeText = metadata.filename;
            errorDialog.detailedText = result.message;
            errorDialog.open();
        });

        xdgUrlHandler.downloadProgress.connect(function(bytesReceived, bytesTotal) {
            progressDialog.primaryLabel.text = "Downloading... ";
            progressDialog.informativeLabel.text = metadata.filename;
            progressDialog.progressBar.value = bytesReceived / bytesTotal;
            progressDialog.progressLabel.text = Utility.convertByteToHumanReadable(bytesReceived)
                    + " / " + Utility.convertByteToHumanReadable(bytesTotal)
        });

        if (xdgUrlHandler.isValid()) {
            confirmDialog.text = "Do you want to " + metadata.command + "?";
            confirmDialog.informativeText = metadata.filename;
            confirmDialog.detailedText = "URL: " + metadata.url + "\n\n"
                    + "File: " + metadata.filename + "\n\n"
                    + "Type: " + metadata.type;
            confirmDialog.open();
        }
        else {
            errorDialog.text = "Validation error";
            errorDialog.detailedText = "Invalid XDG-URL " + xdgUrlHandler.xdgUrl();
            errorDialog.open();
        }
    }
}
