# -------------------------------------------------
# Project created by QtCreator 2013-01-27T14:50:58
# -------------------------------------------------
QT += network \
    xml
QT -= gui
TARGET = AdmissionSurveyServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += Main.cpp \
    AdmissionSurveyServer.cpp \
    ClientSocket.cpp \
    ResponsesSet.cpp \
    DBManager.cpp \
    IllegalArgumentException.cpp \
    ConfigurationXML.cpp \
    RessourceParsingException.cpp \
    RessourceOpenException.cpp \
    RessourceFormatException.cpp \
    RessourceException.cpp \
    RessourceErrorException.cpp \
    Configuration.cpp \
    OutputItem.cpp
HEADERS += AdmissionSurveyServer.h \
    ClientSocket.h \
    QuestionResponse.h \
    ResponsesSet.h \
    DBManager.h \
    IllegalArgumentException.h \
    ConfigurationXML.h \
    RessourceParsingException.h \
    RessourceOpenException.h \
    RessourceFormatException.h \
    RessourceException.h \
    RessourceErrorException.h \
    Configuration.h \
    OutputItem.h
