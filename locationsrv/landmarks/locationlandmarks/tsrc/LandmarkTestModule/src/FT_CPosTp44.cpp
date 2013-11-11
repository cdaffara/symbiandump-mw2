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

#include "FT_CPosTp44.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <e32std.h>
#include <bautils.h>

_LIT8(KNonExistingMimeType1, "Application/vnd.nokia.landmarkcollection+xml");
_LIT8(KNonExistingMimeType2, "application/vnd.nokia.kokoibollen");
_LIT8(KEmptyMimeType, "");
_LIT(KFileName, "c:\\documents\\SomeDummyFile.xml");
_LIT(KFileName2, "c:\\documents\\SomeDummyFile2.xml");
_LIT(KFileName3, "c:\\documents\\SomeDummyFile3.xml");
_LIT(KFileName4, "c:\\documents\\SomeDummyFile4.xml");
_LIT(KFileName5, "c:\\documents\\SomeDummyFile5.xml");
_LIT(KFileName6, "c:\\documents\\SomeDummyFile6.xml");
_LIT(KFileName7, "c:\\documents\\SomeDummyFile7.xml");
_LIT(KFileName8, "c:\\documents\\SomeDummyFile8.xml");
_LIT(KFileName9, "c:\\documents\\SomeDummyFile9.xml");
_LIT(KFileName9a, "c:\\documents\\SomeDummyFile9a.xml");
_LIT(KFileName10, "c:\\documents\\SomeDummyFile10.xml");
_LIT(KFileName11, "c:\\documents\\SomeDummyFile11.xml");
_LIT(KFileName12, "c:\\documents\\SomeDummyFile12.xml");
_LIT(KFileName13, "c:\\documents\\SomeDummyFile13.xml");
_LIT(KFileName14, "c:\\documents\\SomeDummyFile14.xml");
_LIT(KFileName15, "c:\\documents\\SomeDummyFile15.xml");
_LIT(KFileName16, "c:\\documents\\SomeDummyFile16.xml");
_LIT(KFileName17, "c:\\documents\\SomeDummyFile17.xml");
_LIT(KFileName18, "c:\\documents\\SomeDummyFile18.xml");



// More than 255 characters
_LIT(KLongFileName, "c:\\SomeDummeFilewithVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongVeryLongFileName.xml");

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp44::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp44::StartL()
    {

    BaflUtils::EnsurePathExistsL( iFileSession, KFileName );
    
    MakeSurePanicDebugFileExistsL();
    // Use same lm db as in composite search test
    iDatabase = UseCompositeLandmarksDbFileL();   
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    delete iDatabase;
    iDatabase = NULL;
    iDatabase = CPosLandmarkDatabase::OpenL();
    	
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    // Do reset so that global categories are included
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);   
    ExecuteAndDeleteLD(categoryManager->ResetGlobalCategoriesL());
    CleanupStack::PopAndDestroy(categoryManager);

    // <<<<<<<<<<<<<<Panic testing below>>>>>>>>>>>>>>>>>>>
    iLog->Log(_L(">>>>>>>Start Panic test<<<<<<<<<"));
       
    iExpectedExitType = ExitPanic;
    // Add collection data after landmarks has been added to encoder
    iLog->Log(_L("Panic test 1"));
