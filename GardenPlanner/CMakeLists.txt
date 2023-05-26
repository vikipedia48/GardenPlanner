cmake_minimum_required(VERSION 3.5)

project(GardenPlanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(fmt_DIR "fmt")

set(BUILD_SHARED_LIBS ON)
add_subdirectory(Qt-Secret)
set(BUILD_SHARED_LIBS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport Widgets Network)
find_package(fmt REQUIRED)




set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        worker.h worker.cpp
        plant.h plant.cpp
        worker.h worker.cpp
        activity.h activity.cpp
        login.h login.cpp login.ui
        register.h register.cpp register.ui
        creategarden.h creategarden.cpp creategarden.ui
        viewgardens.h viewgardens.cpp viewgardens.ui
        qtfunctions.h qtfunctions.cpp
        customcalendar.h customcalendar.cpp
        commonfunctions.h commonfunctions.cpp
        addactivitydialog.h addactivitydialog.cpp addactivitydialog.ui
        customplants.h customplants.cpp customplants.ui
        garden.h garden.cpp
        settings.h settings.cpp settings.ui
        welcomescreen.h welcomescreen.cpp welcomescreen.ui
        translationstrings.h translationstrings.cpp
        getactivitiesbyworkertask.h getactivitiesbyworkertask.cpp
        barrier.h barrier.cpp
        blobtoplantlisttask.h blobtoplantlisttask.cpp
        activitychecktask.h activitychecktask.cpp
        activitiesoverview.h activitiesoverview.cpp activitiesoverview.ui
        cryptography.h cryptography.cpp
        gardencookiejar.h gardencookiejar.cpp
        waitscreen.h waitscreen.cpp waitscreen.ui
        purchase.h purchase.cpp purchase.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GardenPlanner
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GardenPlanner APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

else()
    if(ANDROID)
        add_library(GardenPlanner SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GardenPlanner
            ${PROJECT_SOURCES}
        )
    endif()

endif()

target_link_libraries(
    GardenPlanner
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport Qt${QT_VERSION_MAJOR}::Network Qt-Secret fmt::fmt
)

set_target_properties(GardenPlanner PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS GardenPlanner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GardenPlanner)
endif()
