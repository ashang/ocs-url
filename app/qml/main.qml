import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

import "ui" as Ui

import "scripts/Utility.js" as Utility

ApplicationWindow {
    id: app

    title: configHandler.getAppConfigApplication().name

    visible: true
    width: 400
    minimumWidth: width
    maximumWidth: width
    height: 200
    minimumHeight: height
    maximumHeight: height

    function init() {
        var metadata = ocsUrlHandler.metadata();

        var primaryMessages = {
            "success_download": qsTr("Download successful"),
            "success_install": qsTr("Installation successful"),
            "error_validation": qsTr("Validation error"),
            "error_network": qsTr("Network error"),
            "error_save": qsTr("Saving file failed"),
            "error_install": qsTr("Installation failed")
        };

        ocsUrlHandler.started.connect(function() {
            confirmDialog.close();
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
            progressDialog.detailedContentLoader.item.progressBar = bytesReceived / bytesTotal;
            progressDialog.detailedContentLoader.item.progressText
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

    function fixWindowSize(dialog) {
        if (dialog.visible) {
            app.height = dialog.implicitHeight + (dialog.anchors.margins * 2);
            app.minimumHeight = app.height;
            app.maximumHeight = app.height;
        }
    }

    Ui.Dialog {
        id: confirmDialog
        icon: "qrc:/images/icons/dialog-information.svg"
        actionButton.text: qsTr("Details")
        actionButton.onClicked: toggleDetails()
        acceptButton.text: qsTr("OK")
        acceptButton.onClicked: ocsUrlHandler.process()
        rejectButton.text: qsTr("Cancel")
        rejectButton.onClicked: Qt.quit()
        onVisibleChanged: app.fixWindowSize(confirmDialog)
        onImplicitHeightChanged: app.fixWindowSize(confirmDialog)
    }

    Ui.Dialog {
        id: infoDialog
        icon: "qrc:/images/icons/emblem-default.svg"
        actionButton.text: qsTr("Details")
        actionButton.onClicked: toggleDetails()
        acceptButton.text: qsTr("Open")
        acceptButton.onClicked: {
            ocsUrlHandler.openDestination();
            Qt.quit();
        }
        rejectButton.text: qsTr("Close")
        rejectButton.onClicked: Qt.quit()
        onVisibleChanged: app.fixWindowSize(infoDialog)
        onImplicitHeightChanged: app.fixWindowSize(infoDialog)
    }

    Ui.Dialog {
        id: errorDialog
        icon: "qrc:/images/icons/dialog-warning.svg"
        actionButton.text: qsTr("Details")
        actionButton.onClicked: toggleDetails()
        rejectButton.text: qsTr("Close")
        rejectButton.onClicked: Qt.quit()
        onVisibleChanged: app.fixWindowSize(errorDialog)
        onImplicitHeightChanged: app.fixWindowSize(errorDialog)
    }

    Component {
        id: progressComponent
        ColumnLayout {
            property alias progressBar: progressBar.value
            property alias progressText: progressText.text
            anchors.fill: parent
            spacing: 4
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
    }

    Ui.Dialog {
        id: progressDialog
        icon: "qrc:/images/icons/emblem-downloads.svg"
        detailsVisible: true
        detailedContentLoader.sourceComponent: progressComponent
        rejectButton.text: qsTr("Cancel")
        rejectButton.onClicked: Qt.quit()
        onVisibleChanged: app.fixWindowSize(progressDialog)
        onImplicitHeightChanged: app.fixWindowSize(progressDialog)
    }

    Component.onCompleted: {
        app.init();
    }
}
