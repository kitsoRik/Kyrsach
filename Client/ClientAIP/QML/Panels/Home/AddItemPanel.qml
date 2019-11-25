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

        ScrollView {
            anchors.fill: parent;
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

                model: ["LED", "SERVO", "MAGSIG", "CAMERA"];
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

            TextField {
                id: textFieldSIOD;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "21";
                placeholderText: "SOID";
            }

            TextField {
                id: textFieldSIOC;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "22";
                placeholderText: "SOIC";
            }

            TextField {
                id: textFieldVSYNC;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "34";
                placeholderText: "VSYNC";
            }

            TextField {
                id: textFieldHREF;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "35"
                placeholderText: "HREF";
            }

            TextField {
                id: textFieldXCLK;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "32";
                placeholderText: "XCLK";
            }

            TextField {
                id: textFieldPCLK;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "33";
                placeholderText: "PCLK";
            }

            TextField {
                id: textFieldD0;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "27";
                placeholderText: "D0";
            }

            TextField {
                id: textFieldD1;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "17";
                placeholderText: "D1";
            }

            TextField {
                id: textFieldD2;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "16";
                placeholderText: "D2";
            }

            TextField {
                id: textFieldD3;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "15";
                placeholderText: "D3";
            }

            TextField {
                id: textFieldD4;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "14";
                placeholderText: "D4";
            }

            TextField {
                id: textFieldD5;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "13";
                placeholderText: "D5";
            }

            TextField {
                id: textFieldD6;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "12";
                placeholderText: "D6";
            }

            TextField {
                id: textFieldD7;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";
                text: "2";
                placeholderText: "D7";
            }

            Button {
                id: acceptButton;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                text: "Accept";

                onClicked: {
                    let arr;

                    if(itemTypeBox.currentText == "CAMERA")
                        arr = [textFieldSIOD.text,
                               textFieldSIOC.text,
                               textFieldVSYNC.text,
                               textFieldHREF.text,
                               textFieldXCLK.text,
                               textFieldPCLK.text,
                               textFieldD0.text,
                               textFieldD1.text,
                               textFieldD2.text,
                               textFieldD3.text,
                               textFieldD4.text,
                               textFieldD5.text,
                               textFieldD6.text,
                               textFieldD7.text];
                    else
                        arr = [pinField.text];
                    client.clientSocket.addItem(arr, itemTypeBox.currentText, globalQtRoom.identifier);
                    addItemPanel.visible = false;
                }
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
