cmake_minimum_required(VERSION 3.16)

project(MotionBasedAuthenticator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Qml)

qt_standard_project_setup()

qt_add_executable(MotionBasedAuthenticator
    main.cpp
)

qt_add_qml_module(MotionBasedAuthenticator
    URI MotionBasedAuthenticator
    VERSION 1.0
    SOURCES
        direction.h
        angle.h
        movement.h
        movement.cpp
        backend.h
        backend.cpp
    QML_FILES
        Main.qml
        PatternPage.qml
        Notification.qml
        SensorsPage.qml
        SensorValues.qml
    RESOURCES
        qtquickcontrols2.conf
)

set_target_properties(MotionBasedAuthenticator PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(MotionBasedAuthenticator
    PRIVATE Qt6::Quick Qt6::Qml
)

include(GNUInstallDirs)
install(TARGETS MotionBasedAuthenticator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
