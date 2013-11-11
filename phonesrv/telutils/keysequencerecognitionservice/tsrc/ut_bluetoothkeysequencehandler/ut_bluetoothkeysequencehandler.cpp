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

#include <QtTest/QtTest>
#ifdef Q_OS_SYMBIAN
#include <btxqserviceapi.h>
#endif
#include "ut_bluetoothkeysequencehandler.h"
#include "bluetoothkeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "qtestmains60.h"
#include "xqappmgr.h"

QString mService;
QString mInterface;
QString mOperation;
bool mEmbedded;
bool mSendCalled;

#ifdef Q_OS_SYMBIAN
XQAiwRequest* XQApplicationManager::create(const QString& service, const QString& interface, const QString& operation, bool embedded)
{
    mService = service;
    mInterface = interface;
    mOperation = operation;
    mEmbedded = embedded;
    return new XQAiwRequest(); 
}
void XQAiwRequest::send() { mSendCalled = true; }
#endif

UT_BluetoothKeySequenceHandler::UT_BluetoothKeySequenceHandler() :  m_handler(0)
{
    
}

UT_BluetoothKeySequenceHandler::~UT_BluetoothKeySequenceHandler()
{
    delete m_handler;
}

void UT_BluetoothKeySequenceHandler::init()
{
    mService = QString("");
    mInterface = QString("");
    mOperation = QString("");
    mEmbedded = false;
    mSendCalled = false;
    m_handler = new BluetoothKeySequenceHandler();
}

void UT_BluetoothKeySequenceHandler::cleanup()
{
    delete m_handler;
    m_handler = 0;
}

void UT_BluetoothKeySequenceHandler::testShowBTLocalAddressKeySequence()
{
#ifdef Q_OS_SYMBIAN
    bool handled = m_handler->executeKeySequence(KShowBTAddress);
    QVERIFY(handled == true);
    QCOMPARE(mService, BluetoothServiceName);
    QCOMPARE(mInterface, BluetoothInterfaceShowLocalAddress);
    QCOMPARE(mOperation, BluetoothShowLocalAddress);
    QVERIFY(mEmbedded == false);
    QVERIFY(mSendCalled == true);
#endif
}

void UT_BluetoothKeySequenceHandler::testActivateBTDutModeKeySequence()
{
#ifdef Q_OS_SYMBIAN
    bool handled = m_handler->executeKeySequence(KActivateBTTestMode);
    QVERIFY(handled == true);
    QCOMPARE(mService, BluetoothServiceName);
    QCOMPARE(mInterface, BluetoothInterfaceActivateDutMode);
    QCOMPARE(mOperation, BluetoothActivateDutMode);
    QVERIFY(mEmbedded == false);
    QVERIFY(mSendCalled == true);
#endif
}

void UT_BluetoothKeySequenceHandler::testToggleBTSSPDebugModeKeySequence()
{
#ifdef Q_OS_SYMBIAN
    bool handled = m_handler->executeKeySequence(KToggleBTSSPDebugMode);
    QVERIFY(handled == true);
    QCOMPARE(mService, BluetoothServiceName);
    QCOMPARE(mInterface, BluetoothInterfaceToggleSspDebugMode);
    QCOMPARE(mOperation, BluetoothToggleSspDebugMode);
    QVERIFY(mEmbedded == false);
    QVERIFY(mSendCalled == true);
#endif
}

void UT_BluetoothKeySequenceHandler::testInvalidKeySequence()
{
#ifdef Q_OS_SYMBIAN
    bool handled = m_handler->executeKeySequence("*#1234#");
    QVERIFY(handled == false);
    QCOMPARE(mService, QString(""));
    QCOMPARE(mInterface, QString(""));
    QCOMPARE(mOperation, QString(""));
    QVERIFY(mEmbedded == false);
    QVERIFY(mSendCalled == false);
#endif
}

QTEST_MAIN_S60(UT_BluetoothKeySequenceHandler)
