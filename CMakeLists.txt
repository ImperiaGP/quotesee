cmake_minimum_required(VERSION 2.6)

project(QuoteSee)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
#add_definitions (-DQT_NO_DEBUG_OUTPUT)
#add_definitions (-DQT_NO_WARNING_OUTPUT)
#add_definitions (-DNO_DEBUG_TO_FILE)

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

add_subdirectory (src)
add_subdirectory (icons)
add_subdirectory (data_engine)
add_subdirectory (plotting)
