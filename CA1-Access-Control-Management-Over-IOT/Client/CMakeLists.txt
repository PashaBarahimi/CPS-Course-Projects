cmake_minimum_required(VERSION 3.5)

project(MonitoringSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS WebSockets)

set(PROJECT_SOURCES
        main.cpp
        windows/cpsmainwindow.cpp
        windows/cpsmainwindow.h
)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.rc")


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MonitoringSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        assets/UT.ico
        ${app_icon_resource_windows}
        windows/cpshistorywindow.h windows/cpshistorywindow.cpp
        windows/cpswindowsapitools.h
        widgets/cpstextfield.h widgets/cpstextfield.cpp
        widgets/cpsbutton.h widgets/cpsbutton.cpp
        widgets/cpslabel.h widgets/cpslabel.cpp
        websocket/cpswebsocketclient.h websocket/cpswebsocketclient.cpp
        websocket/cpswebsocketrequest.h websocket/cpswebsocketrequest.cpp
        websocket/cpswebsocketresponse.h websocket/cpswebsocketresponse.cpp
        users/cpsuser.h users/cpsuser.cpp
        users/cpsrfiduser.h users/cpsrfiduser.cpp
        cpsapplication.h cpsapplication.cpp
    )

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/assets/UT.ico")

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QWidgetsProhect APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MonitoringSystem SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MonitoringSystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if(WIN32)
    target_link_libraries(MonitoringSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets dwmapi)
else()
    target_link_libraries(MonitoringSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
endif()

target_link_libraries(MonitoringSystem PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QWidgetsProhect)
endif()
set_target_properties(MonitoringSystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MonitoringSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MonitoringSystem)
endif()
