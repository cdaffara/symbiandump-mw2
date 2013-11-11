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
#include <mockservice.h>

#include <hbapplication.h>
#include <hblineedit.h>
#include <hbstringutil.h>

#include "dialpadtest.h"
#include "dialpad.h"
#include "dialpadsymbianwrapper.h"
#define protected public
#include "dialpadvoicemailboxeventfilter.h"
#include "dialpadmailboxeventfilterbase.h"


void editText(QString &text)
{
    text = QString("blah");
}


// test cases
class ut_DialpadVoiceMailboxEventFilter : public QObject, MockService
{
    Q_OBJECT

public:    
    ut_DialpadVoiceMailboxEventFilter();
    virtual ~ut_DialpadVoiceMailboxEventFilter();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

    // These are actual voice mailbox event filter unit tests
    void testEventFilter();
    void testHandleCallButtonPress();
    void testHandleMailboxOperation();
    void testHandleLongKeyPress();

private:
    DialpadVoiceMailboxEventFilter *mEventFilter;
    Dialpad *mDialpad;
};

ut_DialpadVoiceMailboxEventFilter::ut_DialpadVoiceMailboxEventFilter()
{
}

ut_DialpadVoiceMailboxEventFilter::~ut_DialpadVoiceMailboxEventFilter()
{
}

void ut_DialpadVoiceMailboxEventFilter::initTestCase()
{
}

void ut_DialpadVoiceMailboxEventFilter::init()
{
    initialize();
    HbMainWindow *window = 0;
    mDialpad = new Dialpad(*window);
    mEventFilter = new DialpadVoiceMailboxEventFilter(mDialpad);
}

void ut_DialpadVoiceMailboxEventFilter::cleanupTestCase()
{
}

void ut_DialpadVoiceMailboxEventFilter::cleanup()
{
    delete mEventFilter;
    delete mDialpad;
    reset();
}

void ut_DialpadVoiceMailboxEventFilter::testEventFilter()
{
    QObject *obj = 0;

    // First test key press functionality (all code branches).
    QKeyEvent event1(QEvent::KeyPress, 0, Qt::NoModifier);
    EXPECT(DialpadMailboxEventFilterBase, checkIfSendEventAndConsumeEvent).returns(true);

    bool eaten = mEventFilter->eventFilter(obj, &event1);
    QVERIFY(verify() && eaten);

    HbLineEdit line;// = new HbLineEdit();
    EXPECT(DialpadMailboxEventFilterBase, checkIfSendEventAndConsumeEvent).returns(false);
    EXPECT(DialpadMailboxEventFilterBase, isLongKeyPressSupported).returns(true);
    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString(""));
    EXPECT(QTimer, stop);
    EXPECT(QTimer ,start);
    eaten = mEventFilter->eventFilter(obj, &event1);
    QVERIFY(verify() && !eaten);

    EXPECT(DialpadMailboxEventFilterBase, checkIfSendEventAndConsumeEvent).returns(false);
    EXPECT(DialpadMailboxEventFilterBase, isLongKeyPressSupported).returns(true);
    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString("blaa"));
    eaten = mEventFilter->eventFilter(obj, &event1);
    QVERIFY(verify() && !eaten);

    EXPECT(DialpadMailboxEventFilterBase, checkIfSendEventAndConsumeEvent).returns(false);
    EXPECT(DialpadMailboxEventFilterBase, isLongKeyPressSupported).returns(false);
    eaten = mEventFilter->eventFilter(obj, &event1);
    QVERIFY(verify() && !eaten);

    //delete event1;

    // Test key release functionality (all code branches).
    QKeyEvent event2(QEvent::KeyRelease, 0, Qt::NoModifier);// = new QKeyEvent(QEvent::KeyRelease, 0, Qt::NoModifier);
    EXPECT(DialpadMailboxEventFilterBase, checkIfSendEventAndConsumeEvent).returns(true);
    eaten = mEventFilter->eventFilter(obj, &event2);
    QVERIFY(verify() && eaten);

    EXPECT(DialpadMailboxEventFilterBase, checkIfSendEventAndConsumeEvent).returns(false);
    EXPECT(DialpadMailboxEventFilterBase, isLongKeyPressSupported).returns(true);
    EXPECT(QTimer, stop);
    eaten = mEventFilter->eventFilter(obj, &event2);
    QVERIFY(verify() && !eaten);

    EXPECT(DialpadMailboxEventFilterBase, checkIfSendEventAndConsumeEvent).returns(false);
    EXPECT(DialpadMailboxEventFilterBase, isLongKeyPressSupported).returns(false);
    eaten = mEventFilter->eventFilter(obj, &event2);
    QVERIFY(verify() && !eaten);

    //delete event2;

    // Test non-supported key type functionality.
    QKeyEvent event3(QEvent::None, 0, Qt::NoModifier);// = new QKeyEvent(QEvent::None, 0, Qt::NoModifier);
    eaten = mEventFilter->eventFilter(obj, &event3);
    QVERIFY(verify() && !eaten);
    //delete event3;

    //delete line;
}

