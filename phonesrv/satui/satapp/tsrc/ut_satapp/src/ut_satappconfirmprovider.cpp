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

#include "ut_satappconfirmprovider.h"
#include "satappconfirmprovider.h"
#include "satappaction.h"

// --------------------------------------------------------
// The unit tests begin here
// --------------------------------------------------------

Ut_SatAppConfirmProvider::Ut_SatAppConfirmProvider(
        QObject *parent)
    :QObject(parent)
{
}

void Ut_SatAppConfirmProvider::testConfirmCommand_data()
{
    QTest::addColumn<int>("commandId");
    QTest::addColumn<int>("alphaIdStatus");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("additionText");
    QTest::addColumn<bool>("userAccepted");
    QTest::addColumn<bool>("expectRes");


    QTest::newRow("t01") << (int)ESatOpenChannelQuery << (int)ESatAlphaIdNull 
            << "comfrm" << "addition"<< false << false;
    QTest::newRow("t02") << (int)ESatOpenChannelQuery << (int)ESatAlphaIdNotProvided
            << "comfrm" << "addition"<< true << true;
    QTest::newRow("t03") << (int)ESatOpenChannelQuery << (int)ESatAlphaIdNotProvided
            << "" << "addition"<< true << true;
    QTest::newRow("t04") << (int)ESatOpenChannelQuery << (int)ESatAlphaIdNotProvided
            << "comfrm" << ""<< true << true;
    QTest::newRow("t05") << (int)ESatSSendSmQuery << (int)ESatAlphaIdNull
            << "comfrm" << "addition"<< true << true;
    QTest::newRow("t06") << (int)ESatSSetUpCallQuery << (int)ESatAlphaIdNull
            << "comfrm" << "addition"<< true << true;
    QTest::newRow("t07") << (int)ESatSSetUpCallQuery << (int)ESatAlphaIdNull
            << "" << "addition"<< true << true;
    QTest::newRow("t08") << (int)ESatSSetUpCallQuery << (int)ESatAlphaIdNull
            << "" << ""<< true << true;
    QTest::newRow("t09") << (int)ESatSLaunchBrowserQuery << (int)ESatAlphaIdNull
            << "" << ""<< true << true;
    QTest::newRow("t10") << (int)ESatSLaunchBrowserQuery << (int)ESatAlphaIdNotProvided
            << "" << ""<< true << true;
    QTest::newRow("t11") << (int)ESatSLaunchBrowserQuery << (int)ESatAlphaIdNull
            << "" << ""<< true << true;
    QTest::newRow("t12") << 10 << (int)ESatAlphaIdNull
            << "" << ""<< true << true;
    QTest::newRow("t05") << (int)ESatSSendUssdQuery << (int)ESatAlphaIdNull
            << "comfrm" << "addition"<< true << true;
    QTest::newRow("t05") << (int)ESatSSendSsQuery << (int)ESatAlphaIdNull
            << "comfrm" << "addition"<< true << true;
    QTest::newRow("t05") << (int)ESatSRefreshQuery << (int)ESatAlphaIdNull
            << "comfrm" << "addition"<< true << true;
}


void Ut_SatAppConfirmProvider::testConfirmCommand()
{
    QFETCH(int, commandId);
    QFETCH(int, alphaIdStatus);
    QFETCH(QString, text);
    QFETCH(QString, additionText);
    QFETCH(bool, userAccepted);
    QFETCH(bool, expectRes);

    SatAppAction action(ESatConfirmAction);
    action.set(KeyQueryCommandId, commandId);
    action.set(KeyAlphaIdStatus, alphaIdStatus);
    action.set(KeyText, text);
    action.set(KeyAdditionalText, additionText);
    action.set(KeyActionAccepted, userAccepted);

    ConfirmProviderTest test;
    QTimer::singleShot(1000, &test, SLOT(simulateFirstActionClicked()));
    test.mConfirm->confirmCommand(action);
    QCOMPARE(userAccepted,expectRes);
}

void Ut_SatAppConfirmProvider::testConfirmFirstAction()
{
    ConfirmProviderTest test;

    SatAppAction action(ESatConfirmAction);
    action.set(KeyQueryCommandId, ESatOpenChannelQuery);
    action.set(KeyAlphaIdStatus, ESatAlphaIdNull);
    action.set(KeyText, "OpenChannel");
    action.set(KeyAdditionalText, "yes");
    test.mConfirm->confirmCommand(action);
    QTimer::singleShot(1000, &test, SLOT(simulateFirstActionClicked()));
    action.waitUntilCompleted();
    QCOMPARE(action.response(), ESatSuccess);
}

void Ut_SatAppConfirmProvider::testConfirmSecondAction()
{
    ConfirmProviderTest test;

    SatAppAction action(ESatConfirmAction);
    action.set(KeyQueryCommandId, ESatOpenChannelQuery);
    action.set(KeyAlphaIdStatus, ESatAlphaIdNull);
    action.set(KeyText, "OpenChannel");
    action.set(KeyAdditionalText, "No");
    test.mConfirm->confirmCommand(action);
    QTimer::singleShot(1000, &test, SLOT(simulateSecondActionClicked()));
    action.waitUntilCompleted();
    QCOMPARE(action.response(), ESatBackwardModeRequestedByUser);
}


void Ut_SatAppConfirmProvider::testConfirmAndClearScreen()
{
    ConfirmProviderTest test;

    SatAppAction action(ESatConfirmAction);
    action.set(KeyQueryCommandId, ESatOpenChannelQuery);
    action.set(KeyAlphaIdStatus, ESatAlphaIdNull);
    action.set(KeyText, "OpenChannel");
    action.set(KeyAdditionalText, "ClearScreen");
    test.mConfirm->confirmCommand(action);
    QTimer::singleShot(1000, &test, SLOT(simulateClearScreen()));
    action.waitUntilCompleted();
    QCOMPARE(action.response(), ESatNoResponseFromUser);

    action.set(KeyQueryCommandId, ESatSSetUpCallQuery);
    action.set(KeyText, "SetUpCall");
    test.mConfirm->confirmCommand(action);
    QTimer::singleShot(1000, &test, SLOT(simulateClearScreen()));
    action.waitUntilCompleted();
    QCOMPARE(action.response(), ESatNoResponseFromUser);
}

// ---------------------------------------------------------------
// internal helper class
// ---------------------------------------------------------------

ConfirmProviderTest::ConfirmProviderTest()
{
    mConfirm = new SatAppConfirmProvider(this);
}

ConfirmProviderTest::~ConfirmProviderTest()
{
    cleanup();
}

void ConfirmProviderTest::simulateFirstActionClicked()
{
    if (mConfirm->mConfirmQuery)
    mConfirm->mConfirmQuery->actions().at(0)->trigger();
}

void ConfirmProviderTest::simulateSecondActionClicked()
{
    if (mConfirm->mConfirmQuery)
    mConfirm->mConfirmQuery->actions().at(1)->trigger();
    
}

void ConfirmProviderTest::simulateClearScreen()
{
    if (mConfirm->mConfirmQuery) mConfirm->clearScreen();
}

void ConfirmProviderTest::cleanup()
{
    delete mConfirm;
    mConfirm = 0;
}

// End of file
