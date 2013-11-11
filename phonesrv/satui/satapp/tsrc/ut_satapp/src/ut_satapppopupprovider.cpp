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

#include <QTimer>
#include <QAction>

#include <hbmessagebox.h>
#include <hbprogressdialog.h>
#include <hbdevicemessagebox.h>

#include "ut_satapppopupprovider.h"
#include "satapppopupprovider.h"
#include "satappaction.h"



// --------------------------------------------------------
// The unit tests begin here
// --------------------------------------------------------

Ut_SatAppPopupProvider::Ut_SatAppPopupProvider(
        QObject *parent)
    :QObject(parent)
{
}


void Ut_SatAppPopupProvider::testDisplayTextAndOk_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("appName");
    QTest::addColumn<bool>("sustainedText");
    QTest::addColumn<int>("duration");

    QTest::newRow("t01") << "title" << "sat" << true << 1000;
    QTest::newRow("t02") << "" << "sat" << false << 100;
    QTest::newRow("t03") << "" << "sat" << true << 500;
    QTest::newRow("t04") << "title" << "sat" << true << 0;
    QTest::newRow("t05") << "a bit longer title that does not fit" << "" << false << 900;
    QTest::newRow("t06") << "title" << "" << false << 400;
    QTest::newRow("t07") << "" << "" << true << 150;
    QTest::newRow("t08") << "a bit longer title that does not fit" << "sat" << true << 2000;
    QTest::newRow("t09") << "title" << "sat" << false << 600;
    QTest::newRow("t10") << "" << "somewhat longer application name" << false << 200;
    QTest::newRow("t11") << "" << "" << false << 0;
    QTest::newRow("t12") << "title" << "" << true << 500;
    QTest::newRow("t13") << "title" << "somewhat longer application name" << false << 0;
    QTest::newRow("t14") << "" << "sat" << true << 700;
    QTest::newRow("t15") << "title" << "sat" << false << 3000;
    
}

void Ut_SatAppPopupProvider::testDisplayTextAndOk()
{
    QFETCH(QString, text);
    QFETCH(QString, appName);
    QFETCH(bool, sustainedText);
    QFETCH(int, duration);

    SatAppAction a(ESatDisplayTextAction);
    a.set(KeyText,text);
    a.set(KeyApplicationName,appName);
    a.set(KeySustainedText,sustainedText);
    a.set(KeyDuration,duration);
    a.set(KeyWaitUserToClear,false);
    a.setResponse(ESatFailure);

    PopupProviderTest test;
    test.mPopup->displayText(a);
    test.simulateOkClicked();
    
    QCOMPARE(a.response(),ESatSuccess);
}


void Ut_SatAppPopupProvider::testDisplayTextAndCancel_data()
{
    testDisplayTextAndOk_data(); // same test material
}

void Ut_SatAppPopupProvider::testDisplayTextAndCancel()
{
    QFETCH(QString, text);
    QFETCH(QString, appName);
    QFETCH(bool, sustainedText);
    QFETCH(int, duration);

    SatAppAction a(ESatDisplayTextAction);
    a.set(KeyText,text);
    a.set(KeyApplicationName,appName);
    a.set(KeySustainedText,sustainedText);
    a.set(KeyDuration,duration);
    a.set(KeyWaitUserToClear,false);
    a.setResponse(ESatFailure);

    PopupProviderTest test;
    test.mPopup->displayText(a);
    test.simulateCancelClicked();
    
    QCOMPARE(a.response(),ESatBackwardModeRequestedByUser);
}


void Ut_SatAppPopupProvider::testDisplayTextAndTimeout()
{
    SatAppAction a(ESatDisplayTextAction);
    a.set(KeyText,"hello world");
    a.set(KeyApplicationName,"sat");
    a.set(KeySustainedText,false);
    a.set(KeyDuration,1000);
    a.set(KeyWaitUserToClear,true);
    a.setResponse(ESatFailure);

    PopupProviderTest test;
    test.mPopup->displayText(a);
    a.waitUntilCompleted();
    QCOMPARE(a.response(),ESatNoResponseFromUser);
}

void Ut_SatAppPopupProvider::testNotificationWithCancel_data()
{
    QTest::addColumn<int>("commandId");
    QTest::addColumn<int>("alphaIdStatus");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("controlResult");
    QTest::addColumn<int>("clickState"); // clikcked->state

    QTest::newRow("t01") << (int)ESatSSendDataNotify << (int)ESatAlphaIdNotNull << "notify"
        << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t02") << (int)ESatSSendDataNotify << (int)ESatAlphaIdNull << ""
        << (int)ESatNotAllowed << (int)ESatSuccess;
    
    QTest::newRow("t03") << (int)ESatSReceiveDataNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t04") << (int)ESatSReceiveDataNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;

    QTest::newRow("t05") << (int)ESatSSendDtmfNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t06") << (int)ESatSSendDtmfNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;
}

