/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Entry point for test cases in stif module
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "testlandmarksapi.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestlandmarksapi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestLandmarksApi::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Ctestlandmarksapi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // For EPos_CPosLandmark.h
        ENTRY( "CreateNewLandmarkL", CTestLandmarksApi::CreateNewLandmarkL ),
        ENTRY( "CreateNewLandmarkLC", CTestLandmarksApi::CreateNewLandmarkLC ),
        ENTRY( "CreateLandmarkCopyL", CTestLandmarksApi::CreateLandmarkCopyL ),
        ENTRY( "CreateLandmarkCopyLC", CTestLandmarksApi::CreateLandmarkCopyLC ),
        ENTRY( "SetLandmarkName", CTestLandmarksApi::SetLandmarkNameL ),
        ENTRY( "GetLandmarkName", CTestLandmarksApi::GetLandmarkNameL ),
        ENTRY( "SetAndGetLandmarkDescription", CTestLandmarksApi::SetAndGetLmkDescL ),
        ENTRY( "SetAndGetCoverageRadius", CTestLandmarksApi::SetAndGetCovRadiusL ),
        ENTRY( "SetAndGetPosition", CTestLandmarksApi::SetAndGetPositionL ),
        ENTRY( "SetAndGetPositionField", CTestLandmarksApi::SetAndGetPositionFieldL ),
        ENTRY( "NumOfAvailablePosFields", CTestLandmarksApi::NumOfPosFieldsL ),
        ENTRY( "IsPosFieldAvailable", CTestLandmarksApi::IsPosFieldAvailableL ),
        ENTRY( "RemovePositionField", CTestLandmarksApi::RemovePositionFieldL ),        
        ENTRY( "RemoveAttributes", CTestLandmarksApi::RemoveAttributesL ),
        ENTRY( "PositionfieldId", CTestLandmarksApi::PositionFieldIdL ),
        ENTRY( "LandmarkId", CTestLandmarksApi::LandmarkIdL ),
		ENTRY( "IsPartial", CTestLandmarksApi::IsPartialL ),
		ENTRY( "SetAndGetIcon", CTestLandmarksApi::SetAndGetIconL ),
		ENTRY( "AddAndRemoveLmCategory", CTestLandmarksApi::AddAndRemoveLmCategoryL ),
		ENTRY( "GetCategories", CTestLandmarksApi::GetCategoriesL ),
		ENTRY( "SetAndGetPlaceId", CTestLandmarksApi::SetAndGetPlaceId ),
		ENTRY( "SetAndGetTimeStamp", CTestLandmarksApi::SetAndGetTimeStamp ),
		// For EPos_CPosLandmarkCategory.h
		ENTRY( "NewLCreateCategory", CTestLandmarksApi::NewLCreateCategoryL ),
		ENTRY( "NewLCCreateCategory", CTestLandmarksApi::NewLCCreateCategoryLC ),
		ENTRY( "NewLCreateCategoryCopy", CTestLandmarksApi::NewLCreateCategoryCopyL ),
		ENTRY( "NewLCCreateCategoryCopy", CTestLandmarksApi::NewLCCreateCategoryCopyLC ),
		ENTRY( "SetAndGetCategoryName", CTestLandmarksApi::SetAndGetCategoryNameL ), 
		ENTRY( "SetAndGetCategoryIcon", CTestLandmarksApi::SetAndGetCategoryIconL ), 
		ENTRY( "CategoryId", CTestLandmarksApi::CategoryIdL ),
		ENTRY( "CheckGlobal", CTestLandmarksApi::CheckGlobalL ),
		ENTRY( "RemoveCategoryAttributes", CTestLandmarksApi::RemoveCategoryAttributesL ),
		// For EPos_CPosLmCategoryManager.h
		ENTRY( "NewLCreateCategoryManager", CTestLandmarksApi::NewLCreateCategoryManagerL ),
		ENTRY( "AddAndRemoveCategory", CTestLandmarksApi::AddAndRemoveCategoryL ),
		ENTRY( "UpdateCategory", CTestLandmarksApi::UpdateCategoryL ),
		ENTRY( "AddCatToLandmarks", CTestLandmarksApi::AddCatToLandmarksL ),
		ENTRY( "RemoveCatFromLandmarks", CTestLandmarksApi::RemoveCatFromLandmarksL ),
		ENTRY( "CategoryIterator", CTestLandmarksApi::CategoryIteratorL ),
		ENTRY( "ReferencedIterator", CTestLandmarksApi::ReferencedIteratorL ),
		ENTRY( "ReadCategory", CTestLandmarksApi::ReadCategoryL ),
		ENTRY( "GetCategory", CTestLandmarksApi::GetCategoryL ),
		ENTRY( "RemoveCategories", CTestLandmarksApi::RemoveCategoriesL ),
		ENTRY( "GetGlobalCategory", CTestLandmarksApi::GetGlobalCategoryL ),
		ENTRY( "GlobalCategoryName", CTestLandmarksApi::GlobalCategoryNameL ),
		ENTRY( "ResetGlobalCategories", CTestLandmarksApi::ResetGlobalCategoriesL ),
		// For EPos_CPosLmItemIterator.h
		ENTRY( "NumOfItems", CTestLandmarksApi::NumOfItemsL ),
		ENTRY( "Next", CTestLandmarksApi::NextItemL ),
		ENTRY( "GetItemIds", CTestLandmarksApi::GetItemIdsL ),
		ENTRY( "ResetIterator", CTestLandmarksApi::ResetIteratorL ),
		// Database
		ENTRY( "OpenDefaultDatabase", CTestLandmarksApi::OpenDefaultDatabaseL ),
		ENTRY( "OpenDatabase", CTestLandmarksApi::OpenDatabaseL ),
		ENTRY( "IsInitializingNeeded", CTestLandmarksApi::IsInitializingNeededL ),
		ENTRY( "Initialize", CTestLandmarksApi::InitializeL ),
		ENTRY( "GetDatabaseUri", CTestLandmarksApi::GetDatabaseUriL ),
		ENTRY( "GetDatabaseSize", CTestLandmarksApi::GetDatabaseSizeL ),
		ENTRY( "LandmarkIterator", CTestLandmarksApi::LandmarkIteratorL ),
		ENTRY( "LmkIteratorSortPref", CTestLandmarksApi::LmkIteratorSortPrefL ),
		ENTRY( "ReadLandmark", CTestLandmarksApi::ReadLandmarkL ),
		ENTRY( "AddAndRemoveLandmark", CTestLandmarksApi::AddAndRemoveLandmarkL ),
		ENTRY( "UpdateLandmark", CTestLandmarksApi::UpdateLandmarkL ),
		ENTRY( "RemoveLandmarks", CTestLandmarksApi::RemoveLandmarksL ),
		ENTRY( "RemoveAllLandmarks", CTestLandmarksApi::RemoveAllLandmarksL ),
		ENTRY( "Compact", CTestLandmarksApi::CompactL ),
		ENTRY( "SetAndGetPartialReadParams", CTestLandmarksApi::SetAndGetPartialReadParamsL ),
		ENTRY( "ReadPartialLandmark", CTestLandmarksApi::ReadPartialLandmarkL ),
		ENTRY( "PrepareAndTakePartialLmks", CTestLandmarksApi::PrepareAndTakePartialLmksL ),
		ENTRY( "ExportLandmarks", CTestLandmarksApi::ExportLandmarksL ),
		ENTRY( "ImportLandmarks", CTestLandmarksApi::ImportLandmarksL ),
		ENTRY( "ImportSelectedLandmarks", CTestLandmarksApi::ImportSelectedLandmarksL ),
		ENTRY( "ImportedLmkIterator", CTestLandmarksApi::ImportedLmkIteratorL ),
		ENTRY( "NotifyEvent", CTestLandmarksApi::NotifyDatabaseEventL ),
		ENTRY( "CancelNotifyEvent", CTestLandmarksApi::CancelNotifyDatabaseEventL ),
		// Extended Database 
		ENTRY( "OpenExtendedDatabase", CTestLandmarksApi::OpenExtendedDatabaseL ),
		ENTRY( "ExtendedDbLandmarkCount", CTestLandmarksApi::ExtendedDbLandmarkCountL ),
		ENTRY( "ExtendedDbCategoryCount", CTestLandmarksApi::ExtendedDbCategoryCountL ),
		// Operation
		ENTRY( "DestructOperation", CTestLandmarksApi::DestructOperationL ),
		ENTRY( "ExecuteOperation", CTestLandmarksApi::ExecuteL ),
		ENTRY( "ExecuteAndDeleteOperation", CTestLandmarksApi::ExecuteAndDeleteOperationL ),
		ENTRY( "NextStep", CTestLandmarksApi::NextStepL ),
		// Encoder
		ENTRY( "CreateEncoder", CTestLandmarksApi::CreateEncoderL ),
		ENTRY( "SetOutputFile", CTestLandmarksApi::SetOutputFileL ),
		ENTRY( "SetOutputBuffer", CTestLandmarksApi::SetOutputBufferL ),
		ENTRY( "AddCollectionData", CTestLandmarksApi::AddCollectionDataL ),
		ENTRY( "AddLandmarkToEncode", CTestLandmarksApi::AddLandmarkToEncodeL ),
		ENTRY( "AddCategoryForLatestLandmark", CTestLandmarksApi::AddCategoryForLatestLandmarkL ),
		ENTRY( "FinalizeEncoding", CTestLandmarksApi::FinalizeEncodingL ),
		// Parser
		ENTRY( "CreateParser", CTestLandmarksApi::CreateParserL ),
		ENTRY( "SetInputBuffer", CTestLandmarksApi::SetInputBufferL ),
		ENTRY( "SetInputFile", CTestLandmarksApi::SetInputFileL ),
		ENTRY( "SetInputFileHandle", CTestLandmarksApi::SetInputFileHandleL ),
		ENTRY( "ParseContent", CTestLandmarksApi::ParseContentL ),
		ENTRY( "NumOfParsedContent", CTestLandmarksApi::NumOfParsedContentL ),
		ENTRY( "FirstAndNextCollDataId", CTestLandmarksApi::FirstAndNextCollDataIdL ),
		ENTRY( "GetCollectionData", CTestLandmarksApi::GetCollectionDataL ),
		ENTRY( "RetrieveParsedLmk", CTestLandmarksApi::RetrieveParsedLmkL ),
		ENTRY( "RetrieveCategory", CTestLandmarksApi::RetrieveCategoryL ),
		// PartialReadParams
		ENTRY( "CreatePartialReadParams", CTestLandmarksApi::CreatePartialReadParamsL ),
		ENTRY( "SetAndGetRequestedAttr", CTestLandmarksApi::SetAndGetRequestedAttrL ),
		ENTRY( "SetAndGetRequestedPosField", CTestLandmarksApi::SetAndGetRequestedPosFieldL ),
		// Sort Preference
		ENTRY( "CreateSortPref", CTestLandmarksApi::CreateSortPref ),
		ENTRY( "GetSortOrder", CTestLandmarksApi::GetSortOrder ),
		ENTRY( "SetAndGetSortAttr", CTestLandmarksApi::SetAndGetSortAttr ),
		// Landmark Serialization
		ENTRY( "PackLandmark", CTestLandmarksApi::PackLandmarkL ),
		ENTRY( "UnpackLandmark", CTestLandmarksApi::UnpackLandmarkL ),
		// Category Serialization
		ENTRY( "PackCategory", CTestLandmarksApi::PackCategoryL ),
		ENTRY( "UnpackCategory", CTestLandmarksApi::UnpackCategoryL ),
        //ADD NEW ENTRY HERE

        }; 

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );
	
	TInt ret = KErrNone;
	TRAPD( error, ret = RunInternalL( KFunctions, count, aItem ));
	if ( error != KErrNone )
		{
		return error;
		}
	return ret;	


    }


// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreateNewLandmarkL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreateNewLandmarkL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->CreateNewLandmarkL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}


// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreateNewLandmarkLC
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreateNewLandmarkLC( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->CreateNewLandmarkLC( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreateLandmarkCopyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreateLandmarkCopyL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->CreateLandmarkCopyL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreateLandmarkCopyLC
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreateLandmarkCopyLC( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->CreateLandmarkCopyLC( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetLandmarkNameL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetLandmarkNameL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->SetLandmarkNameL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetLmkDesc
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetLandmarkNameL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->GetLandmarkNameL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetLmkDesc
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetLmkDescL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->SetAndGetLmkDescL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetCovRadiusL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetCovRadiusL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->SetAndGetCovRadiusL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetPosition
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetPositionL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->SetAndGetPositionL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetPositionField
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetPositionFieldL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->SetAndGetPositionFieldL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::NumOfPosFields
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NumOfPosFieldsL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->NumOfPosFieldsL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::IsPosFieldAvailable
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::IsPosFieldAvailableL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->IsPosFieldAvailableL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::RemovePositionField
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RemovePositionFieldL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->RemovePositionFieldL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::RemoveAttributes
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RemoveAttributesL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->RemoveAttributesL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::PositionFieldId
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::PositionFieldIdL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->PositionFieldIdL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::LandmarkId
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::LandmarkIdL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->LandmarkIdL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::IsPartial
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::IsPartialL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->IsPartialL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetIcon
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetIconL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->SetAndGetIconL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::AddAndRemoveLmCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::AddAndRemoveLmCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL(testLandmark);
	TInt err = testLandmark->AddAndRemoveLmCategoryL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetCategories
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetCategoriesL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
	CleanupStack::PushL( testLandmark );
	TInt err = testLandmark->GetCategoriesL( aItem );
	CleanupStack::PopAndDestroy( testLandmark );
	return err;	
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetPlaceId
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetPlaceId( CStifItemParser& aItem )
    {
    // Call method of tester object
     CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
     CleanupStack::PushL( testLandmark );
     TInt err = testLandmark->SetAndGetPlaceId( aItem );
     CleanupStack::PopAndDestroy( testLandmark );
     return err; 
    }
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetTimeStamp
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetTimeStamp( CStifItemParser& aItem )
    {
    // Call method of tester object
     CTestPosLandmark* testLandmark = CTestPosLandmark::NewL( iLog );
     CleanupStack::PushL( testLandmark );
     TInt err = testLandmark->SetAndGetTimeStamp( aItem );
     CleanupStack::PopAndDestroy( testLandmark );
     return err; 
    }
// -----------------------------------------------------------------------------
// CTestLandmarksApi::NewLCreateCategoryL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NewLCreateCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	// 
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->NewLCreateCategoryL( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;				
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::NewLCCreateCategoryLC
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NewLCCreateCategoryLC( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->NewLCCreateCategoryLC( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;				
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::NewLCreateCategoryCopyL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NewLCreateCategoryCopyL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->NewLCreateCategoryCopyL( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;				
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::NewLCCreateCategoryCopy
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NewLCCreateCategoryCopyLC( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->NewLCCreateCategoryCopyLC( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;			
	}
   
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetCategoryName
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetCategoryNameL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->SetAndGetCategoryNameL( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetCategoryIcon
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetCategoryIconL( CStifItemParser& aItem )
	{
	//TInt num;
	//TInt err = aItem.GetNextInt( num );
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	//if( num == 1 )
		//{
		//TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 14);
		//}
	TInt err = testCategory->SetAndGetCategoryIconL( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;
	}	

// -----------------------------------------------------------------------------
// CTestLandmarksApi::CategoryId
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CategoryIdL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->CategoryIdL( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;		
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::CheckGlobalL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CheckGlobalL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->CheckGlobalL( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;				
			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::RemoveCategoryAttributes
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RemoveCategoryAttributesL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkCategory* testCategory = CTestPosLandmarkCategory::NewL( iLog );
	CleanupStack::PushL( testCategory );
	TInt err = testCategory->RemoveCategoryAttributesL( aItem );
	CleanupStack::PopAndDestroy( testCategory );
	return err;			
	}	

// -----------------------------------------------------------------------------
// CTestLandmarksApi::NewLCreateCategoryManager
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NewLCreateCategoryManagerL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->NewLCreateCategoryManagerL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::AddAndRemoveCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::AddAndRemoveCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->AddAndRemoveCategoryL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::UpdateCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::UpdateCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->UpdateCategoryL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}	
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::AddCatToLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::AddCatToLandmarksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->AddCatToLandmarksL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::RemoveCatFromLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RemoveCatFromLandmarksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->RemoveCatFromLandmarksL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}		
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CategoryIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CategoryIteratorL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->CategoryIteratorL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::ReferencedIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ReferencedIteratorL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->ReferencedIteratorL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::ReadCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ReadCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->ReadCategoryL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->GetCategoryL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}	

// -----------------------------------------------------------------------------
// CTestLandmarksApi::RemoveCategories
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RemoveCategoriesL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->RemoveCategoriesL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetGlobalCategoryL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetGlobalCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->GetGlobalCategoryL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}	
		
// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetGlobalCategoryL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GlobalCategoryNameL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->GlobalCategoryNameL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetGlobalCategoryL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ResetGlobalCategoriesL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmCategoryManager* testCategoryMgr = CTestPosLmCategoryManager::NewL( iLog );
	CleanupStack::PushL( testCategoryMgr );
	TInt err = testCategoryMgr->ResetGlobalCategoriesL( aItem ); 
	CleanupStack::PopAndDestroy( testCategoryMgr );
	return err;
	}			
		
// -----------------------------------------------------------------------------
// CTestLandmarksApi::NumOfItems
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NumOfItemsL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	// 
	CTestPosLmItemIterator* testIter = CTestPosLmItemIterator::NewL( iLog );
	CleanupStack::PushL( testIter );
	TInt err = testIter->NumOfItemsL( aItem );
	CleanupStack::PopAndDestroy( testIter );
	return err;			
	}	

// -----------------------------------------------------------------------------
// CTestLandmarksApi::NextItem
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NextItemL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmItemIterator* testIter = CTestPosLmItemIterator::NewL( iLog );
	CleanupStack::PushL( testIter );
	TInt err = testIter->NextItemL( aItem );
	CleanupStack::PopAndDestroy( testIter );
	return err;			
	}	

// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetItemIdsL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetItemIdsL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmItemIterator* testIter = CTestPosLmItemIterator::NewL( iLog );
	CleanupStack::PushL( testIter );
	TInt err = testIter->GetItemIdsL( aItem );
	CleanupStack::PopAndDestroy( testIter );
	return err;			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::NextItem
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ResetIteratorL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmItemIterator* testIter = CTestPosLmItemIterator::NewL( iLog );
	CleanupStack::PushL( testIter );
	TInt err = testIter->ResetIteratorL( aItem );
	CleanupStack::PopAndDestroy( testIter );
	return err;			
	}		

// -----------------------------------------------------------------------------
// CTestLandmarksApi::OpenDefaultDatabase
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::OpenDefaultDatabaseL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	// 
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->OpenDefaultDatabaseL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::OpenDatabase
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::OpenDatabaseL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->OpenDatabaseL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}		
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::IsInitializingNeeded
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::IsInitializingNeededL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->IsInitializingNeededL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}			
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::Initialize
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::InitializeL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->InitializeL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}			

// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetDatabaseUri
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetDatabaseUriL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->GetDatabaseUriL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}			
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetDatabaseSize
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetDatabaseSizeL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->GetDatabaseSizeL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}				

	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::LandmarkIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::LandmarkIteratorL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->LandmarkIteratorL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}		
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::LmkIteratorSortPref
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::LmkIteratorSortPrefL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->LmkIteratorSortPrefL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}			