void ut_DialpadVoiceMailboxEventFilter::testHandleCallButtonPress()
{
    HbLineEdit line;// = new HbLineEdit();

    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString("1"));
    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString("1"));
    EXPECT(HbStringUtil, convertDigitsTo).returns(QString("1"));
    EXPECT(DialpadSymbianWrapper, getMailboxNumber).returns(97);
    bool handled = mEventFilter->handleCallButtonPress();
    QVERIFY(verify() && handled);

    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString("a"));
    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString("a"));
    EXPECT(HbStringUtil, convertDigitsTo).returns(QString("a"));
    handled = mEventFilter->handleCallButtonPress();
    QVERIFY(verify() && !handled);

    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString(""));
    handled = mEventFilter->handleCallButtonPress();
    QVERIFY(verify() && !handled);

    //delete line;
}

void ut_DialpadVoiceMailboxEventFilter::testHandleMailboxOperation()
{
    EXPECT(DialpadSymbianWrapper, getMailboxNumber).returns(DialpadErrorCancel);
    EXPECT(Dialpad, closeDialpad);
    EXPECT(DialpadSymbianWrapper, defineMailboxNumber).returns(DialpadErrorCancel);
    EXPECT(Dialpad, openDialpad);
    mEventFilter->handleMailboxOperation();
    QVERIFY(verify());

    EXPECT(DialpadSymbianWrapper, getMailboxNumber).returns(DialpadErrorNone);
    EXPECT(Dialpad, closeDialpad);
    EXPECT(DialpadSymbianWrapper, defineMailboxNumber).returns(DialpadErrorCancel);
    EXPECT(Dialpad, openDialpad);
    mEventFilter->handleMailboxOperation();
    QVERIFY(verify());

    EXPECT(DialpadSymbianWrapper, getMailboxNumber).returns(DialpadErrorCancel);
    EXPECT(Dialpad, closeDialpad);
    EXPECT(DialpadSymbianWrapper, defineMailboxNumber).returns(DialpadErrorNone);
    mEventFilter->handleMailboxOperation();
    QVERIFY(verify());

    QString text;
    EXPECT(DialpadSymbianWrapper, getMailboxNumber).with<QString &>(text).
        willOnce(invoke(editText)).times(1).returns(DialpadErrorCancel);
    EXPECT(Dialpad, closeDialpad);
    EXPECT(DialpadSymbianWrapper, defineMailboxNumber).returns(DialpadErrorCancel);
    EXPECT(Dialpad, openDialpad);
    mEventFilter->handleMailboxOperation();
    QVERIFY(verify());

    EXPECT(DialpadSymbianWrapper, getMailboxNumber).with<QString &>(text).
        willOnce(invoke(editText)).times(1).returns(DialpadErrorNone);
    EXPECT(DialpadMailboxEventFilterBase, createCall);
    EXPECT(DialpadMailboxEventFilterBase, clearEditor);
    EXPECT(Dialpad, openDialpad);
    mEventFilter->handleMailboxOperation();
    QVERIFY(verify());
}

void ut_DialpadVoiceMailboxEventFilter::testHandleLongKeyPress()
{
    QString text;
    mEventFilter->mKeyEvent = Qt::Key_1;
    EXPECT(DialpadSymbianWrapper, getMailboxNumber).with<QString &>(text).
        willOnce(invoke(editText)).times(1).returns(DialpadErrorNone);
    mEventFilter->handleLongKeyPress();
    QVERIFY(verify());

    mEventFilter->mKeyEvent = Qt::Key_Apostrophe;
    mEventFilter->handleLongKeyPress();
}


DIALPAD_TEST_MAIN(ut_DialpadVoiceMailboxEventFilter)
#include "ut_dialpadvoicemailboxeventfilter.moc"

