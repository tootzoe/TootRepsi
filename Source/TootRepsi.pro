




#TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#
# below one line project name need to be lowercaes
PRJNAMETOOT = TootRepsi
DEFINES += "TOOTREPSI_API="
DEFINES += "TOOTREPSI_API(...)="
#
DEFINES += "UCLASS()=TOOTREPSI_API"
DEFINES += "UCLASS(...)=TOOTREPSI_API"
#
# this is true during development with unreal-editor...

DEFINES += "WITH_EDITORONLY_DATA=1"

## this project only

##


INCLUDEPATH += ../Intermediate/Build/Win64/UnrealEditor/Inc/$$PRJNAMETOOT/UHT
INCLUDEPATH += $$PRJNAMETOOT $$PRJNAMETOOT/Public $$PRJNAMETOOT/Private
#INCLUDEPATH += ../Plugins/NNEPostProcessing/Source/NNEPostProcessing/Public
# we should follow UE project struct to include files, start from prj.Build.cs folder
#
#  The Thirdparty libs
#
#
#
include(defs.pri)
include(inc.pri)
#
## this project only
# INCLUDEPATH += $$UESRCROOT/Runtime/Renderer/Private
##
#
#

DISTFILES += \
    TootRepsi.Target.cs \
    TootRepsi/TootRepsi.Build.cs \
    TootRepsiEditor.Target.cs

HEADERS += \
    TootRepsi/PrjLog.h \
    TootRepsi/Public/MapRoom.h \
    TootRepsi/Public/TootMainGM.h \
    TootRepsi/Public/TootRepsiCharacter.h \
    TootRepsi/Public/TootRepsiPlayerCtrlor.h \
    TootRepsi/TootRepsi.h

SOURCES += \
    TootRepsi/Private/MapRoom.cpp \
    TootRepsi/Private/TootMainGM.cpp \
    TootRepsi/Private/TootRepsiCharacter.cpp \
    TootRepsi/Private/TootRepsiPlayerCtrlor.cpp \
    TootRepsi/PrjLog.cpp \
    TootRepsi/TootRepsi.cpp
