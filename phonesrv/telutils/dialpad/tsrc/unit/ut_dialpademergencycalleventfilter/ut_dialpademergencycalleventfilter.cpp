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
#include <mockservice.h>

#include "mock_cphcltemergencycall.h"
#include "dialpadtest.h"
#include "dialpadtestutil.h"
#include "dialpademergencycalleventfilter.h"
#include "dialpad.h"

const int WAIT_TIME = 300;

// test cases
class ut_DialpadEmergencyCallEventFilter : public QObject, public MockService
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

    // These are actual emergency call event filter unit tests
    void testEmergencyCallEventFilter();

private:
    HbMainWindow*  mMainWindow; 
    Dialpad*       mDialpad;
    DialpadEmergencyCallEventFilter *mEventFilter;
    DialpadTestUtil* mUtil;
};

void ut_DialpadEmergencyCallEventFilter::initTestCase()
{
    mMainWindow = new HbMainWindow;

    mDialpad = new Dialpad();
    mEventFilter = new DialpadEmergencyCallEventFilter(mDialpad, this);
    //hbInstance->allMainWindows().at(0)->installEventFilter(mEventFilter);
    mMainWindow->installEventFilter(mEventFilter);

    mUtil = new DialpadTestUtil(*mMainWindow);
    
    QRectF rect(mMainWindow->contentsRect());
    rect.setHeight(rect.height()*0.7);
    rect.moveTop((mMainWindow->contentsRect().height()-rect.height())/2);

    mDialpad->setPreferredSize(mMainWindow->layoutRect().width(),
                               mMainWindow->layoutRect().height()/2);
    mDialpad->setPos(0,mMainWindow->layoutRect().height()/4);

    mMainWindow->show();
    mDialpad->show();
    mDialpad->hide();
}

void ut_DialpadEmergencyCallEventFilter::init()
{
    initialize();
}

void ut_DialpadEmergencyCallEventFilter::cleanupTestCase()
{
    delete mDialpad;
    delete mMainWindow;
    delete mUtil;
}

void ut_DialpadEmergencyCallEventFilter::cleanup()
{
    reset();
    mDialpad->editor().setText(QString());
    QTest::qWait( WAIT_TIME ); // delay between tests
}

void checkNumber(const TDesC& aNumber, TBool& aIsEmergencyNumber)
{
    if (aNumber == _L("112")) {
        aIsEmergencyNumber = true;
    }
}

void ut_DialpadEmergencyCallEventFilter::testEmergencyCallEventFilter()
{
    EXPECT(CPhCltEmergencyCallMock, IsEmergencyPhoneNumber).willOnce(
            invoke(checkNumber));
    EXPECT(CPhCltEmergencyCallMock, DialEmergencyCallL);
    
    mDialpad->openDialpad();
    QTest::qWait(WAIT_TIME);
    mUtil->mouseClickDialpad(Qt::Key_1);
    QTest::qWait(1000);
    mUtil->mouseClickDialpad(Qt::Key_1);
    QTest::qWait(1000);
    mUtil->mouseClickDialpad(Qt::Key_2);
    QTest::qWait(1000);
    mUtil->mouseClickDialpad(Qt::Key_Yes);
    QTest::qWait(1000);
    
    mDialpad->closeDialpad();

    QVERIFY(verify());
}

DIALPAD_TEST_MAIN(ut_DialpadEmergencyCallEventFilter)
#include "ut_dialpademergencycalleventfilter.moc"