// -----------------------------------------------------------------------------
// CTestLandmarksApi::ReadLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ReadLandmarkL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ReadLandmarkL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}		

// -----------------------------------------------------------------------------
// CTestLandmarksApi::AddAndRemoveLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::AddAndRemoveLandmarkL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->AddAndRemoveLandmarkL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::UpdateLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::UpdateLandmarkL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->UpdateLandmarkL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}			

// -----------------------------------------------------------------------------
// CTestLandmarksApi::RemoveLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RemoveLandmarksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->RemoveLandmarksL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}		

// -----------------------------------------------------------------------------
// CTestLandmarksApi::RemoveAllLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RemoveAllLandmarksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->RemoveAllLandmarksL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}		
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::Compact
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CompactL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->CompactL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}		

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetPartialReadParams
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetPartialReadParamsL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->SetAndGetPartialReadParamsL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}	
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ReadPartialLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ReadPartialLandmarkL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ReadPartialLandmarkL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::PrepareAndTakePartialLmks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::PrepareAndTakePartialLmksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->PrepareAndTakePartialLmksL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}				

// -----------------------------------------------------------------------------
// CTestLandmarksApi::ExportLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ExportLandmarksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ExportLandmarksL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}				
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ImportLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ImportLandmarksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ImportLandmarksL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}				
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ImportSelectedLandmarks
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ImportSelectedLandmarksL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ImportSelectedLandmarksL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}				
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ImportedLmkIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ImportedLmkIteratorL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ImportedLmkIteratorL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}

