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
#include <qsysteminfo.h>
#include <hbdevicemessagebox.h>
#include "ut_imeikeysequencehandler.h"
#include "imeikeysequencehandler.h"
#include "keysequencerecognitionservicedefs.h"
#include "qtestmains60.h"

QTM_USE_NAMESPACE

UT_ImeiKeySequenceHandler::UT_ImeiKeySequenceHandler()
    : 
    m_handler(0)
{
    
}


UT_ImeiKeySequenceHandler::~UT_ImeiKeySequenceHandler()
{
    delete m_handler;
}


void UT_ImeiKeySequenceHandler::init()
{
    initialize();
    
    m_handler = new ImeiKeySequenceHandler();
}


void UT_ImeiKeySequenceHandler::cleanup()
{
    reset();
    
    delete m_handler;
    m_handler = 0;
}


void UT_ImeiKeySequenceHandler::t_keySequenceValidator()
{
    expect("KeySequenceHandler::setKeySequenceValidator")
        .with(QRegExp::escape(KCodeImei));
    
    QScopedPointer<ImeiKeySequenceHandler> handler(
        new ImeiKeySequenceHandler());
    
    QVERIFY(verify());
}


void UT_ImeiKeySequenceHandler::t_executeValidKeySequence()
{
    const QString KSerialNumber = "1234567890";
    EXPECT(QtMobility::QSystemDeviceInfo, imei).returns(KSerialNumber);
    EXPECT(HbDeviceMessageBox, setTimeout).with(HbPopup::NoTimeout);
    EXPECT(HbDeviceMessageBox, show);
    bool handled = m_handler->executeKeySequence(KCodeImei);
    QCOMPARE(handled, true);
    QVERIFY(verify());
}


void UT_ImeiKeySequenceHandler::t_executeInvalidKeySequence()
{
    const QString KUnknownSequence("123");
    bool handled = m_handler->executeKeySequence(KUnknownSequence);
    QCOMPARE(handled, false);
}

QTEST_MAIN_S60(UT_ImeiKeySequenceHandler)