//    DeleteFileL(KFileName);
    iTestCase = 1;
    iExpectedErrorCode = EPosLmProtocolBreak;
    ResumeThreadAndVerifyExit();

    // Add collection data after landmarks has been added to encoder
    iLog->Log(_L("Panic test 2"));
    //DeleteFileL(KFileName);
    iTestCase = 2;
    iExpectedErrorCode = EPosLmProtocolBreak;
    ResumeThreadAndVerifyExit();

    // Try to use encoder after finalization
    iLog->Log(_L("Panic test 3"));
    //DeleteFileL(KFileName);
    iTestCase = 3;
    iExpectedErrorCode = EPosLmProtocolBreak;
    ResumeThreadAndVerifyExit();

    // 4 The client deletes the operation object before the encoding has been 
    // finalized and then tries to use the encoder object.Should cause Panic and FileLock should have been released
    iLog->Log(_L("Panic test 4"));
    //DeleteFileL(KFileName);
    iTestCase = 4;
    iExpectedErrorCode = EPosLmProtocolBreak;
    ResumeThreadAndVerifyExit();
    // Check that fileLock has been released
    RFile file;
    CleanupClosePushL(file);
    // Make sure that file exist
    TInt err = file.Open(iFileSession, KFileName, EFileShareAny);
    if (err != KErrNone) //LogErrorAndLeave(_L("File should be able to open"));
    {
    iLog->Log(_L("File should be able to open"));
    User::Leave(err);

    }
    CleanupStack::PopAndDestroy(&file);
    iLog->Log(_L("FileLock has been released"));

    // 5 The client tries to specify a collection id already added. Should cause a leave with -11
    iLog->Log(_L("Leave test 5"));
//    DeleteFileL(KFileName);
    iTestCase = 5;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrAlreadyExists;
    ResumeThreadAndVerifyExit();

    // 6 The client tries to specify a mime type that does not exist. Should cause a leave with KErrNotSupported
    iLog->Log(_L("Leave test 6"));
//    DeleteFileL(KFileName);
    iTestCase = 6;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrNotSupported;
    ResumeThreadAndVerifyExit();
    
    // 7 The client tries to specify a output file that already exists. Shoule leave with KErrAlreadyExists
    iLog->Log(_L("Leave test 7"));
    iTestCase = 7;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrAlreadyExists;
    ResumeThreadAndVerifyExit();

    // 8 The client only specifies a collection and then finalize the encoding.
    iLog->Log(_L("Panic test 8"));
//    DeleteFileL(KFileName);
    iTestCase = 8;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = EPosLmProtocolBreak;
    ResumeThreadAndVerifyExit();

    // 9a Export landmark with nonexisting id (positive id), should leave with KErrNotFound
    iLog->Log(_L("Panic test 9a"));
//    DeleteFileL(KFileName);
    iTestCase = 9;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrNotFound;
    ResumeThreadAndVerifyExit();

    // 9b Export landmark with nonexisting id (negative id), should leave with KErrNotFound
    iLog->Log(_L("Leave test 9b"));
//    DeleteFileL(KFileName);
    iTestCase = 10;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrNotFound;
    ResumeThreadAndVerifyExit();

    // 10 Delete export object before completion and then finalize
    iLog->Log(_L("Leave test 10"));
//    DeleteFileL(KFileName);
    iTestCase = 11;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = EPosLmProtocolBreak;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrNone;
    ResumeThreadAndVerifyExit();

    // 11 The client creates an encoder and by using the Landmark API 
    // tries to export landmarks with invalid transfer option. Should cause Panic
    iLog->Log(_L("Panic test 11"));
//    DeleteFileL(KFileName);
    iTestCase = 12;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = EPosLmInvalidArgument;
    ResumeThreadAndVerifyExit();

    // 12 The client creates an encoder by using the file option and 
    // adds some landmarks but instead of finalizing the encoding a new file is set.
    iLog->Log(_L("Leave test 12"));
//    DeleteFileL(KFileName);
//    DeleteFileL(KFileName2);
    iTestCase = 13;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrNone;
    ResumeThreadAndVerifyExit();

    // 13 The client creates an encoder by using the file option and 
    // adds a collection data with EPosLmCollDataNone, should Panic with EPosLmInvalidArgument
    iLog->Log(_L("Leave test 13"));
