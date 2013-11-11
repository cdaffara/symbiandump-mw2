/*
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
* Description: Unit test for satappmainhandler
*
*/

#include <QTimer>
#include <QAction>

#include <hbinputdialog.h>
#include <hbmessagebox.h>

#include "ut_satappinputprovider.h"
#include "satappinputprovider.h"
#include "satappaction.h"


// --------------------------------------------------------
// The unit tests begin here
// --------------------------------------------------------

Ut_SatAppInputProvider::Ut_SatAppInputProvider(
        QObject *parent)
    :QObject(parent)
{
}

void Ut_SatAppInputProvider::testResetState()
{
    InputProviderTest test;
    test.mInput->resetState();
}

void Ut_SatAppInputProvider::testGetInkeyAndOk_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("charset");
    QTest::addColumn<int>("inkey");

    QTest::newRow("t01") << "title" << (int)ESatDigitOnly << 0;
    QTest::newRow("t02") << "title" << (int)ESatCharSmsDefaultAlphabet << 0;
    QTest::newRow("t03") << "title" << (int)ESatCharUcs2Alphabet << 0;
    QTest::newRow("t04") << "title" << (int)ESatYesNo << 0;
    QTest::newRow("t05") << "long title that does not fix to screen" << (int)ESatDigitOnly << 0;
    QTest::newRow("t06") << "long title that does not fix to screen" << (int)ESatCharSmsDefaultAlphabet << 0;
    QTest::newRow("t07") << "long title that does not fix to screen" << (int)ESatCharUcs2Alphabet << 0;
    QTest::newRow("t08") << "long title that does not fix to screen" << (int)ESatYesNo << 0;
    QTest::newRow("t09") << "" << (int)ESatDigitOnly << 1;
    QTest::newRow("t10") << "" << (int)ESatCharSmsDefaultAlphabet << 47;
    QTest::newRow("t11") << "" << (int)ESatCharUcs2Alphabet << 58;
    QTest::newRow("t12") << "" << (int)ESatYesNo << 0;
}

void Ut_SatAppInputProvider::testGetInkeyAndOk()
{
    QFETCH(QString, text);
    QFETCH(int, charset);
    QFETCH(int, inkey);
    bool selfExp = false;

    SatAppAction a(ESatGetInkeyAction);
    a.set(KeyText,text);
    a.set(KeyCharacterSet,charset);
    a.set(KeyInKey,inkey);
    a.set(KeyImmediate,false);
    a.set(KeyDuration,10000);
    
    InputProviderTest test;
    test.mInput->getInkey(a);
    // simulate press OK
    if( (charset!=ESatYesNo) || (text.length()>0)){
    test.simulateFirstActionClicked();
    }

    if (charset==ESatYesNo) {
        if( text.length()>0 ){
            QCOMPARE(a.response(),ESatSuccess);
            QCOMPARE(a.value(KeyInKey).toInt(),1);
        }
        else {
            QCOMPARE(a.response(),ESatFailure);
        }
            
    }
    else {
        QCOMPARE(a.response(),ESatSuccess);
    }
    
}

void Ut_SatAppInputProvider::testGetInkeyAndCancel_data()
{
    // uses same test materil as testGetInkeyAndOk()
    testGetInkeyAndOk_data();
}

void Ut_SatAppInputProvider::testGetInkeyAndCancel()
{
    QFETCH(QString, text);
    QFETCH(int, charset);
    QFETCH(int, inkey);
    bool selfExp = false;

    SatAppAction a(ESatGetInkeyAction);
    a.set(KeyText,text);
    a.set(KeyCharacterSet,charset);
    a.set(KeyInKey,inkey);
    a.set(KeyImmediate,false);
    a.set(KeyDuration,10000);
    
    InputProviderTest test;
    test.mInput->getInkey(a);
    // simulate press CANCEL
    if((charset!=ESatYesNo) || (text.length()>0)) {
        test.simulateSecondActionClicked();
    }

    if (charset==ESatYesNo) {
        if( text.length()>0 ){
            QCOMPARE(a.response(),ESatSuccess);
            QCOMPARE(a.value(KeyInKey).toInt(),0);
        }
        else {
            QCOMPARE(a.response(),ESatFailure);
        }    }
    else {
        QCOMPARE(a.response(),ESatBackwardModeRequestedByUser);
    }
}

void Ut_SatAppInputProvider::testGetInkeyAndTimeout()
{
    SatAppAction a(ESatGetInkeyAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatCharSmsDefaultAlphabet);
    a.set(KeyInKey,64);
    a.set(KeyImmediate,false);
    a.set(KeyDuration,1000);
    
    InputProviderTest test;
    test.mInput->getInkey(a);
    a.waitUntilCompleted();
    
    QCOMPARE(a.response(),ESatNoResponseFromUser);
}

