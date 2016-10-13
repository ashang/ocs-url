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
        detailedText: ''
        onAccepted: Qt.quit()
    }

    MessageDialog {
        id: errorDialog
        title: root.title
        icon: StandardIcon.Warning
        text: ''
        detailedText: ''
        onAccepted: Qt.quit()
    }

    Component.onCompleted: {
        xdgUrlHandler.finished.connect(function(result) {
            result = JSON.parse(result);
            var metadata = JSON.parse(xdgUrlHandler.getMetadata());
            var messages = {
                'download_success': 'Download successfull',
                'install_success': 'Installation successfull',
                'validation_error': 'Invalid XDG-URL',
                'network_error': 'Download failed',
                'filetype_error': 'Incorrect file type',
                'save_error': 'Saving file failed',
                'install_error': 'Installation failed'
            };
            if (result.success) {
                infoDialog.text
                        = (metadata.command === 'install' ? 'Install' : 'Download')
                        + ': ' + metadata.filename
                        + '\n\n'
                        + messages[result.success];
                infoDialog.detailedText
                        = 'The file has stored into: ' + result.destination;
                infoDialog.open();
            }
            else if (result.error) {
                errorDialog.text
                        = (metadata.command === 'install' ? 'Install' : 'Download')
                        + ': ' + metadata.filename
                        + '\n\n'
                        + messages[result.error];
                errorDialog.detailedText = result.detail;
                errorDialog.open();
            }
        });

        if (xdgUrlHandler.isValid()) {
            var metadata = JSON.parse(xdgUrlHandler.getMetadata());
            confirmDialog.text
                    = (metadata.command === 'install' ? 'Install' : 'Download')
                    + ': ' + metadata.filename
                    + '\n\n'
                    + 'Do you want to continue?';
            confirmDialog.detailedText
                    = 'URL: ' + metadata.url
                    + '\n\n'
                    + 'File: ' + metadata.filename
                    + '\n\n'
                    + 'Type: ' + metadata.type;
            confirmDialog.open();
        }
        else {
            errorDialog.text = 'Invalid XDG-URL';
            errorDialog.detailedText = xdgUrlHandler.getXdgUrl();
            errorDialog.open();
        }
    }
}