//    DeleteFileL(KFileName);
//    DeleteFileL(KFileName2);
    iTestCase = 14;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = EPosLmInvalidArgument;
    ResumeThreadAndVerifyExit();

    // 14 The client tries to specify a mime type that does not exist. Should cause a leave with KErrNotSupported
    // the mime type name is similar to KLmxMimeType but contains capital letters
    iLog->Log(_L("Leave test 14"));
//    DeleteFileL(KFileName);
    iTestCase = 15;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrNotSupported;
    ResumeThreadAndVerifyExit();
    
    // 15 The client creates an encoder and by using the Landmark API 
    // tries to export landmarks with invalid transfer option. Should cause Panic
    iLog->Log(_L("Panic test 15"));
//    DeleteFileL(KFileName);
    iTestCase = 16;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = EPosLmInvalidArgument;
    ResumeThreadAndVerifyExit();
    
    // 16 Export landmark with empty subset of ids, should leave with KErrArgument
    iLog->Log(_L("Leave test 16"));
//    DeleteFileL(KFileName);
    iTestCase = 17;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrArgument;
    ResumeThreadAndVerifyExit();

#if 0
    // 17 Try to create encoder with empty mime type
    // TE: the REComSession::CreateImplementationL( leaves with KErrArgument which I think is acceptable
    // Nokia: This is not part of API definition, case removed
    iLog->Log(_L("Leave test 17"));
//    DeleteFileL(KFileName);
    iTestCase = 18;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrArgument;
    ResumeThreadAndVerifyExit();
#endif    

#ifdef __WINS__    
    // 18 Try to create encoder with a very long file name and export one landmark
    iLog->Log(_L("Leave test 18"));
    iTestCase = 19;

    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrBadName;

    ResumeThreadAndVerifyExit();
#endif    
    iLog->Log(_L(">>>>>>>Panic test Done<<<<<<<<<"));
    }

// ---------------------------------------------------------
// CPosTp44::ResumeThreadAndVerifyExit
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp44::ResumeThreadAndVerifyExit()
    {
    iLog->Log(_L("ResumeThreadAndVerifyExit"));
    _LIT(KPanicErr, "Method didn't Panic when expected"); 
    _LIT(KPanicCodeErr, "Paniced with unexpected panic code");
    _LIT(KLeaveErr, "Method didn't end correctly"); 
    _LIT(KLeaveCodeErr, "Unexpected leave error");

    CreateThreadL();
    TRequestStatus status;
    iThread.Logon(status);
    iThread.Resume();
    User::WaitForRequest(status);
        
    
    // Used for debugging
    TBuf<100> buf;
    buf.Format(_L("ExitType %d Exit reason %d"), iThread.ExitType(), iThread.ExitReason());
    iLog->Log(buf);

    if (iExpectedExitType == ExitPanic)
        {
        AssertTrueSecL(iThread.ExitType() == EExitPanic, KPanicErr);
        AssertTrueSecL(iThread.ExitReason() == iExpectedErrorCode, KPanicCodeErr);
        }
    else if (iExpectedExitType == ExitLeave)
        {
        AssertTrueSecL(iThread.ExitType() == EExitKill, KLeaveErr);
        AssertTrueSecL(iThread.ExitReason() == iExpectedErrorCode, KLeaveCodeErr);
        }
       
    iThread.Close();
    }

// ---------------------------------------------------------
// CPosTp44::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp44::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iThread.Close();

    delete iDatabase;
    iDatabase = NULL;
    
    iLog->Log(_L("CloseTest Done"));
	}

// ---------------------------------------------------------
// CPosTp44::DeleteEncoderCallback
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
TInt CPosTp44::DeleteEncoderCallback(TAny* aSelf)
	{
	CPosLandmarkEncoder* encoder = static_cast<CPosLandmarkEncoder*>(aSelf);
	delete encoder;
	return 0;
	}
	
