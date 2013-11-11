/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/


#ifndef CONTENTLISTINGFRAMEWORKTEST_H
#define CONTENTLISTINGFRAMEWORKTEST_H

//  INCLUDES
#include <bautils.h>
#include <barsc.h>
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <CLFContentListing.hrh>
#include <CLFContentListing.h>

// Logging path
_LIT( KContentListingFrameworkTestLogPath, "\\logs\\testframework\\ContentListingFrameworkTest\\" ); 
// Log file
_LIT( KContentListingFrameworkTestLogFile, "ContentListingFrameworkTest.txt" ); 
_LIT( KContentListingFrameworkTestLogFileWithTitle, "ContentListingFrameworkTest_[%S].txt" );

// FORWARD DECLARATIONS
class MCLFContentListingEngine;
class MCLFItemListModel;
class MCLFSortingStyle;
class TTestOperationObserver;
class TTestCustomSorter;
class TTestCustomGrouper;
class TTestPostFilter;
class TTestChangedItemObserver;
class MCLFModifiableItem;
class TTestCLFProcessObserver;
class MCLFItem;

// CLASS DECLARATION
NONSHARABLE_CLASS(CContentListingFrameworkTest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CContentListingFrameworkTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CContentListingFrameworkTest();

    public: // Functions from base classes

        virtual TInt RunMethodL( CStifItemParser& aItem );
        
    private:

        /**
        * C++ default constructor.
        */
        CContentListingFrameworkTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();
        
        /**
        * Test methods are listed below. 
        */
        // setups
        virtual TInt BaseSetupL( CStifItemParser& aItem );
        virtual TInt SortingStyleResourceSetupL( CStifItemParser& aItem );
        virtual TInt CreateModelSetupL( CStifItemParser& aItem );
        virtual TInt CreateModelFromResourceSetupL( CStifItemParser& aItem );
        virtual TInt ListModelSetupL( CStifItemParser& aItem );
        virtual TInt ListModelSetupFromResourceL( CStifItemParser& aItem );
        virtual TInt ListModelAllFileItemsSetupL( CStifItemParser& aItem );
        virtual TInt EngineTestSetupL( CStifItemParser& aItem );
        virtual TInt SortingStyleTestSetupL( CStifItemParser& aItem );
        virtual TInt SortingStyleResourceTestSetupL( CStifItemParser& aItem );
        virtual TInt ModifiableItemTestSetupL( CStifItemParser& aItem );
        virtual TInt ItemTestSetupL( CStifItemParser& aItem );
        virtual TInt MultibleSortingSetupL( CStifItemParser& aItem );
        virtual TInt MultibleSortingResourceSetupL( CStifItemParser& aItem );

        // teardowns
        virtual TInt Teardown( CStifItemParser& aItem );

        // tests
        // Constructor test
        virtual TInt CreateEngineTestL( CStifItemParser& aItem );
        virtual TInt CreateModifiableItemTestL( CStifItemParser& aItem );
        virtual TInt CreateSortignStyleTestL( CStifItemParser& aItem );
        virtual TInt CreateSortignStyleFromResourceTestL( CStifItemParser& aItem );
        virtual TInt CreateListModelTestL( CStifItemParser& aItem );
        virtual TInt CreateListModelFromResourceTestL( CStifItemParser& aItem );

        // Engine test
        virtual TInt UpdateItemsTestL( CStifItemParser& aItem );
        virtual TInt UpdateItemsWithIdTestL( CStifItemParser& aItem );
        virtual TInt UpdateItemsWithOpaqueDataFolderTestL( CStifItemParser& aItem );

        // Sorting Style test;
        virtual TInt SortingStyleResourceTestL( CStifItemParser& aItem );
        virtual TInt SortingStyleOrderingTestL( CStifItemParser& aItem );
        virtual TInt SortingStyleDataTypeTestL( CStifItemParser& aItem );
        virtual TInt SortingStyleUndefinedItemPositionTestL( CStifItemParser& aItem );
        virtual TInt SortingStyleFieldTestL( CStifItemParser& aItem );

        // List model test
        virtual TInt RefreshTestL( CStifItemParser& aItem );
        virtual TInt SetSortingStyleTestL( CStifItemParser& aItem );
        virtual TInt SetCustomSorterTestL( CStifItemParser& aItem );
        virtual TInt GroupingTestL( CStifItemParser& aItem );
        virtual TInt SetPostFilterTestL( CStifItemParser& aItem );
        virtual TInt SetWantedMimeTypesTestL( CStifItemParser& aItem );
        virtual TInt SetWantedMediaTypesTestL( CStifItemParser& aItem );
        virtual TInt SetWantedMediaAndMimeTypesTestL( CStifItemParser& aItem );
        virtual TInt MultibleSortingTestL( CStifItemParser& aItem );
        virtual TInt ModelItemsChangedTestL( CStifItemParser& aItem );

        // item test
        virtual TInt ItemFieldTestL( CStifItemParser& aItem );

        // Modifiable item test
        virtual TInt MIFieldTestL( CStifItemParser& aItem );
        
    private:    // Assistance methods
        void ResourceL( TInt aResourceId );
        void SortingStyleResourceL();
        void ListModelResourceL();
        void CreateNewFileL( TInt aNumber, TDes& aFileName );
        TCLFItemId FindTestFileIdL( TInt aNumber );
        TBool CheckFileNameShortingL();
        TBool CheckFileSizeShortingL();
        TBool CheckFileDateShortingL();
        TBool CheckFileTypesL( const MDesCArray& aMimeTypeArray, const TArray<TInt>& aMediaTypes );
        TBool CheckMimeTypesL( const MDesCArray& aMimeTypeArray, const TDesC& aMimeType );
        TBool CheckMediaTypesL( const TArray<TInt>& aMediaTypes, TCLFMediaType aMediaType );
        HBufC8* MakeOpaqueDataL( const MDesCArray& aFiles );
        void MakeMultibleSortingItemsL( RPointerArray<MCLFModifiableItem>& aItemArray );
        TBool CheckMultibleSortingShortingL();
        TBool CheckMultibleSortingShorting2L();
        const MCLFItem* FindItem( MCLFItemListModel& aModel, TCLFItemId aItemId );
        
    private:    // Data
        MCLFContentListingEngine* iEngine;
        MCLFItemListModel* iListModel;
        MCLFSortingStyle* iSortingStyle;
        MCLFSortingStyle* iSortingStyle1;
        MCLFSortingStyle* iSortingStyle2;
        MCLFSortingStyle* iSortingStyle3;
        MCLFModifiableItem* iModifiableItem;
        const MCLFItem* iItem; // ref. not owned

        TTestOperationObserver* iTestObserver;
        TTestCustomSorter* iTestSorter;
        TTestCustomSorter* iTestSorter1;
        TTestCustomGrouper* iTestGrouper;
        TTestCustomGrouper* iTestGrouper1;
        TTestPostFilter* iTestFilter;
        TTestPostFilter* iTestFilter1;
        TTestCLFProcessObserver* iTestCLFProcessObserver;
        TTestCLFProcessObserver* iTestCLFProcessObserver1;

        CDesCArray* iMimeTypeArray;
        CDesCArray* iMimeTypeArray1;
        RArray<TInt> iMediaTypeArray;
        RArray<TInt> iMediaTypeArray1;

        TTestChangedItemObserver* iChangedItemObserver;
        TTestChangedItemObserver* iChangedItemObserver1;
        RArray<TCLFItemId> iUpdateItemIdArray;
        HBufC8* iOpaqueData;
        TInt iSemanticId;

        RFs iFs;
        RResourceFile iResourceFile;
        HBufC8* iDataBuffer;
        TResourceReader iResourceReader;
        CActiveSchedulerWait iWait;
        RArray<TCLFItemId> iChangedArray;
        TInt iItemCount;
        RPointerArray<MCLFModifiableItem> iModifiableItems;
        TFileName iFileName;
    };

#endif      // CONTENTLISTINGFRAMEWORKTEST_H

// End of File
