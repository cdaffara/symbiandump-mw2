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
* Description: Unit test CSatUiObserver class
*
*/

#ifndef UT_SATAPPSERVERDISPATCHER_H
#define UT_SATAPPSERVERDISPATCHER_H

#include <QtTest/QtTest>
#include <QObject>

// Class forwards
class SatAppServerDispatcher;
class SatAppAction;

/**
 *  CSatUiObserver unit test class.
 */
class Ut_SatAppServerDispatcher : public QObject
{
    Q_OBJECT

public:
    Ut_SatAppServerDispatcher(QObject *parent = 0);

private slots:

    void initTestCase();
    void cleanupTestCase();

    //test MSatUiObserver api
    void testDisplayText_data();
    void testDisplayText();
    void testGetInkey_data();
    void testGetInkey();
    void testGetInput_data();
    void testGetInput();
    void testSetUpMenu_data();
    void testSetUpMenu();
    void testSelectItem_data();
    void testSelectItem();
    void testPlayTone_data();
    void testPlayTone();
    void testConfirmCommand_data();
    void testConfirmCommand();
    void testNotification_data();
    void testNotification();
    void testEventNotification_data();
    void testEventNotification();

    // test slots
    void testMenuSelection();
    void testUserCancelSession();

private:

    SatAppServerDispatcher *mServer;

};


// internal helper class
class ServerDispatcherHelper: public QObject
{
    Q_OBJECT

public slots:
    void exec(SatAppAction& action);
    void simulateItemSelected(SatAppAction& action);
};

#endif // UT_SATAPPSERVERDISPATCHER_H
