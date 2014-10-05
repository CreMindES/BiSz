import QtQuick 2.3

FocusScope {
    id: wrapper

    signal accepted

    property alias text: input.text
    property alias hint: hint.text
    property alias prefix: prefix.text

    Image {
        id: searchIcon
        width:  parent.height - 2
        height: parent.height - 2
        source: "images/searchIcon.png"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        z: 2
    }

    Rectangle {
       anchors.left: searchIcon.left
       anchors.leftMargin: searchIcon.height + 15
       anchors.fill: parent
       border.color: "#707070"
       radius: 4

       Text {
           id: hint
           anchors { fill: parent; leftMargin: 14 }
           verticalAlignment: Text.AlignVCenter
           text: qsTr("Termék...")
           font.pixelSize: 18
           color: "#707070"
           opacity: input.length ? 0 : 1
       }

       Text {
           id: prefix
           anchors {
               left: parent.left;
               leftMargin: 14;
               verticalCenter: parent.verticalCenter
           }
           verticalAlignment: Text.AlignVCenter
           font.pixelSize: 18
           color: "#707070"
           opacity: !hint.opacity
       }

//       TODO: implement suffix

       TextInput {
           id: input
           focus: true
           anchors {
               left: prefix.right;
               right: resetIcon.left
               rightMargin: 7
               top: parent.top;
               bottom: parent.bottom
           }
           verticalAlignment: Text.AlignVCenter
           font.pixelSize: 18
           color: "#707070"
           onAccepted: wrapper.accepted()
           clip: true
       }

       Image {
           id: resetIcon
           width:  parent.height * 0.75
           height: parent.height * 0.75
           source: "images/clearSearchLineEdit.jpg"
           anchors.right: parent.right
           anchors.rightMargin: 7
           anchors.verticalCenter: parent.verticalCenter
           MouseArea {
               anchors { fill: parent; margins: -10 }
               onClicked: input.text = ""
           }
           visible: input.length ? 1 : 0
           opacity: 0.4
       }
    }
}
