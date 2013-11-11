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
# Description: USSD unit test project file.
# this project combines phone server client and server classes,
# glues them in one single package with the test module and test
# the class functionality all together.
#

TEMPLATE = app
TARGET = ut_ussd
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib
HEADERS += $$PUBLIC_HEADERS

symbian {
    TARGET.CAPABILITY = CAP_APPLICATION 
    }

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

# =====================================================================
# CODE SUBJECT TO TEST
# Note that both ussd client and server code is compiled!
# =====================================================================

# server
HEADERS += ../../../../../phoneserver/inc/ussd/cphsrvussdmanager.h
HEADERS += ../../../../../phoneserver/inc/ussd/cphsrvussdnotifynwrelease.h
HEADERS += ../../../../../phoneserver/inc/ussd/cphsrvussdreceivehandler.h
HEADERS += ../../../../../phoneserver/inc/ussd/cphsrvussdreplytimer.h
HEADERS += ../../../../../phoneserver/inc/ussd/cphsrvussdsendhandler.h
HEADERS += ../../../../../phoneserver/inc/ussd/cphsrvussdsessioncancelwaiter.h
HEADERS += ../../../../../phoneserver/inc/ussd/mphsrvussdmessagesentobserver.h
HEADERS += ../../../../../phoneserver/inc/ussd/mphsrvussdnetworkobserver.h
HEADERS += ../../../../../phoneserver/inc/ussd/mphsrvussdreplytimerobserver.h
HEADERS += ../../../../../phoneserver/inc/Standard/mphsrvphoneinterface.h
HEADERS += ../../../../../phoneserver/inc/Standard/cphsrvresourcemanager.h
HEADERS += ../../../../../phoneserver/inc/Standard/phsrvutils.h
HEADERS += ../../../../../phoneserver/inc/Standard/phsrvdebuginfo.h
HEADERS += ../../../../../phoneserver/inc/Standard/phsrvconfigure.h

SOURCES += ../../../../../phoneserver/src/ussd/cphsrvussdmanager.cpp
SOURCES += ../../../../../phoneserver/src/ussd/cphsrvussdnotifynwrelease.cpp
SOURCES += ../../../../../phoneserver/src/ussd/cphsrvussdreceivehandler.cpp
SOURCES += ../../../../../phoneserver/src/ussd/cphsrvussdreplytimer.cpp
SOURCES += ../../../../../phoneserver/src/ussd/cphsrvussdsendhandler.cpp
SOURCES += ../../../../../phoneserver/src/standard/phsrvutils.cpp
SOURCES += ../../../../../phoneserver/src/standard/phsrvdebuginfo.cpp

# client/ussdwrapper
HEADERS += ../../../../../phoneclient/inc/ussd/ussd_ext/rphcltussd.h
HEADERS += ../../../../../phoneclient/inc/ussdwrapper/ussd_ext/cphcltussdimp.h
HEADERS += ../../../../../phoneclient/inc/ussdwrapper/cphcltussdnotecontroller.h
HEADERS += ../../../../../phoneclient/inc/ussdwrapper/cphcltussdrequesthandler.h
HEADERS += ../../../../../phoneclient/inc/ussdwrapper/mphcltussdnotecontrollercallback.h
HEADERS += ../../../../../phoneclient/inc/ussdwrapper/mphcltussdrequesthandler.h
HEADERS += ../../../../../phoneclient/inc/SharedClientServer/phcltclientserver.h
HEADERS += ../../../../../phoneclient/inc/ussdwrapper/cphcltussdcommonconstant.h

SOURCES += ../../../../../phoneclient/src/ussdwrapper/cphcltussdext.cpp
SOURCES += ../../../../../phoneclient/src/ussdwrapper/cphcltussdimp.cpp
SOURCES += ../../../../../phoneclient/src/ussdwrapper/cphcltussdint.cpp
SOURCES += ../../../../../phoneclient/src/ussdwrapper/cphcltussdnotecontroller.cpp
SOURCES += ../../../../../phoneclient/src/ussdwrapper/cphcltussdrequesthandler.cpp
SOURCES += ../../../../../phoneclient/src/ussdwrapper/cphcltussdsatclient.cpp


# =====================================================================
# TEST CODE
# Explanation of the test/mock classes
# * ut_ussd:
#       QTestLib test cases
# * ut_testenv:
#       keep-up of the test environment, stubs and mocks etc.
# * rphcltussd_glue:
#       replaces RPhCltUssd implementation. Instead of connecting to
#       phone server, directs commands directly to USSD manager and
#       receives callbacks.
# * etel_mock:
#       Mocks ETEL, fakes different USSD send responses.
# * phoneclientserver_stubs:
#       Replaces the phoneserver resource reader with a dummy
#       Replaces phoneserver client with a dummy
# =====================================================================

HEADERS += ut_ussd.h
HEADERS += ut_testenv.h
HEADERS += ut_cphcltussdnotecontroller.h

SOURCES += main.cpp
SOURCES += ut_ussd.cpp
SOURCES += ut_testenv.cpp
SOURCES += rphcltussd_glue.cpp
SOURCES += etel_mock.cpp
SOURCES += phoneclientserver_stubs.cpp
SOURCES += ut_cphcltussdnotecontroller.cpp


LIBS += -lefsrv
LIBS += -lgsmu
LIBS += -lflogger
LIBS += -lCommonEngine
LIBS += -laknnotify
LIBS += -leiksrv
LIBS += -lbafl

