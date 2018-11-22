#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QInputMethod>
#include <QDebug>

#include "database.h"
#include "notemodel.h"
#include "actionmodel.h"
#include "tablenote.h"
#include "applicationsettings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_DisableShaderDiskCache);

    QGuiApplication app(argc, argv);

    ApplicationSettings *settings = ApplicationSettings::AppSettingsInstance();
    settings->setFile("settings.json");

    QObject::connect(settings,SIGNAL(commit()),app.inputMethod(),SLOT(commit()));

    qmlRegisterType<NoteModel>("QtModel",1,0,"NoteModel");
    qmlRegisterType<ActionModel>("QtModel",1,0,"ActionModel");

    TableNote tableNote;
    TableAction tableAction;
    Database db(tableNote,tableAction);

    tableNote.getNotesDatabase();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tableNote",&tableNote);
    engine.rootContext()->setContextProperty("tableAction",&tableAction);
    engine.rootContext()->setContextProperty("ApplicationSettings",settings);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    //settings->initializeAndroidKeyboard();

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
