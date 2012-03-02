# -------------------------------------------------
# Project created by QtCreator 2009-11-15T04:10:59
# -------------------------------------------------
QT += sql
TARGET = NEW_GRAPH
TEMPLATE = app
SOURCES += main.cpp \
    qmywindow.cpp \
    qdrawwindow.cpp \
    qtop.cpp \
    qdiagramscene.cpp \
    qarc.cpp \
    qcomment.cpp \
    toppropertydialog.cpp \
    qsyncarc.cpp \
    qarcline.cpp \
    qserialarctop.cpp \
    qobjecteditor.cpp \
    qdatatypeeditor.cpp \
    qdatatypedialog.cpp \
    qvariableeditor.cpp \
    qvariabledialog.cpp \
    databasemanager.cpp \
    qactoreditor.cpp \
    qmoduleregister.cpp \
    qpredicateeditor.cpp \
    arcpropertydialog.cpp \
    qparallelarctop.cpp \
    qterminatearctop.cpp \
    qmultiproctop.cpp \
    commonutils.cpp \
    qsavegraphdialog.cpp \
    qopengraphdialog.cpp \
    md5.cpp \
    qnormaltop.cpp \
    multiproctoppropertydialog.cpp \
    logger.cpp \
    undocommand.cpp \
    compi.cpp \
    datacompiler.cpp \
    qcodeeditor.cpp \
    compiler.cpp \
    qgraphsettings.cpp \
    projectdialog.cpp \
    nativecompiler.cpp \
    DAO/datatypedao.cpp \
    DAO/basedao.cpp \
    DAO/variabledao.cpp \
    DAO/actordao.cpp \
    DAO/predicatedao.cpp \
    VO/variable.cpp \
    VO/predicate.cpp \
    VO/graph.cpp \
    VO/datatype.cpp \
    VO/basemodule.cpp \
    VO/actor.cpp
HEADERS += qmywindow.h \
    qdrawwindow.h \
    qtop.h \
    qdiagramscene.h \
    qarc.h \
    qcomment.h \
    toppropertydialog.h \
    qsyncarc.h \
    qarcline.h \
    qserialarctop.h \
    qobjecteditor.h \
    qdatatypeeditor.h \
    qdatatypedialog.h \
    qvariableeditor.h \
    qvariabledialog.h \
    databasemanager.h \
    qactoreditor.h \
    qmoduleregister.h \
    qpredicateeditor.h \
    arcpropertydialog.h \
    qparallelarctop.h \
    qterminatearctop.h \
    qmultiproctop.h \
    commonutils.h \
    qsavegraphdialog.h \
    qopengraphdialog.h \
    md5.h \
    qnormaltop.h \
    multiproctoppropertydialog.h \
    logger.h \
    globalvariables.h \
    undocommand.h \
    compi.h \
    compiler.h \
    datacompiler.h \
    qcodeeditor.h \
    errors.h \
    qgraphsettings.h \
    projectdialog.h \
    nativecompiler.h \
    DAO/datatypedao.h \
    DAO/basedao.h \
    DAO/variabledao.h \
    DAO/actordao.h \
    DAO/predicatedao.h \
    VO/variable.h \
    VO/predicate.h \
    VO/graph.h \
    VO/datatype.h \
    VO/basemodule.h \
    VO/actor.h
RESOURCES += resources.qrc
FORMS += toppropertydialog.ui \
    qobjecteditor.ui \
    qdatatypeeditor.ui \
    qdatatypedialog.ui \
    qvariableeditor.ui \
    qvariabledialog.ui \
    qactoreditor.ui \
    qmoduleregister.ui \
    qpredicateeditor.ui \
    arcpropertydialog.ui \
    qsavegraphdialog.ui \
    qopengraphdialog.ui \
    multiproctoppropertydialog.ui \
    projectdialog.ui
RC_FILE = images/resources.rc
