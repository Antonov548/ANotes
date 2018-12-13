import QtQuick 2.11
import QtQuick.Controls 2.4
import "../components"

Item{
    id: item
    property alias isOpen: overlay.isOpen

    PopupOverlay{
        id: overlay
        fromTop: true
        content: Page{
            id: page
            width: parent.width
            height: settingsColumn.implicitHeight
            clip: true

            Column{
                id: settingsColumn
                width: parent.width

                ListView{
                    id: settingsListView

                    property var sett_pages: {
                        "Использовать пароль": btn_password,
                        "Изменить пароль": btn_change_password,
                        "Темное оформление": btn_dark_theme,
                        "Сортировать по дате": btn_note_sort
                    }

                    property var handlers : {"Использовать пароль": function(){if(!ApplicationSettings.isBlock)handlers["Изменить пароль"](); else ApplicationSettings.setIsBlock(false)},
                        "Изменить пароль": function(){dialogPassword.clear();},
                        "Темное оформление": function(){ApplicationSettings.setIsDarkTheme(!ApplicationSettings.isDarkTheme)},
                        "Сортировать по дате": function(){ApplicationSettings.setIsOrder(!ApplicationSettings.isOrder); tableNote.resetList(ApplicationSettings.isOrder)}
                    }

                    property var check: {"Использовать пароль": ApplicationSettings.isBlock,
                        "Изменить пароль": ApplicationSettings.isBlock,
                        "Темное оформление": ApplicationSettings.isDarkTheme,
                        "Сортировать по дате": ApplicationSettings.isOrder
                    }

                    SettingsCheckComponent{id: btn_password}
                    SettingsCheckComponent{id: btn_dark_theme}
                    SettingsCheckComponent{id: btn_note_sort}
                    SettingsButtonCompoment{id: btn_change_password}

                    height: contentHeight
                    width: parent.width

                    boundsBehavior: Flickable.StopAtBounds

                    model: ListModel{
                        ListElement{text:"Использовать пароль"}
                        ListElement{text:"Изменить пароль"}
                        ListElement{text:"Темное оформление"}
                        ListElement{text:"Сортировать по дате"}
                    }

                    delegate: Loader{
                        id: loader
                        width: settingsListView.width

                        property string delegateTitle: model.text
                        property var delegateHandler: settingsListView.handlers[model.text]
                        property bool delegateCheck: settingsListView.check[model.text]

                        sourceComponent: settingsListView.sett_pages[model.text]
                    }
                }
            }
        }
    }
}