#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T12:32:22
#
#-------------------------------------------------

QT       += core sql gui

#QT       -= gui

TARGET = COMPI
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += ../../src/compiler/main.cpp \
    ../../src/compiler/nativecompiler.cpp \
    ../../src/compiler/datacompiler.cpp \
    ../../src/compiler/compiler.cpp \
    ../../src/compiler/compi.cpp \
    ../../src/common/DAO/variabledao.cpp \
    ../../src/common/DAO/predicatedao.cpp \
    ../../src/common/DAO/datatypedao.cpp \
    ../../src/common/DAO/basedao.cpp \
    ../../src/common/DAO/actordao.cpp \
    ../../src/common/VO/variable.cpp \
    ../../src/common/VO/predicate.cpp \
    ../../src/common/VO/graph.cpp \
    ../../src/common/VO/datatype.cpp \
    ../../src/common/VO/basemodule.cpp \
    ../../src/common/VO/actor.cpp \
    ../../src/common/qgraphsettings.cpp \
    ../../src/common/md5.cpp \
    ../../src/common/logger.cpp \
    ../../src/common/databasemanager.cpp \
    ../../src/common/commonutils.cpp

HEADERS += \
    ../../src/compiler/nativecompiler.h \
    ../../src/compiler/datacompiler.h \
    ../../src/compiler/compiler.h \
    ../../src/compiler/compi.h \
    ../../src/common/DAO/variabledao.h \
    ../../src/common/DAO/predicatedao.h \
    ../../src/common/DAO/datatypedao.h \
    ../../src/common/DAO/basedao.h \
    ../../src/common/DAO/actordao.h \
    ../../src/common/VO/variable.h \
    ../../src/common/VO/predicate.h \
    ../../src/common/VO/graph.h \
    ../../src/common/VO/datatype.h \
    ../../src/common/VO/basemodule.h \
    ../../src/common/VO/actor.h \
    ../../src/common/qgraphsettings.h \
    ../../src/common/md5.h \
    ../../src/common/logger.h \
    ../../src/common/globalvariables.h \
    ../../src/common/errors.h \
    ../../src/common/databasemanager.h \
    ../../src/common/commonutils.h