// -----------------------------------------------------------------------------
// CTestLandmarksApi::ImportedLmkIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NotifyDatabaseEventL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->NotifyDatabaseEventL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ImportedLmkIterator
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CancelNotifyDatabaseEventL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->CancelNotifyDatabaseEventL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;			
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::OpenExtendedDatabase
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::OpenExtendedDatabaseL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->OpenExtendedDatabaseL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ExtendedDbLandmarkCountL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ExtendedDbLandmarkCountL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ExtendedDbLandmarkCountL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;	
	}		
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ExtendedDbCategoryCount
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ExtendedDbCategoryCountL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkDatabase* testDatabase = CTestPosLandmarkDatabase::NewL( iLog );
	CleanupStack::PushL( testDatabase );
	TInt err = testDatabase->ExtendedDbCategoryCountL( aItem );
	CleanupStack::PopAndDestroy( testDatabase );
	return err;	
	}	
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::DestructOperationL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::DestructOperationL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	// CTestPosLmOperation
	CTestPosLmOperation* testOperation = CTestPosLmOperation::NewL( iLog );
	CleanupStack::PushL( testOperation );
	TInt err = testOperation->DestructOperationL( aItem );
	CleanupStack::PopAndDestroy( testOperation );
	return err;	
	}		

// -----------------------------------------------------------------------------
// CTestLandmarksApi::Execute
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ExecuteL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmOperation* testOperation = CTestPosLmOperation::NewL( iLog );
	CleanupStack::PushL( testOperation );
	TInt err = testOperation->ExecuteL( aItem );
	CleanupStack::PopAndDestroy( testOperation );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ExecuteAndDeleteOperation
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ExecuteAndDeleteOperationL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmOperation* testOperation = CTestPosLmOperation::NewL( iLog );
	CleanupStack::PushL( testOperation );
	TInt err = testOperation->ExecuteAndDeleteOperationL( aItem );
	CleanupStack::PopAndDestroy( testOperation );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::NextStep
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NextStepL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmOperation* testOperation = CTestPosLmOperation::NewL( iLog );
	CleanupStack::PushL( testOperation );
	TInt err = testOperation->NextStepL( aItem );
	CleanupStack::PopAndDestroy( testOperation );
	return err;	
	}		
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreateEncoder
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreateEncoderL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	// CTestPosLandmarkEncoder
	CTestPosLandmarkEncoder* testEncoder = CTestPosLandmarkEncoder::NewL( iLog );
	CleanupStack::PushL( testEncoder );
	TInt err = testEncoder->CreateEncoderL( aItem );
	CleanupStack::PopAndDestroy( testEncoder );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetOutputFile
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetOutputFileL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkEncoder* testEncoder = CTestPosLandmarkEncoder::NewL( iLog );
	CleanupStack::PushL( testEncoder );
	TInt err = testEncoder->SetOutputFileL( aItem );
	CleanupStack::PopAndDestroy( testEncoder );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetOutputBuffer
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetOutputBufferL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkEncoder* testEncoder = CTestPosLandmarkEncoder::NewL( iLog );
	CleanupStack::PushL( testEncoder );
	TInt err = testEncoder->SetOutputBufferL( aItem );
	CleanupStack::PopAndDestroy( testEncoder );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::AddCollectionData
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::AddCollectionDataL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkEncoder* testEncoder = CTestPosLandmarkEncoder::NewL( iLog );
	CleanupStack::PushL( testEncoder );
	TInt err = testEncoder->AddCollectionDataL( aItem );
	CleanupStack::PopAndDestroy( testEncoder );
	return err;	
	}						

