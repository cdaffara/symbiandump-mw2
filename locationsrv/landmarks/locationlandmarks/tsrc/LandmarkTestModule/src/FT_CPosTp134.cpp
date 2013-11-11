/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*   ?description_line
*
*/


//  INCLUDES
#include "FT_CPosTp134.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmMultiDbSearch.h> 
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLMItemIterator.h>    
#include <EPos_TPosLMSortPref.h>
#include "FT_CSearchResult.h"
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
#include "FT_LandmarkConstants.h"
#include <centralrepository.h>
     
// CONSTANTS
_LIT(KUriDb01, "C:TP134DB01.LDB");
_LIT(KUriDb01Full, "file://C:TP134DB01.LDB");
_LIT(KUriDb02Full, "file://C:TP134DB02.LDB");
_LIT(KUriDb02, "C:TP134DB02.LDB");
_LIT(KUriDbNonExisting, "file://C:TP104DBBLABLA.LDB");
_LIT(KUriDbCorrupt, ":jf785hjtr748.abc");
_LIT(KUriNotSupported, "bike://test/tp104.wheel");
_LIT(KProtocolFile, "file");

#ifdef __WINS__
    _LIT(KUriRemovableMediaDb, "file://D:REMOVABLEDB.LDB");
    _LIT(KUriRemovableMediaDbNonExisting, "file://D:REMOVABLEDBBLABLA.LDB");
    #else
    _LIT(KUriRemovableMediaDb, "file://F:REMOVABLEDB.LDB");
    _LIT(KUriRemovableMediaDbNonExisting, "file://F:REMOVABLEDBBLABLA.LDB");
    #endif
    
_LIT(KNewDbName, "TP134 Disp Name");

_LIT(KErrorWrongDrive, "Wrong drive returned from db info");

_LIT(KErrorWrongDefault, "Wrong default db returned from db info");
_LIT(KErrorWrongDbUri, "Wrong db uri returned from db info");
_LIT(KErrorWrongProtocol, "Wrong protocol returned from db info");
_LIT(KErrorWrongDbName, "Wrong db name returned from db info");
_LIT(KErrorWrongAttributeInfo, "Wrong attribute info from db info");
_LIT(KErrorWrongNrOfDbs, "Wrong nr of db:s for ListDatasesL");
_LIT(KErrorWrongCodeForNonExistingDb, "Wrong error code for non-existing db");
_LIT(KErrorWrongCodeForNonExistingDbRem, "Wrong error code for non-existing db on removable media");
_LIT(KErrorWrongCodeForCorruptUri, "Wrong error code for corrupt uri");
_LIT(KErrorWrongCodeForNotSupportedUri, "Wrong error code for not supported uri");

// ================= MEMBER FUNCTIONS =======================
// Cleanup for RPointerArray<HPosLmDatabaseInfo> objects
void TP134CleanupTestPointerArray(TAny* aArray)

    {
    (static_cast<RPointerArray<HPosLmDatabaseInfo>*>(aArray))->ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosTp134::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp134::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    }

