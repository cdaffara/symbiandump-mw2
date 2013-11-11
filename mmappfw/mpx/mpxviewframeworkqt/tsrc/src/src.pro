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
#

TEMPLATE = app
TARGET = tst_mpxviewframeworkqt

CONFIG += qtestlib console hb

INCLUDEPATH += .
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
               /epoc32/include/mw/qt \
               /epoc32/include/mw/qttest

SOURCES += tst_mpxviewframework.cpp 


LIBS +=     -lmpxviewframeworkqt -lxqplugins

symbian {

  TARGET.CAPABILITY = ALL -TCB

}

