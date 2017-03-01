import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

ColumnLayout {
    id: dialog

    visible: false
    anchors.fill: parent
    anchors.margins: 12
    spacing: 12

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
        spacing: 12
        Image {
            id: icon
            source: ""
            visible: source.toString() ? true : false
            Layout.preferredWidth: 48
            Layout.preferredHeight: 48
            sourceSize.width: 128
            sourceSize.height: 128
        }
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4
            Label {
                id: primaryText
                text: ""
                visible: text ? true : false
                Layout.fillWidth: true
                wrapMode: Text.WrapAnywhere
                font.bold: true
            }
            Label {
                id: informativeText
                text: ""
                visible: text ? true : false
                Layout.fillWidth: true
                wrapMode: Text.WrapAnywhere
            }
        }
    }

    Label {
        id: detailedText
        text: ""
        visible: text ? true : false
        Layout.fillWidth: true
        wrapMode: Text.WrapAnywhere
        color: "#444444"
    }

    Item {
        id: content
        Layout.fillWidth: true
    }

    Item {
        Layout.fillHeight: true
    }

    RowLayout {
        Layout.fillWidth: true
        spacing: 4
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
