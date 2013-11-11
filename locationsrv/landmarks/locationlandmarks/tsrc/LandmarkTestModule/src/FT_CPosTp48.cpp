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
#include "FT_CPosTp48.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <LbsPosition.h>
#include <bautils.h>

// CONSTANTS
_LIT(KXMLFile, "c:\\documents\\Tp48.xml");
// Filename with 260 characters
_LIT(KLongFileName, "c:\\File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File12");
_LIT(KEmptyFileName1, "c:\\");
_LIT(KEmptyFileName2, "");
_LIT(KNonExistingFileName, "c:\\NonExistingFile.xml");

_LIT(KNewGlobalCategory, "OutSight");

const TInt KOrigGlobalCategoryId = 45000;// "Transport"

//#define DEBUG EFalse
#define DEBUG ETrue
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp48::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::CloseTest()
    {
    iLog->Log(_L("CloseTest"));

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    delete iOperation;
    iOperation = NULL;

    delete iDatabase;
    iDatabase=NULL;
    
    delete iEncoderBuffer;
    iEncoderBuffer = NULL;

    iLog->Log(_L("CloseTest Done"));
    }

// ---------------------------------------------------------
// CPosTp48::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::StartL()
    {
    BaflUtils::EnsurePathExistsL( iFileSession, KXMLFile );

    TInt lang = User::Language();
    iLog->Log(_L("StartL"));
   
    // Use file
    DoTestL(ETrue, EFalse);

    // Use buffer
    DoTestL(EFalse, EFalse);

    // Use file, parse XML content before import
    DoTestL(ETrue, ETrue);

    // Use buffer, parse XML content before import
    DoTestL(EFalse, ETrue);

    // Use file, cancel before complete
    iLog->Log(_L("Use file, cancel before complete"));
    DoTestL(ETrue, EFalse, EFalse, ETrue);

    iLog->Log(_L("Use buffer, cancel before complete"));
    // Use buffer, cancel before complete
    DoTestL(EFalse, EFalse, EFalse, ETrue);

    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    // Remove one global category
    iLog->Log(_L("TC_Step10 Delete one global category"));
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    // Remove global category "Hotel global id=4"
    
    TPosLmItemId id = categoryManager->GetGlobalCategoryL(39000);
    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(id));
    
    //Verify removed
    
    id = categoryManager->GetGlobalCategoryL(39000);
    if (id != KPosLmNullItemId) 
    {
    	iLog->Log(_L("Global category has not been removed"));
    	User::Leave(-1);
    }

    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    iLandmarkParser->SetInputFileL(KXMLFile);
    
    iLog->Log(_L("TC_Step11 Import all landmarks"));    
    ExecuteAndDeleteLD(iDatabase->ImportLandmarksL(*iLandmarkParser, CPosLandmarkDatabase::EIncludeCategories));

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    //Verify that global category has been added again, since it is predefined it should be 
    // imported as a global category (otherwise it should be a local category)
   
    id = categoryManager->GetGlobalCategoryL(39000);
    if (id == KPosLmNullItemId) iLog->Log(_L("Global category has not been added"));

	CPosLandmarkCategory* catte = categoryManager->ReadCategoryLC(id);

    if (catte->GlobalCategory() == KPosLmNullGlobalCategory) iLog->Log(_L("  Not global category"));
   
    CleanupStack::PopAndDestroy(catte);

    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    
    CleanupStack::PopAndDestroy(categoryManager);
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    // Rename one global category
    // Rename global category "Shop global id=5" to Nisses BilAffär
    iLog->Log(_L("TC_Step12 Rename one global category"));
    id = categoryManager->GetGlobalCategoryL(15000);
    catte = categoryManager->ReadCategoryLC(id);
    catte->SetCategoryNameL(_L("Nisses BilAffär"));
    categoryManager->UpdateCategoryL(*catte);
    CleanupStack::PopAndDestroy(catte);

    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    iLandmarkParser->SetInputFileL(KXMLFile);
    
    iLog->Log(_L("TC_Step13 Import all landmarks"));
    ExecuteAndDeleteLD(iDatabase->ImportLandmarksL(*iLandmarkParser, CPosLandmarkDatabase::EIncludeCategories));

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    //Verify that global category has been added again, since it is predefined it should be 
    // imported as a global category (otherwise it should be a local category)
    
    TPosLmItemId id1 = categoryManager->GetGlobalCategoryL(15000);
    if (id1 == KPosLmNullItemId) iLog->Log(_L("Global category has not been added"));

    TPosLmItemId id2 = categoryManager->GetCategoryL(_L("Nisses BilAffär"));
    if (id2 == KPosLmNullItemId) iLog->Log(_L("Global category has not been added 2"));

    if (id1 != id2) 
    {
    	iLog->Log(_L("Categories should have the same id"));
    	User::Leave(-1);
    }

    catte = categoryManager->ReadCategoryLC(id1);
    if (catte->GlobalCategory() == KPosLmNullGlobalCategory) iLog->Log(_L("Nisses Bilaffär is no global category"));

    id = categoryManager->GetCategoryL(_L("Shop"));
    if (id != KPosLmNullItemId) iLog->Log(_L("Shop should not exist"));
  
    CleanupStack::PopAndDestroy(catte);
    CleanupStack::PopAndDestroy(categoryManager);

    iLog->Log(_L("TC_Step14 Test import with long file name"));
    ImportFromFileWithLongFileNameL();

    iLog->Log(_L("TC_Step15 Change "));
    DoTestChangeInputMethodL();
    
    if (iErrorsFound != KErrNone)
        {
       
       	iLog->Log(_L("Errors were found in TP48"));
    	User::Leave(-1);
        }

    }

