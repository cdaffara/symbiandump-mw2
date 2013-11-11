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
#include "FT_CPosTp104.h"
#include <EPos_CPosLandmarkDatabase.h>
#include "FT_LandmarkConstants.h"
#include <centralrepository.h>
           
// CONSTANTS
const TUint32 KLandmarkDefaultUri = 0x00000001;
const TUid KCRUidLandmarks = {0x101FE99B}; 

// _LIT(KSettingsFileInRom, "c:\\system\\data\\cenrep\\[101FE99B].txt");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp104::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp104::StartL()
    {
    _LIT(KLmA, "LandmarkA");
    _LIT(KLmB, "LandmarkB");
    _LIT(KLmC, "LandmarkC");
    _LIT(KNonExistingDb, "nonexistingdb.ldb");
    _LIT(KLogAboutToSetDefaultDB, "About to set default landmark DB to a non existing DB.");
    _LIT(KLogAboutToDeleteAllDbs, "About to delete all landmark DBs");
    _LIT(KLogAboutToReadBackLmA, "About to read back landmark A");
    _LIT(KLogAboutToReadBackLmB, "About to read back landmark B");
    _LIT(KLogAboutToReadBackLmC, "About to read back landmark C");
    _LIT(KErrorNonExisting, "Was able to set an non-existing DB as default.");
    _LIT(KLandmarksDbB, "lmdb104b.ldb");
    _LIT(KLandmarksDbBFull, "file://c:lmdb104b.ldb");
    _LIT(KErrorWrongDefaultDB, "Unexpected default database");
    _LIT(KCopyOfDefaultDB, "lmdb104copy.ldb");
    _LIT(KFailedToDeleteDB, "Failed to delete default database");
    _LIT(KLogAboutToSetLmDbBAsDefault, "About to set landmarks DB B as default DB.");
    _LIT(KOriginalDb, "file://c:eposlm.ldb");
    
    #ifdef __WINS__
    _LIT(KRemovableMediaDB, "d:removabledb.ldb");
    #else
    _LIT(KRemovableMediaDB, "f:removabledb.ldb");
    #endif
    
    _LIT(KNr01, "01");
    _LIT(KNr02, "02");
    _LIT(KNr03, "03");
    _LIT(KNr04, "04");
    _LIT(KNr05, "05");
    _LIT(KNr06, "06");
    _LIT(KNr07, "07");
    _LIT(KNr08, "08");
    
    _LIT(KNr10, "10");
    _LIT(KNr11, "11");
    _LIT(KNr12, "12");
    _LIT(KNr13, "13");
    _LIT(KNr14, "14");
    _LIT(KNr15, "15");
    _LIT(KNr16, "16");
    _LIT(KNr17, "17");
    _LIT(KNr18, "18");
    _LIT(KNr19, "19");
    _LIT(KNr20, "20");
    _LIT(KNr21, "21");
    _LIT(KNr22, "22");
    _LIT(KNr23, "23");
    _LIT(KNr24, "24");
    _LIT(KNr25, "25");
    _LIT(KNr26, "26");
    
    iLog->Log(_L("Warning: this test case requires E: drive"));
    
    iRepository = CRepository::NewL(KCRUidLandmarks);   
    iDbMan = CPosLmDatabaseManager::NewL();
    
    RemoveAllLmDatabasesL();
    ArgumentTestsL();
    TInt err;
    
    
    TBuf16<NCentralRepositoryConstants::KMaxUnicodeStringLength> defaultUriRep;
    //NCentralRepositoryConstants::KMaxUnicodeStringLength
    User::LeaveIfError(iRepository->Get(KLandmarkDefaultUri, defaultUriRep));
    
    
    
    // 3
    iLog->Log(KNr03);
    // Open and initialize default db
    iLandmarksDb = CPosLandmarkDatabase::OpenL();
    ExecuteAndDeleteLD(iLandmarksDb->InitializeL());
    
    
    // Add landmark A to DB:
    CPosLandmark* lmA = CreateXMLLandmarkLC(KLmA);
    TPosLmItemId lmAId = iLandmarksDb->AddLandmarkL(*lmA);
    CleanupStack::PopAndDestroy(lmA);
    User::After(2000000);
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());
    // User::After(2000000);
    // Check that it was actually added:
    iLog->Log(KLogAboutToReadBackLmA);
    lmA = iLandmarksDb->ReadLandmarkLC(lmAId);
    iLog->Log(KLogAboutToReadBackLmA);
    CleanupStack::PopAndDestroy(lmA);
    
    // 4
    iLog->Log(KNr04);
    // Test to set a non-existing db as default:
    iLog->Log(KLogAboutToSetDefaultDB);
    TRAP(err, iDbMan->SetDefaultDatabaseUriL(KNonExistingDb));
    
    if (err != KErrNotFound)
        {
        
        iLog->Log(KErrorNonExisting);
    	User::Leave(err);
        }
        
    // 5
    iLog->Log(KNr05);
    // Create a new db and add a landmark to it:
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    HPosLmDatabaseInfo* dbInfoB = HPosLmDatabaseInfo::NewLC(KLandmarksDbB);
    iDbMan->CreateDatabaseL(*dbInfoB);
    CleanupStack::PopAndDestroy(dbInfoB);
    iLandmarksDb = CPosLandmarkDatabase::OpenL(KLandmarksDbB);
    ExecuteAndDeleteLD(iLandmarksDb->InitializeL());
    
    CPosLandmark* lmB = CreateXMLLandmarkLC(KLmB);
    TPosLmItemId lmBId = iLandmarksDb->AddLandmarkL(*lmB);
    User::After(2000000);
    CleanupStack::PopAndDestroy(lmB);
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());
    
    // Read back the landmark:
    iLog->Log(KLogAboutToReadBackLmB);
    lmB = iLandmarksDb->ReadLandmarkLC(lmBId);
    CleanupStack::PopAndDestroy(lmB);
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    
    // 6
    iLog->Log(KNr06);
    // Set the new DB as default, and check that it worked OK:
    iLog->Log(KLogAboutToSetLmDbBAsDefault);
    iDbMan->SetDefaultDatabaseUriL(KLandmarksDbB);
    HBufC* defaultURI = iDbMan->DefaultDatabaseUriLC();
    
    // 7
    iLog->Log(KNr07);
    if (defaultURI->Des().CompareF(KLandmarksDbBFull))
        {
        
        iLog->Log(KErrorWrongDefaultDB);
        User::Leave(-1);
        }
        
     CleanupStack::PopAndDestroy(defaultURI);
    
    // 8
    iLog->Log(KNr08);
    // Open the default DB and try to find the landmark that was added before:
    iLandmarksDb = CPosLandmarkDatabase::OpenL();
    if (iLandmarksDb->IsInitializingNeeded())
   {
   ExecuteAndDeleteLD(iLandmarksDb->InitializeL());
   }

    iLog->Log(KLogAboutToReadBackLmB);
    lmB = iLandmarksDb->ReadLandmarkLC(lmBId);
    CleanupStack::PopAndDestroy(lmB);
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    
    // 9
    iLog->Log(KNr01);
    // Change the default db:
    iDbMan->SetDefaultDatabaseUriL(defaultUriRep);
    defaultURI = iDbMan->DefaultDatabaseUriLC();
    
    if (defaultURI->Des().CompareF(defaultUriRep))
        {
        
        iLog->Log(KErrorWrongDefaultDB);
        User::Leave(-1);
        }
        
    CleanupStack::PopAndDestroy(defaultURI);
        
    // 10
    iLog->Log(KNr10);
    // Open the default DB and check that it contains landmark A:
    iLandmarksDb = CPosLandmarkDatabase::OpenL();
    if (iLandmarksDb->IsInitializingNeeded())
   {
   ExecuteAndDeleteLD(iLandmarksDb->InitializeL());
   }
    iLog->Log(KLogAboutToReadBackLmA);
    lmA = iLandmarksDb->ReadLandmarkLC(lmAId);
    CleanupStack::PopAndDestroy(lmA);
    
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    
    
    // ********************** Test Copy **********************
    // 11
    iLog->Log(KNr11);
    // Copy the default DB:
    iDbMan->CopyDatabaseL(defaultUriRep, KCopyOfDefaultDB);
    
    // 12
    iLog->Log(KNr12);
        
    // 13
    iLog->Log(KNr13);
    // Check which DB that is the default:
    defaultURI = iDbMan->DefaultDatabaseUriLC();
    if (defaultURI->Des().CompareF(defaultUriRep))
        {
        
        iLog->Log(KErrorWrongDefaultDB);
        User::Leave(-1);
        }
    CleanupStack::PopAndDestroy(defaultURI);
    
    // 14
    iLog->Log(KNr14);
    iDbMan->SetDefaultDatabaseUriL(KCopyOfDefaultDB);
    
    // Open the default DB and check that it contains landmark A:
    iLandmarksDb = CPosLandmarkDatabase::OpenL();
    if (iLandmarksDb->IsInitializingNeeded())
   {
   ExecuteAndDeleteLD(iLandmarksDb->InitializeL());
   }
    iLog->Log(KLogAboutToReadBackLmA);
    lmA = iLandmarksDb->ReadLandmarkLC(lmAId);
    CleanupStack::PopAndDestroy(lmA);

    // 15
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    iLog->Log(KNr15);
    
    // Create a landmarks DB on the memory card:
    HPosLmDatabaseInfo* dbInfoRemMedia = HPosLmDatabaseInfo::NewLC(KRemovableMediaDB);
    iDbMan->CreateDatabaseL(*dbInfoRemMedia);
    CleanupStack::PopAndDestroy(dbInfoRemMedia);
    
    iLandmarksDb = CPosLandmarkDatabase::OpenL(KRemovableMediaDB);
    ExecuteAndDeleteLD(iLandmarksDb->InitializeL());
    
    // Add a landmark to it:
    CPosLandmark* lmC = CreateXMLLandmarkLC(KLmC);
    TPosLmItemId lmCId = iLandmarksDb->AddLandmarkL(*lmC);
    CleanupStack::PopAndDestroy(lmC);
    User::After(200000);
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    
    // 16
    iLog->Log(KNr16);
    iDbMan->SetDefaultDatabaseUriL(KRemovableMediaDB);
    
    iLandmarksDb = CPosLandmarkDatabase::OpenL();
    if (iLandmarksDb->IsInitializingNeeded())
   {
   ExecuteAndDeleteLD(iLandmarksDb->InitializeL());
   }
    
    // Check that landmark C exists:
    iLog->Log(KLogAboutToReadBackLmC);
    lmC = iLandmarksDb->ReadLandmarkLC(lmCId);
    CleanupStack::PopAndDestroy(lmC);
    
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    
    // URI Testing:
    _LIT(KTestDB104A, "file://c:TestDb104a.ldb");
    _LIT(KTestDB104B, "file://c:TestDb104b.ldb");
    #ifdef __WINS__
    _LIT(KTestDB104MMC, "file://d:TestDbMMC.ldb");
    #else
    _LIT(KTestDB104MMC, "file://f:TestDbMMC.ldb");
    #endif
    
    // Create some testdb:s
    HPosLmDatabaseInfo* dbInfo104A = HPosLmDatabaseInfo::NewLC(KTestDB104A);
    HPosLmDatabaseInfo* dbInfo104B = HPosLmDatabaseInfo::NewLC(KTestDB104B);
    HPosLmDatabaseInfo* dbInfo104MMC = HPosLmDatabaseInfo::NewLC(KTestDB104MMC);
    iDbMan->CreateDatabaseL(*dbInfo104A);
    iDbMan->CreateDatabaseL(*dbInfo104B);
    iDbMan->CreateDatabaseL(*dbInfo104MMC);
    CleanupStack::PopAndDestroy(dbInfo104MMC);
    CleanupStack::PopAndDestroy(dbInfo104B);
    CleanupStack::PopAndDestroy(dbInfo104A);
    
    
    _LIT(KNonsenseURI, "&t87%(s!,uy.poffd");
    _LIT(KNotEndingWithLdbURI, "file://c:TestDb104a");
    _LIT(KEmptyURI, "");
    _LIT(KLongerThanAllowedURI, "longdatabaseewjktgjfkdnbnbfjghskwuwrqtrccxbdhfbbjgnmglkytiyitiugjqoiwiejakxngjhdygrgdskhdhdbxvafarqtwyrjgjtihjkjbnbjyhkjltoyouiylhjrnbmdngfhurtwgewvetqwsahdgfey4756eyrg8328yrgfgggbbnnmnkjgjdhajaqiwuwueytriutiiiutuithjhgergfbfhgnbmbmnmklkrjsfadadeqwyteutiyibnvcbxvdafsdgdhey.ldb"); // more than 256 (KMaxUnicodeStringLength)
    _LIT(KCorrectURI, "file://c:TestDb104b.ldb");
    _LIT(KCorrectButWithoutDriveURI, "file://TestDb104a.LDB");
    _LIT(KOnMmcButWithoutDriveURI, "file://TestDb104c.ldb");
    _LIT(KRemoteURI, "http://www.landmarks.com/dbs/remote104.ldb");
    _LIT(KNoProtocolURI, "c:TestDb104b.ldb");
    
    // 17
    iLog->Log(KNr17);
    TestSetUriL(KNonsenseURI, KErrArgument);
    // 18
    iLog->Log(KNr18);
    TestSetUriL(KNotEndingWithLdbURI, KErrArgument);
    // 19
    iLog->Log(KNr19);
    TestSetUriL(KEmptyURI, KErrArgument);
    // 20
    iLog->Log(KNr20);
    TestSetUriL(KLongerThanAllowedURI, KErrArgument);
    // 21
    iLog->Log(KNr21);
    TestSetUriL(KCorrectURI, KErrNone);
    // 22
    iLog->Log(KNr22);
    TestSetUriL(KCorrectButWithoutDriveURI, KErrNone);
    // 23
    iLog->Log(KNr23);
    TestSetUriL(KOnMmcButWithoutDriveURI, KErrNotFound);
    // 24
    iLog->Log(KNr24);
    TestSetUriL(KRemoteURI, KErrNotSupported);
    // 25
    iLog->Log(KNr25);
    TestSetUriL(KNoProtocolURI, KErrNone);
    // 26
    iLog->Log(KNr26);
    TestSetUriL(KNoProtocolURI, KErrNone);
    
    delete iDbMan;
    iDbMan = NULL;
    
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    
    delete iRepository;
    iRepository = NULL;
    }

