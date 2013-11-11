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
#include <centralrepository.h>
#include <coreapplicationuissdkcrkeys.h>
#include "ut_satappmainhandler.h"
//test class
#include "satappmainhandler.h"
#include "satappconstant.h"

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::Ut_SatAppMainHandler
// -----------------------------------------------------------------------------
//
Ut_SatAppMainHandler::Ut_SatAppMainHandler(
        HbMainWindow* mainWindow, QObject *parent)
    :QObject(parent), mMainWindow(mainWindow)
{
    qDebug("Ut_SatAppMainHandler::Ut_SatAppMainHandler >");
    qDebug("Ut_SatAppMainHandler::Ut_SatAppMainHandler <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::~Ut_SatAppMainHandler
// -----------------------------------------------------------------------------
//
Ut_SatAppMainHandler::~Ut_SatAppMainHandler()
{
    qDebug("Ut_SatAppMainHandler::~Ut_SatAppMainHandler >");
    qDebug("Ut_SatAppMainHandler::~Ut_SatAppMainHandler <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::initTestCase
// QTestLib cleanup method, called after the last testfunction .
// -----------------------------------------------------------------------------
void Ut_SatAppMainHandler::initTestCase()
{
    qDebug("Ut_SatAppMainHandler::initTestCase >");
    qDebug("Ut_SatAppMainHandler::initTestCase <");
}


// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::cleanupTestCase
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppMainHandler::cleanupTestCase()
{
    qDebug("Ut_SatAppMainHandler::cleanupTestCase >");
    if (mMainHandler) {
        qDebug("Ut_SatAppMainHandler::cleanupTestCase delete mMainHandler...");
        delete mMainHandler;
        mMainHandler = 0;
    }
    qDebug("Ut_SatAppMainHandler::cleanupTestCase <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::testCreateMainHandler
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppMainHandler::testCreateMainHandler()
{
    qDebug("Ut_SatAppMainHandler::testCreateMainHandler >");
    mMainHandler = new SatAppMainHandler(*mMainWindow);
    QVERIFY(mMainHandler); 
    qDebug("Ut_SatAppMainHandler::testCreateMainHandler <");
}

// -----------------------------------------------------------------------------
// Ut_SatAppMainHandler::testCreateMainHandler_OfflineMode
// 
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Ut_SatAppMainHandler::testCreateMainHandler_OfflineMode()
{
    qDebug("Ut_SatAppMainHandler::testCreateMainHandler_OfflineMode >");
    cleanupTestCase();
    TInt profileId(0);
    CRepository* cr (NULL);
    TRAPD(err, cr = CRepository::NewL(KCRUidCoreApplicationUIs));
    if ( KErrNone == err )
    {
        // Get the ID of the currently active profile:
        TInt error = cr->Get(KCoreAppUIsNetworkConnectionAllowed, profileId);
        qDebug("Ut_SatAppMainHandler::SatAppMainHandler get active \
                profile error=%d, profileId=%d", error, profileId);
        if( KErrNone != error){
            delete cr;
            return;
        }
        error = cr->Set(KCoreAppUIsNetworkConnectionAllowed,
            ECoreAppUIsNetworkConnectionNotAllowed);
        qDebug("Ut_SatAppMainHandler::SatAppMainHandler set active \
                profile error=%d",error);
        mMainHandler = new SatAppMainHandler(*mMainWindow);
        QVERIFY(mMainHandler); 
        error = cr->Set(KCoreAppUIsNetworkConnectionAllowed, profileId);
        qDebug("Ut_SatAppMainHandler::SatAppMainHandler set active \
                profile error=%d",error);
        delete cr;
    }
    
    qDebug("Ut_SatAppMainHandler::testCreateMainHandler_OfflineMode <");
}
// End of file
