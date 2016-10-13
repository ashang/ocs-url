import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.1

Window {
    id: root
    title: Qt.application.name

    MessageDialog {
        id: confirmDialog
        title: root.title
        text: ''
        detailedText: ''
        icon: StandardIcon.Question
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: xdgUrlHandler.process()
        onRejected: Qt.quit()
    }

    MessageDialog {
        id: infoDialog
        title: root.title
        text: ''
        detailedText: ''
        icon: StandardIcon.Information
        onAccepted: Qt.quit()
    }

    MessageDialog {
        id: errorDialog
        title: root.title
        text: ''
        detailedText: ''
        icon: StandardIcon.Warning
        onAccepted: Qt.quit()
    }

    Component.onCompleted: {
        xdgUrlHandler.finished.connect(function(result) {
            result = JSON.parse(result);
            var messages = {
                'download_success': 'Download finished',
                'install_success': 'Installation finished',
                'validation_error': 'Invalid XDG-URL',
                'network_error': 'Download failed',
                'filetype_error': 'Incorrect file type',
                'save_error': 'Saving file failed',
                'install_error': 'Installation failed'
            };
            if (result.success) {
                infoDialog.text = messages[result.success];
                infoDialog.detailedText = xdgUrlHandler.getXdgUrl();
                infoDialog.open();
            }
            else if (result.error) {
                errorDialog.text = messages[result.error];
                errorDialog.detailedText = xdgUrlHandler.getXdgUrl();
                errorDialog.open();
            }
        });

        if (xdgUrlHandler.isValid()) {
            confirmDialog.text = 'Do you want to continue?';
            confirmDialog.detailedText = xdgUrlHandler.getXdgUrl();
            confirmDialog.open();
        }
        else {
            errorDialog.text = 'Invalid XDG-URL';
            errorDialog.detailedText = xdgUrlHandler.getXdgUrl();
            errorDialog.open();
        }
    }
}