// ---------------------------------------------------------
// CPosTp48::DoTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::DoTestL(TBool aFile, TBool aFullParse, TBool aDoParse, TBool aCancel)
    {
    iLog->Log(_L("DoTestL"));

    // find localized name of needed category
    HBufC* origGlobalCategory = GetGlobalCategoryFromResourceLC( KOrigGlobalCategoryId );

    RArray<TUint> subset;
    CleanupClosePushL(subset);

    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);

    HBufC8* bufferContent = NULL;

    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    if (aFile)
        {
        // Use file for encoding, set output file
        iLandmarkEncoder->SetOutputFileL(KXMLFile);
        }
    else 
        {
        // Use buffer for encoding
        iEncoderBuffer = iLandmarkEncoder->SetUseOutputBufferL();
        }

    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);

    if (aFile)
        {
        iLog->Log(_L("TC_Step2 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step2 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }
    
    iLog->Log(_L("TC_Step3 Import all landmarks Synchronously"));
    ImportLandmarksL(ETrue, aFullParse, CPosLandmarkDatabase::EDefaultOptions, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);

    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    
    if (aFile)
        {
        iLog->Log(_L("TC_Step3 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step3 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }

    iLog->Log(_L("TC_Step3 Import all landmarks Asynchronously"));
    ImportLandmarksL(EFalse, aFullParse, CPosLandmarkDatabase::EDefaultOptions, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);

    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    
    if (aFile)
        {
        iLog->Log(_L("TC_Step3 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step3 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }

    iLog->Log(_L("TC_Step3 Import all landmarks Synchronously"));
    ImportLandmarksL(ETrue, aFullParse, CPosLandmarkDatabase::EDefaultOptions, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);
       
    // Only import a subset of landmarks
    iLog->Log(_L("TC_Step4 Only import a subset of landmarks, Sync"));
    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    
    if (aFile)
        {
        iLog->Log(_L("TC_Step4 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step4 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }
    
    subset.Append(0);
    subset.Append(1);
    subset.Append(2);
    subset.Append(3);

    delete iLandmarks[7];
    delete iLandmarks[6];
    delete iLandmarks[5];
    delete iLandmarks[4];

    iLandmarks.Remove(7);
    iLandmarks.Remove(6);
    iLandmarks.Remove(5);
    iLandmarks.Remove(4);

    ImportLandmarksL(ETrue, aFullParse, CPosLandmarkDatabase::EDefaultOptions, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);
    subset.Reset();

    // Only import a subset of landmarks
    iLog->Log(_L("TC_Step4 Only import a subset of landmarks, Async"));
    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    
    if (aFile)
        {
        iLog->Log(_L("TC_Step4 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step4 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }
    
    subset.Append(0);
    subset.Append(1);
    subset.Append(2);
    subset.Append(3);

    delete iLandmarks[7];
    delete iLandmarks[6];
    delete iLandmarks[5];
    delete iLandmarks[4];

    iLandmarks.Remove(7);
    iLandmarks.Remove(6);
    iLandmarks.Remove(5);
    iLandmarks.Remove(4);

    ImportLandmarksL(EFalse, aFullParse, CPosLandmarkDatabase::EDefaultOptions, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);
    subset.Reset();
    
    iLog->Log(_L("TC_Step5 Only import a subset of landmarks, use EIncludeCategories, ASync"));
    ResetLandmarksDbL();
    subset.Reset();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    
    if (aFile)
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }
    
    subset.Append(0);
    subset.Append(1);
    subset.Append(2);
    subset.Append(3);
    subset.Append(7);

    delete iLandmarks[6];
    delete iLandmarks[5];
    delete iLandmarks[4];

    iLandmarks.Remove(6);
    iLandmarks.Remove(5);
    iLandmarks.Remove(4);

    ImportLandmarksL(EFalse, aFullParse, CPosLandmarkDatabase::EIncludeCategories, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);

    iLog->Log(_L("TC_Step5 Only import a subset of landmarks, use EIncludeCategories, Sync"));
    ResetLandmarksDbL();
    subset.Reset();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    
    if (aFile)
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }
   
    subset.Append(0);
    subset.Append(1);
    subset.Append(2);
    subset.Append(3);
    subset.Append(7);

    delete iLandmarks[6];
    delete iLandmarks[5];
    delete iLandmarks[4];

    iLandmarks.Remove(6);
    iLandmarks.Remove(5);
    iLandmarks.Remove(4);

    ImportLandmarksL(ETrue, aFullParse, CPosLandmarkDatabase::EIncludeCategories, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);
    // Since only EIncludeCategories was used, the global category should not have been overwritten
    CheckGlobalCategoryExistL(*origGlobalCategory, KNewGlobalCategory);

    // Now use EIncludeCategories | EIncludeGlobalCategoryNames which should cause the global cateory name to 
    // be overwritten
    iLog->Log(_L("TC_Step5 Only import a subset of landmarks, use EIncludeCategories | EIncludeGlobalCategoryNames, Sync"));
    subset.Reset();
    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);

    if (aFile)
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }    

    subset.Append(0);
    subset.Append(1);
    subset.Append(2);
    subset.Append(3);
    subset.Append(4);
    subset.Append(5);
    subset.Append(6);
    subset.Append(7);

    ImportLandmarksL(ETrue, aFullParse, CPosLandmarkDatabase::EIncludeCategories | CPosLandmarkDatabase::EIncludeGlobalCategoryNames, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);
    // Since EIncludeCategories| EIncludeGlobalCategoryNames was used, the global category should have been overwritten
    // Dont do check if cancel was performed
    if (!aCancel) CheckGlobalCategoryExistL(KNewGlobalCategory, *origGlobalCategory);

    iLog->Log(_L("TC_Step5 Only import a subset of landmarks, use EIncludeCategories | EIncludeGlobalCategoryNames, Async"));
    ResetLandmarksDbL();
    DeleteFileL(KXMLFile);
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KXMLFile);
    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);

    if (aFile)
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from XML File"));
        iLandmarkParser->SetInputFileL(KXMLFile);
        }
    else
        {
        iLog->Log(_L("TC_Step5 Set input to be parsed from buffer"));
        bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
        TPtr8 ptr = bufferContent->Des();
        iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*bufferContent);
        }

    ImportLandmarksL(EFalse, aFullParse, CPosLandmarkDatabase::EIncludeCategories | CPosLandmarkDatabase::EIncludeGlobalCategoryNames, subset, aDoParse, aCancel);
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    if (!aFile) CleanupStack::PopAndDestroy(bufferContent);
    // Since EIncludeCategories| EIncludeGlobalCategoryNames was used, the global category should have been overwritten
    // Dont do check if cancel was performed
    if (!aCancel) CheckGlobalCategoryExistL(KNewGlobalCategory, *origGlobalCategory);

    CleanupStack::PopAndDestroy(&subset);
    delete iEncoderBuffer;
    iEncoderBuffer = NULL;
    CleanupStack::PopAndDestroy( origGlobalCategory );
	}

// ---------------------------------------------------------
// CPosTp48::ImportLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::ImportLandmarksL(TBool aSync, TBool aFullParse, TUint aTransferOption, const RArray<TUint>&  aLandmarkSubSet, TBool aDoParse, TBool aCancel)
    {
    iLog->Log(_L("ImportLandmarksL"));

    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress = 0;

    if (aFullParse)
        {
        // ParseContent before importing
        ExecuteAndDeleteLD(iLandmarkParser->ParseContentL());
        iLog->Log(_L("After ExecuteAndDeleteLD(iLandmarkParser->ParseContentL());"));
        }
   
    if (aDoParse)
        {
        // Do an "almost full" parsecontent before import
        iOperation = iLandmarkParser->ParseContentL();
        
        
        while (progress <0.7)
            {
            iOperation->NextStep(status, progress);
            // Wait for NextStep to complete
            User::WaitForRequest(status);
            }
        iLog->Log(_L("After aDoParse"));
        delete iOperation;
        iOperation = NULL;
        }

    if (aLandmarkSubSet.Count() == 0)
        {
        iOperation = iDatabase->ImportLandmarksL(*iLandmarkParser, aTransferOption);
        }
    else
        {
        // Import only a subset of the landmarks in parser
        iOperation = iDatabase->ImportLandmarksL(*iLandmarkParser, aLandmarkSubSet,  aTransferOption);
        }

    if (aSync)
        {
        // Synchronous
        ExecuteAndDeleteLD(iOperation);
        iOperation = NULL;
        iLog->Log(_L("After aSync"));
        }
    else
        {
        // Asynchronous
        // Progress us checked in test procedure base
        if (aCancel)
            {
            
            // It is no more possible to do WaitForRequest after ImportLandmarksL
            RunAsyncOperationAndCancelInCallbackLD(iOperation);
            iOperation = NULL;
            iLog->Log(_L("After aCancel"));
            }
        else
            {
            RunAsyncOperationLD(iOperation);
            iOperation = NULL;
            iLog->Log(_L("After RunAsyncOperationLD(iOperation);"));
            }
        }

    if (!aCancel) CheckLandmarksDatabaseL(aTransferOption);

    }

// ---------------------------------------------------------
// CPosTp48::ResetLandmarksDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::ListLandmarksDbL()
    {   
    iLog->Log(_L("ListLandmarksDbL"));

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);
    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);

    iLog->Log(_L("Landmarks:"));
    TPosLmItemId id = iter->NextL();
    TBuf<255> buffe;
    while (id != KPosLmNullItemId)
        {
        TPtrC name;
        
        CPosLandmark* lm = iDatabase->ReadLandmarkLC(id);
        lm->GetLandmarkName(name);

        buffe.Zero();
        buffe.Append(name);
        lm->GetCategoriesL(categories);

        buffe.Append(_L(" With Categories: "));
        iLog->Log(buffe);
        for (TInt i=0;i<categories.Count();i++)
            {
            CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(categories[i]);
            cat->GetCategoryName(name);
            buffe.Zero();
            buffe.Append(name);
            iLog->Log(buffe);
            CleanupStack::PopAndDestroy(cat);
            }
        CleanupStack::PopAndDestroy(lm);
        id = iter->NextL();
        }

    CleanupStack::PopAndDestroy(&categories);
    CleanupStack::PopAndDestroy(iter);

    iter = categoryManager->CategoryIteratorL();
    CleanupStack::PushL(iter);

    iLog->Log(_L("Categories:"));
    id = iter->NextL();
    while (id != KPosLmNullItemId)
        {
        TPtrC name;
        TBuf<255> buffe;

        CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(id);
        cat->GetCategoryName(name);
        buffe.Zero();
        buffe.Append(name);
        iLog->Log(buffe);
        CleanupStack::PopAndDestroy(cat);
        id = iter->NextL();
        }

    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(categoryManager);
    }

// ---------------------------------------------------------
// CPosTp48::ResetLandmarksDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::ResetLandmarksDbL()
    {
    iLog->Log(_L("ResetLandmarksDbL"));
    
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    delete iDatabase;
    iDatabase=NULL;

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

    iLog->Log(_L("ResetLandmarksDbL Done"));
    }

// ---------------------------------------------------------
// CPosTp48::DoTestChangeInputMethodL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::DoTestChangeInputMethodL()
    {
    iLog->Log(_L("DoTestChangeInputMethodL"));
    ResetLandmarksDbL();

    HBufC8* bufferContent = NULL;

    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    iEncoderBuffer = iLandmarkEncoder->SetUseOutputBufferL();

    CreateXMLDataL();
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);

    bufferContent = HBufC8::NewLC(iEncoderBuffer->Size());
    TPtr8 ptr = bufferContent->Des();
    iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
    iLandmarkParser->SetInputBuffer(*bufferContent);

    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress = 0;

    // Do an "almost full" parsecontent
    iOperation = iLandmarkParser->ParseContentL();
    while (progress <0.5)
        {
        iOperation->NextStep(status, progress);
        // Wait for NextStep to complete
        User::WaitForRequest(status);
        }

    delete iOperation;
    iOperation = NULL;
    
    // Change input method
    iLandmarkParser->SetInputFileL(KXMLFile);

    // Now import all XML data from file
    ExecuteAndDeleteLD(iDatabase->ImportLandmarksL(*iLandmarkParser, CPosLandmarkDatabase::EDefaultOptions));
    CheckLandmarksDatabaseL(CPosLandmarkDatabase::EDefaultOptions);

    CleanupStack::PopAndDestroy(bufferContent);

    delete iLandmarkParser;
    iLandmarkParser = NULL;
    }

// ---------------------------------------------------------
// CPosTp48::ImportFromFileWithLongFileNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::ImportFromFileWithLongFileNameL()
    {
    iLog->Log(_L("ImportFromFileWithLongFileNameL"));
    delete iLandmarkParser;
    iLandmarkParser = NULL;

    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    TRAPD(err, iLandmarkParser->SetInputFileL(KLongFileName));
    
    // Error differs between winscw and target
   
    if ( err != KErrBadName ) 
        {
        TBuf<100> buf;    
        buf.Format(_L("ERROR: SetInputFileL should leave with KErrBadName, instead err: %d"), err);  
        iLog->Log(buf);
        iErrorsFound++;        
        }
    
    // Also test empty filename here
    TRAP(err, iLandmarkParser->SetInputFileL(KEmptyFileName1));
    if (err != KErrBadName)
        {
        iLog->Log(_L("ERROR: SetInputFileL(KEmptyFileName1) should leave with KErrBadName"));
        iErrorsFound++;
        }
        
    // Also test empty filename here
    TRAP(err, iLandmarkParser->SetInputFileL(KEmptyFileName2));
    if (err != KErrBadName)
        {
        TBuf<100> buf;
        buf.Format(_L("ERROR: SetInputFileL(KEmptyFileName2) should leave with KErrBadName, instead err: %d"), err);
        iLog->Log(buf);
        iErrorsFound++;
        }    
    
    // Also test non existing filen here
    TRAP(err, iLandmarkParser->SetInputFileL(KNonExistingFileName));
    if (err != KErrNotFound)
        {
        iLog->Log(_L("ERROR: SetInputFileL(KNonExistingFileName) should leave with KErrNotFound"));
        iErrorsFound++;
        }    
    
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    iLog->Log(_L("ImportFromFileWithLongFileNameL Done"));
    }

// ---------------------------------------------------------
// CPosTp48::CheckLandmarksDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::CheckLandmarksDatabaseL(TUint aTransferOption)
    {
    iLog->Log(_L("CheckLandmarksDatabaseL"));
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    TUint attr = CPosLandmark::ELandmarkName;

    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    RArray<TPosLmItemId> sourceArray;
    CleanupClosePushL(sourceArray);

    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    TInt nrOfLandmarks = iLandmarks.Count();
    TPtrC lmName;

    for (TInt i=0;i<nrOfLandmarks; i++)
        {
        iLandmarks[i]->GetLandmarkName(lmName);

        textCriteria->SetAttributesToSearch(attr);
        textCriteria->SetTextL(lmName);

        ExecuteAndDeleteLD(iLandmarkSearch->StartLandmarkSearchL(*textCriteria));
 
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
    
        TUint nr = iLandmarkSearch->NumOfMatches();
        iLog->Log(_L("****** Found %d nr of landmarks when looking for '%S'"),nr, &lmName);
        TPosLmItemId id = iter->NextL();
        
        if (nr > 1)
            {
            // The first landmark is the one existing in db before import,
            // no need to check that one, only the one added from import (the second one)
            id = iter->NextL();
            }
            
        while (id != KPosLmNullItemId)
            {
            CPosLandmark* source = iDatabase->ReadLandmarkLC(id);
            TPtrC sourceName;
            source->GetLandmarkName(sourceName);

            iLog->Log(_L("Found landmark '%S'"), &sourceName);
            
            TInt err = CompareXMLLandmarksL(*iLandmarks[i], *source, DEBUG);
            if (err != KErrNone)
                {
                PrintLandmarkIdFieldsL(*iLandmarks[i]);
                PrintLandmarkIdFieldsL(*source);
                iLog->Log(_L("ERROR: CompareXMLLandmarksL returned error"));
                iErrorsFound+=err;
                }

            source->GetCategoriesL(sourceArray);
            iLandmarks[i]->GetCategoriesL(array);

            CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
            CleanupStack::PushL(categoryManager);

            if (aTransferOption == CPosLandmarkDatabase::EDefaultOptions)
                {
                // No categories imported
                if (sourceArray.Count() != 0)
                    {
                    iLog->Log(_L("ERROR: No categories should have been imported, found %d "), sourceArray.Count());
                    iErrorsFound++;
                    }
                }
            else
                {
                if (sourceArray.Count() != array.Count())
                    {
                    for (TInt j=0;j<sourceArray.Count();j++)
                        {
                        CPosLandmarkCategory* sourceCat = categoryManager->ReadCategoryLC(sourceArray[j]);
                        TPtrC sourceName;
                        sourceCat->GetCategoryName( sourceName );
                        iLog->Log(_L("Found category '%S'"), &sourceName);
                        }
                    iLog->Log(_L("ERROR: Nr of categories differ, was %d should be %d"), sourceArray.Count(), array.Count());
					User::Leave(KErrNone);
                    }

                for (TInt j=0;j<sourceArray.Count();j++)
                    {
                    iLog->Log(_L("Src Id: %d Id: %d"), sourceArray[j], array[j]);
                    
                    CPosLandmarkCategory* sourceCat = categoryManager->ReadCategoryLC(sourceArray[j]);
                    CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(array[j]);
                    TPtrC sourceName, name;
                    cat->GetCategoryName(name);
                    sourceCat->GetCategoryName(sourceName);
                    iLog->Log(_L("Found category, source cat '%S', cat '%S'"), &sourceName, &name);
                    iErrorsFound += CompareXMLCategoriesL(*cat, *sourceCat, DEBUG);
                    CleanupStack::PopAndDestroy(cat);
                    CleanupStack::PopAndDestroy(sourceCat);
                    }
                }
            CleanupStack::PopAndDestroy(categoryManager);

            CleanupStack::PopAndDestroy(source);
            id = iter->NextL();
            }
        CleanupStack::PopAndDestroy(iter);
        }

    CleanupStack::PopAndDestroy(&array);
    CleanupStack::PopAndDestroy(&sourceArray);
    CleanupStack::PopAndDestroy(textCriteria);

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp48::RemoveCategoriesFromLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::RemoveCategoriesFromLandmarkL(CPosLandmark& aLandmark)
    {
    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    aLandmark.GetCategoriesL(array);

    for (TInt i=0;i<array.Count();i++)
        {
        aLandmark.RemoveCategory(array[i]);
        }
    CleanupStack::PopAndDestroy(&array);
    }

// ---------------------------------------------------------
// CPosTp48::CreateXMLDataL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::CreateXMLDataL()
    {
    iLog->Log(_L("CreateXMLDataL"));

    // 3 Add one landmark to be encoded (id7 name "Billigt" in categories 1-3,14-17)
    CPosLandmark* lm1 = iDatabase->ReadLandmarkLC(7);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    // Remove field 408 which is not exported
    lm1->RemovePositionField(408);
    RemoveCategoriesFromLandmarkL(*lm1);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);
    // 4 Add one more landmark to be encoded (id27 "LOG9" in categories 11-17)
    CPosLandmark* lm2 = iDatabase->ReadLandmarkLC(27);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);

    // 5 Create a new category and add to the latest added landmark
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLandmarkCategory* landmarkCategory = categoryManager->ReadCategoryLC(37);
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    CleanupStack::PopAndDestroy(categoryManager);

    RemoveCategoriesFromLandmarkL(*lm2);
    lm2->AddCategoryL(37);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 6 Add one more landmark to be encoded (id52 "Kalles Hundgård" in category 19)
    lm2 = iDatabase->ReadLandmarkLC(52);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);

    // 7 Add several global and local categories to the latest added landmark
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    landmarkCategory = categoryManager->ReadCategoryLC(26);
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);

    landmarkCategory = categoryManager->ReadCategoryLC(25);
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    
    
    TPosLmItemId globalCategoryId = categoryManager->GetGlobalCategoryL(3000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
    }
    else 
    {
    	iLog->Log(_L("Global category was not found1"));
    	User::Leave(-1);
    }

    
    globalCategoryId = categoryManager->GetGlobalCategoryL(15000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else 
    {
    	iLog->Log(_L("Global category was not found2"));
    	User::Leave(-1);	
    }

    CleanupStack::PopAndDestroy(categoryManager);

    RemoveCategoriesFromLandmarkL(*lm2);
    lm2->AddCategoryL(26);
    lm2->AddCategoryL(25);
    lm2->AddCategoryL(27);
    lm2->AddCategoryL(31);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 8 Add one landmark (not containing any categories), id=11 "MLFW"
    lm2 = iDatabase->ReadLandmarkLC(11);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    RemoveCategoriesFromLandmarkL(*lm2);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 9 Add one more landmark (not containing any categories), id=56 "TE, Lund"
    lm2 = iDatabase->ReadLandmarkLC(56);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    
    // 10 Add one category to the latest added landmark
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    landmarkCategory = categoryManager->ReadCategoryLC(18);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    CleanupStack::PopAndDestroy(categoryManager);

    RemoveCategoriesFromLandmarkL(*lm2);
    lm2->AddCategoryL(18);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 11 Add one landmark (not containing any categories), id=43 "asdf"
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    lm2 = iDatabase->ReadLandmarkLC(43);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);

    // 12 Add several local and global categories to the latest added landmark
    landmarkCategory = categoryManager->ReadCategoryLC(23);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    landmarkCategory = categoryManager->ReadCategoryLC(17);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    
    
    globalCategoryId = categoryManager->GetGlobalCategoryL(39000);
    
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else 
        {   
		iLog->Log(_L("Global category was not found3"));
    	User::Leave(-1);
        }

    
    globalCategoryId = categoryManager->GetGlobalCategoryL(27000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else 
        {
    	iLog->Log(_L("Global category was not found4"));
    	User::Leave(-1);
        }
    
    RemoveCategoriesFromLandmarkL(*lm2);
    lm2->AddCategoryL(23);
    lm2->AddCategoryL(17);
    lm2->AddCategoryL(39);
    lm2->AddCategoryL(35);

    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 13) Add a landmark with fields that are encoded to XML set
    lm1 = CreateXMLLandmarkLC(_L("TP48LandmarkWithAllXMLFields"));
    AddAllXMLFieldsL(lm1);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    
    // 13) Add one global category  (Garage) to the latest added landmark
    globalCategoryId = categoryManager->GetGlobalCategoryL(24000);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else 
        {
    	iLog->Log(_L("Global category was not found4"));
    	User::Leave(-1);
        }

    // 14) Add one global category  (Bus stop) to the latest added landmark
    globalCategoryId = categoryManager->GetGlobalCategoryL(42000);//30000
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else 
        {
    	iLog->Log(_L("Global category was not found4"));
    	User::Leave(-1);
        }

    // 15) Add one category (BurgerKing) to the latest added landmark
    landmarkCategory = categoryManager->ReadCategoryLC(17);
     
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);

    RemoveCategoriesFromLandmarkL(*lm1);
    lm1->AddCategoryL(34);
    lm1->AddCategoryL(40);
    lm1->AddCategoryL(17);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);

    CleanupStack::PopAndDestroy(categoryManager);

    // 16 Create new landmark with empty fields
    lm1 = CreateXMLLandmarkLC(_L("TP48Landmark With Empty XML Fields"));
    AddAllEmptyXMLFieldsL(lm1);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    
    // 17 Create a new category and add to the latest added landmark
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    //landmarkCategory = CPosLandmarkCategory::NewLC();
    landmarkCategory = categoryManager->ReadCategoryLC(38);
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);

    // 18 Add global category "Outsight" which is "Sight" id=6 with changed name
    // and add to the latest added landmark
    globalCategoryId = categoryManager->GetGlobalCategoryL(KOrigGlobalCategoryId);
    landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
    landmarkCategory->SetCategoryNameL(KNewGlobalCategory);
    categoryManager->UpdateCategoryL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);

  
    globalCategoryId = categoryManager->GetGlobalCategoryL(KOrigGlobalCategoryId);
    if (globalCategoryId != KPosLmNullItemId)
        {
        landmarkCategory = categoryManager->ReadCategoryLC(globalCategoryId);
        iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
        CleanupStack::PopAndDestroy(landmarkCategory);
        }
    else 
        {
    	iLog->Log(_L("Global category was not found4"));
    	User::Leave(-1);
        }
    
    CleanupStack::PopAndDestroy(categoryManager);

    RemoveCategoriesFromLandmarkL(*lm1);
    // The new category will have id=38
    lm1->AddCategoryL(38);
    lm1->AddCategoryL(globalCategoryId);
    // Remove field 402 which is not included
    lm1->RemovePositionField(402);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);
        
    // 13 Finalize encoding
    //iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    //iLog->Log(_L("FinalizeEncodingL Done"));

    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    //Change back name for this global category, needed to test use of TTransferOptions::EIncludeGlobalCategoryNames
    // by resetting global categories
    ExecuteAndDeleteLD(categoryManager->ResetGlobalCategoriesL());
    CleanupStack::PopAndDestroy(categoryManager);

    iLog->Log(_L("CreateXMLDataL Done"));
    }


