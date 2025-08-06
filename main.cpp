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
#include "synthesetablemodel.h"
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
    TypeCoursModel typeCoursModel;
    SeanceModel seanceModel;
    AbsenceModel absenceModel{nullptr,&seanceModel};
    PresenceModel presenceModel;
    PrinterManage printerManage;
    SyntheseTableModel syntheseTableModel{};

    engine.rootContext()->setContextProperty("databaseManager", &dbManager);
    engine.rootContext()->setContextProperty("wSectionModel", &wSectionModel);
    engine.rootContext()->setContextProperty("sectionModel", &sectionModel);
    engine.rootContext()->setContextProperty("yearMonthString", init());            
    engine.rootContext()->setContextProperty("typeCoursModel", &typeCoursModel);
    engine.rootContext()->setContextProperty("seanceModel", &seanceModel);
    engine.rootContext()->setContextProperty("absenceModel", &absenceModel);
    engine.rootContext()->setContextProperty("presenceModel", &presenceModel);
    engine.rootContext()->setContextProperty("printerManage", &printerManage);
    engine.rootContext()->setContextProperty("syntheseTableModel", &syntheseTableModel);

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

    // Imprimer les absences pour la séance en cours
    QObject::connect(&printerManage,&PrinterManage::s_printAbsence, [&absenceModel,&printerManage]() {
        printerManage.setModel(&absenceModel);
        printerManage.imprimerAbsenceSeance();
    });

    // Imprimer la synthèse pour les absences
    QObject::connect(&printerManage,&PrinterManage::s_printSynthese, [&syntheseTableModel,&printerManage]() {
        printerManage.setModel(&syntheseTableModel);
        printerManage.imprimerSynthese();
    });
    return app.exec();
}
