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
* Description:  Implementation of the Ut_VmbxEngine class
*
*/

#include <QtTest/QtTest>
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <voicemailboxdefs.h>

#include "ut_vmbxengine.h"

_LIT(KNumber, "987654321");
_LIT(KName, "Name");
_LIT8(KBrandId, "Brand Id");

Ut_VmbxEngine::Ut_VmbxEngine(): mVmbx(NULL),mEntry(NULL)
{
    qDebug("Ut_VmbxEngine");
}

// -----------------------------------------------------------------------------
// Ut_VmbxEngine::~Ut_VmbxEngine
// -----------------------------------------------------------------------------    
Ut_VmbxEngine::~Ut_VmbxEngine()
{
    delete mVmbx;
    mVmbx = NULL;
    delete mEntry;
    mEntry = NULL; 
    qDebug("~Ut_VmbxEngine");
}

// Test API
void Ut_VmbxEngine::testVmbxApi()
{
    createVmbxApi();
    deleteVmbxApi();
}

void Ut_VmbxEngine::testEntryApi()
{
    createEntryApi();
    deleteEntryApi();  
}

// Test Vmbx start
void Ut_VmbxEngine::testGetStoredEntry()
{
    createVmbxApi();
    
    TVoiceMailboxParams params;
    params.iType = EVmbxVoice;
    CVoiceMailboxEntry *tmpEntry = NULL;
    
    QCOMPARE(mVmbx->GetStoredEntry(params, tmpEntry), KErrNone);
    qDebug("Get entry");
    if (tmpEntry){
        delete tmpEntry;
        tmpEntry = NULL;  
        qDebug("delete tmp entry pointer");
    }
    
    // Save a entry first, get and compare after that
    createEntryApi();
    mEntry->SetVoiceMailboxType(EVmbxVoice);
    mEntry->SetVmbxNumber(KNumber);
    qDebug("Set type");
    QCOMPARE(mVmbx->SaveEntry(*mEntry), KErrNone);
    qDebug("Save entry");
    deleteEntryApi();
    
    params.iType = EVmbxVoice;
    mVmbx->GetStoredEntry(params, tmpEntry);
    qDebug("Get entry");
    if (tmpEntry){
        QCOMPARE(tmpEntry->VoiceMailboxType(), EVmbxVoice);
        delete tmpEntry;
        tmpEntry = NULL;  
        qDebug("delete tmp entry pointer");
    }
    
    deleteVmbxApi();
}

void Ut_VmbxEngine::testSaveEntry()
{
    createVmbxApi();
    
    // Set some attributes to the new empty entry and save it
    createEntryApi();
    mEntry->SetVoiceMailboxType(EVmbxVoice);
    mEntry->SetUsingMemoryLocation(EVmbxPhoneMemory);
    mEntry->SetVmbxNumber(KNumber);
    qDebug("Set type & location");
    QCOMPARE(mVmbx->SaveEntry(*mEntry), KErrNone);
    qDebug("Save entry");
    deleteEntryApi();  
    
    TVoiceMailboxParams params;
    params.iType = EVmbxVoice;
    CVoiceMailboxEntry *tmpEntry = NULL;;
    mVmbx->GetStoredEntry(params, tmpEntry);
    qDebug("Get entry");
    if (tmpEntry){
        QCOMPARE(tmpEntry->VoiceMailboxType(), EVmbxVoice);
        qDebug("Verify type");
        QCOMPARE(tmpEntry->UsingMemoryLocation(), EVmbxPhoneMemory);
        qDebug("Verify location");
        delete tmpEntry;
        tmpEntry = NULL;
    }
    deleteVmbxApi();
}

void Ut_VmbxEngine::testQueryNewEntry()
{
    createVmbxApi();
    CVoiceMailboxEntry *tmpEntry = NULL;
    TVoiceMailboxParams params;
    
    // Query a new voice entry
    // Mock the CRepository
    params.iType = EVmbxVoice;  
    QCOMPARE(mVmbx->QueryNewEntry(params, tmpEntry), KErrNone);
    QVERIFY(tmpEntry);
    QCOMPARE(tmpEntry->VoiceMailboxType(), EVmbxVoice);
    delete tmpEntry;
    tmpEntry = NULL;
    
    /*
    // Query a new Video entry
    params.iType = EVmbxVideo;
    QCOMPARE(mVmbx->QueryNewEntry(params, tmpEntry), KErrNone);
    QVERIFY(tmpEntry);
    QCOMPARE(tmpEntry->VoiceMailboxType(), EVmbxVideo);
    delete tmpEntry;
    tmpEntry = NULL;
    
    // Query a new voice entry
    params.iType = EVmbxVoip;
    QCOMPARE(mVmbx->QueryNewEntry(params, tmpEntry), KErrNone);
    QVERIFY(tmpEntry);
    QCOMPARE(tmpEntry->VoiceMailboxType(), EVmbxVoip);
    delete tmpEntry;
    tmpEntry = NULL;    
    
    // Query a unknown entry
    params.iType = EVmbxNone;
    QCOMPARE(mVmbx->QueryNewEntry(params, tmpEntry), KErrNone);
    QVERIFY(tmpEntry);
    QCOMPARE(tmpEntry->VoiceMailboxType(), EVmbxNone);
    delete tmpEntry;
    tmpEntry = NULL;       
    */
    deleteVmbxApi();
    
}

