import QtQuick
import QtQuick.VirtualKeyboard
import QtQuick.Controls 6.8

Window {
    id: window
    width: 380
    height: 900
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
        width: 200
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: button
            width: 95
            text: qsTr("Button")
        }

        Button {
            id: button1
            width: 95
            text: qsTr("Button")
        }

        Button {
            id: button2
            width: 95
            text: qsTr("Button")
        }

        Button {
            id: button3
            width: 95
            text: qsTr("Button")
        }
    }

    Column {
        id: column
        y: 40
        width: 200
        height: 820
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
        id: row1
        width: 200
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: button4
            y: 860
            width: 76
            text: qsTr("Button")
        }

        Button {
            id: button5
            y: 860
            width: 76
            text: qsTr("Button")
        }

        Button {
            id: button6
            y: 860
            width: 76
            text: qsTr("Button")
        }

        Button {
            id: button7
            y: 860
            width: 76
            text: qsTr("Button")
        }

        Button {
            id: button8
            y: 860
            width: 76
            text: qsTr("Button")
        }
    }
}
