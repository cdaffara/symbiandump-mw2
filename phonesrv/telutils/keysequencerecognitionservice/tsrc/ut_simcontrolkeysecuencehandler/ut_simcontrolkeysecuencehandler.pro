#
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
# Description:
#
#

MOC_DIR = moc
CONFIG += hb qtestlib


TEMPLATE = app
TARGET = ut_simcontrolkeysecuencehandler 

QT        += core
QT        -= gui 


SOURCES   +=  ut_simcontrolkeysecuencehandler_reg.rss




symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += . ../shared ../../inc
    HEADERS += ../../inc/simcontrolkeysequencehandler.h
    SOURCES += ../../src/simcontrolkeysequencehandler.cpp

    HEADERS += ut_simcontrolkeysequencehandler.h
    SOURCES += ut_simcontrolkeysequencehandler.cpp 
    
    #MOCKED DEPENDENCIES
    HEADERS +=                                  \ 
        ../../inc/keysequencehandler.h          
    
  SOURCES +=                                    \ 
      ../shared/mock_keysequencehandler.cpp     \
      ../shared/mock_secuimanualsecuritysettings.cpp  \
      ../shared/mock_secui.cpp					\
      ../shared/mock_etel.cpp                   \
      ../shared/mock_rmmcustomapi.cpp           \
      ../shared/mock_etelmm.cpp

    
    LIBS += -lmocklib -lsymbianmock 
    LIBS += -letel -letelmm -lcustomapi 
    TARGET.UID3 = 0xED019EF7
}