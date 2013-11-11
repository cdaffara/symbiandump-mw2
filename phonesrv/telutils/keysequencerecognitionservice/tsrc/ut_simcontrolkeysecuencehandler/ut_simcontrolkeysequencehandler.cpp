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
#include <secuimanualsecuritysettings.h>
#include "ut_simcontrolkeysequencehandler.h"
#include "simcontrolkeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "qtestmains60.h"

_LIT(KOldPinCode, "1234");
_LIT(KPukCode, "11111111");
_LIT(KNewPinCode, "4321");
_LIT(KVerifyNewPinCode, "4321");


void generateLeaveL()
{
    User::Leave(KErrUnknown);
}


UT_SimControlKeySequenceHandler::UT_SimControlKeySequenceHandler()
    :
    m_handler(0)
{
    
}


UT_SimControlKeySequenceHandler::~UT_SimControlKeySequenceHandler()
{
    delete m_handler;
}


void UT_SimControlKeySequenceHandler::init()
{
    initialize();
    
    m_handler = new SimControlKeySequenceHandler();
}


void UT_SimControlKeySequenceHandler::cleanup()
{
    reset();
    
    delete m_handler;
    m_handler = 0;
}

void UT_SimControlKeySequenceHandler::t_constructionFails()
{
    EXPECT( CManualSecuritySettings, NewL ).willOnce(invokeWithoutArguments(generateLeaveL));
    
    int result = 0;
    QT_TRYCATCH_ERROR(result, 
        QScopedPointer<SimControlKeySequenceHandler> handler(
            new SimControlKeySequenceHandler())
    )
    QVERIFY(0 != result);
}


void UT_SimControlKeySequenceHandler::t_changePin1()
    {
    const QString KChangePin1KeySequence( "**04*1234*4321*4321*" );
    
    EXPECT(CManualSecuritySettings, ChangePinL)
        .with(CManualSecuritySettings::EPin1, KOldPinCode(), KNewPinCode(), KVerifyNewPinCode());
    
    m_handler->executeKeySequence( KChangePin1KeySequence );
    
    QVERIFY(verify());
    
    
    // leave handling
    EXPECT( CManualSecuritySettings, ChangePinL ).willOnce(invokeWithoutArguments(generateLeaveL))
        .with(CManualSecuritySettings::EPin1, KOldPinCode(), KNewPinCode(), KVerifyNewPinCode());
    int result = 0;
    QT_TRYCATCH_ERROR(result, 
            m_handler->executeKeySequence(KChangePin1KeySequence) )
    
    QVERIFY(0 != result);
    }

void UT_SimControlKeySequenceHandler::t_changePin2()
    {
    const QString KChangePin2KeySequence( "**042*1234*4321*4321*" );
    
    EXPECT(CManualSecuritySettings, ChangePinL)
        .with(CManualSecuritySettings::EPin2, KOldPinCode(), KNewPinCode(), KVerifyNewPinCode());
    
    m_handler->executeKeySequence(KChangePin2KeySequence);
    
    QVERIFY(verify());
    
    
    //leave handling
    EXPECT( CManualSecuritySettings, ChangePinL ).willOnce(invokeWithoutArguments(generateLeaveL))
            .with(CManualSecuritySettings::EPin2, KOldPinCode(), KNewPinCode(), KVerifyNewPinCode());
    
    int result = 0;
    QT_TRYCATCH_ERROR(result, 
            m_handler->executeKeySequence(KChangePin2KeySequence) )
    
    QVERIFY(0 != result);
    }

void UT_SimControlKeySequenceHandler::t_unblockPin1()
    {
    const QString KUnblockPin1KeySequence( "**05*11111111*4321*4321*" );
    
    EXPECT(CManualSecuritySettings, UnblockPinL)
        .with(CManualSecuritySettings::EPin1, KPukCode(), KNewPinCode(), KVerifyNewPinCode());
    
    m_handler->executeKeySequence(KUnblockPin1KeySequence);
    
    QVERIFY(verify());
    
    
    //leave handling
    EXPECT( CManualSecuritySettings, UnblockPinL ).willOnce(invokeWithoutArguments(generateLeaveL))
        .with(CManualSecuritySettings::EPin1, KPukCode(), KNewPinCode(), KVerifyNewPinCode());
    
    int result = 0;
    QT_TRYCATCH_ERROR(result, 
            m_handler->executeKeySequence(KUnblockPin1KeySequence) )
    
    QVERIFY(0 != result);
    }


void UT_SimControlKeySequenceHandler::t_unblockPin2()
    {
     const QString KUnblockPin2KeySequence( "**052*11111111*4321*4321*" );
    
     EXPECT(CManualSecuritySettings, UnblockPinL)
         .with(CManualSecuritySettings::EPin2, KPukCode(), KNewPinCode(), KVerifyNewPinCode());
     
     m_handler->executeKeySequence(KUnblockPin2KeySequence);
     
     QVERIFY(verify());
     
     
     //leave handling
     EXPECT( CManualSecuritySettings, UnblockPinL ).willOnce(invokeWithoutArguments(generateLeaveL))
         .with(CManualSecuritySettings::EPin2, KPukCode(), KNewPinCode(), KVerifyNewPinCode());
     
     int result = 0;
     QT_TRYCATCH_ERROR(result, 
             m_handler->executeKeySequence(KUnblockPin2KeySequence) )
     
     QVERIFY(0 != result);
    }


void UT_SimControlKeySequenceHandler::t_keySequenceValidator()
    {
    QString expectedValidator =
        KCodeChangePin1 + "|" +
        KCodeChangePin2 + "|" +
        KCodeUnblockPin1 + "|" +
        KCodeUnblockPin2;
    
    expect("KeySequenceHandler::setKeySequenceValidator")
        .with(expectedValidator);
    
    QScopedPointer<SimControlKeySequenceHandler> handler(
        new SimControlKeySequenceHandler());
    
    QVERIFY(verify());
    }

QTEST_MAIN_S60(UT_SimControlKeySequenceHandler)
