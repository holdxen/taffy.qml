import QtQuick
import QtQuick.Controls
import Taffy 1.0

ApplicationWindow {
    id: root
    width: 480
    height: 360
    visible: true
    title: qsTr("taffy nested flex example")

    TaffyLayout {
        id: outer
        anchors.fill: parent
        anchors.margins: 10
        display: "flex"
        flexDirection: "column"

        Rectangle {
            id: bar
            height: 100
            color: "#94a3b8"
        }

        TaffyLayout {
            id: inner
            display: "flex"
            flexDirection: "column"
            gapHeight: 10
            alignItems: 'flex-end'
            TaffyLayout.flexGrow: 1
            TaffyLayout.flexShrink: 1
            TaffyLayout.flexBasis: "0%"

            Text {
                id: label
                text: "Example"
            }

            Button {
                id: btn
                text: "ok"
                onClicked: console.log("button clicked")
            }

            Rectangle {
                id: red
                color: "red"
                TaffyLayout.flexGrow: 1
                TaffyLayout.flexShrink: 1
                TaffyLayout.flexBasis: "0%"
            }
        }
    }

}