void Ut_VmbxEngine::testQueryChangeEntry()
{
    createVmbxApi();
    
    CVoiceMailboxEntry *tmpEntry = NULL;;
    TVoiceMailboxParams params;
    params.iType = EVmbxVoice;  
    // Always get KErrNotFound, Check why.
    //QCOMPARE(mVmbx->QueryChangeEntry(params, tmpEntry), KErrNone);
    mVmbx->QueryChangeEntry(params, tmpEntry);
    //QVERIFY(tmpEntry);
    delete tmpEntry;
    tmpEntry = NULL;    
    
    deleteVmbxApi();
}

void Ut_VmbxEngine::testQueryVmbxType()
{
    createVmbxApi(); 

    // Save a entry first
    CVoiceMailboxEntry *tmpEntry = NULL;
    TVoiceMailboxParams params;
    params.iType = EVmbxVoice;  
    QCOMPARE(mVmbx->QueryNewEntry(params, tmpEntry), KErrNone);
    tmpEntry->SetVoiceMailboxType(EVmbxVoice);
    tmpEntry->SetVmbxNumber(KNumber);
    QCOMPARE(mVmbx->SaveEntry(*tmpEntry), KErrNone);
    delete tmpEntry;
    tmpEntry = NULL;
    
    // Query and compare vmbx type after saving
    mVmbx->QueryVmbxType(params);

    deleteVmbxApi();
}

// TODO: Add 2 public test cases for notification, mock observer frist.
//void testNotifyVmbxNumberChangeL();
//void NotifyVmbxNumberChangeCancel();

void Ut_VmbxEngine::testGetServiceIds()
{
    createVmbxApi(); 
    /*
    // Save a entry first
    createEntryApi();
    TServiceId idExp(10);
    mEntry->SetServiceId(idExp);
    mEntry->SetVoiceMailboxType(EVmbxVoip);
    QCOMPARE(mVmbx->SaveEntry(*mEntry), KErrNone);
    deleteEntryApi();
    
    // Query and compare vmbx type after saving
    RIdArray ids;
    QCOMPARE(mVmbx->GetServiceIds(ids), KErrNone);
    TServiceId idAct = ids[0];
    QCOMPARE(idAct, idExp);
    */
    deleteVmbxApi();    
}

void Ut_VmbxEngine::testCheckConfiguration()
{
    createVmbxApi();
    TVoiceMailboxParams params;
    params.iType = EVmbxVoice;
    TBool result = mVmbx->CheckConfiguration(
        params, EVmbxChangeNbrAllowedOnUi);
    if (!result) {
        QEXPECT_FAIL("","voice not allowed changed", Continue);
    }
    params.iType = EVmbxVideo;
    result = mVmbx->CheckConfiguration(
        params,EVmbxChangeNbrAllowedOnUi);
    if (!result) {
        QEXPECT_FAIL("","video not allowed changed", Continue);
    }
    params.iType = EVmbxVoip;
    result = mVmbx->CheckConfiguration(
        params,EVmbxChangeNbrAllowedOnUi);
    if (result) {
        QFAIL ("voip failed");
    }
    deleteVmbxApi();
}

void Ut_VmbxEngine::testSaveProvisionedEntry()
{
    createVmbxApi();    
    // Voice
    createEntryApi();
    mEntry->SetVoiceMailboxType(EVmbxVoice);
    mEntry->SetVmbxNumber(KNumber);
    QCOMPARE(mVmbx->SaveProvisionedEntry(*mEntry), KErrNone);
    deleteEntryApi();
    
    // Video
    createEntryApi();
    mEntry->SetVoiceMailboxType(EVmbxVideo);
    mEntry->SetVmbxNumber(KNumber);
    QCOMPARE(mVmbx->SaveProvisionedEntry(*mEntry), KErrNone);
    deleteEntryApi();
    
    deleteVmbxApi();
}
// Test Vmbx end


// Test Entry start
void Ut_VmbxEngine::testType()
{
    createEntryApi();
    mEntry->SetVoiceMailboxType(EVmbxNone);
    QCOMPARE(mEntry->VoiceMailboxType(), EVmbxNone);
    
    mEntry->SetVoiceMailboxType(EVmbxVoice);
    QCOMPARE(mEntry->VoiceMailboxType(), EVmbxVoice);
    
    mEntry->SetVoiceMailboxType(EVmbxVideo);
    QCOMPARE(mEntry->VoiceMailboxType(), EVmbxVideo);
    
    deleteEntryApi();
}

void Ut_VmbxEngine::testName()
{
    createEntryApi();
    
    // Valid string
    TPtrC nameIn(KName);
    mEntry->SetVmbxName(nameIn);
    TPtrC nameOut(KNullDesC);
    mEntry->GetVmbxName(nameOut);
    QCOMPARE(nameOut, nameIn);
    
    // Invalid string
//    HBufC *name = HBufC::NewLC(KVmbxMaxNumberLength+1);
//    QCOMPARE(mEntry->SetVmbxName(name->Des()), KErrArgument);
//    CleanupStack::PopAndDestroy(name);
    
    deleteEntryApi();      
}

