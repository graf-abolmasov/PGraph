#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T12:32:22
#
#-------------------------------------------------

QT       += core sql gui

TARGET = COMPI
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -I"."

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
    ../../src/common/VO/graphvo.cpp \
    ../../src/common/VO/datatype.cpp \
    ../../src/common/VO/basemodule.cpp \
    ../../src/common/VO/actor.cpp \
    ../../src/common/qgraphsettings.cpp \
    ../../src/common/md5.cpp \
    ../../src/common/logger.cpp \
    ../../src/common/databasemanager.cpp \
    ../../src/common/commonutils.cpp \
    ../../src/compiler/vcompi/vcompiwrapper.cpp \
    ../../src/compiler/vcompi/tpodata.cpp \
    ../../src/compiler/vcompi/TIPGH.cpp \
    ../../src/compiler/vcompi/SwFaza.cpp \
    ../../src/compiler/vcompi/SF14128A2.cpp \
    ../../src/compiler/vcompi/SF8D40B66.cpp \
    ../../src/compiler/vcompi/SEF5AFCAF.cpp \
    ../../src/compiler/vcompi/SEAFC33C7.cpp \
    ../../src/compiler/vcompi/S89829134.cpp \
    ../../src/compiler/vcompi/S19086399.cpp \
    ../../src/compiler/vcompi/S7941FB6A.cpp \
    ../../src/compiler/vcompi/S7730E5ED.cpp \
    ../../src/compiler/vcompi/S1313BA02.cpp \
    ../../src/compiler/vcompi/S371DEA77.cpp \
    ../../src/compiler/vcompi/S19E6D097.cpp \
    ../../src/compiler/vcompi/S7AE89170.cpp \
    ../../src/compiler/vcompi/S6CCCAD75.cpp \
    ../../src/compiler/vcompi/S6CC6E5ED.cpp \
    ../../src/compiler/vcompi/ReadTr.cpp \
    ../../src/compiler/vcompi/print_struct.cpp \
    ../../src/compiler/vcompi/PFF71235F.cpp \
    ../../src/compiler/vcompi/PEA3294B1.cpp \
    ../../src/compiler/vcompi/PDC8C0A19.cpp \
    ../../src/compiler/vcompi/PDC5EFC2C.cpp \
    ../../src/compiler/vcompi/PCD7838ED.cpp \
    ../../src/compiler/vcompi/PB4337ADE.cpp \
    ../../src/compiler/vcompi/P62843319.cpp \
    ../../src/compiler/vcompi/P8095298.cpp \
    ../../src/compiler/vcompi/P92415BF3.cpp \
    ../../src/compiler/vcompi/P4168D9.cpp \
    ../../src/compiler/vcompi/P3869C48C.cpp \
    ../../src/compiler/vcompi/P3708EB64.cpp \
    ../../src/compiler/vcompi/P3307FCC1.cpp \
    ../../src/compiler/vcompi/P3201D7B0.cpp \
    ../../src/compiler/vcompi/P538C8D9F.cpp \
    ../../src/compiler/vcompi/P282E6F54.cpp \
    ../../src/compiler/vcompi/P62B749BA.cpp \
    ../../src/compiler/vcompi/P43C2377C.cpp \
    ../../src/compiler/vcompi/P36DF27A0.cpp \
    ../../src/compiler/vcompi/P11D5122C.cpp \
    ../../src/compiler/vcompi/P6CD51F69.cpp \
    ../../src/compiler/vcompi/P3A844A0E.cpp \
    ../../src/compiler/vcompi/P2EDD443B.cpp \
    ../../src/compiler/vcompi/P1DFF9165.cpp \
    ../../src/compiler/vcompi/P1C72B9D5.cpp \
    ../../src/compiler/vcompi/NumFaz.cpp \
    ../../src/compiler/vcompi/memman.cpp \
    ../../src/compiler/vcompi/graphmv.cpp \
    ../../src/compiler/vcompi/graph.cpp \
    ../../src/compiler/vcompi/Girland.cpp \
    ../../src/compiler/vcompi/GF0BF02CB.cpp \
    ../../src/compiler/vcompi/GE942759B.cpp \
    ../../src/compiler/vcompi/GB4995CC8.cpp \
    ../../src/compiler/vcompi/G9374E37D.cpp \
    ../../src/compiler/vcompi/G8231D095.cpp \
    ../../src/compiler/vcompi/G848B1898.cpp \
    ../../src/compiler/vcompi/G57D6B83A.cpp \
    ../../src/compiler/vcompi/G8EC49CC4.cpp \
    ../../src/compiler/vcompi/G6C578BEC.cpp \
    ../../src/compiler/vcompi/G3CDB9BDD.cpp \
    ../../src/compiler/vcompi/FullTr.cpp \
    ../../src/compiler/vcompi/deftest.cpp \
    ../../src/compiler/vcompi/BPAGR.cpp \
    ../../src/compiler/vcompi/BnameG.cpp \
    ../../src/compiler/vcompi/BFLH.cpp \
    ../../src/compiler/vcompi/BfindE.cpp \
    ../../src/compiler/vcompi/BcodeTr.cpp \
    ../../src/compiler/vcompi/AFDF9CBC5.cpp \
    ../../src/compiler/vcompi/AFCD704BA.cpp \
    ../../src/compiler/vcompi/AFC8A1A59.cpp \
    ../../src/compiler/vcompi/AFA2E8F65.cpp \
    ../../src/compiler/vcompi/AEF99357A.cpp \
    ../../src/compiler/vcompi/AEF49659B.cpp \
    ../../src/compiler/vcompi/AEEDFC3CC.cpp \
    ../../src/compiler/vcompi/AEA8362EB.cpp \
    ../../src/compiler/vcompi/AE05016B2.cpp \
    ../../src/compiler/vcompi/ADF8D7A12.cpp \
    ../../src/compiler/vcompi/AddTop.cpp \
    ../../src/compiler/vcompi/ADCBC8AD7.cpp \
    ../../src/compiler/vcompi/AC544C50.cpp \
    ../../src/compiler/vcompi/AC72B7C3D.cpp \
    ../../src/compiler/vcompi/AC20E2D5E.cpp \
    ../../src/compiler/vcompi/ABFAF5078.cpp \
    ../../src/compiler/vcompi/ABF380A4A.cpp \
    ../../src/compiler/vcompi/ABE7A0F38.cpp \
    ../../src/compiler/vcompi/AB518086E.cpp \
    ../../src/compiler/vcompi/AB5D3E698.cpp \
    ../../src/compiler/vcompi/AAD9D3184.cpp \
    ../../src/compiler/vcompi/A951652FD.cpp \
    ../../src/compiler/vcompi/A289590B9.cpp \
    ../../src/compiler/vcompi/A52792D93.cpp \
    ../../src/compiler/vcompi/A9879A1A2.cpp \
    ../../src/compiler/vcompi/A8418E7E1.cpp \
    ../../src/compiler/vcompi/A997A5B31.cpp \
    ../../src/compiler/vcompi/A781AE049.cpp \
    ../../src/compiler/vcompi/A684C9F79.cpp \
    ../../src/compiler/vcompi/A674A2D96.cpp \
    ../../src/compiler/vcompi/A92AC8659.cpp \
    ../../src/compiler/vcompi/A87DDA0B5.cpp \
    ../../src/compiler/vcompi/A81D79BB8.cpp \
    ../../src/compiler/vcompi/A71A7531D.cpp \
    ../../src/compiler/vcompi/A39A80DFE.cpp \
    ../../src/compiler/vcompi/A25C2C0C3.cpp \
    ../../src/compiler/vcompi/A21A2E4BC.cpp \
    ../../src/compiler/vcompi/A9E722978.cpp \
    ../../src/compiler/vcompi/A9D803DBF.cpp \
    ../../src/compiler/vcompi/A8C95E239.cpp \
    ../../src/compiler/vcompi/A8C6F61DD.cpp \
    ../../src/compiler/vcompi/A8C4D87AE.cpp \
    ../../src/compiler/vcompi/A6DFD2020.cpp \
    ../../src/compiler/vcompi/A5DADAA5E.cpp \
    ../../src/compiler/vcompi/A5B1E6B76.cpp \
    ../../src/compiler/vcompi/A2D21EB55.cpp \
    ../../src/compiler/vcompi/A1B2378BE.cpp \
    ../../src/common/DAO/graphdao.cpp

HEADERS += ../../src/compiler/nativecompiler.h \
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
    ../../src/common/VO/datatype.h \
    ../../src/common/VO/basemodule.h \
    ../../src/common/VO/actor.h \
    ../../src/common/qgraphsettings.h \
    ../../src/common/md5.h \
    ../../src/common/logger.h \
    ../../src/common/globalvariables.h \
    ../../src/common/errors.h \
    ../../src/common/databasemanager.h \
    ../../src/common/commonutils.h \
    ../../src/compiler/vcompi/vcompiwrapper.h \
    ../../src/compiler/vcompi/utype.h \
    ../../src/compiler/vcompi/tpodata.h \
    ../../src/compiler/vcompi/stype.h \
    ../../src/compiler/vcompi/property.h \
    ../../src/compiler/vcompi/memman.h \
    ../../src/compiler/vcompi/graph.h \
    ../../src/compiler/vcompi/defines.h \
    ../../src/common/DAO/graphdao.h \
    ../../src/common/VO/graphvo.h
