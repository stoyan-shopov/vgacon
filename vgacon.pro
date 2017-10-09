#-------------------------------------------------
#
# Project created by QtCreator 2017-10-04T14:28:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vgacon
TEMPLATE = app
DEFINES += CORE_CELLS_COUNT=128*1024 STACK_DEPTH=32
INCLUDEPATH += sforth/

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cxx \
        mainwindow.cxx \
    touchkeyboard.cxx \
    sforth/dictionary.c \
    sforth/engine.c \
    sforth/sf-opt-file.c \
    sforth/sf-opt-prog-tools.c \
    sforth/sf-opt-sample.c \
    sforth/sf-opt-string.c \
    sforth/sf-arch-default.c \
    sforth.cxx \
    vgawidget.cxx \
    vgafont.cxx

HEADERS += \
        mainwindow.hxx \
    vgawidget.hxx \
    touchkeyboard.hxx \
    sforth/dictionary.h \
    sforth/enabled-words.h \
    sforth/engine.h \
    sforth/sf-arch.h \
    sforth/sf-cfg.h \
    sforth/sf-generate-dictionary-entries.h \
    sforth/sf-generate-prototypes.h \
    sforth/sf-generate-word-bodies.h \
    sforth/sf-word-wizard.h \
    sforth.hxx \
    vgafont.hxx

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resources.qrc

