import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Rectangle {
    id: connectPanel;
    color: "black";
    opacity: 0.7;

    visible: !client.clientSocket.connectedToControler;

    MouseArea {
        anchors.fill: parent;
    }

    ColumnLayout {
        id: startConnectPanel;
        visible: !connectControlerPanel.visible && !connectInternerPanel.visible;
        anchors.centerIn: parent;
        width: parent.width;
        height: 180;
        spacing: 5;

        Button {
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 4;
            Layout.preferredHeight: 60;

            text: "Connect to Server";

            onClicked: {
                client.clientSocket.connectToServer();
                startConnectPanel.visible = false;
            }
        }
        Button {
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 4;
            Layout.preferredHeight: 60;

            text: "Connect to Controler";

            onClicked: {
                client.controlerSocket.connectToHost("192.168.4.1", 3000);
            }
        }
    }

    ColumnLayout {
        id: connectControlerPanel;
        visible: client.controlerSocket.connected;
        anchors.centerIn: parent;
        width: parent.width;
        height: 180;
        spacing: 5;

        Rectangle {
            Layout.preferredHeight: 30;
            Layout.preferredWidth: 30;

            color:  client.controlerSocket.connectToWiFi ? "green" : "red";

            Text {
                text: "WIFI";
                anchors.fill: parent;
            }
        }

        Rectangle {
            Layout.preferredHeight: 30;
            Layout.preferredWidth: 30;

            color: client.controlerSocket.connectToServer ? "green" : "red";

            Text {
                text: "HOST";
                anchors.fill: parent;
            }
        }

        TextField {
            id: ssidField;
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 3;
            Layout.preferredHeight: 60;
            text: client.controlerSocket.ssid;
            placeholderText: "Ssid...";

            onTextEdited: client.controlerSocket.ssid = text;
        }
        TextField {
            id: passWiFiField;
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 3;
            Layout.preferredHeight: 60;
            placeholderText: "Password...";
            text: client.controlerSocket.password;

            onTextEdited: client.controlerSocket.password = text;
        }
        Button {
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 4;
            Layout.preferredHeight: 60;

            text: "Connect";

            onClicked: {
                client.controlerSocket.saveSettings();
            }
        }
    }

    ColumnLayout {
        id: connectInternerPanel;
        visible: client.clientSocket.connectedToServer;
        anchors.centerIn: parent;
        width: parent.width;
        height: 180;
        spacing: 5;
        TextField {
            id: keyField;
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 3;
            Layout.preferredHeight: 60;
            placeholderText: "Token...";
            text: "KEYROSTIK"; // FIXIT
        }
        TextField {
            id: loginField;
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 3;
            Layout.preferredHeight: 60;
            placeholderText: "Login...";
            text: "admin"; // FIXIT
        }
        TextField {
            id: passField;
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 3;
            Layout.preferredHeight: 60;
            placeholderText: "Password...";
            text: "admin"; // FIXIT
        }
        Button {
            Layout.alignment: Qt.AlignCenter;
            Layout.preferredWidth: parent.width / 4;
            Layout.preferredHeight: 60;

            text: "Connect";

            onClicked: {
                client.clientSocket.connectToControler(keyField.text, loginField.text, passField.text);
            }
        }
    }
}
