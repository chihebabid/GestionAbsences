#include "misc.h"
#include "planifierseance.h"
#include "absencedatabasemanager.h"
#include "manageslectedsectionmodule.h"
#include "sectionmodel.h"
#include "modulemodel.h"
#include "etudiantsmodel.h"
#include "typecoursmodel.h"
#include "seancemodel.h"
#include "absencemodel.h"
#include "presencemodel.h"
#include "printermanage.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QtQuickControls2/QQuickStyle>
#include <QApplication>


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
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");
    QQmlApplicationEngine engine;

    AbsenceDatabaseManager dbManager;
    ManageSlectedSectionModule sectionManager;
    SectionModel sectionModel;
    ModuleModel moduleModel,moduleModelSynthese;
    SectionModel wSectionModel;
    EtudiantsModel etudiantsModel;
    TypeCoursModel typeCoursModel;
    PlanifierSeance planifierSeance;
    SeanceModel seanceModel;
    AbsenceModel absenceModel{nullptr,&seanceModel};
    PresenceModel presenceModel;
    PrinterManage printerManage;
    engine.rootContext()->setContextProperty("databaseManager", &dbManager);
    engine.rootContext()->setContextProperty("wSectionModel", &wSectionModel);
    engine.rootContext()->setContextProperty("sectionModel", &sectionModel);
    engine.rootContext()->setContextProperty("yearMonthString", init());
    engine.rootContext()->setContextProperty("manageSection", &sectionManager);
    engine.rootContext()->setContextProperty("moduleModel", &moduleModel);
    engine.rootContext()->setContextProperty("moduleModelSynthese", &moduleModelSynthese);
    engine.rootContext()->setContextProperty("etudiantsModel", &etudiantsModel);
    engine.rootContext()->setContextProperty("typeCoursModel", &typeCoursModel);
    engine.rootContext()->setContextProperty("planifierSeance", &planifierSeance);
    engine.rootContext()->setContextProperty("seanceModel", &seanceModel);
    engine.rootContext()->setContextProperty("absenceModel", &absenceModel);
    engine.rootContext()->setContextProperty("presenceModel", &presenceModel);
    engine.rootContext()->setContextProperty("printerManage", &printerManage);

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
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&presenceModel,&PresenceModel::loadFromDatabase);
    dbManager.initialize();
    QObject::connect(&printerManage,&PrinterManage::s_printAbsence, [&absenceModel,&printerManage]() {
        printerManage.setAbsenceModel(&absenceModel);
        printerManage.imprimerAbsenceSeance();
    });
    return app.exec();
}
