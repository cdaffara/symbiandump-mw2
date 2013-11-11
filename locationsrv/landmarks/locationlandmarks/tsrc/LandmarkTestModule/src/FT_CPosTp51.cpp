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

#include "FT_CPosTp51.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <e32std.h>
//#include <ss_std.h>

// CONSTANTS
_LIT8(KNonExistingMimeType1, "APPLICATION/VND.NOKIA.LANDMARKCOLLECTION+XML");
_LIT8(KNonExistingMimeType2, "application/vnd.nokia.kokoibollen");
//_LIT8(KNonExistingMimeType3, "application/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollenapplication/vnd.nokia.kokoibollen");
_LIT8(KEmptyMimeType, "");
_LIT(KPosLandmarksServerName, "*eposlmserver*");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp51::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp51::StartL()
    {
    iErrorsFound = 0;
    MakeSurePanicDebugFileExistsL();

    RemoveDefaultDbL();
    iDatabase = UseGeneratedDbFileL();

    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    // <<<<<<<<<<<<<<Panic testing below>>>>>>>>>>>>>>>>>>>
    iLog->Log(_L(">>>>>>>Start Panic test<<<<<<<<<"));
        
    //	1 The client asks the API to parse XML content both synchronously 
    // and incrementally without specifying the input method. 
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 1"));
    iTestCase = 1;
    iExpectedErrorCode = EPosLmProtocolBreak;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();

    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 2"));
    iTestCase = 2;
    iExpectedErrorCode = EPosLmProtocolBreak;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();

    // Import landmarks with an invalid transfer option
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 3"));
    iTestCase = 3;
    iExpectedErrorCode = EPosLmInvalidArgument;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
     // Import landmarks with an invalid transfer option
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 9"));
    iTestCase = 9;
    iExpectedErrorCode = EPosLmInvalidArgument;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    // Import landmarks with an invalid transfer option
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 10"));
    iTestCase = 10;
    iExpectedErrorCode = EPosLmInvalidArgument;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();

    // Create a parser with incorrect MIME type, should leave
    iExpectedExitType = ExitLeave;
    iLog->Log(_L("Leave test 4"));
    iTestCase = 4;
    iExpectedErrorCode = KErrNotSupported;
    ResumeThreadAndVerifyExitL();

    // Create a parser with incorrect MIME (correct MIME type but with capital letters) type, should leave
    iExpectedExitType = ExitLeave;
    iLog->Log(_L("Leave test 5"));
    iTestCase = 5;
    iExpectedErrorCode = KErrNotSupported;
    ResumeThreadAndVerifyExitL();
    
    // Create a parser with incorrect MIME type, should leave
    iExpectedExitType = ExitLeave;
    iLog->Log(_L("Leave test 18"));
    iTestCase = 18;
    iExpectedErrorCode = KErrNotSupported;
    ResumeThreadAndVerifyExitL();
    
#if 0
    // Create a parser with incorrect MIME type, should leave
    // TE: the REComSession::CreateImplementationL( leaves with KErrArgument which I think is acceptable
    // Nokia: This is not part of API definition, case removed
    iExpectedExitType = ExitLeave;
    iLog->Log(_L("Leave test 19"));
    iTestCase = 19;
    iExpectedErrorCode = KErrArgument;
    ResumeThreadAndVerifyExitL();
#endif    

    // Create a parser, parse inc. but change input method after one step
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Leave test 6"));
    iTestCase = 6;
    iExpectedErrorCode = EPosLmProtocolBreak;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();

#ifndef __WINS__
    // Fetch Import Iterator with NULL argument
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 7"));
    iTestCase = 7;
    iExpectedErrorCode = 3; // Kern-Exec 3
    ResumeThreadAndVerifyExitL();
    ListAllServers();
    if (!ServerAlive(KPosLandmarksServerName))
        {
        iLog->Log(_L("Server not alive as expected(1)!"));
        User::Leave(-1);
        
        }

    // Use a CPosLmOperation object from a compact operation as input to an
    // ImportedLandmarksIteratorL function call
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 8"));
    iTestCase = 8;
    iExpectedErrorCode = 3; // Kern-Exec 3
    ResumeThreadAndVerifyExitL();
    ListAllServers();
    if (!ServerAlive(KPosLandmarksServerName))
        {
        iLog->Log(_L("Server not alive as expected(2)!"));
        User::Leave(-1);
        
        }
#endif

    // Import subset of landmarks with non existing ids
    iLog->Log(_L("Panic test 11"));
    iTestCase = 11;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = EPosInvalidIndex;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    // Import subset of landmarks with empty subset of ids
    // should leave with KErrArgument
    iLog->Log(_L("Panic test 12"));
    iTestCase = 12;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrArgument;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    // Try to receive parsed landmark with invalid index
    iLog->Log(_L("Panic test 13"));
    iTestCase = 13;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = EPosInvalidIndex;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    // Try to ParseContentL from empty buffer
    iLog->Log(_L("Panic test 14"));
    iTestCase = 14;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrPosLmUnknownFormat;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    // Try toParseContentL from NULL input buffer
    iLog->Log(_L("Panic test 15"));
    iTestCase = 15;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = 3; // Kern-Exec 3
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    // Try to ImportLandmarks from empty buffer
    iLog->Log(_L("Panic test 16"));
    iTestCase = 16;
    iExpectedExitType = ExitLeave;
    iExpectedErrorCode = KErrPosLmUnknownFormat;
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    // Try to ImportLandmarks from NULL buffer
    iLog->Log(_L("Panic test 17"));
    iTestCase = 17;
    iExpectedExitType = ExitPanic;
    iExpectedErrorCode = 3; // Kern-Exec 3
    iMimeType.Set( KLmxMimeType );
    ResumeThreadAndVerifyExitL();
    iMimeType.Set( KGpxMimeType );
    ResumeThreadAndVerifyExitL();
    
    iLog->Log(_L(">>>>>>>Panic test Done<<<<<<<<<"));

    if (iErrorsFound != KErrNone)
        {
        iLog->Log(_L("Errors found in TP51"));
        User::Leave(-2);
        }
    }

