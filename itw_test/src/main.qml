import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import ValuesModel 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Median calculator")

    FileDialog {
        id: idFileDialog
        title: qsTr("Please choose a JSON file")
        folder: "."
        onAccepted: {
            ValuesModel.loadValues(idFileDialog.fileUrl);
        }
        onRejected: {

        }

    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Rectangle {
           color:"aquamarine"
           Layout.fillWidth: true
           height: 80

           RowLayout {
               anchors.fill: parent

               Button {
                   text: qsTr("Open file")
                   onClicked: {
                       idFileDialog.open();
                   }
               }

               Item {
                   Layout.fillWidth: true
               }

               Text {
                   text: qsTr("Result: ")
               }

                Rectangle {
                    width: 200
                    height: 60
                    TextArea {
                        id: idMedianResult
//                        readOnly: true
                         anchors.fill: parent
                         text: ValuesModel.median
                    }
                }

               Rectangle {
                   width: 60
                   height: 60
                   Image {
                       anchors.fill: parent
                       source: "qrc:/content-copy.png"
                   }
                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                            idMedianResult.copy();
                       }
                   }
               }
           } // RowLayout
        } // Rectangle


        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: idContainer
            border.width: 2
            border.color: "black"

            ListView {
                anchors.fill: parent
                clip: true
                model: ValuesModel.values
                delegate: Rectangle {
                    height: 40
                    width: idContainer.width
                    color: index %2 ? "grey" : "aquamarine"
                    Text {
                        anchors.centerIn: parent
                        text: modelData
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 80
            Text {
                anchors.centerIn: parent
                text: qsTr("Some values are out of ranges! [-70, +2.28E9]")
                color: "red"
                visible: ValuesModel.error
            }
        }



    }

}
