#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: 
#

HEADERS += ../../../../phonesrv_plat/phone_settings_api/inc/psetcallbarringwrapper.h \
           ../../../../phonesrv_plat/phone_settings_api/inc/psetcalldivertingwrapper.h \
           ../../../../phonesrv_plat/phone_settings_api/inc/psetcallwaitingwrapper.h \
           ../../../../phonesrv_plat/phone_settings_api/inc/psetcliwrapper.h \
           ../../../../phonesrv_plat/phone_settings_api/inc/psetnetworkwrapper.h \
           ../../../../phonesrv_plat/phone_settings_api/inc/psetwrapper.h \
           ../../../../phonesrv_plat/phone_settings_api/inc/psetwrappertypes.h \
           ./src/psetcallbarringwrapper_p.h \
           ./src/psetcalldivertingwrapper_p.h \
           ./src/psetcallwaitingwrapper_p.h \
           ./src/psetcliwrapper_p.h \
           ./src/psetnetworkwrapper_p.h
           
symbian: {
    SOURCES += ./src/psetcallbarringwrapper_p.cpp \
        ./src/psetcalldivertingwrapper_p.cpp \
        ./src/psetcliwrapper_p.cpp \
        ./src/psetcallwaitingwrapper_p.cpp \
        ./src/psetnetworkwrapper_p.cpp \
        ./src/psetwrapper.cpp \
        ./src/psetcliwrapper.cpp \
        ./src/psetcallwaitingwrapper.cpp \
        ./src/psetcalldivertingwrapper.cpp \
        ./src/psetnetworkwrapper.cpp \
        ./src/psetnetworkinfoconverter.cpp \
        ./src/psetcallbarringwrapper.cpp
} 
else: {
    SOURCES = ./src/psetwrapper_s.cpp
}
           