cmake_minimum_required(VERSION 3.16)

project(GestionAbsences VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Sql QuickControls2 PrintSupport Svg)


set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.9)

qt_add_executable(appGestionAbsences
    main.cpp
    #resources.qrc
)

qt_add_qml_module(appGestionAbsences
    URI com.enhancetech.absences
    VERSION 1.0
    RESOURCES resources.qrc
    QML_FILES
        Main.qml GestionSections.qml WelcomeInterface.qml GestionModules.qml AboutDialog.qml DrawerItem.qml Espacement.qml  PopupDate.qml
        PageDrawer.qml GestionEtudiants.qml Synthese.qml
    SOURCES absencedatabasemanager.h absencedatabasemanager.cpp
    SOURCES manageslectedsectionmodule.h manageslectedsectionmodule.cpp    
    SOURCES misc.h
    SOURCES misc.cpp
    SOURCES sectionmodel.h sectionmodel.cpp
    SOURCES modulemodel.h modulemodel.cpp    
    SOURCES etudiantsmodel.h etudiantsmodel.cpp
    SOURCES planifierseance.h planifierseance.cpp
    SOURCES typecoursmodel.h typecoursmodel.cpp
    SOURCES seancemodel.h seancemodel.cpp
    SOURCES absencemodel.h absencemodel.cpp
    SOURCES presencemodel.h presencemodel.cpp
    SOURCES printermanage.h printermanage.cpp    
    SOURCES synthesetablemodel.h synthesetablemodel.cpp
    SOURCES misc/basemodel.h misc/basemodel.cpp
    SOURCES misc/basemanager.h misc/basemanager.cpp
    SOURCES misc/globals.h
    SOURCES misc/studentmodel.h misc/studentmodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appGestionAbsences PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGestionAbsences
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appGestionAbsences PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
)

target_link_libraries(appGestionAbsences
    PRIVATE Qt6::Quick Qt6::Sql Qt6::QuickControls2
)
target_link_libraries(appGestionAbsences PRIVATE Qt6::Core Qt6::QuickControls2 Qt6::PrintSupport Qt6::Svg)


include(GNUInstallDirs)
install(TARGETS appGestionAbsences
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