// ---------------------------------------------------------
// CPosTp51::ResumeThreadAndVerifyExitL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp51::ResumeThreadAndVerifyExitL()
    {
    iLog->Log(_L("ResumeThreadAndVerifyExitL"));
    _LIT(KPanicErr, "ERROR: Method didn't panic, exit type %d"); 
    _LIT(KPanicCodeErr, "ERROR: Unexpected panic code, actual %d, expected %d");
    _LIT(KLeaveErr, "ERROR: Method didn't leave, exit type %d"); 
    _LIT(KLeaveCodeErr, "ERROR: Unexpected leave error, actual %d, expected %d");

    CreateThreadL();
    TRequestStatus status;
    iThread.Logon(status);
    iThread.Resume();
        
    User::WaitForRequest(status);
    
    if (iExpectedExitType == ExitPanic)
        {
        if (iThread.ExitType() != EExitPanic )
            {
            iLog->Log( KPanicErr, iThread.ExitType() );
            iErrorsFound++;
            }
        if ( iThread.ExitReason() != iExpectedErrorCode )
            {
            iLog->Log( KPanicCodeErr, iThread.ExitReason(), iExpectedErrorCode );
            iErrorsFound++;
            }
        }
    else if (iExpectedExitType == ExitLeave)
        {
        if (iThread.ExitType() != EExitKill )
            {
            iLog->Log( KLeaveErr, iThread.ExitType() );
            iErrorsFound++;
            }
        if ( iThread.ExitReason() != iExpectedErrorCode )
            {
            iLog->Log( KLeaveCodeErr, iThread.ExitReason(), iExpectedErrorCode );
            iErrorsFound++;
            }
        }
    iThread.Close();
    }

// ---------------------------------------------------------
// CPosTp51::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp51::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iThread.Close();

    delete iDatabase;
    iDatabase = NULL;

    iLog->Log(_L("CloseTest Done"));
	}

