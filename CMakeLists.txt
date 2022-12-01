cmake_minimum_required(VERSION 3.20)
project(desktop-builder C)

set(CMAKE_C_STANDARD 99)

add_executable(desktop-builder main.c)
configure_file(gtk.glade <output> COPYONLY)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(desktop-builder ${GTK3_LIBRARIES})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -rdynamic")