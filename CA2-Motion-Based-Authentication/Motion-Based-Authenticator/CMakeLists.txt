cmake_minimum_required(VERSION 3.16)

project(MotionBasedAuthenticator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES ( "$ENV{EIGEN3_INCLUDE_DIR}" )

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Qml)
find_package(Qt6 6.4 REQUIRED COMPONENTS Sensors)

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
        SOURCES accelerometerhandler.h accelerometerhandler.cpp
        SOURCES movementpattern.h movementpattern.cpp
        SOURCES patternrecognizer.h patternrecognizer.cpp
        SOURCES acceleration.h
        SOURCES kalmanfilter.h kalmanfilter.cpp
        SOURCES gyroscopehandler.h gyroscopehandler.cpp
        SOURCES rotation.h
        SOURCES patternrecorder.h patternrecorder.cpp
        SOURCES authenticator.h authenticator.cpp
)

set_target_properties(MotionBasedAuthenticator PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(MotionBasedAuthenticator PRIVATE Qt6::Quick Qt6::Qml)
target_link_libraries(MotionBasedAuthenticator PRIVATE Qt6::Sensors)


include(GNUInstallDirs)
install(TARGETS MotionBasedAuthenticator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
