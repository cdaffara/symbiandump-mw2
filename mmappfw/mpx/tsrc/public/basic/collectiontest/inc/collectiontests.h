/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MpxCollectionTest test module.
*
*/

#ifndef COLLECTIONTESTS_H
#define COLLECTIONTESTS_H

// INCLUDES
#include <mpxcollectionplugin.hrh>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxcollectionmessagedefs.h> //EMPXItemDeleted
#include "testmenuview.h"
#include "testcommonpluginuids.h"
#include "testcollectionplugintype.h"

// CONSTANTS
_LIT(KTestModuleHeaderL1,   "**************************************************");
_LIT(KTestModuleHeaderL2,   "** Test Module: %S");
_LIT(KTestModuleHeaderL3,   "**");

_LIT(KTestModuleFooterL1,   "**");
_LIT(KTestModuleFooterL2,   "** Test Module: %S Completed");
_LIT(KTestModuleFooterL3,   "**************************************************");


_LIT(KTestModuleCaseL1,     "--------------------------------------------------");
_LIT(KTestModuleCaseL2,     "-- Test case: [%d]%S");
_LIT(KTestModuleCaseL3,     "--");

_LIT(KTestModuleCaseLeave,      "-- Test case failed: left with error: %d");
_LIT(KTestModuleCheckptFail,    "-- Test case failed at check point: %d");
_LIT(KTestModuleTimeout,        "-- Test case TIMEOUT");
_LIT(KTestModuleUserCancel,     "-- Test case user CANCELLED");

_LIT(KTestModuleInitializeLeft, "-- Test Modules Initialization left with error: %d");
_LIT(KTestModuleUninitializeLeft,"-- Test Modules Uninitialization left with error: %d");

// CONSTANTS
// common text
const TUint32 KMusicCollectionUID = 0x101FFC3A;
const TUint32 KPodcastCollectionUID = 0x101FFC3C;
const TInt KColTestPluginIndex = 2;
const TInt KRootLvPluginNum = 3;

_LIT(KTextNotImplemented,   "Not Implemented");
_LIT(KTextUninstallPlugin,  "collectiontestplugin plugin need to be uninstalled");
_LIT(KTextImproperMsg,      "Improper TMPXCollectionMessage received");
_LIT(KTextPanicExpected,    "NOTE: PANICE EXPECTED - test case is marked as Failed and skipped");
_LIT(KTextConfusingBehaviour,"NOTE: CONFUSING BEHAVIOUR");
_LIT(KTextManualCheck,      "NOTE: REQUIRE MANUAL CHECK");
_LIT(KTextApiDeprecated,    "NOTE: API IS DEPRECATED");

_LIT(KTextHandleColMsg,     "Event: %d, Type: %d, Data: %d");
_LIT(KTextHandleOpenLArg,   "Index: %d, Complete: %d, Error: %d");

_LIT(KTextPathChangedByOpen,"TMPXCollectionMessage: TMPXCollectionMessage::EPathChanged, EMcPathChangedByOpen");
_LIT(KTextCollectionChange, "TMPXCollectionMessage: TMPXCollectionMessage::ECollectionChanged");
_LIT(KTextFocusChanged,     "TMPXCollectionMessage: TMPXCollectionMessage::EFocusChanged");
_LIT(KTextPluginImpUid,     "Plugin implementation Id=0x%x");

// API names
_LIT(KTextApiNewL,          "Testing MMPXCollectionUtility::NewL");
_LIT(KTextApiCollectionIDL, "Testing MMPXCollectionUtility::CollectionIDL");
_LIT(KTextApiOpenL,         "Testing MMPXCollection::OpenL");
_LIT(KTextApiSetFilterL,    "Testing MMPXCollection::SetFilterL/FilterL");
_LIT(KTextApiUidL,          "Testing MMPXCollection::UidL");
_LIT(KTextApiPathL,         "Testing MMPXCollection::PathL");
_LIT(KTextApiBackL,         "Testing MMPXCollection::BackL");
_LIT(KTextApiIsRemote,      "Testing MMPXCollection::IsRemote");
_LIT(KTextApiCancelRequest, "Testing MMPXCollection::CancelRequest");
_LIT(KTextApiAddL,          "Testing MMPXCollection::AddL");
_LIT(KTextApiRemoveL,       "Testing MMPXCollection::RemoveL");
_LIT(KTextApiSetSyncL,      "Testing MMPXCollection::SetSyncL");
_LIT(KTextApiSetL,          "Testing MMPXCollection::SetL");
_LIT(KTextApiFindAllL_Asyn, "Testing MMPXCollection::FindAllL_Asyn");
_LIT(KTextApiFindAllL_Syn,  "Testing MMPXCollection::FindAllL_Syn");
_LIT(KTextApiMediaL,        "Testing MMPXCollection::MediaL");
_LIT(KTextApiCommandL,      "Testing MMPXCollection::CommandL");
_LIT(KTextApiCommandL_Ext,  "Testing MMPXCollection::CommandL_Ext");
_LIT(KTextApiGetSupportedTypesL,"Testing MMPXCollection::GetSupportedTypesL");
_LIT(KTextApiGetSupportedCapabilitiesL,"Testing MMPXCollection::GetSupportedCapabilitiesL");
_LIT(KTextApiCollectionIDL_Col,"Testing MMPXCollection::CollectionIDL");
_LIT(KTextApiNotifyL,       "Testing MMPXCollection::NotifyL");


