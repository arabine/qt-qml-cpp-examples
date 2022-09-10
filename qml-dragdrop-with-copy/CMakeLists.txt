cmake_minimum_required(VERSION 3.16)

project(qml-dragdrop-with-copy VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

set(SOURCES main.cpp)
qt_add_resources(SOURCES resources.qrc)

qt_add_executable(appqml-dragdrop-with-copy
    ${SOURCES}
)

qt_add_qml_module(appqml-dragdrop-with-copy
    URI qml-dragdrop-with-copy
    VERSION 1.0
    QML_FILES main.qml ToolBar.qml TileBody.qml DropItem.qml
)


set_target_properties(appqml-dragdrop-with-copy PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqml-dragdrop-with-copy
    PRIVATE Qt6::Quick)

install(TARGETS appqml-dragdrop-with-copy
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
