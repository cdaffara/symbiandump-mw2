/*
* Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Ut_VmbxUiEngine class
*
*/

// System includes
#include <QtTest/QtTest>
#include <cvoicemailboxentry.h>

// User includes
#include "vmbxuiengine.h"
#include "ut_vmbxuiengine.h"

void setTestEnv(TInt aExpRet)
{
    globalExpRet = aExpRet;
    qDebug("setTestEnv %d", globalExpRet);
}


/*!
    Ut_VmbxUiEngine::initTestCase
    QTestLib initialization method, called for each test case.
*/
void Ut_VmbxUiEngine::initTestCase()
{
    mUiEngine = new VmbxUiEngine();
    QVERIFY(mUiEngine);
}

/*!
    Ut_VmbxUiEngine::cleanupTestCase
    QTestLib cleanup method, called for each test case.
*/
void Ut_VmbxUiEngine::cleanupTestCase()
{
    if ( mUiEngine ) {
        delete mUiEngine;
        mUiEngine = NULL;
    }
}

/*!
    Ut_VmbxUiEngine::testGetCsVoice1Number
    testCreateVmbxEntry test case
    Connects to test object signal and verifies received data.
*/
void Ut_VmbxUiEngine::testCsVoice1Number()
{
    QVERIFY(mUiEngine);
    QString in("123456");
    QString out("");
    setTestEnv(KErrNone);
    mUiEngine->setCsVoiceNumber(in);
    mUiEngine->getCsVoiceNumber(out);
    QCOMPARE(in, out);
    
    setTestEnv(KErrNotFound);
    mUiEngine->setCsVoiceNumber(in);
    mUiEngine->getCsVoiceNumber(out);
    QCOMPARE(in, out);
}

/*!
    Ut_VmbxUiEngine::testGetCsVideo1Number
    testVmbxType test case
    Connects to test object signal and verifies received data.
*/
void Ut_VmbxUiEngine::testCsVideo1Number()
{
    QVERIFY(mUiEngine);
    
    QString in("123456");
    QString out("");

    setTestEnv(KErrNone);
    mUiEngine->setCsVideoNumber(in);
    mUiEngine->getCsVideoNumber(out);
    QCOMPARE(in, out);
    
    setTestEnv(KErrNotFound);
    mUiEngine->setCsVideoNumber(in);
    mUiEngine->getCsVideoNumber(out);
    QCOMPARE(in, out);
}

void Ut_VmbxUiEngine::testIsVideoSupport()
{
    QVERIFY(mUiEngine);
    mUiEngine->isVideoSupport();
}

void Ut_VmbxUiEngine::testHandleNotifyL()
{
    QVERIFY(mUiEngine);
    CVoiceMailboxEntry *entry = CVoiceMailboxEntry::NewL();
    QVERIFY(entry);
    mUiEngine->HandleNotifyL(*entry);
}


//End file
