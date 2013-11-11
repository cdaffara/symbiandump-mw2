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
#include <QTranslator>
#include <smcmockclassincludes.h>
#include "ut_keysequencerecognitionprovider.h"
#include "keysequencerecognitionprovider.h"
#include "keysequencehandler.h"
#include "qtestmains60.h"

void QCoreApplication::installTranslator(QTranslator * messageFile)
{
    SMC_MOCK_METHOD1( void, QTranslator *, messageFile)
}


UT_KeySequenceRecognitionProvider::UT_KeySequenceRecognitionProvider()
    :
    m_provider(0)
{
    
}


UT_KeySequenceRecognitionProvider::~UT_KeySequenceRecognitionProvider()
{
    delete m_provider;
}


void UT_KeySequenceRecognitionProvider::init()
{
    initialize();
    QT_TRAP_THROWING(SmcDefaultValue<QString>::SetL(QString("")))
    
    m_provider = new KeySequenceRecognitionProvider();
}


void UT_KeySequenceRecognitionProvider::cleanup()
{
    reset();
    SmcDefaultValue<QString>::Reset();
    
    delete m_provider;
    m_provider = 0;
}


void UT_KeySequenceRecognitionProvider::t_setupLocalization()
{
    // localization setup ok
    EXPECT(QTranslator, load).returns(true);
    EXPECT(QCoreApplication, installTranslator).times(1);
    
    QScopedPointer<KeySequenceRecognitionProvider> provider1(
        new KeySequenceRecognitionProvider());
    
    QVERIFY(verify());
    
    // localization setup fails
    EXPECT(QTranslator, load).returns(false);
    EXPECT(QCoreApplication, installTranslator).times(0);
    
    QScopedPointer<KeySequenceRecognitionProvider> provider2(
        new KeySequenceRecognitionProvider());
    
    QVERIFY(verify());
}


void UT_KeySequenceRecognitionProvider::t_keySequenceValidator()
{
    QString KExpectedValidator = "1|2|3||";
    EXPECT(KeySequenceHandler, keySequenceValidator)
        .returns(QString("1"));
    EXPECT(KeySequenceHandler, keySequenceValidator)
        .returns(QString("2"));
    EXPECT(KeySequenceHandler, keySequenceValidator)
        .returns(QString("3"));
    
    QString actualValidator = m_provider->keySequenceValidator();
    
    QCOMPARE(actualValidator, KExpectedValidator);
    QVERIFY(verify());
    
    // second query should not require validator string construction again
    EXPECT(KeySequenceHandler, keySequenceValidator).times(0);
    actualValidator = m_provider->keySequenceValidator();
    QCOMPARE(actualValidator, KExpectedValidator);
    QVERIFY(verify());
}


void UT_KeySequenceRecognitionProvider::t_keySequenceValidatorEmptyString()
{
    QString KExpectedValidator = "";
    EXPECT(KeySequenceHandler, keySequenceValidator)
        .returns(QString(""));
    EXPECT(KeySequenceHandler, keySequenceValidator)
        .returns(QString(""));
    EXPECT(KeySequenceHandler, keySequenceValidator)
        .returns(QString(""));
    
    QString actualValidator = m_provider->keySequenceValidator();
    
    QCOMPARE(actualValidator, KExpectedValidator);
    QVERIFY(verify());
}


void UT_KeySequenceRecognitionProvider::t_executeValidKeySequence()
{
    QT_TRAP_THROWING(SmcDefaultValue<bool>::SetL(true))
    
    bool handled = m_provider->executeKeySequence("123");
    
    SmcDefaultValue<bool>::Reset();
    QCOMPARE(handled, true);
    QVERIFY(verify());
}


void UT_KeySequenceRecognitionProvider::t_executeInvalidKeySequence()
{
    bool handled = m_provider->executeKeySequence("123");
    
    QCOMPARE(handled, false);
}

QTEST_MAIN_S60(UT_KeySequenceRecognitionProvider)
