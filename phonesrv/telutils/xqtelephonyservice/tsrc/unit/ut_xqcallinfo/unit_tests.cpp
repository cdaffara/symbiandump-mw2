/*!
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Unit tests for DialService.
*
*/

#include <QtTest/QtTest>

#include "xqcallinfo.h"
#include <ccpdefs.h>

extern bool GlobalEmptyCallList;
extern bool GlobalTriggerSignal;

class TestXQCallInfo : public QObject
{
    Q_OBJECT
public:
    TestXQCallInfo();
    virtual ~TestXQCallInfo();
    
public slots:
    void initTestCase ();
    void cleanupTestCase ();
    
private slots:
    void testGetCallInfo ();
    void testGetCallInfoEmptyCallInfo();
    void testIsCallInState();

private:
    
    XQCallInfo *m_XQCallInfo;
};

TestXQCallInfo::TestXQCallInfo():
    m_XQCallInfo(0)
    {
    }

TestXQCallInfo::~TestXQCallInfo ()
    {
    }

void TestXQCallInfo::initTestCase ()
    {
    GlobalEmptyCallList=false;
    m_XQCallInfo = XQCallInfo::create();
    }

void TestXQCallInfo::cleanupTestCase ()
    {
    delete m_XQCallInfo;
    }


void TestXQCallInfo::testGetCallInfo()
    {
    QList<CallInfo> calls;
    CallInfo callInfo;
    m_XQCallInfo->getCalls(calls);
    callInfo = calls.first();
    CallInfo callInfo2;
    callInfo2 = calls.last();
    
    QCOMPARE(callInfo.callType(), CCPCall::ECallTypeCSVoice );
    QCOMPARE(callInfo.callDirection(), CCPCall::EMobileOriginated);
    QCOMPARE(callInfo.callState(), CCPCall::EStateDialling);
    
    QCOMPARE(callInfo2.callType(), CCPCall::ECallTypeCSVoice );
    QCOMPARE(callInfo2.callDirection(), CCPCall::EMobileTerminated);
    QCOMPARE(callInfo2.callState(), CCPCall::EStateConnected);
    }

void TestXQCallInfo::testGetCallInfoEmptyCallInfo()
    {
    GlobalEmptyCallList=true;
    QList<CallInfo> calls;
    m_XQCallInfo->getCalls(calls);
    
    QCOMPARE(calls.count(), 0 );
    }

void TestXQCallInfo::testIsCallInState()
    {
    GlobalTriggerSignal=true;
    QSignalSpy spy( m_XQCallInfo, SIGNAL( callInformationChanged() ) );
    
    GlobalEmptyCallList=false;
    bool value(false);
    const CCPCall::TCallState state(CCPCall::EStateConnected );
    value = m_XQCallInfo->isCallInState(CCPCall::EStateConnected);
    
    QCOMPARE(value, true );
    QCOMPARE( spy.count(), 1 );
    }


QTEST_MAIN(TestXQCallInfo)
#include "unit_tests.moc"
