cmake_minimum_required(VERSION 3.16)

project(GestionAbsences VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Sql QuickControls2 PrintSupport Svg)


set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.9)

qt_add_executable(appGestionAbsences
    main.cpp    
)

qt_add_qml_module(appGestionAbsences
    URI com.enhancetech.absences
    VERSION 1.0
    RESOURCES resources.qrc
    QML_FILES
        Main.qml ui/pages/GestionSections.qml ui/pages/WelcomeInterface.qml ui/pages/GestionModules.qml ui/dialogs/AboutDialog.qml ui/components/DrawerItem.qml ui/components/Espacement.qml ui/dialogs/PopupDate.qml
        ui/components/PageDrawer.qml ui/pages/GestionEtudiants.qml ui/pages/Synthese.qml
    # Moved sources organized in folders
    SOURCES managers/absencedatabasemanager.h managers/absencedatabasemanager.cpp
    SOURCES modules/manageselectedsectionmodule.h modules/manageselectedsectionmodule.cpp
    SOURCES misc/misc.h misc/misc.cpp
    SOURCES models/sectionmodel.h models/sectionmodel.cpp
    SOURCES models/modulemodel.h models/modulemodel.cpp
    SOURCES planner/sessionplanner.h planner/sessionplanner.cpp
    SOURCES models/typecoursmodel.h models/typecoursmodel.cpp
    SOURCES models/absencemodel.h models/absencemodel.cpp
    SOURCES models/presencemodel.h models/presencemodel.cpp
    SOURCES managers/printermanager.h managers/printermanager.cpp
    SOURCES models/summarytablemodel.h models/summarytablemodel.cpp
    SOURCES misc/basemodel.h misc/basemodel.cpp
    SOURCES misc/basemanager.h misc/basemanager.cpp
    SOURCES misc/globals.h
    SOURCES misc/studentmodel.h misc/studentmodel.cpp
    SOURCES misc/studentmanager.h misc/studentmanager.cpp
    SOURCES models/coursetypemodel.h models/coursetypemodel.cpp
    SOURCES models/sessionmodel.h models/sessionmodel.cpp
    SOURCES models/studentsmodel.h models/studentsmodel.cpp
    SOURCES db/dbsessionplanner.h db/dbsessionplanner.cpp
    SOURCES db/dbclient.h db/dbclient.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/managers
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/planner
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
