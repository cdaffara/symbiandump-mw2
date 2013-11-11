/*!
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#include <QtGui>
#include <QtTest/QtTest>
#include <hbdevicemessagebox.h>
#include <centralrepository.h>
#include <etel.h>
#include <etelmm.h>
#include <rmmcustomapi.h>
#include <mmtsy_names.h>
#include "ut_lifetimerkeysequencehandler.h"
#include "lifetimerkeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "telservicesinternalcrkeys.h"
#include "telservicesvariant.hrh"
#include "qtestmains60.h"

void generateLeaveL()
{
    User::Leave(KErrUnknown);
}

UT_LifeTimerKeySequenceHandler::UT_LifeTimerKeySequenceHandler()
    :
    m_handler(0)
{
    
}


UT_LifeTimerKeySequenceHandler::~UT_LifeTimerKeySequenceHandler()
{
    delete m_handler;
}


void UT_LifeTimerKeySequenceHandler::setLifeTimerData(
    TUint32 aKey,
    TInt & aValue )
{
    if (KTelSrvVariationFlags == aKey) {
        aValue = KTelSrvLVFlagLifeTimer;
    }
}


void UT_LifeTimerKeySequenceHandler::setLifeTimeData(
    TDes8& aLifeTimeInfo)
{
    RMmCustomAPI::TLifeTimeDataPckg &pckg = 
        reinterpret_cast<RMmCustomAPI::TLifeTimeDataPckg&>(aLifeTimeInfo);
    pckg.Copy(aLifeTimeInfo);
    pckg().iHours = 999;
    pckg().iMinutes = 59;
}


void UT_LifeTimerKeySequenceHandler::init()
{
    initialize();
    
    m_handler = new LifeTimerKeySequenceHandler();
}


void UT_LifeTimerKeySequenceHandler::cleanup()
{
    reset();
    
    delete m_handler;
    m_handler = 0;
}


void UT_LifeTimerKeySequenceHandler::t_constructionFails()
{
    EXPECT(CRepository, NewL).willOnce(invoke(generateLeaveL));
    
    int result = 0;
    QT_TRYCATCH_ERROR(result, 
        QScopedPointer<LifeTimerKeySequenceHandler> handler(
            new LifeTimerKeySequenceHandler())
    )
    QVERIFY(0 != result);
}


void UT_LifeTimerKeySequenceHandler::t_keySequenceValidator()
{
    // life timer feature enabled scenario
    EXPECT(CRepository, Get)
        .willOnce(invoke(this, setLifeTimerData))
        .returns(KErrNone);
    expect("KeySequenceHandler::setKeySequenceValidator")
        .with(QRegExp::escape(KCodeLifeTimer));
    
    QScopedPointer<LifeTimerKeySequenceHandler> handler1(
        new LifeTimerKeySequenceHandler());
    
    QVERIFY(verify());
    
    // life timer feature not enabled scenario
    EXPECT(CRepository, Get).returns(KErrNotFound);
    expect("KeySequenceHandler::setKeySequenceValidator").times(0);
    
    QScopedPointer<LifeTimerKeySequenceHandler> handler2(
        new LifeTimerKeySequenceHandler());
    
    QVERIFY(verify());
}


void UT_LifeTimerKeySequenceHandler::t_executeValidKeySequence()
{
    EXPECT(CRepository, Get)
        .willOnce(invoke(this, setLifeTimerData))
        .returns(KErrNone);
    EXPECT(RMmCustomAPI, GetLifeTime)
        .willOnce(invoke(this, setLifeTimeData));
    
    EXPECT(HbDeviceMessageBox, setTimeout).with(HbPopup::NoTimeout);
    EXPECT(HbDeviceMessageBox, show);
    bool handled = m_handler->executeKeySequence(KCodeLifeTimer);
    QCOMPARE(handled, true);
    
    QVERIFY(verify());
}


void UT_LifeTimerKeySequenceHandler::t_executeInvalidKeySequence()
{
    const QString KUnknownSequence("123");
    bool handled = m_handler->executeKeySequence(KUnknownSequence);
    QCOMPARE(handled, false);
}


void UT_LifeTimerKeySequenceHandler::t_executeKeySequenceEtelConnectionCreationFails()
{
    EXPECT(CRepository, Get)
        .willOnce(invoke(this, setLifeTimerData))
        .returns(KErrNone);
    EXPECT(RTelServer, Connect).willOnce(invoke(generateLeaveL));
    
    int result = 0;
    QT_TRYCATCH_ERROR(result, m_handler->executeKeySequence(KCodeLifeTimer));
    
    QVERIFY(0 != result);
    QVERIFY(verify());
}


void UT_LifeTimerKeySequenceHandler::t_executeKeySequenceLifeTimeDataQueryFails()
{
    EXPECT(CRepository, Get)
        .willOnce(invoke(this, setLifeTimerData))
        .returns(KErrNone);
    EXPECT(RMmCustomAPI, GetLifeTime).returns(KErrArgument);
    
    bool handled = m_handler->executeKeySequence(KCodeLifeTimer);
    
    QCOMPARE(handled, true);
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_LifeTimerKeySequenceHandler)
