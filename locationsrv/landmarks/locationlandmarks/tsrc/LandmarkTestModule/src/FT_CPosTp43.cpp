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
#include "FT_CPosTp43.h"

#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkDatabase.h> 
#include <LbsPosition.h>
#include <e32math.h>
#include <bautils.h>

// CONST
_LIT(KFileName, "c:\\documents\\Tp43EncodeOutput.xml");
_LIT(KFileName2, "c:\\documents\\Tp43EncodeOutputIncMode.xml");
_LIT(KFileName3, "c:\\documents\\Tp43EncodeOutputCollectionData1.xml");
_LIT(KFileName4, "c:\\documents\\Tp43EncodeOutputCollectionData2.xml");
_LIT(KFileEmptyCollectionData, "c:\\documents\\Tp43EncodeOutputEmptyCollectionData.xml");
_LIT(KFileEmptyCollectionData2, "c:\\documents\\Tp43EncodeOutputEmptyCollectionData2.xml");
_LIT(KFileLongCollectionFieldName, "c:\\documents\\Tp43EncodeOutputLongCollectionField.xml");

// Filename with 260 characters
_LIT(KLongFileName, "c:\\File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File12");
_LIT(KEmptyFileName, "");
_LIT8(KLongMimeType, "application/vnd.nokia.landmarkcollection+xmlFile123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File123456File12");

_LIT(KFileNameCorrect, "c:\\system\\test\\TestData\\Tp43EncodeOutput_Correct.xml");
_LIT(KFileName2Correct, "c:\\system\\test\\TestData\\Tp43EncodeOutputIncMode_Correct.xml");
_LIT(KFileName3Correct, "c:\\system\\test\\TestData\\Tp43EncodeOutputCollectionData1_Correct.xml");
_LIT(KFileName4Correct, "c:\\system\\test\\TestData\\Tp43EncodeOutputCollectionData2_Correct.xml");
_LIT(KFileEmptyCollectionDataCorrect, "c:\\system\\test\\TestData\\Tp43EncodeOutputEmptyCollectionData_Correct.xml");
_LIT(KFileEmptyCollectionData2Correct, "c:\\system\\test\\TestData\\Tp43EncodeOutputEmptyCollectionData2_Correct.xml");
_LIT(KFileLongCollectionFieldNameCorrect, "c:\\system\\test\\TestData\\Tp43EncodeOutputLongCollectionField_Correct.xml");
_LIT(KErrorScenario4FileCorrect, "c:\\system\\test\\TestData\\Tp43EncodeOutputErrorScenario4_Correct.xml");
_LIT(KErrorScenario5FileCorrect, "c:\\system\\test\\TestData\\Tp43EncodeOutputErrorScenario5_Correct.xml");

_LIT(KErrorTestFileName, "c:\\documents\\Tp43EncodeOutputErrorTest.xml");
_LIT(KErrorScenario4File, "c:\\documents\\Tp43EncodeOutputErrorScenario4.xml");
_LIT(KErrorScenario5File, "c:\\documents\\Tp43EncodeOutputErrorScenario5.xml");

// This number must be updated if any more landmarks are added
const TInt KNrOfAddedLandmarks = 7;

const TInt KBufSize = 1024;

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp43::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::CloseTest()
    {
    iLog->Log(_L("CloseTest"));

    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    delete iDatabase;
    iDatabase=NULL;

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iOperation;
    iOperation = NULL;

    }

