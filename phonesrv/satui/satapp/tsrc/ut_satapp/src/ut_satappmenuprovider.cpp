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

#include <hbmainwindow.h>
#include <hblistwidget.h>
#include <hbaction.h>

#include "ut_satappmenuprovider.h"
#include "satappmenuprovider.h"
#include "satappaction.h"



// --------------------------------------------------------
// The unit tests begin here
// --------------------------------------------------------

Ut_SatAppMenuProvider::Ut_SatAppMenuProvider(
        HbMainWindow* mainWindow, QObject *parent)
    :QObject(parent)
{
    mMainWindow = mainWindow;
}

void Ut_SatAppMenuProvider::testResetState()
{
    MenuProviderTest test(mMainWindow);
    test.mMenu->resetState();
}

void Ut_SatAppMenuProvider::testSetUpMenu_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("menuitems");
    QTest::addColumn<int>("selection");

    QTest::newRow("t1") << "title" << 1 << 0;
    QTest::newRow("t2") << "a very long title that does not fit to screen" << 2 << 1;
    QTest::newRow("t3") << "manyrows" << 999 << 444;
    QTest::newRow("t4") << "" << 1 << 0;
    QTest::newRow("t5") << "" << 10 << 1;
    QTest::newRow("t6") << "a" << 10 << 9;
}

void Ut_SatAppMenuProvider::testSetUpMenu()
{
    QFETCH(QString, text);
    QFETCH(int, menuitems);
    QFETCH(int, selection);

    SatAppAction a(ESatSetUpMenuAction);
    a.set(KeyText,text);
    QStringList list;
    for(int i=0; i<menuitems; ++i) list<<QString::number(i);
    a.set(KeyMenu,list);
    
    MenuProviderTest test(mMainWindow);
    test.mMenu->setUpMenu(a);

    test.mItemIndexToBeSelected = selection;
    test.simulateItemSelected();

    QCOMPARE(a.response(), ESatSuccess);
    QVERIFY(a.hasValue(KeySelection));

}

void Ut_SatAppMenuProvider::testSetUpMenuAndReset()
{
    SatAppAction a(ESatSetUpMenuAction);
    a.set(KeyText,"hello");
    QStringList list;
    for(int i=0; i<10; ++i) {list<<QString::number(i);}
    a.set(KeyMenu,list);

    MenuProviderTest test(mMainWindow);
    test.mMenu->setUpMenu(a);
    
    test.mMenu->resetState();

    QCOMPARE(a.response(), ESatSuccess);

}

void Ut_SatAppMenuProvider::testSetUpMenuAndDelete()
{
    SatAppAction a(ESatSetUpMenuAction);
    a.set(KeyText,"hello");
    QStringList list;
    for(int i=0; i<10; ++i) {list<<QString::number(i);}
    a.set(KeyMenu,list);

    {
        MenuProviderTest test(mMainWindow);
        
        test.mMenu->setUpMenu(a);
    }
    // now SatAppMenuProvider is out of scope and deleted.
    QCOMPARE(a.response(), ESatSuccess);
}

void Ut_SatAppMenuProvider::testSelectItem_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("menuitems");
    QTest::addColumn<int>("defaultitem");
    QTest::addColumn<int>("selection");

    QTest::newRow("t1") << "title" << 1 << 0 << 0;
    QTest::newRow("t2") << "a very long title that does not fit to screen" << 2 << 0 << 1;
    QTest::newRow("t3") << "manyrows" << 999 << 333 << 444 ;
    QTest::newRow("t4") << "" << 3 << 2 << 1 ;
    QTest::newRow("t5") << "" << 4 << 3 << 0 ;
    QTest::newRow("t6") << "default out of range" << 5 << 7 << 1 ;
}

void Ut_SatAppMenuProvider::testSelectItem()
{
    QFETCH(QString, text);
    QFETCH(int, menuitems);
    QFETCH(int, defaultitem);
    QFETCH(int, selection);

    SatAppAction a(ESatSelectItemAction);
    a.set(KeyText,text);
    QStringList list;
    for(int i=0; i<menuitems; ++i) {list<<QString::number(i);}
    a.set(KeyMenu,list);
    a.set(KeyDefault,defaultitem);

    MenuProviderTest test(mMainWindow);
    test.loadMainview();
    test.mMenu->selectItem(a);

    test.mItemIndexToBeSelected = selection;
    test.simulateItemSelected();
    
    QCOMPARE(a.response(), ESatSuccess);
    QVERIFY(a.hasValue(KeySelection));

}

void Ut_SatAppMenuProvider::testSelectItemAndBack()
{
    SatAppAction a(ESatSelectItemAction);
    a.set(KeyText,"hello");
    QStringList list;
    for(int i=0; i<10; ++i) {list<<QString::number(i);}
    a.set(KeyMenu,list);
    a.set(KeyDefault,5);

    MenuProviderTest test(mMainWindow);
    test.loadMainview();
    test.mMenu->selectItem(a);
    test.simulateBackButtonClicked();

    QCOMPARE(a.response(), ESatBackwardModeRequestedByUser);

}


void Ut_SatAppMenuProvider::testSelectItemAndTimeout()
{
    SatAppAction a(ESatSelectItemAction);
    a.set(KeyText,"hello");
    QStringList list;
    for(int i=0; i<10; ++i) {list<<QString::number(i);}
    a.set(KeyMenu,list);
    a.set(KeyDefault,5);

    MenuProviderTest test(mMainWindow);
    test.loadMainview();
    test.mMenu->selectItem(a);
    a.waitUntilCompleted(); // wait selectItem timeout

    QCOMPARE(a.response(), ESatNoResponseFromUser);

}

void Ut_SatAppMenuProvider::testSelectItemAndReset()
{
    SatAppAction a(ESatSelectItemAction);
    a.set(KeyText,"hello");
    QStringList list;
    for(int i=0; i<10; ++i) {list<<QString::number(i);}
    a.set(KeyMenu,list);
    a.set(KeyDefault,5);

    MenuProviderTest test(mMainWindow);
    test.loadMainview();
    test.mMenu->selectItem(a);
    test.mMenu->resetState();

    QCOMPARE(a.response(), ESatSuccess);
   
}

void Ut_SatAppMenuProvider::testSelectItemAndDelete()
{
    SatAppAction a(ESatSelectItemAction);
    a.set(KeyText,"hello");
    QStringList list;
    for(int i=0; i<10; ++i) {list<<QString::number(i);}
    a.set(KeyMenu,list);
    a.set(KeyDefault,5);

    {
        MenuProviderTest test(mMainWindow);
        test.loadMainview();
        test.mMenu->selectItem(a);
    }

    // object deleted, blocking function should return safely
    QCOMPARE(a.response(), ESatSuccess);
}

// ---------------------------------------------------------------
// internal helper class
// ---------------------------------------------------------------

MenuProviderTest::MenuProviderTest(HbMainWindow* mainWindow)
{
    mMenu = new SatAppMenuProvider(mainWindow,this);
}

MenuProviderTest::~MenuProviderTest()
{
    cleanup();
}

void MenuProviderTest::loadMainview()
{
    mMenu->loadMainView();
}

void MenuProviderTest::simulateItemSelected()
{
    mMenu->menuItemSelected(mMenu->mSelectListWidget->item(
            mItemIndexToBeSelected));
}

void MenuProviderTest::simulateBackButtonClicked()
{
    mMenu->mSoftKeyBackAction->trigger();
}


void MenuProviderTest::cleanup()
{
    delete mMenu;
    mMenu = 0;
}

// End of file
