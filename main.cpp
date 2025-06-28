#include "misc.h"
#include "planifierseance.h"
#include "absencedatabasemanager.h"
#include "manageslectedsectionmodule.h"
#include "sectionmodel.h"
#include "modulemodel.h"
#include "etudiantsmodel.h"
#include "typecoursmodel.h"
#include "seancemodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QtQuickControls2/QQuickStyle>


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
    QQuickStyle::setStyle("Fusion");
    QQmlApplicationEngine engine;

    AbsenceDatabaseManager dbManager;
    ManageSlectedSectionModule sectionManager;
    SectionModel sectionModel;
    ModuleModel moduleModel;
    SectionModel wSectionModel;
    EtudiantsModel etudiantsModel;
    TypeCoursModel typeCoursModel;
    PlanifierSeance planifierSeance;
    SeanceModel seanceModel;
    engine.rootContext()->setContextProperty("databaseManager", &dbManager);
    engine.rootContext()->setContextProperty("wSectionModel", &wSectionModel);
    engine.rootContext()->setContextProperty("sectionModel", &sectionModel);
    engine.rootContext()->setContextProperty("yearMonthString", init());
    engine.rootContext()->setContextProperty("manageSection", &sectionManager);
    engine.rootContext()->setContextProperty("moduleModel", &moduleModel);
    engine.rootContext()->setContextProperty("etudiantsModel", &etudiantsModel);
    engine.rootContext()->setContextProperty("typeCoursModel", &typeCoursModel);
    engine.rootContext()->setContextProperty("planifierSeance", &planifierSeance);
    engine.rootContext()->setContextProperty("seanceModel", &seanceModel);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GestionAbsences", "Main");
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&sectionModel,&SectionModel::loadSections);
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&typeCoursModel,&TypeCoursModel::load);
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&seanceModel,&SeanceModel::loadSeances);
    dbManager.initialize();

    return app.exec();
}
