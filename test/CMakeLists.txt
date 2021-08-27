cmake_minimum_required(VERSION 3.10)

project(TopRss_Tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019_64\\")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Test REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

set(SOURCE
    RssItem_Tests.cpp
    RssParser_Tests.cpp
    TopRss_Tests.cpp
    UnityBuild.cpp)

add_executable(TopRss_Tests ${SOURCE})

set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
add_subdirectory(gtest)
enable_testing()

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
)

link_directories(
    ${CMAKE_BINARY_DIR}/src/${CMAKE_BUILD_TYPE}
    ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}
)

target_link_libraries(TopRss_Tests gtest Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Test)

set_property(TARGET TopRss_Tests PROPERTY CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)
    execute_process(
        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt5_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string(
        APPEND _full_bin_path
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
    )

    add_custom_command(TARGET TopRss_Tests POST_BUILD 
    COMMAND ${qt5_install_prefix}/bin/windeployqt.exe "${_full_bin_path}")
endif()
