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

HEADERS += ../../../../phonesrv_plat/ss_settings_api/inc/sssettingswrapper.h \
           ../../../../phonesrv_plat/ss_settings_api/inc/sssettingswrappertypes.h \
   
symbian: {
    SOURCES += ./src/sssettingswrapper.cpp \
              ./src/sssettingswrapper_p.cpp
} 
else: {
    SOURCES = ./src/sssettingswrapper_s.cpp
}
       
