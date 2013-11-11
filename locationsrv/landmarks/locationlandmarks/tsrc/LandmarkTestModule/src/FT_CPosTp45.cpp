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

#include "FT_CPosTp45.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <bautils.h>


// CONSTANTS
const TInt KNoCategoriesTp45  = 100;
// Filename with 260 characters
_LIT(KLongFileName, "c:\\File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File12");
// Filename with 264 characters
_LIT(KLongFileName2, "c:\\File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File12.txt");

_LIT(KDefaultOptions,   "c:\\system\\test\\testdata\\TP45-DefOptions.xml"); 
_LIT(KInclCategories,   "c:\\system\\test\\testdata\\TP45-IncCategories.xml"); 
_LIT(KExclCategories,   "c:\\system\\test\\testdata\\TP45-ExcCategories.xml");
_LIT(KOneLandmark,      "c:\\system\\test\\testdata\\TP45-OneLandmark.xml");
_LIT(KTwoLandmarks,     "c:\\system\\test\\testdata\\TP45-TwoLandmarks.xml");
_LIT(KTwoLandmarksWithColData, "c:\\system\\test\\testdata\\TP45-TwoLandmarksWithCollData.xml");

_LIT(KFileSync,         "c:\\documents\\TP45-DefOptions.xml"); 
_LIT(KFileInc,          "c:\\documents\\TP45-DefOptionsInc.xml");     
_LIT(KFileWait4Req,     "c:\\documents\\TP45-DefOptionsWait4Req.xml");     
_LIT(KBufferSync,       "c:\\documents\\TP45-DefOptionsBuffer.xml");                      
_LIT(KBufferInc,        "c:\\documents\\TP45-DefOptionsBufferInc.xml"); 
_LIT(KBufferWait4Req,   "c:\\documents\\TP45-DefOptionsBufferWait4Req.xml"); 

_LIT(KFileSync2,        "c:\\documents\\TP45-IncCategories.xml");
_LIT(KFileInc2,         "c:\\documents\\TP45-IncCategoriesInc.xml");
_LIT(KFileWait4Req2,    "c:\\documents\\TP45-IncCategoriesWait4Req.xml");
_LIT(KBufferSync2,      "c:\\documents\\TP45-IncCategoriesBuffer.xml");
_LIT(KBufferInc2,       "c:\\documents\\TP45-IncCategoriesBufferInc.xml");
_LIT(KBufferWait4Req2,  "c:\\documents\\TP45-IncCategoriesBufferWait4Req.xml");

_LIT(KFileSync3,        "c:\\documents\\TP45-ExcCategories.xml");
_LIT(KFileInc3,         "c:\\documents\\TP45-ExcCategoriesInc.xml");
_LIT(KFileWait4Req3,    "c:\\documents\\TP45-ExcCategoriesWait4Req.xml");
_LIT(KBufferSync3,      "c:\\documents\\TP45-ExcCategoriesBuffer.xml");
_LIT(KBufferInc3,       "c:\\documents\\TP45-ExcCategoriesBufferInc.xml");
_LIT(KBufferWait4Req3,  "c:\\documents\\TP45-ExcCategoriesBufferWait4Req.xml");

_LIT(KFileSync4,        "c:\\documents\\TP45-OneLandmark.xml");
_LIT(KFileInc4,         "c:\\documents\\TP45-OneLandmarkInc.xml");
_LIT(KFileWait4Req4,    "c:\\documents\\TP45-OneLandmarkWait4Req.xml");

_LIT(KFileSync5,        "c:\\documents\\TP45-TwoLandmarks.xml");
_LIT(KFileInc5,         "c:\\documents\\TP45-TwoLandmarksInc.xml");
_LIT(KFileWait4Req5,    "c:\\documents\\TP45-TwoLandmarksWait4Req.xml");

_LIT(KFileSync6,        "c:\\documents\\TP45-TwoLandmarksWithCollData.xml");

_LIT(KFileCancelled,    "c:\\documents\\TP45-Cancelled.xml");

_LIT(KFileCancelledInCallback, "c:\\documents\\TP45-CancelledInCallback.xml");