// ---------------------------------------------------------
// CPosTp44::RunPanicTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp44::RunPanicTestL(TAny* aData)
    {
    CPosTp44* self = reinterpret_cast<CPosTp44*>(aData);

     if (self->iTestCase == 1)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder *landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName);
        // Add landmark to be encoded
        CPosLandmark* lm = database->ReadLandmarkLC(1);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        // Add one more landmark to be encoded
        lm = database->ReadLandmarkLC(2);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        // Add collection data after that landmarks has been added, should cause Panic
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 2)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName3);
        CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*database);
        CleanupStack::PushL(categoryManager);
        CPosLandmarkCategory* landmarkCategory = categoryManager->ReadCategoryLC(1);
        // Add one category when no landmark has been addded , should cause Panic        
        landmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        CleanupStack::PopAndDestroy(categoryManager);
     }
     else if (self->iTestCase == 3)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName4);
        // Add landmark to be encoded
        CPosLandmark* lm = database->ReadLandmarkLC(1);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        // Finalize
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        // Try to use encoder after finalization, should cause Panic
        lm = database->ReadLandmarkLC(2);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        }
     else if (self->iTestCase == 4)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName5);
        // Add landmark to be encoded
        CPosLandmark* lm = database->ReadLandmarkLC(1);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        // Finalize
        CPosLmOperation* op = landmarkEncoder->FinalizeEncodingL();
        delete op;
        op = NULL;
        // Try to use encoder after deletion of object, should cause Panic
        lm = database->ReadLandmarkLC(2);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        }
     else if (self->iTestCase == 5)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName6);
        // Add collection data
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        // Add same collection data again, should cause leave
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName2"));
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 6)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        // Create encoder with invalid MIME type, should leave with KErrNotSupported
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KNonExistingMimeType2);
        landmarkEncoder->SetOutputFileL(KFileName7);
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 7)
        {
         RFs fs;
         RFile file;
         User::LeaveIfError(fs.Connect());
         CleanupClosePushL(fs);
         CleanupClosePushL(file);
         
         // Make sure that file exist
         TInt err = file.Create(fs, KFileName8, EFileShareAny);
         // Should never happen
         if (err != KErrNone) User::Leave(666);
         
         CleanupStack::PopAndDestroy(&file);
         CleanupStack::PopAndDestroy(&fs);
         
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
         // Use same file for encoding, set output file should leave
         landmarkEncoder->SetOutputFileL(KFileName8);
         CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 8)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName9);
        // Add collection data
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        // Finalize without adding any landmarks, should cause panic
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 9 || self->iTestCase == 10)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        if(self->iTestCase == 9) 
            landmarkEncoder->SetOutputFileL(KFileName9a);
        if(self->iTestCase == 10) 
            landmarkEncoder->SetOutputFileL(KFileName10);
        
        // Add collection data
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        arr.Append(1);
        arr.Append(15);
        arr.Append(21);
        arr.Append(38);
        arr.Append(32);
        arr.Append(12);

        if (self->iTestCase == 9)
            {
            // 9a
            // Should never exist
            arr.Append(666);
            }
        else if (self->iTestCase == 10)
            {
            // 9b
            // Should never exist,
            TInt dummy = -666;
            arr.Append(dummy);
            }
        // Should leave with KErrNotFound
        ExecuteAndDeleteLD(database->ExportLandmarksL(*landmarkEncoder, arr, CPosLandmarkDatabase::EDefaultOptions));
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        CleanupStack::PopAndDestroy(&arr);
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 11)
        {
        // Delete export object before completion and then finalize
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName11);
        // Add collection data
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        arr.Append(1);
        arr.Append(15);
        arr.Append(21);
        arr.Append(38);
        arr.Append(32);
        arr.Append(12);

        // Delete encoder from callback
    	TCallBack callback(DeleteEncoderCallback, landmarkEncoder);
    	CPeriodic* periodicTimer = CPeriodic::NewL(CActive::EPriorityHigh);
    	CleanupStack::PushL(periodicTimer);
    	periodicTimer->Start(1, 1, callback);
    	
        ExecuteAndDeleteLD(database->ExportLandmarksL(*landmarkEncoder, arr, CPosLandmarkDatabase::EDefaultOptions));
        // Try to FinalizeEncodingL
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        
        CleanupStack::PopAndDestroy(periodicTimer);
        CleanupStack::PopAndDestroy(&arr);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 12)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName12);
        // Add collection data
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        arr.Append(1);
        arr.Append(15);
        arr.Append(21);
        arr.Append(38);
        arr.Append(32);
        arr.Append(12);

        // Export with invalid transfer options, should panic
        ExecuteAndDeleteLD(database->ExportLandmarksL(*landmarkEncoder, arr, 666));
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 13)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        CleanupStack::PushL(landmarkEncoder);
        landmarkEncoder->SetOutputFileL(KFileName13);
        // Add landmark to be encoded
        CPosLandmark* lm = database->ReadLandmarkLC(1);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        // Set new output file instead of finalization
        landmarkEncoder->SetOutputFileL(KFileName2);

        // Check that filelock has been released
        RFs fs;
        RFile file;
        User::LeaveIfError(fs.Connect());
        CleanupClosePushL(fs);
        CleanupClosePushL(file);
        // Make sure that file exist
        TInt err = file.Open(fs, KFileName13, EFileShareAny);
        if (err != KErrNone) User::Leave(667);
        CleanupStack::PopAndDestroy(&file);
        CleanupStack::PopAndDestroy(&fs);

        // Add landmark to be encoded
        lm = database->ReadLandmarkLC(11);
        landmarkEncoder->AddLandmarkL(*lm);
        CleanupStack::PopAndDestroy(lm);
        // Finalize
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 14)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        CleanupStack::PushL(landmarkEncoder);
        landmarkEncoder->SetOutputFileL(KFileName14);
        // Add collection data of type EPosLmCollDataNone, should panic
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataNone, _L("TP44CollectionDataName"));
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 15)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        // Create encoder with invalid MIME type, should leave with KErrNotSupported
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KNonExistingMimeType1);
        landmarkEncoder->SetOutputFileL(KFileName15);
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 16)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName16);
        // Add collection data
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        arr.Append(1);
        arr.Append(15);
        arr.Append(21);
        arr.Append(38);
        arr.Append(32);
        arr.Append(12);

        // Export with invalid transfer options, should panic
        ExecuteAndDeleteLD(database->ExportLandmarksL(*landmarkEncoder, arr, 0x04));
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 17)
        {
        // Export with empty landmark array
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        landmarkEncoder->SetOutputFileL(KFileName17);
        // Add collection data
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        
        ExecuteAndDeleteLD(database->ExportLandmarksL(*landmarkEncoder, arr, CPosLandmarkDatabase::EDefaultOptions));
        ExecuteAndDeleteLD(landmarkEncoder->FinalizeEncodingL());
        CleanupStack::PopAndDestroy(&arr);
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 18)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        // Create encoder with empty MIME type, should leave with KErrArgument
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KEmptyMimeType);
        landmarkEncoder->SetOutputFileL(KFileName18);
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 19)
        {
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
        // Create encoder with long file name, should leave with KErrBadName
        CPosLandmarkEncoder* landmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
        // Should leave here
        landmarkEncoder->SetOutputFileL(KLongFileName);
        landmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP44CollectionDataName"));
        CleanupStack::PopAndDestroy(landmarkEncoder);
        CleanupStack::PopAndDestroy(database);
        }
    }

// ---------------------------------------------------------
// LOCAL_C ThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp44::RunPanicTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp44::CreateThreadL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp44::CreateThreadL()
    {
    _LIT(KThreadName, "Tp44testthread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");
	TBuf<100> buf;
	buf.Format(KThreadName, iTestCase);
	
     TInt err=0;
     err = iThread.Create(buf, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
     AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    }

//  End of File
