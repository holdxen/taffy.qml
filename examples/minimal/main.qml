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
        // column 的行间距是 gapHeight（row gap）；gapWidth 是列间距
        gapHeight: 8


        Repeater {
            model: 3
            delegate: TaffyLayout {
                required property int index;
                display: 'flex'
                clip: true
                Text {
                    text: index
                    TaffyLayout.flexGrow: 1
                    TaffyLayout.flexShrink: 1
                    TaffyLayout.flexBasis: '0%'
                }
                Text {
                    text: 'end'
                }
            }
        }
    }
}
