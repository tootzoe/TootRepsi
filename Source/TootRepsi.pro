




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
DEFINES += PLATFORM_ANDROID
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
    TootRepsiClient.Target.cs \
    TootRepsiEditor.Target.cs \
    TootRepsiServer.Target.cs

HEADERS += \
    TootRepsi/FlybotBasicShot.h \
    TootRepsi/Private/GameDspHUD.h \
    TootRepsi/PrjLog.h \
    TootRepsi/Public/MainPlayerHUD.h \
    TootRepsi/Public/MapRoom.h \
    TootRepsi/Public/PlayerBotController.h \
    TootRepsi/Public/PlayerBotPawn.h \
    TootRepsi/Public/TootFlyBotGM.h \
    TootRepsi/Public/TootMainGM.h \
    TootRepsi/Public/TootRepsiCharacter.h \
    TootRepsi/Public/TootRepsiPlayerCtrlor.h \
    TootRepsi/TootDefs.h \
    TootRepsi/TootRepsi.h

SOURCES += \
    TootRepsi/FlybotBasicShot.cpp \
    TootRepsi/Private/GameDspHUD.cpp \
    TootRepsi/Private/MainPlayerHUD.cpp \
    TootRepsi/Private/MapRoom.cpp \
    TootRepsi/Private/PlayerBotController.cpp \
    TootRepsi/Private/PlayerBotPawn.cpp \
    TootRepsi/Private/TootFlyBotGM.cpp \
    TootRepsi/Private/TootMainGM.cpp \
    TootRepsi/Private/TootRepsiCharacter.cpp \
    TootRepsi/Private/TootRepsiPlayerCtrlor.cpp \
    TootRepsi/PrjLog.cpp \
    TootRepsi/TootRepsi.cpp
