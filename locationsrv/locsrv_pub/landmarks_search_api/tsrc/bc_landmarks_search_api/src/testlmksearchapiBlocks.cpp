/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:    Entry point of the test cases in the stif test module
*
*/






// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <EPos_Landmarks.h>
#include <EPos_CPosLmDisplayItem.h>
#include "testlmksearchapi.h"
#include "testposlmksearch.h"
#include "testposlmareacriteria.h"
#include "testposlmcategorycriteria.h"
#include "testposlmcatnamecriteria.h"
#include "testposlmcompositecriteria.h"
#include "testposlmdisplaydata.h"
#include "testposlmnearestcriteria.h"
#include "testposlmtextcriteria.h"
#include "testposlmidlistcriteria.h"
#include "testposlmdisplayitem.h"
#include "testposlmmultidbsearch.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Ctestlmksearchapi::Delete() 
    {
		ReleaseLandmarkResources();
    }

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        
        ENTRY( "CPosLandmarkSearch_NewL", Ctestlmksearchapi::CPosLandmarkSearch_NewL),
        ENTRY( "CPosLandmarkSearch_MaxNumOfMatches", Ctestlmksearchapi::CPosLandmarkSearch_MaxNumOfMatchesL),
        ENTRY( "CPosLandmarkSearch_SetMaxNumOfMatches", Ctestlmksearchapi::CPosLandmarkSearch_SetMaxNumOfMatchesL),
		ENTRY( "CPosLandmarkSearch_StartLandmarkSearchL", Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL),
		ENTRY( "CPosLandmarkSearch_StartLandmarkSearchL1", Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL1L),
		ENTRY( "CPosLandmarkSearch_StartLandmarkSearchL2", Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL2L),
		ENTRY( "CPosLandmarkSearch_NumOfMatches", Ctestlmksearchapi::CPosLandmarkSearch_NumOfMatchesL),
		ENTRY( "CPosLandmarkSearch_MatchIteratorL", Ctestlmksearchapi::CPosLandmarkSearch_MatchIteratorL),
		ENTRY( "CPosLandmarkSearch_SetDisplayData", Ctestlmksearchapi::CPosLandmarkSearch_SetDisplayDataL),
		ENTRY( "CPosLandmarkSearch_UnsetDisplayData", Ctestlmksearchapi::CPosLandmarkSearch_UnsetDisplayDataL),
		ENTRY( "CPosLmAreaCriteria_NewLC", Ctestlmksearchapi::CPosLmAreaCriteria_NewLC),
        ENTRY( "CPosLmAreaCriteria_GetSearchArea", Ctestlmksearchapi::CPosLmAreaCriteria_GetSearchAreaL),
		ENTRY( "CPosLmAreaCriteria_SetSearchArea", Ctestlmksearchapi::CPosLmAreaCriteria_SetSearchAreaL),
		ENTRY( "CPosLmCategoryCriteria_NewLC", Ctestlmksearchapi::CPosLmCategoryCriteria_NewLC),
		ENTRY( "CPosLmCategoryCriteria_CategoryItemId", Ctestlmksearchapi::CPosLmCategoryCriteria_CategoryItemIdL),
		ENTRY( "CPosLmCategoryCriteria_GlobalCategory", Ctestlmksearchapi::CPosLmCategoryCriteria_GlobalCategoryL),
		ENTRY( "CPosLmCategoryCriteria_CategoryName", Ctestlmksearchapi::CPosLmCategoryCriteria_CategoryNameL),
		ENTRY( "CPosLmCategoryCriteria_SetCategoryItemId", Ctestlmksearchapi::CPosLmCategoryCriteria_SetCategoryItemIdL),
		ENTRY( "CPosLmCategoryCriteria_SetGlobalCategory", Ctestlmksearchapi::CPosLmCategoryCriteria_SetGlobalCategoryL),
        ENTRY( "CPosLmCategoryCriteria_SetCategoryNameL", Ctestlmksearchapi::CPosLmCategoryCriteria_SetCategoryNameL),
        ENTRY( "CPosLmCatNameCriteria_NewLC", Ctestlmksearchapi::CPosLmCatNameCriteria_NewLC),
		ENTRY( "CPosLmCatNameCriteria_SearchPattern", Ctestlmksearchapi::CPosLmCatNameCriteria_SearchPatternL),
        ENTRY( "CPosLmCatNameCriteria_SetSearchPatternL", Ctestlmksearchapi::CPosLmCatNameCriteria_SetSearchPatternL),
        ENTRY( "CPosLmCompositeCriteria_NewLC_AND", Ctestlmksearchapi::CPosLmCompositeCriteria_NewLC_AND),
        ENTRY( "CPosLmCompositeCriteria_NewLC_OR", Ctestlmksearchapi::CPosLmCompositeCriteria_NewLC_OR),
		ENTRY( "CPosLmCompositeCriteria_CompositionType", Ctestlmksearchapi::CPosLmCompositeCriteria_CompositionTypeL),
		ENTRY( "CPosLmCompositeCriteria_SetCompositionType", Ctestlmksearchapi::CPosLmCompositeCriteria_SetCompositionTypeL),
		ENTRY( "CPosLmCompositeCriteria_NumOfArguments", Ctestlmksearchapi::CPosLmCompositeCriteria_NumOfArgumentsL),
		ENTRY( "CPosLmCompositeCriteria_Argument1", Ctestlmksearchapi::CPosLmCompositeCriteria_Argument1L),
		ENTRY( "CPosLmCompositeCriteria_Argument2", Ctestlmksearchapi::CPosLmCompositeCriteria_Argument2L),
		ENTRY( "CPosLmCompositeCriteria_AddArgument", Ctestlmksearchapi::CPosLmCompositeCriteria_AddArgumentL),
        ENTRY( "CPosLmCompositeCriteria_RemoveArgument", Ctestlmksearchapi::CPosLmCompositeCriteria_RemoveArgumentL),
		ENTRY( "CPosLmCompositeCriteria_ClearArguments", Ctestlmksearchapi::CPosLmCompositeCriteria_ClearArgumentsL),
		ENTRY( "CPosLmDisplayData_NewL", Ctestlmksearchapi::CPosLmDisplayData_NewL),
		ENTRY( "CPosLmDisplayData_NewItemIndex", Ctestlmksearchapi::CPosLmDisplayData_NewItemIndexL),
		ENTRY( "CPosLmDisplayData_Count", Ctestlmksearchapi::CPosLmDisplayData_CountL),
		ENTRY( "CPosLmDisplayData_Reset", Ctestlmksearchapi::CPosLmDisplayData_ResetL),
		ENTRY( "CPosLmDisplayData_DisplayItem", Ctestlmksearchapi::CPosLmDisplayData_DisplayItemL),
		ENTRY( "CPosLmDisplayData_SetPartialReadParametersL", Ctestlmksearchapi::CPosLmDisplayData_SetPartialReadParametersL),
		ENTRY( "CPosLmDisplayData_UnsetPartialReadParameters", Ctestlmksearchapi::CPosLmDisplayData_UnsetPartialReadParametersL),
		ENTRY( "CPosLmNearestCriteria_NewLC", Ctestlmksearchapi::CPosLmNearestCriteria_NewLC),
		ENTRY( "CPosLmNearestCriteria_GetCoordinate", Ctestlmksearchapi::CPosLmNearestCriteria_GetCoordinateL),
		ENTRY( "CPosLmNearestCriteria_SetCoordinate", Ctestlmksearchapi::CPosLmNearestCriteria_SetCoordinateL),
		ENTRY( "CPosLmNearestCriteria_UseCoverageRadius", Ctestlmksearchapi::CPosLmNearestCriteria_UseCoverageRadiusL),
		ENTRY( "CPosLmNearestCriteria_SetUseCoverageRadius", Ctestlmksearchapi::CPosLmNearestCriteria_SetUseCoverageRadiusL),
		ENTRY( "CPosLmNearestCriteria_MaxDistance", Ctestlmksearchapi::CPosLmNearestCriteria_MaxDistanceL),
		ENTRY( "CPosLmNearestCriteria_SetMaxDistance", Ctestlmksearchapi::CPosLmNearestCriteria_SetMaxDistanceL),
		ENTRY( "CPosLmTextCriteria_NewLC", Ctestlmksearchapi::CPosLmTextCriteria_NewLC),
		ENTRY( "CPosLmTextCriteria_Text", Ctestlmksearchapi::CPosLmTextCriteria_TextL),
		ENTRY( "CPosLmTextCriteria_SetTextL", Ctestlmksearchapi::CPosLmTextCriteria_SetTextL),
		ENTRY( "CPosLmTextCriteria_AttributesToSearch", Ctestlmksearchapi::CPosLmTextCriteria_AttributesToSearchL),
		ENTRY( "CPosLmTextCriteria_SetAttributesToSearch", Ctestlmksearchapi::CPosLmTextCriteria_SetAttributesToSearchL),
		ENTRY( "CPosLmTextCriteria_GetPositionFieldsToSearchL", Ctestlmksearchapi::CPosLmTextCriteria_GetPositionFieldsToSearchL),
		ENTRY( "CPosLmTextCriteria_SetPositionFieldsToSearchL", Ctestlmksearchapi::CPosLmTextCriteria_SetPositionFieldsToSearchL),
		ENTRY( "CPosLmTextCriteria_ClearPositionFieldsToSearch", Ctestlmksearchapi::CPosLmTextCriteria_ClearPositionFieldsToSearchL),
		ENTRY( "CPosLmIdListCriteria_NewLC", Ctestlmksearchapi::CPosLmIdListCriteria_NewLC),
		ENTRY( "CPosLmIdListCriteria_SetLandmarkIdsL", Ctestlmksearchapi::CPosLmIdListCriteria_SetLandmarkIdsL),
		ENTRY( "CPosLmIdListCriteria_GetLandmarkIdsL", Ctestlmksearchapi::CPosLmIdListCriteria_GetLandmarkIdsL),
		ENTRY( "CPosLmDisplayItem_NewL1", Ctestlmksearchapi::CPosLmDisplayItem_NewL1L),
		ENTRY( "CPosLmDisplayItem_NewL2", Ctestlmksearchapi::CPosLmDisplayItem_NewL2L),
		ENTRY( "CPosLmDisplayItem_DisplayItemType", Ctestlmksearchapi::CPosLmDisplayItem_DisplayItemTypeL),
		ENTRY( "CPosLmDisplayItem_DatabaseIndex", Ctestlmksearchapi::CPosLmDisplayItem_DatabaseIndexL),
		ENTRY( "CPosLmDisplayItem_Category", Ctestlmksearchapi::CPosLmDisplayItem_CategoryL),
		ENTRY( "CPosLmDisplayItem_Landmark", Ctestlmksearchapi::CPosLmDisplayItem_LandmarkL),
		ENTRY( "CPosLmDisplayItem_GetDistance", Ctestlmksearchapi::CPosLmDisplayItem_GetDistanceL),
		ENTRY( "CPosLmMultiDbSearch_NewL", Ctestlmksearchapi::CPosLmMultiDbSearch_NewL),
		ENTRY( "CPosLmMultiDbSearch_SetDatabasesToSearchL", Ctestlmksearchapi::CPosLmMultiDbSearch_SetDatabasesToSearchL),
		ENTRY( "CPosLmMultiDbSearch_DatabasesToSearchL", Ctestlmksearchapi::CPosLmMultiDbSearch_DatabasesToSearchL),
		ENTRY( "CPosLmMultiDbSearch_SetMaxNumOfMatches", Ctestlmksearchapi::CPosLmMultiDbSearch_SetMaxNumOfMatchesL),
		ENTRY( "CPosLmMultiDbSearch_StartLandmarkSearchL1", Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL1L),
		ENTRY( "CPosLmMultiDbSearch_StartLandmarkSearchL2", Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL2L),
		ENTRY( "CPosLmMultiDbSearch_StartLandmarkSearchL3", Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL3L),
		ENTRY( "CPosLmMultiDbSearch_StartCategorySearchL", Ctestlmksearchapi::CPosLmMultiDbSearch_StartCategorySearchL),
		ENTRY( "CPosLmMultiDbSearch_NumOfSearchErrors", Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfSearchErrorsL),
		ENTRY( "CPosLmMultiDbSearch_GetSearchError", Ctestlmksearchapi::CPosLmMultiDbSearch_GetSearchErrorL),
		ENTRY( "CPosLmMultiDbSearch_TotalNumOfMatches", Ctestlmksearchapi::CPosLmMultiDbSearch_TotalNumOfMatchesL),
		ENTRY( "CPosLmMultiDbSearch_NumOfMatches", Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfMatchesL),
		ENTRY( "CPosLmMultiDbSearch_NumOfDatabasesToSearch", Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfDatabasesToSearchL),
		ENTRY( "CPosLmMultiDbSearch_DatabaseUriPtr", Ctestlmksearchapi::CPosLmMultiDbSearch_DatabaseUriPtrL),
		ENTRY( "CPosLmMultiDbSearch_MatchIteratorL", Ctestlmksearchapi::CPosLmMultiDbSearch_MatchIteratorL),
		ENTRY( "CPosLmMultiDbSearch_SetDisplayData", Ctestlmksearchapi::CPosLmMultiDbSearch_SetDisplayDataL),
		ENTRY( "CPosLmMultiDbSearch_UnsetDisplayData", Ctestlmksearchapi::CPosLmMultiDbSearch_UnsetDisplayDataL),
		ENTRY( "CPosLmMultiDbSearch_MaxNumOfMatches", Ctestlmksearchapi::CPosLmMultiDbSearch_MaxNumOfMatchesL),
		};	

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );
	
	TInt ret = KErrNone;
	TRAPD( error, ret = RunInternalL( KFunctions, count, aItem ));
	if ( error )
		{
		return error;
		}
    return ret;
    }




// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_NewL
// This test case test the NewL function of CPosLandmarkSearch class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_NewL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestNewL();
	CleanupStack::PopAndDestroy(testPosLmkSearch);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_MaxNumOfMatchesL
// This test case test the MaxNumOfMatches function of CPosLandmarkSearch class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_MaxNumOfMatchesL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestMaxNumOfMatchesL();
	CleanupStack::PopAndDestroy(testPosLmkSearch);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_SetMaxNumOfMatchesL
// This test case test the SetMaxNumOfMatches function of CPosLandmarkSearch class.
// Default value is checked here
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_SetMaxNumOfMatchesL( CStifItemParser& aItem)
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	TInt value;
	if(aItem.GetNextInt(value)==KErrNotFound)
		{
		testPosLmkSearch->TestSetMaxNumOfMatchesL();	
		}
	else
		{
		testPosLmkSearch->TestSetMaxNumOfMatchesL(10);
		}
	CleanupStack::PopAndDestroy(testPosLmkSearch);
	return KErrNone;	
	}

	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL
// This test case test the StartLandmarkSearchL function of CPosLandmarkSearch class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL( CStifItemParser& aItem)
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	TInt value;
	if(aItem.GetNextInt(value)==KErrNotFound)
		{
		testPosLmkSearch->TestStartLandmarkSearchL();	
		}
	else
		{
		testPosLmkSearch->TestStartLandmarkSearchL(ETrue);	
		}
	CleanupStack::Pop(testPosLmkSearch);
	delete testPosLmkSearch ;
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL 
// This test case test the StartLandmarkSearchL function of CPosLandmarkSearch class.
// Sorting type is specified.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL1L( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestStartLandmarkSearchL1L();
	CleanupStack::Pop(testPosLmkSearch);
	delete testPosLmkSearch ;
	return KErrNone;		
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL 
// This test case test the StartLandmarkSearchL function of CPosLandmarkSearch class.
// Sorting type and category of sort is specified
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_StartLandmarkSearchL2L( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestStartLandmarkSearchL2L();
	CleanupStack::Pop(testPosLmkSearch);
	delete testPosLmkSearch ;
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_NumOfMatchesL 
// This test case test the StartLandmarkSearchL function of CPosLandmarkSearch class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_NumOfMatchesL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestNumOfMatchesL();
	CleanupStack::Pop(testPosLmkSearch);
	delete testPosLmkSearch ;
	return KErrNone;		
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_MatchIteratorL 
// This test case test the MatchIteratorL function of CPosLandmarkSearch class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_MatchIteratorL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestMatchIteratorL();
	CleanupStack::Pop(testPosLmkSearch);
	delete testPosLmkSearch ;
	return KErrNone;		
	}


// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_SetDisplayDataL 
// This test case test the SetDisplayData function of CPosLandmarkSearch class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_SetDisplayDataL( CStifItemParser&/* aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestSetDisplayDataL();
	CleanupStack::Pop(testPosLmkSearch);
	delete testPosLmkSearch ;
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLandmarkSearch_UnsetDisplayDataL 
// This test case test the UnsetDisplayData function of CPosLandmarkSearch class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLandmarkSearch_UnsetDisplayDataL( CStifItemParser&/* aItem*/ )
	{
	CTestPosLmkSearch* testPosLmkSearch=CTestPosLmkSearch::NewLC();
	testPosLmkSearch->TestUnsetDisplayDataL();
	CleanupStack::Pop(testPosLmkSearch);
	delete testPosLmkSearch ;
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmAreaCriteria_NewLC
// This test case test the NewLC function of CPosLmAreaCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmAreaCriteria_NewLC( CStifItemParser& aItem )
	{
	CTestPosLmAreaCriteria* testareaCriteria=CTestPosLmAreaCriteria::NewLC();
	
	TReal64 southLat;
	TReal64	northLat;
	TReal64	westLong;
	TReal64	eastLong;
	
	TPtrC ptr1;
	TInt err1= aItem.GetNextString(ptr1);
	TLex lex1(ptr1);
        lex1.Val(southLat);
        
        TPtrC ptr2;
	TInt err2= aItem.GetNextString(ptr2);
	TLex lex2(ptr2);
        lex2.Val(northLat);

	TPtrC ptr3;
	TInt err3= aItem.GetNextString(ptr3);
	TLex lex3(ptr3);
	lex3.Val(westLong);

	TPtrC ptr4;
	TInt err4= aItem.GetNextString(ptr4);
	TLex lex4(ptr4);
	lex4.Val(eastLong);

	testareaCriteria->TestNewLC(southLat,northLat,westLong,eastLong);
	CleanupStack::PopAndDestroy(testareaCriteria);
	return KErrNone;
	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmAreaCriteria_GetSearchAreaL
// This test case test the GetSearchArea function of CPosLmAreaCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmAreaCriteria_GetSearchAreaL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmAreaCriteria* testareaCriteria=CTestPosLmAreaCriteria::NewLC();
	testareaCriteria->TestGetSearchAreaL();
	CleanupStack::PopAndDestroy(testareaCriteria);
	return KErrNone;
	}

	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmAreaCriteria_SetSearchAreaL
// This test case test the SetSearchArea function of CPosLmAreaCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmAreaCriteria_SetSearchAreaL( CStifItemParser& aItem)
	{
	CTestPosLmAreaCriteria* testareaCriteria=CTestPosLmAreaCriteria::NewLC();
	
	TReal64 southLat;
	TReal64	northLat;
	TReal64	westLong;
	TReal64	eastLong;
		
	TPtrC ptr1;
	TInt err1= aItem.GetNextString(ptr1);
	TLex lex1(ptr1);
        lex1.Val(southLat);
        
        TPtrC ptr2;
	TInt err2= aItem.GetNextString(ptr2);
	TLex lex2(ptr2);
        lex2.Val(northLat);

	TPtrC ptr3;
	TInt err3= aItem.GetNextString(ptr3);
	TLex lex3(ptr3);
	lex3.Val(westLong);

	TPtrC ptr4;
	TInt err4= aItem.GetNextString(ptr4);
	TLex lex4(ptr4);
	lex4.Val(eastLong);

	testareaCriteria->TestSetSearchAreaL(southLat,northLat,westLong,eastLong);
	CleanupStack::PopAndDestroy(testareaCriteria);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCategoryCriteria_NewLC
// This test case test the NewLC function of CPosLmCategoryCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCategoryCriteria_NewLC( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCategoryCriteria* testCategoryCriteria=CTestPosLmCategoryCriteria::NewLC();
	testCategoryCriteria->TestNewLC();
	CleanupStack::PopAndDestroy(testCategoryCriteria);
	return KErrNone;	
	}
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCategoryCriteria_CategoryItemIdL
// This test case test the CategoryItemId function of CPosLmCategoryCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCategoryCriteria_CategoryItemIdL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCategoryCriteria* testCategoryCriteria=CTestPosLmCategoryCriteria::NewLC();
	testCategoryCriteria->TestCategoryItemIdL();
	CleanupStack::PopAndDestroy(testCategoryCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCategoryCriteria_GlobalCategoryL
// This test case test the GlobalCategory function of CPosLmCategoryCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCategoryCriteria_GlobalCategoryL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCategoryCriteria* testCategoryCriteria=CTestPosLmCategoryCriteria::NewLC();
	testCategoryCriteria->TestGlobalCategoryL();
	CleanupStack::PopAndDestroy(testCategoryCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCategoryCriteria_CategoryNameL
// This test case test the CategoryName function of CPosLmCategoryCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCategoryCriteria_CategoryNameL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCategoryCriteria* testCategoryCriteria=CTestPosLmCategoryCriteria::NewLC();
	testCategoryCriteria->TestCategoryNameL();
	CleanupStack::PopAndDestroy(testCategoryCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCategoryCriteria_SetCategoryItemIdL
// This test case test the SetCategoryItemId function of CPosLmCategoryCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCategoryCriteria_SetCategoryItemIdL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCategoryCriteria* testCategoryCriteria=CTestPosLmCategoryCriteria::NewLC();
	testCategoryCriteria->TestSetCategoryItemIdL();
	CleanupStack::PopAndDestroy(testCategoryCriteria);
	return KErrNone;	
	}
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCategoryCriteria_SetGlobalCategoryL
// This test case test the SetGlobalCategory function of CPosLmCategoryCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCategoryCriteria_SetGlobalCategoryL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCategoryCriteria* testCategoryCriteria=CTestPosLmCategoryCriteria::NewLC();
	testCategoryCriteria->TestSetGlobalCategoryL();
	CleanupStack::PopAndDestroy(testCategoryCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCategoryCriteria_SetCategoryNameL
// This test case test the SetCategoryNameL function of CPosLmCategoryCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCategoryCriteria_SetCategoryNameL( CStifItemParser& aItem)
	{
	CTestPosLmCategoryCriteria* testCategoryCriteria=CTestPosLmCategoryCriteria::NewLC();
	TPtrC categoryName;
	TInt error=aItem.GetNextString(categoryName);
	testCategoryCriteria->TestSetCategoryNameL(categoryName);
	CleanupStack::PopAndDestroy(testCategoryCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCatNameCriteria_NewLC
// This test case test the NewLC function of CPosLmCatNameCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCatNameCriteria_NewLC( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCatNameCriteria* testNameCriteria=CTestPosLmCatNameCriteria::NewLC();
	testNameCriteria->TestNewLC();
	CleanupStack::PopAndDestroy(testNameCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCatNameCriteria_SearchPatternL
// This test case test the SearchPattern function of CPosLmCatNameCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCatNameCriteria_SearchPatternL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCatNameCriteria* testNameCriteria=CTestPosLmCatNameCriteria::NewLC();
	testNameCriteria->TestSearchPatternL();
	CleanupStack::PopAndDestroy(testNameCriteria);
	return KErrNone;	
	}


// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCatNameCriteria_SetSearchPatternL
// This test case test the SetSearchPatternL function of CPosLmCatNameCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCatNameCriteria_SetSearchPatternL( CStifItemParser& aItem )
	{
	CTestPosLmCatNameCriteria* testNameCriteria=CTestPosLmCatNameCriteria::NewLC();
	TPtrC searchPattern;
	TInt error=aItem.GetNextString(searchPattern);
	testNameCriteria->TestSetSearchPatternL(searchPattern);
	CleanupStack::PopAndDestroy(testNameCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_NewLC
// This test case test the NewLC function of CPosLmCompositeCriteria class with ECompositionAND composition type.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_NewLC_AND( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestNewLC_AND();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_NewLC
// This test case test the NewLC function of CPosLmCompositeCriteria class with ECompositionOR composition type.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_NewLC_OR( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestNewLC_OR();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}	
	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_CompositionTypeL
// This test case test the CompositionType function of CPosLmCompositeCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_CompositionTypeL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestCompositionTypeL();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_SetCompositionTypeL
// This test case test the SetCompositionType function of CPosLmCompositeCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_SetCompositionTypeL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestSetCompositionTypeL();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_NumOfArgumentsL
// This test case test the NumOfArguments function of CPosLmCompositeCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_NumOfArgumentsL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestNumOfArgumentsL();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_Argument1L
// This test case test the Argument function of CPosLmCompositeCriteria class
// which returns constant pointer.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_Argument1L( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestArgument1L();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_Argument2L
// This test case test the Argument function of CPosLmCompositeCriteria class
// returns pointer which is not constant.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_Argument2L( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestArgument2L();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_AddArgumentL
// This test case test the AddArgument function of CPosLmCompositeCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_AddArgumentL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestAddArgumentL();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_RemoveArgumentL
// This test case test the RemoveArgument function of CPosLmCompositeCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_RemoveArgumentL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestRemoveArgumentL();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmCompositeCriteria_ClearArgumentsL
// This test case test the ClearArguments function of CPosLmCompositeCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmCompositeCriteria_ClearArgumentsL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmCompositeCriteria* testCompositeCriteria=CTestPosLmCompositeCriteria::NewLC();
	testCompositeCriteria->TestClearArgumentsL();
	CleanupStack::PopAndDestroy(testCompositeCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayData_NewL
// This test case test the NewL function of CPosLmDisplayData class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayData_NewL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmDisplayData* testDisplayData=CTestPosLmDisplayData::NewLC();
	testDisplayData->TestNewL();
	CleanupStack::PopAndDestroy(testDisplayData);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayData_NewItemIndexL
// This test case test the NewItemIndex function of CPosLmDisplayData class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayData_NewItemIndexL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmDisplayData* testDisplayData=CTestPosLmDisplayData::NewLC();
	testDisplayData->TestNewItemIndexL();
	CleanupStack::PopAndDestroy(testDisplayData);
	return KErrNone;	
	}	

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayData_CountL
// This test case test the Count function of CPosLmDisplayData class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayData_CountL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmDisplayData* testDisplayData=CTestPosLmDisplayData::NewLC();
	testDisplayData->TestCountL();
	CleanupStack::PopAndDestroy(testDisplayData);
	return KErrNone;	
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayData_ResetL
// This test case test the Reset function of CPosLmDisplayData class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayData_ResetL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmDisplayData* testDisplayData=CTestPosLmDisplayData::NewLC();
	testDisplayData->TestResetL();
	CleanupStack::PopAndDestroy(testDisplayData);
	return KErrNone;	
	}		


// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayData_DisplayItemL
// This test case test the DisplayItem function of CPosLmDisplayData class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayData_DisplayItemL( CStifItemParser& aItem)
	{
	CTestPosLmDisplayData* testDisplayData=CTestPosLmDisplayData::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	if(value)
		{
		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 14);

		}
	testDisplayData->TestDisplayItemL(value);
	CleanupStack::PopAndDestroy(testDisplayData);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayData_SetPartialReadParametersL
// This test case test the SetPartialReadParametersL function of CPosLmDisplayData class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayData_SetPartialReadParametersL( CStifItemParser& aItem)
	{
	CTestPosLmDisplayData* testDisplayData=CTestPosLmDisplayData::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testDisplayData->TestSetPartialReadParametersL(value);
	CleanupStack::PopAndDestroy(testDisplayData);
	return KErrNone;	
	}			

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayData_UnsetPartialReadParametersL
// This test case test the UnsetPartialReadParameters function of CPosLmDisplayData class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayData_UnsetPartialReadParametersL( CStifItemParser& /*aItem*/ )
	{
	CTestPosLmDisplayData* testDisplayData=CTestPosLmDisplayData::NewLC();
	testDisplayData->TestUnsetPartialReadParametersL();
	CleanupStack::PopAndDestroy(testDisplayData);
	return KErrNone;	
	}	
	
		
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmNearestCriteria_NewLC
// This test case test the NewLC function of CPosLmNearestCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmNearestCriteria_NewLC( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmNearestCriteria* testNearestCriteria=CTestPosLmNearestCriteria::NewLC();
	testNearestCriteria->TestNewLC();
	CleanupStack::PopAndDestroy(testNearestCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmNearestCriteria_GetCoordinateL
// This test case test the GetCoordinate function of CPosLmNearestCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmNearestCriteria_GetCoordinateL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmNearestCriteria* testNearestCriteria=CTestPosLmNearestCriteria::NewLC();
	testNearestCriteria->TestGetCoordinateL();
	CleanupStack::PopAndDestroy(testNearestCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmNearestCriteria_SetCoordinateL
// This test case test the SetCoordinate function of CPosLmNearestCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmNearestCriteria_SetCoordinateL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmNearestCriteria* testNearestCriteria=CTestPosLmNearestCriteria::NewLC();
	testNearestCriteria->TestSetCoordinateL();
	CleanupStack::PopAndDestroy(testNearestCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmNearestCriteria_UseCoverageRadiusL
// This test case test the UseCoverageRadius function of CPosLmNearestCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmNearestCriteria_UseCoverageRadiusL( CStifItemParser& aItem )	
	{
	CTestPosLmNearestCriteria* testNearestCriteria=CTestPosLmNearestCriteria::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testNearestCriteria->TestUseCoverageRadiusL(value);
	CleanupStack::PopAndDestroy(testNearestCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmNearestCriteria_SetUseCoverageRadiusL
// This test case test the SetUseCoverageRadius function of CPosLmNearestCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmNearestCriteria_SetUseCoverageRadiusL( CStifItemParser& aItem )	
	{
	CTestPosLmNearestCriteria* testNearestCriteria=CTestPosLmNearestCriteria::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testNearestCriteria->TestSetUseCoverageRadiusL(value);
	CleanupStack::PopAndDestroy(testNearestCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmNearestCriteria_MaxDistanceL
// This test case test the MaxDistance function of CPosLmNearestCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmNearestCriteria_MaxDistanceL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmNearestCriteria* testNearestCriteria=CTestPosLmNearestCriteria::NewLC();
	testNearestCriteria->TestMaxDistanceL();
	CleanupStack::PopAndDestroy(testNearestCriteria);
	return KErrNone;	
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmNearestCriteria_SetMaxDistanceL
// This test case test the SetMaxDistance function of CPosLmNearestCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmNearestCriteria_SetMaxDistanceL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmNearestCriteria* testNearestCriteria=CTestPosLmNearestCriteria::NewLC();
	testNearestCriteria->TestSetMaxDistanceL();
	CleanupStack::PopAndDestroy(testNearestCriteria);
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_NewLC
// This test case test the  NewLC function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_NewLC( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestNewLC();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}
		
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_TextL
// This test case test the  Text function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_TextL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestTextL();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_SetTextL
// This test case test the  SetTextL function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_SetTextL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestSetTextL();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_AttributesToSearchL
// This test case test the  AttributesToSearch function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_AttributesToSearchL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestAttributesToSearchL();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_SetAttributesToSearchL
// This test case test the  SetAttributesToSearch function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_SetAttributesToSearchL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestSetAttributesToSearchL();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_GetPositionFieldsToSearchL
// This test case test the  GetPositionFieldsToSearchL function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_GetPositionFieldsToSearchL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestGetPositionFieldsToSearchL();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_SetPositionFieldsToSearchL
// This test case test the  SetPositionFieldsToSearchL function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_SetPositionFieldsToSearchL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestSetPositionFieldsToSearchL();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmTextCriteria_ClearPositionFieldsToSearchL
// This test case test the  ClearPositionFieldsToSearch function of CPosLmTextCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmTextCriteria_ClearPositionFieldsToSearchL( CStifItemParser& /*aItem*/ )	
	{
	CTestPosLmTextCriteria* testTextCriteria=CTestPosLmTextCriteria::NewLC();
	testTextCriteria->TestClearPositionFieldsToSearchL();
	CleanupStack::PopAndDestroy(testTextCriteria);
	return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmIdListCriteria_NewLC
// This test case test the  NewLC function of CPosLmIdListCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmIdListCriteria_NewLC( CStifItemParser& /*aItem*/ )				
	{
	CTestPosLmIdListCriteria* testListCriteria=CTestPosLmIdListCriteria::NewLC();
	testListCriteria->TestNewLC();
	CleanupStack::PopAndDestroy(testListCriteria);
	return KErrNone;		
	}
		
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmIdListCriteria_SetLandmarkIdsL
// This test case test the  SetLandmarkIdsL function of CPosLmIdListCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmIdListCriteria_SetLandmarkIdsL( CStifItemParser& /*aItem*/ )				
	{
	CTestPosLmIdListCriteria* testListCriteria=CTestPosLmIdListCriteria::NewLC();
	testListCriteria->TestSetLandmarkIdsL();
	CleanupStack::PopAndDestroy(testListCriteria);
	return KErrNone;		
	}
	
		
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmIdListCriteria_GetLandmarkIdsL
// This test case test the  GetLandmarkIdsL function of CPosLmIdListCriteria class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmIdListCriteria_GetLandmarkIdsL( CStifItemParser& /*aItem*/ )				
	{
	CTestPosLmIdListCriteria* testListCriteria=CTestPosLmIdListCriteria::NewLC();
	testListCriteria->TestGetLandmarkIdsL();
	CleanupStack::PopAndDestroy(testListCriteria);
	return KErrNone;		
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayItem_NewL1L
// This test case test the  NewL function of CPosLmDisplayItem class which takes 
// CPosLandmark pointer.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayItem_NewL1L( CStifItemParser& aItem )
	{
	CTestPosLmDisplayItem* testDisplayItem=CTestPosLmDisplayItem::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testDisplayItem->TestNewL1L(value);
	CleanupStack::PopAndDestroy(testDisplayItem);
	return KErrNone;		
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayItem_NewL2L
// This test case test the  NewL function of CPosLmDisplayItem class which takes 
// CPosLandmarkCategory pointer.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayItem_NewL2L( CStifItemParser& aItem )
	{
	CTestPosLmDisplayItem* testDisplayItem=CTestPosLmDisplayItem::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testDisplayItem->TestNewL2L(value);
	CleanupStack::PopAndDestroy(testDisplayItem);
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayItem_DisplayItemTypeL
// This test case test the  DisplayItemType function of CPosLmDisplayItem class.
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayItem_DisplayItemTypeL( CStifItemParser& aItem )
	{
	CTestPosLmDisplayItem* testDisplayItem=CTestPosLmDisplayItem::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testDisplayItem->TestDisplayItemTypeL(value);
	CleanupStack::PopAndDestroy(testDisplayItem);
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayItem_DatabaseIndexL
// This test case test the  DatabaseIndex function of CPosLmDisplayItem class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayItem_DatabaseIndexL( CStifItemParser& aItem )
	{
	CTestPosLmDisplayItem* testDisplayItem=CTestPosLmDisplayItem::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testDisplayItem->TestDatabaseIndexL(value);
	CleanupStack::PopAndDestroy(testDisplayItem);
	return KErrNone;		
	}		

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayItem_CategoryL
// This test case test the  Category function of CPosLmDisplayItem class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayItem_CategoryL( CStifItemParser& aItem )
	{
	CTestPosLmDisplayItem* testDisplayItem=CTestPosLmDisplayItem::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	if(value)
		{
		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 15 );

		}
	testDisplayItem->TestCategoryL(value);
	CleanupStack::PopAndDestroy(testDisplayItem);
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayItem_LandmarkL
// This test case test the  Landmark function of CPosLmDisplayItem class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayItem_LandmarkL( CStifItemParser& aItem )
	{
	CTestPosLmDisplayItem* testDisplayItem=CTestPosLmDisplayItem::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	if(value)
		{
		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 15 );
		}
	testDisplayItem->TestLandmarkL(value);
	CleanupStack::PopAndDestroy(testDisplayItem);
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmDisplayItem_GetDistanceL
// This test case test the  GetDistance function of CPosLmDisplayItem class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmDisplayItem_GetDistanceL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmDisplayItem* testDisplayItem=CTestPosLmDisplayItem::NewLC();
	testDisplayItem->TestGetDistanceL();	
	CleanupStack::PopAndDestroy(testDisplayItem);
	return KErrNone;		
	}
	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_NewL
// This test case test the  NewL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_NewL( CStifItemParser& aItem)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testMultiDbSearch->TestNewL(value);
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}
	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_SetDatabasesToSearchL
// This test case test the  SetDatabasesToSearchL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_SetDatabasesToSearchL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestSetDatabasesToSearchL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_DatabasesToSearchL
// This test case test the  DatabasesToSearchL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_DatabasesToSearchL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestDatabasesToSearchL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}	

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_SetMaxNumOfMatchesL
// This test case test the  SetMaxNumOfMatches function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_SetMaxNumOfMatchesL( CStifItemParser& aItem)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	testMultiDbSearch->TestSetMaxNumOfMatchesL(value);
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}			

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL1L
// This test case test the  StartLandmarkSearchL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL1L( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestStartLandmarkSearchL1L();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL2L
// This test case test the overloaded StartLandmarkSearchL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL2L( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestStartLandmarkSearchL2L();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}	
		
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL3L
// This test case test the overloaded StartLandmarkSearchL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_StartLandmarkSearchL3L( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestStartLandmarkSearchL3L();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_StartCategorySearchL
// This test case test the StartCategorySearchL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_StartCategorySearchL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestStartCategorySearchL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfSearchErrorsL
// This test case test the  NumOfSearchErrors function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfSearchErrorsL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestNumOfSearchErrorsL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_GetSearchErrorL
// This test case test the  GetSearchError function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_GetSearchErrorL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestGetSearchErrorL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}

	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_TotalNumOfMatchesL
// This test case test the  TotalNumOfMatches function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_TotalNumOfMatchesL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestTotalNumOfMatchesL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}	
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfMatchesL
// This test case test the  NumOfMatches function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfMatchesL( CStifItemParser& aItem)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	if(value)
		{
		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 14);
		}
	testMultiDbSearch->TestNumOfMatchesL(value);
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}
	
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfDatabasesToSearchL
// This test case test the  NumOfDatabasesToSearch function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_NumOfDatabasesToSearchL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestNumOfDatabasesToSearchL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}				

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_DatabaseUriPtrL
// This test case test the  DatabaseUriPtr function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_DatabaseUriPtrL( CStifItemParser& aItem)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	TInt value;
	TInt error=aItem.GetNextInt(value);
	if(value)
		{
		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, 14);
		}
	testMultiDbSearch->TestDatabaseUriPtrL(value);
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_MatchIteratorL
// This test case test the  MatchIteratorL function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_MatchIteratorL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestMatchIteratorL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}				
				
			
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_SetDisplayDataL
// This test case test the  SetDisplayData function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_SetDisplayDataL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestSetDisplayDataL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}				
				
// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_UnsetDisplayDataL
// This test case test the  UnsetDisplayData function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_UnsetDisplayDataL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestUnsetDisplayDataL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}				

// -----------------------------------------------------------------------------
// Ctestlmksearchapi::CPosLmMultiDbSearch_MaxNumOfMatchesL
// This test case test the  MaxNumOfMatches function of CPosLmMultiDbSearch class .
// -----------------------------------------------------------------------------
//
TInt Ctestlmksearchapi::CPosLmMultiDbSearch_MaxNumOfMatchesL( CStifItemParser& /*aItem*/)
	{
	CTestPosLmMultiDbSearch* testMultiDbSearch=CTestPosLmMultiDbSearch::NewLC();
	testMultiDbSearch->TestMaxNumOfMatchesL();
	CleanupStack::PopAndDestroy(testMultiDbSearch);
	return KErrNone;		
	}				
	
	
//  End of File
