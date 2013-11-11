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
#include <HbMainWindow>
#include <HbLineEdit>
#ifdef Q_OS_SYMBIAN
#include <xqaiwinterfacedescriptor.h>
#include <xqaiwrequest.h>
#endif
#include "ut_dialpadkeysequenceeventfilter.h"
#include "dialpadtest.h"
#include "dialpadkeysequenceeventfilter.h"
#include "dialpad.h"

const QString KValidKeySequence("*#1#");

void setValidatorReturnValue(QVariant &returnValue)
{
    returnValue.setValue<QString>(QRegExp::escape(KValidKeySequence));
}

UT_DialpadKeySequenceEventFilter::UT_DialpadKeySequenceEventFilter()
    :
    m_dialPad(0), 
    m_eventFilter(0),
    m_lineEditMock(0)
{
    
}

UT_DialpadKeySequenceEventFilter::~UT_DialpadKeySequenceEventFilter()
{
    delete m_dialPad;
    delete m_eventFilter;
    delete m_lineEditMock;
}

void UT_DialpadKeySequenceEventFilter::init()
{
    initialize();
    
    m_lineEditMock = new HbLineEdit();
    QT_TRAP_THROWING(SmcDefaultValue<HbLineEdit&>::SetL(*m_lineEditMock))
    
    XQAiwInterfaceDescriptor descriptor;
    QString dummyOperation;
    bool isEmbedded = false;
    QList<XQAiwInterfaceDescriptor> interfaceList;
    interfaceList.append(descriptor);
    EXPECT(XQApplicationManager, list).returns(interfaceList);
    QPointer<XQAiwRequest> aiwRequest(new XQAiwRequest(
        descriptor, dummyOperation, isEmbedded));
    EXPECT(XQApplicationManager, create).returns(aiwRequest.data());
    EXPECT(XQAiwRequest, send)
        .willOnce(invoke(setValidatorReturnValue))
        .returns(true);
    
    SmcDefaultValue<QString>::SetL(QString());
    HbMainWindow *dummyWindow = NULL;
    m_dialPad = new Dialpad(*dummyWindow);
    m_eventFilter = new DialpadKeySequenceEventFilter(m_dialPad);
    
    QVERIFY(aiwRequest.isNull());
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::cleanup()
{
    reset();
    
    SmcDefaultValue<QString>::Reset();
    SmcDefaultValue<HbLineEdit&>::Reset();
    delete m_dialPad;
    m_dialPad = NULL;
    delete m_eventFilter;
    m_eventFilter = NULL;
    delete m_lineEditMock;
    m_lineEditMock = NULL;
}

#ifdef Q_OS_SYMBIAN

void setServiceRequestReturnValue(QVariant & returnValue)
{
    returnValue.setValue<bool>(true);
}

void UT_DialpadKeySequenceEventFilter::eventFilterValidKeySequence()
{
    EXPECT(HbLineEdit, text).returns(KValidKeySequence);
    EXPECT(XQAiwInterfaceDescriptor, isValid).returns(false);
    EXPECT(XQAiwInterfaceDescriptor, isValid).returns(true);
    XQAiwInterfaceDescriptor dummyDescriptor;
    QString dummyOperation;
    QPointer<XQAiwRequest> aiwRequest(new XQAiwRequest(
        dummyDescriptor, dummyOperation, false));
    EXPECT(XQApplicationManager, create).returns(aiwRequest.data());
    EXPECT(XQAiwRequest, send)
        .willOnce(invoke(setServiceRequestReturnValue))
        .returns(true);
    QKeyEvent keyEvent(
        QEvent::KeyRelease,
        Qt::Key_NumberSign,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    QVERIFY(aiwRequest.isNull());
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::eventFilterNotAKeyEvent()
{
    EXPECT(XQAiwRequest, send).times(0);
    QMouseEvent mouseEvent(
        QEvent::MouseMove,
        QPoint(),
        Qt::LeftButton,
        Qt::LeftButton,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &mouseEvent);
    QVERIFY(!filtered);
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::eventFilterNotAHashKey()
{
    EXPECT(XQAiwRequest, send).times(0);
    QKeyEvent keyEvent(
        QEvent::KeyRelease,
        Qt::Key_Escape,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::eventFilterNotValidKeySequence()
{
    const QString KInvalidKeySequence1("*##");
    const QString KInvalidKeySequence2("#1234#");
    const QString KInvalidKeySequence3("**1234#");
    const QString KInvalidKeySequence4("*#1234*");
    
    EXPECT(XQAiwRequest, send).times(0);
    
    EXPECT(HbLineEdit, text).returns(KInvalidKeySequence1);
    QKeyEvent keyEvent(
        QEvent::KeyRelease,
        Qt::Key_NumberSign,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    EXPECT(HbLineEdit, text).returns(KInvalidKeySequence2);
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    EXPECT(HbLineEdit, text).returns(KInvalidKeySequence3);
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    EXPECT(HbLineEdit, text).returns(KInvalidKeySequence4);
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    
    QVERIFY(verify());
}

void UT_DialpadKeySequenceEventFilter::eventFilterServiceRequestFails()
{
    // Qt Highway error while issuing service request
    EXPECT(HbLineEdit, text).returns(KValidKeySequence);
    EXPECT(XQAiwInterfaceDescriptor, isValid).returns(false);
    EXPECT(XQAiwInterfaceDescriptor, isValid).returns(true);
    XQAiwInterfaceDescriptor dummyDescriptor;
    QString dummyOperation;
    QPointer<XQAiwRequest> aiwRequest1(new XQAiwRequest(
        dummyDescriptor, dummyOperation, false));
    EXPECT(XQApplicationManager, create).returns(aiwRequest1.data());
    EXPECT(XQAiwRequest, send).returns(false);
    QKeyEvent keyEvent(
        QEvent::KeyRelease,
        Qt::Key_NumberSign,
        Qt::NoModifier);
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    QVERIFY(aiwRequest1.isNull());
    QVERIFY(verify());
    
    // service provider fails to fulfill request
    EXPECT(HbLineEdit, text).returns(KValidKeySequence);
    EXPECT(XQAiwInterfaceDescriptor, isValid).returns(false);
    EXPECT(XQAiwInterfaceDescriptor, isValid).returns(true);
    QPointer<XQAiwRequest> aiwRequest2(new XQAiwRequest(
        dummyDescriptor, dummyOperation, false));
    EXPECT(XQApplicationManager, create).returns(aiwRequest2.data());
    EXPECT(XQAiwRequest, send).returns(true);
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    QVERIFY(aiwRequest2.isNull());
    QVERIFY(verify());
}

#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    UT_DialpadKeySequenceEventFilter tc;
    return QTest::qExec(&tc, argc, argv);
}
