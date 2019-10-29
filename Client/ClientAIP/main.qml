import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import RostikObjects 1.0;

import "QML/Panels/Home";
import "QML/Panels/ConnectPanel";

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    HomePanel {
        anchors.fill: parent;
    }

    ConnectPanel {
        anchors.fill: parent;
    }
}