void Ut_SatAppInputProvider::testGetInkeyAndReset()
{
    SatAppAction a(ESatGetInkeyAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatCharSmsDefaultAlphabet);
    a.set(KeyInKey,64);
    a.set(KeyImmediate,false);
    a.set(KeyDuration,10000);
    a.setResponse(ESatFailure);

    InputProviderTest test;
    test.mInput->getInkey(a);
    test.mInput->resetState();

    QCOMPARE(a.response(),ESatNoResponseFromUser);
}

void Ut_SatAppInputProvider::testGetInkeyAndDelete()
{
    SatAppAction a(ESatGetInkeyAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatCharSmsDefaultAlphabet);
    a.set(KeyInKey,64);
    a.set(KeyImmediate,false);
    a.set(KeyDuration,10000);
    a.setResponse(ESatFailure);

    {
    InputProviderTest test;
    test.mInput->getInkey(a);
    }
    // at this point everything should be cleaned up safely, and action
    // should be completed later when application exit or timeout
    
    QCOMPARE(a.response(),ESatFailure);
}

void Ut_SatAppInputProvider::testGetInkeyImmediateAndOk()
{
    SatAppAction a(ESatGetInkeyAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatDigitOnly);
    a.set(KeyInKey,64);
    a.set(KeyImmediate,true);
    a.set(KeyDuration,10000);
    a.setResponse(ESatFailure);

    InputProviderTest test;
    test.mInput->getInkey(a);
    test.simulateImmediateKeypress("0");
    
    QCOMPARE(a.response(),ESatSuccess);
    QCOMPARE(a.value(KeyInKey).toInt(),0x30);
}

void Ut_SatAppInputProvider::testGetInkeyImmediateAndCancel()
{
    SatAppAction a(ESatGetInkeyAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatDigitOnly);
    a.set(KeyInKey,64);
    a.set(KeyImmediate,true);
    a.set(KeyDuration,10000);
    a.setResponse(ESatFailure);

    InputProviderTest test;
    test.mInput->getInkey(a);
    test.simulateFirstActionClicked(); // cancel
    
    QCOMPARE(a.response(),ESatBackwardModeRequestedByUser);
}

void Ut_SatAppInputProvider::testGetInputAndOk_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("charset");
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("min");
    QTest::addColumn<int>("max");
    QTest::addColumn<bool>("hide");

    QTest::newRow("t01") << "title" << (int)ESatDigitOnly << "" << 1 << 5 << false;
    QTest::newRow("t02") << "title" << (int)ESatCharSmsDefaultAlphabet << "" << 1 << 5 << false;
    QTest::newRow("t03") << "title" << (int)ESatCharUcs2Alphabet << "" << 1 << 5 << false;
    QTest::newRow("t04") << "very long title that does not fit into screen" << (int)ESatDigitOnly << "" << 1 << 5 << false;
    QTest::newRow("t05") << "very long title that does not fit into screen" << (int)ESatCharSmsDefaultAlphabet << "" << 2 << 8 << false;
    QTest::newRow("t06") << "very long title that does not fit into screen" << (int)ESatCharUcs2Alphabet << "" << 2 << 8 << false;
    QTest::newRow("t07") << "title" << (int)ESatDigitOnly << "12345" << 1 << 5 << false;
    QTest::newRow("t08") << "title" << (int)ESatCharSmsDefaultAlphabet << "12345" << 1 << 5 << false;
    QTest::newRow("t09") << "title" << (int)ESatCharUcs2Alphabet << "12345" << 1 << 5 << false;
    QTest::newRow("t10") << "title" << (int)ESatDigitOnly << "12345" << 1 << 5 << true;
    QTest::newRow("t11") << "title" << (int)ESatCharSmsDefaultAlphabet << "12345" << 1 << 5 << true;
    QTest::newRow("t12") << "title" << (int)ESatCharUcs2Alphabet << "12345" << 1 << 5 << true;
    QTest::newRow("t13") << "" << (int)ESatDigitOnly << "" << 1 << 5 << false;
    QTest::newRow("t14") << "" << (int)ESatCharSmsDefaultAlphabet << "" << 1 << 5 << false;
    QTest::newRow("t15") << "" << (int)ESatCharUcs2Alphabet << "" << 1 << 5 << false;
    QTest::newRow("t16") << "" << (int)ESatDigitOnly << "123456789123456789" << 1 << 5 << false;
    QTest::newRow("t17") << "" << (int)ESatCharSmsDefaultAlphabet << "123456789123456789" << 1 << 5 << false;
    QTest::newRow("t18") << "" << (int)ESatCharUcs2Alphabet << "123456789123456789" << 1 << 5 << false;
    
}