// -----------------------------------------------------------------------------
// CTestLandmarksApi::AddLandmarkToEncode
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::AddLandmarkToEncodeL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkEncoder* testEncoder = CTestPosLandmarkEncoder::NewL( iLog );
	CleanupStack::PushL( testEncoder );
	TInt err = testEncoder->AddLandmarkToEncodeL( aItem );
	CleanupStack::PopAndDestroy( testEncoder );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::AddCategoryForLatestLandmark
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::AddCategoryForLatestLandmarkL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkEncoder* testEncoder = CTestPosLandmarkEncoder::NewL( iLog );
	CleanupStack::PushL( testEncoder );
	TInt err = testEncoder->AddCategoryForLatestLandmarkL( aItem );
	CleanupStack::PopAndDestroy( testEncoder );
	return err;	
	}							

// -----------------------------------------------------------------------------
// CTestLandmarksApi::FinalizeEncoding
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::FinalizeEncodingL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkEncoder* testEncoder = CTestPosLandmarkEncoder::NewL( iLog );
	CleanupStack::PushL( testEncoder );
	TInt err = testEncoder->FinalizeEncodingL( aItem );
	CleanupStack::PopAndDestroy( testEncoder );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreateParser
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreateParserL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	// 
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->CreateParserL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetInputBuffer
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetInputBufferL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->SetInputBufferL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetInputFile
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetInputFileL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->SetInputFileL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetInputFileHandle
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetInputFileHandleL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->SetInputFileHandleL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::ParseContent
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::ParseContentL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->ParseContentL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::NumOfParsedContent
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::NumOfParsedContentL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->NumOfParsedContentL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::FirstAndNextCollDataId
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::FirstAndNextCollDataIdL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->FirstAndNextCollDataIdL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::GetCollectionData
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetCollectionDataL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->GetCollectionDataL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}														

	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::RetrieveParsedLmk
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RetrieveParsedLmkL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->RetrieveParsedLmkL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}	