// ---------------------------------------------------------
// CPosTp43::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::StartL()
    {
    BaflUtils::EnsurePathExistsL( iFileSession, KFileName );

    DeleteFileL(KFileName);
    DeleteFileL(KFileName2);
    DeleteFileL(KFileName3);
    DeleteFileL(KFileName4);
    DeleteFileL(KFileEmptyCollectionData);
    DeleteFileL(KFileEmptyCollectionData2);


	RemoveAllLmDatabasesL();
	
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
       	
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);

    // Do reset so that global categories are included
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);   
    ExecuteAndDeleteLD(categoryManager->ResetGlobalCategoriesL());
    CleanupStack::PopAndDestroy(categoryManager);

   
    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);

    // 2) Use file for encoding, set output file
    iLandmarkEncoder->SetOutputFileL(KFileName);
    CreateEncoderPartsL();

    // 13 Finalize encoding
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    // Check data
    HBufC8* emptyBuffer=NULL;
    ParseAndCheckDataFromEncoderL(KFileName, _L(""), _L(""), *emptyBuffer);

    // Repeate step 2-12, finalize with incremental mode and monitor progress 
    iLandmarkEncoder->SetOutputFileL(KFileName2);
    CreateEncoderPartsL();

    // 15 Finalize encoding with incremental mode and monitor progress 
    iOperation = iLandmarkEncoder->FinalizeEncodingL();

    CActiveLmOperation* activeLmOperation = new(ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(activeLmOperation);

    // Progress is monitored in CLandmarkTestProcedureBase class
    activeLmOperation->Start(iOperation);
    CActiveScheduler::Start();

    // Check for no errors
    iLog->Log(_L("CheckOperationL"));
    activeLmOperation->CheckOperationL();
    delete iOperation;
    iOperation = NULL;

    ParseAndCheckDataFromEncoderL(KFileName2, _L(""), _L(""), *emptyBuffer);

    CleanupStack::PopAndDestroy(activeLmOperation);
 
    // 16 Repeat step 3-15, use buffer
    CBufBase* myEncoderBuffer = iLandmarkEncoder->SetUseOutputBufferL();
    CleanupStack::PushL(myEncoderBuffer);

    // 17a Repeat step 3-15, use buffer
    iLog->Log(_L("Part 17a Repeat step 3-15 use buffer"));
    CreateEncoderPartsL();

    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    HBufC8* filecontent = HBufC8::NewLC(myEncoderBuffer->Size());
    TPtr8 ptr = filecontent->Des();
    myEncoderBuffer->Read(0, ptr, myEncoderBuffer->Size());
    ParseAndCheckDataFromEncoderL(KNullDesC, _L(""), _L(""), *filecontent);
    myEncoderBuffer->Reset();
    CleanupStack::PopAndDestroy(filecontent);
    CleanupStack::PopAndDestroy(myEncoderBuffer);

    // 17b Repeat step 3-15 use buffer, finalize with incremental mode and monitor progress 
    iLog->Log(_L("Part 17b Repeat step 3-15 use buffer"));
    CBufBase* myEncoderBuffer2 = iLandmarkEncoder->SetUseOutputBufferL();
    CleanupStack::PushL(myEncoderBuffer2);

    CreateEncoderPartsL();
    iOperation = iLandmarkEncoder->FinalizeEncodingL();

    activeLmOperation = new(ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(activeLmOperation);

    // Progress is monitored in CLandmarkTestProcedureBase class
    activeLmOperation->Start(iOperation);
    CActiveScheduler::Start();
    
    // Check for no errors
    iLog->Log(_L("CheckOperationL"));
    activeLmOperation->CheckOperationL();
    delete iOperation;
    iOperation = NULL;

    HBufC8* filecontent2 = HBufC8::NewLC(myEncoderBuffer2->Size());
    TPtr8 ptr2 = filecontent2->Des();
    myEncoderBuffer2->Read(0, ptr2, myEncoderBuffer2->Size());

    ParseAndCheckDataFromEncoderL(KNullDesC, _L(""), _L(""), *filecontent2);

    myEncoderBuffer2->Reset();
    CleanupStack::PopAndDestroy(filecontent2);
    CleanupStack::PopAndDestroy(activeLmOperation);
    CleanupStack::PopAndDestroy(myEncoderBuffer2);

    // 18 Use file when encoding, set output file
    iLog->Log(_L("Part 18 Use file when encoding, set output file"));
    iLandmarkEncoder->SetOutputFileL(KFileName3);

    // 19 Add collection data to the encoder, name and description set
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP43CollectionDataNameFile1"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("TP43CollectionDataDescriptionFile1"));
    
    CreateEncoderPartsL();

    // Finalize encoding
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    // Check data
    ParseAndCheckDataFromEncoderL(KFileName3, _L("TP43CollectionDataNameFile1"), _L("TP43CollectionDataDescriptionFile1"), *emptyBuffer);

    // 20a Repeat step 3-17 Use file when encoding, set output file, finalize with incremental mode and monitor progress 
    iLog->Log(_L("Part 20a "));
    iLandmarkEncoder->SetOutputFileL(KFileName4);

    // Add collection data to the encoder, name and description set
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP43CollectionDataNameFile2"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("TP43CollectionDataDescriptionFile2"));
    
    CreateEncoderPartsL();

    // finalize with incremental mode and monitor progress 
    iOperation = iLandmarkEncoder->FinalizeEncodingL();

    activeLmOperation = new(ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(activeLmOperation);

    // Progress is monitored in CLandmarkTestProcedureBase class
    activeLmOperation->Start(iOperation);
    CActiveScheduler::Start();

    // Check for no errors
    iLog->Log(_L("CheckOperationL"));
    activeLmOperation->CheckOperationL();
    delete iOperation;
    iOperation = NULL;
    CleanupStack::PopAndDestroy(activeLmOperation);

    // Check data
    ParseAndCheckDataFromEncoderL(KFileName4, _L("TP43CollectionDataNameFile2"), _L("TP43CollectionDataDescriptionFile2"), *emptyBuffer);

    // 20b Use buffer when encoding
    iLog->Log(_L("Part 20b "));
    CBufBase* myEncoderBuffer3 = iLandmarkEncoder->SetUseOutputBufferL();
    CleanupStack::PushL(myEncoderBuffer3);

    // Add collection data to the encoder, name and description set
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP43CollectionDataNameBuffer1"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("TP43CollectionDataDescriptionBuffer1"));
    CreateEncoderPartsL();

    // Finalize encoding
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    HBufC8* filecontent3 = HBufC8::NewLC(myEncoderBuffer3->Size());
    TPtr8 ptr3 = filecontent3->Des();
    myEncoderBuffer3->Read(0, ptr3, myEncoderBuffer3->Size());
    ParseAndCheckDataFromEncoderL(KNullDesC, _L("TP43CollectionDataNameBuffer1"), _L("TP43CollectionDataDescriptionBuffer1"), *filecontent3);
    myEncoderBuffer3->Reset();
    CleanupStack::PopAndDestroy(filecontent3);
    CleanupStack::PopAndDestroy(myEncoderBuffer3);


    // 20c Repeat step 3-17 Use buffer when encoding, set output file, finalize with incremental mode and monitor progress 
    iLog->Log(_L("Part 20c "));
    CBufBase* myEncoderBuffer4 = iLandmarkEncoder->SetUseOutputBufferL();
    CleanupStack::PushL(myEncoderBuffer4);

    // Add collection data to the encoder, name and description set
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP43CollectionDataNameBuffer2"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("TP43CollectionDataDescriptionBuffer2"));
    
    CreateEncoderPartsL();

    // finalize with incremental mode and monitor progress 
    iOperation = iLandmarkEncoder->FinalizeEncodingL();

    activeLmOperation = new(ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(activeLmOperation);

    // Progress is monitored in CLandmarkTestProcedureBase class
    activeLmOperation->Start(iOperation);
    CActiveScheduler::Start();

    // Check for no errors
    iLog->Log(_L("CheckOperationL"));
    activeLmOperation->CheckOperationL();
    delete iOperation;
    iOperation = NULL;
    
    HBufC8* filecontent4 = HBufC8::NewLC(myEncoderBuffer4->Size());
    TPtr8 ptr4 = filecontent4->Des();
    myEncoderBuffer4->Read(0, ptr4, myEncoderBuffer4->Size());
    ParseAndCheckDataFromEncoderL(KNullDesC, _L("TP43CollectionDataNameBuffer2"), _L("TP43CollectionDataDescriptionBuffer2"), *filecontent4);
    myEncoderBuffer4->Reset();
    CleanupStack::PopAndDestroy(filecontent4);

    CleanupStack::PopAndDestroy(activeLmOperation);
    CleanupStack::PopAndDestroy(myEncoderBuffer4);

    // 21 Use file when encoding, set output file
    iLog->Log(_L("Part 21 "));
    iLandmarkEncoder->SetOutputFileL(KFileEmptyCollectionData);
    // 22 Add collection data to the encoder, but without specifying name and description
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L(""));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L(""));

    CreateEncoderPartsL();

    // Finalize encoding
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    // Check data
    ParseAndCheckDataFromEncoderL(KFileEmptyCollectionData, _L(""), _L(""), *emptyBuffer);

    // 23a Repeat step 3-17 Use file when encoding, set output file, finalize with incr. mode
    iLog->Log(_L("Part 23a "));
    iLandmarkEncoder->SetOutputFileL(KFileEmptyCollectionData2);
    // Add collection data to the encoder, but without specifying name and description
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L(""));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L(""));

    CreateEncoderPartsL();

    // finalize with incremental mode and monitor progress 
    iOperation = iLandmarkEncoder->FinalizeEncodingL();

    activeLmOperation = new(ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(activeLmOperation);

    // Progress is monitored in CLandmarkTestProcedureBase class
    activeLmOperation->Start(iOperation);
    CActiveScheduler::Start();

    // Check for no errors
    iLog->Log(_L("CheckOperationL"));
    activeLmOperation->CheckOperationL();
    delete iOperation;
    iOperation = NULL;
    CleanupStack::PopAndDestroy(activeLmOperation);
    
    // Check data
    ParseAndCheckDataFromEncoderL(KFileEmptyCollectionData, _L(""), _L(""), *emptyBuffer);

    // 23b Repeat step 3-17 Use buffer when encoding
    iLog->Log(_L("Part 23b "));
    CBufBase* myEncoderBuffer5 = iLandmarkEncoder->SetUseOutputBufferL();
    CleanupStack::PushL(myEncoderBuffer5);

    // Add collection data to the encoder, name and description set
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP43CollectionDataNameBuffer3"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("TP43CollectionDataDescriptionBuffer3"));
    CreateEncoderPartsL();

    // Finalize encoding
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    // Check data here
    HBufC8* filecontent5 = HBufC8::NewLC(myEncoderBuffer5->Size());
    TPtr8 ptr5 = filecontent5->Des();
    myEncoderBuffer5->Read(0, ptr5, myEncoderBuffer5->Size());
    ParseAndCheckDataFromEncoderL(KNullDesC, _L("TP43CollectionDataNameBuffer3"), _L("TP43CollectionDataDescriptionBuffer3"), *filecontent5);
    myEncoderBuffer5->Reset();
    CleanupStack::PopAndDestroy(filecontent5);
    CleanupStack::PopAndDestroy(myEncoderBuffer5);

    // 23c Repeat step 3-17 Use buffer when encoding, finalize with incremental mode and monitor progress 
    iLog->Log(_L("Part 23c "));
    CBufBase* myEncoderBuffer6 = iLandmarkEncoder->SetUseOutputBufferL();
    CleanupStack::PushL(myEncoderBuffer6);

    // Add collection data to the encoder, name and description set
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("TP43CollectionDataNameBuffer4"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("TP43CollectionDataDescriptionBuffer4"));
    CreateEncoderPartsL();

    // finalize with incremental mode and monitor progress 
    iOperation = iLandmarkEncoder->FinalizeEncodingL();

    activeLmOperation = new(ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(activeLmOperation);

    // Progress is monitored in CLandmarkTestProcedureBase class
    activeLmOperation->Start(iOperation);
    CActiveScheduler::Start();

    // Check for no errors
    iLog->Log(_L("CheckOperationL"));
    activeLmOperation->CheckOperationL();
    delete iOperation;
    iOperation = NULL;
    CleanupStack::PopAndDestroy(activeLmOperation);

    // Check data here
    HBufC8* filecontent6 = HBufC8::NewLC(myEncoderBuffer6->Size());
    TPtr8 ptr6 = filecontent6->Des();
    myEncoderBuffer6->Read(0, ptr6, myEncoderBuffer6->Size());
    ParseAndCheckDataFromEncoderL(KNullDesC, _L("TP43CollectionDataNameBuffer4"), _L("TP43CollectionDataDescriptionBuffer4"), *filecontent6);
    myEncoderBuffer6->Reset();
    CleanupStack::PopAndDestroy(filecontent6);
    CleanupStack::PopAndDestroy(myEncoderBuffer6);

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    
    DeleteFileL(KFileLongCollectionFieldName);
    // Test with file
    TestLongCollectionFieldsL(ETrue);
    // Test with buffer
    TestLongCollectionFieldsL(EFalse);
    
    // Check that all created XML files are correct by comparing to saved files
    iLog->Log(_L("Comparing files..."));

    TFileName filename;
    CreateCorrectXmlFileL( KFileNameCorrect, filename );
    CompareTwoFilesL(filename, KFileName);
    
    CreateCorrectXmlFileL( KFileName2Correct, filename );
    CompareTwoFilesL(filename, KFileName2);

    CreateCorrectXmlFileL( KFileName3Correct, filename );
    CompareTwoFilesL(filename, KFileName3);

    CreateCorrectXmlFileL( KFileName4Correct, filename );
    CompareTwoFilesL(filename, KFileName4);

    CreateCorrectXmlFileL( KFileEmptyCollectionDataCorrect, filename );
    CompareTwoFilesL(filename, KFileEmptyCollectionData);
    
    CreateCorrectXmlFileL( KFileEmptyCollectionData2Correct, filename );
    CompareTwoFilesL(filename, KFileEmptyCollectionData2);

    CreateCorrectXmlFileL( KFileLongCollectionFieldNameCorrect, filename );
    CompareTwoFilesL(filename, KFileLongCollectionFieldName);

    // Test for correct leave codes
    TestErrorScenariosL();

    // check for correct output
    CreateCorrectXmlFileL( KErrorScenario4FileCorrect, filename );
    CompareTwoFilesL(filename, KErrorScenario4File);

    CreateCorrectXmlFileL( KErrorScenario5FileCorrect, filename );
    CompareTwoFilesL(filename, KErrorScenario5File);
    
    // Test to encode to file with long filename (more than 255 characters)
    TestWithLongFileNameL();

    if (iErrorsFound != KErrNone) 
    {
    	iLog->Log(_L("Errors found in TP43"));
    	User::Leave(-1);
    }
	}

// ---------------------------------------------------------
// CPosTp43::CreateEncoderPartsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::CreateEncoderPartsL()
    {
    iLog->Log(_L("CreateEncoderPartsL"));
    // 3 Add one landmark to be encoded (id7 name "Billigt" in categories 1-3,14-17)
    CPosLandmark* lm1 = iDatabase->ReadLandmarkLC(7);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    // Remove field 408 which is not exported
    lm1->RemovePositionField(408);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);
    // 4 Add one more landmark to be encoded (id27 "LOG9" in categories 11-17)
    CPosLandmark* lm2 = iDatabase->ReadLandmarkLC(27);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 5 Add one category to the latest added landmark
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLandmarkCategory* landmarkCategory = categoryManager->ReadCategoryLC(1);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    CleanupStack::PopAndDestroy(categoryManager);

    // 6 Add one more landmark to be encoded (id52 "Kalles Hundgård" in category 19)
    lm2 = iDatabase->ReadLandmarkLC(52);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

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

    //globalCategoryId = categoryManager->GetGlobalCategoryL(6000);
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

    // 8 Add one landmark (not containing any categories), id=11 "MLFW"
    lm2 = iDatabase->ReadLandmarkLC(11);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 9 Add one more landmark (not containing any categories), id=56 "TE, Lund"
    lm2 = iDatabase->ReadLandmarkLC(56);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 10 Add one category to the latest added landmark
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    landmarkCategory = categoryManager->ReadCategoryLC(18);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    CleanupStack::PopAndDestroy(categoryManager);

    // 11 Add one landmark (not containing any categories), id=43 "asdf"
    lm2 = iDatabase->ReadLandmarkLC(43);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 12 Add several local and global categories to the latest added landmark
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    landmarkCategory = categoryManager->ReadCategoryLC(23);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    landmarkCategory = categoryManager->ReadCategoryLC(17);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    
    
    globalCategoryId = categoryManager->GetGlobalCategoryL(24000);
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

    // 13) Add a landmark with fields that are encoded to XML set
    lm1 = CreateXMLLandmarkLC(_L("TP43LandmarkWithAllXMLFields"));
    AddAllXMLFieldsL(lm1);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);

    // 13) Add one global category  (Garage) to the latest added landmark
    globalCategoryId = categoryManager->GetGlobalCategoryL(36000);
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

    // 14) Add one global category  (Bus stop) to the latest added landmark
    globalCategoryId = categoryManager->GetGlobalCategoryL(39000);
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

    CleanupStack::PopAndDestroy(categoryManager);
    iLog->Log(_L("CreateEncoderParts Done"));
    }

