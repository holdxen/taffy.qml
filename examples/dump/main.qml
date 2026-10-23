import QtQuick
import QtQuick.Controls
import Taffy 1.0

ApplicationWindow {
    id: root
    width: 560
    height: 420
    visible: true
    title: qsTr("taffy dump example")

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Button {
            text: qsTr("Dump TaffyLayout + children")
            onClicked: {
                console.log("======== layout.dump() ========")
                console.log(layout.dump())
                console.log("======== layout.taffyStyle ========")
                console.log(JSON.stringify(layout.taffyStyle, null, 2))
                console.log("======== layout.dumpTree() ========")
                console.log(layout.dumpTree())
                console.log("======== child TaffyLayout.dump() ========")
                console.log(red.TaffyLayout.dump())
                console.log("======== child taffyStyle ========")
                console.log(JSON.stringify(red.TaffyLayout.taffyStyle, null, 2))
            }
        }

        TaffyLayout {
            id: layout
            width: parent.width
            height: 240
            display: "flex"
            flexDirection: "row"
            gapWidth: 8
            paddingTop: 4
            paddingBottom: 4
            paddingLeft: 4
            paddingRight: 4

            Text {
                text: qsTr("label")
            }

            Rectangle {
                id: red
                color: "#ef4444"
                TaffyLayout.height: "80px"
                TaffyLayout.flexGrow: 1
                TaffyLayout.flexBasis: "0"
            }

            Rectangle {
                id: blue
                color: "#3b82f6"
                TaffyLayout.width: "120px"
                TaffyLayout.height: "80px"
                TaffyLayout.alignSelf: "flex-start"
            }
        }
    }

    Component.onCompleted: {
        console.log("======== initial layout.dump() ========")
        console.log(layout.dump())
        console.log("======== initial dumpTree() ========")
        console.log(layout.dumpTree())
    }
}
