import QtQuick
import QtQuick.Controls
import Taffy 1.0

ApplicationWindow {
    id: root
    width: 480
    height: 320
    visible: true
    title: qsTr("taffy minimal example")

    TaffyLayout {
        anchors.fill: parent
        anchors.margins: 24
        display: "flex"
        flexDirection: "column"
        gapHeight: 8

        Rectangle {
            color: "#22c55e"
            TaffyLayout.height: "40px"
        }
        Rectangle {
            color: "#f59e0b"
            TaffyLayout.height: "40px"
            TaffyLayout.flexGrow: 1
        }
    }
}
