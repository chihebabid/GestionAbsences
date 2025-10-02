import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ComboBox {
    id: control
    font.pixelSize: 14
    leftPadding: 12
    rightPadding: 12
    topPadding: 9
    bottomPadding: 9

    // Fond du ComboBox
    background: Rectangle {
        id: comboBackground
        color: control.hovered && control.enabled ? "#0078d4" : "#ffffff"
        radius: 5
        border.color: control.activeFocus && control.enabled ? "#0078d4" : (control.enabled ? "#0078d4" : "#c0c0c0")
        border.width: control.activeFocus ? 3 : 2

        // Animation pour le changement de couleur
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }

        // Effet de clic : changement d'échelle
        scale: control.pressed ? 0.90 : 1.0
        Behavior on scale {
            NumberAnimation {
                duration: 100
            }
        }
    }

    // Texte affiché (élément sélectionné)
    contentItem: Text {
        text: control.displayText
        color: control.hovered && control.enabled ? "#ffffff" : (control.enabled ? "#0078d4" : "#c0c0c0")
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font: control.font
        leftPadding: 12
        rightPadding: control.indicator.width + control.spacing
    }

    // Indicateur (flèche déroulante)
    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        onPaint: {
            context.reset()
            context.beginPath()
            context.moveTo(0, 0)
            context.lineTo(width / 2, height)
            context.lineTo(width, 0)
            context.closePath()
            context.fillStyle = control.hovered && control.enabled ? "#ffffff" : (control.enabled ? "#0078d4" : "#c0c0c0")
            context.fill()
        }

        Connections {
            target: control
            function onPressedChanged() { canvas.requestPaint() }
            function onHoveredChanged() { canvas.requestPaint() }
            function onEnabledChanged() { canvas.requestPaint() }
        }
    }

    // Fond de la liste déroulante
    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator {}

            // Animation de défilement
            Behavior on contentY {
                NumberAnimation {
                    duration: 200
                }
            }
        }

        background: Rectangle {
            color: "#ffffff"
            border.color: "#0078d4"
            border.width: 2
            radius: 5
        }
    }

    // Style des éléments de la liste déroulante
    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: control.textRole ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
            color: highlighted ? "#ffffff" : "#0078d4"
            font: control.font
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
        background: Rectangle {
            color: highlighted ? "#0078d4" : "#ffffff"
            radius: 5
        }
    }
}
