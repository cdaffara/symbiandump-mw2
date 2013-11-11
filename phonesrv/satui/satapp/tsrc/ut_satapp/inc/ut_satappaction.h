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
* Description: 
*
*/

#ifndef UT_SATAPPACTION_H
#define UT_SATAPPACTION_H

// includes
#include <QtTest/QtTest>
#include <QObject>


// forward declarations
class SatAppAction;



// QTest class
class Ut_SatAppAction : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppAction(QObject *parent = 0);

private slots:

    void testAction();
    void testParams();
    void testComplete();
    void testCompleteSignal();
    void testDoubleComplete();
    void testAsyncActionRobustness();
    void testWait();

private:


};



// Helper object
class ActionHelper : public QObject
{
    Q_OBJECT

public:
    ActionHelper() {mCounter=0;}
    int& count() {return mCounter;}

public slots:
    void inc() {++mCounter;}
    void del(SatAppAction*);

private:
    int mCounter;
};


#endif // UT_SATAPPACTION_H
