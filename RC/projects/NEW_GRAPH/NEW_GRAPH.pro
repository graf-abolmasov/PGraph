# -------------------------------------------------
# Project created by QtCreator 2009-11-15T04:10:59
# -------------------------------------------------
QT += sql core
TARGET = NEW_GRAPH
TEMPLATE = app
SOURCES += ../../src/editor/main.cpp \
    ../../src/editor/qmywindow.cpp \
    ../../src/editor/qdrawwindow.cpp \
    ../../src/editor/qtop.cpp \
    ../../src/editor/qdiagramscene.cpp \
    ../../src/editor/qarc.cpp \
    ../../src/editor/qcomment.cpp \
    ../../src/editor/qsyncarc.cpp \
    ../../src/editor/qarcline.cpp \
    ../../src/editor/qserialarctop.cpp \
    ../../src/common/databasemanager.cpp \
    ../../src/editor/qparallelarctop.cpp \
    ../../src/editor/qterminatearctop.cpp \
    ../../src/editor/qmultiproctop.cpp \
    ../../src/common/commonutils.cpp \
    ../../src/common/md5.cpp \
    ../../src/editor/qnormaltop.cpp \
    ../../src/common/logger.cpp \
    ../../src/editor/undocommand.cpp \
    ../../src/compiler/graphdebugger.cpp \
    ../../src/editor/qcodeeditor.cpp \
    ../../src/common/qgraphsettings.cpp \
    ../../src/compiler/nativecompiler.cpp \
    ../../src/common/DAO/datatypedao.cpp \
    ../../src/common/DAO/basedao.cpp \
    ../../src/common/DAO/variabledao.cpp \
    ../../src/common/DAO/actordao.cpp \
    ../../src/common/DAO/predicatedao.cpp \
    ../../src/common/VO/variable.cpp \
    ../../src/common/VO/predicate.cpp \
    ../../src/common/VO/datatype.cpp \
    ../../src/common/VO/basemodule.cpp \
    ../../src/common/VO/actor.cpp \
    ../../src/editor/dialogs/toppropertydialog.cpp \
    ../../src/editor/dialogs/qvariableeditor.cpp \
    ../../src/editor/dialogs/qvariabledialog.cpp \
    ../../src/editor/dialogs/qsavegraphdialog.cpp \
    ../../src/editor/dialogs/qpredicateeditor.cpp \
    ../../src/editor/dialogs/qopengraphdialog.cpp \
    ../../src/editor/dialogs/qobjecteditor.cpp \
    ../../src/editor/dialogs/qmoduleregister.cpp \
    ../../src/editor/dialogs/qdatatypeeditor.cpp \
    ../../src/editor/dialogs/qdatatypedialog.cpp \
    ../../src/editor/dialogs/qactoreditor.cpp \
    ../../src/editor/dialogs/projectdialog.cpp \
    ../../src/editor/dialogs/multiproctoppropertydialog.cpp \
    ../../src/editor/dialogs/arcpropertydialog.cpp \
    ../../src/editor/dialogs/otherfilesdialog.cpp \
    ../../src/common/DAO/graphdao.cpp \
    ../../src/common/VO/graphvo.cpp \
    ../../src/compiler/graphcompiler.cpp \
    ../../src/compiler/abstractcompiler.cpp \
    ../../src/compiler/compiler.cpp \
    ../../src/compiler/sourcecompiler.cpp \
    ../../src/compiler/astylewrapper.cpp
HEADERS += ../../src/editor/qmywindow.h \
    ../../src/editor/qdrawwindow.h \
    ../../src/editor/qtop.h \
    ../../src/editor/qdiagramscene.h \
    ../../src/editor/qarc.h \
    ../../src/editor/qcomment.h \
    ../../src/editor/qsyncarc.h \
    ../../src/editor/qarcline.h \
    ../../src/editor/qserialarctop.h \
    ../../src/editor/qcodeeditor.h \
    ../../src/editor/qparallelarctop.h \
    ../../src/editor/qterminatearctop.h \
    ../../src/editor/qmultiproctop.h \
    ../../src/editor/qnormaltop.h \
    ../../src/editor/undocommand.h \
    ../../src/common/databasemanager.h \
    ../../src/common/commonutils.h \
    ../../src/common/md5.h \
    ../../src/common/logger.h \
    ../../src/common/globalvariables.h \
    ../../src/common/errors.h \
    ../../src/common/qgraphsettings.h \
    ../../src/compiler/nativecompiler.h \
    ../../src/compiler/graphdebugger.h \
    ../../src/common/DAO/datatypedao.h \
    ../../src/common/DAO/basedao.h \
    ../../src/common/DAO/variabledao.h \
    ../../src/common/DAO/actordao.h \
    ../../src/common/DAO/predicatedao.h \
    ../../src/common/VO/variable.h \
    ../../src/common/VO/predicate.h \
    ../../src/common/VO/datatype.h \
    ../../src/common/VO/basemodule.h \
    ../../src/common/VO/actor.h \
    ../../src/editor/dialogs/toppropertydialog.h \
    ../../src/editor/dialogs/qvariableeditor.h \
    ../../src/editor/dialogs/qvariabledialog.h \
    ../../src/editor/dialogs/qsavegraphdialog.h \
    ../../src/editor/dialogs/qpredicateeditor.h \
    ../../src/editor/dialogs/qopengraphdialog.h \
    ../../src/editor/dialogs/qobjecteditor.h \
    ../../src/editor/dialogs/qmoduleregister.h \
    ../../src/editor/dialogs/qdatatypeeditor.h \
    ../../src/editor/dialogs/qdatatypedialog.h \
    ../../src/editor/dialogs/qactoreditor.h \
    ../../src/editor/dialogs/projectdialog.h \
    ../../src/editor/dialogs/multiproctoppropertydialog.h \
    ../../src/editor/dialogs/arcpropertydialog.h \
    ../../src/editor/dialogs/otherfilesdialog.h \
    ../../src/common/DAO/graphdao.h \
    ../../src/common/VO/graphvo.h \
    ../../src/compiler/graphcompiler.h \
    ../../src/compiler/abstractcompiler.h \
    ../../src/compiler/compiler.h \
    ../../src/compiler/sourcecompiler.h \
    ../../src/compiler/astylewrapper.h
RESOURCES += ../../src/resources.qrc
FORMS += ../../src/editor/dialogs/toppropertydialog.ui \
    ../../src/editor/dialogs/qvariableeditor.ui \
    ../../src/editor/dialogs/qvariabledialog.ui \
    ../../src/editor/dialogs/qsavegraphdialog.ui \
    ../../src/editor/dialogs/qpredicateeditor.ui \
    ../../src/editor/dialogs/qopengraphdialog.ui \
    ../../src/editor/dialogs/qobjecteditor.ui \
    ../../src/editor/dialogs/qmoduleregister.ui \
    ../../src/editor/dialogs/qdatatypeeditor.ui \
    ../../src/editor/dialogs/qdatatypedialog.ui \
    ../../src/editor/dialogs/qactoreditor.ui \
    ../../src/editor/dialogs/projectdialog.ui \
    ../../src/editor/dialogs/multiproctoppropertydialog.ui \
    ../../src/editor/dialogs/arcpropertydialog.ui \
    ../../src/editor/dialogs/otherfilesdialog.ui
RC_FILE = ../../images/resources.rc



unix:!macx:!symbian|win32: LIBS += -L$$PWD/../../lib/ -lastyle

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

win32: PRE_TARGETDEPS += $$PWD/../../lib/astyle.lib
else:unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../lib/libastyle.a