_LIT(KTmpFile, "c:\\documents\\TMP.xml");

//const TPosLmItemId KGlobalCategoryId = 2;
//const TPosLmItemId KGlobalCategoryId2 = 4;
const TPosLmItemId KGlobalCategoryId = 5;
const TPosLmItemId KGlobalCategoryId2 = 9;

const TPosLmItemId KLandmarkId = 3;
const TPosLmItemId KLandmarkId2 = 1;

const TInt KMaxCancelled = 10;

const TInt KMaxCancelledInCallback = 50;

_LIT(KCollectionName, "TP45-CollectionName");
_LIT(KCollectionDes, "TP45-Description");


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp45::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp45::CloseTest()
    {
    iLog->Log(_L("CloseTest"));   
    
    delete iDatabase;
    delete iLandmarkEncoder;    
    delete iBuffer;   
    
    // Added for BC testing         
    delete iCategoryManager;
    iGlobalCategoryIds.Close();
    iGlobalCategoryNames.Close();    
    iGlobalCategoryTestNames.Close(); 
    
    iCategories.ResetAndDestroy();
    iCategories.Close();  
    RemoveGlobalCategoriesL();
    }
// ---------------------------------------------------------
// CPosTp45::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp45::StartL()
    { 
    BaflUtils::EnsurePathExistsL( iFileSession, KTmpFile );
    
    RemoveDefaultDbL();
    RemoveGlobalCategoriesL();
  
    iDatabase = CPosLandmarkDatabase::OpenL();        

    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
       
   
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    ExecuteAndDeleteLD(iDatabase->CompactL());          

    // Setup global category info
    SetupGlobalCategoriesL(); 
    
    AddLandmarksL();
    AddCategoriesL();
  
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    
    TestErrorCodesL();
    
    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
    
    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);
        
    TInt numberOfElements = iter->NumOfItemsL();    
    iter->GetItemIdsL(arrayOfIds, 0, (numberOfElements));   
    
    iLogToFile = ETrue;
    TBool useWaitForRequest = ETrue;

// Exporting all landmarks to file
    iLog->Log(_L("Exporting all landmarks syncronously to file")); 
    ExportLandmarksL(KFileSync, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions);
    	
   	iLog->Log(_L("Exporting all landmarks incrementally to file")); 
    ExportLandmarksIncL(KFileInc, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, !useWaitForRequest);
             
   	iLog->Log(_L("Exporting all landmarks incrementally using User::WaitForRequest() to file")); 
    ExportLandmarksIncL(KFileWait4Req, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, useWaitForRequest);
             
    RenameGlobalCategoryL();

// Exporting with global cateogries to file
    iLog->Log(_L("Exporting all landmarks with global categories names syncronously to file")); 
    ExportLandmarksL(KFileSync2, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories | 
                                     CPosLandmarkDatabase::EIncludeGlobalCategoryNames);      
                                     
    iLog->Log(_L("Exporting all landmarks with global categories names incrementally to file")); 
    ExportLandmarksIncL(KFileInc2, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories | 
                                     CPosLandmarkDatabase::EIncludeGlobalCategoryNames,
                                     !useWaitForRequest);
     
    iLog->Log(_L("Exporting all landmarks with global categories names incrementally using User::WaitForRequest() to file")); 
    ExportLandmarksIncL(KFileWait4Req2, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories | 
                                     CPosLandmarkDatabase::EIncludeGlobalCategoryNames,
                                     useWaitForRequest);

// Exporting without global cateogries to file
    iLog->Log(_L("Exporting all landmarks with categories exc. global categories syncronously to file")); 
    ExportLandmarksL(KFileSync3, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories);
                          
    iLog->Log(_L("Exporting all landmarks with categories exc. global categories incrementally to file")); 
    ExportLandmarksIncL(KFileInc3, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories, !useWaitForRequest);
      
    iLog->Log(_L("Exporting all landmarks with categories exc. global categories incrementally using User::WaitForRequest() to file")); 
    ExportLandmarksIncL(KFileWait4Req3, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories, useWaitForRequest);
      
    iLogToFile = EFalse;
    
