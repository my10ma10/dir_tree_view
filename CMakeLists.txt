cmake_minimum_required(VERSION 3.5)

project(dir_view_test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        dir_tree_model.h
        dir_tree_model.cpp
        dir_tree_view.h
        dir_tree_view.cpp
        dir_filter_model.h
        dir_filter_model.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dir_view_test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET dir_view_test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(dir_view_test SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(dir_view_test
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(dir_view_test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(dir_view_test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(dir_view_test)
endif()

install(TARGETS dir_view_test RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_NAME "dir_tree_view")
set(CPACK_PACKAGE_VERSION "1.0")
set(CPACK_PACKAGE_CONTACT "sokol28.nik@gmail.com")

set(CPACK_DEBIAN_PACKAGE_NAME "dir_view_test")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "sokol")
set(CPACK_DEBIAN_PACKAGE_CONTACT "sokol28.nik@gmail.com")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A graphical application for viewing the file system tree with support for filtering, sorting, and searching")

set(CPACK_GENERATOR "DEB")
include(CPack)
