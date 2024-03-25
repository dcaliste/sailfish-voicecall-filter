cmake_minimum_required(VERSION 3.10)

project(sailfish-voicecall-filter
	VERSION 0.2
	DESCRIPTION "A voice call filter for Sailfish OS")

set(CMAKE_AUTOMOC ON)

set(QT_MIN_VERSION "5.6.0")
find_package(Qt5 ${QT_MIN_VERSION} COMPONENTS Core DBus Test REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OFONO ofono REQUIRED)
pkg_check_modules(MLITE mlite5 IMPORTED_TARGET REQUIRED)
pkg_check_modules(GLIB glib-2.0 IMPORTED_TARGET REQUIRED)
pkg_check_modules(GIO gio-2.0 IMPORTED_TARGET REQUIRED)
pkg_check_modules(DBUS dbus-1 IMPORTED_TARGET REQUIRED)
pkg_check_modules(COMM commhistory-qt5 IMPORTED_TARGET REQUIRED)

#include(CTest)
include(GNUInstallDirs)

add_subdirectory(lib)
add_subdirectory(daemon)
add_subdirectory(plugin)
add_subdirectory(tests)
