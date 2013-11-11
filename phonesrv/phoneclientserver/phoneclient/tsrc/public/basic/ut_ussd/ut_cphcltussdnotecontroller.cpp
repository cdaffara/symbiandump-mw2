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

#include <QTimer> // timeout callback
#include <QEventLoop>

#include "ut_cphcltussdnotecontroller.h"
#include "cphcltussdcommonconstant.h"
//test class
#include "cphcltussdnotecontroller.h"
//const
const int KExtendTimeoutMseconds = 50000;

// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::Ut_CphCltUssdNoteController
// -----------------------------------------------------------------------------
//
Ut_CphCltUssdNoteController::Ut_CphCltUssdNoteController(QObject *parent):
        QObject(parent)
{
    qDebug("Ut_CphCltUssdNoteController::Ut_CphCltUssdNoteController >");
    qDebug("Ut_CphCltUssdNoteController::Ut_CphCltUssdNoteController <");
}

// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::~Ut_CphCltUssdNoteController
// -----------------------------------------------------------------------------
//
Ut_CphCltUssdNoteController::~Ut_CphCltUssdNoteController()
{
    qDebug("Ut_CphCltUssdNoteController::~Ut_CphCltUssdNoteController >");
    qDebug("Ut_CphCltUssdNoteController::~Ut_CphCltUssdNoteController <");
}


void Ut_CphCltUssdNoteController::GlobalWaitNoteHidden()
{
    qDebug("Ut_CphCltUssdNoteController::GlobalWaitNoteHidden >");
    qDebug("Ut_CphCltUssdNoteController::GlobalWaitNoteHidden <");
}
// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::initTestCase
// QTestLib cleanup method, called after the last testfunction .
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::initTestCase()
{
    qDebug("Ut_CphCltUssdNoteController::initTestCase >");
    qDebug("Ut_CphCltUssdNoteController::initTestCase <");
}


// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::cleanupTestCase
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::cleanupTestCase()
{
    qDebug("Ut_CphCltUssdNoteController::cleanupTestCase >");
    if (mNoteController) {
        qDebug("Ut_CphCltUssdNoteController::cleanupTestCase delete \
            mNoteController");
        delete mNoteController;
        mNoteController = 0;
    }
    qDebug("Ut_CphCltUssdNoteController::cleanupTestCase <");
}

// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::testCreateMainHandler
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::testCreateMainHandler()
{
    qDebug("Ut_CphCltUssdNoteController::testProfileState >");
    mNoteController = CPhCltUssdNoteController::NewL(*this);
    QVERIFY(mNoteController);
    qDebug("Ut_CphCltUssdNoteController::testProfileState <");
}


// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::testShowGlobalInformationNote_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::testShowGlobalInformationNote_data()
{
    qDebug("Ut_CphCltUssdNoteController::testShowGlobalInformationNote_data >");
    QTest::addColumn<int>("noteType");
    QTest::newRow("Unconfirme") << 1;
    QTest::newRow("Notallowed") << 2;
    QTest::newRow("Noservice") << 3;
    QTest::newRow("Offline") << 4;
    QTest::newRow("Done") << 5;
    QTest::newRow("NotDone") << 6;
    qDebug("Ut_CphCltUssdNoteController::testShowGlobalInformationNote_data <");
}

// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::testShowGlobalInformationNote
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::testShowGlobalInformationNote()
{
    qDebug("Ut_CphCltUssdNoteController::testShowGlobalInformationNote >");
    QFETCH(int, noteType);
    TPhCltUssdInformationType infoType = 
        static_cast<TPhCltUssdInformationType>(noteType);
    TRAPD(err,mNoteController->ShowGlobalInformationNoteL(infoType) );
    QCOMPARE(KErrNone, err);
    qDebug("Ut_CphCltUssdNoteController::testShowGlobalInformationNote <");
}

// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::testProfileState
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::testShowGlobalWaitNote()
{
    qDebug("Ut_CphCltUssdNoteController::testShowGlobalWaitNote >");
    QVERIFY(mNoteController);
    TRAPD(err, mNoteController->ShowGlobalWaitNoteL() );
    QCOMPARE(KErrNone, err);
    qDebug("Ut_CphCltUssdNoteController::testShowGlobalWaitNote <");
}

// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::testDestroyGlobalWaitNote
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::testDestroyGlobalWaitNote()
{
    qDebug("Ut_CphCltUssdNoteController::testDestroyGlobalWaitNote >");
    QVERIFY(mNoteController);
    mNoteController->DestroyGlobalWaitNote();
    qDebug("Ut_CphCltUssdNoteController::testDestroyGlobalWaitNote <");
}

// -----------------------------------------------------------------------------
// Ut_CphCltUssdNoteController::testCancelGlobalWaitNote
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_CphCltUssdNoteController::testCancelGlobalWaitNote()
{
    qDebug("Ut_CphCltUssdNoteController::testCancelGlobalWaitNote >");
    QVERIFY(mNoteController);
    TRAPD(err, mNoteController->ShowGlobalWaitNoteL() );
    QTimer *timer = new QTimer(this);
    timer->start(KExtendTimeoutMseconds);
    bool ret = connect(timer, SIGNAL(timeout()), timer, SLOT(stop()));


    QEventLoop *loop = new QEventLoop(this);
    ret = connect(timer, SIGNAL(timeout()), loop, SLOT(quit()));
    loop->exec(QEventLoop::AllEvents);
    mNoteController->DestroyGlobalWaitNote();
    qDebug("Ut_CphCltUssdNoteController::testCancelGlobalWaitNote <");
}

// End of file
