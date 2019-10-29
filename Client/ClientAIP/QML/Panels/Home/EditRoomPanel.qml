import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import RostikObjects 1.0

Rectangle {
    property QtRoom qtRoom;

    function show(qtroom)
    {
        qtRoom = qtroom;
        visible = true;
    }

    color: "black";
    opacity: 0.9;
    ScrollView {
        height: parent.height;
        width: parent.width * 0.78;
        anchors.horizontalCenter: parent.horizontalCenter;

        ColumnLayout {
            width: parent.parent.width;
            height: parent.height;

            Item {
                Layout.fillWidth: true;
                Layout.preferredHeight: 30;
            }

            TextField {
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;
                text: qtRoom.name;

                onEditingFinished: {
                    qtRoom.name = text;
                }
            }

            Item {Layout.preferredHeight: 30;}

            Button {
                id: saveButton;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                text: "Save";
            }
        }
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: parent.visible = false;
        z: -1;
    }
}
