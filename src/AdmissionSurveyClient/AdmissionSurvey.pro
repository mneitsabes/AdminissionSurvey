QT += xml \
    webkit \
    network
SOURCES += Main.cpp \
    GlobalView.cpp \
    Engine.cpp \
    Question.cpp \
    EngineXML.cpp \
    RessourceParsingException.cpp \
    RessourceOpenException.cpp \
    RessourceFormatException.cpp \
    RessourceException.cpp \
    RessourceErrorException.cpp \
    QuestionData.cpp \
    Response.cpp \
    ResponseItem.cpp \
    ResponseYesNoUnknown.cpp \
    ResponseYesNo.cpp \
    Tools.cpp \
    ResponseDeclared.cpp \
    RecorderSession.cpp \
    Record.cpp \
    IllegalArgumentException.cpp \
    NetworkTransfert.cpp \
    DBManager.cpp \
    QWebViewSelectionSuppressor.cpp \
    ResponseEmbedded.cpp \
    Configuration.cpp
HEADERS += GlobalView.h \
    Engine.h \
    Language.h \
    Response.h \
    Question.h \
    EngineXML.h \
    RessourceParsingException.h \
    RessourceOpenException.h \
    RessourceFormatException.h \
    RessourceException.h \
    RessourceErrorException.h \
    EngineListener.h \
    QuestionData.h \
    ResponseItem.h \
    ResponseYesNoUnknown.h \
    ResponseYesNo.h \
    Tools.h \
    ResponseDeclared.h \
    RecorderSession.h \
    Record.h \
    IllegalArgumentException.h \
    NetworkTransfert.h \
    NetworkTransfertListener.h \
    DBManager.h \
    QWebViewSelectionSuppressor.h \
    ResponseEmbedded.h \
    Configuration.h
RESOURCES += Ressources.qrc
CONFIG += console