// Exporting all landmarks to buffer
    iLog->Log(_L("Exporting all landmarks syncronously to buffer")); 
    ExportLandmarksL(KBufferSync, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions);
    	
   	iLog->Log(_L("Exporting all landmarks incrementally to buffer")); 
    ExportLandmarksIncL(KBufferInc, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, !useWaitForRequest);
              
   	iLog->Log(_L("Exporting all landmarks incrementally using User::WaitForRequest() to buffer")); 
    ExportLandmarksIncL(KBufferWait4Req, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, useWaitForRequest);
       
// Exporting all landmarks with global categories to buffer
    iLog->Log(_L("Exporting all landmarks with global categories names syncronously to buffer")); 
    ExportLandmarksL(KBufferSync2, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories | 
                                     CPosLandmarkDatabase::EIncludeGlobalCategoryNames);      
                                     
    iLog->Log(_L("Exporting all landmarks with global categories names incrementally to buffer")); 
    ExportLandmarksIncL(KBufferInc2, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories | 
                                     CPosLandmarkDatabase::EIncludeGlobalCategoryNames,
                                     !useWaitForRequest);
      
    iLog->Log(_L("Exporting all landmarks with global categories names incrementally using User::WaitForRequest() to buffer")); 
    ExportLandmarksIncL(KBufferWait4Req2, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories | 
                                     CPosLandmarkDatabase::EIncludeGlobalCategoryNames,
                                     useWaitForRequest);

// Exporting all landmarks with global categories to buffer
    iLog->Log(_L("Exporting all landmarks with categories exc. global categories syncronously to buffer")); 
    ExportLandmarksL(KBufferSync3, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories);
                           
    iLog->Log(_L("Exporting all landmarks with categories exc. global categories incrementally to buffer")); 
    ExportLandmarksIncL(KBufferInc3, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories, !useWaitForRequest);
      
    iLog->Log(_L("Exporting all landmarks with categories exc. global categories incrementally using User::WaitForRequest() to buffer")); 
    ExportLandmarksIncL(KBufferWait4Req3, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories, useWaitForRequest);
      
    iLogToFile = ETrue;
    CleanupStack::PopAndDestroy(iter);  
    arrayOfIds.Reset();

// Exporting one landmark to file    
    arrayOfIds.Append(KLandmarkId);
    iLog->Log(_L("Exporting one landmark syncronously to file")); 
    ExportLandmarksL(KFileSync4, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions);
    
    iLog->Log(_L("Exporting one landmark syncronously to file incrementally")); 
    ExportLandmarksIncL(KFileInc4, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, !useWaitForRequest);
    
    iLog->Log(_L("Exporting one landmark syncronously to file incrementally using User::WaitForRequest()")); 
    ExportLandmarksIncL(KFileWait4Req4, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, useWaitForRequest);
    
// Exporting two landmarks to file    
    arrayOfIds.Append(KLandmarkId2);
    iLog->Log(_L("Exporting two landmark syncronously to file")); 
    ExportLandmarksL(KFileSync5, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions);
    
    iLog->Log(_L("Exporting two landmark syncronously to file incrementally")); 
    ExportLandmarksIncL(KFileInc5, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, !useWaitForRequest);
    
    iLog->Log(_L("Exporting two landmark syncronously to file incrementally using User::WaitForRequest()")); 
    ExportLandmarksIncL(KFileWait4Req5, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions, useWaitForRequest);
    