void Ut_SatAppInputProvider::testGetInputAndOk()
{
    QFETCH(QString, text);
    QFETCH(int, charset);
    QFETCH(QString, input);
    QFETCH(int, min);
    QFETCH(int, max);
    QFETCH(bool, hide);

    SatAppAction a(ESatGetInputAction);
    a.set(KeyText,text);
    a.set(KeyCharacterSet,charset);
    a.set(KeyInputString,input);
    a.set(KeyMinLength,min);
    a.set(KeyMaxLength,max);
    a.set(KeyHideInput,hide);
    a.set(KeyDuration,10000);

    InputProviderTest test;
    test.mInput->getInput(a);
    test.simulateFirstActionClicked();

    QCOMPARE(a.response(),ESatSuccess);
}

void Ut_SatAppInputProvider::testGetInputAndCancel_data()
{
    // uses same test data as testGetInputAndOk
    testGetInputAndOk_data();
}

void Ut_SatAppInputProvider::testGetInputAndCancel()
{
    QFETCH(QString, text);
    QFETCH(int, charset);
    QFETCH(QString, input);
    QFETCH(int, min);
    QFETCH(int, max);
    QFETCH(bool, hide);

    SatAppAction a(ESatGetInputAction);
    a.set(KeyText,text);
    a.set(KeyCharacterSet,charset);
    a.set(KeyInputString,input);
    a.set(KeyMinLength,min);
    a.set(KeyMaxLength,max);
    a.set(KeyHideInput,hide);
    a.set(KeyDuration,10000);

    InputProviderTest test;
    test.mInput->getInput(a);
    test.simulateSecondActionClicked();

    QCOMPARE(a.response(),ESatBackwardModeRequestedByUser);
}

void Ut_SatAppInputProvider::testGetInputAndTimeout()
{
    SatAppAction a(ESatGetInputAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatCharSmsDefaultAlphabet);
    a.set(KeyInputString,"12345");
    a.set(KeyMinLength,1);
    a.set(KeyMaxLength,10);
    a.set(KeyHideInput,false);
    a.set(KeyDuration,1000);

    InputProviderTest test;
    test.mInput->getInput(a);
    a.waitUntilCompleted();

    QCOMPARE(a.response(),ESatNoResponseFromUser);
}

void Ut_SatAppInputProvider::testGetInputAndReset()
{
    SatAppAction a(ESatGetInputAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatCharSmsDefaultAlphabet);
    a.set(KeyInputString,"12345");
    a.set(KeyMinLength,1);
    a.set(KeyMaxLength,10);
    a.set(KeyHideInput,false);
    a.set(KeyDuration,10000);

    InputProviderTest test;
    test.mInput->getInput(a);
    test.mInput->resetState();

    QCOMPARE(a.response(),ESatNoResponseFromUser);
}

void Ut_SatAppInputProvider::testGetInputAndDelete()
{
    SatAppAction a(ESatGetInputAction);
    a.set(KeyText,"hello");
    a.set(KeyCharacterSet,ESatCharSmsDefaultAlphabet);
    a.set(KeyInputString,"12345");
    a.set(KeyMinLength,1);
    a.set(KeyMaxLength,10);
    a.set(KeyHideInput,false);
    a.set(KeyDuration,10000);
    a.setResponse(ESatFailure);

    {
    InputProviderTest test;
    test.mInput->getInput(a);
    }
    // at this point input dialog out of scope and destroyed.
    // pending action should have been completed later when application exit or timeout

    QCOMPARE(a.response(),ESatFailure);
}


// ---------------------------------------------------------------
// internal helper class
// ---------------------------------------------------------------

InputProviderTest::InputProviderTest()
{
    mInput = new SatAppInputProvider(this);
}

InputProviderTest::~InputProviderTest()
{
    cleanup();
}

void InputProviderTest::simulateFirstActionClicked()
{
    mInput->mCurrentDialog->actions().at(0)->trigger();
}

void InputProviderTest::simulateSecondActionClicked()
{
    mInput->mCurrentDialog->actions().at(1)->trigger();
}

void InputProviderTest::simulateImmediateKeypress(QString s)
{
    mInput->transmitInkeyImmediately(s);
}

void InputProviderTest::cleanup()
{
    delete mInput;
    mInput = 0;
}


// End of file