// ---------------------------------------------------------
// CPosTp104::TestSetUriL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
void CPosTp104::TestSetUriL(const TDesC& aUriString, TInt aExpectedErrorCode)
    {
    TRAPD(err,iDbMan->SetDefaultDatabaseUriL(aUriString));
    if (err != aExpectedErrorCode)
        {
        _LIT(KErrorText, "Received wrong error code when setting URI. Expected %d, Received %d");
        TBuf<100> buf;
        buf.Format(KErrorText, aExpectedErrorCode, err);
       
       iLog->Log(buf);
        User::Leave(err);
        }
    }
   
// ---------------------------------------------------------
// CPosTp104::ArgumentTests
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp104::ArgumentTestsL()
	{
	_LIT(KErrorWrongCode, "Didn't receive correct error code");
	_LIT(KMaxLengthString64,    "1aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString64Bad, "x1aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString66,    "c:2aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString66Bad, "c:x2aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString71,    "file://3aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString71Bad, "file://x3aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString73,    "file://c:4aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    _LIT(KMaxLengthString73Bad, "file://c:x4aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.ldb");
    TInt err = 0;
    
        // Should work OK:
    HPosLmDatabaseInfo* dbInfo64 = HPosLmDatabaseInfo::NewLC(KMaxLengthString64);
    HPosLmDatabaseInfo* dbInfo64Bad = HPosLmDatabaseInfo::NewLC(KMaxLengthString64Bad);
    HPosLmDatabaseInfo* dbInfo66 = HPosLmDatabaseInfo::NewLC(KMaxLengthString66);
    HPosLmDatabaseInfo* dbInfo66Bad = HPosLmDatabaseInfo::NewLC(KMaxLengthString66Bad);
    HPosLmDatabaseInfo* dbInfo71 = HPosLmDatabaseInfo::NewLC(KMaxLengthString71);
    HPosLmDatabaseInfo* dbInfo71Bad = HPosLmDatabaseInfo::NewLC(KMaxLengthString71Bad);
    HPosLmDatabaseInfo* dbInfo73 = HPosLmDatabaseInfo::NewLC(KMaxLengthString73);
    HPosLmDatabaseInfo* dbInfo73Bad = HPosLmDatabaseInfo::NewLC(KMaxLengthString73Bad);
    
    iDbMan->CreateDatabaseL(*dbInfo64);
    iDbMan->CreateDatabaseL(*dbInfo66);
    iDbMan->CreateDatabaseL(*dbInfo71);
    iDbMan->CreateDatabaseL(*dbInfo73);
    
    
    
    TRAP(err, iDbMan->CreateDatabaseL(*dbInfo64Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    TRAP(err, iDbMan->CreateDatabaseL(*dbInfo66Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    TRAP(err, iDbMan->CreateDatabaseL(*dbInfo71Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    TRAP(err, iDbMan->CreateDatabaseL(*dbInfo73Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    
    CleanupStack::PopAndDestroy(dbInfo73Bad);
    CleanupStack::PopAndDestroy(dbInfo73);
    CleanupStack::PopAndDestroy(dbInfo71Bad);
    CleanupStack::PopAndDestroy(dbInfo71);
    CleanupStack::PopAndDestroy(dbInfo66Bad);
    CleanupStack::PopAndDestroy(dbInfo66);
    CleanupStack::PopAndDestroy(dbInfo64Bad);
    CleanupStack::PopAndDestroy(dbInfo64);
    
/////////////////////////////////////////////////////////////////////
    // Should work OK:
    iDbMan->DatabaseExistsL(KMaxLengthString64);
    iDbMan->DatabaseExistsL(KMaxLengthString66);
    iDbMan->DatabaseExistsL(KMaxLengthString71);
    iDbMan->DatabaseExistsL(KMaxLengthString73);
        
    TRAP(err, iDbMan->DatabaseExistsL(KNullDesC));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    
    TRAP(err, iDbMan->DatabaseExistsL(KMaxLengthString64Bad));
    AssertTrueSecL(err == KErrNone, KErrorWrongCode);
    TRAP(err, iDbMan->DatabaseExistsL(KMaxLengthString66Bad));
    AssertTrueSecL(err == KErrNone, KErrorWrongCode);
    TRAP(err, iDbMan->DatabaseExistsL(KMaxLengthString71Bad));
    AssertTrueSecL(err == KErrNone, KErrorWrongCode);
    TRAP(err, iDbMan->DatabaseExistsL(KMaxLengthString73Bad));
    AssertTrueSecL(err == KErrNone, KErrorWrongCode);
    
/////////////////////////////////////////////////////////////////////
    
    // Should work OK:
    iDbMan->SetDefaultDatabaseUriL(KMaxLengthString64);
    iDbMan->SetDefaultDatabaseUriL(KMaxLengthString66);
    iDbMan->SetDefaultDatabaseUriL(KMaxLengthString71);
    iDbMan->SetDefaultDatabaseUriL(KMaxLengthString73);
    
    // Should not work:
    TRAP(err, iDbMan->SetDefaultDatabaseUriL(KNullDesC));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    TRAP(err, iDbMan->SetDefaultDatabaseUriL(KMaxLengthString64Bad));
    AssertTrueSecL(err == KErrNotFound, KErrorWrongCode);
    TRAP(err, iDbMan->SetDefaultDatabaseUriL(KMaxLengthString66Bad));
    AssertTrueSecL(err == KErrNotFound, KErrorWrongCode);
    TRAP(err, iDbMan->SetDefaultDatabaseUriL(KMaxLengthString71Bad));
    AssertTrueSecL(err == KErrNotFound, KErrorWrongCode);
    TRAP(err, iDbMan->SetDefaultDatabaseUriL(KMaxLengthString73Bad));
    AssertTrueSecL(err == KErrNotFound, KErrorWrongCode);
    
/////////////////////////////////////////////////////////////////////
    // Should work OK:
    iDbMan->DeleteDatabaseL(KMaxLengthString64);
    iDbMan->DeleteDatabaseL(KMaxLengthString66);
    iDbMan->DeleteDatabaseL(KMaxLengthString71);
    iDbMan->DeleteDatabaseL(KMaxLengthString73);
    
    // Should not work: 
    
    TRAP(err, iDbMan->DeleteDatabaseL(KMaxLengthString64Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    TRAP(err, iDbMan->DeleteDatabaseL(KMaxLengthString66Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    TRAP(err, iDbMan->DeleteDatabaseL(KMaxLengthString71Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    TRAP(err, iDbMan->DeleteDatabaseL(KMaxLengthString73Bad));
    AssertTrueSecL(err == KErrArgument, KErrorWrongCode);
    
	}
	
// ---------------------------------------------------------
// CPosTp104::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp104::CloseTest()
	{
	iLog->Log(_L("CloseTest"));
	delete iDbMan;
    iDbMan = NULL;
    
    delete iLandmarksDb;
    iLandmarksDb = NULL;
    
    delete iRepository;
    iRepository = NULL;
	}
	
//  End of File
