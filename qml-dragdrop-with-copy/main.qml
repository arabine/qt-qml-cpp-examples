import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import "."

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("QML drag-drop with icon copy (ghost)")


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        z: 0

        ToolBar {
            Layout.fillWidth: true
            z: 20
        }


        Item { Layout.fillHeight: true }


        DropItem {
            z: 10
        }

        Item { Layout.fillHeight: true }
    }
}