// ---------------------------------------------------------
// CPosTp43::PrintLandmarkIdFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::PrintLandmarkIdFieldsL(const CPosLandmark& aLandmark)
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
        //iLog->Log(buf);
        }

    while (sourceFieldId != EPositionFieldNone)
        {
        TPtrC sourceValue;
        aLandmark.GetPositionField(sourceFieldId, sourceValue);
        buf.Format(_L("IdField Id: %d Value:"), sourceFieldId);
        buf.Append(sourceValue);
        //iLog->Log(buf);
        sourceFieldId = aLandmark.NextPositionFieldId(sourceFieldId);
        }
    }

// ---------------------------------------------------------
// CPosTp43::PrintParsedDataFromEncoderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::PrintParsedDataFromEncoderL(const TDesC& aFile, const TDesC8&  aBuffer)
    {
    iLog->Log(_L("PrintParsedDataFromEncoderL"));

    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);

    if (aFile != KNullDesC)
        {
        iLog->Log(_L("----->Using FILE<-------"));
        iLandmarkParser->SetInputFileL(aFile);
        }
    else
        {
        iLog->Log(_L("----->Using BUFFER<-------"));
        iLandmarkParser->SetInputBuffer(aBuffer);
        }

    iOperation = iLandmarkParser->ParseContentL();
    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;
    TInt number = 0;
    
    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    while (status == KPosLmOperationNotComplete)
        {
        //iLog->Log(_L("------------------------------"));
        iOperation->NextStep(status, progress);
        
        // Wait for NextStep to complete
        User::WaitForRequest(status);
        // Get last parsed landmark
        CPosLandmark* lm = iLandmarkParser->LandmarkLC();
        TPtrC lmName;
        TPtrC catName;
        lm->GetLandmarkName(lmName);
        lm->GetCategoriesL(array);
        //iLog->Log(lmName);
        for (TInt i=0;i<array.Count();i++)
            {
            TBuf<150> buf;
            CPosLandmarkCategory* category = iLandmarkParser->LandmarkCategoryLC(array[i]);

            if (category->GlobalCategory()) buf.Append(_L("Global category: "));
            else buf.Append(_L("Local category: "));
            category->GetCategoryName(catName);
            buf.Append(catName);
            //iLog->Log(buf);
            CleanupStack::PopAndDestroy(category);
            }
        PrintLandmarkIdFieldsL(*lm);
        //iLog->Log(_L("---------->"));
        PrintLandmarkIdFieldsL(*iLandmarks[number]);
        //iLog->Log(_L("<----------"));
        number++;
        // CPosLmOperation::NextStep call will parse a new landmark.
        CleanupStack::PopAndDestroy(lm);
        }
    
    //iLog->Log(_L("------------------------------"));
    CleanupStack::PopAndDestroy(&array);
    
    TPosLmCollectionDataId lmId = iLandmarkParser->FirstCollectionDataId();

    //iLog->Log(_L("----->CollectionData"));

    TBuf<100> buf;
    if (lmId != EPosLmCollDataNone)
        {
        TPtrC first = iLandmarkParser->CollectionData(lmId);
        buf.Format(_L("Collectiond Id: %d CollectionData: "), lmId);
        buf.Append(first);
        //iLog->Log(buf);
        }
    //else iLog->Log(_L("No collection data found 1"));
    
    while (lmId != EPosLmCollDataNone)
        {
        lmId = iLandmarkParser->NextCollectionDataId(lmId);
        if (lmId != EPosLmCollDataNone)
        {
            TPtrC first = iLandmarkParser->CollectionData(lmId);
            buf.Format(_L("Collectiond Id: %d CollectionData: "), lmId);
            buf.Append(first);
            //iLog->Log(buf);
        }
        //else iLog->Log(_L("No collection data found 2"));
        }

    //iLog->Log(_L("<-----CollectionData"));

    delete iOperation;
    iOperation = NULL;
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    iLog->Log(_L("PrintParsedDataFromEncoderL Done"));
    }

