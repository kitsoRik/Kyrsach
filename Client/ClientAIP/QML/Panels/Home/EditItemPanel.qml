import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import RostikObjects 1.0

Rectangle {
    property QtItem qtItem;

    function show(qtitem)
    {
        qtItem = qtitem;
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
                text: qtItem.name;

                onEditingFinished: {
                    qtItem.name = text;
                }
            }

            TextField {
                Layout.preferredHeight: 50;
                Layout.fillWidth: true;
            }

            RowLayout {
                Layout.fillWidth: true;
                Layout.preferredHeight: 50;

                Button {
                    Layout.fillHeight: true;
                    Layout.fillWidth: true;

                    text: "Add";

                    onClicked: {
                        qtItem.itemActions.addItemAction(qtItem);
                    }
                }
            }

            Repeater {
                model: qtItem.itemActions.size;
                Rectangle {
                    property ItemAction itemAction: qtItem.itemActions.itemActionAt(index);
                    Layout.fillWidth: true;
                    Layout.minimumHeight: 200;

                    color: "transparent";
                    border {
                        width: 1;
                        color: "white";
                    }

                    ColumnLayout {
                        anchors.fill: parent;

                        Text {
                            Layout.fillWidth: true;
                            Layout.preferredHeight: 30;

                            text: "Title";
                            color: "white";
                            horizontalAlignment: Qt.AlignHCenter;
                        }
                        Button {
                            Layout.fillWidth: true;
                            Layout.preferredHeight: 30;
                            text: "Delete";

                            onClicked: {
                                parent.parent.itemAction.deleteAction();
                            }
                        }

                        ComboBox {
                            id: typeBox;
                            Layout.fillWidth: true;
                            Layout.preferredHeight: 50;

                            model: {
                                let size = qtItem.room.qtItems.itemsSize;
                                let array = new Array(size+1);
                                array[0] = "Send message";
                                for(let i = 1; i < size+1; i++)
                                {
                                    let it = qtItem.room.qtItems.itemAt(i-1);
                                    array[i] = it.type + " " + it.pin;
                                }
                                return array;
                            }

                            onCurrentIndexChanged: {
                                if(currentIndex == 0)
                                {
                                    parent.parent.itemAction.receiver = null;
                                    return;
                                }
                                let receiver = qtItem.room.qtItems.itemAt(currentIndex - 1);
                                parent.parent.itemAction.receiver = receiver;
                            }
                        }

                        ComboBox {
                            id: whenBox;
                            Layout.fillWidth: true;
                            Layout.preferredHeight: 50;

                            model: ["When on", "When off"];

                            onCurrentTextChanged: {
                                if(currentText == "When on")
                                {
                                    parent.parent.itemAction.needValue = 1;
                                }else
                                {
                                    parent.parent.itemAction.needValue = 0;

                                }
                            }
                        }

                        ComboBox {
                            visible: {
                                if(typeBox.currentIndex == 0)
                                    return false;
                                return true;
                            }

                            Layout.fillWidth: true;
                            Layout.preferredHeight: 50;

                            model: ["On", "Off"];

                            onCurrentIndexChanged: {
                                parent.parent.itemAction.setValue = currentIndex ? 0 : 1;
                            }

                        }
                    }

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
