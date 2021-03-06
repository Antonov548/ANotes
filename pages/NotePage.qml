import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4
import QtModel 1.0
import "../components"

ScrollablePage{
    id: page

    property string month: ""
    property string day_w: ""
    property string date: ""
    property real day: -1
    property real indexNote: -1
    property bool isListDoneOpen: false

    signal signalClose()
    signal editNote(string date)

    function popSignal(){
        if(noteOptions.isOpen){
            noteOptions.isOpen = false
            return
        }
        tableNote.setNotCompletedActionsCount(date,indexNote,modelAction.rowCount())
        signalClose()
    }

    function getLabel(parse_date){
        var string_date = parse_date.toString()

        if(parseInt(string_date[string_date.length-1]) === 1)
            return "Остался " + parse_date + " день"
        if(parseInt(string_date[string_date.length-1]) >1 && parseInt(string_date[string_date.length-1]) <5)
            return "Осталось " + parse_date + " дня"
        if(parseInt(string_date[string_date.length-1]) >= 5 || parseInt(string_date[string_date.length-1]) === 0)
            return "Осталось " + parse_date + " дней"
    }

    FontLoader{
        id: titleFont
        source: "qrc:/font/header_font.ttf"
    }

    header: Rectangle{
        width: parent.width
        height: 50
        color: ApplicationSettings.isDarkTheme ? "#1B1B1B" : "white"
        visible: true

        Button{
            id: btn_back
            width: height
            height: parent.height/1.8
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            icon.name: "back"
            icon.color: ApplicationSettings.isDarkTheme ? "#D7D7D7" : "#444444"
            padding: 0
            background: Rectangle{
                width: Math.max(btn_back.height,btn_back.width)+10
                height: Math.max(btn_back.height,btn_back.width)+10
                color: ApplicationSettings.isDarkTheme ? "#3F3F3F" : "#E4E4E4"
                radius: height/2
                anchors.centerIn: parent
                opacity: btn_back.pressed ? 1 : 0

                Behavior on opacity{
                    NumberAnimation{
                        duration: 500
                        easing.type: Easing.OutExpo
                    }
                }
            }

            onClicked: {popSignal()}
        }

        SettingsButton{
            height: parent.height/1.8 - 5
            width: height + 5
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            onClicked: function(){noteOptions.isOpen = true}
        }

        Rectangle{
            width: parent.width
            height: 1
            visible: page.contentYPosition
            color: ApplicationSettings.isDarkTheme ? "#505050" : "#C5C5C5"
            anchors.bottom: parent.bottom
            opacity: Math.abs(page.contentYPosition)/100
        }
    }

    backgroundColor: ApplicationSettings.isDarkTheme ? "#1B1B1B" : "white"

    content: Column{
        anchors.fill: parent
        spacing: 40
        topPadding: 10
        bottomPadding: 20

        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            Label{
                anchors.horizontalCenter: parent.horizontalCenter
                text: day + " " + month + ", " + day_w
                color: ApplicationSettings.isDarkTheme ? "silver" : "#4E4E4E"
                font.family: titleFont.name
                font.pixelSize: 30
            }
            Label{
                anchors.horizontalCenter: parent.horizontalCenter
                text: {
                    var hours_count = (new Date(date+"T24:00:00") - Date.now())/(1000*60*60)
                    if(hours_count<0)
                        return "Заметка устарела"
                    if(hours_count >0 && hours_count < 24)
                        return "Сегодня"
                    if(hours_count > 24)
                        getLabel(Math.floor(hours_count/24))
                }
                color: "#909090"
                font.family: ApplicationSettings.font
                font.pixelSize: 14
            }
        }
        Column{
            width: parent.width

            ListView{
                id: listView
                width: parent.width
                height: contentHeight
                spacing: 0
                anchors.horizontalCenter: parent.horizontalCenter
                boundsBehavior: Flickable.StopAtBounds
                addDisplaced: Transition{
                    YAnimator{
                        duration: 150
                        easing.type: Easing.Linear
                    }
                }

                delegate: ListNoteAction{}
                model: ActionModel{
                    id: modelAction
                    list: tableAction
                }
            }
            Column{
                topPadding: 10
                bottomPadding: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Button{
                    opacity: listDone.count===0 ? 0 : 1
                    Behavior on opacity {
                        OpacityAnimator{
                            duration: 300
                            easing.type: Easing.OutCirc
                        }
                    }

                    contentItem: Row{
                        spacing: 8
                        IconLabel{
                            icon.name: "open"
                            icon.color: "#909090"
                            icon.width: 14
                            icon.height: 14
                            anchors.top: parent.top
                            transform: Rotation{
                                angle: page.isListDoneOpen ? 180 : 0
                                Behavior on angle {
                                    RotationAnimation{
                                        duration: 200
                                        easing.type: Easing.OutCirc
                                    }
                                }
                                origin.x: 7
                                origin.y: 7
                            }
                        }
                        Label{
                            text: "Завершенные (" + listDone.count + ")"
                            font.pixelSize: 14
                            font.family: ApplicationSettings.font
                            color: "#909090"
                        }
                    }
                    background: Rectangle{
                        anchors.fill: parent
                        color: "transparent"
                    }
                    onClicked: page.isListDoneOpen = !page.isListDoneOpen
                }
            }
            ListView{
                id: listDone
                width: parent.width
                state: page.isListDoneOpen ? "open" : "close"
                enabled: page.isListDoneOpen
                states: [
                    State {
                        name: "open"
                        PropertyChanges {target: listDone; height: listDone.contentHeight; opacity: 1}
                    },
                    State {
                        name: "close"
                        PropertyChanges {target: listDone; height: 0; opacity: 0}
                    }
                ]
                transitions: [
                    Transition {
                        ParallelAnimation{
                            PropertyAnimation{property: "height"; duration: 300; easing.type: Easing.OutCirc}
                            PropertyAnimation{property: "opacity"; duration: 400; easing.type: Easing.OutCirc}
                        }
                    }
                ]
                spacing: 0
                anchors.horizontalCenter: parent.horizontalCenter
                boundsBehavior: Flickable.StopAtBounds
                clip: true
                addDisplaced: Transition{
                    YAnimator{
                        duration: 150
                        easing.type: Easing.Linear
                    }
                }

                delegate: ListNoteDoneAction{}
                model: ActionModelDone{
                    id: modelActionDone
                    list: tableAction
                }
            }
        }
    }
    Column{
        visible: (listView.count === 0 && !page.isListDoneOpen)
        opacity: (listView.count === 0 && !page.isListDoneOpen) ? 1 : 0
        Behavior on opacity{
            NumberAnimation{
                duration: 300
                easing.type: Easing.OutCirc
            }
        }
        anchors.centerIn: parent
        spacing: 5

        Label{
            text: "Дел больше не осталось"
            font.pixelSize: 18
            font.family: ApplicationSettings.font
            font.bold: true
            color: ApplicationSettings.isDarkTheme ? "silver" : "#454545"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label{
            text: "Заметку можно удалить"
            font.pixelSize: 15
            font.family: ApplicationSettings.font
            color: "#909090"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    NoteOptionsOverlay{id: noteOptions; isOpen: false}
}