// ---------------------------------------------------------
// CPosTp43::ParseAndCheckDataFromEncoderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::ParseAndCheckDataFromEncoderL(const TDesC& aFile , const TDesC& aCollectionName, 
            const TDesC& aCollectiondDescription, const TDesC8&  aBuffer)
    {
    iLog->Log(_L("ParseAndCheckDataFromEncoderL"));

    PrintParsedDataFromEncoderL(aFile, aBuffer);

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);

    if (aFile != KNullDesC)
        {
        //iLog->Log(_L("----->Using FILE<-------"));
        iLandmarkParser->SetInputFileL(aFile);
        }
    else
        {
        //iLog->Log(_L("----->Using BUFFER<-------"));
        iLandmarkParser->SetInputBuffer(aBuffer);
        }

    iLog->Log(_L("ParseContentL"));
    iOperation = iLandmarkParser->ParseContentL();
    iLog->Log(_L("ParseContentL Done"));

    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;
    TInt number = 0;
    
    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    // 1) Get the first landmark, "Billigt", no categories
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    CPosLandmark* lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 0, _L("Wrong number of landmark categories"));   
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);
    CleanupStack::PopAndDestroy(lm);

    // 2) Get LOG9, included category Pizzeria
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 1, _L("Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);
    CPosLandmarkCategory* category = iLandmarkParser->LandmarkCategoryLC(array[0]);
    CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(1);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);
    CleanupStack::PopAndDestroy(lm);

    // 3) Get Kalles Hundgård, included categories sjukhus, läkare, global Restaurant, Shop
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    if (array.Count()!=4)
        {
        iLog->Log(_L("ERROR: Wrong number of landmark categories"));
        iErrorsFound++;
        }
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);

    category = iLandmarkParser->LandmarkCategoryLC(array[0]);
    cat = categoryManager->ReadCategoryLC(26);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    category = iLandmarkParser->LandmarkCategoryLC(array[1]);
    cat = categoryManager->ReadCategoryLC(25);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    //Check global category "id=1 Restaurant" here
    category = iLandmarkParser->LandmarkCategoryLC(array[2]);
    cat = categoryManager->ReadCategoryLC(27);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    //Check global category "id=5 Shop" here
     if (array.Count()== 4)
        {
        category = iLandmarkParser->LandmarkCategoryLC(array[3]);
        cat = categoryManager->ReadCategoryLC(31);//28
        iErrorsFound += CompareXMLCategoriesL(*cat, *category);
        CleanupStack::PopAndDestroy(cat);
        CleanupStack::PopAndDestroy(category);
        }

    CleanupStack::PopAndDestroy(lm);

    // 4) Get MLFW, no categories
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 0, _L("Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);
    CleanupStack::PopAndDestroy(lm);

    // 5) Get Te, Lund category Frisör
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 1, _L("Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);

    category = iLandmarkParser->LandmarkCategoryLC(array[0]);
    cat = categoryManager->ReadCategoryLC(18);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);
    CleanupStack::PopAndDestroy(lm);

    // 6) Get asdf, category Dagis, Burger King and global hotel, car park
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 4, _L("Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);

    category = iLandmarkParser->LandmarkCategoryLC(array[0]);
    cat = categoryManager->ReadCategoryLC(23);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    category = iLandmarkParser->LandmarkCategoryLC(array[1]);
    cat = categoryManager->ReadCategoryLC(17);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    //Check global category "id=4 Hotel" here
    category = iLandmarkParser->LandmarkCategoryLC(array[2]);
    cat = categoryManager->ReadCategoryLC(34);//39
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);
    
    //Check global category "id=9 Car Park" here
    category = iLandmarkParser->LandmarkCategoryLC(array[3]);
    cat = categoryManager->ReadCategoryLC(35);//30
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);
    CleanupStack::PopAndDestroy(lm);

    // 7) Get TP43LandmarkWithAllXMLFields, category Burger King and global Garage, Bus stop
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 3, _L("Wrong number of landmark categories"));
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);

    //Check global category "id=4 Garage" here
    category = iLandmarkParser->LandmarkCategoryLC(array[0]);
     cat = categoryManager->ReadCategoryLC(38);//31
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
     CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    //Check global category "id=10 Bus Stop" here
    category = iLandmarkParser->LandmarkCategoryLC(array[1]);
    cat = categoryManager->ReadCategoryLC(39);//34
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);

    // Check local category here, burger king
    category = iLandmarkParser->LandmarkCategoryLC(array[2]);
    cat = categoryManager->ReadCategoryLC(17);
    iErrorsFound += CompareXMLCategoriesL(*cat, *category);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PopAndDestroy(category);
    
    CleanupStack::PopAndDestroy(lm);

    // Check that correct number of landmarks has been parsed
    if (number != KNrOfAddedLandmarks)
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);
    }
    TUint32 nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != KNrOfAddedLandmarks) 
    {
    	iLog->Log(_L("Wrong number of landmarks parsed"));
    	User::Leave(-1);	
    }

    CleanupStack::PopAndDestroy(&array);

    TPosLmCollectionDataId lmId = iLandmarkParser->FirstCollectionDataId();
    // Check that collection data and name is correct
    if (lmId == EPosLmCollDataCollectionName)
        {
        TPtrC first = iLandmarkParser->CollectionData(lmId);
        if (aCollectionName != first)
            {
            
            iLog->Log(_L("CollectionName does not match"));
    		User::Leave(-1);	
            }
        }
    else if (lmId == EPosLmCollDataNone)
        {
        if (aCollectionName != KNullDesC)
            {
            iLog->Log(_L("ERROR: CollectionName should be defined"));
            iErrorsFound++;
            }
        }
    else 
    {
    iLog->Log(_L("Wrong order of collection data1"));
    User::Leave(-1);	
    }

    lmId = iLandmarkParser->NextCollectionDataId(lmId);

    if (lmId == EPosLmCollDataCollectionDescription)
        {
        TPtrC first = iLandmarkParser->CollectionData(lmId);
        if (aCollectiondDescription != first)
            {
            
            iLog->Log(_L("CollectionDescription does not match"));
            User::Leave(-1);
            }
        }
    else if (lmId == EPosLmCollDataNone)
        {
        if (aCollectiondDescription != KNullDesC)
            {
            
            iLog->Log(_L(" CollectionDescription should be defined"));
            User::Leave(KErrNone);
            }
        }
    else 
    {
    	iLog->Log(_L("Wrong order of collection data2 "));
          User::Leave(-1);	
    }
    
    // Test with some non existing ids
    lmId = iLandmarkParser->NextCollectionDataId((TPosLmCollectionDataId)66700);
    if (lmId != EPosLmCollDataNone)
        {
        iLog->Log(_L("ERROR: No collection data should be found"));
        iErrorsFound++;
        }
        
    TPtrC empty = iLandmarkParser->CollectionData((TPosLmCollectionDataId)66700);
    if (empty != KNullDesC)
        {
        iLog->Log(_L("ERROR: CollectionData should be emtpy"));
        iErrorsFound++;
        }
    
    delete iOperation;
    iOperation = NULL;
    delete iLandmarkParser;
    iLandmarkParser = NULL;

    CleanupStack::PopAndDestroy(categoryManager);
    iLandmarks.ResetAndDestroy();

    iLog->Log(_L("ParseAndCheckDataFromEncoderL Done"));
    }

