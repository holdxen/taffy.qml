import QtQuick
import QtQuick.Controls
import Taffy 1.0

ApplicationWindow {
    id: root
    width: 480
    height: 360
    visible: true
    title: qsTr("taffy grid example")

    TaffyLayout {
        id: grid
        anchors.fill: parent
        anchors.margins: 16

        display: "grid"
        gridTemplateRows: "48px 1fr 48px"
        gridTemplateColumns: "120px 1fr 120px"
        gridTemplateAreas: '"hd hd hd" "nav main side" "ft ft ft"'
        gapWidth: 8
        gapHeight: 8

        // header
        Rectangle {
            id: headerRect
            color: "#3b82f6"
            TaffyLayout.gridRow: "1"
            TaffyLayout.gridColumn: "1 / 4"

            Text {
                anchors.centerIn: parent
                text: qsTr("header")
                color: "white"
            }
        }

        // nav
        Rectangle {
            id: navRect
            color: "#22c55e"
            TaffyLayout.gridRow: "2"
            TaffyLayout.gridColumn: "1"

            Text {
                anchors.centerIn: parent
                text: qsTr("nav")
            }
        }

        // main
        Rectangle {
            id: mainRect
            color: "#f59e0b"
            TaffyLayout.gridRow: "2"
            TaffyLayout.gridColumn: "2"

            Text {
                anchors.centerIn: parent
                text: qsTr("main")
            }
        }

        // side
        Rectangle {
            id: sideRect
            color: "#a855f7"
            TaffyLayout.gridRow: "2"
            TaffyLayout.gridColumn: "3"

            Text {
                anchors.centerIn: parent
                text: qsTr("side")
            }
        }

        // footer
        Rectangle {
            id: footerRect
            color: "#64748b"
            TaffyLayout.gridRow: "3"
            TaffyLayout.gridColumn: "1 / 4"

            Text {
                anchors.centerIn: parent
                text: qsTr("footer")
                color: "white"
            }
        }
    }


}
