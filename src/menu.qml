import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0

Rectangle {
    id: menu

    objectName: "qmlMenu"

    property alias productPreviewImage: imageProductThumbnail
    property string productThumbnailPath

    SystemPalette { id: myPalette; colorGroup: SystemPalette.Active }

    width: 200
    height: 700

    color: myPalette.window

    ColumnLayout {
        id: menuLayout

        width: parent.width
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        Rectangle {
            id: spacer1
            width: parent.width
            height: 50
            color: myPalette.window
        }

        Image {
            id: logo

            objectName: "biglogo"
            signal clicked()

            source: "qrc:/images/logo.png"

            height: 145
            width: 145
            sourceSize.width: 145
            sourceSize.height: 145
            fillMode: Image.PreserveAspectFit
            smooth: true
            antialiasing: true

            anchors.horizontalCenter: parent.horizontalCenter

            MouseArea {
                z: 1
                anchors.fill: parent
                onClicked: {
                    logo.clicked()
                    console.log("BiSz 0.4.0")
                    //Qt.createComponent("about.qml").createObject(parent, {});
                }
            }
        }

        Rectangle {
            id: spacer2
            width: parent.width
            height: 20
            color: myPalette.window
        }

        ColumnLayout {
            id: menuButtonLayout

            //Layout.alignment: Qt.AlignCenter
            anchors.horizontalCenter: parent.horizontalCenter

            MyButton {
                id: buttonNew
                objectName: "newButton"
                text: "Új termék"
                buttonIcon: !buttonHovered ? "qrc:/images/menuAdd.png" :
                                             "qrc:/images/menuAddGreen.png"

            }

            MyButton {
                id: buttonModify
                objectName: "modifyButton"
                text: "Módosítás"
                buttonIcon: "qrc:/images/menuModify.png"
            }

            MyButton {
                id: buttonDelete
                objectName: "deleteButton"
                text: "Törlés"
                buttonIcon: !buttonHovered ? "qrc:/images/menuDelete.png" :
                                             "qrc:/images/menuDeleteRed.png"
            }

            Rectangle {
                id: spacer3
                width: parent.width
                height: 10
                color: myPalette.window
            }

            MyButton {
                id: buttonPrint
                objectName: "printButton"
                text: "Nyomtatás"
                buttonIcon: "qrc:/images/menuPrint.png"

                //anchors.top: buttonDelete.bottom
                //anchors.topMargin: 20
            }

            Rectangle {
                id: menuDivider
                //x: -15
                width: parent.width
                height: 40
                color: myPalette.window

                Rectangle {
                    width: 180
                    height: 2
                    color: "#e3e3e3"
                    anchors.verticalCenter: parent.verticalCenter
                    x: -15
                }
            }

            MyButton {
                id: buttonOptions
                objectName: "optionsButton"
                text: "Beállítások"
                buttonIcon: "qrc:/images/menuOptions.png"

            }

            Rectangle {
                id: spacer4
                width: parent.width
                height: 10
                color: myPalette.window
            }

            MyButton {
                id: buttonExportCSV
                objectName: "exportCSVButton"
                text: "Export CSV"
                buttonIcon: "qrc:/images/menuExport.png"

                //anchors.top: buttonOptions.bottom
                //anchors.topMargin: 15
            }

            MyButton {
                id: buttonImportCSV
                objectName: "importCSVButton"
                text: "Import CSV"
                buttonIcon: "qrc:/images/menuImport.png"
            }
        }

        Rectangle {
            id: spacer5
            width: parent.width
            height: 15
            color: myPalette.window
        }

        Rectangle {
            id: imageProductThumbnailContainer

            width: 162
            height: 162

            color: "transparent"

            border.width: 1
            border.color: "#859c94"

            radius: 10
            //clip: true

            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                id: imageProductThumbnail
                objectName: "productThumbnailImage"

                signal clicked()
                z: 1

                //sourceChanged transition
                //onSourceChanged: console.log(productThumbnailPath)
                source: productThumbnailPath

                //anchors.fill: imageProductThumbnailContainer
                height: 160
                width: 160
                sourceSize.width: 160
                sourceSize.height: 160
                fillMode: Image.PreserveAspectFit
                smooth: true
                antialiasing: true

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    hoverEnabled: true
                    anchors.fill: imageProductThumbnail
                    onClicked: {
                        parent.clicked()
                    }
                    onEntered: imageProductThumbnailContainer.border.color = "#4b5853"
                    onExited: imageProductThumbnailContainer.border.color = "#859c94"
                }
            }

            Text {
                id: productImageNotAvailableText
                color: "#000"

                anchors.fill: parent

                text: "Termékelőnézet nem érhető el"
                opacity: 0.51
                clip: false

                maximumLineCount: 3
                z: -2

                font.pointSize: 10
                font.family: "Helvetica"
                font.bold: true

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }
}
