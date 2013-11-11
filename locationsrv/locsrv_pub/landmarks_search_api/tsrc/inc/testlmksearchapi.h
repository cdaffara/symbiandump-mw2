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
* Description:  Ctestlmksearchapi class
*
*/



#ifndef TESTLMKSEARCHAPI_H
#define TESTLMKSEARCHAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


// Logging path
_LIT( KtestlmksearchapiLogPath, "\\logs\\testframework\\testlmksearchapi\\" ); 
// Log file
_LIT( KtestlmksearchapiLogFile, "testlmksearchapi.txt" ); 

class Ctestlmksearchapi;


// CLASS DECLARATION

/**
*  Ctestlmksearchapi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib testlmksearchapi.lib
*  @since S60 v3.2
*/
NONSHARABLE_CLASS(Ctestlmksearchapi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Ctestlmksearchapi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Ctestlmksearchapi();


        

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    
    private:

        /**
        * C++ default constructor.
        */
        Ctestlmksearchapi( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        
        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

                
        /**
        * Test case for NewL of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_NewL( CStifItemParser& aItem );
        
        /**
        * Test case for MaxNumOfMatches of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
         TInt CPosLandmarkSearch_MaxNumOfMatchesL( CStifItemParser& aItem );
       
        /**
        * Test case for SetMaxNumOfMatches of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_SetMaxNumOfMatchesL( CStifItemParser& aItem );
		
		/**
        * Test case for StartLandmarkSearchL of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_StartLandmarkSearchL( CStifItemParser& aItem );
		
		/**
        * Test case for StartLandmarkSearchL of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_StartLandmarkSearchL1L( CStifItemParser& aItem );
        
        /**
        * Test case for StartLandmarkSearchL of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_StartLandmarkSearchL2L( CStifItemParser& aItem );
	
		/**.
        * Test case for NumOfMatches of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_NumOfMatchesL( CStifItemParser& aItem );
        
        /**.
        * Test case for MatchIteratorL of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
		TInt CPosLandmarkSearch_MatchIteratorL( CStifItemParser& aItem );
	
		/**
        * Test case for SetDisplayData of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_SetDisplayDataL( CStifItemParser& aItem );
	
		/**
        * Test case for UnsetDisplayData of CPosLandmarkSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLandmarkSearch_UnsetDisplayDataL( CStifItemParser& aItem );
		
		/**
        * Test case for NewLC of CPosLmAreaCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmAreaCriteria_NewLC( CStifItemParser& aItem );
	
		/**
        * Test case for GetSearchArea of CPosLmAreaCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmAreaCriteria_GetSearchAreaL( CStifItemParser& aItem );
	
		/**
        * Test case for SetSearchArea of CPosLmAreaCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmAreaCriteria_SetSearchAreaL( CStifItemParser& aItem );
	
		/**
        * Test case for NewLC of CPosLmCategoryCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCategoryCriteria_NewLC( CStifItemParser& aItem );
	
		/**
        * Test case for CategoryItemId of CPosLmCategoryCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCategoryCriteria_CategoryItemIdL( CStifItemParser& aItem );
     
       	/**
        * Test case for GlobalCategory of CPosLmCategoryCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCategoryCriteria_GlobalCategoryL( CStifItemParser& aItem );
	
		/**
        * Test case for CategoryName of CPosLmCategoryCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCategoryCriteria_CategoryNameL( CStifItemParser& aItem );
     
        /**
        * Test case for SetCategoryItemId of CPosLmCategoryCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCategoryCriteria_SetCategoryItemIdL( CStifItemParser& aItem );
    
        /**
        * Test case for SetGlobalCategory of CPosLmCategoryCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCategoryCriteria_SetGlobalCategoryL( CStifItemParser& aItem );
    
        /**
        * Test case for SetCategoryNameL of CPosLmCategoryCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCategoryCriteria_SetCategoryNameL( CStifItemParser& aItem );
    
        /**
        * Test case for NewLC of CPosLmCatNameCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCatNameCriteria_NewLC( CStifItemParser& aItem );
    
        /**
        * Test case for SearchPattern of CPosLmCatNameCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCatNameCriteria_SearchPatternL( CStifItemParser& aItem );
    
        /**
        * Test case for SetSearchPatternL of CPosLmCatNameCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCatNameCriteria_SetSearchPatternL( CStifItemParser& aItem );
        /**
        * Test case for NewLC of CPosLmCompositeCriteria class with ECompositionAND composition type.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_NewLC_AND( CStifItemParser& aItem );
               
        /**
        * Test case for NewLC of CPosLmCompositeCriteria class ECompositionOR composition type..
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_NewLC_OR( CStifItemParser& aItem );
             
        /**
        * Test case for CompositionType of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_CompositionTypeL( CStifItemParser& aItem );
    
        /**
        * Test case for SetCompositionType of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_SetCompositionTypeL( CStifItemParser& aItem );
    
        /**
        * Test case for MaxNumOfMatches of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_NumOfArgumentsL( CStifItemParser& aItem );
    
        /**
        * Test case for Argument of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_Argument1L( CStifItemParser& aItem );
     
        /**
        * Test case for Argument of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_Argument2L( CStifItemParser& aItem );
     
        /**
        * Test case for AddArgument of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_AddArgumentL( CStifItemParser& aItem );
    
        /**
        * Test case for RemoveArgument of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_RemoveArgumentL( CStifItemParser& aItem );
      
        /**
        * Test case for ClearArguments of CPosLmCompositeCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmCompositeCriteria_ClearArgumentsL( CStifItemParser& aItem );
      
        /**
        * Test case for NewL of CPosLmDisplayData class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayData_NewL( CStifItemParser& aItem );
      
        /**
        * Test case for NewItemIndex of CPosLmDisplayData class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayData_NewItemIndexL( CStifItemParser& aItem );
     
        /**
        * Test case for Count of CPosLmDisplayData class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayData_CountL( CStifItemParser& aItem );
     
        /**
        * Test case for Reset of CPosLmDisplayData class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayData_ResetL( CStifItemParser& aItem );
      
        /**
        * Test case for DisplayItem of CPosLmDisplayData class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayData_DisplayItemL( CStifItemParser& aItem );
       
        /**
        * Test case for SetPartialReadParametersL of CPosLmDisplayData class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayData_SetPartialReadParametersL( CStifItemParser& aItem );
        
        /**
        * Test case for UnsetPartialReadParameters of CPosLmDisplayData class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayData_UnsetPartialReadParametersL( CStifItemParser& aItem );
       
        /**
        * Test case for NewLC of CPosLmNearestCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmNearestCriteria_NewLC( CStifItemParser& aItem );
      
        /**
        * Test case for GetCoordinate of CPosLmNearestCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmNearestCriteria_GetCoordinateL( CStifItemParser& aItem );
      
        /**
        * Test case for SetCoordinate of CPosLmNearestCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmNearestCriteria_SetCoordinateL( CStifItemParser& aItem );
      
        /**
        * Test case for UseCoverageRadius of CPosLmNearestCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmNearestCriteria_UseCoverageRadiusL( CStifItemParser& aItem );
      
        /**
        * Test case for SetUseCoverageRadius of CPosLmNearestCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmNearestCriteria_SetUseCoverageRadiusL( CStifItemParser& aItem );
      
        /**
        * Test case for MaxDistance of CPosLmNearestCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmNearestCriteria_MaxDistanceL( CStifItemParser& aItem );
      
        /**
        * Test case for SetMaxDistance of CPosLmNearestCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmNearestCriteria_SetMaxDistanceL( CStifItemParser& aItem );
     
        /**
        * Test case for NewLC of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_NewLC( CStifItemParser& aItem );
     
        /**
        * Test case for Text of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_TextL( CStifItemParser& aItem );
     
        /**
        * Test case for SetTextL of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_SetTextL( CStifItemParser& aItem );
     
        /**
        * Test case for AttributesToSearch of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_AttributesToSearchL( CStifItemParser& aItem );
     
        /**
        * Test case for SetAttributesToSearch of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_SetAttributesToSearchL( CStifItemParser& aItem );
     
        /**
        * Test case for GetPositionFieldsToSearchL of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_GetPositionFieldsToSearchL( CStifItemParser& aItem );
     
        /**
        * Test case for SetPositionFieldsToSearchL of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_SetPositionFieldsToSearchL( CStifItemParser& aItem );
     
        /**
        * Test case for ClearPositionFieldsToSearch of CPosLmTextCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmTextCriteria_ClearPositionFieldsToSearchL( CStifItemParser& aItem );
       
        /**
        * Test case for NewLC of CPosLmIdListCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmIdListCriteria_NewLC( CStifItemParser& aItem );
      
        /**
        * Test case for SetLandmarkIdsL of CPosLmIdListCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmIdListCriteria_SetLandmarkIdsL( CStifItemParser& aItem );
     
        /**
        * Test case for GetLandmarkIdsL of CPosLmIdListCriteria class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmIdListCriteria_GetLandmarkIdsL( CStifItemParser& aItem );
        
        /**
        * Test case for NewL of CPosLmDisplayItem class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayItem_NewL1L( CStifItemParser& aItem );
     
       /**
        * Test case for NewL of CPosLmDisplayItem class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayItem_NewL2L( CStifItemParser& aItem );
        
        /**
        * Test case for DisplayItemType of CPosLmDisplayItem class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayItem_DisplayItemTypeL( CStifItemParser& aItem );
	
		/**
        * Test case for DatabaseIndex of CPosLmDisplayItem class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayItem_DatabaseIndexL( CStifItemParser& aItem );
	
		/**
        * Test case for Category of CPosLmDisplayItem class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayItem_CategoryL( CStifItemParser& aItem );
	
		/**
        * Test case for Landmark of CPosLmDisplayItem class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayItem_LandmarkL( CStifItemParser& aItem );
	
		/**
        * Test case for GetDistance of CPosLmDisplayItem class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmDisplayItem_GetDistanceL( CStifItemParser& aItem );
	
		/**
        * Test case for NewL of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_NewL( CStifItemParser& aItem );
	
		/**
        * Test case for SetDatabasesToSearchL of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_SetDatabasesToSearchL( CStifItemParser& aItem );
	
		/**
        * Test case for DatabasesToSearchL of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_DatabasesToSearchL( CStifItemParser& aItem );
	
		/**
        * Test case for SetMaxNumOfMatches of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_SetMaxNumOfMatchesL( CStifItemParser& aItem );
	
		/**
        * Test case for StartLandmarkSearchL of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_StartLandmarkSearchL1L( CStifItemParser& aItem );
	
		/**
        * Test case for StartLandmarkSearchL of CPosLaCPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_StartLandmarkSearchL2L( CStifItemParser& aItem );
	
		/**
        * Test case for StartLandmarkSearchL of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_StartLandmarkSearchL3L( CStifItemParser& aItem );
	
		/**
        * Test case for StartCategorySearchL of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_StartCategorySearchL( CStifItemParser& aItem );

		/**
        * Test case for NumOfSearchErrors of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_NumOfSearchErrorsL( CStifItemParser& aItem );
        
        /**
        * Test case for GetSearchError of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_GetSearchErrorL( CStifItemParser& aItem );
    
        /**
        * Test case for TotalNumOfMatches of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
		TInt CPosLmMultiDbSearch_TotalNumOfMatchesL( CStifItemParser& aItem );
	
		/**
        * Test case for NumOfMatches of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_NumOfMatchesL( CStifItemParser& aItem );
	
		/**
        * Test case for NumOfDatabasesToSearch of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_NumOfDatabasesToSearchL( CStifItemParser& aItem );
	
		/**
        * Test case for DatabaseUriPtr of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_DatabaseUriPtrL( CStifItemParser& aItem );
   		
   		/**
        * Test case for MatchIteratorL of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
		TInt CPosLmMultiDbSearch_MatchIteratorL ( CStifItemParser& aItem );
		
   		/**
        * Test case for SetDisplayData of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_SetDisplayDataL( CStifItemParser& aItem );
   
   		/**
        * Test case for UnsetDisplayData of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt CPosLmMultiDbSearch_UnsetDisplayDataL( CStifItemParser& aItem );
        
        /**
        * Test case for MaxNumOfMatches of CPosLmMultiDbSearch class.
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
     	TInt CPosLmMultiDbSearch_MaxNumOfMatchesL( CStifItemParser& aItem );
   
   
    };

#endif      // TESTLMKSEARCHAPI_H

// End of File
