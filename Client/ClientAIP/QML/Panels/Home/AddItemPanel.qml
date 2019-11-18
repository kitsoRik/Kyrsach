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

                placeholderText: "SOID";
            }

            TextField {
                id: textFieldSIOC;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "SOIC";
            }

            TextField {
                id: textFieldVSYNC;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "VSYNC";
            }

            TextField {
                id: textFieldHREF;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "HREF";
            }

            TextField {
                id: textFieldXCLK;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "XCLK";
            }

            TextField {
                id: textFieldPCLK;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "PCLK";
            }

            TextField {
                id: textFieldD0;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "D0";
            }

            TextField {
                id: textFieldD1;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "D1";
            }

            TextField {
                id: textFieldD2;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "D2";
            }

            TextField {
                id: textFieldD3;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "D3";
            }

            TextField {
                id: textFieldD4;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "D4";
            }

            TextField {
                id: textFieldD5;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "D5";
            }

            TextField {
                id: textFieldD6;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

                placeholderText: "D6";
            }

            TextField {
                id: textFieldD7;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                visible: itemTypeBox.currentText === "CAMERA";

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
                        arr = [21,22,34,35,32,33,27,17,16,15,14,13,12,2];
//                    arr = [textFieldSIOD.text,
//                               textFieldSIOC.text,
//                               textFieldVSYNC.text,
//                               textFieldHREF.text,
//                        textFieldXCLK.text,
//                        textFieldPCLK.text,
//                               textFieldD0.text,
//                               textFieldD1.text,
//                               textFieldD2.text,
//                               textFieldD3.text,
//                               textFieldD4.text,
//                               textFieldD5.text,
//                               textFieldD6.text,
//                               textFieldD7.text];
                    else
                    arr = [pinField.text];
                    client.clientSocket.addItem(arr, itemTypeBox.currentText, globalQtRoom.name);
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
