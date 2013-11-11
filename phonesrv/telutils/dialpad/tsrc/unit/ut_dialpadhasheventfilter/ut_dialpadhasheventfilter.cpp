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
#include "ut_dialpadhasheventfilter.h"
#include "dialpadtest.h"
#include "dialpadhasheventfilter.h"
#include "dialpad.h"

const int WAIT_TIME = 300;

/*void setValidatorReturnValue(QVariant &returnValue)
{
    returnValue.setValue<QString>(QRegExp::escape(KValidKeySequence));
}*/

UT_DialpadHashEventFilter::UT_DialpadHashEventFilter()
    :
    m_dialPad(0), 
    m_eventFilter(0),
    m_lineEditMock(0)
{
    
}

UT_DialpadHashEventFilter::~UT_DialpadHashEventFilter()
{
    delete m_dialPad;
    delete m_eventFilter;
    delete m_lineEditMock;
}

void UT_DialpadHashEventFilter::init()
{
    initialize();
    
    m_lineEditMock = new HbLineEdit();
    QT_TRAP_THROWING(SmcDefaultValue<HbLineEdit&>::SetL(*m_lineEditMock))
    
    XQAiwInterfaceDescriptor descriptor;
    QString dummyOperation;
    bool isEmbedded = false;
    QList<XQAiwInterfaceDescriptor> interfaceList;
    interfaceList.append(descriptor);
    /*EXPECT(XQApplicationManager, list).returns(interfaceList);
    QPointer<XQAiwRequest> aiwRequest(new XQAiwRequest(
        descriptor, dummyOperation, isEmbedded));
    EXPECT(XQApplicationManager, create).returns(aiwRequest.data());
    EXPECT(XQAiwRequest, send)
        .willOnce(invoke(setValidatorReturnValue))
        .returns(true);*/
    
    SmcDefaultValue<QString>::SetL(QString());
    HbMainWindow *dummyWindow = NULL;
    m_dialPad = new Dialpad(*dummyWindow);
    m_eventFilter = new DialpadHashEventFilter(m_dialPad);
    
    //QVERIFY(aiwRequest.isNull());
    QVERIFY(verify());
}

void UT_DialpadHashEventFilter::cleanup()
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

/*void setServiceRequestReturnValue(QVariant & returnValue)
{
    returnValue.setValue<bool>(true);
}*/

void UT_DialpadHashEventFilter::testeventFilter()
{
    // start long press timer
    EXPECT(XQAiwRequest, send).times(0);
    QKeyEvent keyEvent(
        QEvent::KeyPress,
        Qt::Key_NumberSign,
        Qt::NoModifier);    
    bool filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent);
    QVERIFY(!filtered);
    QVERIFY(verify());
    
    // stop long press timer
    QKeyEvent keyEvent2(
        QEvent::KeyRelease,
        Qt::Key_NumberSign,
        Qt::NoModifier);
    
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent2);
    QVERIFY(!filtered);
    QVERIFY(verify());
    
    // some other buttons pressed and released
    EXPECT(XQAiwRequest, send).times(0);
    QKeyEvent keyEvent3(
        QEvent::KeyPress,
        Qt::Key_3,
        Qt::NoModifier);
    // start long press timer
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent3);
    QVERIFY(!filtered);
    QVERIFY(verify());
    
    QKeyEvent keyEvent4(
        QEvent::FocusIn,
        Qt::Key_1,
        Qt::NoModifier);
    // stop long press timer
    filtered = m_eventFilter->eventFilter(m_dialPad, &keyEvent4);
    QVERIFY(!filtered);
    QVERIFY(verify());
    
}

void UT_DialpadHashEventFilter::testhandleLongKeyPress()
{
    m_eventFilter->handleLongKeyPress();
    m_eventFilter->mKeyEvent = Qt::Key_NumberSign;
    m_eventFilter->handleLongKeyPress();
}

void UT_DialpadHashEventFilter::testhandleCallButtonPress()
{   
    // inherited method not supported
    EXPECT(XQAiwRequest, send).times(0);
    bool callButtonPress = m_eventFilter->handleCallButtonPress();
    QVERIFY(!callButtonPress);
}

void UT_DialpadHashEventFilter::testhandleMailboxOperation()
{
    m_eventFilter->handleMailboxOperation();
    
    // another call to test other note
    m_eventFilter->handleMailboxOperation();
}

#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    UT_DialpadHashEventFilter tc;
    return QTest::qExec(&tc, argc, argv);
}
