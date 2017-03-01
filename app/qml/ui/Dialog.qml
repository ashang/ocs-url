import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

ColumnLayout {
    id: dialog

    anchors.fill: parent
    anchors.margins: 12
    spacing: 8
    visible: false

    property alias icon: icon.source
    property alias primaryText: primaryText.text
    property alias informativeText: informativeText.text
    property alias detailedText: detailedText.text
    property alias content: content.children
    property alias acceptButton: acceptButton
    property alias rejectButton: rejectButton

    function open() {
        dialog.visible = true;
    }

    function close() {
        dialog.visible = false;
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: parent.spacing
        Image {
            id: icon
            source: ""
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            visible: source.toString() ? true : false
        }
        ColumnLayout {
            Layout.fillWidth: true
            spacing: parent.spacing
            Label {
                id: primaryText
                text: ""
                font.bold: true
                visible: text ? true : false
            }
            Label {
                id: informativeText
                text: ""
                visible: text ? true : false
            }
            Label {
                id: detailedText
                text: ""
                visible: text ? true : false
            }
            Item {
                id: content
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: parent.spacing
        Item {
            Layout.fillWidth: true
        }
        Button {
            id: acceptButton
            text: ""
            visible: text ? true : false
        }
        Button {
            id: rejectButton
            text: ""
            visible: text ? true : false
        }
    }
}
