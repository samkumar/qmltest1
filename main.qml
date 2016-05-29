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
        width: Window.width / 2
        height: Window.height
    }

    color: "red"
}