// ---------------------------------------------------------
// CPosTp43::CompareTwoFilesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::CompareTwoFilesL(const TDesC16&  aCorrectFile, const TDesC16&  aFile)
{
    iLog->Log(_L("ComparingTwoFilesL"));
    // Now compare tha trace file against the correct one
    RFile correctFile;
    RFile file;
    
    TBuf<150> buf;
    buf.Append(_L("Comparing files "));
    buf.Append(aCorrectFile);
    buf.Append(_L(" and "));
    buf.Append(aFile);
    iLog->Log(buf);

    CleanupClosePushL(correctFile);
    CleanupClosePushL(file);
    TInt err = KErrNone;
    err = correctFile.Open(iFileSession, aCorrectFile,  EFileRead);
    if (err != KErrNone)
    {
    	iLog->Log(_L(" Problem with correct file"));
        User::Leave(err);
    }
    err = file.Open(iFileSession, aFile,  EFileRead);
    if (err != KErrNone) 
    {
    	iLog->Log(_L(" Problem with encoded XML file"));
        User::Leave(err);
    }

    TBuf<KBufSize> correctLine;
    TBuf<KBufSize> line;
    TBool done = EFalse;
    TInt lineNr = 0;

    while (ReadLn(correctFile, correctLine) != KErrEof && !done)
        {
        lineNr++;
        err = ReadLn(file, line);
        if (err == KErrEof) done = ETrue;

        if (correctLine.CompareC(line) != KErrNone)
            {
            TBuf<50> buffe;
            buffe.Format(_L("ERROR in XML file on line %d: "), lineNr);
            iLog->Log(buffe);
            iLog->Log(correctLine);
            iLog->Log(line);
            iErrorsFound++;
            }
        }

    // EOF for trace file was reach before EOF in the correct file
    if (done) 
        {
        iLog->Log(_L("ERROR: EOF for Trace file was reached to early"));
        iErrorsFound++;
        }

    CleanupStack::PopAndDestroy(&file);
    CleanupStack::PopAndDestroy(&correctFile);
    }

