######################################################################
# Automatically generated by qmake (2.01a) gio 8. mag 01:13:34 2008
######################################################################

TEMPLATE = lib
TARGET = qnet
DEPENDPATH += . ../qtzip
INCLUDEPATH += . ../qtzip


CONFIG   += qt warn_off release staticlib
LANGUAGE	= C++

MOC_DIR	= build/.moc
RCC_DIR	= build/.rcc
OBJECTS_DIR = build/.obj

CONFIG += create_prl 

DESTDIR	= ../lib/

QT += xml core gui sql network
include( ../mac_univ.pri )


# Input
HEADERS += network_get.h
SOURCES += network_get.cpp



LIBS   += -L../lib/ -lqzip
