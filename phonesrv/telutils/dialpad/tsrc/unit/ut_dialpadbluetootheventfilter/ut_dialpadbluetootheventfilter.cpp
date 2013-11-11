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

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbview.h>
#include <hblineedit.h>
#include <hbinstance.h>
#include <btxqserviceapi.h>

#ifdef Q_OS_SYMBIAN
#include "xqappmgr.h"
#endif

#include "dialpadtest.h"
#include "dialpadtestutil.h"
#include "dialpadbluetootheventfilter.h"
#include "dialpad.h"

const int WAIT_TIME = 300;
QString mService;
QString mInterface;
QString mOperation;
bool mEmbedded;
bool mSendCalled;

#ifdef Q_OS_SYMBIAN
XQAiwRequest* XQApplicationManager::create(const QString& service, const QString& interface, const QString& operation, bool embedded)
{
    mService = service;
    mInterface = interface;
    mOperation = operation;
    mEmbedded = embedded;
    return new XQAiwRequest(); 
}
void XQAiwRequest::send() { mSendCalled = true; }
#endif

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
class ut_DialpadBluetoothEventFilter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();
    void testLongPressAsteriskKey();
    void testShortAndLongPressAsteriskKey();

private:
    HbMainWindow*  mMainWindow;
    Dialpad*       mDialpad;
    DialpadBluetoothEventFilter *mEventFilter;
    KeyEventCatcher* mKeyCatcher;
    DialpadTestUtil* mUtil;
};

void ut_DialpadBluetoothEventFilter::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mKeyCatcher = new KeyEventCatcher;
    mMainWindow->installEventFilter(mKeyCatcher);

    mUtil = new DialpadTestUtil(*mMainWindow);

    mDialpad = new Dialpad();
    mEventFilter = new DialpadBluetoothEventFilter(mDialpad, this);
    hbInstance->allMainWindows().at(0)->installEventFilter(mEventFilter);
            
    QRectF rect(mMainWindow->contentsRect());
    rect.setHeight(rect.height()*0.7);
    rect.moveTop((mMainWindow->contentsRect().height()-rect.height())/2);

    mDialpad->setPreferredSize(360,400);
    mDialpad->setPos(0,100);

    mMainWindow->show();
    mDialpad->show();
    mDialpad->hide();
}

void ut_DialpadBluetoothEventFilter::init()
{
    mService = QString("");
    mInterface = QString("");
    mOperation = QString("");
    mEmbedded = false;
    mSendCalled = false;
}

void ut_DialpadBluetoothEventFilter::cleanupTestCase()
{
    delete mDialpad;
    delete mMainWindow;
    delete mKeyCatcher;
    delete mUtil;
}

void ut_DialpadBluetoothEventFilter::cleanup()
{
    mKeyCatcher->mKeyPresses.clear();
    mKeyCatcher->mKeyReleases.clear();
    mDialpad->editor().setText(QString());
    QTest::qWait(WAIT_TIME); // delay between tests
}

void ut_DialpadBluetoothEventFilter::testLongPressAsteriskKey()
{
    mDialpad->openDialpad();
    QTest::qWait(2*WAIT_TIME);

    // Basic long press
    mUtil->mouseClickDialpad(Qt::Key_Asterisk, DialpadTestUtil::Press);
    QTest::qWait(2000);
    mUtil->mouseClickDialpad(Qt::Key_Asterisk, DialpadTestUtil::Release);
    QTest::qWait(1000);
    QCOMPARE(mDialpad->editor().text(), QString(""));
    mDialpad->closeDialpad();
    
    QCOMPARE(mService, BluetoothServiceName);
    QCOMPARE(mInterface, BluetoothInterfaceTogglePower);
    QCOMPARE(mOperation, BluetoothTogglePower);
    QVERIFY(mEmbedded == false);
    QVERIFY(mSendCalled == true);
}

void ut_DialpadBluetoothEventFilter::testShortAndLongPressAsteriskKey()
{
    mDialpad->openDialpad();

    // Short press and long press shouldn't do anything
    mUtil->mouseClickDialpad(Qt::Key_Asterisk, DialpadTestUtil::Press);
    mUtil->mouseClickDialpad(Qt::Key_Asterisk, DialpadTestUtil::Release);
    mUtil->mouseClickDialpad(Qt::Key_Asterisk, DialpadTestUtil::Press);
    QTest::qWait(2000);
    mUtil->mouseClickDialpad(Qt::Key_Asterisk, DialpadTestUtil::Release);
    QCOMPARE(mDialpad->editor().text(), QString("**"));
    mDialpad->closeDialpad();	
}

DIALPAD_TEST_MAIN(ut_DialpadBluetoothEventFilter)
#include "ut_dialpadbluetootheventfilter.moc"

