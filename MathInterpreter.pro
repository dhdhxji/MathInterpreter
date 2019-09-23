TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        mathinterpreter.cpp \
        term/number.cpp \
        term/operation.cpp \
        term/specialterm.cpp \
        term/term.cpp

HEADERS += \
    mathinterpreter.h \
    term/number.h \
    term/operation.h \
    term/specialterm.h \
    term/term.h
