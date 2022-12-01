cmake_minimum_required(VERSION 3.20)
project(gtk_demo C)

set(CMAKE_C_STANDARD 99)

add_executable(gtk_demo main.c)


FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(gtk_demo ${GTK3_LIBRARIES})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -rdynamic")