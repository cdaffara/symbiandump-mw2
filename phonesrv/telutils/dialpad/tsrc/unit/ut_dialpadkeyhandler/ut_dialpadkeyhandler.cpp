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
#ifdef Q_OS_SYMBIAN
#include <featmgr.h>
#endif // Q_OS_SYMBIAN

#include <smcmockclassincludes.h>
#include "ut_dialpadkeyhandler.h"
#include "dialpad.h"
#include "dialpadkeyhandler.h"
#include "dialpadtest.h"

HbMainWindow *HbWidget::mainWindow() const
{
    return 0;
}


void QObject::installEventFilter(QObject *obj)
{
    SMC_MOCK_METHOD1( void, QObject *, obj )
}

void QObject::removeEventFilter(QObject *obj)
{
    SMC_MOCK_METHOD1( void, QObject *, obj )
}

UT_DialpadKeyHandler::UT_DialpadKeyHandler()
    :
    m_dialPad(0), 
    m_keyHandler(0)
{
    
}


UT_DialpadKeyHandler::~UT_DialpadKeyHandler()
{
    delete m_dialPad;
    delete m_keyHandler;
}


void UT_DialpadKeyHandler::init()
{
    initialize();
    
    HbMainWindow *dummyWindow = 0;
    m_dialPad = new Dialpad(*dummyWindow);
}


void UT_DialpadKeyHandler::cleanup()
{
    reset();
    
    delete m_dialPad;
    m_dialPad = NULL;
    delete m_keyHandler;
    m_keyHandler = NULL;
}


void UT_DialpadKeyHandler::t_constructionWithAllFilters()
{
    EXPECT(QObject, installEventFilter).times(5);
    EXPECT(QObject, removeEventFilter).times(5);
    
    DialpadKeyHandler::DialpadKeyEventFilters filters(
        DialpadKeyHandler::VoiceMailbox |
        DialpadKeyHandler::VideoMailBox |
        DialpadKeyHandler::Bluetooth |
        DialpadKeyHandler::KeySequence |
        DialpadKeyHandler::EmergencyCall |
        DialpadKeyHandler::Hash );
    
    m_keyHandler = new DialpadKeyHandler(m_dialPad, filters, this);
    delete m_keyHandler;
    m_keyHandler = 0;
    
    QVERIFY(verify());
}


void UT_DialpadKeyHandler::t_constructionWithNoFilters()
{
    EXPECT(QObject, installEventFilter).times(0);
    
    DialpadKeyHandler::DialpadKeyEventFilters filters;
    m_keyHandler = new DialpadKeyHandler(m_dialPad, filters, this);
    
    QVERIFY(verify());
}


void UT_DialpadKeyHandler::t_deprecatedConstructionWithVideoMailbox()
{
#ifdef Q_OS_SYMBIAN
    EXPECT(FeatureManager, FeatureSupported).returns(true);
    EXPECT(QObject, installEventFilter).times(4);
    
    HbMainWindow *dummyWindow = 0;
    m_keyHandler = new DialpadKeyHandler(m_dialPad, *dummyWindow, this);
    
    QVERIFY(verify());
#endif // Q_OS_SYMBIAN
}


void UT_DialpadKeyHandler::t_deprecatedConstructionWithoutVideoMailbox()
{
    EXPECT(FeatureManager, FeatureSupported).returns(false);
    EXPECT(QObject, installEventFilter).times(3);
    
    HbMainWindow *dummyWindow = 0;
    m_keyHandler = new DialpadKeyHandler(m_dialPad, *dummyWindow, this);
    
    QVERIFY(verify());
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    UT_DialpadKeyHandler tc;
    return QTest::qExec(&tc, argc, argv);
}