// ---------------------------------------------------------
// CPosTp51::RunPanicTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp51::RunPanicTestL(TAny* aData)
    {
    CPosTp51* self = reinterpret_cast<CPosTp51*>(aData);

	_LIT(KXML1, "c:\\system\\test\\TestData\\TP47_UTF-16-bigEndian.xml");
	_LIT(KXML2, "c:\\system\\test\\TestData\\TP47_UTF-16-littleEndian.xml");
	_LIT(KXML3, "c:\\system\\test\\TestData\\TP47_UTF-16-bigEndian.gpx");
	_LIT(KXML4, "c:\\system\\test\\TestData\\TP47_UTF-16-littleEndian.gpx");

     if (self->iTestCase == 1)
        {
         // Create parser 
        CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         
        //Should Panic when no input is set, parse sync
        ExecuteAndDeleteLD(landmarkParser->ParseContentL());
        }
     else if (self->iTestCase == 2)
        {
         // Create parser, 
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         // Parse content, should panic when no input set, parse incrementally
         CPosLmOperation* operation = landmarkParser->ParseContentL();
         self->RunAsyncOperationLD(operation);
        }
     else if (self->iTestCase == 3)
        {
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         // Importlandmarks with invalid transfer option, should panic
         ExecuteAndDeleteLD(database->ImportLandmarksL(*landmarkParser, 666));
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 4)
        {
         // Create parser with incorrect MIME type, should leave with KErrNotSupported
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(KNonExistingMimeType2);
         CleanupStack::PushL(landmarkParser);
         CleanupStack::PopAndDestroy(landmarkParser);
        }
     else if (self->iTestCase == 5)
        {
         // Create parser with incorrect MIME (including capital letters) type, should leave with KErrNotSupported
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(KNonExistingMimeType1);
         CleanupStack::PushL(landmarkParser);
         CleanupStack::PopAndDestroy(landmarkParser);
        }
     else if (self->iTestCase == 18)
        {
         // Create parser with incorrect MIME type (long string), should leave with KErrNotSupported
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(KNonExistingMimeType2);
         CleanupStack::PushL(landmarkParser);
         CleanupStack::PopAndDestroy(landmarkParser);
        }
    else if (self->iTestCase == 19)
        {
         // Create parser with incorrect MIME (empty string) type, should leave with KErrNotSupported (or KErrArgument)
         // however the REComSession::CreateImplementationL( leaves with KErrArgument which I think is acceptable
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(KEmptyMimeType);
         CleanupStack::PushL(landmarkParser);
         CleanupStack::PopAndDestroy(landmarkParser);
        }
     else if (self->iTestCase == 6)
        {
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         landmarkParser->SetInputFileL(KXML1);
         // Call ParseContentL
         CPosLmOperation* operation = landmarkParser->ParseContentL();
         TRequestStatus status = KPosLmOperationNotComplete;
         TReal32 progress;

         // Set new input method before first operation has completed
         landmarkParser->SetInputFileL(KXML2);

         // Use first operation object->should panic
         operation->NextStep(status, progress);
         User::WaitForRequest(status);

         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
     else if (self->iTestCase == 7)
        {
        // <<<<<<<<<<<<<< Fetch iterator with CPosLmOperation == NULL >>>>>>>>>>>>>>>>>>>
        CPosLandmarkDatabase* databasen = self->OpenDatabaseLC();
        CPosLmItemIterator* iterator = databasen->ImportedLandmarksIteratorL(NULL);
        delete iterator;
        CleanupStack::PopAndDestroy(databasen);
        }
     else if (self->iTestCase == 8)
        {
        // <<<<<<<<<<<<<< Fetch iterator with CPosLmOperation != ImportLandmarksL() >>>>>>>>>>>>>>>>>>>
        CPosLandmarkDatabase* databasen = self->OpenDatabaseLC();
        CPosLmOperation* operation = databasen->CompactL();
        CleanupStack::PushL(operation);
        CPosLmItemIterator* iterator = databasen->ImportedLandmarksIteratorL(operation);
        delete iterator;
        CleanupStack::PopAndDestroy(2, databasen);
        }
    else if (self->iTestCase == 9)
        {
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         // Importlandmarks with invalid transfer option, should panic
         ExecuteAndDeleteLD(database->ImportLandmarksL(*landmarkParser, 0x17));
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 10)
        {
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         RArray<TUint> subset;
         CleanupClosePushL(subset);
         subset.Append(12);subset.Append(13);
         // Importlandmarks with invalid transfer option, should panic
         ExecuteAndDeleteLD(database->ImportLandmarksL(*landmarkParser, subset, 0x17));
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 11)
        {
        // Importlandmarks with non existing subset of ids, should panic
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         if ( self->iMimeType != KGpxMimeType )
             {
             landmarkParser->SetInputFileL(KXML1);
             }
         else
             {
             landmarkParser->SetInputFileL(KXML3);
             }
         RArray<TUint> subset;
         CleanupClosePushL(subset);
         subset.Append(1);subset.Append(2);subset.Append(1200);subset.Append(1300);
         // Importlandmarks with non existing subset of ids, should panic
         // Should it Panic with EPosInvalidIndex
         ExecuteAndDeleteLD(database->ImportLandmarksL(*landmarkParser, subset, CPosLandmarkDatabase::EIncludeCategories));
         CleanupStack::PopAndDestroy(&subset);
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 12)
        {
        // Importlandmarks with empty subset of id, leaves with KErrArgument
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         landmarkParser->SetInputFileL(KXML1);
         RArray<TUint> subset;
         CleanupClosePushL(subset);
         // Importlandmarks with empty subset of ids, should leave with KErrArgument
         ExecuteAndDeleteLD(database->ImportLandmarksL(*landmarkParser, subset, CPosLandmarkDatabase::EIncludeCategories));
         CleanupStack::PopAndDestroy(&subset);
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }        
    else if (self->iTestCase == 13)
        {
        // Retreive parsed landmark with index outside interval of parsed landmarks
         CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         if ( self->iMimeType != KGpxMimeType )
             {
             landmarkParser->SetInputFileL(KXML1);
             }
         else
             {
             landmarkParser->SetInputFileL(KXML3);
             }
         
         ExecuteAndDeleteLD(landmarkParser->ParseContentL());
         
         // Should panic with EPosInvalidIndex
         landmarkParser->LandmarkLC(2000);
         
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 14)
        {
        // ParseContentL from empty buffer
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         HBufC8* bufferContent = HBufC8::NewLC(0);
         landmarkParser->SetInputBuffer(*bufferContent);
         
         CPosLmOperation* operation = landmarkParser->ParseContentL();
         ExecuteAndDeleteLD(operation);
         
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 15)
        {
        // ParseContentL from NULL input buffer
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         HBufC8* bufferContent = NULL;
         landmarkParser->SetInputBuffer(*bufferContent);
         
         ExecuteAndDeleteLD(landmarkParser->ParseContentL());
         
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }
    else if (self->iTestCase == 16)
        {
        // ImportLandmarks from empty buffer
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         HBufC8* bufferContent = HBufC8::NewLC(0);
         landmarkParser->SetInputBuffer(*bufferContent);
         
         CPosLmOperation* operation = database->ImportLandmarksL(*landmarkParser, CPosLandmarkDatabase::EIncludeCategories);
         ExecuteAndDeleteLD(operation);
         
         CleanupStack::PopAndDestroy(bufferContent);
         CleanupStack::PopAndDestroy(landmarkParser);
         CleanupStack::PopAndDestroy(database);
        }        
    else if (self->iTestCase == 17)
        {
        // ImportLandmarks from NULL input buffer
        CPosLandmarkDatabase* database = self->OpenDatabaseLC();
         // Create parser
         CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(self->iMimeType);
         CleanupStack::PushL(landmarkParser);
         HBufC8* bufferContent = NULL;
         landmarkParser->SetInputBuffer(*bufferContent);
         
         CPosLmOperation* operation = database->ImportLandmarksL(*landmarkParser, CPosLandmarkDatabase::EIncludeCategories);
         ExecuteAndDeleteLD(operation);
         CleanupStack::PopAndDestroy(landmarkParser);
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

    TRAPD(err, CPosTp51::RunPanicTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp51::ListAllServers
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp51::ListAllServers()
    {
	TFindServer find(_L("*"));
    TFullName   name;
    
    iLog->Log(_L("ListAllServers"));

	while (find.Next(name) == KErrNone)
		{
		iLog->Log(name);
        

		}
	iLog->Log(_L("--------------------"));
    }
    
// ---------------------------------------------------------
// CPosTp51::CreateThreadL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp51::CreateThreadL()
    {
    _LIT(KThreadName, "Tp51test%dthread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

	TBuf<100> buf;
	buf.Format(KThreadName, iTestCase, ++iThreadCounter);
	
    TInt err=0;
    err = iThread.Create(buf, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    }

//  End of File
