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

#ifdef Q_OS_SYMBIAN
#include "xqservicerequest.h"
#endif

#include "dialpadtest.h"
#include "dialpadtestutil.h"
#include "dialpadvideomailboxeventfilter.h"
#include "dialpad.h"
#include "dialpadsymbianwrapper.h"

const int WAIT_TIME = 300;
QString mService;
QString mMessage;
bool mXQServiceConstructed;
bool mSendCalled;

DialpadSymbianWrapper::DialpadSymbianWrapper(QObject */*parent*/) : d_ptr(NULL) {}
DialpadSymbianWrapper::~DialpadSymbianWrapper() {}
int DialpadSymbianWrapper::getVideoMailboxNumber(QString &vmbxNumber) { vmbxNumber=QString("12345678"); return 0; }
int DialpadSymbianWrapper::defineVideoMailboxNumber(QString &/*vmbxNumber*/) { return 0; }

#ifdef Q_OS_SYMBIAN
XQServiceRequest::XQServiceRequest(const QString& service, const QString& message, const bool& /*synchronous*/) { mService=service; mMessage=message; mXQServiceConstructed=true; }
XQServiceRequest::~XQServiceRequest() {}
bool XQServiceRequest::send(QVariant& /*retValue*/) { mSendCalled=true; return true; }
void XQServiceRequest::addArg(const QVariant& /*v*/) {}
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
class ut_DialpadVideoMailboxEventFilter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

    // These are actual voice mailbox event filter unit tests
    void testNumericKeyTwoLongPress();
    void testNumericKeyTwoShortPress();
    void testNumericKeyTwoShortThenLongPress();

private:
    HbMainWindow*  mMainWindow;
    Dialpad*       mDialpad;
    DialpadVideoMailboxEventFilter *mEventFilter;
    KeyEventCatcher* mKeyCatcher;
    DialpadTestUtil* mUtil;
};

void ut_DialpadVideoMailboxEventFilter::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mKeyCatcher = new KeyEventCatcher;
    mMainWindow->installEventFilter(mKeyCatcher);

    mUtil = new DialpadTestUtil(*mMainWindow);

    mDialpad = new Dialpad();
    mEventFilter = new DialpadVideoMailboxEventFilter(mDialpad, this);
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

void ut_DialpadVideoMailboxEventFilter::init()
{
    mService = QString("");
    mMessage = QString("");
    mXQServiceConstructed = false;
    mSendCalled = false;
}

void ut_DialpadVideoMailboxEventFilter::cleanupTestCase()
{
    delete mDialpad;
    delete mMainWindow;
    delete mKeyCatcher;
    delete mUtil;
}

void ut_DialpadVideoMailboxEventFilter::cleanup()
{
    mKeyCatcher->mKeyPresses.clear();
    mKeyCatcher->mKeyReleases.clear();
    mDialpad->editor().setText(QString());
    QTest::qWait( WAIT_TIME ); // delay between tests
}

void ut_DialpadVideoMailboxEventFilter::testNumericKeyTwoLongPress()
{
    mDialpad->openDialpad();
    QTest::qWait(WAIT_TIME);
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Press);
    QTest::qWait(2000);
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Release);
    QTest::qWait(1000);
    QCOMPARE(mDialpad->editor().text(), QString(""));
    mDialpad->closeDialpad();

#ifdef Q_OS_SYMBIAN
    QVERIFY(mXQServiceConstructed == true);
    QVERIFY(mSendCalled == true);
    QCOMPARE(mService, QString("com.nokia.symbian.ICallDial"));
    QCOMPARE(mMessage, QString("dialVideo(QString)"));
#endif
}

void ut_DialpadVideoMailboxEventFilter::testNumericKeyTwoShortPress()
{
    mDialpad->openDialpad();
    QTest::qWait(WAIT_TIME);
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Press);
    QTest::qWait(200);
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Release);
    QTest::qWait(1000);
    // Check that character '1' is in editor.
    QCOMPARE(mDialpad->editor().text(), QString("2"));
}

void ut_DialpadVideoMailboxEventFilter::testNumericKeyTwoShortThenLongPress()
{
    // Then one short and one long press
    mDialpad->openDialpad();
    QTest::qWait( WAIT_TIME );
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Press);
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Release);
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Press);
    QTest::qWait(2000);
    mUtil->mouseClickDialpad(Qt::Key_2, DialpadTestUtil::Release);
    QTest::qWait(1000);
    QVERIFY(mDialpad->editor().text()=="22");
    mDialpad->closeDialpad();
}

DIALPAD_TEST_MAIN(ut_DialpadVideoMailboxEventFilter)
#include "ut_dialpadvideomailboxeventfilter.moc"
