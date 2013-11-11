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
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbview.h>
#include <hblineedit.h>
#include <hbmessagebox.h>

#include "dialpadtest.h"
#include "dialpadtestutil.h"
#include "dialpad.h"

const int WAIT_TIME = 300;
const int LONGPRESS_DURATION = 1000;

// helper class
class KeyEventCatcher : public QObject
{
public:
    bool eventFilter(QObject* watched, QEvent * event)
    {
        Q_UNUSED(watched);

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            mKeyPresses.append(keyEvent->key());
            return false;
        } else if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            mKeyReleases.append(keyEvent->key());
            return false;
        }
        return false;
    }

public:
    QList<int> mKeyPresses;
    QList<int> mKeyReleases;
};

// test cases
class mt_Dialpad : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    void testNumericButtons();
    void testNumericButtonLongPress();
    void testBackspace();
    void testAsteriskMultitap();
    void testCallButton();
    void testCloseGesture();
    void testOpenDialogWhileButtonPressed();
    void testTapOutsideDismiss();

#ifndef Q_OS_SYMBIAN
    void testCloseGestureLandscape();
    void testOrientationChange();
#endif

private:
    HbMainWindow*  mMainWindow;
    Dialpad*       mDialpad;
    KeyEventCatcher* mKeyCatcher;
    DialpadTestUtil* mUtil;
};

void mt_Dialpad::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mKeyCatcher = new KeyEventCatcher;
    mMainWindow->installEventFilter(mKeyCatcher);

    mUtil = new DialpadTestUtil(*mMainWindow);

    mDialpad = new Dialpad(*mMainWindow);

    QRectF rect(mMainWindow->contentsRect());
    rect.setHeight(rect.height()*0.7);
    rect.moveTop((mMainWindow->contentsRect().height()-rect.height())/2);

    mDialpad->resize(mMainWindow->layoutRect().width(),
                               mMainWindow->layoutRect().height()/2);
    mDialpad->setPos(0,mMainWindow->layoutRect().height()/4);

    mMainWindow->show();
    mDialpad->show();
    mDialpad->hide();
}

void mt_Dialpad::cleanupTestCase()
{
    delete mDialpad;
    delete mMainWindow;
    delete mKeyCatcher;
    delete mUtil;
}

void mt_Dialpad::cleanup()
{
    mKeyCatcher->mKeyPresses.clear();
    mKeyCatcher->mKeyReleases.clear();
    mDialpad->editor().setText(QString());
    QTest::qWait( WAIT_TIME ); // delay between tests
}

void mt_Dialpad::testNumericButtons()
{
    QSignalSpy aboutToOpenSpy( mDialpad, SIGNAL(aboutToOpen()));
    QSignalSpy aboutToCloseSpy( mDialpad, SIGNAL(aboutToClose()));

    QVERIFY(mDialpad->isOpen()==false);
    mDialpad->openDialpad();
    QVERIFY(mDialpad->isOpen()==true);
    QVERIFY(aboutToOpenSpy.count()==1);
    QTest::qWait( 2*WAIT_TIME );

    mUtil->mouseClickDialpad(Qt::Key_1);
    mUtil->mouseClickDialpad(Qt::Key_2);
    mUtil->mouseClickDialpad(Qt::Key_3);
    mUtil->mouseClickDialpad(Qt::Key_4);
    mUtil->mouseClickDialpad(Qt::Key_5);
    mUtil->mouseClickDialpad(Qt::Key_6);
    mUtil->mouseClickDialpad(Qt::Key_7);
    mUtil->mouseClickDialpad(Qt::Key_8);
    mUtil->mouseClickDialpad(Qt::Key_9);
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    mUtil->mouseClickDialpad(Qt::Key_0);
    mUtil->mouseClickDialpad(Qt::Key_NumberSign);

    QVERIFY(mDialpad->editor().text()=="123456789*0#");

    QVERIFY(mKeyCatcher->mKeyPresses.count()==12);
    QVERIFY(mKeyCatcher->mKeyReleases.count()==12);
    QVERIFY(mKeyCatcher->mKeyPresses.at(0)==Qt::Key_1);
    QVERIFY(mKeyCatcher->mKeyReleases.at(0)==Qt::Key_1);
    QVERIFY(mKeyCatcher->mKeyPresses.at(11)==Qt::Key_NumberSign);
    QVERIFY(mKeyCatcher->mKeyReleases.at(11)==Qt::Key_NumberSign);
    mDialpad->closeDialpad();
    QVERIFY(mDialpad->isOpen()==false);
    QVERIFY(aboutToCloseSpy.count()==1);
}

