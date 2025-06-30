cmake_minimum_required(VERSION 3.16)

project(GestionAbsences VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)



qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appGestionAbsences
    absencedatabasemanager.cpp manageslectedsectionmodule.cpp main.cpp
)

qt_add_qml_module(appGestionAbsences
    URI GestionAbsences
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES absencedatabasemanager.h absencedatabasemanager.cpp
    SOURCES manageslectedsectionmodule.h manageslectedsectionmodule.cpp
    QML_FILES GestionSections.qml
    SOURCES misc.h
    SOURCES misc.cpp
    SOURCES sectionmodel.h sectionmodel.cpp
    QML_FILES WelcomeInterface.qml
    QML_FILES GestionModules.qml
    SOURCES modulemodel.h modulemodel.cpp
    QML_FILES GestionEtudiants.qml
    SOURCES etudiantsmodel.h etudiantsmodel.cpp
    QML_FILES PopupDate.qml
    QML_FILES Espacement.qml
    SOURCES
    SOURCES planifierseance.h planifierseance.cpp
    SOURCES typecoursmodel.h typecoursmodel.cpp
    SOURCES seancemodel.h seancemodel.cpp
    SOURCES absencemodel.h absencemodel.cpp
    SOURCES presencemodel.h presencemodel.cpp
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

target_link_libraries(appGestionAbsences
    PRIVATE Qt6::Quick Qt6::Sql Qt6::QuickControls2
)
target_link_libraries(appGestionAbsences PRIVATE Qt6::Core Qt6::QuickControls2)
target_link_libraries(appGestionAbsences PRIVATE Qt6::Core)
target_link_libraries(appGestionAbsences PRIVATE Qt6::Core)
target_link_libraries(appGestionAbsences PRIVATE Qt6::Core)
target_link_libraries(appGestionAbsences PRIVATE Qt6::Core)
target_link_libraries(appGestionAbsences PRIVATE Qt6::Core)



include(GNUInstallDirs)
install(TARGETS appGestionAbsences
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
