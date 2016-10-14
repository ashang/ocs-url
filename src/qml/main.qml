import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.1

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
        standardButtons: StandardButton.Ok
        onAccepted: Qt.quit()
    }

    MessageDialog {
        id: errorDialog
        title: root.title
        icon: StandardIcon.Warning
        text: ''
        informativeText: ''
        detailedText: ''
        standardButtons: StandardButton.Ok
        onAccepted: Qt.quit()
    }

    Component.onCompleted: {
        xdgUrlHandler.finished.connect(function(result) {
            result = JSON.parse(result);
            var metadata = JSON.parse(xdgUrlHandler.getMetadata());
            var messages = {
                'success_download': 'Download successfull',
                'success_install': 'Installation successfull',
                'error_validation': 'Validation error',
                'error_network': 'Network error',
                'error_filetype': 'File type error',
                'error_save': 'Saving file failed',
                'error_install': 'Installation failed'
            };
            if (result.status.split('_').shift() === 'success') {
                infoDialog.text = messages[result.status];
                infoDialog.informativeText = metadata.filename;
                infoDialog.detailedText = result.message;
                infoDialog.open();
            }
            else {
                errorDialog.text = messages[result.status];
                errorDialog.informativeText = metadata.filename;
                errorDialog.detailedText = result.message;
                errorDialog.open();
            }
        });

        if (xdgUrlHandler.isValid()) {
            var metadata = JSON.parse(xdgUrlHandler.getMetadata());
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
}
