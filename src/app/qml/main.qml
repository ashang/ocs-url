import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

import "scripts/Utility.js" as Utility

Window {
    id: app
    title: Qt.application.name
    width: 400
    height: 200
    minimumWidth: 400
    minimumHeight: 200
    maximumWidth: 800
    maximumHeight: 400

    MessageDialog {
        id: confirmDialog
        title: app.title
        icon: StandardIcon.Question
        text: ""
        informativeText: ""
        detailedText: ""
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: ocsUrlHandler.process()
        onRejected: Qt.quit()
    }

    MessageDialog {
        id: infoDialog
        title: app.title
        icon: StandardIcon.Information
        text: ""
        informativeText: ""
        detailedText: ""
        standardButtons: StandardButton.Open | StandardButton.Close
        onAccepted: {
            ocsUrlHandler.openDestination();
            Qt.quit();
        }
        onRejected: Qt.quit()
    }

    MessageDialog {
        id: errorDialog
        title: app.title
        icon: StandardIcon.Warning
        text: ""
        informativeText: ""
        detailedText: ""
        standardButtons: StandardButton.Close
        onRejected: Qt.quit()
    }

    Dialog {
        id: progressDialog
        title: app.title
        property alias primaryLabel: primaryLabel
        property alias informativeLabel: informativeLabel
        property alias progressBar: progressBar
        property alias progressLabel: progressLabel
        contentItem: Item {
            implicitWidth: 400
            implicitHeight: 150
            Column {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                Label {
                    id: primaryLabel
                    text: " "
                    font.bold: true
                }
                Label {
                    id: informativeLabel
                    text: " "
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
                    text: " "
                    anchors.right: parent.right
                }
                Button {
                    id: cancelButton
                    text: qsTr("Cancel")
                    anchors.right: parent.right
                    onClicked: Qt.quit()
                }
            }
        }
    }

    Component.onCompleted: {
        var metadata = ocsUrlHandler.metadata();
        var primaryMessages = {
            "success_download": qsTr("Download successfull"),
            "success_install": qsTr("Installation successfull"),
            "error_validation": qsTr("Validation error"),
            "error_network": qsTr("Network error"),
            "error_save": qsTr("Saving file failed"),
            "error_install": qsTr("Installation failed")
        };

        ocsUrlHandler.started.connect(function() {
            progressDialog.open();
        });

        ocsUrlHandler.finishedWithSuccess.connect(function(result) {
            progressDialog.close();
            infoDialog.text = primaryMessages[result.status];
            infoDialog.informativeText = metadata.filename;
            infoDialog.detailedText = qsTr(result.message);
            infoDialog.open();
        });

        ocsUrlHandler.finishedWithError.connect(function(result) {
            progressDialog.close();
            errorDialog.text = primaryMessages[result.status];
            errorDialog.informativeText = metadata.filename;
            errorDialog.detailedText = qsTr(result.message);
            errorDialog.open();
        });

        ocsUrlHandler.downloadProgress.connect(function(id, bytesReceived, bytesTotal) {
            progressDialog.primaryLabel.text = qsTr("Downloading");
            progressDialog.informativeLabel.text = metadata.filename;
            progressDialog.progressBar.value = bytesReceived / bytesTotal;
            progressDialog.progressLabel.text = Utility.convertByteToHumanReadable(bytesReceived)
                    + " / " + Utility.convertByteToHumanReadable(bytesTotal)
        });

        if (ocsUrlHandler.isValid()) {
            if (metadata.command === "download") {
                confirmDialog.text = qsTr("Do you want to download?");
            }
            else if (metadata.command === "install") {
                confirmDialog.text = qsTr("Do you want to install?");
            }
            confirmDialog.informativeText = metadata.filename;
            confirmDialog.detailedText = qsTr("URL") + ": " + metadata.url + "\n\n"
                    + qsTr("File") + ": " + metadata.filename + "\n\n"
                    + qsTr("Type") + ": " + metadata.type;
            confirmDialog.open();
        }
        else {
            errorDialog.text = qsTr("Validation error");
            errorDialog.detailedText = qsTr("Invalid OCS-URL");
            errorDialog.open();
        }
    }
}
