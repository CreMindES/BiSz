import QtQuick 2.3

Rectangle {
    id: mainWindow
    width: 800
    height: 600

    SearchField {
        id: searchField
        y: -100
        x: parent.width / 2 - width / 2
        height: 40
        width: 500
        NumberAnimation on y {
            from: mainWindow.height
            to: mainWindow.height / 2 - 25
            duration: 500
            easing.type: Easing.OutCirc
        }
        onAccepted: {
            console.log("SIGNAL: Search field submitted")
            dd.start()
            pd1.visible = true;
            //y = 100
        }

        PropertyAnimation {
            id: dd
            target: searchField
            property: "y"
            to: searchField.height
            duration: 500
            easing.type: Easing.OutCirc
        }
    }

    Product {
        id: pd1
        height: 100
        width: 800
        visible: false
        anchors.top: searchField.bottom
        anchors.topMargin: 50
        name: "Bútorláb 60x60mm alumínium profilból"
        number: "1645"
        category: "Bútorlábak -> b6-153303"
        description: "- króm\n- magasság 100mm\n- állíthatóság +15mm"
        imageUrl: "images/products/1001.jpg"
        price: 1212
        price2: 1212 / 1.27
    }
}