_LIT(KTextHandleColMsgL1,   "MMPXCollectionObserver::HandleCollectionMessageL-TMPXCollectionMessage");
_LIT(KTextHandleColMsgL2,   "MMPXCollectionObserver::HandleCollectionMessageL-CMPXMessage");
_LIT(KTextHandleOpenL1,     "MMPXCollectionObserver::HandleOpenL-CMPXMedia");
_LIT(KTextHandleOpenL2,     "MMPXCollectionObserver::HandleOpenL-CMPXCollectionPlaylist");
_LIT(KTextHandleCommandComplete,"MMPXCollectionObserver::HandleCommandComplete");
_LIT(KTextHandleMediaL,     "MMPXCollectionObserver::HandleCollectionMediaL");
_LIT(KTextHandleRemoveL,    "MMPXCollectionRemoveObserver::HandleRemoveL");
_LIT(KTextHandleFindAllL,   "MMPXCollectionFindObserver::HandleFindAllL");

// MACROS

// FORWARD DECLARATIONS
class MMPXCollectionUtility;
class CStifLogger;

// DATA TYPES
_LIT(KTxtCollectionTests, "CollectionTests");

// CLASS DECLARATION

class CCollectionTests : public CTestMenuView,
                         public MMPXCollectionObserver,
                         public MMPXCollectionRemoveObserver,
                         public MMPXCollectionFindObserver
    {
    public: 
    
        /**
        * Two-phased constructor
        * @return object of CConsoleMain
        */
        static CCollectionTests* NewL(CConsoleMain* aConsoleMain,
                                      CTestBaseView* aParent,
                                      const TDesC& aName,
                                      CStifLogger* aLogger,
                                      TInt testIndex);
                                                                          
        /**
        * Two-phased constructor
        * @return object of CConsoleMain
        */
        static CCollectionTests* NewLC(CConsoleMain* aConsoleMain,
                                       CTestBaseView* aParent,
                                       const TDesC& aName,
                                       CStifLogger* aLogger,
                                       TInt testIndex);
   
        /**
        * Destructor of CConsoleMain
        */
        virtual ~CCollectionTests();
        
    private:
    
        /** 
        * C++ default constructor.
        */
        CCollectionTests(CConsoleMain* aConsoleMain,
                         CTestBaseView* aParent,
                         const TDesC& aName,
                         CStifLogger* aLogger,
                         TInt testIndex);

        /**
        * the second phase constructor ConstructL to safely construct things 
        * that can leave 
        */
        void ConstructL();
    
    public:
    
    	/*
    	* Return whether test requires scheduler for aysynchronous wait.
    	*/
    	TBool IsTestSynchronous(int iTestIndex);
	
		/**
 		* Result from latest test
 		*/
		TInt GetLatestTestResult();
		
		/**
		* Stop the current test
		*/
		void TestCompleteL();		
		
        /**
        * From CTestBaseView
        * Handle number key press
        */
        void HandleNumKeyL();
        
        /**
        * From CTestBaseView
        * Handle left key press
        */
        void HandleLeftKeyL();
        
        /**
        * From CTestBaseView
        * Handle right/enter key press
        */
        void HandleRightKeyL();
                        	
        /**
        * From CTestBaseView
        * Cleanup the view before deactivate/destroy view
        */
        void CleanupViewL();
        
        /**
        * From CTestBaseView
        * Initialize the view before display view
        */
        void InitializeViewL();
                    	
    	void RunNext();
    	
        void StartNextTestL();        
    	
        TInt StartTest0001L();
        TInt StartTest0002L();
        TInt StartTest0003L();
        TInt StartTest0004L();
        TInt StartTest0005L();
        TInt StartTest0006L();
        TInt StartTest0007L();
        TInt StartTest0008L();
        TInt StartTest0009L();
        TInt StartTest0010L();
        TInt StartTest0011L();
        TInt StartTest0012L();
        TInt StartTest0013L();
        TInt StartTest0014L();
        TInt StartTest0015L();
        TInt StartTest0016L();
        TInt StartTest0017L();
        TInt StartTest0018L();
        TInt StartTest0019L();
        TInt StartTest0020L();
        TInt StartTest0021L();
        TInt StartTest0022L();
        TInt StartTest0023L();
        TInt StartTest0024L();
        TInt StartTest0025L();
        TInt StartTest0026L();
        TInt StartTest0027L();
        TInt StartTest0028L();
        TInt StartTest0029L();
        TInt StartTest0030L();
        TInt StartTest0031L();
        TInt StartTest0032L();
        TInt StartTest0033L();
        TInt StartTest0034L();
        TInt StartTest0035L();
        TInt StartTest0036L();
        TInt StartTest0037L();
        TInt StartTest0038L();
        TInt StartTest0039L();
        TInt StartTest0040L();
        TInt StartTest0041L();
        TInt StartTest0042L();
        TInt StartTest0043L();
        TInt StartTest0044L();
        TInt StartTest0045L();
        TInt StartTest0046L();
        TInt StartTest0047L();
        TInt StartTest0048L();
        TInt StartTest0049L();
        TInt StartTest0050L();
        TInt StartTest0051L();
        TInt StartTest0052L();
        TInt StartTest0053L();
        TInt StartTest0054L();
        TInt StartTest0055L();
        TInt StartTest0056L();
        TInt StartTest0057L();
        TInt StartTest0058L();
        TInt StartTest0059L();
        TInt StartTest0060L();
        TInt StartTest0061L();
        TInt StartTest0062L();
        TInt StartTest0063L();
        TInt StartTest0064L();
        TInt StartTest0065L();
        TInt StartTest0066L();
        TInt StartTest0067L();
        TInt StartTest0068L();
        TInt StartTest0069L();                
        TInt StartTest0070L();
        TInt StartTest0071L();
        TInt StartTest0072L();
        TInt StartTest0073L();
        TInt StartTest0074L();
        TInt StartTest0075L();
        TInt StartTest0076L();
        TInt StartTest0077L();
        TInt StartTest0078L();
        TInt StartTest0079L();
        TInt StartTest0080L();
        TInt StartTest0081L();
        TInt StartTest0082L();
        TInt StartTest0083L();
        TInt StartTest0084L();
        TInt StartTest0085L();
        TInt StartTest0086L();
        TInt StartTest0087L();
        TInt StartTest0088L();
        TInt StartTest0089L();
        TInt StartTest0090L();
        TInt StartTest0091L();
        TInt StartTest0092L();
        TInt StartTest0093L();
        TInt StartTest0094L();
        TInt StartTest0095L();
        TInt StartTest0096L();
        TInt StartTest0097L();
        TInt StartTest0098L();
        TInt StartTest0099L();
        TInt StartTest0100L();
        TInt StartTest0101L();
        TInt StartTest0102L();
        TInt StartTest0103L();
        TInt StartTest0104L();
        TInt StartTest0105L();
        TInt StartTest0106L();
        TInt StartTest0107L();
        TInt StartTest0108L();
        TInt StartTest0109L();
        TInt StartTest0110L();
        TInt StartTest0111L();
        TInt StartTest0112L();
        TInt StartTest0113L();
        TInt StartTest0114L();
        TInt StartTest0115L();
        TInt StartTest0116L();
        TInt StartTest0117L();
        TInt StartTest0118L();
        TInt StartTest0119L();
        TInt StartTest0120L();
        TInt StartTest0121L();
        TInt StartTest0122L();
        TInt StartTest0123L();
        TInt StartTest0124L();
        TInt StartTest0125L();
        TInt StartTest0126L();
        TInt StartTest0127L();
        TInt StartTest0128L();
        TInt StartTest0129L();
        TInt StartTest0130L();
        TInt StartTest0131L();
        TInt StartTest0132L();
        TInt StartTest0133L();
        TInt StartTest0134L();
        TInt StartTest0135L();
        TInt StartTest0136L();
        TInt StartTest0137L();
        TInt StartTest0138L();
        TInt StartTest0139L();
        TInt StartTest0140L();
        TInt StartTest0141L();
        TInt StartTest0142L();
        TInt StartTest0143L();
        TInt StartTest0144L();
        TInt StartTest0145L();
        TInt StartTest0146L();
        TInt StartTest0147L();
        TInt StartTest0148L();
        TInt StartTest0149L();
        TInt StartTest0150L();
        TInt StartTest0151L();
        TInt StartTest0152L();
        TInt StartTest0153L();
        TInt StartTest0154L();
        TInt StartTest0155L();
        TInt StartTest0156L();
        TInt StartTest0157L();
        TInt StartTest0158L();
        TInt StartTest0159L();
        TInt StartTest0160L();
        TInt StartTest0161L();
        TInt StartTest0162L();
        TInt StartTest0163L();
        TInt StartTest0164L();
        TInt StartTest0165L();
        TInt StartTest0166L();
        TInt StartTest0167L();
        TInt StartTest0168L();
        TInt StartTest0169L();
        TInt StartTest0170L();
        TInt StartTest0171L();
        TInt StartTest0172L();
        TInt StartTest0173L();
        TInt StartTest0174L();
        TInt StartTest0175L();
        TInt StartTest0176L();
        TInt StartTest0177L();
        TInt StartTest0178L();
        TInt StartTest0179L();
        TInt StartTest0180L();
        TInt StartTest0181L();
        TInt StartTest0182L();
        TInt StartTest0183L();
        TInt StartTest0184L();
        TInt StartTest0185L();
        TInt StartTest0186L();
        TInt StartTest0187L();
        TInt StartTest0188L();
        TInt StartTest0189L();
        TInt StartTest0190L();
        TInt StartTest0191L();
        TInt StartTest0192L();
        TInt StartTest0193L();
        TInt StartTest0194L();
        TInt StartTest0195L();
        TInt StartTest0196L();
        TInt StartTest0197L();
        TInt StartTest0198L();
        TInt StartTest0199L();
        TInt StartTest0200L();
        TInt StartTest0201L();
        TInt StartTest0202L();

  		void DoHandleCollectionTest0001L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0011L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0012L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0021L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0022L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0023L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0029L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0029L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0030L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0031L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0032L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0033L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0034L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0035L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0036L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0036L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0037L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0038L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0039L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0039L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0040L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0041L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0043L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0044L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0047L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0048L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0051L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0052L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0056L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0057L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0058L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0059L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0059L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0060L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0060L(const CMPXMedia& aEntries,
                                         TInt aIndex,TBool aComplete,TInt aError);
        void DoHandleCollectionTest0078L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0079L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0080L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0084L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0085L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0086L( const TMPXCollectionMessage& aMessage );
        void DoHandleRemoveLTest0088L(MDesCArray& aUriArray, TInt aError);
        void DoHandleRemoveLTest0089L(MDesCArray& aUriArray, TInt aError);
        void DoHandleRemoveLTest0090L(MDesCArray& aUriArray, TInt aError);
        void DoHandleRemoveLTest0091L(MDesCArray& aUriArray, TInt aError);
        void DoHandleCollectionTest0091L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0099L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0100L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0108L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0109L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0117L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0118L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0119L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0120L( const TMPXCollectionMessage& aMessage );
        void DoHandleFindAllTest0126L(const CMPXMedia& aResults, TBool aComplete,TInt aError);
        void DoHandleCollectionTest0126L( const TMPXCollectionMessage& aMessage );
        void DoHandleFindAllTest0127L(const CMPXMedia& aResults, TBool aComplete,TInt aError);
        void DoHandleCollectionTest0127L( const TMPXCollectionMessage& aMessage );
        void DoHandleFindAllTest0128L(const CMPXMedia& aResults, TBool aComplete,TInt aError);
        void DoHandleFindAllTest0129L(const CMPXMedia& aResults, TBool aComplete,TInt aError);
        void DoHandleFindAllTest0130L(const CMPXMedia& aResults, TBool aComplete,TInt aError);
        void DoHandleCollectionTest0130L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0145L( const TMPXCollectionMessage& aMessage );
        void DoHandleMediaTest0145L(const CMPXMedia& aMedia, TInt aError);
        void DoHandleMediaTest0146L(const CMPXMedia& aMedia, TInt aError);
        void DoHandleMediaTest0147L(const CMPXMedia& aMedia, TInt aError);
        void DoHandleCollectionTest0148L( const TMPXCollectionMessage& aMessage );
        void DoHandleMediaTest0148L(const CMPXMedia& aMedia, TInt aError);
        void DoHandleMediaTest0149L(const CMPXMedia& aMedia, TInt aError);
        void DoHandleCollectionTest0162L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0163L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0171L( const TMPXCollectionMessage& aMessage );
        void DoHandleCommandCompleteTest0171L(CMPXCommand* aCommandResult, TInt aError);
        void DoHandleCommandCompleteTest0173L(CMPXCommand* aCommandResult, TInt aError);
        void DoHandleCollectionTest0173L( const TMPXCollectionMessage& aMessage );
        void DoHandleCommandCompleteTest0176L(CMPXCommand* aCommandResult, TInt aError);
        void DoHandleCommandCompleteTest0177L(CMPXCommand* aCommandResult, TInt aError);
        void DoHandleCollectionTest0177L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0179L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0182L( const TMPXCollectionMessage& aMessage );
        void DoHandleCommandCompleteTest0184L(CMPXCommand* aCommandResult, TInt aError);
        void DoHandleCollectionTest0194L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0195L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0196L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0197L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0198L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0199L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0200L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0201L( const TMPXCollectionMessage& aMessage );
        void DoHandleCollectionTest0202L( const TMPXCollectionMessage& aMessage );
         
		//***********************************************************************    
      
        /**
        *  From MMPXCollectionObserver
        *  Handle extended media properties
        *  @param aMedia media 
        *  @param aError error code    
        */
        void HandleCollectionMediaL(
                        const CMPXMedia& aMedia, 
                        TInt aError);
            
        /**
        *  From MMPXCollectionObserver
        *  Handle collection message
        *  @param aMessage collection message
        */
        void HandleCollectionMessageL(const CMPXMessage& aMsg);

        /**
        *  From MMPXCollectionObserver
        *  Handles the collection entries being opened. Typically called
        *  when client has Open()'d a folder
        *
        *  @param aEntries collection entries opened
        *  @param aIndex focused entry
        *  @param aComplete ETrue no more entries. EFalse more entries
        *                   expected
        *  @param aError error code   
        */
        void HandleOpenL(const CMPXMedia& aEntries,
                                 TInt aIndex,TBool aComplete,TInt aError);
        
        /**
        *  From MMPXCollectionObserver
        *  Handles the item being opened. Typically called
        *  when client has Open()'d an item. Client typically responds by
        *  'playing' the item via the playlist
        *
        *  @param aPlaylist collection playlist, owner ship is transfered    
        *  @param aError error code   
        */                         
        void HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError);
        
                		    
        void HandleCommandComplete(CMPXCommand* aCommandResult, TInt aError);
        
		//***********************************************************************
    
    	/**
    	*  From MMPXCollectionRemoveObserver
    	*  Handles removing a collection path
    	*  calls back with the list of file paths that were deleted
    	*  This callback is only applicable for local file storage plugins.
    	*
    	*  @param aUriArray, list of files that were deleted
    	*/
    	void HandleRemoveL(MDesCArray& aUriArray, TInt aError);

		//***********************************************************************

	    /**
	    *  From MMPXCollectionFindObserver
    	*  Handle callback for "find" operation
    	*  @param aEntries, CMPXMedia to be returned
    	*  @param aComplete ETrue no more entries. EFalse more entries
    	*                   expected
    	*  @param aError error code   
    	*/
    	void HandleFindAllL(const CMPXMedia& aResults, TBool aComplete,TInt aError);
         
	private:
	
	   	// Assert function
    	void AssertL(TBool aCondition, TInt aCheckPt);
    	
    	// Compare function
    	static TBool CompareByTMPXItemId(const TMPXItemId& aFirst, 
                                         const TMPXItemId& aSecond); 
                  
        // Initialize new collection                      
		void Collection1_Initialize();
                                         
    	// Initialize new collection and open with TMPXOpenMode (Test 9 - )
    	void Collection2_InitializeAndOpenL(TMPXOpenMode aOpenMode);
    	
    private:
    
        MMPXCollectionUtility*  iCollectionUtility1;  	//owns
        MMPXCollectionUtility*  iCollectionUtility2;  	//owns
        CStifLogger*            iLogger;       			//uses       
        TInt					iTestIndex;  
       	TInt 					iStopAtTestIndex;  
        TInt                    iTempCnt;    
        TInt					iLatestTestResult;
    };

#endif // COLLECTIONTESTS_H