void mt_Dialpad::testNumericButtonLongPress()
{
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_1, DialpadTestUtil::Press);
    QTest::qWait( LONGPRESS_DURATION );
    mUtil->mouseClickDialpad(Qt::Key_1, DialpadTestUtil::Release);
    QVERIFY(mDialpad->editor().text()=="1");
    mDialpad->closeDialpad();
}

void mt_Dialpad::testBackspace()
{
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_1);
    QVERIFY(mDialpad->editor().text()=="1");
    mUtil->mouseClickDialpad(Qt::Key_Backspace);
    QVERIFY(mDialpad->editor().text()=="");
    QVERIFY(mKeyCatcher->mKeyPresses.count()==2);
    QVERIFY(mKeyCatcher->mKeyReleases.count()==2);
    QVERIFY(mKeyCatcher->mKeyPresses.at(1)==Qt::Key_Backspace);
    QVERIFY(mKeyCatcher->mKeyReleases.at(1)==Qt::Key_Backspace);
    // test repeats
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_1);
    mUtil->mouseClickDialpad(Qt::Key_2);
    mUtil->mouseClickDialpad(Qt::Key_3);
    QVERIFY(mDialpad->editor().text()=="123");
    mUtil->mouseClickDialpad(Qt::Key_Backspace, DialpadTestUtil::Press);
    QTest::qWait( 1500 );
    mUtil->mouseClickDialpad(Qt::Key_Backspace, DialpadTestUtil::Release);
    QVERIFY(mDialpad->editor().text()=="");
    mDialpad->closeDialpad();
}

void mt_Dialpad::testAsteriskMultitap()
{
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QVERIFY(mDialpad->editor().text()=="*");
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QVERIFY(mDialpad->editor().text()=="+");
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QVERIFY(mDialpad->editor().text()=="p");
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QVERIFY(mDialpad->editor().text()=="w");

    QVERIFY(mKeyCatcher->mKeyPresses.count()==4);
    QVERIFY(mKeyCatcher->mKeyReleases.count()==4);
    QVERIFY(mKeyCatcher->mKeyPresses.at(1)==Qt::Key_Asterisk);
    QVERIFY(mKeyCatcher->mKeyReleases.at(1)==Qt::Key_Asterisk);
    QVERIFY(mKeyCatcher->mKeyPresses.at(2)==Qt::Key_Asterisk);
    QVERIFY(mKeyCatcher->mKeyReleases.at(2)==Qt::Key_Asterisk);

    // Verify that the second char can't be +
    QTest::qWait(1000);
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QCOMPARE(mDialpad->editor().text(), QString("w*"));
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QCOMPARE(mDialpad->editor().text(), QString("wp"));
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QCOMPARE(mDialpad->editor().text(), QString("ww"));
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QCOMPARE(mDialpad->editor().text(), QString("w*"));

    // test entering two asterisk
    QTest::qWait(1000);
    mDialpad->editor().setText(QString());
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QTest::qWait( 1000 ); // longer than short tap
    mUtil->mouseClickDialpad(Qt::Key_Asterisk);
    QCOMPARE(mDialpad->editor().text(), QString("**"));

    mDialpad->closeDialpad();
}

void mt_Dialpad::testCallButton()
{
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    mDialpad->setCallButtonEnabled(false);
    mUtil->mouseClickDialpad(Qt::Key_Yes);
    QVERIFY(mKeyCatcher->mKeyPresses.count()==0);
    QVERIFY(mKeyCatcher->mKeyReleases.count()==0);
    mDialpad->setCallButtonEnabled(true);
    mUtil->mouseClickDialpad(Qt::Key_Yes);
    QTest::qWait( WAIT_TIME );
    QVERIFY(mKeyCatcher->mKeyPresses.count()==1);
    QVERIFY(mKeyCatcher->mKeyReleases.count()==1);
    QVERIFY(mKeyCatcher->mKeyPresses.at(0)==Qt::Key_Yes);
    QVERIFY(mKeyCatcher->mKeyReleases.at(0)==Qt::Key_Yes);
    mDialpad->closeDialpad();
}

void mt_Dialpad::testCloseGesture()
{
    // QTest::mouseMove() doesn't work
}

