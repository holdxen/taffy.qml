import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Taffy 1.0

ApplicationWindow {
    id: root
    width: 1100
    height: 720
    visible: true
    title: qsTr("taffy playground")

    readonly property var displayValues: ["flex", "block", "grid", "flow-root", "none"]
    readonly property var flexDirValues: ["row", "column", "row-reverse", "column-reverse"]
    readonly property var flexWrapValues: ["nowrap", "wrap", "wrap-reverse", "balance", "balance-reverse"]
    readonly property var alignItemsValues: ["normal", "start", "end", "flex-start", "flex-end",
                                             "self-start", "self-end", "center", "baseline", "stretch"]
    readonly property var alignContentValues: ["normal", "start", "end", "flex-start", "flex-end",
                                               "center", "stretch", "space-between", "space-evenly", "space-around"]
    readonly property var gridAutoFlowValues: ["row", "column", "row-dense", "column-dense"]
    readonly property var boxSizingValues: ["border-box", "content-box"]
    readonly property var overflowValues: ["visible", "clip", "hidden", "scroll"]
    readonly property var directionValues: ["ltr", "rtl"]
    readonly property var positionValues: ["static", "relative", "absolute", "fixed", "sticky"]
    readonly property var cssFloatValues: ["none", "left", "right"]
    readonly property var clearValues: ["none", "left", "right", "both"]
    readonly property var textAlignValues: ["auto", "left", "right", "center"]

    property Item currentItem: itemA

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // ---------- control panel ----------
        Frame {
            Layout.preferredWidth: 360
            Layout.fillHeight: true
            padding: 8

            ScrollView {
                anchors.fill: parent
                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: 10

                    Label { text: qsTr("Container (TaffyLayout)"); font.bold: true }

                    Label { text: qsTr("display") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.displayValues
                        currentIndex: Math.max(0, model.indexOf(layout.display))
                        onActivated: layout.display = model[currentIndex]
                    }
                    Label { text: qsTr("flexDirection") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.flexDirValues
                        currentIndex: Math.max(0, model.indexOf(layout.flexDirection))
                        onActivated: layout.flexDirection = model[currentIndex]
                    }
                    Label { text: qsTr("flexWrap") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.flexWrapValues
                        currentIndex: Math.max(0, model.indexOf(layout.flexWrap))
                        onActivated: layout.flexWrap = model[currentIndex]
                    }
                    Label { text: qsTr("flexLineCount") }
                    SpinBox {
                        Layout.fillWidth: true
                        from: 1; to: 32
                        value: layout.flexLineCount
                        onValueModified: layout.flexLineCount = value
                    }
                    Label { text: qsTr("alignItems") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.alignItemsValues
                        currentIndex: Math.max(0, model.indexOf(layout.alignItems))
                        onActivated: layout.alignItems = model[currentIndex]
                    }
                    Label { text: qsTr("alignContent") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.alignContentValues
                        currentIndex: Math.max(0, model.indexOf(layout.alignContent))
                        onActivated: layout.alignContent = model[currentIndex]
                    }
                    Label { text: qsTr("justifyContent") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.alignContentValues
                        currentIndex: Math.max(0, model.indexOf(layout.justifyContent))
                        onActivated: layout.justifyContent = model[currentIndex]
                    }
                    Label { text: qsTr("gapWidth / gapHeight") }
                    RowLayout {
                        SpinBox {
                            Layout.fillWidth: true
                            from: 0; to: 200
                            value: layout.gapWidth
                            onValueModified: layout.gapWidth = value
                        }
                        SpinBox {
                            Layout.fillWidth: true
                            from: 0; to: 200
                            value: layout.gapHeight
                            onValueModified: layout.gapHeight = value
                        }
                    }
                    Label { text: qsTr("padding T/R/B/L") }
                    RowLayout {
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: layout.paddingTop; onValueModified: layout.paddingTop = value }
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: layout.paddingRight; onValueModified: layout.paddingRight = value }
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: layout.paddingBottom; onValueModified: layout.paddingBottom = value }
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: layout.paddingLeft; onValueModified: layout.paddingLeft = value }
                    }
                    Label { text: qsTr("gridAutoFlow") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.gridAutoFlowValues
                        currentIndex: Math.max(0, model.indexOf(layout.gridAutoFlow))
                        onActivated: layout.gridAutoFlow = model[currentIndex]
                    }
                    Label { text: qsTr("gridTemplateRows") }
                    TextField {
                        Layout.fillWidth: true
                        text: layout.gridTemplateRows
                        placeholderText: qsTr("e.g. 48px 1fr 48px")
                        onEditingFinished: layout.gridTemplateRows = text
                    }
                    Label { text: qsTr("gridTemplateColumns") }
                    TextField {
                        Layout.fillWidth: true
                        text: layout.gridTemplateColumns
                        placeholderText: qsTr("e.g. 120px 1fr")
                        onEditingFinished: layout.gridTemplateColumns = text
                    }
                    Label { text: qsTr("gridTemplateAreas") }
                    TextField {
                        Layout.fillWidth: true
                        text: layout.gridTemplateAreas
                        placeholderText: qsTr("\"hd hd\" \"main side\"")
                        onEditingFinished: layout.gridTemplateAreas = text
                    }
                    Label { text: qsTr("boxSizing") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.boxSizingValues
                        currentIndex: Math.max(0, model.indexOf(layout.boxSizing))
                        onActivated: layout.boxSizing = model[currentIndex]
                    }
                    Label { text: qsTr("overflowX / Y") }
                    RowLayout {
                        ComboBox {
                            Layout.fillWidth: true
                            model: root.overflowValues
                            currentIndex: Math.max(0, model.indexOf(layout.overflowX))
                            onActivated: layout.overflowX = model[currentIndex]
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            model: root.overflowValues
                            currentIndex: Math.max(0, model.indexOf(layout.overflowY))
                            onActivated: layout.overflowY = model[currentIndex]
                        }
                    }
                    Label { text: qsTr("scrollbarWidth") }
                    SpinBox {
                        Layout.fillWidth: true
                        from: 0; to: 50
                        value: layout.scrollbarWidth
                        onValueModified: layout.scrollbarWidth = value
                    }
                    Label { text: qsTr("direction") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.directionValues
                        currentIndex: Math.max(0, model.indexOf(layout.direction))
                        onActivated: layout.direction = model[currentIndex]
                    }

                    MenuSeparator {}

                    Label { text: qsTr("Selected item"); font.bold: true }
                    ComboBox {
                        Layout.fillWidth: true
                        textRole: "text"
                        valueRole: "item"
                        model: [
                            { text: qsTr("A — Rectangle"), item: itemA },
                            { text: qsTr("B — Text"), item: itemB },
                            { text: qsTr("C — Button"), item: itemC },
                            { text: qsTr("D — Column layout"), item: itemD },
                            { text: qsTr("E — Rectangle"), item: itemE }
                        ]
                        onActivated: root.currentItem = currentValue
                    }

                    Label { text: qsTr("width / height") }
                    RowLayout {
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "auto / 100 / 50%"
                            text: root.currentItem ? root.currentItem.TaffyLayout.width : ""
                            onEditingFinished: if (root.currentItem) root.currentItem.TaffyLayout.width = text
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "auto / 100 / 50%"
                            text: root.currentItem ? root.currentItem.TaffyLayout.height : ""
                            onEditingFinished: if (root.currentItem) root.currentItem.TaffyLayout.height = text
                        }
                    }
                    Label { text: qsTr("flexBasis / grow / shrink") }
                    RowLayout {
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "auto / 0 / 0%"
                            text: root.currentItem ? root.currentItem.TaffyLayout.flexBasis : ""
                            onEditingFinished: if (root.currentItem) root.currentItem.TaffyLayout.flexBasis = text
                        }
                        SpinBox {
                            Layout.fillWidth: true
                            from: 0; to: 20
                            value: root.currentItem ? root.currentItem.TaffyLayout.flexGrow * 10 : 0
                            onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.flexGrow = value / 10.0
                        }
                        SpinBox {
                            Layout.fillWidth: true
                            from: 0; to: 20
                            value: root.currentItem ? root.currentItem.TaffyLayout.flexShrink * 10 : 10
                            onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.flexShrink = value / 10.0
                        }
                    }
                    Label { text: qsTr("alignSelf") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: ["", "auto"].concat(root.alignItemsValues)
                        currentIndex: {
                            if (!root.currentItem) return 0
                            const v = root.currentItem.TaffyLayout.alignSelf
                            return Math.max(0, model.indexOf(v))
                        }
                        onActivated: if (root.currentItem) root.currentItem.TaffyLayout.alignSelf = model[currentIndex]
                    }
                    Label { text: qsTr("margin T/R/B/L") }
                    RowLayout {
                        SpinBox { Layout.fillWidth: true; from: -50; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.marginTop : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.marginTop = value }
                        SpinBox { Layout.fillWidth: true; from: -50; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.marginRight : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.marginRight = value }
                        SpinBox { Layout.fillWidth: true; from: -50; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.marginBottom : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.marginBottom = value }
                        SpinBox { Layout.fillWidth: true; from: -50; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.marginLeft : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.marginLeft = value }
                    }
                    Label { text: qsTr("padding T/R/B/L") }
                    RowLayout {
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.paddingTop : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.paddingTop = value }
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.paddingRight : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.paddingRight = value }
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.paddingBottom : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.paddingBottom = value }
                        SpinBox { Layout.fillWidth: true; from: 0; to: 100; value: root.currentItem ? root.currentItem.TaffyLayout.paddingLeft : 0; onValueModified: if (root.currentItem) root.currentItem.TaffyLayout.paddingLeft = value }
                    }
                    Label { text: qsTr("position / gridRow / gridColumn") }
                    ComboBox {
                        Layout.fillWidth: true
                        model: root.positionValues
                        currentIndex: {
                            if (!root.currentItem) return 0
                            return Math.max(0, model.indexOf(root.currentItem.TaffyLayout.position))
                        }
                        onActivated: if (root.currentItem) root.currentItem.TaffyLayout.position = model[currentIndex]
                    }
                    RowLayout {
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "gridRow: auto / 1 / 1 / 3"
                            text: root.currentItem ? root.currentItem.TaffyLayout.gridRow : ""
                            onEditingFinished: if (root.currentItem) root.currentItem.TaffyLayout.gridRow = text
                        }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "gridColumn"
                            text: root.currentItem ? root.currentItem.TaffyLayout.gridColumn : ""
                            onEditingFinished: if (root.currentItem) root.currentItem.TaffyLayout.gridColumn = text
                        }
                    }

                    Button {
                        Layout.fillWidth: true
                        text: qsTr("Dump layout tree")
                        onClicked: console.log(layout.dumpTree())
                    }
                }
            }
        }

        // ---------- preview ----------
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TaffyLayout {
                id: layout
                anchors.fill: parent
                anchors.margins: 12
                display: "flex"
                flexDirection: "row"
                flexWrap: "wrap"
                gapWidth: 8
                gapHeight: 8
                paddingTop: 8
                paddingBottom: 8
                paddingLeft: 8
                paddingRight: 8

                Rectangle {
                    id: itemA
                    color: "#ef4444"
                    TaffyLayout.width: "160px"
                    TaffyLayout.height: "80px"
                    Text { anchors.centerIn: parent; text: "A"; color: "white" }
                }

                Rectangle {
                    id: itemB
                    color: "#e2e8f0"
                    radius: 4
                    TaffyLayout.flexGrow: 1
                    TaffyLayout.flexBasis: "120px"
                    TaffyLayout.height: "80px"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("B — flex item")
                        color: "#0f172a"
                    }
                }

                Button {
                    id: itemC
                    text: qsTr("C")
                    TaffyLayout.height: "48px"
                }

                TaffyLayout {
                    id: itemD
                    display: "flex"
                    flexDirection: "column"
                    gapHeight: 4
                    TaffyLayout.width: "180px"
                    TaffyLayout.height: "120px"
                    Rectangle { color: "#22c55e"; TaffyLayout.height: "36px" }
                    Rectangle { color: "#3b82f6"; TaffyLayout.flexGrow: 1 }
                }

                Rectangle {
                    id: itemE
                    color: "#a855f7"
                    TaffyLayout.width: "120px"
                    TaffyLayout.height: "80px"
                    Text { anchors.centerIn: parent; text: "E"; color: "white" }
                }
            }
        }
    }
}
