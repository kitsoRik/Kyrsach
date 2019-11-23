import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import RostikObjects 1.0

Rectangle {
    property QtRoom globalQtRoom: null;
    color: "gray";

    Drawer {
        id: drawer;
        width: parent.width * 2/3;
        height: parent.height;
        z: -1;
        ColumnLayout {
            anchors.fill: parent;

            Rectangle {
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                Text {
                    anchors.fill: parent;
                    font.pointSize: 24;
                    color: "black";
                    text: {
                        let resultText;
                        if(client.isAdmin)
                            resultText = "Admin: ";
                        else
                            resultText = "User: ";
                        resultText += client.login;
                        return resultText;
                    }
                }
            }

            Button {
                Layout.fillWidth: true;
                Layout.preferredHeight: 30;
                anchors.margins: 20;
                hoverEnabled: true;

                visible: client.isAdmin;

                background: Rectangle {
                    anchors.fill: parent;
                    radius: 5;
                    color: {
                        if(parent.pressed)
                            return "yellow";
                        if(parent.hovered)
                            return "blue";
                        return "cyan";
                    }
                }

                text: "Add room";

                onClicked: {
                    addRoomPanel.visible = true;
                }
            }

            ListView {
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                clip: true;
                model: client.rooms.roomsSize;

                delegate: Rectangle {
                    property QtRoom qtRoom: client.rooms.roomAt(index);
                    width: parent.width;
                    height: 50;
                    color: "#ff0000";

                    RowLayout {
                        anchors.fill: parent;
                        Text {
                            Layout.fillHeight: true;
                            Layout.preferredWidth: 40;
                            text: parent.parent.qtRoom.name;
                            color: "white";
                            font.pointSize: 20;
                        }

                        Item {
                            Layout.fillHeight: true;
                            Layout.fillWidth: true;


                            MouseArea {
                                anchors.fill: parent;

                                onClicked: {
                                    globalQtRoom = qtRoom;
                                    drawer.close();
                                }
                            }
                        }

                        Image {
                            visible: true;
                            Layout.fillHeight: true;
                            Layout.preferredWidth: height;
                            source: "qrc:///settings.png"
                            MouseArea {
                                anchors.fill: parent;

                                onClicked: {
                                    editRoomPanel.show(qtRoom);
                                }
                            }
                        }

                    }
                }
            }

            Rectangle {
                visible: client.isAdmin;
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                RowLayout {
                    anchors.fill: parent;

                    Text {
                        Layout.fillHeight: true;
                        Layout.fillWidth: true;
                        font.pointSize: 24;
                        color: "black";
                        text: "Users";
                    }

                    Button {
                        Layout.fillHeight: true;
                        Layout.fillWidth: true;
                        font.pointSize: 24;
                        text: "Add user";

                        onClicked: {
                            addUserPanel.visible = true;
                        }
                    }
                }
            }

            ListView {
                visible: client.isAdmin;
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                clip: true;
                model: client.usersObject.usersSize;

                delegate: Rectangle {
                    property UserObject userObject: client.usersObject.userAt(index);
                    width: parent.width;
                    height: 50;
                    color: "yellow";

                    RowLayout {
                        anchors.fill: parent;
                        Text {
                            Layout.fillHeight: true;
                            Layout.preferredWidth: 40;
                            text: parent.parent.userObject.login;
                            color: "blue";
                            font.pointSize: 20;
                        }

                        Item {
                            Layout.fillHeight: true;
                            Layout.fillWidth: true;
                        }

                        Rectangle {
                            Layout.fillHeight: true;
                            Layout.preferredWidth: height;

                            color: parent.parent.userObject.online ? "green" : "red";
                        }

                    }
                }
            }

        }
    }

    ColumnLayout {
        visible: globalQtRoom != null;
        anchors.fill: parent;

        Rectangle {
            Layout.fillWidth: true;
            Layout.preferredHeight: 40;

            visible: globalQtRoom != null;

            Text {
                Layout.fillHeight: true;
                Layout.preferredWidth: 40;
                anchors.fill: parent;
                text: globalQtRoom.name;
            }
        }

        ListView {
            Layout.fillHeight: true;
            Layout.fillWidth: true;

            model: globalQtRoom.qtItems.itemsSize;

            delegate: Rectangle {
                property QtItem qtItem: globalQtRoom.qtItems.itemAt(index);
                width: parent.width;
                height: 70;
                color: "orange";

                RowLayout {
                    anchors.fill: parent;

                    Text {
                        Layout.fillHeight: true;
                        Layout.fillWidth: true;
                        text: parent.parent.qtItem.name;
                        font.pixelSize: height * 4/7;
                        verticalAlignment: Qt.AlignVCenter;
                        elide: Qt.ElideMiddle;
                    }

                    Slider {
                        Layout.fillHeight: true;
                        Layout.fillWidth: true;
                        visible: qtItem.type === "SERVO";

                        from: 0;
                        to: 180;

                        onPressedChanged: {
                            if(pressed)
                                return;
                            parent.parent.qtItem.turnServo(value);
                        }
                    }

                    Item {Layout.fillWidth: true;}

                    Image {
                        id: editButton;
                        Layout.fillHeight: true;
                        Layout.preferredWidth: height;
                        source: "qrc:///settings.png";

                        MouseArea {
                            anchors.fill: parent;

                            onClicked: {
                                editItemPanel.show(qtItem);
                            }
                        }

                        PropertyAnimation {
                            target: editButton;
                            running: true;
                            property: "rotation";
                            from: 0;
                            to: 360;
                            duration: 5000;
                            loops: Animation.Infinite;
                        }
                    }

                    Rectangle {
                        Layout.fillHeight: true;
                        Layout.preferredWidth: height;
                        color: parent.parent.qtItem.on ? "green" : "red";
                    }
                }

                MouseArea {
                    anchors.fill: parent;

                    z: -1;

                    onClicked: {
                        console.log("C");
                        if(parent.qtItem.type == "LED")
                            parent.qtItem.turnLed();
                        if(parent.qtItem.type == "MAGSIG")
                            parent.qtItem.turnMagSig();
                        if(parent.qtItem.type == "CAMERA")
                        {
                            cameraViewPanel.qtItem = parent.qtItem;
                            cameraViewPanel.visible = true;
                        }
                    }
                }
            }
        }

        Button {
            Layout.preferredHeight: 30;
            Layout.fillWidth: true;
            visible: client.isAdmin;

            text: "Add item";

            onClicked: {
                addItemPanel.visible = true;
            }
        }
    }

    AddRoomPanel {
        id: addRoomPanel;
        visible: false;

        anchors.fill: parent;
    }

    AddItemPanel {
        id: addItemPanel;
        visible: false;

        anchors.fill: parent;

        onVisibleChanged: {
            drawer.visible = !visible;
        }
    }

    AddUserPanel {
        id: addUserPanel;
        visible: false;

        anchors.fill: parent;

        onVisibleChanged: {
            drawer.visible = !visible;
        }
    }

    EditItemPanel {
        id: editItemPanel;
        visible: false;

        anchors.fill: parent;

        onVisibleChanged: {
            drawer.visible = !visible;
        }
    }

    EditRoomPanel {
        id: editRoomPanel;
        visible: false;

        onVisibleChanged: {
            drawer.visible = !visible;
        }

        anchors.fill: parent;
        z: 1;
    }

    CameraViewPanel {
        id: cameraViewPanel;
        visible: false;
        anchors.fill: parent;


    }
}
