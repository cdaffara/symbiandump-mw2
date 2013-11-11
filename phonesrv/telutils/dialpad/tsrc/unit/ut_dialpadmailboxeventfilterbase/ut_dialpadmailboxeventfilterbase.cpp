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

#include "dialpadtest.h"
#include "dialpad.h"
#include "dialpadmailboxeventfilterbase.h"


// Dummy helper class
class DummyFilter : public DialpadMailboxEventFilterBase
{
    Q_OBJECT
    friend class ut_DialpadMailboxEventFilterBase;
public:
    DummyFilter(Dialpad* dialpad, QObject* parent = 0);
    virtual ~DummyFilter(){};

public:
    bool eventFilter(QObject *, QEvent *){return true;};
    bool handleCallButtonPress(){return true;};
    void handleMailboxOperation(){};

public slots:
    void handleLongKeyPress(){};

};

DummyFilter::DummyFilter(Dialpad *dialpad, QObject *parent)
:DialpadMailboxEventFilterBase(dialpad, parent)
{
}


// test class
class ut_DialpadMailboxEventFilterBase : public QObject, MockService
{
    Q_OBJECT

public:    
    ut_DialpadMailboxEventFilterBase();
    virtual ~ut_DialpadMailboxEventFilterBase();

private slots:
    void init();
    void cleanup();

    // Mailbox event filter base unit tests
    void testCheckIfSendEventAndConsumeEvent();
    void testIsLongKeyPressSupported();
    void testClearEditor();
    void testCreateCall();

private:
    DummyFilter *mEventFilter;
    Dialpad *mDialpad;
};


ut_DialpadMailboxEventFilterBase::ut_DialpadMailboxEventFilterBase()
{
}

ut_DialpadMailboxEventFilterBase::~ut_DialpadMailboxEventFilterBase()
{
}

void ut_DialpadMailboxEventFilterBase::init()
{
    initialize();
    HbMainWindow *window = 0;
    mDialpad = new Dialpad(*window);
    mEventFilter = new DummyFilter(mDialpad);
}

void ut_DialpadMailboxEventFilterBase::cleanup()
{
    delete mEventFilter;
    delete mDialpad;
    reset();
}

void ut_DialpadMailboxEventFilterBase::testCheckIfSendEventAndConsumeEvent()
{
    int key = Qt::Key_Yes;
    int type = QEvent::KeyPress;
    bool handled = mEventFilter->checkIfSendEventAndConsumeEvent(key, type);
    QVERIFY(handled);

    key = Qt::Key_Enter;
    type = QEvent::KeyRelease;
    HbLineEdit line;
    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString("blah"));
    handled = mEventFilter->checkIfSendEventAndConsumeEvent(key, type);
    QVERIFY(verify() && handled);

    key = Qt::Key_Enter;
    type = QEvent::KeyRelease;
    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, text).returns(QString(""));
    handled = mEventFilter->checkIfSendEventAndConsumeEvent(key, type);
    QVERIFY(verify() && !handled);

    key = Qt::Key_Enter;
    type = QEvent::MouseButtonPress;
    handled = mEventFilter->checkIfSendEventAndConsumeEvent(key, type);
    QVERIFY(!handled);

    key = Qt::Key_Apostrophe;
    handled = mEventFilter->checkIfSendEventAndConsumeEvent(key, type);
    QVERIFY(!handled);
}

void ut_DialpadMailboxEventFilterBase::testIsLongKeyPressSupported()
{
    int key = Qt::Key_Apostrophe;
    bool supported = mEventFilter->isLongKeyPressSupported(key);
    QVERIFY(!supported);

    key = Qt::Key_1;
    supported = mEventFilter->isLongKeyPressSupported(key);
    QVERIFY(supported);
}

void ut_DialpadMailboxEventFilterBase::testClearEditor()
{
    HbLineEdit line;
    EXPECT(Dialpad, editor).returns(&line);
    EXPECT(HbLineEdit, setText).with(QString(""));
    mEventFilter->clearEditor();
    QVERIFY(verify());
}

void ut_DialpadMailboxEventFilterBase::testCreateCall()
{
    QString number("313");
    mEventFilter->createCall(number, true);
    mEventFilter->createCall(number, false);
}


DIALPAD_TEST_MAIN(ut_DialpadMailboxEventFilterBase)
#include "ut_dialpadmailboxeventfilterbase.moc"