// ---------------------------------------------------------
// CPosTp43::TestLongCollectionFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::TestLongCollectionFieldsL(TBool aUseFile)
    {
    iLog->Log(_L("TestLongCollectionFieldsL"));
    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    CBufBase* myEncoderBuffer = NULL;
    HBufC8* filecontent = NULL;

    if (aUseFile)
        {
        // 2) Use file for encoding, set output file
        iLandmarkEncoder->SetOutputFileL(KFileLongCollectionFieldName);
        }
    else
        {
        // 2) Use buffer for encoding
        myEncoderBuffer = iLandmarkEncoder->SetUseOutputBufferL();
        CleanupStack::PushL(myEncoderBuffer);
        }

    // 300 characters
    _LIT(KLongFieldName, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
    _LIT(KLongFieldDescription, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, KLongFieldName);
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, KLongFieldDescription);
    
    iLog->Log(_L("Create Data..."));
    // 3) Add one landmark to be encoded (id7 name "Billigt" in categories 1-3,14-17)
    CPosLandmark* lm1 = iDatabase->ReadLandmarkLC(7);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    // Remove field 408 which is not exported
    lm1->RemovePositionField(408);
    iLandmarks.Append(lm1);
    CleanupStack::Pop(lm1);
    // 4) Add one more landmark to be encoded (id27 "LOG9" in categories 11-17)
    CPosLandmark* lm2 = iDatabase->ReadLandmarkLC(27);
    iLandmarkEncoder->AddLandmarkL(*lm2);
    // Remove field 408 which is not exported
    lm2->RemovePositionField(408);
    iLandmarks.Append(lm2);
    CleanupStack::Pop(lm2);

    // 5) Add one category (pizzeria) to the latest added landmark
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLandmarkCategory* landmarkCategory = categoryManager->ReadCategoryLC(1);
        
    iLandmarkEncoder->AddCategoryForLatestLandmarkL(*landmarkCategory);
    CleanupStack::PopAndDestroy(landmarkCategory);
    CleanupStack::PopAndDestroy(categoryManager);

    // 6) Finalize encoding
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    // Check data
    iLandmarkParser = CPosLandmarkParser::NewL(KLmxMimeType);
    if (aUseFile)
        {
        iLog->Log(_L("----->Using FILE<-------"));
        iLandmarkParser->SetInputFileL(KFileLongCollectionFieldName);
        }
    else
        {
        iLog->Log(_L("----->Using BUFFER<-------"));
        filecontent = HBufC8::NewLC(myEncoderBuffer->Size());
        TPtr8 ptr = filecontent->Des();
        myEncoderBuffer->Read(0, ptr, myEncoderBuffer->Size());
        iLandmarkParser->SetInputBuffer(*filecontent);
        ptr.Zero();
        }

    iLog->Log(_L("ParseContentL"));
    iOperation = iLandmarkParser->ParseContentL();
    iLog->Log(_L("ParseContentL Done"));

    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;
    TInt number = 0;
    
    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    // 1) Get the first landmark, "Billigt", no categories
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    // Get last parsed landmark
    CPosLandmark* lm = iLandmarkParser->LandmarkLC();
    lm->GetCategoriesL(array);
    AssertTrueSecL(array.Count() == 0, _L("Wrong number of landmark categories"));   
    iErrorsFound += CompareXMLLandmarksL(*iLandmarks[number++], *lm);
    CleanupStack::PopAndDestroy(lm);

    iLog->Log(_L("----->CollectionData"));
    TPosLmCollectionDataId lmId = iLandmarkParser->FirstCollectionDataId();
    // First should be name
    TPtrC first = iLandmarkParser->CollectionData(lmId);
    if (first.Compare(KLongFieldName) != KErrNone)
        {
        iLog->Log(_L("ERROR: Wrong name!!"));
        iErrorsFound++;
        }

    lmId = iLandmarkParser->NextCollectionDataId(lmId);
    TPtrC next = iLandmarkParser->CollectionData(lmId);
    if (next.Compare(KLongFieldDescription) != KErrNone)
        {
        iLog->Log(_L("ERROR: Wrong description!!"));
        iErrorsFound++;
        }

    lmId = iLandmarkParser->NextCollectionDataId(lmId);
    if (lmId != EPosLmCollDataNone)
        {
        iLog->Log(_L("ERROR: Wrong description!!"));
        iErrorsFound++;
        }


    iLog->Log(_L("<-----CollectionData"));
    CleanupStack::PopAndDestroy(&array);
    if (!aUseFile)
        {
        CleanupStack::PopAndDestroy(filecontent);
        myEncoderBuffer->Reset();
        CleanupStack::PopAndDestroy(myEncoderBuffer);
        }

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iOperation;
    iOperation = NULL;

    iLandmarks.ResetAndDestroy();
    iLog->Log(_L("TestLongCollectionFieldsL Done"));
    }

