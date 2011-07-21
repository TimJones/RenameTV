#-------------------------------------------------
#
# Project created by QtCreator 2011-02-16T20:45:41
#
#-------------------------------------------------

QT       += network

TARGET = renametv
TEMPLATE = app

unix {
    target.path =/usr/bin
    INSTALLS += target
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
