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

#ifndef UT_SATAPPMENUPROVIDER_H
#define UT_SATAPPMENUPROVIDER_H

// includes
#include <QtTest/QtTest>
#include <QObject>


// forward declarations
class HbMainWindow;
class SatAppMenuProvider;



// QTest class
class Ut_SatAppMenuProvider : public QObject
{
    Q_OBJECT

public:

    Ut_SatAppMenuProvider(
            HbMainWindow* mainWindow, QObject *parent = 0);

private slots:

    void testResetState();

    void testSetUpMenu_data();
    void testSetUpMenu();
    void testSetUpMenuAndReset();
    void testSetUpMenuAndDelete();

    void testSelectItem_data();
    void testSelectItem();
    void testSelectItemAndBack();
    void testSelectItemAndTimeout();
    void testSelectItemAndReset();
    void testSelectItemAndDelete();

private:

    HbMainWindow* mMainWindow; // not own

};


// internal helper class
class MenuProviderTest: public QObject
{
    Q_OBJECT

public:

    MenuProviderTest(HbMainWindow* mainWindow);
    ~MenuProviderTest();

public slots:
    void loadMainview();
    void simulateItemSelected();
    void simulateBackButtonClicked();
    void cleanup();

public:

    // test data
    SatAppMenuProvider *mMenu;
    int mItemIndexToBeSelected;

};


#endif // UT_SATAPPMENUPROVIDER_H
