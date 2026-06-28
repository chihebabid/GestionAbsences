
#include "misc.h"
#include "studentmanager.h"
#include "sessionplanner.h"
#include "absencedatabasemanager.h"
#include "manageselectedsectionmodule.h"
#include "sectionmodel.h"
#include "modulemodel.h"
#include "studentsmodel.h"
#include "coursetypemodel.h"
#include "sessionmodel.h"

#include "presencemodel.h"
#include "printermanager.h"

#include "summarytablemodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QtQuickControls2/QQuickStyle>
#include <QApplication>





int main(int argc, char *argv[])
{
    /*if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
    // if using org.kde.desktop, ensure we use kde style if possible
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORMTHEME")) {
        qputenv("QT_QPA_PLATFORMTHEME", "kde");
    }*/
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");
    QQmlApplicationEngine engine;

    AbsenceDatabaseManager dbManager;    
    SectionModel sectionModel;
    SectionModel wSectionModel;    
    CourseTypeModel courseTypeModel;
    SessionModel sessionModel;
    AbsenceModel absenceModel{nullptr,&sessionModel};
    PresenceModel presenceModel;
    PrinterManager printerManage;
    SummaryTableModel summaryTableModel{};
    auto studentManager {new StudentManager{}};

    engine.rootContext()->setContextProperty("databaseManager", &dbManager);
    engine.rootContext()->setContextProperty("wSectionModel", &wSectionModel);
    engine.rootContext()->setContextProperty("sectionModel", &sectionModel);              
    engine.rootContext()->setContextProperty("courseTypeModel", &courseTypeModel);
    engine.rootContext()->setContextProperty("sessionModel", &sessionModel);
    engine.rootContext()->setContextProperty("absenceModel", &absenceModel);
    engine.rootContext()->setContextProperty("presenceModel", &presenceModel);
    engine.rootContext()->setContextProperty("printerManage", &printerManage);
    engine.rootContext()->setContextProperty("summaryTableModel", &summaryTableModel);
    engine.rootContext()->setContextProperty("studentManager", studentManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("com.enhancetech.absences", "Main");
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&sectionModel,&SectionModel::loadSections);
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&courseTypeModel,&CourseTypeModel::load);
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&sessionModel,&SessionModel::loadSessions);
    QObject::connect(&dbManager,&AbsenceDatabaseManager::databaseReady,&presenceModel,&PresenceModel::loadFromDatabase);
    dbManager.initialize();

    // Impression les absences pour une séance
    QObject::connect(&printerManage,&PrinterManager::s_printAbsence, [&absenceModel,&printerManage]() {
        printerManage.setModel(&absenceModel);
        printerManage.imprimerAbsenceSeance();
    });

    // Imprimer les absences pour l'ensemble des séances
    QObject::connect(&printerManage,&PrinterManager::s_printSynthese, [&summaryTableModel,&printerManage]() {
        printerManage.setModel(&summaryTableModel);
        printerManage.imprimerSynthese();
    });
    return app.exec();
}
