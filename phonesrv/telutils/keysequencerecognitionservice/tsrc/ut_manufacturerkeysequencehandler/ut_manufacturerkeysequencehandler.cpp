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
#include "ut_manufacturerkeysequencehandler.h"
#define private friend class UT_ManufacturerKeySequenceHandler;private
#include "manufacturerkeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "qtestmains60.h"

const bool KIsEmbedded = false;

void generateException()
{
    qBadAlloc();
}

UT_ManufacturerKeySequenceHandler::UT_ManufacturerKeySequenceHandler()
    :
    m_handler(0)
{
    
}


UT_ManufacturerKeySequenceHandler::~UT_ManufacturerKeySequenceHandler()
{
    delete m_handler;
}


void UT_ManufacturerKeySequenceHandler::init()
{
    initialize();
    
    m_handler = new ManufacturerKeySequenceHandler();
}


void UT_ManufacturerKeySequenceHandler::cleanup()
{
    reset();
    
    delete m_handler;
    m_handler = 0;
}


void UT_ManufacturerKeySequenceHandler::t_keySequenceValidator()
{
    QString expectedValidator =
        QRegExp::escape(KCodeSwVersion) + "|" +
        QRegExp::escape(KCodeActivateRfsNormal)  + "|" +
        QRegExp::escape(KCodeActivateRfsDeep) + "|" +
        QRegExp::escape(KCodeBtAddress) + "|" +
        QRegExp::escape(KCodeRFLoopback);
    
    expect("KeySequenceHandler::setKeySequenceValidator")
        .with(expectedValidator);
    
    QScopedPointer<ManufacturerKeySequenceHandler> handler(
        new ManufacturerKeySequenceHandler());
    
    QVERIFY(verify());
}


void UT_ManufacturerKeySequenceHandler::t_executeKeySequenceDeviceManagerCodes()
{
    XQAiwInterfaceDescriptor dummyDescriptor;
    QPointer<XQAiwRequest> request =
        new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.services"), QString("devicemanager"), 
            QString("showVersionNumber()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(true);
    
    bool handled = m_handler->executeKeySequence(KCodeSwVersion);
    
    QVERIFY(verify());
    emit request->requestOk(QVariant(true));
    QVERIFY(request.isNull());
    QCOMPARE(handled, true);
}


void UT_ManufacturerKeySequenceHandler::t_executeKeySequenceFactoryResetCodes()
{
    XQAiwInterfaceDescriptor dummyDescriptor;
    QPointer<XQAiwRequest> request =
        new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.symbian"), QString("IFactoryReset"), 
            QString("showResetUi()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(true);
    
    bool handled = m_handler->executeKeySequence(KCodeActivateRfsNormal);
    
    QVERIFY(verify());
    emit request->requestOk(QVariant(true));
    QVERIFY(request.isNull());
    QCOMPARE(handled, true);
    
    request = new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.symbian"), QString("IFactoryReset"), 
            QString("showResetUi()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(true);
    
    handled = m_handler->executeKeySequence(KCodeActivateRfsDeep);
    
    QVERIFY(verify());
    emit request->requestOk(QVariant(true));
    QVERIFY(request.isNull());
    QCOMPARE(handled, true);
}


void UT_ManufacturerKeySequenceHandler::t_executeKeySequenceBluetoothCodes()
{
    XQAiwInterfaceDescriptor dummyDescriptor;
    QPointer<XQAiwRequest> request =
        new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.services"), QString("bluetooth"), 
            QString("showBluetoothDeviceAddress()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(true);
    
    bool handled = m_handler->executeKeySequence(KCodeBtAddress);
    
    QVERIFY(verify());
    emit request->requestOk(QVariant(true));
    QVERIFY(request.isNull());
    QCOMPARE(handled, true);
    
    request = new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.services"), QString("bluetooth"), 
            QString("showBluetoothLoopback()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(true);
    
    handled = m_handler->executeKeySequence(KCodeRFLoopback);
    
    QVERIFY(verify());
    emit request->requestOk(QVariant(true));
    QVERIFY(request.isNull());
    QCOMPARE(handled, true);
}


void UT_ManufacturerKeySequenceHandler::t_executeInvalidKeySequence()
{
    const QString KUnknownSequence = "123";
    bool handled = m_handler->executeKeySequence(KUnknownSequence);
    QCOMPARE(handled, false);
}


void UT_ManufacturerKeySequenceHandler::t_executeKeySequenceRequestDispatchingFails()
{
    // controlled synchronous Qt Highway error during request processing
    XQAiwInterfaceDescriptor dummyDescriptor;
    QPointer<XQAiwRequest> request =
        new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.services"), QString("devicemanager"), 
            QString("showVersionNumber()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(false);
    
    bool handled = m_handler->executeKeySequence(KCodeSwVersion);
    m_handler->requestError(0, QString(""));
    
    QVERIFY(verify());
    QVERIFY(request.isNull());
    QCOMPARE(handled, false);
    
    // controlled asynchronous Qt Highway error during request processing
    request = new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.services"), QString("devicemanager"), 
            QString("showVersionNumber()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(true);
    
    handled = m_handler->executeKeySequence(KCodeSwVersion);
    
    QVERIFY(verify());
    emit request->requestError(0, QString(""));
    QVERIFY(request.isNull());
    // asynchronous errors have to be silently discarded
    QCOMPARE(handled, true);
    
    // exception during service request dispatching/send
    request = new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.services"), QString("devicemanager"), 
            QString("showVersionNumber()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send)
        .willOnce(invokeWithoutArguments(generateException));
    
    int result = 0;
    QT_TRYCATCH_ERROR(
        result, handled = m_handler->executeKeySequence(KCodeSwVersion));
    
    QVERIFY(verify());
    QVERIFY(request.isNull());
    QVERIFY(0 != result);
    
    // request object creation fails
    handled = m_handler->executeKeySequence(KCodeSwVersion);
    QCOMPARE(handled, false);
}


void UT_ManufacturerKeySequenceHandler::t_executeKeySequenceSimultaneousRequest()
{
    XQAiwInterfaceDescriptor dummyDescriptor;
    QPointer<XQAiwRequest> request =
        new XQAiwRequest(dummyDescriptor, QString(""), KIsEmbedded);
    EXPECT(XQApplicationManager, create)
        .with(
            QString("com.nokia.services"), QString("devicemanager"), 
            QString("showVersionNumber()"), KIsEmbedded)
        .returns(request.data());
    EXPECT(XQAiwRequest, send).returns(true);
    
    bool handled = m_handler->executeKeySequence(KCodeSwVersion);
    
    QVERIFY(verify());
    QVERIFY(!request.isNull());
    QCOMPARE(handled, true);
    
    // simultaneous request attempt
    EXPECT(XQApplicationManager, create).times(0);
    EXPECT(XQAiwRequest, send).times(0);
    handled = m_handler->executeKeySequence(KCodeSwVersion);
    QCOMPARE(handled, false);
}

QTEST_MAIN_S60(UT_ManufacturerKeySequenceHandler)
