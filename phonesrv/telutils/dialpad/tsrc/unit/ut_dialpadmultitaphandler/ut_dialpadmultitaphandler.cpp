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
* Description:
*
*/

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hblineedit.h>

#include "dialpadmultitaphandler.h"
#include "dialpadtest.h"


class ut_DialpadMultitapHandler : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    void testWatchedNotEditor();
    void testNumericKeyPress();
    void testNonNumericKeyPress();
    void testAsterisk();
    void testAsteriskMultitap();

private:
    QObject*  mHandler;
    HbLineEdit* mEditor;
};

void ut_DialpadMultitapHandler::initTestCase()
{
    mEditor = new HbLineEdit;
    mHandler = new DialpadMultitapHandler(*mEditor);
}

void ut_DialpadMultitapHandler::cleanupTestCase()
{
    delete mHandler;
    delete mEditor;
}

void ut_DialpadMultitapHandler::cleanup()
{
    mEditor->setText(QString());
}

void ut_DialpadMultitapHandler::testWatchedNotEditor()
{
    QKeyEvent asteriskPress(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier);
    QKeyEvent asteriskRelease(QEvent::KeyRelease, Qt::Key_Asterisk, Qt::NoModifier);
    HbLineEdit edit2;

    QVERIFY(mHandler->eventFilter(&edit2,&asteriskPress)==false);
    QVERIFY(mHandler->eventFilter(&edit2,&asteriskRelease)==false);
}

void ut_DialpadMultitapHandler::testNumericKeyPress()
{
    QKeyEvent key1Press(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier);
    QKeyEvent key1Char(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier, "1");
    QKeyEvent key1Release(QEvent::KeyRelease, Qt::Key_1, Qt::NoModifier);

    QVERIFY(mHandler->eventFilter(mEditor,&key1Press)==true);
    QVERIFY(mHandler->eventFilter(mEditor,&key1Char)==false);
    QVERIFY(mHandler->eventFilter(mEditor,&key1Release)==false);
}

void ut_DialpadMultitapHandler::testNonNumericKeyPress()
{
    QKeyEvent keyYesPress(QEvent::KeyPress, Qt::Key_Yes, Qt::NoModifier);
    QKeyEvent keyYesRelease(QEvent::KeyRelease, Qt::Key_Yes, Qt::NoModifier);

    QVERIFY(mHandler->eventFilter(mEditor,&keyYesPress)==false);
    QVERIFY(mHandler->eventFilter(mEditor,&keyYesRelease)==false);

    QKeyEvent keyBackspacePress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    QKeyEvent keyBackspaceRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);

    QVERIFY(mHandler->eventFilter(mEditor,&keyBackspacePress)==false);
    QVERIFY(mHandler->eventFilter(mEditor,&keyBackspaceRelease)==false);
}

void ut_DialpadMultitapHandler::testAsterisk()
{
    QKeyEvent asteriskPress(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier);
    QKeyEvent asteriskChar(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier, "*");
    QKeyEvent asteriskRelease(QEvent::KeyRelease, Qt::Key_Asterisk, Qt::NoModifier);

    QVERIFY(mHandler->eventFilter(mEditor,&asteriskPress)==true);
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskChar)==false);
    QTest::qWait( 1000 );
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskRelease)==false);
}

void ut_DialpadMultitapHandler::testAsteriskMultitap()
{
    QKeyEvent asteriskPress(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier);
    QKeyEvent asteriskChar(QEvent::KeyPress, Qt::Key_Asterisk, Qt::NoModifier, "*");
    QKeyEvent asteriskRelease(QEvent::KeyRelease, Qt::Key_Asterisk, Qt::NoModifier);

    QVERIFY(mHandler->eventFilter(mEditor,&asteriskPress)==true);
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskChar)==false);
    QTest::qWait( 100 );
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskChar)==true); // +
    QTest::qWait( 100 );
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskChar)==true); // p
    QTest::qWait( 100 );
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskChar)==true); // w
    QTest::qWait( 100 );
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskChar)==true); // *
    QVERIFY(mHandler->eventFilter(mEditor,&asteriskRelease)==false);
}

DIALPAD_TEST_MAIN(ut_DialpadMultitapHandler)
#include "ut_dialpadmultitaphandler.moc"

