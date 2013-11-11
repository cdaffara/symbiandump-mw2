# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:  QTMS API project description
#

TEMPLATE = lib
TARGET   = qtmsapi
CONFIG  += dll
DEFINES += BUILD_TMS_WRAPPER_DLL

QT = core
CONFIG -= gui

DEFINES += SYMBIAN
TARGET.CAPABILITY = All -tcb
TARGET.EPOCALLOWDLLDATA	= 1
TARGET.UID2 = 0x10009D8D
TARGET.UID3 = 0x10207CA5

BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
                             "rom/qtms.iby CORE_MW_LAYER_IBY_EXPORT_PATH(qtms.iby)" \
                             "data/qtmsapi_stub.sis /epoc32/data/z/system/install/qtmsapi_stub.sis"

DEPENDPATH += . inc
INCLUDEPATH += . \
               ./inc

defBlock = \
           "$${LITERAL_HASH}if defined(EABI)" \
           "DEFFILE  eabi/qtms.def" \
           "$${LITERAL_HASH}else" \
           "DEFFILE  bwins/qtms.def" \
           "$${LITERAL_HASH}endif"
MMP_RULES += defBlock

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
               $$OS_LAYER_LIBC_SYSTEMINCLUDE \
               $$OS_LAYER_GLIB_SYSTEMINCLUDE

LIBS +=	-ltmsfactory \
        -ltmsapi \
        -ltmsutility \
        -llibglib \
        -llibgobject \
        -llibgmodule \
        -llibstdcpp

# $$_PRO_FILE_PWD_ points to the directory of the pro file
MOC_DIR = ./tmp

INCLUDEPATH += . \
               . inc \
               ../ \
               ../mmappfw_plat/qt_telephony_multimedia_service_api/inc/

DEPENDPATH  += $$INCLUDEPATH src

# Headers
HEADERS += qtmsfactory.h \
           qtmscall.h \
           qtmsstream.h \
           qtmsbuffer.h \
           qtmsclientsource.h \
           qtmsclientsink.h \
           qtmsvolumeeffect.h \
           qtmsgaineffect.h \
           qtmsglobalgaineffect.h \
           qtmsglobalvoleffect.h \
           qtmsglobalrouting.h \
           qtmsamrformat.h \
           qtmspcmformat.h \
           qtmsilbcformat.h \
           qtmsg711format.h \
           qtmsg729format.h \
           qtmsformat.h \
           qtmsmodemsink.h \
           qtmsspeakersink.h \
           qtmsmodemsource.h \
           qtmsmicsource.h \
           qtmsringtone.h \
           qtmsdtmf.h \
           qtmsinbandtone.h

# Source
SOURCES += qtmsfactory.cpp \
           qtmsfactoryimpl.cpp \
           qtmsbufferimpl.cpp \
           qtmsmembuffer.cpp \
           qtmscall.cpp \
           qtmscallimpl.cpp \
           qtmsstream.cpp \
           qtmsstreamimpl.cpp \
           qtmsclientsource.cpp \
           qtmsclientsourceimpl.cpp \
           qtmsclientsink.cpp \
           qtmsclientsinkimpl.cpp \
           qtmsvolumeeffect.cpp \
           qtmsvolumeeffectimpl.cpp \
           qtmsgaineffect.cpp \
           qtmsgaineffectimpl.cpp \
           qtmsglobalgaineffect.cpp \
           qtmsglobalgaineffectimpl.cpp \
           qtmsglobalvoleffect.cpp \
           qtmsglobalvoleffectimpl.cpp \
           qtmsglobalrouting.cpp \
           qtmsglobalroutingimpl.cpp \
           qtmspcm.cpp \
           qtmspcmimpl.cpp \
           qtmsilbc.cpp \
           qtmsilbcimpl.cpp \
           qtmsamr.cpp \
           qtmsamrimpl.cpp \
           qtmsg711.cpp \
           qtmsg711impl.cpp \
           qtmsg729.cpp \
           qtmsg729impl.cpp \
           qtmsformat.cpp \
           qtmsmodemsink.cpp \
           qtmsmodemsinkimpl.cpp \
           qtmsspeakersink.cpp \
           qtmsspeakersinkimpl.cpp \
           qtmsmodemsource.cpp \
           qtmsmodemsourceimpl.cpp \
           qtmsmicsource.cpp \
           qtmsmicsourceimpl.cpp \
           qtmsringtone.cpp \
           qtmsringtoneimpl.cpp \
           qtmsdtmf.cpp \
           qtmsdtmfimpl.cpp \
           qtmsinbandtone.cpp \
           qtmsinbandtoneimpl.cpp

SYMBIAN_PLATFORMS = DEFAULT

