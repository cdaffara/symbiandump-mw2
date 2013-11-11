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

#include <QVariant>
#include <QTimer>
#include <badesca.h>

#include <hbmainwindow.h>
#include <hblistwidget.h>

#include "ut_satappaction.h"
#include "satappaction.h"



// --------------------------------------------------------
// The unit tests begin here
// --------------------------------------------------------

Ut_SatAppAction::Ut_SatAppAction(QObject *parent)
    :QObject(parent)
{
}


void Ut_SatAppAction::testAction()
{
    SatAppAction a(ESatDisplayTextAction);
    SatAppAction b(ESatGetInputAction);
    SatAppAction c(ESatPlayToneAction);
    
    QCOMPARE(a.action(),(int)ESatDisplayTextAction);
    QCOMPARE(b.action(),(int)ESatGetInputAction);
    QCOMPARE(c.action(),(int)ESatPlayToneAction);

}

void Ut_SatAppAction::testParams()
{
    SatAppAction a(ESatDisplayTextAction);
    // qvariant
    a.set(KeyText, QVariant("hello"));
    QCOMPARE(a.value(KeyText).toString(),QString("hello"));

    // int
    a.set(KeyMinLength, 25);
    QCOMPARE(a.value(KeyMinLength).toInt(),25);

    // string
    a.set(KeyInputString,_L("kiss"));
    QCOMPARE(a.value(KeyInputString).toString(),QString("kiss"));

    // descriptor array
    CDesCArrayFlat* array1 = new CDesCArrayFlat(8);
    array1->AppendL(_L("item_0"));
    array1->AppendL(_L("item_1"));
    array1->AppendL(_L("item_2"));
    a.set(KeyMenu,*array1);
    QList<QVariant> list1 = a.value(KeyMenu).toList();
    QCOMPARE(list1.size(), 3);
    QCOMPARE(list1[0].toString(), QString("item_0"));
    QCOMPARE(list1[1].toString(), QString("item_1"));
    QCOMPARE(list1[2].toString(), QString("item_2"));

    // int array
    CArrayFixFlat<TInt>* array2 = new CArrayFixFlat<TInt>(8);
    array2->AppendL(1);
    array2->AppendL(2);
    array2->AppendL(3);
    a.set(KeyIconIdArray,*array2);
    QList<QVariant> list2 = a.value(KeyIconIdArray).toList();
    QCOMPARE(list2.size(), 3);
    QCOMPARE(list2[0].toInt(), 1);
    QCOMPARE(list2[1].toInt(), 2);
    QCOMPARE(list2[2].toInt(), 3);

    // TTimeIntervalSeconds
    TTimeIntervalSeconds seconds(3);
    a.set(KeyDuration,seconds);
    QCOMPARE(a.value(KeyDuration).toInt(),3000);
    
    // TTimeIntervalMicroSeconds
    TTimeIntervalMicroSeconds ysec(TInt64(500000));
    a.set(KeyDuration,ysec);
    QCOMPARE(a.value(KeyDuration).toInt(),500);
    
    // test hasValue
    QVERIFY(a.hasValue(KeyText));
    QVERIFY(a.hasValue(KeyMinLength));
    QVERIFY(a.hasValue(KeyInputString));
    QVERIFY(a.hasValue(KeyMenu));
    QVERIFY(a.hasValue(KeyIconIdArray));
    QVERIFY(a.hasValue(KeyDuration));
    QVERIFY(!a.hasValue(KeyHelpRequested)); // <-- not set
    QVERIFY(!a.hasValue(KeyHelpRequested)); // <-- not set
    QVERIFY(!a.hasValue(KeyCharacterSet)); // <-- not set
    QVERIFY(!a.hasValue(QString("billy the kid"))); // <-- definitely not set
}

void Ut_SatAppAction::testComplete()
{
    SatAppAction a(ESatDisplayTextAction);
    SatAppAction b(ESatGetInputAction);
    SatAppAction c(ESatPlayToneAction);
    SatAppAction d(ESatPlayToneAction);
    SatAppAction e(ESatPlayToneAction);
    a.complete();
    b.completeWithSuccess();
    c.completeWithFailure();
    d.completeWithBackRequested();
    e.completeWithSessionTerminated();
    QCOMPARE(a.response(),ESatNoResponseFromUser);
    QCOMPARE(b.response(),ESatSuccess);
    QCOMPARE(c.response(),ESatFailure);
    QCOMPARE(d.response(),ESatBackwardModeRequestedByUser);
    QCOMPARE(e.response(),ESatSessionTerminatedByUser);
}

void Ut_SatAppAction::testCompleteSignal()
{
    SatAppAction a(ESatDisplayTextAction);
    SatAppAction b(ESatGetInputAction);
    SatAppAction c(ESatPlayToneAction);
    SatAppAction d(ESatPlayToneAction);
    SatAppAction e(ESatPlayToneAction);
    ActionHelper x;
    connect(&a, SIGNAL(actionCompleted(SatAppAction*)), &x, SLOT(inc()));
    connect(&b, SIGNAL(actionCompleted(SatAppAction*)), &x, SLOT(inc()));
    connect(&c, SIGNAL(actionCompleted(SatAppAction*)), &x, SLOT(inc()));
    connect(&d, SIGNAL(actionCompleted(SatAppAction*)), &x, SLOT(inc()));
    connect(&e, SIGNAL(actionCompleted(SatAppAction*)), &x, SLOT(inc()));
    a.complete();
    b.completeWithSuccess();
    c.completeWithFailure();
    d.completeWithBackRequested();
    e.completeWithSessionTerminated();
    QCOMPARE(x.count(),5);
}

void Ut_SatAppAction::testDoubleComplete()
{
    SatAppAction a(ESatDisplayTextAction);
    a.completeWithSuccess();
    QCOMPARE(a.response(),ESatSuccess);
    a.completeWithFailure(); // <-- no effect because already completed
    QCOMPARE(a.response(),ESatSuccess); // <-- still in same state!
}

void Ut_SatAppAction::testAsyncActionRobustness()
{
    // this function tests if an action can be safely deleted in the "completed"
    // slot, at the same time testing robustness and memory leaks by repeating
    // the sequence multiple times
    ActionHelper x;
    for(int i=0; i<10000; ++i)
    {
        SatAppAction* a = new SatAppAction(ESatDisplayTextAction, this);
        connect(a,SIGNAL(actionCompleted(SatAppAction*)),&x,SLOT(del(SatAppAction*)));
        a->complete();
    }
}


void Ut_SatAppAction::testWait()
{
    SatAppAction a(ESatDisplayTextAction);
    QTimer::singleShot(1000, &a, SLOT(completeWithSuccess()));
    // this should last 1 second
    a.waitUntilCompleted();
    QCOMPARE(a.response(),ESatSuccess);

    // this should flow through directly, because already completed
    a.waitUntilCompleted();
    QCOMPARE(a.response(),ESatSuccess); // <-- response remains unchanged
    
}



// helper
void ActionHelper::del(SatAppAction* action)
{
    delete action;
}


// End of file
