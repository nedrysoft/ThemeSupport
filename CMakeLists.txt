#
# Copyright (C) 2020 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 31/03/2021.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-DQT_NO_KEYWORDS)

add_definitions(-DNEDRYSOFT_LIBRARY_THEMESUPPORT_EXPORT)

# create the font awesome library

project(ThemeSupport)

# discover which Qt version is available

if (NOT DEFINED QT_VERSION_MAJOR)
    if (DEFINED USE_QT_VERSION)
        set(QT_VERSION_MAJOR ${USE_QT_VERSION})
        message(STATUS "Qt${QT_VERSION_MAJOR} has been manually selected")
    else()
        message(STATUS "Detecting Qt version")

        find_package(Qt6 COMPONENTS Core QUIET)
        find_package(Qt5 COMPONENTS Core QUIET)

        if (Qt6_FOUND)
            set(QT_VERSION_MAJOR 6)
        elseif(Qt5_FOUND)
            set(QT_VERSION_MAJOR 5)
        else()
            message(FATAL_ERROR "No valid Qt version was set, and none could be found")
        endif()
        message(STATUS "Detecting Qt version - done")
    endif()

    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core QUIET)

    if (NOT Qt${QT_VERSION_MAJOR}_FOUND)
        message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} could not be found")
    endif()

    message(STATUS "Qt major version: ${QT_VERSION_MAJOR}")
endif()

# end of qt selection/detection

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

set(Qt_LIBS Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

set(library_SOURCES
    src/ThemeDialog.cpp
    src/ThemeDialog.h
    src/ThemeSupportConfigurationWidget.cpp
    src/ThemeSupportConfigurationWidget.h
    src/ThemeSupportConfigurationWidget.ui
    src/ThemeSupport.cpp
    src/ThemeSupport.h
    src/ThemeSupportSpec.h
    src/resources.qrc
)

if(APPLE)
    set(platform_SOURCES
            src/ThemeSupport_mac.mm
            src/ThemeSupportConfigurationWidget_mac.cpp
    )

    set(platform_LIBS "-framework Foundation" "-framework Cocoa" "stdc++" "objc")
elseif(UNIX)
    set(platform_SOURCES
            src/ThemeSupport_linux.cpp
            src/ThemeSupportConfigurationWidget_linux.cpp
    )

    set(platform_LIBS "")
elseif(WIN32)
    set(platform_SOURCES
            src/ThemeSupport_windows.cpp
            src/ThemeSupportConfigurationWidget_windows.cpp
    )
    set(platform_LIBS "")

    configure_file("src/Version.h.in" "Version.h")
    list(APPEND library_SOURCES "src/version.rc")
    add_definitions("-DNEDRYSOFT_MODULE_FILENAME=\"${PROJECT_NAME}.dll\"")
    add_definitions("-DNEDRYSOFT_MODULE_NAME=\"${PROJECT_NAME}\"")
endif()

add_library(${PROJECT_NAME} SHARED
    ${library_SOURCES}
    ${platform_SOURCES}
)

if(DEFINED NEDRYSOFT_THEMESUPPORT_LIBRARY_DIR)
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${NEDRYSOFT_THEMESUPPORT_LIBRARY_DIR}")
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NEDRYSOFT_THEMESUPPORT_LIBRARY_DIR}")
else()
    message(STATUS "Set NEDRYSOFT_THEMESUPPORT_LIBRARY_DIR to set the binary output dir.")
endif()

if(APPLE)
    target_link_directories(${PROJECT_NAME} PRIVATE ${NEDRYSOFT_MACHELPER_LIBRARY_DIR})
    target_include_directories(${PROJECT_NAME} PRIVATE "${NEDRYSOFT_MACHELPER_INCLUDE_DIR}")

    target_link_libraries(${PROJECT_NAME} MacHelper)
endif()

target_link_libraries(${PROJECT_NAME} ${Qt_LIBS} ${platform_LIBS})
