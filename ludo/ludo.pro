#-------------------------------------------------
#
# Project created by QtCreator 
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ludo
TEMPLATE = app
unix{
LIBS += -L/usr/lib64
LIBS += -L/usr/local
LIBS += -L/usr
LIBS += -lboost_thread
LIBS += -lboost_system
LIBS += -lboost_iostreams
LIBS += -lboost_serialization
}

SOURCES += main.cpp\
        dialog.cpp \
    game.cpp \
    ludo_player.cpp \
    ludo_player_random.cpp \
    main_class.cpp \
    ludo_player_a.cpp \
    q_player.cpp \

HEADERS  += dialog.h \
    game.h \
    ludo_player.h \
    positions_and_dice.h \
    ludo_player_random.h \
    main_class.h \
    ludo_player_a.h \
    q_player.hpp \
    
FORMS    += dialog.ui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wunused