void Ut_SatAppPopupProvider::testNotificationWithCancel()
{
    QFETCH(int, commandId);
    QFETCH(int, alphaIdStatus);
    QFETCH(QString, text);
    QFETCH(int, controlResult);
    QFETCH(int, clickState);

    SatAppAction *a = new SatAppAction(ESatNotifyAction);
    a->set(KeyCommandId,commandId);
    a->set(KeyAlphaIdStatus,alphaIdStatus);
    a->set(KeyText,text);
    a->set(KeyControlResult,controlResult);
    a->setResponse(ESatSuccess);

    PopupProviderTest test;
    test.mPopup->notification(*a);
    qDebug() << "action response" << a->response() << "/" << clickState;
    QVERIFY(a->response()==(TSatUiResponse)clickState);
}

void Ut_SatAppPopupProvider::testNotification_data()
{
    QTest::addColumn<int>("commandId");
    QTest::addColumn<int>("alphaIdStatus");
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("controlResult");
    QTest::addColumn<int>("clickState"); // clikcked->state    

    QTest::newRow("t01") << (int)ESatSCloseChannelNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatNotAllowed << (int)ESatSuccess;
    QTest::newRow("t02") << (int)ESatSCloseChannelNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatAllowedNoModification << (int)ESatSuccess;

    QTest::newRow("t03") << (int)ESatSMoSmControlNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t04") << (int)ESatSMoSmControlNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;
    QTest::newRow("t05") << (int)ESatSMoSmControlNotify << (int)ESatAlphaIdNotProvided
        << "" << (int)ESatAllowedWithModifications << (int)ESatSuccess;

    QTest::newRow("t06") << (int)ESatSCallControlNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t07") << (int)ESatSCallControlNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;
    QTest::newRow("t08") << (int)ESatSCallControlNotify << (int)ESatAlphaIdNotProvided
        << "" << (int)ESatAllowedWithModifications << (int)ESatSuccess;

    QTest::newRow("t09") << (int)ESatSSendUssdNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t10") << (int)ESatSSendUssdNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;

    QTest::newRow("t11") << (int)ESatSSendUssdNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t12") << (int)ESatSSendUssdNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;

    QTest::newRow("t13") << (int)ESatSSendSsNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t14") << (int)ESatSSendSsNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;
    
    QTest::newRow("t15") << (int)ESatSSendSmsNotify << (int)ESatAlphaIdNotNull
        << "notify" << (int)ESatAllowedNoModification << (int)ESatSuccess;
    QTest::newRow("t16") << (int)ESatSSendSmsNotify << (int)ESatAlphaIdNull
        << "" << (int)ESatNotAllowed << (int)ESatSuccess;
}

void Ut_SatAppPopupProvider::testNotification()
{
    QFETCH(int, commandId);
    QFETCH(int, alphaIdStatus);
    QFETCH(QString, text);
    QFETCH(int, controlResult);
    QFETCH(int, clickState);

    SatAppAction a(ESatNotifyAction);
    a.set(KeyCommandId,commandId);
    a.set(KeyAlphaIdStatus,alphaIdStatus);
    a.set(KeyText,text);
    a.set(KeyControlResult,controlResult);
    a.setResponse(ESatSuccess);
    
    PopupProviderTest test;
    test.mPopup->notification(a);

    qDebug() << "action response:" << a.response() << "/" << clickState;
    QCOMPARE(a.response(),(TSatUiResponse)clickState);
}

// ---------------------------------------------------------------
// internal helper class
// ---------------------------------------------------------------

PopupProviderTest::PopupProviderTest()
{
    mPopup = new SatAppPopupProvider(this);
}

PopupProviderTest::~PopupProviderTest()
{
    cleanup();
}

bool PopupProviderTest::simulateOkClicked()
{
    if (mPopup->mDisplayText->actions().size()<=0) return false;
    mPopup->mDisplayText->actions().at(0)->trigger();
    return true;
}

bool PopupProviderTest::simulateCancelClicked()
{
    if (mPopup->mDisplayText->actions().size()<=1) return false;
    mPopup->mDisplayText->actions().at(1)->trigger();
    return true;
}

void PopupProviderTest::cleanup()
{
    delete mPopup;
    mPopup = 0;
}


// End of file