// ---------------------------------------------------------
// CPosTp43::TestErrorScenariosL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::TestErrorScenariosL()
    {
    iLog->Log(_L("TestErrorScenariosL"));

    TInt err = KErrNone;
    TRAP(err, ErrorScenario1L());
    if (err != KErrAlreadyExists) //LogErrorAndLeave(_L("ErrorScenario1L did not leave with error KErrAlreadyExists"));
    {
    	iLog->Log(_L(" ErrorScenario1L did not leave with error KErrAlreadyExists"));
        User::Leave(err);
    }
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    DeleteFileL(KErrorTestFileName);

    ErrorScenario2L();
    DeleteFileL(KErrorTestFileName);

    TRAP(err, ErrorScenario3L());
    if (err != KErrAlreadyExists)// LogErrorAndLeave(_L("ErrorScenario3L did not leave with error KErrAlreadyExists"));
    {
    	iLog->Log(_L(" ErrorScenario3L did not leave with error KErrAlreadyExists"));
        User::Leave(err);
    }
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    DeleteFileL(KErrorTestFileName);

    TRAP(err, ErrorScenario4L());
    if (err != KErrNone) 
        {
        iLog->Log(_L("ERROR: Adding meta data \"out-of-range\" should be silently ignored, instead leave error"));
        iErrorsFound++;
        }
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    DeleteFileL(KErrorTestFileName);

    ErrorScenario5L();

    }

// ---------------------------------------------------------
// CPosTp43::ErrorScenario1L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::ErrorScenario1L()
    {
    iLog->Log(_L("ErrorScenario1L"));

    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    
    RFile file;
    CleanupClosePushL(file);

    // Make sure that file exist
    TInt err = file.Replace(iFileSession, KErrorTestFileName, EFileShareAny);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error when replacing file "));
        User::Leave(err);
    }

    CleanupStack::PopAndDestroy(&file);

    // 2) Use file for encoding, set output file should leave
    iLandmarkEncoder->SetOutputFileL(KErrorTestFileName);

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
}

