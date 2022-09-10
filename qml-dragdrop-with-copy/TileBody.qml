import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Rectangle {
    id: idTileBodyRoot

    width: 64; height: 64
    property string title: "Empty"
    property string imageName: ""
    color: "white"

    Image {
        id: idTileImage
        anchors.centerIn: parent
        height: parent.height - 4
        width: parent.width - 4
        source: idTileBodyRoot.imageName
        fillMode: Image.PreserveAspectFit
    }

}