// Exporting landmarks with collection data    
    iLog->Log(_L("Exporting landmarks with collection data added to the encoder")); 
    ExportWithCollectionDataL(arrayOfIds, CPosLandmarkDatabase::EDefaultOptions);
    
    iLog->Log(_L("Compares the created XML files with saved correct ones")); 
    
    TInt errors = 0;
    
    errors += CompareL(KDefaultOptions, KFileSync);
    errors += CompareL(KDefaultOptions, KFileInc);
    errors += CompareL(KDefaultOptions, KFileWait4Req);
    errors += CompareL(KDefaultOptions, KBufferSync);
    errors += CompareL(KDefaultOptions, KBufferInc);
    errors += CompareL(KDefaultOptions, KBufferWait4Req);

    TFileName filename;
    CreateCorrectXmlFileL( KInclCategories, filename );
    errors += CompareL(filename, KFileSync2);
    errors += CompareL(filename, KFileInc2);
    errors += CompareL(filename, KFileWait4Req2);
    errors += CompareL(filename, KFileInc2);
    errors += CompareL(filename, KBufferSync2);
    errors += CompareL(filename, KBufferInc2);
    errors += CompareL(filename, KBufferWait4Req2);

    CreateCorrectXmlFileL( KExclCategories, filename );
    errors += CompareL(filename, KFileSync3);
    errors += CompareL(filename, KFileInc3);
    errors += CompareL(filename, KFileWait4Req3);
    errors += CompareL(filename, KBufferSync3);
    errors += CompareL(filename, KBufferInc3);
    errors += CompareL(filename, KBufferWait4Req3);

    errors += CompareL(KOneLandmark, KFileSync4);
    errors += CompareL(KOneLandmark, KFileInc4);
    errors += CompareL(KOneLandmark, KFileWait4Req4);

    errors += CompareL(KTwoLandmarks, KFileSync5);
    errors += CompareL(KTwoLandmarks, KFileInc5);
    errors += CompareL(KTwoLandmarks, KFileWait4Req5);

    errors += CompareL(KTwoLandmarksWithColData, KFileSync6);
  
    if (errors > 0)
        {
        errors = errors / KErrGeneral;
        }

		AssertTrueSecL(errors == 0, _L("%d file(s) are not equal"), errors);
	
		delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = UseGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    
    iLog->Log(_L("Exports landmarks and cancels the operation")); 
    ExportAndCancelL(KFileCancelled, EFalse, CPosLandmarkDatabase::EDefaultOptions);
    
    iLog->Log(_L("Exports landmarks and cancels the operation in a callback")); 
    ExportAndCancelL(KFileCancelledInCallback, ETrue, CPosLandmarkDatabase::EDefaultOptions);
   
		VerifyCancelL(KFileCancelled, KMaxCancelled);
	
		VerifyCancelL(KFileCancelledInCallback, KMaxCancelledInCallback);

    CleanupStack::PopAndDestroy(&arrayOfIds);  
    
  	}

// ---------------------------------------------------------
// CPosTp45::ExportLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::ExportLandmarksL(const TDesC& aFileName, 
                                RArray<TPosLmItemId>& aArrayOfIds, 
                                CPosLandmarkDatabase::TTransferOptions aTransferOptions)
    {
    iLog->Log(_L("ExportLandmarksL"));
    TBuf<100> file;
    file.Append(aFileName);
        
    if (iLogToFile)
        {        
        DeleteFile(file);
        iLandmarkEncoder->SetOutputFileL(file);
        }
    else
        {
        iBuffer = iLandmarkEncoder->SetUseOutputBufferL(); 
        }
    
    CPosLmOperation* op = iDatabase->ExportLandmarksL(*iLandmarkEncoder, aArrayOfIds, aTransferOptions); 
    CleanupStack::PushL(op);
    op->ExecuteL();
    CleanupStack::PopAndDestroy(op);
    
    op = iLandmarkEncoder->FinalizeEncodingL(); 
    CleanupStack::PushL(op);
    op->ExecuteL();
    CleanupStack::PopAndDestroy(op);
    
    if (!iLogToFile)
        {
        WriteBufferToFileL(iBuffer, file); 
        delete iBuffer;
        iBuffer = NULL;
        }
    }
    
