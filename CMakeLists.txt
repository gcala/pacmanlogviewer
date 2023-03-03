project(plv)
# ==================================================================================
# Information to update before to release this package.
include(GNUInstallDirs)
SET(PLV_VERSION "1.5.1" )
set( APPLICATION_EXECUTABLE "plv" )
set( SHAREDIR ${CMAKE_INSTALL_FULL_DATADIR} )
configure_file(config-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-version.h )
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
# ==================================================================================

cmake_minimum_required(VERSION 3.14)
#add_definitions(-std=c++0x)
set( CMAKE_AUTOMOC ON )

find_package(Qt6 COMPONENTS Core Widgets Gui LinguistTools Sql Network)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets Gui LinguistTools Sql Network)
endif()

# Handle Translations, pick all pacmanlogviewer_* files from trans directory.
file( GLOB TRANS_FILES ${CMAKE_SOURCE_DIR}/lang/pacmanlogviewer_*.ts)
set( TRANSLATIONS ${TRANS_FILES} )

set(plv_SRCS actioncolumndelegate.cpp
             dialog.cpp
             pkgnameslineedit.cpp
             datecolumndelegate.cpp
             pkgnamecompleter.cpp
             aboutdialog.cpp
             filterswidget.cpp
             main.cpp)

set(plv_RCS ${CMAKE_SOURCE_DIR}/pacmanlogviewer.qrc)

# use ui files
set(plv_UI filterswidget.ui
           aboutdialog.ui
           dialog.ui)
           
include_directories(${QtCore_INCLUDE_DIRS}
                ${QtWidgets_INCLUDE_DIRS}
                ${QtSql_INCLUDE_DIRS}
                ${QtNetwork_INCLUDE_DIRS}
                ${CMAKE_CURRENT_BINARY_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR})

qt_add_resources(plv_RC_SRCS ${plv_RCS} )
qt_wrap_ui(plv_UI_HEADERS ${plv_UI})
qt_add_translation(plv_I18N ${TRANSLATIONS})


add_executable(plv main.cpp ${plv_SRCS} ${plv_RC_SRCS} ${plv_UI_HEADERS} ${plv_I18N})

target_link_libraries(plv PRIVATE Qt::Core Qt::Widgets Qt::Sql Qt::Network)

install(TARGETS plv RUNTIME DESTINATION bin)

install(FILES ${plv_I18N} DESTINATION ${CMAKE_INSTALL_DATADIR}/${APPLICATION_EXECUTABLE}/i18n)

install(FILES ${CMAKE_SOURCE_DIR}/it.cuteworks.pacmanlogviewer.desktop 
        DESTINATION share/applications )
install(FILES ${CMAKE_SOURCE_DIR}/it.cuteworks.pacmanlogviewer.appdata.xml 
        DESTINATION share/metainfo)
install(FILES ${CMAKE_SOURCE_DIR}/images/16x16/plv.png
        DESTINATION share/icons/hicolor/16x16/apps/ RENAME plv.png)
install(FILES ${CMAKE_SOURCE_DIR}/images/24x24/plv.png
        DESTINATION share/icons/hicolor/24x24/apps/ RENAME plv.png)
install(FILES ${CMAKE_SOURCE_DIR}/images/32x32/plv.png
        DESTINATION share/icons/hicolor/32x32/apps/ RENAME plv.png)
install(FILES ${CMAKE_SOURCE_DIR}/images/48x48/plv.png
        DESTINATION share/icons/hicolor/48x48/apps/ RENAME plv.png)
install(FILES ${CMAKE_SOURCE_DIR}/images/64x64/plv.png
        DESTINATION share/icons/hicolor/64x64/apps/ RENAME plv.png)
install(FILES ${CMAKE_SOURCE_DIR}/images/128x128/plv.png
        DESTINATION share/icons/hicolor/128x128/apps/ RENAME plv.png)
