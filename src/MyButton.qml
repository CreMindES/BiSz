import QtQuick 2.1

Rectangle {
    id: button

    property string buttonText: "Push me"
    property string buttonIcon
    property color  buttonColor: "#2e79b7"

    signal clicked()

    height: 25
    width: 150
    color: buttonColor
    radius: 10
    z: 0

    anchors.topMargin: 5
    anchors.bottomMargin: 5

    MouseArea {id: mouseArea; anchors.fill: parent; onClicked: button.clicked(); hoverEnabled: true}

    Rectangle {
        id: buttonIconContainer
        height: parent.height
        width: 30
        color: "#ffffff"
        radius: parent.radius
        z: 1

        Rectangle {
            id: buttonDivider
            height: parent.height
            width: parent.radius
            x: buttonIconContainer.width-button.radius
            color: buttonIconContainer.color
            z: 3
        }

        Rectangle {
            id: buttonDividerLine
            height: buttonDivider.height
            width: 1
            anchors.right: buttonDivider.right
            color: "#4766a4"
            z: 3
        }

        Image {
            id: buttonIconImage

            source: buttonIcon

            width: 21
            height: 21

            smooth: true
            opacity: 0.8
            antialiasing: true

            anchors.centerIn: parent
            z:3
        }
    }

    Rectangle {
        id: buttonTextContainer
        z: 2
        x: buttonIconContainer.width
        height: parent.height
        width: parent.width - buttonIconContainer.width
        color: buttonColor
        radius: parent.radius

        /*
        gradient: Gradient {
            GradientStop { position: 0 ; color: "#000" }
            GradientStop { position: 1 ; color: "#fff" }
        }
        */
    }

    Text {
        id: buttonText_text

        text: buttonText

        color: "#ffffff"
        font.pointSize: 10
        font.family: "Helvetica"
        font.bold: true

        anchors.centerIn: buttonTextContainer
        z: 3
    }

    states:  [
        State {
           name: "mouse-down"; when: mouseArea.pressedButtons
           PropertyChanges { target: button; color: "#266293" }
           PropertyChanges { target: buttonTextContainer; opacity: 0.5; color: "#266293" }
        },

        State {
           name: "mouse-over"; when: mouseArea.containsMouse
           PropertyChanges { target: button; color: "#368bd1" }
           PropertyChanges { target: buttonTextContainer; color: "#368bd1" }
           PropertyChanges { target: buttonIconImage; opacity: 1 }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "color"; easing.type: Easing.InOutCubic; duration: 100  }
    }
}
