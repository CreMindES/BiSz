import QtQuick 2.0

Rectangle {
    id: menu

    SystemPalette { id: myPalette; colorGroup: SystemPalette.Active }

    width: 200
    height: 180

    color: myPalette.window

    Rectangle {
        id: buttonContiner
        anchors.horizontalCenter: parent.horizontalCenter
        width: 150

        MyButton {
            id: buttonPrint
            objectName: "printButton"
            buttonText: "Nyomtatás"
            buttonIcon: "qrc:/images/print.png"
        }


        Rectangle {
            id: menuDivider
            x: -15
            width: 180
            height: 2

            color: "#e3e3e3"
            anchors.topMargin: 20
            anchors.top: buttonPrint.bottom
        }


        MyButton {
            id: buttonOptions
            objectName: "optionsButton"
            buttonText: "Beállítások"
            buttonIcon: "qrc:/images/gear.png"
//            onClicked: console.log(objectName + " clicked")

            anchors.top: menuDivider.bottom
            anchors.topMargin: 20
        }

        MyButton {
            id: buttonExportCSV
            objectName: "exportCSVButton"
            buttonText: "Export CSV"
            buttonIcon: "qrc:/images/rightArrow2.png"

            anchors.top: buttonOptions.bottom
            anchors.topMargin: 15
        }

        MyButton {
            id: buttonImportCSV
            objectName: "importCSVButton"
            buttonText: "Import CSV"
            buttonIcon: "qrc:/images/leftArrow2.png"

            anchors.top: buttonExportCSV.bottom
        }
    }
}