void mt_Dialpad::testOpenDialogWhileButtonPressed()
{
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_5, DialpadTestUtil::Press);
    QTest::qWait( LONGPRESS_DURATION );

    HbMessageBox* box = new HbMessageBox("Test dialog!");
    box->setAttribute(Qt::WA_DeleteOnClose);
    box->setTimeout(1000);
    box->show();
    QTest::qWait( 500 );
    mUtil->mouseClickDialpad(Qt::Key_5, DialpadTestUtil::Release);
    QTest::qWait( 100 );

    QVERIFY(mDialpad->editor().text()=="5");
    QVERIFY(mKeyCatcher->mKeyPresses.at(0)==Qt::Key_5);
    QVERIFY(mKeyCatcher->mKeyReleases.at(0)==Qt::Key_5);
    mDialpad->closeDialpad();
}

void mt_Dialpad::testTapOutsideDismiss()
{
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );

    QPointF aboveTop = mDialpad->scenePos();
    aboveTop.setY(aboveTop.y()-10);
    aboveTop.setX(aboveTop.x()+mDialpad->rect().center().x());
    QPoint windowPos = mMainWindow->mapFromScene( aboveTop );

    // test tap outside dismiss is OFF (default)
    QTest::mouseClick( mMainWindow->viewport(), Qt::LeftButton,
                       0, windowPos );
    QTest::qWait( 500 );
    QVERIFY(mDialpad->isVisible());
    mDialpad->closeDialpad();

    // test tap outside dismiss is ON    
    mDialpad->setTapOutsideDismiss(true);
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    QTest::mouseClick( mMainWindow->viewport(), Qt::LeftButton,
                       0, windowPos );
    QTest::qWait( 500 );
    QVERIFY(!mDialpad->isVisible());
}

#ifndef Q_OS_SYMBIAN
void mt_Dialpad::testCloseGestureLandscape()
{
    /*
    // switch to landscape
    mMainWindow->setOrientation(Qt::Horizontal);
    mMainWindow->resize(640,360);
    QTest::qWait( 1000 );

    mDialpad->resize(mMainWindow->layoutRect().width()/2,
                               mMainWindow->layoutRect().height());
    mDialpad->setPos(mMainWindow->layoutRect().width()/2,0);

    mDialpad->openDialpad();

    // slow swipe
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_4, Press, false);
    QTest::qWait( 500 ); // longer than swipe
    mUtil->mouseClickDialpad(Qt::Key_6, Release, false);
    QTest::qWait( 500 );
    QVERIFY(mDialpad->isVisible()==true);

    // short swipe
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_4, Press, false);
    QTest::qWait( 200 );
    mUtil->mouseClickDialpad(Qt::Key_5, Release, false);
    QTest::qWait( 500 );
    QVERIFY(mDialpad->isVisible()==true);

    // ok swipe
    mDialpad->editor().setText(QString());
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_4, Press, false);
    QTest::qWait( 200 );
    mUtil->mouseClickDialpad(Qt::Key_6, Release, false);
    QVERIFY(mDialpad->editor().text()=="");
    QTest::qWait( 1000 );
    QVERIFY(mDialpad->isVisible()==false);*/
}

void mt_Dialpad::testOrientationChange()
{
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );

    // test landscape
    mMainWindow->setOrientation(Qt::Horizontal);
    mMainWindow->resize(640,360);
    QTest::qWait( 1000 );
    mDialpad->resize(mMainWindow->layoutRect().width()/2,
                               mMainWindow->layoutRect().height());
    mDialpad->setPos(mMainWindow->layoutRect().width()/2,0);
    QTest::qWait( 1000 );

    QVERIFY(mDialpad->isVisible()==true);
    mUtil->mouseClickDialpad(Qt::Key_5);
    QVERIFY(mDialpad->editor().text()=="5");
    QTest::qWait( 500 );

    // test portrait
    mMainWindow->setOrientation(Qt::Vertical);
    mMainWindow->resize(360,640);
    mDialpad->resize(mMainWindow->layoutRect().width(),
                               mMainWindow->layoutRect().height()/2);
    mDialpad->setPos(0,mMainWindow->layoutRect().height()/4);
    QTest::qWait( 1000 );

    QVERIFY(mDialpad->isVisible()==true);
    mDialpad->editor().setText("");
    mUtil->mouseClickDialpad(Qt::Key_5);
    QVERIFY(mDialpad->editor().text()=="5");

    mDialpad->closeDialpad();
    QTest::qWait( 500 );
    QVERIFY(mDialpad->isVisible()==false);
}

#endif //Q_OS_SYMBIAN

DIALPAD_TEST_MAIN(mt_Dialpad)
#include "mt_dialpad.moc"

