import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import "."

// Horizontal toolbar with drag and drop tool icons
Rectangle {
    id: idStepsToolBar
    implicitHeight: 80
    border.width: 1
    border.color: "black"
    color: "teal"
    radius: 10

    ListModel {
        id: idToolBarModel

        ListElement {
            icon: "qrc:/step_screwdriver.png"
            name: "tightening"
        }
        ListElement {
            icon: "qrc:/step_pick_to_light.png"
            name: "picktolight"
        }
        ListElement {
            icon: "qrc:/step_input.png"
            name: "input"
        }
        ListElement {
            icon: "qrc:/step_user_input.png"
            name: "userInput"
        }
        ListElement {
            icon: "qrc:/step_switch.png"
            name: "switch"
        }
    }

    Component {
        id: itemComponent
        TileBody {

        }
    }

    RowLayout {
        id: idToolBarLayout

        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        spacing: 5
        property var dragedComp;

        Repeater {
            model: idToolBarModel
            delegate: Item {
                width: 64
                height: 64

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent

                    onPressed: (mouse)=> {
                        console.log("Pressed");
                        addItem(mouse.x, mouse.y);
                    }

                    onReleased: {
                        Drag.active = false;
                        if (idToolBarLayout.dragedComp !== null) {
                            idToolBarLayout.dragedComp.Drag.drop();
                            idToolBarLayout.dragedComp.destroy();
                        }
                    }

                    function addItem(x, y)
                    {
                        var positionInWindow = mapToItem(idStepsToolBar, x, y)
                        idToolBarLayout.dragedComp = itemComponent.createObject(idToolBarLayout, { title: name, imageName: icon, x: positionInWindow.x - 25, y: positionInWindow.y - 25 });
                        if (idToolBarLayout.dragedComp !== null) {
                            idToolBarLayout.dragedComp.parent = idStepsToolBar;
                            idToolBarLayout.dragedComp.z = 100;

                            idToolBarLayout.dragedComp.Drag.keys = [ "step" ]
                            idToolBarLayout.dragedComp.Drag.active = true;
                            idToolBarLayout.dragedComp.Drag.hotSpot.x = 32;
                            idToolBarLayout.dragedComp.Drag.hotSpot.y = 32;

                            mouseArea.drag.target = idToolBarLayout.dragedComp;
                            Drag.active = true;
                        }
                    }

                    TileBody {
                        anchors.fill: parent
                        title: name // titre de notre icône
                        imageName: icon // emplacement de l'icône (dans un QRC)
                    }

                }
            }
        }
        Item { Layout.fillWidth:  true }
    } // RowLayout

}

