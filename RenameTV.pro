#-------------------------------------------------
#
# Project created by QtCreator 2011-02-16T20:45:41
#
#-------------------------------------------------

QT       += network

TARGET = RenameTV
TEMPLATE = app

win32 {
    RC_FILE += package\RenameTV.rc
}

SOURCES += main.cpp\
        mainwindow.cpp \
    showholder.cpp \
    searchresults.cpp \
    episodeselector.cpp

HEADERS  += mainwindow.hpp \
    showholder.hpp \
    searchresults.hpp \
    episodeselector.hpp

FORMS    += mainwindow.ui \
    searchresults.ui \
    episodeselector.ui
