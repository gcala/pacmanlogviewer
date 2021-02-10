project(plv)
# ==================================================================================
# Information to update before to release this package.

# pacmanlogviewer info
SET(PLV_VERSION "1.4.2" )
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-version.h )
# ==================================================================================

cmake_minimum_required(VERSION 3.14)
#add_definitions(-std=c++0x)
set( CMAKE_AUTOMOC ON )

option(BUILD_WITH_QT5 "Build Pacman Log Viewer with Qt5, if found" OFF)

if( BUILD_WITH_QT5 )
    find_package(Qt5Core)
    if( Qt5Core_DIR )
        message(STATUS "Found Qt5! Be aware that Qt5-support is still experimental and not officially supported!")
        ADD_DEFINITIONS(-fPIC)
        find_package(Qt5Widgets REQUIRED)
        find_package(Qt5Gui REQUIRED)
        find_package(Qt5LinguistTools REQUIRED)
        find_package(Qt5Sql REQUIRED)
            find_package(Qt5Network REQUIRED)
    endif( Qt5Core_DIR )
endif( BUILD_WITH_QT5 )
if( NOT Qt5Core_DIR )
    find_package(Qt4 4.7.0 REQUIRED QtCore QtGui)
endif( NOT Qt5Core_DIR )

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

if( Qt5Core_DIR )
    include_directories(${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Sql_INCLUDE_DIRS}
                    ${Qt5Network_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR})

    qt5_add_resources(plv_RC_SRCS ${plv_RCS} )
    qt5_wrap_ui(plv_UI_HEADERS ${plv_UI})
    qt5_add_translation(plv_I18N ${TRANSLATIONS})
endif( Qt5Core_DIR )
if( NOT Qt5Core_DIR )
    include_directories(${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR})

    qt4_add_resources(plv_RC_SRCS ${plv_RCS} )
    qt4_wrap_ui(plv_UI_HEADERS ${plv_UI})
    qt4_add_translation(plv_I18N ${TRANSLATIONS})
endif( NOT Qt5Core_DIR )

add_executable(plv main.cpp ${plv_SRCS} ${plv_RC_SRCS} ${plv_UI_HEADERS} ${plv_I18N})

if( Qt5Core_DIR )
target_link_libraries(plv
                      ${LINK_LIBRARIES}
                      ${Qt5Core_LIBRARIES}
                      ${Qt5Widgets_LIBRARIES}
                      ${Qt5Sql_LIBRARIES}
                      ${Qt5Network_LIBRARIES})
endif( Qt5Core_DIR )
if( NOT Qt5Core_DIR )
target_link_libraries(plv
                      ${LINK_LIBRARIES}
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTSQL_LIBRARY})
endif( NOT Qt5Core_DIR )


install(TARGETS plv RUNTIME DESTINATION bin)

if( BUILD_WITH_QT5 )
install(FILES ${plv_I18N} 
        DESTINATION share/qt5/translations/)
else( BUILD_WITH_QT5 )
install(FILES ${plv_I18N} 
        DESTINATION share/qt/translations/)
endif( BUILD_WITH_QT5 )

install(FILES ${CMAKE_SOURCE_DIR}/pacmanlogviewer.desktop 
        DESTINATION share/applications )
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
