import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3


Rectangle {
    id: idStepNodeRoot
    width: 200
    height: 80
    color: "orange"
    border.width: 1
    border.color: "black"
    radius: 5

    DropArea {
        anchors.fill: parent
        keys: ["step"]
        onEntered: {
            console.log("ENTERED");
            idDropOverlay.visible = true;
        }
        onExited: {
            console.log("EXIT");
            idDropOverlay.visible = false;
        }

        onDropped: {
            console.log("DROP");
            idDropOverlay.visible = false;
            console.log(drag.source.title)
        }

        Text {
            anchors.centerIn: parent
            text: "Target item!"
        }
    }

    Rectangle {
        id: idDropOverlay
        visible: false
        anchors.fill: parent
        z: 50
        color: "grey"
        opacity: 0.5
//        border.width: 1
        radius: 5
//        border.color: "grey"
        Image {
            id: idDropOverlayImage
            anchors.centerIn: parent
            height: parent.height - 10
            width: parent.width - 10
            source: "qrc:/drop_icon.png"
            fillMode: Image.PreserveAspectFit
        }

    }

} // Rectangle
