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
* Description: Unit test for CSatUiObserver
*
*/

#include "ut_satappserverdispatcher.h"
#include "satappserverdispatcher.h"
#include "satappaction.h"

Ut_SatAppServerDispatcher::Ut_SatAppServerDispatcher(QObject *parent):QObject(parent) 
{
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::initTestCase
// QTestLib cleanup method, called after the last testfunction .
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::initTestCase()
{
    mServer = new SatAppServerDispatcher(this);
}


// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::cleanupTestCase
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::cleanupTestCase()
{
    delete mServer;
    mServer = 0;
}




// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testDisplayText_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testDisplayText_data()
{
    qDebug("Ut_SatAppServerDispatcher::testDisplayText_data >");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("appName");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<bool>("isIconDisplayed");
    QTest::addColumn<bool>("sustainedText");
    QTest::addColumn<int>("duration");
    QTest::addColumn<bool>("isWaitUsertoClear");
    QTest::addColumn<int>("expectValue");

    QTest::newRow("first") << "Show Display text!" << "Unit test SIM Service" << 
        84 << 1 << false << false << 2500 << true << 0;

    QTest::newRow("second") << "Show Display text!" << "SIM Service" << 
        84 << 1 << true << false << 2500 << false << 0;
    // test null text
    QTest::newRow("third") << "" << "SIM Service" << 
        84 << 1 << true << false << 2500 << false << -1;

    // test icon is available for command
    QTest::newRow("fouth") << "Show Display text!" << "SIM Service" << 
        84 << 2 << true << false << 2500 << false << 0;

    // test show a log text over the limitation
    QTest::newRow("five") << "Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. Show a very very long text. " 
        << "SIM Service" << 84 << 2 << true << false << 2500 << false << -1;
   
    // test leave occurs
    //QTest::newRow("forth") << "Show Display text!" << "" << 
    //    84 << 2 << true << false << 2500 << false << 1;

    qDebug("Ut_SatAppServerDispatcher::testDisplayText_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testDisplayText
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testDisplayText()
{
    qDebug("Ut_SatAppServerDispatcher::testDisplayText >");
    QVERIFY(mServer);
    QFETCH(QString, text);
    QFETCH(QString, appName);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(bool, isIconDisplayed);
    QFETCH(bool, sustainedText);
    QFETCH(int, duration);
    QFETCH(bool, isWaitUsertoClear);
    QFETCH(int, expectValue);

    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // sim name
    HBufC* hSimAppName = HBufC::New(appName.length());
    TPtr aSimApplicationName( hSimAppName->Des() );
    aSimApplicationName.Copy( appName.utf16() );

    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    TBool aIsIconDisplayed(isIconDisplayed);

    TSatUiResponse result(ESatSuccess);
    ServerDispatcherHelper helper;
    connect(mServer,SIGNAL(displayText(SatAppAction&)),&helper,SLOT(exec(SatAppAction&)));
    // It seems that TRAPD&QT_TRAP_THROWING does nothing if leave occurs, need 
    // to investigate.
    QT_TRAP_THROWING( result = mServer->DisplayTextL(
                              aText, 
                              aSimApplicationName,
                              aIconId,
                              aIsIconDisplayed,
                              sustainedText,
                              duration,
                              isWaitUsertoClear
                              ));
    delete hText;
    hText = 0;
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectValue);
    QCOMPARE(exValue, result);
    qDebug("Ut_SatAppServerDispatcher::testDisplayText <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testGetInkey_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testGetInkey_data()
{
    qDebug("Ut_SatAppServerDispatcher::testGetInkey_data >");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("characterSet");//enum 0~3
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("isHelpIsAvailable");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<bool>("isRequestedIconDisplayed");
    QTest::addColumn<int>("duration");
    QTest::addColumn<bool>("isImmediateDigitResponse");
    QTest::addColumn<int>("expectValue");
    //// test ESatDigitOnly
    QTest::newRow("first") << "Test GetInKey" << 0 << 
        "8" << false << 60 << 1 << false << 2500 << true << 0;
    // null text
    QTest::newRow("second") << "" << 0 << 
        "s" << false<< 60 << 1 << false << 2500 << true << 0;
    // test EIconGetYesNo
    QTest::newRow("third") << "Test GetInKey" << 3 << 
        "s" << false << 60 << 1 << false << 2500 << true << 0;
    // test ESatCharSmsDefaultAlphabet
    QTest::newRow("fourth") << "Test GetInKey" << 3 << 
        "$" << false << 60 << 1 << false << 2500 << true << 0;
    // ino immediateDigitResponse
    QTest::newRow("fifth") << "Test GetInKey" << 1 << 
    "#" << false<< 60 << 1 << false << 2500 << false << 0;
    // test zero lenght and yes no
    QTest::newRow("sisth") << "" << 3 << 
        "S" << false << 60 << 1 << false << 2500 << true << -1;
    qDebug("Ut_SatAppServerDispatcher::testGetInkey_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testGetInkey
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testGetInkey()
{
    qDebug("Ut_SatAppServerDispatcher::testGetInkey >");
    QVERIFY(mServer);
    QFETCH(QString, text);
    QFETCH(int, characterSet);
    QFETCH(QString, input);
    QFETCH(bool, isHelpIsAvailable);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(bool, isRequestedIconDisplayed);
    QFETCH(int, duration);
    QFETCH(bool, isImmediateDigitResponse);
    QFETCH(int, expectValue);

    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // aCharacterSet
    TSatCharacterSet aCharacterSet = static_cast<TSatCharacterSet>(characterSet);
    // aInput
    TBuf<1> tInput = input.utf16();
    TChar aInput(tInput[0]);
    // aHelpIsAvailable
    TBool aHelpIsAvailable(isHelpIsAvailable);
    // aIconId
    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    // aRequestedIconDisplayed
    TBool aRequestedIconDisplayed(isRequestedIconDisplayed);
    // aDuration
    TUint aDuration= static_cast<TUint>(duration);
    // aRequestedIconDisplayed
    //TBool aImmediateDigitResponse(isImmediateDigitResponse);
    ServerDispatcherHelper helper;
    connect(mServer,SIGNAL(getInkey(SatAppAction&)),&helper,SLOT(exec(SatAppAction&)));
    TSatUiResponse result(ESatSuccess);
    QT_TRAP_THROWING( result = mServer->GetInkeyL(
                              aText, 
                              aCharacterSet,
                              aInput,
                              aHelpIsAvailable,
                              aIconId,
                              aRequestedIconDisplayed,
                              aDuration,
                              isImmediateDigitResponse
                              ));
    delete hText;
    hText = 0;
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectValue);
    QCOMPARE(exValue, result);

    qDebug("Ut_SatAppServerDispatcher::testGetInkey <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testGetInput_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testGetInput_data()
{
    qDebug("Ut_SatAppServerDispatcher::testGetInput_data >");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("characterSet");//enum 0~3
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("minLength");
    QTest::addColumn<int>("maxLength");
    QTest::addColumn<bool>("isHideInput");
    QTest::addColumn<bool>("isHelpIsAvailable");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<bool>("isRequestedIconDisplayed");
    QTest::addColumn<int>("expectValue");
    // test characterSet ESatCharSmsDefaultAlphabet
    QTest::newRow("first") << "Test GetInput" << 1 << 
        "get input" << 1 << 5 << false << false <<60 << 1 << true << 0;
    // test iconIdQualifier ESatSelfExplanatory
    QTest::newRow("second") << "Test GetInput" << 0 << 
      "get input" << 1 << 5 << false << false <<60 << 2 << true << 0;
    // test hide
    QTest::newRow("third") << "Test GetInput" << 1 << 
      "" << 1 << 5 << true << false <<60 << 2 << true << -1;
    qDebug("Ut_SatAppServerDispatcher::testGetInput_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testGetInput
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testGetInput()
{
    qDebug("Ut_SatAppServerDispatcher::testGetInput >");
    QVERIFY(mServer);
    QFETCH(QString, text);
    QFETCH(int, characterSet);
    QFETCH(QString, input);
    QFETCH(int, minLength);
    QFETCH(int, maxLength);
    QFETCH(bool, isHideInput);
    QFETCH(bool, isHelpIsAvailable);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(bool, isRequestedIconDisplayed);
    QFETCH(int, expectValue);

    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // aCharacterSet
    TSatCharacterSet aCharacterSet = static_cast<TSatCharacterSet>(characterSet);
    // aInput
    HBufC* hInput = HBufC::New(input.length());
    TPtr aInput( hInput->Des() );
    aInput.Copy( input.utf16() );
    // aIconId
    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    // aRequestedIconDisplayed
    TBool aRequestedIconDisplayed(isRequestedIconDisplayed);
    ServerDispatcherHelper helper;
    connect(mServer,SIGNAL(getInput(SatAppAction&)),&helper,SLOT(exec(SatAppAction&)));
    TSatUiResponse result(ESatSuccess);
    QT_TRAP_THROWING( result = mServer->GetInputL(
                              aText, 
                              aCharacterSet,
                              aInput,
                              minLength,
                              maxLength,
                              isHideInput,
                              isHelpIsAvailable,
                              aIconId,
                              aRequestedIconDisplayed
                              ));
    delete hText;
    hText = 0;
    delete hInput;
    hInput = 0;
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectValue);
    QCOMPARE(exValue, result);
    qDebug("Ut_SatAppServerDispatcher::testGetInput <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testSetUpMenu_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testSetUpMenu_data()
{
    qDebug("Ut_SatAppServerDispatcher::testSetUpMenu_data >");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("setupMenuItem");
    QTest::addColumn<bool>("isHelpIsAvailable");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<int>("iconListQualifier");
    QTest::addColumn<int>("expectReturnValue");

    QStringList testSetUpMenu_1_items;
    testSetUpMenu_1_items 
    << "SendData" 
    << "ReceiveData" 
    << "CloseChannel"
    << "OpenChannel"
    << "GetChannelStatus"
    << "DisplayText with different symbols and durations"
    << "GetInkey"
    << "GetInput"
    << "SelectItem"
    << "PlayTone"
    << "RefreshRequired"
    << "DoRefresh"
    << "SendSm"
    << "SendSs"
    << "SendUssd"
    << "SetUpCall"
    << "SetUpIdleModeText"
    << "CallControl"
    << "MoSmControl"
    << "LaunchBrowser"
    << "SendDTMF"
    << "SetUpEventList"
    << "ProvideLocalInfo"
    << "EventDownload for IdleScreen available event"
    << "SetUpMenu";

    QTest::newRow("first") << "SAT" << testSetUpMenu_1_items << 
        false << 80 << 1 << 2 << 0;

    qDebug("Ut_SatAppServerDispatcher::testSetUpMenu_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testSetUpMenu
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testSetUpMenu()
{
    qDebug("Ut_SatAppServerDispatcher::testSetUpMenu >");
    QVERIFY(mServer);
    QFETCH(QString, text);
    QFETCH(QStringList, setupMenuItem);
    QFETCH(bool, isHelpIsAvailable);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(int, iconListQualifier);
    QFETCH(int, expectReturnValue);

    // aText
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // aMenuItems
    TBuf<128> menuItem;
    CDesCArrayFlat* aMenuItems = new CDesCArrayFlat(4);
    QVERIFY(aMenuItems);
    for (int i = 0; i< setupMenuItem.count(); i++) {
        menuItem = setupMenuItem.at(i).utf16();
        QT_TRAP_THROWING(aMenuItems->AppendL(menuItem));
    }
    // aIconId
    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    // aMenuItemNextActions
    CArrayFixFlat<TSatAction>* aMenuItemNextActions = 
          new CArrayFixFlat<TSatAction> (4);
     QVERIFY(aMenuItemNextActions);
    // Temp test
     QT_TRAP_THROWING(aMenuItemNextActions->AppendL(ESatSetUpMenuAction));
    // aMenuIcons
    CArrayFixFlat<TInt>* aMenuIcons = new CArrayFixFlat<TInt>(2);
    QVERIFY(aMenuIcons);
    // Temp test
    QT_TRAP_THROWING(aMenuIcons->AppendL(1));
    // aIconListQualifier
    TSatIconQualifier aIconListQualifier = 
        static_cast<TSatIconQualifier>(iconListQualifier);
    // TSatSelectionPreference, useless
    TSatSelectionPreference aSelectionPreference(ESatNoSelectionPreference);
    TSatUiResponse result(ESatSuccess);
    QT_TRAP_THROWING(result = mServer->SetUpMenuL(
                              aText,
                              *aMenuItems,
                              aMenuItemNextActions,
                              isHelpIsAvailable,
                              aIconId,
                              aMenuIcons,
                              aIconListQualifier,
                              aSelectionPreference
                              ));
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectReturnValue);
    QCOMPARE(exValue, result);

    delete hText;
    hText = 0;
    delete aMenuItems;
    aMenuItems = 0;
    delete aMenuItemNextActions;
    aMenuItemNextActions = 0;
    delete aMenuIcons;
    aMenuIcons = 0;
    qDebug("Ut_SatAppServerDispatcher::testSetUpMenu <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testSelectItem_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testSelectItem_data()
{
    qDebug("Ut_SatAppServerDispatcher::testSelectItem_data >");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("selectItemItem");
    QTest::addColumn<int>("defaultItem");
    QTest::addColumn<bool>("isHelpIsAvailable");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<int>("iconListQualifier");
    QTest::addColumn<int>("expectReturnValue");

    QStringList testSelectItem_1_items;
    testSelectItem_1_items 
    << "Select Item 1" 
    << "Select Item 2" 
    << "Select Item 3" 
    << "Select Item 4" 
    << "Select Item 5";

    QTest::newRow("first") << "Make a Choice" << testSelectItem_1_items << 
        0 << false << 80 << 1 << 2 << 0;

    qDebug("Ut_SatAppServerDispatcher::testSelectItem_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testSetUpMenu
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testSelectItem()
{
    qDebug("Ut_SatAppServerDispatcher::testSelectItem >");
    QVERIFY(mServer);
    QFETCH(QString, text);
    QFETCH(QStringList, selectItemItem);
    QFETCH(int, defaultItem);
    QFETCH(bool, isHelpIsAvailable);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(int, iconListQualifier);
    QFETCH(int, expectReturnValue);

    // aText
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // aMenuItems
    TBuf<128> menuItem;
    CDesCArrayFlat* aMenuItems = new CDesCArrayFlat(4);
    QVERIFY(aMenuItems);
    for (int i = 0; i< selectItemItem.count(); i++) {
        menuItem = selectItemItem.at(i).utf16();
        QT_TRAP_THROWING(aMenuItems->AppendL(menuItem));
    }
    
    TUint8 aSelectedItem( 1 );
    // aIconId
    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    // aMenuItemNextActions
    CArrayFixFlat<TSatAction>* aMenuItemNextActions = 
          new CArrayFixFlat<TSatAction> (4);
     QVERIFY(aMenuItemNextActions);
    // Temp test
     QT_TRAP_THROWING(aMenuItemNextActions->AppendL(ESatSelectItemAction));
    // aMenuIcons
    CArrayFixFlat<TInt>* aMenuIcons = new CArrayFixFlat<TInt>(2);
    QVERIFY(aMenuIcons);
    // Temp test
    QT_TRAP_THROWING(aMenuIcons->AppendL(1));
    // aIconListQualifier
    TSatIconQualifier aIconListQualifier = 
        static_cast<TSatIconQualifier>(iconListQualifier);
    // TSatSelectionPreference, useless
    TSatSelectionPreference aSelectionPreference(ESatNoSelectionPreference);
    TBool aRequestIconDisplayed (EFalse);
    
    TSatUiResponse result(ESatSuccess);
    ServerDispatcherHelper helper;
    connect(mServer,SIGNAL(selectItem(SatAppAction&)),&helper,SLOT(exec(SatAppAction&)));
    connect(mServer,SIGNAL(selectItem(SatAppAction&)),
        &helper,SLOT(simulateItemSelected(SatAppAction&)));
    QT_TRAP_THROWING(result = mServer->SelectItemL(
                              aText,
                              *aMenuItems,
                              aMenuItemNextActions,
                              defaultItem,
                              aSelectedItem,
                              isHelpIsAvailable,
                              aIconId,
                              aMenuIcons,
                              aIconListQualifier,
                              aRequestIconDisplayed,
                              aSelectionPreference
                              ));
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectReturnValue);
    QCOMPARE(exValue, result);

    delete hText;
    hText = 0;
    delete aMenuItems;
    aMenuItems = 0;
    delete aMenuItemNextActions;
    aMenuItemNextActions = 0;
    delete aMenuIcons;
    aMenuIcons = 0;
    qDebug("Ut_SatAppServerDispatcher::testSelectItem <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testSetUpMenu
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testPlayTone_data()
{
    qDebug("Ut_SatAppServerDispatcher::testPlayTone_data >");
    QTest::addColumn<QString>("text");//enum 0~3
    QTest::addColumn<int>("satTone");
    QTest::addColumn<int>("duration");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<bool>("isRequestedIconDisplayed");
    QTest::addColumn<int>("expectReturnValue");
    QTest::newRow("Response_Failure") << "PlayStandardTone" << 100 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("ToneNotSet") << "PlayStandardTone" << -1 << 
        0 << 80<< 0 << false << 0;
    QTest::newRow("DialTone_0") << "PlayStandardTone" << 1 << 
        0 << 80<< 1 << false << 0;
    QTest::newRow("DialTone_2000") << "PlayStandardTone" << 1 << 
        2000 << 80 << 2 << false << 0;
    QTest::newRow("DialTone_true") << "PlayStandardTone" << 1 << 
        2000 << 80 << 3 << true << 0;
    QTest::newRow("Busy") << "PlayStandardTone" << 2 << 
        2000 << 80 << 3 << false << 0;
    QTest::newRow("Congestion") << "PlayStandardTone" << 3 << 
        25000 << 80 << 3 << false << 0;
    QTest::newRow("Congestion_0") << "PlayStandardTone" << 3 << 
        0 << 80 << 3 << false << 0;
    QTest::newRow("Acknowledge ") << "PlayStandardTone" << 4 << 
        25000 << 80 << 3 << false << 0;
    QTest::newRow("Acknowledge_0") << "PlayStandardTone" << 4 << 
        0 << 80 << 3 << false << 0;
    QTest::newRow("CallDropped") << "PlayStandardTone" << 5 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("CallDropped_0") << "PlayStandardTone" << 5 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("SpecialInfo") << "PlayStandardTone" << 6 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("SpecialInfo_0") << "PlayStandardTone" << 6 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("WaitingTone") << "PlayStandardTone" << 7 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("WaitingTone_0") << "PlayStandardTone" << 7 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("RingingTone") << "PlayStandardTone" << 8 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("RingingTone_0") << "PlayStandardTone" << 8 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("GeneralBeep") << "PlayStandardTone" << 16 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("GeneralBeep_0") << "PlayStandardTone" << 16 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("PositiveTone") << "PlayStandardTone" << 17 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("PositiveTone_0") << "PlayStandardTone" << 17 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("NegativeTone") << "PlayStandardTone" << 18 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("NegativeTone_0") << "PlayStandardTone" << 18 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("Speech") << "PlayStandardTone" << 19 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("Speech_0") << "PlayStandardTone" << 19 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("Sms") << "PlayStandardTone" << 20 << 
        25000 << 80 << 1 << false << 0;
    QTest::newRow("Sms_0") << "PlayStandardTone" << 20 << 
        0 << 80 << 1 << false << 0;
    QTest::newRow("Busy_0") << "PlayStandardTone" << 2 << 
        0 << 80 << 3 << false << 0;
    qDebug("Ut_SatAppServerDispatcher::testPlayTone_data <");
}


// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testSetUpMenu
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testPlayTone()
{
    qDebug("Ut_SatAppServerDispatcher::testPlayTone >");
    QVERIFY(mServer);
    QFETCH(QString, text);
    QFETCH(int, duration);
    QFETCH(int, satTone);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(bool, isRequestedIconDisplayed);
    QFETCH(int, expectReturnValue);
    qDebug("Ut_SatAppServerDispatcher::testPlayTone after fetch");
    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // aTone
    TSatTone aTone = static_cast<TSatTone>(satTone);
    // aDuration
    TUint aDuration= static_cast<TUint>(duration);
    // aIconId
    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    // aRequestedIconDisplayed
    TBool aRequestedIconDisplayed(isRequestedIconDisplayed);
    qDebug("Ut_SatAppServerDispatcher::testPlayTone call observer");
    ServerDispatcherHelper helper;
    connect(mServer,SIGNAL(playTone(SatAppAction&)),&helper,SLOT(exec(SatAppAction&)));
    TSatUiResponse result(ESatSuccess);
    result = mServer->PlayTone(
                              aText,
                              aTone,
                              aDuration,
                              aIconId,
                              aRequestedIconDisplayed
                              );
    TSatUiResponse exValue = static_cast<TSatUiResponse>(expectReturnValue);
    QCOMPARE(exValue, result);
    delete hText;
    hText = 0;
    qDebug("Ut_SatAppServerDispatcher::testPlayTone <");
}


// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testConfirmCommand_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testConfirmCommand_data()
{
    qDebug("Ut_SatAppServerDispatcher::testConfirmCommand_data >");
    QTest::addColumn<int>("commandId");// enum 0~6
    QTest::addColumn<int>("alphaIdStatus");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("additionalText");
    QTest::addColumn<bool>("isctionAccepted");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<bool>("isRequestedIconDisplayed");
    QTest::addColumn<bool>("isterminatedByUser");
    // test commandId ESatSSendSmQuery
    QTest::newRow("first") << 0 << 1 << "ConfirmCommand SendSm" 
        << "SendSm" <<false << 60 << 1 << false <<false;
    // test commandId ESatSSendSsQuery
    QTest::newRow("second") << 1 << 1 << "ConfirmCommand SendSs" 
        << "SendSs" <<false << 60 << 1 << false <<false;
    // test commandId ESatSSendUssdQuery
    QTest::newRow("third") << 2 << 1 << "ConfirmCommand SendUssd" 
        << "SendUssd" <<false << 60 << 1 << false <<false;
    // test commandId ESatSRefreshQuery
    QTest::newRow("fourth") << 3 << 1 << "ConfirmCommand Refresh" 
        << "First test case" <<false << 60 << 1 << false <<false;
    // test commandId ESatSLaunchBrowserQuery
    QTest::newRow("fifth") << 4 << 1 << "ConfirmCommand LaunchBrowser" 
        << "LaunchBrowser" <<false << 60 << 1 << false <<false;
    // test commandId ESatOpenChannelQuery
    QTest::newRow("sixth") << 5 << 1 << "ConfirmCommand OpenChannel" 
        << "OpenChanne" <<false << 60 << 1 << false <<false;
    // test commandId ESatSSetUpCallQuery
    QTest::newRow("seventh") << 6 << 1 << "ConfirmCommand SetUpCall" 
        << "SetUpCall" <<false << 60 << 1 << false <<false;
    // test commandId ESatSelfExplanatory
    QTest::newRow("eighth") << 0 << 1 << "ConfirmCommand SendSm" 
        << "SendSm SelfExplanatory" <<false << 60 << 2 << true <<false;

    qDebug("Ut_SatAppServerDispatcher::testConfirmCommand_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testConfirmCommand
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testConfirmCommand()
{
    qDebug("Ut_SatAppServerDispatcher::testConfirmCommand >");
    QVERIFY(mServer);
    QFETCH(int, commandId);
    QFETCH(int, alphaIdStatus);
    QFETCH(QString, text);
    QFETCH(QString, additionalText);
    QFETCH(bool, isctionAccepted);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(bool, isRequestedIconDisplayed);
    QFETCH(bool, isterminatedByUser);

    // aCharacterSet
    TSatSQueryCommand aCommandId = static_cast<TSatSQueryCommand>(commandId);
    // aAlphaIdStatus
    TSatAlphaIdStatus aAlphaIdStatus = static_cast<TSatAlphaIdStatus>(alphaIdStatus);
    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );
    // aAdditionalText
    HBufC* hAdditionalText = HBufC::New(additionalText.length());
    TPtr aAdditionalText( hAdditionalText->Des() );
    aAdditionalText.Copy( additionalText.utf16() );
    // aActionAccepted
    TBool aActionAccepted(isctionAccepted);
    // aIconId
    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    // aRequestedIconDisplayed
    TBool aRequestedIconDisplayed(isRequestedIconDisplayed);
    // aRequestedIconDisplayed
    TBool aTerminatedByUser(isterminatedByUser);
    ServerDispatcherHelper helper;
    connect(mServer,SIGNAL(confirmCommand(SatAppAction&)),&helper,SLOT(exec(SatAppAction&)));
    mServer->ConfirmCommand(
                              aCommandId,
                              aAlphaIdStatus,
                              aText, 
                              aAdditionalText,
                              aActionAccepted,
                              aIconId,
                              aRequestedIconDisplayed,
                              aTerminatedByUser
                              );
    delete hText;
    hText = 0;
    delete hAdditionalText;
    hAdditionalText = 0;
    qDebug("Ut_SatAppServerDispatcher::testConfirmCommand <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testNotification_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testNotification_data()
{
    qDebug("Ut_SatAppServerDispatcher::testNotification_data >");
    QTest::addColumn<int>("commandId");// enum 0~6
    QTest::addColumn<int>("alphaIdStatus");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("iconIdIdentifier");
    QTest::addColumn<int>("iconIdQualifier");//enum 0~3
    QTest::addColumn<bool>("isRequestedIconDisplayed");
    QTest::addColumn<int>("controlResult");
 
    // test commandId ESatSSendDataNotify    
    QTest::newRow("Senddata_0id") << 0 << 0 << "Send data notify" 
    << 2 << 0 << false <<0;
    // test commandId ESatSSendDataNotify alpha id not null but string length is zero   
    QTest::newRow("Senddata_1id") << 0 << 1 << "" 
    << 2 << 0 << false <<0;
    QTest::newRow("Senddata_2id") << 0 << 2 << "Send data notify AlphaIdNotProvided" 
    << 2 << 0 << false <<0;
    // test commandId ESatSReceiveDataNotify
    QTest::newRow("Recievedata_0id") << 1 << 0 << "Recieve data notify" 
    << 2 << 3 << false <<0;
    QTest::newRow("Recievedata_1id") << 1 << 1 << "Recieve data notify" 
    << 2 << 3 << false <<0;
    QTest::newRow("Recievedata_2id") << 1 << 2 << "Recieve data notify AlphaIdNotProvided" 
    << 2 << 3 << false <<0;
    // test commandId ESatSCloseChannelNotify
    QTest::newRow("CloseChannel_0id") << 2 << 0 << "Close channel notify" 
    << 2 << 3 << false <<0;
    QTest::newRow("CloseChannel_1id") << 2 << 1 << "" 
    << 2 << 3 << false <<0;
    QTest::newRow("CloseChannel_2id") << 2 << 2 << "Close channel notify AlphaIdNotProvided" 
    << 2 << 3 << false <<0;
    // test commandId ESatSMoSmControlNotify
    QTest::newRow("MoSm_0id") << 3 << 0 << "SM control notify" 
    << 2 << 0 << false <<0;
    QTest::newRow("MoSm_1id") << 3 << 1 << "0" 
    << 2 << 0 << false <<0;
    QTest::newRow("MoSm_2id") << 3 << 2 << "SM control notify AlphaIdNotProvided" 
    << 2 << 0 << false <<0;
    // test commandId ESatSCallControlNotify
    QTest::newRow("CallControl_0id") << 4 << 0 << "Call control notify" 
    << 2 << 1 << false <<0;
    QTest::newRow("CallControl_1id") << 4 << 1 << ""
    << 2 << 1 << false <<0;
    QTest::newRow("CallControl_2id") << 4 << 2 << "Call control notify AlphaIdNotProvided" 
    << 2 << 1 << false <<0;
    // test commandId ESatSSendSsNotify
    QTest::newRow("SendSs_0id") << 5 << 0 << "Send SS notify" 
    << 2 << 2 << false <<0;
    QTest::newRow("SendSs_1id") << 5 << 1 << "" 
    << 2 << 2 << false <<0;
    QTest::newRow("SendSs_2id") << 5 << 2 << "Send SS notify AlphaIdNotProvided" 
    << 2 << 2 << false <<0;
    // test commandId ESatSSendUssdNotify
    QTest::newRow("SendUssd_0id") << 6 << 0 << "Send Ussd notify" 
    << 2 << 3 << false <<0;
    // test commandId ESatSSendUssdNotify
    QTest::newRow("SendUssd_1id") << 6 << 1 << "" 
    << 2 << 3 << false <<0;
    // test commandId ESatSSendUssdNotify
    QTest::newRow("SendUssd_2id") << 6 << 2 << "Send Ussd notify AlphaIdNotProvided" 
    << 2 << 3 << false <<0;
    // test commandId ESatSSendDtmfNotify
    QTest::newRow("SendDtmf_0id") << 7 << 0 << "Send dtmf notify" 
    << 2 << 3 << false <<0;
    QTest::newRow("SendDtmf_1id") << 7 << 1 << "Send dtmf notify" 
    << 2 << 3 << false <<0;
    QTest::newRow("SendDtmf_2id") << 7 << 2 << "Send dtmf notify AlphaIdNotProvided" 
    << 2 << 3 << false <<0;
    // test commandId ESatSSendSmsNotify
    QTest::newRow("SendSm_0id") << 8 << 0 << "SendSm notify" 
    << 2 << 2 << false <<0;
    QTest::newRow("SendSm_1id") << 8 << 1 << "SendSm notify" 
    << 2 << 2 << false <<0;
    QTest::newRow("SendSm_2id") << 8 << 2 << "SendSm notify AlphaIdNotProvided" 
    << 2 << 2 << false <<0;
    qDebug("Ut_SatAppServerDispatcher::testNotification_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testNotification
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testNotification()
{
    qDebug("Ut_SatAppServerDispatcher::testNotification >");
    QVERIFY(mServer);
    QFETCH(int, commandId);
    QFETCH(int, alphaIdStatus);
    QFETCH(QString, text);
    QFETCH(int, iconIdIdentifier);
    QFETCH(int, iconIdQualifier);
    QFETCH(bool, isRequestedIconDisplayed);
    QFETCH(int, controlResult);

    // aCommandId
    TSatSNotifyCommand aCommandId = static_cast<TSatSNotifyCommand>(commandId);
    // aAlphaIdStatus
    TSatAlphaIdStatus aAlphaIdStatus = static_cast<TSatAlphaIdStatus>(alphaIdStatus);
    // text
    HBufC* hText = HBufC::New(text.length());
    TPtr aText( hText->Des() );
    aText.Copy( text.utf16() );

    // aIconId
    TSatIconId aIconId;
    aIconId.iIdentifier = iconIdIdentifier;
    aIconId.iIconQualifier = static_cast<TSatIconQualifier>(iconIdQualifier);
    // aRequestedIconDisplayed
    TBool aRequestedIconDisplayed(isRequestedIconDisplayed);
    // aRequestedIconDisplayed
    TSatControlResult aControlResult = static_cast<TSatControlResult>(controlResult);
    ServerDispatcherHelper helper;
    connect(mServer,SIGNAL(notification(SatAppAction&)),&helper,SLOT(exec(SatAppAction&)));
    mServer->Notification(
                              aCommandId,
                              aAlphaIdStatus,
                              aText, 
                              aIconId,
                              aRequestedIconDisplayed,
                              aControlResult
                              );
    delete hText;
    hText = 0;
    qDebug("Ut_SatAppServerDispatcher::testNotification <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testEventNotification_data
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testEventNotification_data()
{
    qDebug("Ut_SatAppServerDispatcher::testEventNotification_data >");
    QTest::addColumn<int>("eventId");// enum 0~6
    
    // test eventId ESatSRefreshStartEvent    
    QTest::newRow("RefreshStartEvent") << 0 ;
    // test eventId ESatSRefreshEndEvent
    QTest::newRow("RefreshEndEvent") << 1;
    // test eventId ESatSSmEndEvent
    QTest::newRow("SSmEndEvent") << 2;
    // test eventId ESatSClearScreenEvent
    QTest::newRow("ClearScreenEvent") << 3;
    // test eventId ESatSCloseSatUiAppEvent
    QTest::newRow("CloseSatUiAppEvent") << 4;
    // test eventId ESatSsEndEvent
    QTest::newRow("SsEndEvent") << 5;
    // test eventId ESatSsErrorEvent
    QTest::newRow("SsErrorEvent") << 6;
    // test eventId ESatSDtmfEndEvent
    QTest::newRow("DtmfEndEvent") << 7;

    qDebug("Ut_SatAppServerDispatcher::testEventNotification_data <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppServerDispatcher::testEventNotification
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppServerDispatcher::testEventNotification()
{
    qDebug("Ut_SatAppServerDispatcher::testEventNotification >");
    QVERIFY(mServer);
    QFETCH(int, eventId);

    TSatSEvent aEventId = static_cast<TSatSEvent>(eventId);
    TSatSEventStatus aEventStatus = ESatEventCompleteOk;
    mServer->EventNotification(aEventId, aEventStatus, 0);
    qDebug("Ut_SatAppServerDispatcher::testEventNotification <");
}


void Ut_SatAppServerDispatcher::testMenuSelection()
{
    // menu selection 1
    SatAppAction* a = new SatAppAction(ESatSetUpMenuAction,mServer);
    a->set(KeySelection,4);
    a->set(KeyHelpRequested,false);
    connect(a,SIGNAL(actionCompleted(SatAppAction*)),mServer,SLOT(menuSelection(SatAppAction*)));
    a->completeWithSuccess();

    // menu selection 2 - with different params
    SatAppAction* b = new SatAppAction(ESatSetUpMenuAction,mServer);
    b->set(KeySelection,4);
    b->set(KeyHelpRequested,false);
    connect(b,SIGNAL(actionCompleted(SatAppAction*)),mServer,SLOT(menuSelection(SatAppAction*)));
    b->completeWithSuccess();

    // menu selection 3 - no user resp
    SatAppAction* c = new SatAppAction(ESatSetUpMenuAction,mServer);
    connect(c,SIGNAL(actionCompleted(SatAppAction*)),mServer,SLOT(menuSelection(SatAppAction*)));
    c->completeWithNoResponse();

    // menu selection 4 - session terminated
    SatAppAction* d = new SatAppAction(ESatSetUpMenuAction,mServer);
    connect(d,SIGNAL(actionCompleted(SatAppAction*)),mServer,SLOT(menuSelection(SatAppAction*)));
    d->completeWithSessionTerminated();

}

void Ut_SatAppServerDispatcher::testUserCancelSession()
{
    SatAppAction* a = new SatAppAction(ESatSetUpMenuAction);
    mServer->userCancelSession(a);
}


/*!
*User cancel response, Send DTMF, Send Data, Receive Data
*/

// helper class
void ServerDispatcherHelper::exec(SatAppAction& action)
{
    action.completeWithSuccess();
}

void ServerDispatcherHelper::simulateItemSelected(SatAppAction& action)
{
    action.set(KeySelection, 0);
}

// End of file