// ---------------------------------------------------------
// CPosTp45::ExportLandmarksIncL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::ExportLandmarksIncL(const TDesC& aFileName,
                                   RArray<TPosLmItemId>& aArrayOfIds, 
                                   CPosLandmarkDatabase::TTransferOptions aTransferOptions,
                                   TBool aUseWaitForRequest)
    {
    iLog->Log(_L("ExportLandmarksIncL"));
    TBuf<100> file;
    file.Append(aFileName);
        
    if (iLogToFile)
        {        
        DeleteFile(file);
        iLandmarkEncoder->SetOutputFileL(file); 
        }
    else
        {
        iBuffer = iLandmarkEncoder->SetUseOutputBufferL(); 
        }
        
    CPosLmOperation* op = iDatabase->ExportLandmarksL(*iLandmarkEncoder, aArrayOfIds, aTransferOptions); 
    if (aUseWaitForRequest)
        {
        CleanupStack::PushL(op);
        RunAsyncOperationByWaitForReqL(op);
        CleanupStack::PopAndDestroy(op);
        }
    else
        {
        RunAsyncOperationLD(op);
        }
    
    op = iLandmarkEncoder->FinalizeEncodingL(); 
    if (aUseWaitForRequest)
        {
        CleanupStack::PushL(op);
        RunAsyncOperationByWaitForReqL(op);
        CleanupStack::PopAndDestroy(op);
        }
    else
        {
        RunAsyncOperationLD(op);
        }
    
    if (!iLogToFile)
        {
        WriteBufferToFileL(iBuffer, file);
        delete iBuffer;
        iBuffer = NULL; 
        }
	}

// ---------------------------------------------------------
// CPosTp45::ExportWithCollectionDataL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::ExportWithCollectionDataL(RArray<TPosLmItemId>& aArrayOfIds, 
                                         CPosLandmarkDatabase::TTransferOptions aTransferOptions)
    {
    iLog->Log(_L("ExportWithCollectionDataL"));
    TBuf<100> file;
    //file.Append(KTargetPath);
    file.Append(KFileSync6);
    
    DeleteFile(file);
    iLandmarkEncoder->SetOutputFileL(file); 
        
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, KCollectionName);
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, KCollectionDes);
      
    CPosLmOperation* op = iDatabase->ExportLandmarksL(*iLandmarkEncoder, aArrayOfIds, aTransferOptions); 
    CleanupStack::PushL(op);
    op->ExecuteL();
    CleanupStack::PopAndDestroy(op);
    
    op = iLandmarkEncoder->FinalizeEncodingL(); 
    CleanupStack::PushL(op);
    op->ExecuteL();
    CleanupStack::PopAndDestroy(op);
    iLog->Log(_L("ExportWithCollectionDataL Done"));
    }

// ---------------------------------------------------------
// CPosTp45::ExportAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::ExportAndCancelL(const TDesC& aFileName, 
                                const TBool& aInCallback, 
                                CPosLandmarkDatabase::TTransferOptions aTransferOptions)
    {
    iLog->Log(_L("ExportAndCancelL"));
    TBuf<100> file;
    //file.Append(KTargetPath);
    file.Append(aFileName);
    
    DeleteFile(file);
    iLandmarkEncoder->SetOutputFileL(file); 
    
    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);
    
    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
    
    TInt numberOfElements = iter->NumOfItemsL();    
    iter->GetItemIdsL(arrayOfIds, 0, (numberOfElements));
    
    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iDatabase->ExportLandmarksL(*iLandmarkEncoder, arrayOfIds, aTransferOptions));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iDatabase->ExportLandmarksL(*iLandmarkEncoder, arrayOfIds, aTransferOptions));
        } 
    
    CPosLmOperation* op = iLandmarkEncoder->FinalizeEncodingL(); 
    CleanupStack::PushL(op);
    op->ExecuteL();
    
    CleanupStack::PopAndDestroy(3, iter);
    iLog->Log(_L("ExportAndCancelL Done"));
    }

// ---------------------------------------------------------
// CPosTp45::VerifyCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	    
void CPosTp45::VerifyCancelL(const TDesC& aFileName, const TInt aMaxExported)
    {
    iLog->Log(_L("VerifyCancelL"));
    TBuf<100> file;
    file.Append(aFileName);
       
    CPosLandmarkParser* parser = CPosLandmarkParser::NewL(KLmxMimeType);
    CleanupStack::PushL(parser);
    
    parser->SetInputFileL(file);
    
    CPosLmOperation* op = parser->ParseContentL(); 
    CleanupStack::PushL(op);
    
    op->ExecuteL();
     
    TInt parsed = parser->NumOfParsedLandmarks();  
    AssertTrueSecL(aMaxExported > parsed , _L("More than %d landmarks have been exported when cancelled"), aMaxExported);
    
    CleanupStack::PopAndDestroy(2, parser);
    iLog->Log(_L("VerifyCancelL Done"));
    }
    