// ---------------------------------------------------------
// CPosTp134::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp134::StartL()
    {
    iLog->Log(_L("Warning: this test case requires E: drive"));
    
    iErrorsFound = KErrNone;
    RemoveAllLmDatabasesL();
    SetupExpectedResultsL();
    // Create the test DB:s
    iDbMan = CPosLmDatabaseManager::NewL();
    HPosLmDatabaseInfo* dbInfo01 = HPosLmDatabaseInfo::NewLC(KUriDb01);
    iDbMan->CreateDatabaseL(*dbInfo01);
    CleanupStack::PopAndDestroy(dbInfo01);
    HPosLmDatabaseInfo* dbInfo02 = HPosLmDatabaseInfo::NewLC(KUriDb02);
    iDbMan->CreateDatabaseL(*dbInfo02);
    CleanupStack::PopAndDestroy(dbInfo02);
    HPosLmDatabaseInfo* dbInfoRemovableMedia = HPosLmDatabaseInfo::NewLC(KUriRemovableMediaDb);
    iDbMan->CreateDatabaseL(*dbInfoRemovableMedia);
    CleanupStack::PopAndDestroy(dbInfoRemovableMedia);
    dbInfo02 = NULL;
    dbInfo01 = NULL;
    dbInfoRemovableMedia = NULL;
    
    // 1 Get info about the default DB
  /*  dbInfo01 = HPosLmDatabaseInfo::NewLC(KUriDb01Full);
    iDbMan->SetDefaultDatabaseUriL(KUriDb01Full);
    iDbMan->GetDatabaseInfoL(*dbInfo01);
    CheckInfoL(dbInfo01, 1);
    CleanupStack::PopAndDestroy(dbInfo01);
    dbInfo01 = NULL;*/
    
    // 2 Get info about a non-default DB
    dbInfo02 = HPosLmDatabaseInfo::NewLC(KUriDb02Full);
    iDbMan->GetDatabaseInfoL(*dbInfo02);
    CheckInfoL(dbInfo02, 2);
    CleanupStack::PopAndDestroy(dbInfo02);
    dbInfo02 = NULL;
    
    // 3a Change the default db
    // Check error report "ESLI-6CXJP3 Default database not marked as default"
   /* iDbMan->SetDefaultDatabaseUriL(KUriDb02);
    dbInfo02 = HPosLmDatabaseInfo::NewLC(KUriDb02Full);
    iDbMan->GetDatabaseInfoL(*dbInfo02);
    CheckInfoL(dbInfo02, 3);
    CleanupStack::PopAndDestroy(dbInfo02);
    dbInfo02 = NULL;
    
    iDbMan->SetDefaultDatabaseUriL(KUriDb01Full);
    
    // 3b Change the default db
    // Check error report "ESLI-6CXJP3 Default database not marked as default"
    iDbMan->SetDefaultDatabaseUriL(KUriDb02Full);
    iDbUri[3] = KUriDb02;
    iProtocol[3]  = KNullDesC;
    dbInfo02 = HPosLmDatabaseInfo::NewLC(KUriDb02);
    iDbMan->GetDatabaseInfoL(*dbInfo02);
    CheckInfoL(dbInfo02, 3);
    CleanupStack::PopAndDestroy(dbInfo02);
    dbInfo02 = NULL;
    
    iDbMan->SetDefaultDatabaseUriL(KUriDb01Full);
    // 3c Change the default db
    // Check error report "ESLI-6CXJP3 Default database not marked as default"
    iDbMan->SetDefaultDatabaseUriL(KUriDb02Full);
    iDbUri[3] = KUriDb02Full;
    iProtocol[3]  = KProtocolFile;
    dbInfo02 = HPosLmDatabaseInfo::NewLC(KUriDb02Full);
    iDbMan->GetDatabaseInfoL(*dbInfo02);
    CheckInfoL(dbInfo02, 3);
    CleanupStack::PopAndDestroy(dbInfo02);
    dbInfo02 = NULL;*/
    
    
    // 4 DB on removable media
    dbInfoRemovableMedia = HPosLmDatabaseInfo::NewLC(KUriRemovableMediaDb);
    iDbMan->GetDatabaseInfoL(*dbInfoRemovableMedia);
    CheckInfoL(dbInfoRemovableMedia, 4);
    CleanupStack::PopAndDestroy(dbInfoRemovableMedia);
    dbInfoRemovableMedia = NULL;
    
    // 5 Get info for all databases using ListDatabasesL
    iDbMan->ListDatabasesL(iDbArray);
    
    HPosLmDatabaseInfo* dbInfo = NULL;
    for(TInt count = 0; count < iDbArray.Count(); count++)
        {
        dbInfo = iDbArray[count];
        TBuf<50> testbuf = dbInfo->DatabaseUri();
        if (dbInfo->DatabaseUri() == KUriDb01Full)
            {
            CheckInfoL(dbInfo, 5);
            }
        else if (dbInfo->DatabaseUri() == KUriDb02Full)
            {
            CheckInfoL(dbInfo, 6);
            }
        else if (dbInfo->DatabaseUri() == KUriRemovableMediaDb)
            {
            CheckInfoL(dbInfo, 7);
            }
        }
        
    // According to header for ListDatabasesL in EPos_CPosLmDatabaseManager.h
    // "Any objects which are in the array when it is passed to this function are not removed."
    // Therefore add object and make sure that it is not removed
    HPosLmDatabaseInfo* testDbInfo = HPosLmDatabaseInfo::NewLC(_L("VeryMuchDummyURI.ldb"));
    TInt initialDbCount = iDbArray.Count();
    iDbArray.ResetAndDestroy();
    iDbArray.Append(testDbInfo);
    iDbMan->ListDatabasesL(iDbArray);
    AssertTrueSecL(iDbArray.Count() == initialDbCount +1, _L("iDbArray count should be incremented by 1 due to addition of new db"));

    CleanupStack::Pop(testDbInfo);
    iDbArray.ResetAndDestroy();
         
    // 6 Get info from a non-existing db:
    TPosLmDatabaseSettings dummySettings;
    HPosLmDatabaseInfo* dbInfoNonExisting = HPosLmDatabaseInfo::NewLC(KUriDbNonExisting);
    TInt err = KErrNone;
    TRAP(err, iDbMan->GetDatabaseInfoL(*dbInfoNonExisting));
    AssertTrueSecL(err == KErrNotFound, KErrorWrongCodeForNonExistingDb);
    
    CleanupStack::PopAndDestroy(dbInfoNonExisting);
    dbInfoNonExisting = NULL;
    
    // 7 Get info from a non-existing db on the removable media:
    HPosLmDatabaseInfo* dbInfoNonExistingRem = HPosLmDatabaseInfo::NewLC(KUriRemovableMediaDbNonExisting);
    TRAP(err, iDbMan->GetDatabaseInfoL(*dbInfoNonExistingRem));
    AssertTrueSecL(err == KErrNotFound, KErrorWrongCodeForNonExistingDbRem);
    CleanupStack::PopAndDestroy(dbInfoNonExistingRem);
    dbInfoNonExistingRem = NULL;
    
    // 8 Get info from a corrupt uri:
    HPosLmDatabaseInfo* dbInfoCorrupt = HPosLmDatabaseInfo::NewLC(KUriDbCorrupt);
    TRAP(err, iDbMan->GetDatabaseInfoL(*dbInfoCorrupt));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForCorruptUri);
    TRAP(err, iDbMan->ModifyDatabaseSettingsL(KUriDbCorrupt, dummySettings));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCodeForCorruptUri);
    CleanupStack::PopAndDestroy(dbInfoCorrupt);
    dbInfoCorrupt = NULL;
    
    // 9 Get info from a non-supported protocol in uri
    HPosLmDatabaseInfo* dbInfoNotSupported = HPosLmDatabaseInfo::NewLC(KUriNotSupported);
    TRAP(err, iDbMan->GetDatabaseInfoL(*dbInfoNotSupported));
    AssertTrueSecL(err == KErrNotSupported, KErrorWrongCodeForNotSupportedUri);
    TRAP(err, iDbMan->ModifyDatabaseSettingsL(KUriNotSupported, dummySettings));
    AssertTrueSecL(err == KErrNotSupported, KErrorWrongCodeForNotSupportedUri);
    CleanupStack::PopAndDestroy(dbInfoNotSupported);
    dbInfoNotSupported = NULL;
    
    // 10 Get info from the non-default db
    dbInfo01 = HPosLmDatabaseInfo::NewLC(KUriDb01);
    iDbMan->GetDatabaseInfoL(*dbInfo01);
    CheckInfoL(dbInfo01, 8);
    
    // 11 Change the display name for the non-default DB
    TPosLmDatabaseSettings settings = dbInfo01->Settings();
    TPosLmDatabaseName bufName(KNewDbName);
    settings.SetDatabaseName(bufName);
    iDbMan->ModifyDatabaseSettingsL(KUriDb01, settings);
    CleanupStack::PopAndDestroy(dbInfo01);
    dbInfo01 = NULL;
    
    // 12 Get info after name change
    dbInfo01 = HPosLmDatabaseInfo::NewLC(KUriDb01);
    iDbMan->GetDatabaseInfoL(*dbInfo01);
    CheckInfoL(dbInfo01, 9);
    CleanupStack::PopAndDestroy(dbInfo01);
    dbInfo01 = NULL;
    
    // 10b Get info from the removable media db
    dbInfoRemovableMedia = HPosLmDatabaseInfo::NewLC(KUriRemovableMediaDb);
    iDbMan->GetDatabaseInfoL(*dbInfoRemovableMedia);
    CheckInfoL(dbInfoRemovableMedia, 11);
    
    // 11b Change the display name for the non-default DB
    settings = dbInfoRemovableMedia->Settings();
    bufName = KNewDbName;
    settings.SetDatabaseName(bufName);
    iDbMan->ModifyDatabaseSettingsL(KUriRemovableMediaDb, settings);
    CleanupStack::PopAndDestroy(dbInfoRemovableMedia);
    dbInfoRemovableMedia = NULL;
    
    // 12b Get info after name change
    dbInfoRemovableMedia = HPosLmDatabaseInfo::NewLC(KUriRemovableMediaDb);
    iDbMan->GetDatabaseInfoL(*dbInfoRemovableMedia);
    CheckInfoL(dbInfoRemovableMedia, 12);
    CleanupStack::PopAndDestroy(dbInfoRemovableMedia);
    dbInfoRemovableMedia = NULL;
    
    // 13 Get info after name change
    dbInfo01 = HPosLmDatabaseInfo::NewLC(KUriDb01);
    iDbMan->GetDatabaseInfoL(*dbInfo01);
    CheckInfoL(dbInfo01, 9);
    
    // 14 Change display name, but use the unset attribute mechanism
    settings = dbInfo01->Settings();
    settings.UnsetAttribute(TPosLmDatabaseSettings::EName);
    iDbMan->ModifyDatabaseSettingsL(KUriDb01, settings);
    CleanupStack::PopAndDestroy(dbInfo01);
    dbInfo01 = NULL;
    
    // 15 Get info after "non-name-change"
    dbInfo01 = HPosLmDatabaseInfo::NewLC(KUriDb01);
    iDbMan->GetDatabaseInfoL(*dbInfo01);
    CheckInfoL(dbInfo01, 10);
    CleanupStack::PopAndDestroy(dbInfo01);
    dbInfo01 = NULL;
    
    // 16 Check max length and KNullDesC
    //_LIT(KErrorWrongCode, "Didn't receive correct error code");
	_LIT(KMaxLengthString64,    "1aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString64Bad, "x1aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString66,    "c:2aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString66Bad, "c:x2aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString71,    "file://3aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString71Bad, "file://x3aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString73,    "file://c:4aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString73Bad, "file://c:x4aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    
    CheckUriLengthL(KMaxLengthString64, KErrNotFound);
    CheckUriLengthL(KMaxLengthString64Bad, KErrNotFound); // tbd shall it be like this? (Should be KErrArgument)
    CheckUriLengthL(KMaxLengthString66, KErrNotFound);
    CheckUriLengthL(KMaxLengthString66Bad, KErrNotFound); // tbd shall it be like this?
    CheckUriLengthL(KMaxLengthString71, KErrNotFound);
    CheckUriLengthL(KMaxLengthString71Bad, KErrNotFound); // tbd shall it be like this?
    CheckUriLengthL(KMaxLengthString73, KErrNotFound);
    CheckUriLengthL(KMaxLengthString73Bad, KErrNotFound); // tbd shall it be like this?
    CheckUriLengthL(KNullDesC, KErrArgument); //tbd This crashes, should it be so?
    
    if (iErrorsFound != KErrNone) //LogErrorAndLeave(_L("Errors found in TP134"));
    	{
    	iLog->Log(_L("Errors found in TP134"));
    	User::Leave(-1);	
    	}
    else iLog->Log(_L("CPosTp134 Passed"));
    
    }
    
