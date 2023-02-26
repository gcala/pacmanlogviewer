#-------------------------------------------------
#
# Project created by QtCreator 2012-04-07T19:22:11
#
#-------------------------------------------------

QT       += core gui sql widgets

TARGET = plv
TEMPLATE = app


SOURCES += main.cpp\
    dialog.cpp \
    filterswidget.cpp \
    datecolumndelegate.cpp \
    actioncolumndelegate.cpp \
    pkgnamecompleter.cpp \
    pkgnameslineedit.cpp \
    aboutdialog.cpp

HEADERS  += dialog.h \
    connection.h \
    filterswidget.h \
    datecolumndelegate.h \
    actioncolumndelegate.h \
    pkgnamecompleter.h \
    pkgnameslineedit.h \
    aboutdialog.h

FORMS    += dialog.ui \
    filterswidget.ui \
    aboutdialog.ui

TRANSLATIONS += lang/pacmanlogviewer_en.ts \
                lang/pacmanlogviewer_it.ts

RESOURCES += \
    pacmanlogviewer.qrc

isEmpty(PREFIX) {
    PREFIX =   /usr
}

target.path =  $$PREFIX/bin

desktop.files = pacmanlogviewer.desktop
desktop.path =  $$PREFIX/share/applications

icon_16.files = images/16x16/plv.png
icon_24.files = images/24x24/plv.png
icon_32.files = images/32x32/plv.png
icon_48.files = images/48x48/plv.png
icon_64.files = images/64x64/plv.png
icon_128.files = images/128x128/plv.png
icon_16.path =  $$PREFIX/share/icons/hicolor/16x16/apps
icon_24.path =  $$PREFIX/share/icons/hicolor/24x24/apps
icon_32.path =  $$PREFIX/share/icons/hicolor/32x32/apps
icon_48.path =  $$PREFIX/share/icons/hicolor/48x48/apps
icon_64.path =  $$PREFIX/share/icons/hicolor/64x64/apps
icon_128.path =  $$PREFIX/share/icons/hicolor/128x128/apps

translations.files = lang/*.qm
translations.path =  $$PREFIX/share/qt/translations/
translations.CONFIG += no_check_exist

INSTALLS += target desktop icon_16 icon_24 icon_32 icon_48 icon_64 icon_128 translations