// ---------------------------------------------------------
// CPosTp45::TestErrorCodesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::TestErrorCodesL()
    {
    iLog->Log(_L("TestErrorCodesL"));
    _LIT(KNotFoundErr, "Incorrect error code %d returned from ExecuteL when executed with Lm id not in db");
    
    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);

    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
    
    TInt numberOfElements = iter->NumOfItemsL();    
    iter->GetItemIdsL(arrayOfIds, 0, (numberOfElements));
     
    arrayOfIds.Append(666);
    
    DeleteFile( KTmpFile );

    iLog->Log(_L("SetOutputFileL"));
    iLandmarkEncoder->SetOutputFileL( KTmpFile );
    iLog->Log(_L("After SetOutputFileL"));

    CPosLmOperation* op = iDatabase->ExportLandmarksL(*iLandmarkEncoder, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions);      
    
    TRAPD(err, op->ExecuteL());
    delete op;
   
    CleanupStack::PopAndDestroy(2, iter);
   
    AssertTrueSecL(err == KErrNotFound, KNotFoundErr, err); 

    iLog->Log(_L("TestErrorCodesL Done"));
    }

    
// ---------------------------------------------------------
// CPosTp45::AddLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::AddLandmarksL()
    {
    iLog->Log(_L("AddLandmarksL"));
    CPosLandmark* lm = CreateXMLLandmarkLC(_L("TP45 - LM with all fields set"));
    AddAllXMLFieldsL(lm);    
    iDatabase->AddLandmarkL(*lm);
    
    CPosLandmark* lm2 = CPosLandmark::NewLC();
    lm2->SetLandmarkNameL(_L("TP45 - LM, only name set"));
    iDatabase->AddLandmarkL(*lm2);

    CPosLandmark* lm3 = CreateLandmarkLC(_L("TP45 - LM with categories"));
    iDatabase->AddLandmarkL(*lm3);
    
    CPosLandmark* lm4 = CPosLandmark::NewLC();
    lm4->SetLandmarkNameL(_L("TP45 - LM with Global categories"));
    lm4->AddCategoryL(KGlobalCategoryId);
    lm4->AddCategoryL(KGlobalCategoryId2);  
    iDatabase->AddLandmarkL(*lm4);
  
    CleanupStack::PopAndDestroy(4, lm);
    }

// ---------------------------------------------------------
// CPosTp45::RenameGlobalCategoryL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::RenameGlobalCategoryL()
    {
    /*
    * Rename a global category 
    */

    TBuf<150>info;
    // Get sample global category info     
    TPosLmItemId catId(0);
    
    CPosLandmarkCategory* category = NULL;    
    catId = iCategoryManager->GetGlobalCategoryL( KAccommodation );
    category = iCategoryManager->ReadCategoryLC( catId );
    category->SetCategoryNameL(_L("TP45 - Renamed global"));
    iCategoryManager->UpdateCategoryL( *category );    
    
    CleanupStack::PopAndDestroy( category );    
    }

