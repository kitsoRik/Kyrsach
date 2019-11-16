import QtQuick 2.0

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

        onClicked: {
            qtItem.turnCamera();
        }
    }
}
