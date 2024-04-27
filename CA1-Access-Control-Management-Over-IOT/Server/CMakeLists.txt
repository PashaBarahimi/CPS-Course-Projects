cmake_minimum_required(VERSION 3.14)

project(Server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS HttpServer)

add_executable(Server
  main.cpp
  monitoringsystemauthenticator.h monitoringsystemauthenticator.cpp
  monitoringsystemuser.h monitoringsystemuser.cpp
  utils.h utils.cpp
  rfidauthenticationitem.h rfidauthenticationitem.cpp
  rfidauthenticationhistory.h rfidauthenticationhistory.cpp
  websocketserver.h websocketserver.cpp
  websocketrequest.h websocketrequest.cpp
  websocketresponse.h websocketresponse.cpp
  data/monitoring_system_history.json
  data/monitoring_system_users.json
  httpserver.h httpserver.cpp
  httpresponse.h httpresponse.cpp
  httprequest.h httprequest.cpp
  rfidauthenticator.h rfidauthenticator.cpp
  data/rfid_tags.json
)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::WebSockets)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::HttpServer)

include(GNUInstallDirs)
install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
