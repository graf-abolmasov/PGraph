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
    qsyncarc.cpp \
    qarcline.cpp \
    qserialarctop.cpp \
    databasemanager.cpp \
    qparallelarctop.cpp \
    qterminatearctop.cpp \
    qmultiproctop.cpp \
    commonutils.cpp \
    md5.cpp \
    qnormaltop.cpp \
    logger.cpp \
    undocommand.cpp \
    compi.cpp \
    datacompiler.cpp \
    qcodeeditor.cpp \
    compiler.cpp \
    qgraphsettings.cpp \
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
    VO/actor.cpp \
    dialogs/toppropertydialog.cpp \
    dialogs/qvariableeditor.cpp \
    dialogs/qvariabledialog.cpp \
    dialogs/qsavegraphdialog.cpp \
    dialogs/qpredicateeditor.cpp \
    dialogs/qopengraphdialog.cpp \
    dialogs/qobjecteditor.cpp \
    dialogs/qmoduleregister.cpp \
    dialogs/qdatatypeeditor.cpp \
    dialogs/qdatatypedialog.cpp \
    dialogs/qactoreditor.cpp \
    dialogs/projectdialog.cpp \
    dialogs/multiproctoppropertydialog.cpp \
    dialogs/arcpropertydialog.cpp
HEADERS += qmywindow.h \
    qdrawwindow.h \
    qtop.h \
    qdiagramscene.h \
    qarc.h \
    qcomment.h \
    qsyncarc.h \
    qarcline.h \
    qserialarctop.h \
    databasemanager.h \
    qparallelarctop.h \
    qterminatearctop.h \
    qmultiproctop.h \
    commonutils.h \
    md5.h \
    qnormaltop.h \
    logger.h \
    globalvariables.h \
    undocommand.h \
    compi.h \
    compiler.h \
    datacompiler.h \
    qcodeeditor.h \
    errors.h \
    qgraphsettings.h \
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
    VO/actor.h \
    dialogs/toppropertydialog.h \
    dialogs/qvariableeditor.h \
    dialogs/qvariabledialog.h \
    dialogs/qsavegraphdialog.h \
    dialogs/qpredicateeditor.h \
    dialogs/qopengraphdialog.h \
    dialogs/qobjecteditor.h \
    dialogs/qmoduleregister.h \
    dialogs/qdatatypeeditor.h \
    dialogs/qdatatypedialog.h \
    dialogs/qactoreditor.h \
    dialogs/projectdialog.h \
    dialogs/multiproctoppropertydialog.h \
    dialogs/arcpropertydialog.h
RESOURCES += resources.qrc
FORMS += dialogs/toppropertydialog.ui \
    dialogs/qvariableeditor.ui \
    dialogs/qvariabledialog.ui \
    dialogs/qsavegraphdialog.ui \
    dialogs/qpredicateeditor.ui \
    dialogs/qopengraphdialog.ui \
    dialogs/qobjecteditor.ui \
    dialogs/qmoduleregister.ui \
    dialogs/qdatatypeeditor.ui \
    dialogs/qdatatypedialog.ui \
    dialogs/qactoreditor.ui \
    dialogs/projectdialog.ui \
    dialogs/multiproctoppropertydialog.ui \
    dialogs/arcpropertydialog.ui
RC_FILE = images/resources.rc