// ---------------------------------------------------------
// CPosTp43::ErrorScenario2L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::ErrorScenario2L()
    {
    iLog->Log(_L("ErrorScenario2L"));

    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // 2) Use file for encoding, set output file
    DeleteFileL(KErrorTestFileName);
    iLandmarkEncoder->SetOutputFileL(KErrorTestFileName);
    
    RFile file;
    CleanupClosePushL(file);

    // Make sure that file exist
    TInt err = file.Open(iFileSession, KErrorTestFileName, EFileShareAny);
    if (err != KErrInUse) //LogErrorAndLeave(_L("File should not be able to open"));
    {
    	iLog->Log(_L("File should not be able to open"));
        User::Leave(err);
    }

    CleanupStack::PopAndDestroy(&file);

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
}

// ---------------------------------------------------------
// CPosTp43::ErrorScenario3L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::ErrorScenario3L()
    {
    iLog->Log(_L("ErrorScenario3L"));

    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // 2) Use file for encoding, set output file
    DeleteFileL(KErrorTestFileName);
    iLandmarkEncoder->SetOutputFileL(KErrorTestFileName);
    
    // AddCollectionDataL(id) with same id twice should leave
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("A yada yada collextion of landmarks"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("Another yada yada collextion of landmarks"));

    // We should never get here
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
}

// ---------------------------------------------------------
// CPosTp43::ErrorScenario4L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::ErrorScenario4L()
    {
    iLog->Log(_L("ErrorScenario4L"));

    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // 2) Use file for encoding, set output file
    DeleteFileL(KErrorScenario4File);
    iLandmarkEncoder->SetOutputFileL(KErrorScenario4File);

    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("A yada yada name for landmarks"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("A yada yada description for landmarks"));
    
    // Will be silently ignored
    iLandmarkEncoder->AddCollectionDataL((TPosLmCollectionDataId)5, _L("A dummy string"));

    TInt pos = EPosLmCollDataContentSpecificDataBegin;
    _LIT(KFormatString, "Dummmmmmmmmmmy string: %d");
    TBuf<100> buf;

    for (TInt i=0;i<10;i++)
        {
        buf.Format(KFormatString, pos);
        iLog->Log(buf);
        // Will be silently ignored
        iLandmarkEncoder->AddCollectionDataL((TPosLmCollectionDataId)pos, buf);
        pos++;
        }

    // Will be silently ignored
    iLog->Log(_L("Adding the last meta data collection allowed"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataLast, _L("The last dummy string"));
    iLog->Log(_L("After adding the last meta data collection allowed"));

    CPosLandmark* lm1 = iDatabase->ReadLandmarkLC(7);
    iLandmarkEncoder->AddLandmarkL(*lm1);
    CleanupStack::PopAndDestroy(lm1);

    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));
    
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
}

// ---------------------------------------------------------
// CPosTp43::ErrorScenario5L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::ErrorScenario5L()
    {
    iLog->Log(_L("ErrorScenario5L"));

    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    // 2) Use file for encoding, set output file
    DeleteFileL(KErrorScenario5File);
    iLandmarkEncoder->SetOutputFileL(KErrorScenario5File);

    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("A yada yada name for landmarks"));
    iLandmarkEncoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("</lm:lmx>"));
   
    CPosLandmark* lm1 = CreateXMLLandmarkLC(_L("</lm:lmx>"));
    iLandmarkEncoder->AddLandmarkL(*lm1);
    CleanupStack::PopAndDestroy(lm1);

    lm1 = CreateXMLLandmarkLC(_L("</lm:lmx>"));
    iLandmarkEncoder->AddLandmarkL(*lm1);
    CleanupStack::PopAndDestroy(lm1);
    
    iLog->Log(_L("FinalizeEncodingL"));
    ExecuteAndDeleteLD(iLandmarkEncoder->FinalizeEncodingL());
    iLog->Log(_L("FinalizeEncodingL Done"));

    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
}

// ---------------------------------------------------------
// CPosTp43::TestWithLongFileNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp43::TestWithLongFileNameL()
    {
    iLog->Log(_L("TestWithLongFileNameL"));
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    // 1) Create an encoder object
    iLandmarkEncoder = CPosLandmarkEncoder::NewL(KLmxMimeType);
    TInt errorsFound = KErrNone;
    
    // 2) Set file with long filename (more than 255 characters)
    TRAPD(err, iLandmarkEncoder->SetOutputFileL(KLongFileName));
  
    
    
   if ( err != KErrBadName ) 
        {
        TBuf<100> buf;    
        buf.Format(_L("ERROR: SetInputFileL should leave with KErrBadName, instead err: %d"), err);  
        iLog->Log(buf);
        iErrorsFound++;        
        }

    
    // 3) Set file to 0 characters
    TRAP(err, iLandmarkEncoder->SetOutputFileL(KEmptyFileName));
    if (err != KErrBadName) 
        {
        TBuf<100> buf;
        buf.Format(_L("ERROR: SetOutputFileL(KEmptyFileName) should leave with KErrBadName, instead err: %d"), err);
        iLog->Log(buf);
        errorsFound++;
        }
    
    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;
    
    // Try to create an encoder with the long file name, should leave with KErrNotSupported
    TRAP(err, CPosLandmarkEncoder::NewL(KLongMimeType));
    if (err != KErrNotSupported) 
    {
    	iLog->Log(_L("CPosLandmarkEncoder::NewL should leave with KErrNotSupported"));
        User::Leave(err);
    }
    
    if (errorsFound != KErrNone) 
    {
    iLog->Log(_L("SetOutputFileL leaved with wrong error code"));
     User::Leave(-1);	

	}
    
    
        
    }
    

//  End of File
