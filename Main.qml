import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Controls 6.8

Window {
    id: window
    width: 380
    height: 760
    visible: true
    title: qsTr("Hello World")

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Row {
        id: row
        y: 40
        width: 200
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: button
            width: 95
            text: "最新"
        }

        Button {
            id: button1
            width: 95
            text: qsTr("最热")
        }

        Button {
            id: button2
            width: 95
            text: qsTr("热帖")
        }

        Button {
            id: button3
            width: 95
            text: qsTr("精华")
        }
    }

    Column {
        id: column
        y: 80
        width: 200
        height: 640
        anchors.left: parent.left
        anchors.right: parent.right

        ListView {
            id: listView
            anchors.fill: parent
            model: ListModel {
                ListElement {
                    name: "Red"
                    colorCode: "red"
                }

                ListElement {
                    name: "Green"
                    colorCode: "green"
                }

                ListElement {
                    name: "Blue"
                    colorCode: "blue"
                }

                ListElement {
                    name: "White"
                    colorCode: "white"
                }
            }
            delegate: Row {
                spacing: 5
                Rectangle {
                    width: 100
                    height: 20
                    color: colorCode
                }

                Text {
                    width: 100
                    text: name
                }
            }
        }
    }

    Row {
        id: row2
        x: 0
        y: 0
        width: 200
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right

        TextInput {
            id: textInput
            width: 190
            height: 40
            font.pixelSize: 12
        }

        Button {
            id: button9
            width: 190
            text: qsTr("Button")
        }
    }

    Row {
        id: row1
        y: 720
        width: 200
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0


        Button {
            id: button5
            y: 0
            width: 76
            text: qsTr("VAJP")
        }

        Button {
            id: button6
            y: 0
            width: 76
            text: qsTr("KOAD")
        }

        Button {
            id: button7
            y: 0
            width: 76
            text: qsTr("EAPO")
        }

        Button {
            id: button8
            y: 0
            width: 76
            text: qsTr("CAFL")
        }
        Button {
            id: button4
            y: 0
            width: 76
            text: qsTr("ANGM")
        }
    }

}
