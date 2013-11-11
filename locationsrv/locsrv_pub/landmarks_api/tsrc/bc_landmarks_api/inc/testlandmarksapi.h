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
* Description:  Declaration of CTestLandmarksApi class
*
*/



#ifndef TESTLANDMARKSAPI_H
#define TESTLANDMARKSAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include "testcposlandmark.h"
#include "testcposlandmarkcategory.h"
#include "testcposlmcategorymanager.h"
#include "testcposlmitemiterator.h"
#include "testcposlandmarkdatabase.h"
#include "testcposlandmarkencoder.h"
#include "testcposlandmarkparser.h"
#include "testcposlmoperation.h"
#include "testcposlmpartialreadparameters.h"
#include "testtposlmsortpref.h"
#include "testposlandmarkserialization.h"
#include "testposlmcategoryserialization.h"


// Logging path
_LIT( KtestlandmarksapiLogPath, "\\logs\\testframework\\testlandmarksapi\\" ); 
// Log file
_LIT( KtestlandmarksapiLogFile, "testlandmarksapi.txt" ); 



// FORWARD DECLARATIONS
class CTestLandmarksApi;


// CLASS DECLARATION

/**
*  Ctestlandmarksapi test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTestLandmarksApi) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTestLandmarksApi* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CTestLandmarksApi();

          

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        

    protected:  // Functions from base classes

        
    private:

        /**
        * C++ default constructor.
        */
        CTestLandmarksApi( CTestModuleIf& aTestModuleIf );

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
        * CreateNewLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreateNewLandmarkL( CStifItemParser& aItem );
        
        /**
        * CreateNewLandmarkLC test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreateNewLandmarkLC( CStifItemParser& aItem );
        
        /**
        * CreateLandmarkCopyL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreateLandmarkCopyL( CStifItemParser& aItem );

		/**
        * CreateLandmarkCopyLC test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreateLandmarkCopyLC( CStifItemParser& aItem );

		/**
        * SetLandmarkNameL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetLandmarkNameL( CStifItemParser& aItem );

		/**
        * GetLandmarkNameL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetLandmarkNameL( CStifItemParser& aItem );

		/**
        * SetAndGetLmkDescL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetLmkDescL( CStifItemParser& aItem );

		/**
        * SetAndGetCovRadiusL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetCovRadiusL( CStifItemParser& aItem );
			
		/**
        * SetAndGetPositionL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetPositionL( CStifItemParser& aItem );

		/**
        * SetAndGetPositionFieldL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetPositionFieldL( CStifItemParser& aItem );
		
		/**
        * NumOfPosFieldsL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NumOfPosFieldsL( CStifItemParser& aItem );

		/**
        * IsPosFieldAvailableL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt IsPosFieldAvailableL( CStifItemParser& aItem );
		
		/**
        * RemovePositionFieldL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RemovePositionFieldL( CStifItemParser& aItem );

		/**
        * RemoveAttributesL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RemoveAttributesL( CStifItemParser& aItem );

		/**
        * PositionFieldIdL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt PositionFieldIdL( CStifItemParser& aItem );

		/**
        * LandmarkIdL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt LandmarkIdL( CStifItemParser& aItem );

		/**
        * IsPartialL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt IsPartialL( CStifItemParser& aItem );

		/**
        * SetAndGetIconL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetIconL( CStifItemParser& aItem );

		/**
        * AddAndRemoveLmCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt AddAndRemoveLmCategoryL( CStifItemParser& aItem );
		
		/**
        * GetCategoriesL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetCategoriesL( CStifItemParser& aItem );

		/**
        * NewLCreateCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NewLCreateCategoryL( CStifItemParser& aItem );
        
        /**
        * NewLCCreateCategoryLC test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NewLCCreateCategoryLC( CStifItemParser& aItem );

		/**
        * NewLCreateCategoryCopyL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NewLCreateCategoryCopyL( CStifItemParser& aItem );
        
        /**
        * NewLCCreateCategoryCopyLC test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NewLCCreateCategoryCopyLC( CStifItemParser& aItem );
        
        /**
        * SetAndGetCategoryNameL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetCategoryNameL( CStifItemParser& aItem );
        
        /**
        * SetAndGetCategoryIconL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetCategoryIconL( CStifItemParser& aItem );

		/**
        * CategoryIdL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CategoryIdL( CStifItemParser& aItem );
        
        /**
        * CheckGlobalL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CheckGlobalL( CStifItemParser& aItem );
        
        /**
        * RemoveCategoryAttributesL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RemoveCategoryAttributesL( CStifItemParser& aItem );
        
        /**
        * NewLCreateCategoryManagerL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NewLCreateCategoryManagerL( CStifItemParser& aItem );
        
        /**
        * AddAndRemoveCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt AddAndRemoveCategoryL( CStifItemParser& aItem );
        
        /**
        * UpdateCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt UpdateCategoryL( CStifItemParser& aItem );
        
        /**
        * AddCatToLandmarksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt AddCatToLandmarksL( CStifItemParser& aItem );
        
        /**
        * RemoveCatFromLandmarksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RemoveCatFromLandmarksL( CStifItemParser& aItem );
        
        /**
        * CategoryIteratorL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CategoryIteratorL( CStifItemParser& aItem );
        
        /**
        * ReferencedIteratorL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ReferencedIteratorL( CStifItemParser& aItem );
        
        /**
        * ReadCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ReadCategoryL( CStifItemParser& aItem );
        
        /**
        * GetCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetCategoryL( CStifItemParser& aItem );
        /**
        * SetAndGetPlaceId test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetPlaceId( CStifItemParser& aItem );

        /**
        * SetAndGetTimeStamp test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetTimeStamp( CStifItemParser& aItem );

		/**
        * RemoveCategoriesL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RemoveCategoriesL( CStifItemParser& aItem );
        
        /**
        * GetGlobalCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetGlobalCategoryL( CStifItemParser& aItem );

				/**
        * GetGlobalCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GlobalCategoryNameL( CStifItemParser& aItem );
        
        /**
        * GetGlobalCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ResetGlobalCategoriesL( CStifItemParser& aItem );
        
        /**
        * NumOfItemsL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NumOfItemsL( CStifItemParser& aItem );
        
        /**
        * NextItemL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NextItemL( CStifItemParser& aItem );
        
        /**
        * GetItemIdsL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetItemIdsL( CStifItemParser& aItem );
        
        /**
        * ResetIteratorL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ResetIteratorL( CStifItemParser& aItem );
        
        /**
        * OpenDefaultDatabaseL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt OpenDefaultDatabaseL( CStifItemParser& aItem );
        
        /**
        * OpenDatabaseL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt OpenDatabaseL( CStifItemParser& aItem );
        
        /**
        * IsInitializingNeededL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt IsInitializingNeededL( CStifItemParser& aItem );
        
        /**
        * InitializeL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt InitializeL( CStifItemParser& aItem );
        
        /**
        * GetDatabaseUriL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetDatabaseUriL( CStifItemParser& aItem );

		/**
        * GetDatabaseSizeL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetDatabaseSizeL( CStifItemParser& aItem );
		        
        /**
        * LandmarkIteratorL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt LandmarkIteratorL( CStifItemParser& aItem );
        
        /**
        * LmkIteratorSortPrefL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt LmkIteratorSortPrefL( CStifItemParser& aItem );
        
        /**
        * ReadLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ReadLandmarkL( CStifItemParser& aItem );
        
        /**
        * AddAndRemoveLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt AddAndRemoveLandmarkL( CStifItemParser& aItem );
        
        /**
        * UpdateLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt UpdateLandmarkL( CStifItemParser& aItem );

		/**
        * RemoveLandmarksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RemoveLandmarksL( CStifItemParser& aItem );

		/**
        * RemoveAllLandmarksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RemoveAllLandmarksL( CStifItemParser& aItem );

		/**
        * CompactL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CompactL( CStifItemParser& aItem );
        
        /**
        * SetAndGetPartialReadParamsL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetPartialReadParamsL( CStifItemParser& aItem );
        
        /**
        * ReadPartialLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ReadPartialLandmarkL( CStifItemParser& aItem );
        
        /**
        * PrepareAndTakePartialLmksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt PrepareAndTakePartialLmksL( CStifItemParser& aItem );

		/**
        * ExportLandmarksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ExportLandmarksL( CStifItemParser& aItem );
        
        /**
        * ImportLandmarksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ImportLandmarksL( CStifItemParser& aItem );
        
        /**
        * ImportSelectedLandmarksL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ImportSelectedLandmarksL( CStifItemParser& aItem );
        
        /**
        * ImportedLmkIteratorL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ImportedLmkIteratorL( CStifItemParser& aItem );

		/**
        * NotifyDBEventL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NotifyDatabaseEventL( CStifItemParser& aItem );
		
		/**
        * NotifyDBEventL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CancelNotifyDatabaseEventL( CStifItemParser& aItem );
        
        /**
        * OpenExtendedDatabaseL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt OpenExtendedDatabaseL( CStifItemParser& aItem );
        
        /**
        * ExtendedDbLandmarkCountL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ExtendedDbLandmarkCountL( CStifItemParser& aItem );
        
        /**
        * ExtendedDbCategoryCountL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ExtendedDbCategoryCountL( CStifItemParser& aItem );		        
        				
		/**
        * DestructOperationL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt DestructOperationL( CStifItemParser& aItem );
        
        /**
        * ExecuteL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ExecuteL( CStifItemParser& aItem );
        
        /**
        * ExecuteAndDeleteOperationL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ExecuteAndDeleteOperationL( CStifItemParser& aItem );
        
        /**
        * NextStepL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NextStepL( CStifItemParser& aItem );
        
        /**
        * CreateEncoderL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreateEncoderL( CStifItemParser& aItem );
        
        /**
        * SetOutputFileL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetOutputFileL( CStifItemParser& aItem );
        
        /**
        * SetOutputBufferL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetOutputBufferL( CStifItemParser& aItem );
        
        /**
        * AddCollectionDataL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt AddCollectionDataL( CStifItemParser& aItem );
        
        /**
        * AddLandmarkToEncodeL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt AddLandmarkToEncodeL( CStifItemParser& aItem );

		/**
        * AddCategoryForLatestLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt AddCategoryForLatestLandmarkL( CStifItemParser& aItem );
        
        /**
        * FinalizeEncodingL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt FinalizeEncodingL( CStifItemParser& aItem );
        
        /**
        * CreateParserL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreateParserL( CStifItemParser& aItem );
        
        /**
        * SetInputBufferL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetInputBufferL( CStifItemParser& aItem );
        
        /**
        * SetInputFileL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetInputFileL( CStifItemParser& aItem );
        
        /**
        * SetInputFileHandleL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetInputFileHandleL( CStifItemParser& aItem );
        
        /**
        * ParseContentL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt ParseContentL( CStifItemParser& aItem );

		/**
        * NumOfParsedContentL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt NumOfParsedContentL( CStifItemParser& aItem );
        
        /**
        * FirstAndNextCollDataIdL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt FirstAndNextCollDataIdL( CStifItemParser& aItem );
        
        /**
        * GetCollectionDataL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetCollectionDataL( CStifItemParser& aItem );
        
        /**
        * RetrieveParsedLmkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RetrieveParsedLmkL( CStifItemParser& aItem );
        
        /**
        * RetrieveCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt RetrieveCategoryL( CStifItemParser& aItem );
        
        /**
        * CreatePartialReadParamsL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreatePartialReadParamsL( CStifItemParser& aItem );
        
        /**
        * SetAndGetRequestedAttrL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetRequestedAttrL( CStifItemParser& aItem );
        
        /**
        * SetAndGetRequestedPosFieldL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetRequestedPosFieldL( CStifItemParser& aItem );
        
        /**
        * CreateSortPref test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt CreateSortPref( CStifItemParser& aItem );

		/**
        * GetSortOrder test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt GetSortOrder( CStifItemParser& aItem );

		/**
        * SetAndGetSortAttr test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt SetAndGetSortAttr( CStifItemParser& aItem );
        
        /**
        * PackLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt PackLandmarkL( CStifItemParser& aItem );
        
        /**
        * UnpackLandmarkL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt UnpackLandmarkL( CStifItemParser& aItem );
        
        /**
        * PackCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt PackCategoryL( CStifItemParser& aItem );
        
        /**
        * UnpackCategoryL test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */        
        virtual TInt UnpackCategoryL( CStifItemParser& aItem );
		        	        
        
    };

#endif      // TESTLANDMARKSAPI_H

// End of File