// ---------------------------------------------------------
// CPosTp48::CheckGlobalCategoryExistL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::CheckGlobalCategoryExistL(const TDesC& aShouldExistName, const TDesC& aShouldNotExistName)
    {
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    // Should be found
    TPosLmItemId id1 = categoryManager->GetCategoryL(aShouldExistName);
    // Should not be found
    TPosLmItemId id2 = categoryManager->GetCategoryL(aShouldNotExistName);

    if (id1 == KPosLmNullItemId)
        {
        iLog->Log(_L("ERROR: CheckGlobalCategoryExistL Global Category '%S' should be found"),
            &aShouldExistName);
        iErrorsFound++;
        }

    if (id2 != KPosLmNullItemId)
        {
        iLog->Log(_L("ERROR: CheckGlobalCategoryExistL Global Category '%S' should NOT be found"),
            &aShouldNotExistName);
        iErrorsFound++;
        }

    CleanupStack::PopAndDestroy(categoryManager);
    }

// ---------------------------------------------------------
// CPosTp48::PrintLandmarkIdFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp48::PrintLandmarkIdFieldsL(const CPosLandmark& aLandmark)
    {
    TPositionFieldId sourceFieldId = aLandmark.FirstPositionFieldId();
    TLocality loc;
    TBuf<150> buf;    
    TInt err = aLandmark.GetPosition(loc);
    if (err == KErrNone)
        {
        TReal vertAcc(loc.VerticalAccuracy());
        TReal horAcc(loc.HorizontalAccuracy());
        TReal alt(loc.Altitude());
        TReal lon = loc.Longitude();
        TReal lat = loc.Latitude();
        TReal32 sourceR;
        err = aLandmark.GetCoverageRadius(sourceR);
        if (err == KErrNone )
            {
            buf.Format(_L("LM: Long %f Lat %f vertAcc %f horAcc %f alt %f srcRadius %f"), lon, lat, vertAcc, horAcc, alt, sourceR);
            }
        else
            {
            buf.Format(_L("LM: Long %f Lat %f vertAcc %f horAcc %f alt %f "), lon, lat, vertAcc, horAcc, alt);
            }
        iLog->Log(buf);
        }

    while (sourceFieldId != EPositionFieldNone)
        {
        TPtrC sourceValue;
        aLandmark.GetPositionField(sourceFieldId, sourceValue);
        buf.Format(_L("IdField Id: %d Value:"), sourceFieldId);
        buf.Append(sourceValue);
        iLog->Log(buf);
        sourceFieldId = aLandmark.NextPositionFieldId(sourceFieldId);
        }
    }
    


//  End of File
