#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include "misc.h"
#include "absencedatabasemanager.h"
#include "manageslectedsectionmodule.h"
#include "sectionmodel.h"
#include "modulemodel.h"

QString init() {
    QDate currentDate = QDate::currentDate();
    auto month {currentDate.month()};
    if (month<=8) {
        return QString::number(currentDate.year()-1)+" - "+QString::number(currentDate.year());
    }
    else {
        return QString::number(currentDate.year())+" - "+QString::number(currentDate.year()+1);
    }
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AbsenceDatabaseManager dbManager;
    ManageSlectedSectionModule sectionManager;
    SectionModel sectionModel;
    ModuleModel moduleModel;
    SectionModel wSectionModel;
    engine.rootContext()->setContextProperty("databaseManager", &dbManager);
    engine.rootContext()->setContextProperty("wSectionModel", &wSectionModel);
    engine.rootContext()->setContextProperty("sectionModel", &sectionModel);
    engine.rootContext()->setContextProperty("yearMonthString", init());
    engine.rootContext()->setContextProperty("manageSection", &sectionManager);
    engine.rootContext()->setContextProperty("moduleModel", &moduleModel);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GestionAbsences", "Main");
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&sectionModel,&SectionModel::loadSections);
    dbManager.initialize();

    return app.exec();
}
