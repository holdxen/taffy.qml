import QtQuick
import QtQuick.Controls
import Taffy 1.0

ApplicationWindow {
    id: root
    width: 480
    height: 320
    visible: true
    title: qsTr("taffy minimal example")

    TaffyItem {
        anchors.fill: parent
        anchors.margins: 24
        display: TaffyTypes.DisplayFlex
        flexDirection: TaffyTypes.FlexDirectionColumn
        color: "#3b82f6"

        Rectangle {
            width: 200; height: 40
            color: "#22c55e"
        }
        Rectangle {
            width: 200; height: 40
            color: "#f59e0b"
        }
    }
}
