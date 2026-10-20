import QtQuick
import QtQuick.Controls
import Taffy 1.0

ApplicationWindow {
    id: root
    width: 480
    height: 320
    visible: true
    title: qsTr("taffy minimal example")

    Column {
        anchors.centerIn: parent
        spacing: 12

        TaffyItem {
            width: 240
            height: 80
            mode: LayoutEngine.Flex
            spacing: 8
            color: "#3b82f6"
        }

        Label {
            text: qsTr("TaffyItem with Rust layout engine")
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