// -----------------------------------------------------------------------------
// CTestLandmarksApi::RetrieveCategory
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::RetrieveCategoryL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLandmarkParser* testParser = CTestPosLandmarkParser::NewL( iLog );
	CleanupStack::PushL( testParser );
	TInt err = testParser->RetrieveCategoryL( aItem );
	CleanupStack::PopAndDestroy( testParser );
	return err;	
	}	
	
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatePartialReadParams
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreatePartialReadParamsL( CStifItemParser& aItem )
	{
	
	// Call method of tester object
	//
	CTestPosLmPartialReadParams* testPartReadParams = CTestPosLmPartialReadParams::NewL( iLog );
	CleanupStack::PushL( testPartReadParams );
	TInt err = testPartReadParams->CreatePartialReadParamsL( aItem );
	CleanupStack::PopAndDestroy( testPartReadParams );
    return err; 
    }
// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetRequestedAttr
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetRequestedAttrL( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    //
    CTestPosLmPartialReadParams* testPartReadParams = CTestPosLmPartialReadParams::NewL( iLog );
    CleanupStack::PushL( testPartReadParams );
    TInt err = testPartReadParams->SetAndGetRequestedAttrL( aItem );
    CleanupStack::PopAndDestroy( testPartReadParams );
    return err;
    }                                                       