void CPosTp134::CheckInfoL(HPosLmDatabaseInfo* aDbInfo, TInt aTestNr)
    {
    _LIT(KTestInfo, "Step nr %d");
    TBuf<20> testInfo;
    testInfo.Format(KTestInfo, aTestNr);
    iLog->Log(testInfo);
    
    AssertTrueSecL(iDbDrive[aTestNr] == (aDbInfo->DatabaseDrive()).GetUpperCase(), KErrorWrongDrive);

   
    iLog->Log(aDbInfo->DatabaseUri());
    iLog->Log(iDbUri[aTestNr]);
    AssertTrueSecL(iDbUri[aTestNr] == aDbInfo->DatabaseUri(), KErrorWrongDbUri);
    
    AssertTrueSecL(iProtocol[aTestNr] == aDbInfo->Protocol(), KErrorWrongProtocol);
    
    TPosLmDatabaseSettings settings = aDbInfo->Settings();
    if ( iDbName[aTestNr] != settings.DatabaseName() )
        {
        iLog->Log(_L("CheckInfo: dbNames do not match: exp '%S', act '%S'"),
            &iDbName[aTestNr], &settings.DatabaseName() );
        User::Leave( KErrGeneral );
        }

    AssertTrueSecL(iAttrSet[aTestNr] == settings.IsAttributeSet(TPosLmDatabaseSettings::EName),
                                                                KErrorWrongAttributeInfo);
    }
    
