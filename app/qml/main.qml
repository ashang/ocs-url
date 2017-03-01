// Still support Qt 5.2
import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import "ui" as Ui

import "scripts/Utility.js" as Utility

ApplicationWindow {
    id: app

    title: configHandler.getAppConfigApplication().name

    visible: true
    width: 450
    minimumWidth: 450
    maximumWidth: 450
    height: 200
    minimumHeight: 200
    maximumHeight: 200

    function init() {
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
            infoDialog.primaryText = primaryMessages[result.status];
            infoDialog.informativeText = metadata.filename;
            infoDialog.detailedText = result.message;
            infoDialog.open();
        });

        ocsUrlHandler.finishedWithError.connect(function(result) {
            progressDialog.close();
            errorDialog.primaryText = primaryMessages[result.status];
            errorDialog.informativeText = metadata.filename;
            errorDialog.detailedText = result.message;
            errorDialog.open();
        });

        ocsUrlHandler.downloadProgress.connect(function(id, bytesReceived, bytesTotal) {
            progressDialog.primaryText = qsTr("Downloading");
            progressDialog.informativeText = metadata.filename;
            progressDialog.progress = bytesReceived / bytesTotal;
            progressDialog.progressText
                    = Utility.convertByteToHumanReadable(bytesReceived)
                    + " / " + Utility.convertByteToHumanReadable(bytesTotal);
        });

        if (ocsUrlHandler.isValid()) {
            if (metadata.command === "download") {
                confirmDialog.primaryText = qsTr("Do you want to download?");
            }
            else if (metadata.command === "install") {
                confirmDialog.primaryText = qsTr("Do you want to install?");
            }
            confirmDialog.informativeText = metadata.filename;
            confirmDialog.detailedText
                    = qsTr("URL") + ": " + metadata.url + "\n"
                    + qsTr("File") + ": " + metadata.filename + "\n"
                    + qsTr("Type") + ": " + configHandler.getAppConfigInstallTypes()[metadata.type].name;
            confirmDialog.open();
        }
        else {
            errorDialog.primaryText = primaryMessages["error_validation"];
            errorDialog.detailedText = qsTr("Invalid OCS-URL");
            errorDialog.open();
        }
    }

    Ui.Dialog {
        id: confirmDialog
        icon: "qrc:/images/icons/dialog-information.svg"
        acceptButton.text: qsTr("OK")
        acceptButton.onClicked: {
            close();
            ocsUrlHandler.process();
        }
        rejectButton.text: qsTr("Cancel")
        rejectButton.onClicked: {
            close();
            Qt.quit();
        }
    }

    Ui.Dialog {
        id: infoDialog
        icon: "qrc:/images/icons/emblem-default.svg"
        acceptButton.text: qsTr("Open")
        acceptButton.onClicked: {
            close();
            ocsUrlHandler.openDestination();
            Qt.quit();
        }
        rejectButton.text: qsTr("Close")
        rejectButton.onClicked: {
            close();
            Qt.quit();
        }
    }

    Ui.Dialog {
        id: errorDialog
        icon: "qrc:/images/icons/dialog-warning.svg"
        rejectButton.text: qsTr("Close")
        rejectButton.onClicked: {
            close();
            Qt.quit();
        }
    }

    Ui.Dialog {
        id: progressDialog
        icon: "qrc:/images/icons/emblem-downloads.svg"
        property alias progress: progressBar.value
        property alias progressText: progressText.text
        content: ColumnLayout {
            anchors.fill: parent
            spacing: 8
            ProgressBar {
                id: progressBar
                minimumValue: 0
                maximumValue: 1
                value: 0
                Layout.fillWidth: true
            }
            Label {
                id: progressText
                text: ""
                Layout.alignment: Qt.AlignRight
            }
        }
        rejectButton.text: qsTr("Cancel")
        rejectButton.onClicked: {
            close();
            Qt.quit();
        }
    }

    Component.onCompleted: {
        app.init();
    }
}
