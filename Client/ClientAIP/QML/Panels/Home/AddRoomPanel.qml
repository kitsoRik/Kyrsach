import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import RostikObjects 1.0

Rectangle {
    id: root;
    color: "black";
    opacity: 0.9;
    Rectangle {
        color: "transparent";
        width: 250;
        height: 100;
        anchors.centerIn: parent;

        ColumnLayout {
            anchors.fill: parent;
            TextField {
                id: nameField;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                placeholderText: "Name"
            }

            Button {
                id: acceptButton;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                text: "Accept";

                onClicked: {
                    client.clientSocket.addRoom(nameField.text);
                    root.visible = false;
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: parent.visible = false;
        z: -1;
    }
}
