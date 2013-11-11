/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the Mt_vmbxEngine class
*
*/

//qt
#include <QtGui>
//hb
#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbview.h>
//symbian
#include <e32base.h>

#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <voicemailboxdefs.h>
#include <mvoicemailboxobserver.h>

#include "mt_vmbxengine.h"


/*------------------------------------------------------------------------------
This module testing project links to vmbxengine.dll.
------------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Constants for verifying test case results
// -----------------------------------------------------------------------------
//
_LIT(KVmbxNumber,"123456789");

 // -----------------------------------------------------------------------------
// Mt_SatAppEngine::Mt_vmbxEngine
// Default constructor, remember to null new members here.
// -----------------------------------------------------------------------------
//
Mt_vmbxEngine::Mt_vmbxEngine()
{
    qDebug("Mt_vmbxEngine::Mt_vmbxEngine >");
    qDebug("Mt_vmbxEngine::Mt_vmbxEngine <");
}

// -----------------------------------------------------------------------------
// Mt_SatAppEngine::~Mt_SatAppEngine
// -----------------------------------------------------------------------------
Mt_vmbxEngine::~Mt_vmbxEngine()
{
    qDebug("Mt_vmbxEngine::~Mt_vmbxEngine >");
    qDebug("Mt_vmbxEngine::~Mt_vmbxEngine <");
}


// -----------------------------------------------------------------------------
// Mt_SatAppEngine::initTestCase
// QTestLib initialization method, called for each test case.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::initTestCase()
{
    qDebug("Mt_vmbxEngine::initTestCase >");
    qDebug("Mt_vmbxEngine::initTestCase <");
}


// -----------------------------------------------------------------------------
// Mt_SatAppEngine::cleanupTestCase
// QTestLib cleanup method, called for each test case.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::cleanupTestCase()
{
    qDebug("Mt_SatAppEngine::cleanupTestCase >");
    qDebug("Mt_SatAppEngine::cleanupTestCase <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testCreateVmbxEntry
// testCreateVmbxEntry test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testCreateVmbxEntry()
{
    qDebug("Mt_vmbxEngine::testCreateVmbxEntry >");
    TRAPD( err, mVmbxEntry = CVoiceMailboxEntry::NewL());
    QVERIFY2(KErrNone == err, "create CVoiceMailboxEntry failed ");
    qDebug("Mt_vmbxEngine::testCreateVmbxEntry <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testCreateVmbxMailbox
// CreateVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testCreateVmbxMailbox()
{
    qDebug("Mt_vmbxEngine::testCreateVmbxMailbox >");
    TRAPD( err, mVmbxEngine = CVoiceMailbox::NewL());
    QVERIFY2(KErrNone == err, "create CVoiceMailbox failed ");
    qDebug("Mt_vmbxEngine::testCreateVmbxMailbox <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testCheckConfiguration
// CreateVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testCheckConfiguration()
{
    qDebug("Mt_vmbxEngine::testCheckConfiguration >");
    TVoiceMailboxParams params;
    params.iType = EVmbxVoice;
    TBool result = 
    mVmbxEngine->CheckConfiguration(params,EVmbxChangeNbrAllowedOnUi);
    if (!result) {
    qDebug("Mt_vmbxEngine::testCheckConfiguration: voice not allowed changed");
    QEXPECT_FAIL("","testCheckConfiguration voice not allowed changed", Continue);
    }
    params.iType = EVmbxVideo;
    result = 
    mVmbxEngine->CheckConfiguration(params,EVmbxChangeNbrAllowedOnUi);
    if (!result) {
    qDebug( "Mt_vmbxEngine::testCheckConfiguration: video not allowed changed");
    QEXPECT_FAIL("","testCheckConfiguration video not allowed changed", Continue);
    }
    params.iType = EVmbxVoip;
    result = 
    mVmbxEngine->CheckConfiguration(params,EVmbxChangeNbrAllowedOnUi);
    if (result) {
    QFAIL ("testCheckConfiguration voip failed");
    }
    qDebug("Mt_vmbxEngine::testCheckConfiguration <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testCreateWindow
// testCreateWindow test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testCreateWindow()
{
    qDebug("Mt_vmbxEngine::testCreateWindow >");
    mWindow = new HbMainWindow();
    QVERIFY(mWindow);
    mWindow->show();
    qDebug("Mt_vmbxEngine::testCreateWindow <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testSaveProvisionedEntry
// testSaveProvisionedEntry test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testSaveProvisionedEntry()
{
    qDebug("Mt_vmbxEngine::testSaveProvisionedEntry >");
    QVERIFY(mVmbxEngine);
    QVERIFY(mVmbxEntry);
    TVoiceMailboxParams params;
    params.iType = EVmbxVoice;
    CVoiceMailboxEntry* storedEntry = 0;
    TInt result(0);
    if (mVmbxEngine->CheckConfiguration(params,EVmbxChangeNbrAllowedOnUi)) {
        qDebug("Mt_vmbxEngine::testSaveProvisionedEntry voice");
        result = mVmbxEngine->GetStoredEntry(params,storedEntry);
        if (KErrNotFound == result) {
            mVmbxEntry->SetVmbxNumber(KNullDesC);
        } else if(KErrNone == result) {
            mVmbxEntry->SetVmbxNumber(KVmbxNumber);
        } else {
            QFAIL("Mt_vmbxEngine::testSaveProvisionedEntry: voice \
                     GetStoredEntry failed");
        }
        mVmbxEntry->SetVoiceMailboxType(EVmbxVoice);
        result = mVmbxEngine->SaveProvisionedEntry(*mVmbxEntry);
        QCOMPARE(result, KErrNone);
    }
    params.iType = EVmbxVideo;
    if (mVmbxEngine->CheckConfiguration(params,EVmbxChangeNbrAllowedOnUi)) {
        qDebug("Mt_vmbxEngine::testSaveProvisionedEntry video");
        result = mVmbxEngine->GetStoredEntry(params,storedEntry);
        if (KErrNotFound == result) {
            mVmbxEntry->SetVmbxNumber(KNullDesC);
        } else if (KErrNone == result){
            TPtrC vmbxNumber(KNullDesC);
            storedEntry->GetVmbxNumber(vmbxNumber);
            mVmbxEntry->SetVmbxNumber(vmbxNumber);
        } else {
            QFAIL("Mt_vmbxEngine::testSaveProvisionedEntry video \
                    GetStoredEntry failed");
        }
        mVmbxEntry->SetVoiceMailboxType(EVmbxVideo);
        result = mVmbxEngine->SaveProvisionedEntry(*mVmbxEntry);
        QCOMPARE(result, KErrNone);
    }
    delete storedEntry;
    storedEntry = 0;
    qDebug("Mt_vmbxEngine::testSaveProvisionedEntry <");    
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testQueryVmbxMailbox
// CreateVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testQueryVmbxMailbox()
{
    qDebug("Mt_vmbxEngine::testQueryVmbxMailbox >");
    TVoiceMailboxParams params;
    // test QueryVmbxType
    TInt result = mVmbxEngine->QueryVmbxType(params);
    CVoiceMailboxEntry* vmbxEntry = 0;
    if (KErrNotFound == result) {
        qDebug("Mt_vmbxEngine::testQueryVmbxMailbox no number defined");
        // test QueryNewEntry
        result = mVmbxEngine->QueryNewEntry( params, vmbxEntry );
        //QVERIFY2(KErrNone == result, "QueryNewEntry Failed.");
        if (mVmbxEngine->CheckConfiguration(params,EVmbxChangeNbrAllowedOnUi)
            && KErrNone == result) {
        // test SaveEntry
        result = mVmbxEngine->SaveEntry( *vmbxEntry );
        QVERIFY2(KErrNone == result, "SaveEntry Failed.");
        }
    } else if (KErrNone == result) {
        qDebug("Mt_vmbxEngine::testQueryVmbxMailbox change number");
        // test change entry
        result = mVmbxEngine->QueryChangeEntry( params, vmbxEntry );
        //QVERIFY2(KErrNone == result, "QueryChangeEntry Failed.");
        if (mVmbxEngine->CheckConfiguration(params,EVmbxChangeNbrAllowedOnUi) 
            && KErrNone == result ) {
            result = mVmbxEngine->SaveEntry( *vmbxEntry );
            QVERIFY2(KErrNone == result, "SaveEntry Failed.");
            // test GetStoredEntry
            CVoiceMailboxEntry* storedEntry = 0;
            result = mVmbxEngine->GetStoredEntry(params,storedEntry);
            QVERIFY2(KErrNone == result, "GetStoredEntry Failed.");
            TPtrC storedNumber(KNullDesC);
            result = storedEntry->GetVmbxNumber(storedNumber);
            QVERIFY2(KErrNone == result, "GetVmbxNumber Failed.");
            TPtrC vmbxNumber(KNullDesC);
            vmbxEntry->GetVmbxNumber(vmbxNumber);
            QVERIFY2(storedNumber.Compare(vmbxNumber)==0, "number not match");
            delete storedEntry;
            storedEntry = 0;
        }
    } else {
        QFAIL("Mt_vmbxEngine::testQueryVmbxMailbox: QueryVmbxType failed");
    }
    delete vmbxEntry;
    vmbxEntry = 0;
    qDebug("Mt_vmbxEngine::testQueryVmbxMailbox <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testNotifyVmbxNumberChange
// testNotifyVmbxNumberChange test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testNotifyVmbxNumberChangeCancel()
{
    qDebug("Mt_vmbxEngine::testNotifyVmbxNumberChangeCancel >");
    mVmbxEngine->NotifyVmbxNumberChangeCancel();
    qDebug("Mt_vmbxEngine::testNotifyVmbxNumberChangeCancel <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testDeleteWindow
// testDeleteWindow test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testDeleteWindow()
{
    qDebug("Mt_vmbxEngine::testDeleteWindow >");
    delete mWindow;
    mWindow = 0;
    qDebug("Mt_vmbxEngine::testDeleteWindow <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testDeleteVmbxMailbox
// testDeleteVmbxMailbox test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testDeleteVmbxMailbox()
{
    qDebug("Mt_vmbxEngine::testDeleteVmbxMailbox >");
    delete mVmbxEngine;
    mVmbxEngine = 0;
    qDebug("Mt_vmbxEngine::testDeleteVmbxMailbox <");
}

// -----------------------------------------------------------------------------
// Mt_vmbxEngine::testDeleteVmbxEntry
// testDeleteVmbxEntry test case
// Connects to test object signal and verifies received data.
// -----------------------------------------------------------------------------
void Mt_vmbxEngine::testDeleteVmbxEntry()
{
    qDebug("Mt_vmbxEngine::testDeleteVmbxEntry >");
    delete mVmbxEntry;
    mVmbxEntry = 0;
    qDebug("Mt_vmbxEngine::testDeleteVmbxEntry <");
}

// -----------------------------------------------------------------------------
// main()
// Main method implemented for directing test output to a file.
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    qDebug("Mt_vmbxEngine.cpp: main() >");
    QApplication app(argc, argv);

    qDebug("Mt_vmbxEngine.cpp: Mt_vmbxEngine");
    Mt_vmbxEngine tc; 
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\vmbx\\mt_vmbxengine.txt";
    const int result = QTest::qExec(&tc, 3, pass);

    qDebug("Mt_vmbxEngine.cpp: main() <, result=%d", result);
    return result;
    } 

//End file