// -----------------------------------------------------------------------------
// CTestLandmarksApi::SetAndGetRequestedPosField
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetRequestedPosFieldL( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    //
    CTestPosLmPartialReadParams* testPartReadParams = CTestPosLmPartialReadParams::NewL( iLog );
    CleanupStack::PushL( testPartReadParams );
    TInt err = testPartReadParams->SetAndGetRequestedPosFieldL( aItem );
    CleanupStack::PopAndDestroy( testPartReadParams );
    return err;
    }                                                       
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatenewlcL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::CreateSortPref( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    //
    TTestPosLmSortPref testSortPref( iLog );
    return testSortPref.CreateSortPref( aItem );
            
    }                                                       
    
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatenewlcL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::GetSortOrder( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    //
    TTestPosLmSortPref testSortPref( iLog );
    return testSortPref.GetSortOrder( aItem );
            
    }                                                       
    
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatenewlcL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::SetAndGetSortAttr( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    //
    TTestPosLmSortPref testSortPref( iLog );
    return testSortPref.SetAndGetSortAttr( aItem );
            
    }                                                       
    
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatenewlcL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::PackLandmarkL( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    // 
    TestPosLmkSerialization testLmkSerialization( iLog );
    return testLmkSerialization.PackLandmarkL( aItem );
            
    }
    
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatenewlcL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::UnpackLandmarkL( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    //
    TestPosLmkSerialization testLmkSerialization( iLog );
    return testLmkSerialization.UnpackLandmarkL( aItem );
            
    }
    
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatenewlcL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::PackCategoryL( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    // 
    TestPosLmkCatSerialization testCatSerialization( iLog );
    return testCatSerialization.PackCategoryL( aItem );
            
    }
    
// -----------------------------------------------------------------------------
// CTestLandmarksApi::CreatenewlcL
// 
// 
// -----------------------------------------------------------------------------
//
TInt CTestLandmarksApi::UnpackCategoryL( CStifItemParser& aItem )
    {
    
    // Call method of tester object
    //
    TestPosLmkCatSerialization testCatSerialization( iLog );
    return testCatSerialization.UnpackCategoryL( aItem );
            
    }                                                               
    
//  End of File
