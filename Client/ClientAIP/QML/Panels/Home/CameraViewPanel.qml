import QtQuick 2.0
import QtQuick.Controls 2.12

import RostikObjects 1.0

Rectangle {
    property QtItem qtItem;
    visible: false;
    color: "black";

    opacity: 0.9;

    Image {
        source: qtItem.image;
        anchors.fill: parent;
    }

    MouseArea {
        anchors.fill: parent;

        onPressedChanged: {
            if(pressed)
            {
                qtItem.pressCamera = true;
                qtItem.turnCamera();
            }else
            {
                qtItem.pressCamera = false;
            }
        }
    }
    Button {
        anchors {
            right: parent.right;
            top: parent.top;
        }

        opacity: 0.5;
        width: parent.width * 0.3;
        height: width;

        onClicked: {
            parent.visible = false;
        }
    }
}
