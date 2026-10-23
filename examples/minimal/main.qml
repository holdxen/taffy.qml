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
        flexDirection: "row"
        gapWidth: 8

        Text {
            text: 'ok =================='
            color: 'black'
            TaffyLayout.flexShrink: 0
        }

        Rectangle {
            color: 'red'
            TaffyLayout.width: '200px'
            TaffyLayout.height: '100px'
            TaffyLayout.flexGrow: 1
        }
    }
}
