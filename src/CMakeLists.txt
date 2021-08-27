cmake_minimum_required(VERSION 3.10)

project(TopRss)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019_64\\")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

set(SOURCE
    ItemDelegate.cpp
    ItemListModel.cpp
    ItemRenderCompactWidget.cpp
    ItemRenderWidget.cpp
    main.cpp
    RssItem.cpp
    RssItemsFetcher.cpp
    RssParser.cpp
    TopRss.cpp
    UrlFetcher.cpp)

set(HEADER
    ItemDelegate.h
    ItemListModel.h
    ItemRenderCompactWidget.h
    ItemRenderer.h
    ItemRenderWidget.h
    RssItem.h
    RssItemsFetcher.h
    RssParser.h
    TextHelpers.h
    TopRss.h
    UrlFetcher.h)

set(UI
    ItemRenderCompactWidget.ui
    ItemRenderWidget.ui
    TopRss.ui)

set(QRC
    TopRss.qrc)

source_group("Ui" FILES ${UI})
source_group("Resources" FILES ${QRC})

add_executable(TopRss WIN32 ${SOURCE} ${HEADER} ${UI} ${QRC})

target_link_libraries(TopRss Qt5::Widgets Qt5::Xml Qt5::Network)

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

    add_custom_command(TARGET TopRss POST_BUILD 
    COMMAND ${qt5_install_prefix}/bin/windeployqt.exe "${_full_bin_path}")
endif()
