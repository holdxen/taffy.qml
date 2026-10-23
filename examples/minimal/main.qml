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
        }

        Rectangle {
            color: 'red'
            // TaffyLayout.width: '200x'
            // TaffyLayout.height: 'auto'
            TaffyLayout.flexGrow: 1
            TaffyLayout.flexShrink: 1
            TaffyLayout.flexBasis: '0%'
        }
    }
}