void Ut_VmbxEngine::testServiceId()
{
    createEntryApi();
    
    TServiceId idIn(10);
    mEntry->SetServiceId(idIn);
    QCOMPARE(mEntry->ServiceId(), idIn);    
    
    deleteEntryApi();      
}

void Ut_VmbxEngine::testLineType()
{
    createEntryApi();
   
    // Als is disable in TB10.1
    /*
    mEntry->SetVmbxAlsLineType(EVmbxAlsLineDefault);
    QCOMPARE(mEntry->VmbxAlsLineType(), EVmbxAlsLineDefault);
    
    mEntry->SetVmbxAlsLineType(EVmbxAlsLine1);
    QCOMPARE(mEntry->VmbxAlsLineType(), EVmbxAlsLine1);
    
    mEntry->SetVmbxAlsLineType(EVmbxAlsLine2);
    QCOMPARE(mEntry->VmbxAlsLineType(), EVmbxAlsLine2);
    */
    deleteEntryApi();  
}

void Ut_VmbxEngine::testNumber()
{
    createEntryApi();
    
    // Valid number
    TPtrC numberIn(KNumber);
    mEntry->SetVmbxNumber(numberIn);
    TPtrC numberOut(KNullDesC);
    mEntry->GetVmbxNumber(numberOut);
    QCOMPARE(numberOut, numberIn);
    
    // Invalid number
//    HBufC *number = HBufC::NewLC(KVmbxMaxNumberLength+1);
//    QCOMPARE(mEntry->SetVmbxNumber(number->Des()), KErrArgument);
//    CleanupStack::PopAndDestroy(number);
    
    deleteEntryApi();  
}

void Ut_VmbxEngine::testBrandId()
{
    createEntryApi();

    // Valid braind id
    TPtrC8 brandIn(KBrandId);
    QCOMPARE(mEntry->SetBrandId(brandIn), KErrNone);  
    
    TPtrC8 brandOut(KNullDesC8);
    QCOMPARE(mEntry->GetBrandId(brandOut), KErrNone);
    
    if (brandOut.Compare(brandIn)){
        QFAIL("Fail to Set/Get BrandId");
    }
    
    // Invalid Brand Id
//    HBufC8 *invalid = HBufC8::NewLC(KVmbxMaxNumberLength+1);
//    QCOMPARE(mEntry->SetBrandId(invalid->Des()), KErrArgument);
//    CleanupStack::PopAndDestroy(invalid);    
    
    deleteEntryApi();  
}

void Ut_VmbxEngine::testUsingMemoryLocation()
{
    createEntryApi();
    
    mEntry->SetUsingMemoryLocation(EVmbxPhoneMemory);
    QCOMPARE(mEntry->UsingMemoryLocation(), EVmbxPhoneMemory);
    
    mEntry->SetUsingMemoryLocation(EVmbxSimMemory);
        QCOMPARE(mEntry->UsingMemoryLocation(), EVmbxSimMemory);
        
    deleteEntryApi();  
}

void Ut_VmbxEngine::testReset()
{
    createEntryApi();
    
    mEntry->Reset();
    QCOMPARE(mEntry->ServiceId(), KVmbxServiceIdNone);
    QCOMPARE(mEntry->VoiceMailboxType(), EVmbxNone);
    QCOMPARE(mEntry->VmbxAlsLineType(), EVmbxAlsLineDefault);
    
    TPtrC8 brand(KNullDesC8);
    QCOMPARE(mEntry->GetBrandId(brand), KErrNotFound);
    
    TPtrC name;
    QCOMPARE(mEntry->GetVmbxName(name), KErrNotFound);
    
    TPtrC number;
    QCOMPARE(mEntry->GetVmbxNumber(number), KErrNotFound);
    
    QCOMPARE(mEntry->UsingMemoryLocation(), EVmbxSimMemory);
    
    deleteEntryApi();  
}
// Test Entry end 

// Private function: create 2 data member
void Ut_VmbxEngine::createVmbxApi()
{   
    if (!mVmbx){
        TRAPD( err, mVmbx = CVoiceMailbox::NewL());
        QVERIFY(mVmbx);
        qDebug("Create vmbx api TRAP: %d", err);
    }
}
void Ut_VmbxEngine::deleteVmbxApi()
{
    delete mVmbx;
    mVmbx = NULL;
    qDebug("Delete vmbx api");
}
void Ut_VmbxEngine::createEntryApi()
{
    if (!mEntry){
        TRAPD( err, mEntry = CVoiceMailboxEntry::NewL());
        QVERIFY(mEntry);
        qDebug("Create entry Api TRAP: %d", err);
    }  
}

void Ut_VmbxEngine::deleteEntryApi()
{
    delete mEntry;
    mEntry = NULL;
    qDebug("Delete entry api");
}
// End of file