void CPosTp134::SetupExpectedResultsL()
    {
    TChar cDrive = 'C';
    #ifdef __WINS__
    TChar removableDrive = 'D';
    TMediaType mediaTypeRemovable = EMediaHardDisk;
    // DatabaseMedia() seems to return EMediaRam for WINS and EMediaFlash for target
    TMediaType mediaType = EMediaRam;
    #else
    TMediaType mediaTypeRemovable = EMediaFlash;
    TChar removableDrive = 'F';
    // DatabaseMedia() seems to return EMediaRam for WINS and EMediaFlash for target
    TMediaType mediaType = EMediaNANDFlash;
    #endif
    
    HBufC* dbName = ReadDefaultDatabaseNameFromResourceLC();
    iLog->Log(_L("Default database name: '%S'"), dbName );
    
    iDbDrive[1]   = cDrive;
    iMediaType[1] = mediaType;
    iDefaultDb[1] = ETrue;
    iDbUri[1]     = KUriDb01Full;
    iProtocol[1]  = KProtocolFile;
    iDbName[1]    = *dbName;
    iAttrSet[1]   = ETrue; // TBD Shall it actually be so?
    
    iDbDrive[2]   = cDrive;
    iMediaType[2] = mediaType;
    iDefaultDb[2] = EFalse;
    iDbUri[2]     = KUriDb02Full;
    iProtocol[2]  = KProtocolFile;
    iDbName[2]    = KNullDesC;
    iAttrSet[2]   = EFalse;
    
    iDbDrive[3]   = cDrive;
    iMediaType[3] = mediaType;
    iDefaultDb[3] = ETrue;
    iDbUri[3]     = KUriDb02Full;
    iProtocol[3]  = KProtocolFile;
    iDbName[3]    = *dbName;
    iAttrSet[3]   = ETrue;
    
    iDbDrive[4]   = removableDrive;
    iMediaType[4] = mediaTypeRemovable;
    iDefaultDb[4] = EFalse;
    iDbUri[4]     = KUriRemovableMediaDb;
    iProtocol[4]  = KProtocolFile;
    iDbName[4]    = KNullDesC;
    iAttrSet[4]   = EFalse;
    
    iDbDrive[5]   = cDrive;
    iMediaType[5] = mediaType;
    iDefaultDb[5] = EFalse;
    iDbUri[5]     = KUriDb01Full;
    iProtocol[5]  = KProtocolFile;
    iDbName[5]    = KNullDesC;
    iAttrSet[5]   = EFalse;
    
    iDbDrive[6]   = cDrive;
    iMediaType[6] = mediaType;
    iDefaultDb[6] = ETrue;
    iDbUri[6]     = KUriDb02Full;
    iProtocol[6]  = KProtocolFile;
    iDbName[6]    = KNullDesC;
    iAttrSet[6]   = EFalse;
    
    iDbDrive[7]   = removableDrive;
    iMediaType[7] = mediaTypeRemovable;
    iDefaultDb[7] = EFalse;
    iDbUri[7]     = KUriRemovableMediaDb;
    iProtocol[7]  = KProtocolFile;
    iDbName[7]    = KNullDesC;
    iAttrSet[7]   = EFalse;
    
    iDbDrive[8]   = cDrive;
    iMediaType[8] = mediaType;
    iDefaultDb[8] = EFalse;
    iDbUri[8]     = KUriDb01;
    iProtocol[8]  = KNullDesC;
    iDbName[8]    = KNullDesC;
    iAttrSet[8]   = EFalse;

    iDbDrive[9]   = cDrive;
    iMediaType[9] = mediaType;
    iDefaultDb[9] = EFalse;
    iDbUri[9]     = KUriDb01;
    iProtocol[9]  = KNullDesC;
    iDbName[9]    = KNewDbName;
    iAttrSet[9]   = ETrue;
    
    iDbDrive[10]   = cDrive;
    iMediaType[10] = mediaType;
    iDefaultDb[10] = EFalse;
    iDbUri[10]     = KUriDb01;
    iProtocol[10]  = KNullDesC;
    iDbName[10]    = KNullDesC;
    iAttrSet[10]   = EFalse;
    
    iDbDrive[11]   = removableDrive;
    iMediaType[11] = mediaTypeRemovable;
    iDefaultDb[11] = EFalse;
    iDbUri[11]     = KUriRemovableMediaDb;
    iProtocol[11]  = KProtocolFile;
    iDbName[11]    = KNullDesC;
    iAttrSet[11]   = EFalse;
    
    iDbDrive[12]   = removableDrive;
    iMediaType[12] = mediaTypeRemovable;
    iDefaultDb[12] = EFalse;
    iDbUri[12]     = KUriRemovableMediaDb;
    iProtocol[12]  = KProtocolFile;
    iDbName[12]    = KNewDbName;
    iAttrSet[12]   = ETrue;
    
    CleanupStack::PopAndDestroy( dbName );
    }
    
void CPosTp134::CheckUriLengthL(const TDesC& aUri,
                                TInt aExpectedErrorCode)
    {
    _LIT(KErrorWrongCode, "Wrong code when testing uri lengths");
    TPosLmDatabaseSettings dummySettings;
    HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(aUri);
    TInt err = KErrNone;
    TRAP(err, iDbMan->GetDatabaseInfoL(*dbInfo));
    AssertTrueSecL(err == aExpectedErrorCode, KErrorWrongCode);
    TRAP(err, iDbMan->ModifyDatabaseSettingsL(aUri, dummySettings));
    AssertTrueSecL(err == aExpectedErrorCode, KErrorWrongCode);
    CleanupStack::PopAndDestroy(dbInfo);    
    }

//  End of File
