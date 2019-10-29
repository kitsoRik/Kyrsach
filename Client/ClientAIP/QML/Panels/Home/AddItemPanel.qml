import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import RostikObjects 1.0

Rectangle {
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
                id: pinField;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                placeholderText: "Pin...";
            }
            ComboBox {
                id: itemTypeBox;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                model: ["LED", "SERVO", "MAGSIG"];
            }

            TextField {
                id: textField1;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: {
                    let curr = itemTypeBox.currentText;
                    return (curr === "SERVO");
                }

                placeholderText: {
                    let curr = itemTypeBox.currentText;
                    if(curr === "SERVO")
                        return "Angle...";
                    return "";
                }
            }

            Button {
                id: acceptButton;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                text: "Accept";

                onClicked: {
                    client.clientSocket.addItem(pinField.text, itemTypeBox.currentText, globalQtRoom.name);
                    addItemPanel.visible = false;
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