// ---------------------------------------------------------
// CPosTp45::ExportToFileWithLongFileNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp45::ExportToFileWithLongFileNameL()
    {
    iLog->Log(_L("ExportToFileWithLongFileNameL"));

#ifndef __WINS__
	DeleteFileL(KLongFileName);
	DeleteFileL(KLongFileName2);
#endif

    // 1) Create an encoder object
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    
    TInt errorsFound=KErrNone;

    // 2) Set file with long filename (more than 255 characters)
    TRAPD(err, iLandmarkEncoder->SetOutputFileL(KLongFileName));
    TInt expErr;
    
#ifdef __WINS__
    expErr = KErrBadName;
    if (err != KErrBadName)
#else
    expErr = KErrNone;
    if (err != KErrNone)
#endif
        {
        TBuf<100> buf;
        buf.Format(_L("SetOutputFileL should leave with %d, instead err:%d"),expErr, err);
        errorsFound++;
        iLog->Log(buf);
        }
        
    // 3) Set file with long filename (more than 255 characters)
    TRAP(err, iLandmarkEncoder->SetOutputFileL(KLongFileName2));
#ifdef __WINS__
    expErr = KErrBadName;
    if (err != KErrBadName) 
#else
    expErr = KErrAlreadyExists;
    if (err != KErrAlreadyExists) 
#endif
        {
        TBuf<100> buf;
        buf.Format(_L("SetOutputFileL(2) should leave with %d, instead err:%d"), expErr, err);
        //LogErrorAndLeave(buf);
        errorsFound++;
        iLog->Log(buf);
        }
        
    if (errorsFound != KErrNone)
    {
    	iLog->Log(_L("Wrong error code from SetOutputFileL"));
    	User::Leave(-1);
    }

    iLog->Log(_L("ExportToFileWithLongFileNameL Done"));
    }
    
// ---------------------------------------------------------
// CPosTp45::DeleteFile
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp45::DeleteFile(const TDesC& aFile)
    {
    TInt err = iFileSession.Delete(aFile);
    if (err != KErrNone) 
        {
        TBuf<100> buf;
        buf.Format(_L("Delete of file %S failed with error %d"), &aFile, err);
        iLog->Log(buf);
        }
    }


void CPosTp45::MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
                                     CPosLmCategoryManager& aCatMgr )    
    {
    
    CPosLmOperation* operation1 = aLdb.RemoveAllLandmarksL();
    //Remove all landmarks from default db : Sync call     
    ExecuteAndDeleteLD( operation1 ); 

    RArray<TPosLmItemId> catArray;
    CleanupClosePushL( catArray );
    
    // Get all the categories from the database and delete them if necessary 
    CPosLmItemIterator* iter = aCatMgr.CategoryIteratorL();
    CleanupStack::PushL( iter );

    TUint count = iter->NumOfItemsL();
    if ( count > 0 )
        { // can only be called if there are some items
        iter->GetItemIdsL( catArray, 0, count );
        }
        
    // Remove all the categories here Sync call     
    ExecuteAndDeleteLD(aCatMgr.RemoveCategoriesL( catArray ));    
    CleanupStack::PopAndDestroy( 2 ); // catArray, iter
    }

void CPosTp45::AddCategoriesL()
    {    
    _LIT(KName, "CategoryTP45 - %d");
    TInt err=KErrLocked;
    for (TInt i=0; i<KNoCategoriesTp45; i++)
        {
        TBuf<100> categoryName;
        categoryName.Format(KName,i);

        CPosLandmarkCategory* lmc = CPosLandmarkCategory::NewLC();
        lmc->SetCategoryNameL(categoryName);
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, iCategoryManager->AddCategoryL(*lmc));
            }

        iCategories.Append(lmc);       
        CleanupStack::Pop(lmc);
        }    
    iLog->Log(_L("Adding categories to database done "));
    }
    
// ---------------------------------------------------------
// CPosTp45::SetupGlobalCategoriesL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp45::SetupGlobalCategoriesL()
    {        
    // Testing with only 3 global categories     
    iGlobalCategoryNames.Append(_L("Accommodation"));
    iGlobalCategoryIds.Append(3000);  

    /*iGlobalCategoryNames.Append(_L("Business"));
    iGlobalCategoryIds.Append(6000);  

    iGlobalCategoryNames.Append(_L("Communication"));
    iGlobalCategoryIds.Append(9000);*/
        
	iGlobalCategoryNames.Append(_L("Entertainment"));
    iGlobalCategoryIds.Append(15000);  

    iGlobalCategoryNames.Append(_L("People"));
    iGlobalCategoryIds.Append(27000);
              
    iGlobalCategoryTestNames.Append(_L("Housing"));
    iGlobalCategoryTestNames.Append(_L("Movie"));
    iGlobalCategoryTestNames.Append(_L("Society"));    
    }
    

//  End of File
