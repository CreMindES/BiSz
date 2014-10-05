import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: product;

    property url   imageUrl
    property alias name:        name.text
    property alias number:      number.text
    property alias description: description.text
    property alias category:    category.text
    property double price : 0
    property double price2 : 0

    width: 800
    height: 100
    color: "#d4d4d4"

    Image {
        id: image
        source: imageUrl
        height: product.height - 10
        width:  product.height - 10

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
    }

    Rectangle {

        id: properties

        width: 630

        anchors.left: image.right
        //anchors.right: trade.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5

        color: "transparent"

        Row {
            id: identifier
            spacing: 10

            Text {
                id: number
                font.pointSize: 12
                color: "grey"
            }
            Text {
                id: name
                font.pointSize: 12
            }
        }

        Text {
            id: category
            color: "grey"
            anchors.bottom: properties.bottom
        }
        // TODO: replace this Text element with a grid
        Text {
            id: description
            anchors.top: identifier.bottom
            anchors.topMargin: 5
        }
    }

    Item {
        id: trade

        width: parent.width - image.width - properties.width
        height: parent.height

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: properties.right
        anchors.right: parent.right
        anchors.margins: 5

        Item {
            id: item1
            //anchors.fill: parent

            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -30

            Text {
                id: bruttoPrice

                text: price.toFixed(2) + " Ft."
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 12

            }


            Text {
                id: nettoPrice

                text: "(nettó:  " + price2.toFixed(2) + " Ft.)"
                font.pointSize: 8

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: bruttoPrice.bottom
                anchors.topMargin: 10
            }
        }
    }

}
