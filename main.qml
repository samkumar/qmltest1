import QtQuick 2.6
import QtQuick.Window 2.2
import custom 1.0

Window {
    visible: true

    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            if (Window.width > 300) {
                Qt.quit();
            } else {
                console.log("Updating");
                tc.update();
            }
        }
    }

    Rectangle {
        x: Window.width * 0.75
        y: Window.height / 3
        width: Window.width / 2
        height: Window.height / 3
        color: "lightgray"
    }

    TimeComponent {
        id: tc
        width: 2 * Window.width / 3
        height: Window.height

        PinchArea {
            anchors.fill: parent
            onPinchStarted: console.log("started");
            onPinchUpdated: tc.updateZoom(pinch.scale, false);
            onPinchFinished: tc.updateZoom(pinch.scale, true);

            MouseArea {
                anchors.fill: parent

                onPositionChanged: tc.updatePosition(mouseX, mouseY);
                onPressed: tc.updatePosition(mouseX, mouseY);
                onReleased: tc.updatePosition(mouseX, mouseY);
                onWheel: {
                    tc.updateZoom(1.0 + (wheel.angleDelta.y / 1000.0), true);
                }


                /*propagateComposedEvents: true

                onClicked: mouse.accepted = false;
                onPressed: mouse.accepted = false;
                onReleased: mouse.accepted = false;
                onDoubleClicked: mouse.accepted = false;
                onPositionChanged: mouse.accepted = false;
                onPressAndHold: mouse.accepted = false;
                onClicked: {
                    console.log("Clicked!");
                }*/
            }
        }
    }

    color: "red"
}
