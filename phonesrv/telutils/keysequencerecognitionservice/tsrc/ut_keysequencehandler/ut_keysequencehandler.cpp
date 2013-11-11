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
#include "ut_keysequencehandler.h"
#include "keysequencehandler.h"
#include "qtestmains60.h"

bool TestClassDriver::executeKeySequence(const QString &keySequence)
{
    Q_UNUSED(keySequence)
    return true;
}

void TestClassDriver::setKeySequenceValidator(const QString &validator)
{
    KeySequenceHandler::setKeySequenceValidator(validator);
}

UT_KeySequenceHandler::UT_KeySequenceHandler()
    :
    m_handler(0)
{
    
}

UT_KeySequenceHandler::~UT_KeySequenceHandler()
{
    
}

void UT_KeySequenceHandler::init()
{
    initialize();
    
    m_handler = new TestClassDriver();
}

void UT_KeySequenceHandler::cleanup()
{
    reset();
    
    delete m_handler;
    m_handler = 0;
}


void UT_KeySequenceHandler::t_keySequenceValidator()
{
    QString validator = m_handler->keySequenceValidator();
    QCOMPARE(validator, QString(""));
}


void UT_KeySequenceHandler::t_setKeySequenceValidator()
{
    const QString KValidatorString = "123";
    m_handler->setKeySequenceValidator(KValidatorString);
    QCOMPARE(m_handler->keySequenceValidator(), KValidatorString);
}

QTEST_MAIN_S60(UT_KeySequenceHandler)
