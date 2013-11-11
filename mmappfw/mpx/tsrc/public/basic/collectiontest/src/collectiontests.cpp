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

// INCLUDE FILES
#include <mpxcollectionutility.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionplaylist.h>
#include <mpxplaybackframeworkdefs.h>
#include <mpxcollectionmessage.h>
#include <mpxcollectionframeworkdefs.h>
#include <mpxmessagegeneraldefs.h> // KMPXMessageGeneralEvent, KMPXMessageGeneralType
#include <mpxmediageneraldefs.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxattribute.h>
#include <stiflogger.h>
#include "testutility.h"
#include "testplaybackview.h"
#include "pathoptionmenu.h" 
#include "collectiontests.h"
#include "testpanic.h"

// MACROS

#define DELETE_PTR( PTR_VAR )\
	if( PTR_VAR )\
		{\
		delete PTR_VAR;\
		PTR_VAR = NULL;\
		}
		
#define DELETE_COLLECTION_PTR( PTR_VAR )\
	if( PTR_VAR )\
		{\
		PTR_VAR->Close();\
		PTR_VAR = NULL;\
		}

// return on failure
#define TMC_ASSERT( _TMCcode, _TMCcheckpt )\
	{\
	AssertL( _TMCcode, _TMCcheckpt );\
	if( !(_TMCcode ) )\
		{\
		return;\
		}\
	}
			
			
// DATA TYPES

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCollectionTests::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCollectionTests* CCollectionTests::NewL(CConsoleMain* aConsoleMain,
                                    	 CTestBaseView* aParent,
                                   		 const TDesC& aName,
                                   	     CStifLogger* aLogger,
                                   	     TInt testIndex)
    {
    CCollectionTests* self = CCollectionTests::NewLC(aConsoleMain,
                                                     aParent,
                                                   	 aName,
                                                     aLogger,
                                                     testIndex);
    CleanupStack::PopAndDestroy(self);

    return self;
    }
    
// -----------------------------------------------------------------------------
// CCollectionTests::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCollectionTests* CCollectionTests::NewLC(CConsoleMain* aConsoleMain,
                                    	  CTestBaseView* aParent,
                                   		  const TDesC& aName,
                                   	      CStifLogger* aLogger,
                                   	      TInt testIndex)
    {
    CCollectionTests* self = new (ELeave) CCollectionTests(aConsoleMain,
                                                           aParent,
                                                   	       aName,
                                                           aLogger,
                                                           testIndex);
    
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }
    
// -----------------------------------------------------------------------------
// C++ default constructor.
// -----------------------------------------------------------------------------
CCollectionTests::CCollectionTests(CConsoleMain* aConsoleMain,
                                   CTestBaseView* aParent,
                                   const TDesC& aName,
                                   CStifLogger* aLogger,
                                   TInt testIndex)
    : CTestMenuView(aConsoleMain, aParent, aName),
      iCollectionUtility1(NULL), 
      iCollectionUtility2(NULL),
      iLogger(aLogger), 
      iTestIndex(testIndex), 
      iStopAtTestIndex(testIndex), 
      iTempCnt(0),
      iLatestTestResult(KErrNone)
    {    
    // set stop index for groups
    switch(iTestIndex)
    	{
    	case  21: iStopAtTestIndex =  35; break;
    	case  36: iStopAtTestIndex =  41; break;
    	case  42: iStopAtTestIndex =  53; break;
    	case  54: iStopAtTestIndex =  60; break;
    	case  70: iStopAtTestIndex =  81; break;
    	case  82: iStopAtTestIndex = 100; break;
    	case 101: iStopAtTestIndex = 109; break;
    	case 110: iStopAtTestIndex = 120; break;
    	case 121: iStopAtTestIndex = 140; break;
    	case 141: iStopAtTestIndex = 149; break;
    	case 150: iStopAtTestIndex = 169; break;
    	case 170: iStopAtTestIndex = 184; break;
    	case 185: iStopAtTestIndex = 189; break;
    	case 194: iStopAtTestIndex = 202; break;
    	}
    }

// -----------------------------------------------------------------------------
// Second phase constructor
// -----------------------------------------------------------------------------
void CCollectionTests::ConstructL()
    {
    CTestMenuView::ConstructL();
    }
  
// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CCollectionTests::~CCollectionTests()
    {
    iLogger->Log(_L("+CCollectionTests::~CCollectionTests"));  
    
    DELETE_COLLECTION_PTR(iCollectionUtility1);    	
    DELETE_COLLECTION_PTR(iCollectionUtility2);	    

    iLogger->Log(_L("-CCollectionTests::~CCollectionTests"));
    }
           
// -----------------------------------------------------------------------------
// Return whether test requires scheduler for aysynchronous wait.
// -----------------------------------------------------------------------------

TBool CCollectionTests::IsTestSynchronous(int testIndex)
	{
	// some cases do not use callbacks; so, no need to use scheduler
	return (testIndex == 20 ) || (testIndex >= 190 && testIndex <= 193);
	}
	
// -----------------------------------------------------------------------------
// Result from latest test
// -----------------------------------------------------------------------------    	
TInt CCollectionTests::GetLatestTestResult()
    {
    return iLatestTestResult;
	}
	
	
// -----------------------------------------------------------------------------
// Stop the current test
// -----------------------------------------------------------------------------
void CCollectionTests::TestCompleteL()
	{
	CurrentViewDoneL();
	}
	
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle number key press
// -----------------------------------------------------------------------------
void CCollectionTests::HandleNumKeyL()
    {            
    }
    
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle left key press
// -----------------------------------------------------------------------------
void CCollectionTests::HandleLeftKeyL()
    {
    iLogger->Log(_L("+CCollectionTests::HandleLeftKeyL"));

    iLogger->Log(_L("-CCollectionTests::HandleLeftKeyL"));
    }
        
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle right/enter key press
// -----------------------------------------------------------------------------
void CCollectionTests::HandleRightKeyL()
    {
    iLogger->Log(_L("+CCollectionTests::HandleRightKeyL"));

    iLogger->Log(_L("-CCollectionTests::HandleRightKeyL"));
    }   

// -----------------------------------------------------------------------------
// From CTestBaseView
// Cleanup the view before deactivate/destroy view
// -----------------------------------------------------------------------------
void CCollectionTests::CleanupViewL()
    {  
    }
     
// -----------------------------------------------------------------------------
// From CTestBaseView
// Initialize the view before display view
// -----------------------------------------------------------------------------
void CCollectionTests::InitializeViewL()
    {
    Collection1_Initialize();
    }          
           
void CCollectionTests::RunNext()
	{        
	if(iStopAtTestIndex == iTestIndex)
		{
		TestCompleteL();
		return;
		}
		
	iTestIndex++;
 
    StartNextTestL();
	}
    	    
void CCollectionTests::StartNextTestL()
{	
    switch(iTestIndex)
    	{
    	case   1: StartTest0001L(); break;
    	case   2: StartTest0002L(); break;
    	case   3: StartTest0003L(); break;
    	case   4: StartTest0004L(); break;
    	case   5: StartTest0005L(); break;
    	case   6: StartTest0006L(); break;
    	case   7: StartTest0007L(); break;
    	case   8: StartTest0008L(); break;
    	case   9: StartTest0009L(); break;
    	case  10: StartTest0010L(); break;
    	case  11: StartTest0011L(); break;
    	case  12: StartTest0012L(); break;
    	case  13: StartTest0013L(); break;
    	case  14: StartTest0014L(); break;
    	case  15: StartTest0015L(); break;
    	case  16: StartTest0016L(); break;
    	case  17: StartTest0017L(); break;
    	case  18: StartTest0018L(); break;
    	case  19: StartTest0019L(); break;
    	case  20: StartTest0020L(); break;
    	case  21: StartTest0021L(); break;
    	case  22: StartTest0022L(); break;
    	case  23: StartTest0023L(); break;
    	case  24: StartTest0024L(); break;
    	case  25: StartTest0025L(); break;
    	case  26: StartTest0026L(); break;
    	case  27: StartTest0027L(); break;
    	case  28: StartTest0028L(); break;
    	case  29: StartTest0029L(); break;
    	case  30: StartTest0030L(); break;
    	case  31: StartTest0031L(); break;
    	case  32: StartTest0032L(); break;
    	case  33: StartTest0033L(); break;
    	case  34: StartTest0034L(); break;
    	case  35: StartTest0035L(); break;
    	case  36: StartTest0036L(); break;
    	case  37: StartTest0037L(); break;
    	case  38: StartTest0038L(); break;
    	case  39: StartTest0039L(); break;
    	case  40: StartTest0040L(); break;
    	case  41: StartTest0041L(); break;
    	case  42: StartTest0042L(); break;
    	case  43: StartTest0043L(); break;
    	case  44: StartTest0044L(); break;
    	case  45: StartTest0045L(); break;
    	case  46: StartTest0046L(); break;
    	case  47: StartTest0047L(); break;
    	case  48: StartTest0048L(); break;
    	case  49: StartTest0049L(); break;
    	case  50: StartTest0050L(); break;
    	case  51: StartTest0051L(); break;
    	case  52: StartTest0052L(); break;
    	case  53: StartTest0053L(); break;
    	case  54: StartTest0054L(); break;
    	case  55: StartTest0055L(); break;
    	case  56: StartTest0056L(); break;
    	case  57: StartTest0057L(); break;
    	case  58: StartTest0058L(); break;
    	case  59: StartTest0059L(); break;
    	case  60: StartTest0060L(); break;
    	case  61: StartTest0061L(); break;
    	case  62: StartTest0062L(); break;
    	case  63: StartTest0063L(); break;
    	case  64: StartTest0064L(); break;
    	case  65: StartTest0065L(); break;
    	case  66: StartTest0066L(); break;
    	case  67: StartTest0067L(); break;
    	case  68: StartTest0068L(); break;
    	case  69: StartTest0069L(); break;
    	case  70: StartTest0070L(); break;
    	case  71: StartTest0071L(); break;
    	case  72: StartTest0072L(); break;
    	case  73: StartTest0073L(); break;
    	case  74: StartTest0074L(); break;
    	case  75: StartTest0075L(); break;
    	case  76: StartTest0076L(); break;
    	case  77: StartTest0077L(); break;
    	case  78: StartTest0078L(); break;
    	case  79: StartTest0079L(); break;
    	case  80: StartTest0080L(); break;
    	case  81: StartTest0081L(); break;
    	case  82: StartTest0082L(); break;
    	case  83: StartTest0083L(); break;
    	case  84: StartTest0084L(); break;
    	case  85: StartTest0085L(); break;
    	case  86: StartTest0086L(); break;
    	case  87: StartTest0087L(); break;
    	case  88: StartTest0088L(); break;
    	case  89: StartTest0089L(); break;
    	case  90: StartTest0090L(); break;
    	case  91: StartTest0091L(); break;
    	case  92: StartTest0092L(); break;
    	case  93: StartTest0093L(); break;
    	case  94: StartTest0094L(); break;
    	case  95: StartTest0095L(); break;
    	case  96: StartTest0096L(); break;
    	case  97: StartTest0097L(); break;
    	case  98: StartTest0098L(); break;
    	case  99: StartTest0099L(); break;
    	case 100: StartTest0100L(); break;
    	case 101: StartTest0101L(); break;
    	case 102: StartTest0102L(); break;
    	case 103: StartTest0103L(); break;
    	case 104: StartTest0104L(); break;
    	case 105: StartTest0105L(); break;
    	case 106: StartTest0106L(); break;
    	case 107: StartTest0107L(); break;
    	case 108: StartTest0108L(); break;
    	case 109: StartTest0109L(); break;
    	case 110: StartTest0110L(); break;
    	case 111: StartTest0111L(); break;
    	case 112: StartTest0112L(); break;
    	case 113: StartTest0113L(); break;
    	case 114: StartTest0114L(); break;
    	case 115: StartTest0115L(); break;
    	case 116: StartTest0116L(); break;
    	case 117: StartTest0117L(); break;
    	case 118: StartTest0118L(); break;
    	case 119: StartTest0119L(); break;
    	case 120: StartTest0120L(); break;
    	case 121: StartTest0121L(); break;
    	case 122: StartTest0122L(); break;
    	case 123: StartTest0123L(); break;
    	case 124: StartTest0124L(); break;
    	case 125: StartTest0125L(); break;
    	case 126: StartTest0126L(); break;
    	case 127: StartTest0127L(); break;
    	case 128: StartTest0128L(); break;
    	case 129: StartTest0129L(); break;
    	case 130: StartTest0130L(); break;
    	case 131: StartTest0131L(); break;
    	case 132: StartTest0132L(); break;
    	case 133: StartTest0133L(); break;
    	case 134: StartTest0134L(); break;
    	case 135: StartTest0135L(); break;
    	case 136: StartTest0136L(); break;
    	case 137: StartTest0137L(); break;
    	case 138: StartTest0138L(); break;
    	case 139: StartTest0139L(); break;
    	case 140: StartTest0140L(); break;
    	case 141: StartTest0141L(); break;
    	case 142: StartTest0142L(); break;
    	case 143: StartTest0143L(); break;
    	case 144: StartTest0144L(); break;
    	case 145: StartTest0145L(); break;
    	case 146: StartTest0146L(); break;
    	case 147: StartTest0147L(); break;
    	case 148: StartTest0148L(); break;
    	case 149: StartTest0149L(); break;
    	case 150: StartTest0150L(); break;
    	case 151: StartTest0151L(); break;
    	case 152: StartTest0152L(); break;
    	case 153: StartTest0153L(); break;
    	case 154: StartTest0154L(); break;
    	case 155: StartTest0155L(); break;
    	case 156: StartTest0156L(); break;
    	case 157: StartTest0157L(); break;
    	case 158: StartTest0158L(); break;
    	case 159: StartTest0159L(); break;
    	case 160: StartTest0160L(); break;
    	case 161: StartTest0161L(); break;
    	case 162: StartTest0162L(); break;
    	case 163: StartTest0163L(); break;
    	case 164: StartTest0164L(); break;
    	case 165: StartTest0165L(); break;
    	case 166: StartTest0166L(); break;
    	case 167: StartTest0167L(); break;
    	case 168: StartTest0168L(); break;
    	case 169: StartTest0169L(); break;
    	case 170: StartTest0170L(); break;
    	case 171: StartTest0171L(); break;
    	case 172: StartTest0172L(); break;
    	case 173: StartTest0173L(); break;
    	case 174: StartTest0174L(); break;
    	case 175: StartTest0175L(); break;
    	case 176: StartTest0176L(); break;
    	case 177: StartTest0177L(); break;
    	case 178: StartTest0178L(); break;
    	case 179: StartTest0179L(); break;
    	case 180: StartTest0180L(); break;
    	case 181: StartTest0181L(); break;
    	case 182: StartTest0182L(); break;
    	case 183: StartTest0183L(); break;
    	case 184: StartTest0184L(); break;
    	case 185: StartTest0185L(); break;
    	case 186: StartTest0186L(); break;
    	case 187: StartTest0187L(); break;
    	case 188: StartTest0188L(); break;
    	case 189: StartTest0189L(); break;
    	case 190: StartTest0190L(); break;
    	case 191: StartTest0191L(); break;
    	case 192: StartTest0192L(); break;
    	case 193: StartTest0193L(); break;
    	case 194: StartTest0194L(); break;
    	case 195: StartTest0195L(); break;
    	case 196: StartTest0196L(); break;
    	case 197: StartTest0197L(); break;
    	case 198: StartTest0198L(); break;
    	case 199: StartTest0199L(); break;
    	case 200: StartTest0200L(); break;
    	case 201: StartTest0201L(); break;
    	case 202: StartTest0202L(); break;
    	}
    }
		
TInt CCollectionTests::StartTest0001L()
    {
    // Initialization
    iLogger->Log( KTextApiNewL );

    iCollectionUtility1->Collection().OpenL();
    return 0;
    }
    
TInt CCollectionTests::StartTest0002L()
    {
    // MMPXCollectionUtility::CollectionIDL not implemented
    iLogger->Log( KTextApiCollectionIDL );
    iLogger->Log( KTextNotImplemented );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0003L()
    {
    // MMPXCollectionUtility::CollectionIDL not implemented
    iLogger->Log( KTextApiCollectionIDL );
    iLogger->Log( KTextNotImplemented );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0004L()
    {
    // MMPXCollectionUtility::CollectionIDL not implemented
    iLogger->Log( KTextApiCollectionIDL );
    iLogger->Log( KTextNotImplemented );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0005L()
    {
    // MMPXCollectionUtility::CollectionIDL not implemented
    iLogger->Log( KTextApiCollectionIDL );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0006L()
    {
    // MMPXCollectionUtility::CollectionIDL not implemented
    iLogger->Log( KTextApiCollectionIDL );
    iLogger->Log( KTextNotImplemented );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0007L()
    {
    // MMPXCollectionUtility::CollectionIDL not implemented
    iLogger->Log( KTextApiCollectionIDL );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0008L()
    {
    // MMPXCollectionUtility::CollectionIDL not implemented
    iLogger->Log( KTextApiCollectionIDL );
    iLogger->Log( KTextNotImplemented );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0009L()
    {
    // MMPXCollection::OpenL
    iLogger->Log( KTextApiOpenL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0010L()
    {
    // MMPXCollection::OpenL
    iLogger->Log( KTextApiOpenL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0011L()
    {
    iLogger->Log( KTextApiOpenL );
    Collection2_InitializeAndOpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0012L()
    {
    // Included in Test0011
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0013L()
    {
    iLogger->Log( KTextApiOpenL );
    Collection2_InitializeAndOpenL( EMPXOpenGroupOrPlaylist );
    return 0;
    }
    
TInt CCollectionTests::StartTest0014L()
    {
    // Included in Test0013
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0015L()
    {
    iLogger->Log( KTextApiOpenL );
    Collection2_InitializeAndOpenL( EMPXOpenAllItems );
    return 0;
    }
    
TInt CCollectionTests::StartTest0016L()
    {
    // Included in Test0015
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0017L()
    {
    iLogger->Log( KTextApiOpenL );
    Collection2_InitializeAndOpenL( EMPXOpenNoPlaylist );
    return 0;
    }
    
TInt CCollectionTests::StartTest0018L()
    {
    // Included in Test0017
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0019L()
    {
    iLogger->Log( KTextApiOpenL );
    Collection2_InitializeAndOpenL( EMPXOpenPlaylistOnly );
    return 0;
    }
    
TInt CCollectionTests::StartTest0020L()
    {    
    // Included in Test0019
    //RunNext();
    
    // copy case 19 here
    iLogger->Log( KTextApiOpenL );
    Collection2_InitializeAndOpenL( EMPXOpenPlaylistOnly );
    
    // Cleanup up iCollectionUtility2
    DELETE_COLLECTION_PTR(iCollectionUtility2);

    return 0;
    }

TInt CCollectionTests::StartTest0021L()
    {
    iLogger->Log( KTextApiOpenL );
    
    
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault);
    return 0;
    }

TInt CCollectionTests::StartTest0022L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL(KColTestPluginIndex, EMPXOpenDefault);
    return 0;
    }

TInt CCollectionTests::StartTest0023L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0024L()
    {
    iLogger->Log( KTextApiOpenL );
    // No HandleMessage should be received, since No Path and No Collection plugin changed
    iCollectionUtility1->Collection().OpenL( EMPXOpenGroupOrPlaylist );
    return 0;
    }

TInt CCollectionTests::StartTest0025L()
    {
    iLogger->Log( KTextApiOpenL );
    // No HandleMessage should be received, since No Path and No Collection plugin changed
    iCollectionUtility1->Collection().OpenL( EMPXOpenAllItems );
    return 0;
    }
    
TInt CCollectionTests::StartTest0026L()
    {
    iLogger->Log( KTextApiOpenL );
    // No HandleMessage should be received, since No Path and No Collection plugin changed
    iCollectionUtility1->Collection().OpenL( EMPXOpenNoPlaylist );
    return 0;
    }

TInt CCollectionTests::StartTest0027L()
    {
    iLogger->Log( KTextApiOpenL );
    // No HandleMessage should be received, since No Path and No Collection plugin changed
    // TODO: SINCE TMPXOpenMode IS DECPRECATED, REMOVED THESE TEST SOONER OR LATER
    iCollectionUtility1->Collection().OpenL( EMPXOpenPlaylistOnly );
    return 0;
    }

TInt CCollectionTests::StartTest0028L()
    {
    iLogger->Log( KTextApiOpenL );
    // No HandleMessage should be received, since No Path and No Collection plugin changed
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0029L()
    {
    iLogger->Log( KTextApiOpenL );
    // Open first entry in CollectionTestPlugin root menu
    iCollectionUtility1->Collection().OpenL(0, EMPXOpenDefault);
    return 0;
    }

TInt CCollectionTests::StartTest0030L()
    {
    iLogger->Log( KTextApiOpenL );
    // Open the CollectionTestPlugin root menu through path
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    attrs.Append( KMPXMediaGeneralId ); //TODO: NOTE: attribute is not selected through path
    path->Set( attrs.Array() );
    
    iCollectionUtility1->Collection().OpenL(*path, EMPXOpenDefault);
    CleanupStack::PopAndDestroy(2, path);   // attrs, path
    return 0;
    }

TInt CCollectionTests::StartTest0031L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0032L()
    {
    iLogger->Log( KTextApiOpenL );
    // Open second entry in CollectionTestPlugin root menu
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    attrs.Append( KMPXMediaGeneralId );
    attrs.Append( KMPXMediaGeneralUri );
    
    iCollectionUtility1->Collection().OpenL(1, attrs.Array(), EMPXOpenDefault);
    CleanupStack::PopAndDestroy( &attrs );   // attrs
    return 0;
    }

TInt CCollectionTests::StartTest0033L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0034L()
    {
    iLogger->Log( KTextApiOpenL );
    // Open the CollectionTestPlugin container level13 through path
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 13 );
    
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    attrs.Append( KMPXMediaGeneralId );
    attrs.Append( KMPXMediaGeneralUri );
    
    iCollectionUtility1->Collection().OpenL(*path, attrs.Array(), EMPXOpenDefault);
    CleanupStack::PopAndDestroy(2, path);   // attrs, path
    return 0;
    }

TInt CCollectionTests::StartTest0035L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0036L()
    {
    iLogger->Log( KTextApiOpenL );
    RArray<TUid> uidArray;
    CleanupClosePushL( uidArray );
    uidArray.Append( TUid::Uid( ECollectionTestPluginType ) );
    iCollectionUtility1->Collection().OpenL(uidArray.Array(), EMPXOpenDefault);
    CleanupStack::PopAndDestroy( &uidArray );
    return 0;
    }

TInt CCollectionTests::StartTest0037L()
    {
    iLogger->Log( KTextApiOpenL );
    RArray<TUid> uidArray;
    CleanupClosePushL( uidArray );
    uidArray.Append( TUid::Uid( EMPXCollectionPluginMusic ) );
    uidArray.Append( TUid::Uid( EMPXCollectionPluginPodCast ) );
    iCollectionUtility1->Collection().OpenL(uidArray.Array(), EMPXOpenDefault);
    CleanupStack::PopAndDestroy( &uidArray );
    return 0;
    }

TInt CCollectionTests::StartTest0038L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0039L()
    {
    iLogger->Log( KTextApiOpenL );
    // No plugin Id should returned through HandleOpenL
    iCollectionUtility1->Collection().OpenL(TUid::Uid(EMPXCollectionPluginUnknown), EMPXOpenDefault);
    return 0;
    }

TInt CCollectionTests::StartTest0040L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL(TUid::Uid( ECollectionTestPluginType ), EMPXOpenDefault);
    return 0;
    }
    
TInt CCollectionTests::StartTest0041L()
    {
    iLogger->Log( KTextApiOpenL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0042L()
    {
    iLogger->Log( KTextApiSetFilterL );
    CMPXFilter* filter = CMPXFilter::NewL();
    CleanupStack::PushL( filter );
    
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    TMPXAttribute idAttr(KMPXMediaGeneralId);
    
    // Filter out Title="level211"
    filter->SetTextValueL(titleAttr, _L("level211"));
    // Filter out Id=213
    filter->SetTObjectValueL<TInt>(idAttr, 213);
    
    iCollectionUtility1->Collection().SetFilterL( filter );
    CleanupStack::PopAndDestroy( filter );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0043L()
    {
    iLogger->Log( KTextApiSetFilterL );
    // Open the CollectionTestPlugin container "level11"
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 11 );
    
    iCollectionUtility1->Collection().OpenL(*path, EMPXOpenDefault);
    CleanupStack::PopAndDestroy(path);   // path
    return 0;
    }

TInt CCollectionTests::StartTest0044L()
    {
    iLogger->Log( KTextApiSetFilterL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0045L()
    {
    iLogger->Log( KTextApiSetFilterL );
    CMPXFilter* filter = iCollectionUtility1->Collection().FilterL();
    CleanupStack::PushL( filter );
    TMC_ASSERT((filter != NULL), 1);
    
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    TMPXAttribute idAttr(KMPXMediaGeneralId);
    TMC_ASSERT(filter->IsSupported(titleAttr) && filter->IsSupported(titleAttr), 2);
    
    TInt filterId = *filter->Value<TInt>( idAttr );
    TMC_ASSERT(filterId == 213, 3);
    
    const TDesC& filterTitle = filter->ValueText( titleAttr );
    TMC_ASSERT(filterTitle == _L("level211"), 4);
    
    CleanupStack::PopAndDestroy( filter );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0046L()
    {
    iLogger->Log( KTextApiSetFilterL );
    CMPXFilter* filter = CMPXFilter::NewL();
    CleanupStack::PushL( filter );
    
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    // Filter out Uri="\song3311.ts"
    filter->SetTextValueL(uriAttr, _L("\\song3311.ts"));
    iCollectionUtility1->Collection().SetFilterL( filter );
    CleanupStack::PopAndDestroy( filter );
    RunNext();
    return 0;
    }
TInt CCollectionTests::StartTest0047L()
    {
    iLogger->Log( KTextApiSetFilterL );
    // Open the CollectionTestPlugin container "level231"
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 13 );
    path->AppendL( 231 );
    
    iCollectionUtility1->Collection().OpenL(*path, EMPXOpenDefault);
    CleanupStack::PopAndDestroy(path);   // path
    return 0;
    }

TInt CCollectionTests::StartTest0048L()
    {
    iLogger->Log( KTextApiSetFilterL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }

TInt CCollectionTests::StartTest0049L()
    {
    iLogger->Log( KTextApiSetFilterL );
    CMPXFilter* filter = iCollectionUtility1->Collection().FilterL();
    CleanupStack::PushL( filter );
    TMC_ASSERT(filter != NULL, 1);
    
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    TMC_ASSERT(filter->IsSupported(uriAttr), 2);
    
    const TDesC& filterUri = filter->ValueText( uriAttr );
    TMC_ASSERT(filterUri == _L("\\song3311.ts"), 3);
    
    CleanupStack::PopAndDestroy( filter );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0050L()
    {
    iLogger->Log( KTextApiSetFilterL );
    iCollectionUtility1->Collection().SetFilterL( NULL );
    CMPXFilter* filter = iCollectionUtility1->Collection().FilterL();
    TMC_ASSERT(filter == NULL, 1);
    
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0051L()
    {
    iLogger->Log( KTextApiSetFilterL );
    // Open the CollectionTestPlugin container level11 through path
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 11 );
    
    iCollectionUtility1->Collection().OpenL(*path, EMPXOpenDefault);
    CleanupStack::PopAndDestroy( path);   // path
    return 0;
    }

TInt CCollectionTests::StartTest0052L()
    {
    iLogger->Log( KTextApiSetFilterL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }
    
TInt CCollectionTests::StartTest0053L()
    {
    iLogger->Log( KTextApiUidL );
    TUid pluginUid;
    TRAPD(err, pluginUid = iCollectionUtility1->Collection().UidL() );
    TMC_ASSERT(err == KErrNone, 1);
    TMC_ASSERT(pluginUid == TUid::Uid(KCollectionTestPluginImpId), 2);
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0054L()
    {
    iLogger->Log( KTextApiUidL );
    
    // Close all the collection utility API
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this, KMcModeDefault);
    TUid pluginUid;
    TRAPD(err, pluginUid = iCollectionUtility1->Collection().UidL() );
    TMC_ASSERT(err == KErrNone, 1);
    TMC_ASSERT(pluginUid == KNullUid, 1);
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0055L()
    {
    iLogger->Log( KTextApiPathL );
    
    CMPXCollectionPath* path = iCollectionUtility1->Collection().PathL();
    CleanupStack::PushL( path );
    TMC_ASSERT(path->Levels() == 0, 1);
    CleanupStack::PopAndDestroy( path );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0056L()
    {
    iLogger->Log( KTextApiPathL );
    iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
    return 0;
    }    

TInt CCollectionTests::StartTest0057L()
    {
    iLogger->Log( KTextApiPathL );
    iCollectionUtility1->Collection().OpenL(KColTestPluginIndex, EMPXOpenDefault);
    return 0;
    }    

TInt CCollectionTests::StartTest0058L()
    {
    iLogger->Log( KTextApiPathL );
    iCollectionUtility1->Collection().OpenL(2, EMPXOpenDefault);
    return 0;
    }    

TInt CCollectionTests::StartTest0059L()
    {        
    iLogger->Log( KTextApiBackL );
    iCollectionUtility1->Collection().BackL();
    return 0;
    }

TInt CCollectionTests::StartTest0060L()
    {
    iLogger->Log( KTextApiBackL );
    iCollectionUtility1->Collection().BackL();
    return 0;
    }

TInt CCollectionTests::StartTest0061L()
    {
    // MMPXCollectionUtility::IsRemote not implemented
    iLogger->Log( KTextApiIsRemote );
    iLogger->Log( KTextNotImplemented );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0062L()
    {
    iLogger->Log( KTextApiCancelRequest );
    iLogger->Log( _L("Cancel Request can be called through UI \"Stop\" key") );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0063L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0064L()
    {
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0065L()
    {
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0066L()
    {
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0067L()
    {
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0068L()
    {
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0069L()
    {
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0070L()
    {
    /*
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    // Adding empty media
    TMC_ASSERT(err == KErrArgument, 1);
    
    CleanupStack::PopAndDestroy( media );
    */
    RunNext(); // do not remove
    return 0;
    }

TInt CCollectionTests::StartTest0071L()
    {
    /*
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    // Adding an empty item
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNotSupported, 1);
    
    CleanupStack::PopAndDestroy( media );
    */
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0072L()
    {
    /*
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, KNullUid);
    // Adding with NULL collection Id
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNotSupported, 1);
    
    CleanupStack::PopAndDestroy( media );
    */
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0073L()
    {
    /*
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(0x12345678)); // arbitary Uid
    // Adding with non-supported collection Id
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNotSupported, 1);
    
    CleanupStack::PopAndDestroy( media );
    */
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0074L()
    {
    /*
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTextValueL(KMPXMediaGeneralUri, KNullDesC);
    // Adding with NULL Uri
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrArgument, 1);
    
    CleanupStack::PopAndDestroy( media );
    */
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0075L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    
    /*
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    // Adding with uri with non-suported extension
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTextValueL(KMPXMediaGeneralUri, _L("c:\\song.abc"));    // unsupported extension
    
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );

    TMC_ASSERT(err == KErrArgument, 1);
    
    CleanupStack::PopAndDestroy( media );
    RunNext();
    */
    
    //TODO: Collection framework won't check the Uri extension and select right plugin
    //      Therefore, this test case doesn't make any sense until above feature has been implemented
    iLogger->Log( _L("TODO: feature not implemented.  Test case skipped") );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0076L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );

	/*
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXGroup);
    // Adding an empty group item
    iLogger->Log( KTextPanicExpected );

    // TODO: panic will raised
    //TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    //TMC_ASSERT(err == KErrNotSupported, 1);
    TMC_ASSERT(EFalse, 1);  // mark it as failing
    
    CleanupStack::PopAndDestroy( media );
    */
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0077L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    CMPXMediaArray* array = CMPXMediaArray::NewL();
    CleanupStack::PushL( array );
    
    // Add empty media
    CMPXMedia* content = CMPXMedia::NewL();
    CleanupStack::PushL( content );
    content->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    array->AppendL( content );
    CleanupStack::Pop( content );
    
    // Adding an array and set media to group
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXGroup);
    media->SetCObjectValueL(KMPXMediaArrayContents, array);
    media->SetTObjectValueL<TInt>(KMPXMediaArrayCount, array->Count() );
            
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNotSupported, 1);
    
    CleanupStack::PopAndDestroy(2, media);  // array, media
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0078L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0078"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );

    return 0;
    }

TInt CCollectionTests::StartTest0079L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    CMPXMediaArray* array = CMPXMediaArray::NewL();
    CleanupStack::PushL( array );
    
    // Add 1st media
    CMPXMedia* content = CMPXMedia::NewL();
    CleanupStack::PushL( content );
    content->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    content->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    content->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0079"));
    array->AppendL( content );
    CleanupStack::Pop( content );
    // Add 2nd media
    content = CMPXMedia::NewL();
    CleanupStack::PushL( content );
    content->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    content->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    content->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0079"));
    array->AppendL( content );
    CleanupStack::Pop( content );
    
    // Adding an array and set media to group
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXGroup);
    media->SetCObjectValueL(KMPXMediaArrayContents, array);
    media->SetTObjectValueL<TInt>(KMPXMediaArrayCount, array->Count() );
            
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy(2, media);  // array, media
    iTempCnt = 2;   // expecting 2 sets of callback

    return 0;
    }

TInt CCollectionTests::StartTest0080L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0080"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );

    return 0;
    }

TInt CCollectionTests::StartTest0081L()
    {
    // MMPXCollection::AddL
    iLogger->Log( KTextApiAddL );
    iLogger->Log( _L("Reuse part of Test0078") );
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTextValueL(KMPXMediaGeneralUri, _L("\\song.ts"));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0078"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().AddL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );

    return 0;
    }
    
TInt CCollectionTests::StartTest0082L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }    
    
TInt CCollectionTests::StartTest0083L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0084L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
          
	/*          
    CMPXCollectionPath* path = iCollectionUtility1->Collection().PathL();
    CleanupStack::PushL( path );
    //TestUtility::LogCollectionPath(*path, iLogger);

    TMC_ASSERT(path->Levels() == 1, 1); // Assume currently browing plugins main view
        
    CMPXCollectionPath* removePath = CMPXCollectionPath::NewL();
    CleanupStack::PushL( removePath );
    TRAPD(err, iCollectionUtility1->Collection().RemoveL(*removePath, NULL));
    CleanupStack::PopAndDestroy(2, path);   // removePath, path
    // No callback expected
    TMC_ASSERT(err == KErrArgument, 2);
    */
    
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0085L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    iCollectionUtility1->Collection().OpenL( *path );
    CleanupStack::PopAndDestroy( path );
    return 0;
    }
    
TInt CCollectionTests::StartTest0086L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );    // empty path
    TRAPD(err, iCollectionUtility1->Collection().RemoveL( *path ));
    CleanupStack::PopAndDestroy( path );
    TMC_ASSERT(err == KErrArgument, 1);
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0087L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );    // empty path
    TRAPD(err, iCollectionUtility1->Collection().RemoveL(*path, this));
    CleanupStack::PopAndDestroy( path );
    TMC_ASSERT(err == KErrArgument, 1);
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0088L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 11 );
    path->AppendL( 211 );
    path->AppendL( 3111 );  // for testing purpose, not actually deleting
    iCollectionUtility1->Collection().RemoveL(*path, this);
    CleanupStack::PopAndDestroy( path );
    return 0;
    }

TInt CCollectionTests::StartTest0089L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 11 );
    path->AppendL( 211 );
    path->AppendL( 3111 );  // for testing purpose, not actually deleting
    iCollectionUtility1->Collection().RemoveL(*path, NULL);
    CleanupStack::PopAndDestroy( path );
    RunNext();  // No callback expected
    return 0;
    }

TInt CCollectionTests::StartTest0090L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 11 );
    path->AppendL( 212 );   // for testing purpose, not actually deleting
    iCollectionUtility1->Collection().RemoveL(*path, this);
    CleanupStack::PopAndDestroy( path );
    return 0;
    }

TInt CCollectionTests::StartTest0091L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 14 );    // for testing purpose, not actually deleting
    iCollectionUtility1->Collection().RemoveL(*path, this);
    CleanupStack::PopAndDestroy( path );
    return 0;
    }

TInt CCollectionTests::StartTest0092L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }        
    
TInt CCollectionTests::StartTest0093L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }        

TInt CCollectionTests::StartTest0094L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }        

TInt CCollectionTests::StartTest0095L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }        

TInt CCollectionTests::StartTest0096L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }        

TInt CCollectionTests::StartTest0097L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }        

TInt CCollectionTests::StartTest0098L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }        

TInt CCollectionTests::StartTest0099L()
    {
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0099"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().RemoveL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );

    return 0;
    }

TInt CCollectionTests::StartTest0100L()
    {
    /*
    // MMPXCollection::RemoveL
    iLogger->Log( KTextApiRemoveL );
    
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0100"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().RemoveL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );
    iTempCnt = 2;   // expecting receiving 2 HandleCollectionMessages
    */
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0101L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0102L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0103L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0104L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0105L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0106L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0107L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0108L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0108"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().SetSyncL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );

    return 0;
    }

TInt CCollectionTests::StartTest0109L()
    {
    // MMPXCollection::SetSyncL
    iLogger->Log( KTextApiSetSyncL );
    
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0109"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().SetSyncL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );
    iTempCnt = 2;   // expecting receiving 2 HandleCollectionMessages
    return 0;
    }

TInt CCollectionTests::StartTest0110L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0111L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0112L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0113L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0114L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0115L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0116L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0117L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    
    /*
    // Reinitialize collection utility first
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this);
    iLogger->Log( KTextPanicExpected ); 
    // TODO: panic will raised - because of no EMcsSetMediaAsync handle
    //       in CMPXCollectionUtility::HandleRunErrorL
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    iCollectionUtility1->Collection().SetL( *media );
    CleanupStack::PopAndDestroy( media );
    TMC_ASSERT(EFalse, 1);  // temp
    */
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0118L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    iLogger->Log( KTextPanicExpected ); 
    // TODO: panic will raised - because of NULL callback
    // Update DoHandleCollectionTest0118L() as the problem is fixed
    
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    iCollectionUtility1->Collection().OpenL( *path );
    CleanupStack::PopAndDestroy( path );
    return 0;
    }
TInt CCollectionTests::StartTest0119L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    
    /*
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0119"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().SetL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );
    //TODO: API deprecated
    iLogger->Log( KTextApiDeprecated );
    */
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0120L()
    {
    // MMPXCollection::SetL
    iLogger->Log( KTextApiSetL );
    
    /*
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    media->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0120"));
    // Adding valid media
    TRAPD(err, iCollectionUtility1->Collection().SetL(*media) );
    TMC_ASSERT(err == KErrNone, 1);
    
    CleanupStack::PopAndDestroy( media );
    iTempCnt = 2;   // expecting receiving 2 HandleCollectionMessages
    //TODO: API deprecated
    iLogger->Log( KTextApiDeprecated );
    */
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0121L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0122L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0123L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0124L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0125L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0126L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    
    // Close iCollectionUtility and re-initialize
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this);
    
    // Find with empty attribute list and cristeria
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array(), *this);
    
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    return 0;
    }
    
TInt CCollectionTests::StartTest0127L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, KNullUid);
    iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array(), *this);
    
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    return 0;
    }

TInt CCollectionTests::StartTest0128L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0128"));
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array(), *this);
    
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    return 0;
    }

TInt CCollectionTests::StartTest0129L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    attrs.Append( KMPXMediaGeneralTitle );
    attrs.Append( KMPXMediaGeneralUri );
    attrs.Append( KMPXMediaGeneralId );
    
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0129"));
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array(), *this);
    
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    return 0;
    }

TInt CCollectionTests::StartTest0130L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0130"));
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array(), *this);
    
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    return 0;
    }    

TInt CCollectionTests::StartTest0131L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Syn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0132L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Syn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0133L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Syn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0134L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Syn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0135L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Syn );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0136L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Syn );
    
    // Close iCollectionUtility and re-initialize
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this);
    
    // Find with empty attribute list and cristeria
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    CMPXMedia* media = NULL;
    TRAPD(err, media = iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array()) );
    DELETE_PTR( media );
    TMC_ASSERT(err == KErrNotSupported, 1);
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0137L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Syn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, KNullUid);
    CMPXMedia* media = NULL;
    TRAPD(err, media = iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array()) );
    DELETE_PTR( media );
    TMC_ASSERT(err == KErrNotSupported, 1);
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0138L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0138"));
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    CMPXMedia* media = NULL;
    TRAPD(err, media = iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array()) );
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    CleanupStack::PushL( media );
    TMC_ASSERT(err == KErrNone, 1);
    TMC_ASSERT(media != NULL, 2);
    
    // Result comparsion
    if( !media->IsSupported(KMPXMediaGeneralTitle) )
        {
        TMC_ASSERT(EFalse , 3);
        return 0;
        }
    const TDesC& title = media->ValueText( KMPXMediaGeneralTitle );
    TMC_ASSERT(title == _L("CollectionPluginTest0138"), 4 );
    
    if( !media->IsSupported(KMPXMediaGeneralCollectionId) )
        {
        TMC_ASSERT(EFalse , 5);
        return 0;
        }
    TUid uid = media->ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );
    TMC_ASSERT(uid == TUid::Uid(KCollectionTestPluginImpId), 6);
    
    if( !media->IsSupported(KMPXMediaGeneralSize) )
        {
        TMC_ASSERT(EFalse , 7);
        return 0;
        }
    TInt attrSize = media->ValueTObjectL<TInt>( KMPXMediaGeneralSize );
    TMC_ASSERT(attrSize == 0, 8);    
    CleanupStack::PopAndDestroy( media );
    RunNext();
    return 0;
    }    

TInt CCollectionTests::StartTest0139L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    attrs.Append( KMPXMediaGeneralTitle );
    attrs.Append( KMPXMediaGeneralUri );
    attrs.Append( KMPXMediaGeneralId );
    
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0139"));
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    CMPXMedia* media = NULL;
    TRAPD(err, media = iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array()) );
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    CleanupStack::PushL( media );
    TMC_ASSERT(err == KErrNone, 1);
    TMC_ASSERT(media != NULL, 2);
    
    // Result comparsion
    if( !media->IsSupported(KMPXMediaGeneralTitle) )
        {
        TMC_ASSERT(EFalse , 3);
        return 0;
        }
    const TDesC& title = media->ValueText( KMPXMediaGeneralTitle );
    TMC_ASSERT(title == _L("CollectionPluginTest0139"), 4 );
    
    if( !media->IsSupported(KMPXMediaGeneralCollectionId) )
        {
        TMC_ASSERT(EFalse , 5);
        return 0;
        }
    TUid uid = media->ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );
    TMC_ASSERT(uid == TUid::Uid(KCollectionTestPluginImpId), 6);
    
    if( !media->IsSupported(KMPXMediaGeneralSize) )
        {
        TMC_ASSERT(EFalse , 7);
        return 0;
        }
    TInt attrSize = media->ValueTObjectL<TInt>( KMPXMediaGeneralSize );
    TMC_ASSERT(attrSize == 3, 8);
    CleanupStack::PopAndDestroy( media );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0140L()
    {
    // MMPXCollection::FindAllL
    iLogger->Log( KTextApiFindAllL_Asyn );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXSearchCriteria* criteria = CMPXSearchCriteria::NewL();
    CleanupStack::PushL( criteria );
    criteria->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0140"));
    criteria->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(KCollectionTestPluginImpId));
    CMPXMedia* media = NULL;
    TRAPD(err, media = iCollectionUtility1->Collection().FindAllL(*criteria, attrs.Array()) );
    CleanupStack::PopAndDestroy(2, &attrs); // criteria, attrs
    
    TMC_ASSERT(err == KErrNotFound, 1);
    TMC_ASSERT(media == NULL, 2);
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0141L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }    

TInt CCollectionTests::StartTest0142L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }    

TInt CCollectionTests::StartTest0143L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }    

TInt CCollectionTests::StartTest0144L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    TRAPD(err, iCollectionUtility1->Collection().MediaL(*path, attrs.Array()) );
    TMC_ASSERT(err == KErrArgument, 1);
    CleanupStack::PopAndDestroy(2, path);   // attrs, path
    RunNext();
    return 0;
    }    

TInt CCollectionTests::StartTest0145L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( NULL );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    iCollectionUtility1->Collection().MediaL(*path, attrs.Array());
    CleanupStack::PopAndDestroy(2, path);   // attrs, path
    return 0;
    }    

TInt CCollectionTests::StartTest0146L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 14 );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    iCollectionUtility1->Collection().MediaL(*path, attrs.Array());
    CleanupStack::PopAndDestroy(2, path);   // attrs, path
    
    // Do HandleMediaTest0146L() is never called
    RunNext();
    return 0;
    } 

TInt CCollectionTests::StartTest0147L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 15 );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    iCollectionUtility1->Collection().MediaL(*path, attrs.Array());
    CleanupStack::PopAndDestroy(2, path);   // attrs, path
    return 0;
    }    
    
TInt CCollectionTests::StartTest0148L()
    {
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 16 );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    iCollectionUtility1->Collection().MediaL(*path, attrs.Array());
    CleanupStack::PopAndDestroy(2, path);   // attrs, path
    return 0;
    }    

TInt CCollectionTests::StartTest0149L()
    {
    /*
    // MMPXCollection::MediaL
    iLogger->Log( KTextApiMediaL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    path->AppendL( 17 );
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    CMPXAttributeSpecs* spec = CMPXAttributeSpecs::NewL();
    CleanupStack::PushL( spec );
    spec->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0149"));
    iCollectionUtility1->Collection().MediaL(*path, attrs.Array(), spec);
    CleanupStack::PopAndDestroy(3, path);   // spec, attrs, path
    */
    
    RunNext(); // remove later

    return 0;
    }

TInt CCollectionTests::StartTest0150L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0151L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    } 

TInt CCollectionTests::StartTest0152L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    } 

TInt CCollectionTests::StartTest0153L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    TRAPD(err, iCollectionUtility1->Collection().CommandL(EMcCmdRemoveAll, NULL));
    TMC_ASSERT(err == KErrNotSupported, 1);
    RunNext();
    return 0;
    } 

TInt CCollectionTests::StartTest0154L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    TRAPD(err, iCollectionUtility1->Collection().CommandL(EMcCmdRemoveAll, 0x12345678));
    TMC_ASSERT(err == KErrNotSupported, 1);
    RunNext();
    return 0;
    } 

TInt CCollectionTests::StartTest0155L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( _L("Issuing EMcCmdRemoveAll command") );
    
    /* Requires manual check		
    iCollectionUtility1->Collection().CommandL(EMcCmdRemoveAll, KCollectionTestPluginImpId);
    iLogger->Log( KTextManualCheck );
    TMC_ASSERT(EFalse, 1);
    */
    
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0156L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( _L("Issuing EMcCmdReCreateDB command") );
    
	/* Requires manual check		
    iCollectionUtility1->Collection().CommandL(EMcCmdReCreateDB, KCollectionTestPluginImpId);
    iLogger->Log( KTextManualCheck );
    TMC_ASSERT(EFalse, 1);
    */
    
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0157L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( _L("Issuing EMcCmdDbCorrupted command") );
    
    /* Requires manual check		
    iCollectionUtility1->Collection().CommandL(EMcCmdDbCorrupted, KCollectionTestPluginImpId);
    iLogger->Log( KTextManualCheck );
    TMC_ASSERT(EFalse, 1);
    */
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0158L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( _L("Issuing EMcCmdCollectionInit command") );
    
    /* Requires manual check		
    iCollectionUtility1->Collection().CommandL(EMcCmdCollectionInit, KCollectionTestPluginImpId);
    iLogger->Log( KTextManualCheck );
    TMC_ASSERT(EFalse, 1);
    */
    
    RunNext();
    return 0;
    }
TInt CCollectionTests::StartTest0159L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    iLogger->Log( _L("Issuing EMcCmdCollectionResyn command") );
    
    /* Requires manual check		
    iCollectionUtility1->Collection().CommandL(EMcCmdCollectionResyn, KCollectionTestPluginImpId);
    iLogger->Log( KTextManualCheck );
    TMC_ASSERT(EFalse, 1);
    */
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0160L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    TRAPD(err, iCollectionUtility1->Collection().CommandL(EMcCmdCollectionResyn, NULL));
    TMC_ASSERT(err == KErrNotSupported, 1);
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0161L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    
    // Close iCollectionUtility and re-initialize
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this);
    
    // Execute command
    iLogger->Log( _L("Issuing EMcCmdSelect command") );
    TRAPD(err, iCollectionUtility1->Collection().CommandL(EMcCmdSelect, 0));
    TMC_ASSERT(err == KErrNotReady, 1);
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0162L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    // Open CollectionPluginTest
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    iCollectionUtility1->Collection().OpenL( *path );
    CleanupStack::PopAndDestroy( path );
    return 0;
    }  
    
TInt CCollectionTests::StartTest0163L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    // Current path should in main view of CollectionTestPlugin 
    // containing 3 entries: "level11", "level12", "level13" and now select 3rd
    iCollectionUtility1->Collection().CommandL(EMcCmdSelect, 2);
    return 0;
    }
    
TInt CCollectionTests::StartTest0164L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    // Current path should in main view of CollectionTestPlugin 
    // containing 3 entries: "level11", "level12", "level13" and now select -1th one
    TRAPD(err, iCollectionUtility1->Collection().CommandL(EMcCmdSelect, -1));
    TMC_ASSERT(err == KErrArgument, 1);
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0165L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    // Current path should in main view of CollectionTestPlugin 
    // containing 3 entries: "level11", "level12", "level13" and now select 1000th one
    TRAPD(err, iCollectionUtility1->Collection().CommandL(EMcCmdSelect, 1000));
    TMC_ASSERT(err == KErrArgument, 1);
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0166L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL );
    TRAPD(err, iCollectionUtility1->Collection().CommandL(EMcCmdRefresh, 0));
    TMC_ASSERT(err == KErrNotSupported, 1);
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0167L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0168L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0169L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    iLogger->Log( KTextUninstallPlugin );
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0170L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    
    // Close collection utilty
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this);
    
    // Send command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    TRAPD(err, iCollectionUtility1->Collection().CommandL( *cmd ));
    CleanupStack::PopAndDestroy( cmd );
    
    // Seems to be Ready		
    TMC_ASSERT(err == KErrNone, 1);
    //TMC_ASSERT(err == KErrNotReady, 1);
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0171L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    return 0;
    }
    
TInt CCollectionTests::StartTest0172L()    
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, NULL);
    TRAPD(err, iCollectionUtility1->Collection().CommandL( *cmd ));
    CleanupStack::PopAndDestroy( cmd );
    TMC_ASSERT(err == KErrNotSupported, 1);
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0173L()    
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, NULL);
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    return 0;
    }

TInt CCollectionTests::StartTest0174L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0175L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, 175);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0175"));
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0176L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, 176);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0176"));
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    return 0;
    }

TInt CCollectionTests::StartTest0177L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, 177);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0177"));
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    return 0;
    }

TInt CCollectionTests::StartTest0178L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, KMPXCommandIdCollectionSelect);
    cmd->SetTObjectValueL<TInt>(TMPXAttribute(KMPXCommandContentIdCollection, EMPXCollectionCommandIndex),
                                0);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0178"));
    TRAPD(err, iCollectionUtility1->Collection().CommandL( *cmd ));
    CleanupStack::PopAndDestroy( cmd );
    TMC_ASSERT(err == KErrNotReady, 1);
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0179L()
    {
    iLogger->Log( KTextApiCommandL_Ext );
    // Open CollectionPluginTest
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    iCollectionUtility1->Collection().OpenL( *path );
    CleanupStack::PopAndDestroy( path );
    return 0;
    }

TInt CCollectionTests::StartTest0180L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, KMPXCommandIdCollectionSelect);
    cmd->SetTObjectValueL<TInt>(TMPXAttribute(KMPXCommandContentIdCollection, EMPXCollectionCommandIndex),
                                -1);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0180"));
    TRAPD(err, iCollectionUtility1->Collection().CommandL( *cmd ));
    CleanupStack::PopAndDestroy( cmd );
    TMC_ASSERT(err == KErrArgument, 1);
    RunNext();
    return 0;
    }    

TInt CCollectionTests::StartTest0181L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, KMPXCommandIdCollectionSelect);
    cmd->SetTObjectValueL<TInt>(TMPXAttribute(KMPXCommandContentIdCollection, EMPXCollectionCommandIndex),
                                1000);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0181"));
    TRAPD(err, iCollectionUtility1->Collection().CommandL( *cmd ));
    CleanupStack::PopAndDestroy( cmd );
    TMC_ASSERT(err == KErrArgument, 1);
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0182L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, KMPXCommandIdCollectionSelect);
    cmd->SetTObjectValueL<TInt>(TMPXAttribute(KMPXCommandContentIdCollection, EMPXCollectionCommandIndex),
                                2);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0182"));
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    return 0;
    }

TInt CCollectionTests::StartTest0183L()
    {    
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    
    /* This test must be manually checked		
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, 183);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0183"));
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    iLogger->Log( KTextManualCheck );
    TMC_ASSERT(EFalse, 1);    
    */
    
    RunNext();
    return 0;
    }    

TInt CCollectionTests::StartTest0184L()
    {
    // MMPXCollection::CommandL
    iLogger->Log( KTextApiCommandL_Ext );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralCollectionId, KCollectionTestPluginImpId);
    cmd->SetTObjectValueL<TMPXCommandId>(KMPXCommandGeneralId, 184);
    cmd->SetTextValueL(KMPXMediaGeneralTitle, _L("CollectionPluginTest0184"));
    iCollectionUtility1->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );
    return 0;
    }

TInt CCollectionTests::StartTest0185L()
    {
    // MMPXCollection::GetSupportedTypesL
    iLogger->Log( KTextApiGetSupportedTypesL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0186L()
    {
    /*
    // MMPXCollection::GetSupportedTypesL
    iLogger->Log( KTextApiGetSupportedTypesL );
    RPointerArray<CMPXCollectionType> typeArr;
    CleanupClosePushL( typeArr );
    TRAPD(err, iCollectionUtility1->Collection().GetSupportedTypesL( typeArr ));
    TMC_ASSERT(err == KErrNone, 1);
    for(TInt i = 0; i < typeArr.Count(); i++)
        {
        TBuf<512> log;
        CMPXCollectionType* type = typeArr[i];
        TUid uid = type->Uid();
        const CDesCArray& exts = type->Extensions();
        const CDesCArray& mimes = type->Mimetypes();
        
        iLogger->Log(_L("[%d] Uid=0x%x"), i, uid.iUid);
        
        TInt j = 0;
        log.Format( _L("[%d] supports ext: "), i );
        for(j = 0; j < exts.Count(); j++)
            {
            TPtrC ext = exts[j];
            log.AppendFormat(_L("[%S]"), &ext);
            }
        iLogger->Log( log );
        
        log.Format( _L("[%d] supports mime: "), i );
        for(j = 0; j < mimes.Count(); j++)
            {
            TPtrC mime = mimes[j];
            log.AppendFormat(_L("[%S]"), &mime);
            }
        iLogger->Log( log );
        }
    typeArr.ResetAndDestroy();
    CleanupStack::PopAndDestroy( &typeArr );
    iLogger->Log( KTextManualCheck );
    TMC_ASSERT(EFalse, 1);
    */
    
    RunNext();
    
    return 0;
    }


TInt CCollectionTests::StartTest0187L()
    {
    // MMPXCollection::GetSupportedCapabilitiesL 
    iLogger->Log( KTextApiGetSupportedCapabilitiesL );
    iLogger->Log( KTextUninstallPlugin );
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0188L()
    {
    /*
    // MMPXCollection::GetSupportedCapabilitiesL 
    iLogger->Log( KTextApiGetSupportedCapabilitiesL );
    TCollectionCapability capability;
    iCollectionUtility1->Collection().GetSupportedCapabilitiesL( capability );
    TMC_ASSERT(capability == EMcNotModifiable, 1);
    */
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0189L()
    {
    // MMPXCollection::GetSupportedCapabilitiesL 
    iLogger->Log( KTextApiGetSupportedCapabilitiesL );
    
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this);
    
    // Get current context capability
    TCollectionCapability capability;
    iCollectionUtility1->Collection().GetSupportedCapabilitiesL( capability );
    TMC_ASSERT(capability == 0, 1);
    
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0190L()
    {
    // MMPXCollection::CollectionIDL
    iLogger->Log( KTextApiCollectionIDL_Col );
    TUid absUid = KLocalCollection;
    TUid uid = iCollectionUtility1->Collection().CollectionIDL( absUid );
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    TMC_ASSERT(uid == TUid::Uid(0x101FFC3A), 1);
    
    RunNext();
    return 0;
    }
    
TInt CCollectionTests::StartTest0191L()
    {
    // MMPXCollection::CollectionIDL
    iLogger->Log( KTextApiCollectionIDL );
    TUid absUid = KPodcastCollection;
    TUid uid = iCollectionUtility1->Collection().CollectionIDL( absUid );
    TMC_ASSERT(uid == TUid::Uid(0x101FFC3C), 1);
    
	RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0192L()
    {
    // MMPXCollection::CollectionIDL
    iLogger->Log( KTextApiCollectionIDL );
    TUid absUid = KInMemoryCollection;
    TUid uid = iCollectionUtility1->Collection().CollectionIDL( absUid );
    TMC_ASSERT(uid == TUid::Uid(0x101FFCD8), 1);
    
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0193L()
    {
    // MMPXCollection::CollectionIDL
    iLogger->Log( KTextApiCollectionIDL );
    TUid absUid = TUid::Uid(0x12345678);    // invalid Uid
    TUid uid;
    TRAPD(err, uid = iCollectionUtility1->Collection().CollectionIDL( absUid ));
    TMC_ASSERT(err == KErrNone, 1);
    TMC_ASSERT(uid == KNullUid, 2);
        
    RunNext();
    return 0;
    }

TInt CCollectionTests::StartTest0194L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    CMPXCollectionPath* path = CMPXCollectionPath::NewL();
    CleanupStack::PushL( path );
    path->AppendL( KCollectionTestPluginImpId );
    iCollectionUtility1->Collection().OpenL( *path );
    CleanupStack::PopAndDestroy( path );
    return 0;
    }

TInt CCollectionTests::StartTest0195L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgFormatStart, EDriveE);
    return 0;
    }

TInt CCollectionTests::StartTest0196L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgFormatEnd, EDriveE);
    return 0;
    }

TInt CCollectionTests::StartTest0197L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgDiskRemoved, EDriveE);
    return 0;
    }

TInt CCollectionTests::StartTest0198L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgDiskInserted, EDriveE);
    return 0;
    }

TInt CCollectionTests::StartTest0199L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgUSBMassStorageStart, EDriveE);
    return 0;
    }

TInt CCollectionTests::StartTest0200L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgUSBMassStorageEnd, EDriveE);
    return 0;
    }
    
TInt CCollectionTests::StartTest0201L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgRefreshStart, EDriveE);
    return 0;
    }

TInt CCollectionTests::StartTest0202L()
    {
    // MMPXCollection::NotifyL
    iLogger->Log( KTextApiNotifyL );
    iCollectionUtility1->Collection().NotifyL(EMcMsgRefreshEnd, EDriveE);
    return 0;
    }

void CCollectionTests::DoHandleCollectionTest0001L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iCollectionUtility1->Collection().OpenL();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0011L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        // To protect that 2 messages will be received from iCollectionUtility1 and iCollectionUtility2
        if(iTempCnt++ == 0)
            {
            TMPXOpenMode openMode = EMPXOpenDefault;
            switch( iTestIndex )
                {
                case 11:
                    openMode = EMPXOpenDefault; break;
                case 13:
                    openMode = EMPXOpenGroupOrPlaylist; break;
                case 15:
                    openMode = EMPXOpenAllItems; break;
                case 17:
                    openMode = EMPXOpenNoPlaylist; break;
                case 19:
                case 20: 
                    openMode = EMPXOpenPlaylistOnly; break;
                default:
                    break;
                };
            iCollectionUtility2->Collection().OpenL( openMode );
            }
        }
    else if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)
        {
        iLogger->Log( KTextCollectionChange );
        // If open at root level, only EMPXOpenDefault and EMPXOpenGroupOrPlaylist are supported
        // otherwise, framework assume openmode has changed and reopen at root level
        // For ETCFwCollectTest0015(0016), 0017(0018), and 0019(0020), we have to handle callback here
        // However, TMPXOpenMode will be changed to support EMPXOpenDefault only later on
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0012L(const CMPXMedia& aEntries, TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);
    
    TMPXAttribute uidAttr(KMPXMediaIdGeneral,EMPXMediaGeneralId);
    RArray<TMPXItemId> impUids;
    CleanupClosePushL( impUids );
    impUids.Append( KMusicCollectionUID );
    impUids.Append( KPodcastCollectionUID );
    impUids.Append( KCollectionTestPluginImpId );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, impUids, 
                                           uidAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &impUids );
    
    TMC_ASSERT(match, 2);

	// Each of tests 2-19 is closes and reopens so no need to Cancel.
    RunNext();
    }
    
void CCollectionTests::DoHandleCollectionTest0021L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    if(iTestIndex == 20)
    	{
    	DELETE_COLLECTION_PTR(iCollectionUtility2);
    	}
	else 
		{
    	(void)aComplete;
    	(void)aIndex;
    	TMC_ASSERT(aError == KErrNone, 1);
    
    	TMPXAttribute uidAttr(KMPXMediaIdGeneral,EMPXMediaGeneralId);
    	RArray<TMPXItemId> impUids;
    	CleanupClosePushL( impUids );
    	impUids.Append( KMusicCollectionUID );
    	impUids.Append( KPodcastCollectionUID );
    	impUids.Append( KCollectionTestPluginImpId );
    	TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, impUids, 
        	                              uidAttr, CCollectionTests::CompareByTMPXItemId);
    	CleanupStack::PopAndDestroy( &impUids );
    
    	TMC_ASSERT(match, 2);
    	}
    	
    RunNext();	
    }

void CCollectionTests::DoHandleCollectionTest0022L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)   // 1st
        {
        iLogger->Log( KTextCollectionChange );
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0023L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 11 );
    itemIds.Append( 12 );
    itemIds.Append( 13 );
    itemIds.Append( 990 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                             itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0029L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        iCollectionUtility1->Collection().OpenL();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0029L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 2);

    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 211 );
    itemIds.Append( 212 );
    itemIds.Append( 213 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                        itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 3);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0030L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0031L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 11 );
    itemIds.Append( 12 );
    itemIds.Append( 13 );
    itemIds.Append( 990 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                          itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0032L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0033L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 221 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                              itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    
    // Match Uri
    CDesCArrayFlat* textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    textArray->AppendL( KNullDesC );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, uriAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 3);
    
    // Match Title - should not contain this attribute
    textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    textArray->AppendL( _L("level221") );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, titleAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match == EFalse, 4);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0034L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0035L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 231 );
    itemIds.Append( 232 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                            itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    
    // Match Uri
    CDesCArrayFlat* textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    textArray->AppendL( KNullDesC );
    textArray->AppendL( KNullDesC );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, uriAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 3);
    
    // Match Title - should not contain this attribute
    textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    textArray->AppendL( _L("level231") );
    textArray->AppendL( _L("level232") );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, titleAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match == EFalse, 4);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0036L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)   // 1st
        {
        iLogger->Log( KTextCollectionChange );
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        // iCollectionUtility1->Collection().OpenL();   // TODO: may update later
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0036L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    // TODO: Update may require
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 2);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 231 );
    itemIds.Append( 232 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                                   itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 3);
    
    // Match Uri
    CDesCArrayFlat* textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    textArray->AppendL( KNullDesC );
    textArray->AppendL( KNullDesC );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, uriAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 4);
    
    // Match Title
    textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    textArray->AppendL( _L("level231") );
    textArray->AppendL( _L("level232") );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, titleAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 5);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0037L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0038L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( KPodcastCollectionUID );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                             itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0039L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        iCollectionUtility1->Collection().OpenL( EMPXOpenDefault );
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
void CCollectionTests::DoHandleCollectionTest0039L(const CMPXMedia& aEntries,
                                     TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 2);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                          itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 3);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0040L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0041L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( KCollectionTestPluginImpId );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                         itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0043L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)   // 1st
        {
        iLogger->Log( KTextCollectionChange );
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0044L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 212 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                            itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    
    // Match Uri
    CDesCArrayFlat* textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    textArray->AppendL( KNullDesC );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, uriAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 3);
    
    // Match Title
    textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    textArray->AppendL( _L("level212") );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, titleAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 4);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0047L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0048L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                            itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    
    // Match Uri
    CDesCArrayFlat* textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, uriAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 3);
    
    // Match Title
    textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, titleAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 4);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0051L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0052L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);

    // Match Id
    RArray<TMPXItemId> itemIds;
    CleanupClosePushL( itemIds );
    TMPXAttribute itemAttr(KMPXMediaGeneralId);
    itemIds.Append( 211 );
    itemIds.Append( 212 );
    itemIds.Append( 213 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, itemIds, 
                                              itemAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &itemIds );
    TMC_ASSERT(match, 2);
    
    // Match Uri
    CDesCArrayFlat* textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute uriAttr(KMPXMediaGeneralUri);
    textArray->AppendL( KNullDesC );
    textArray->AppendL( KNullDesC );
    textArray->AppendL( KNullDesC );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, uriAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 3);
    
    // Match Title
    textArray = new (ELeave) CDesCArrayFlat( 2 );
    CleanupStack::PushL( textArray );
    TMPXAttribute titleAttr(KMPXMediaGeneralTitle);
    textArray->AppendL( _L("level211") );
    textArray->AppendL( _L("level212") );
    textArray->AppendL( _L("level213") );
    match = TestUtility::MatchMediaArrayTextObject(aEntries, *textArray, titleAttr);
    CleanupStack::PopAndDestroy( textArray );
    TMC_ASSERT(match, 4);
    
   	RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0056L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        
        CMPXCollectionPath* path = iCollectionUtility1->Collection().PathL();
        CleanupStack::PushL( path );
        
        TMC_ASSERT(path->Levels() == 1, 1);
        TMC_ASSERT(path->Count() == KRootLvPluginNum, 2);
        TMC_ASSERT(path->Index(0) == 0, 3);
        TMC_ASSERT(path->Id(0).iId1 == (TInt)KMusicCollectionUID, 4);
        path->SetToFirst();
        TMC_ASSERT(path->Id().iId1 == (TInt)KMusicCollectionUID, 5);
        ++(*path);
        TMC_ASSERT(path->Id().iId1 == (TInt)KPodcastCollectionUID, 6);
        ++(*path);
        TMC_ASSERT(path->Id().iId1 == (TInt)KCollectionTestPluginImpId, 7);
        
        CleanupStack::PopAndDestroy( path );        
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0057L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)   // 1st
        {
        iLogger->Log( KTextCollectionChange );
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        CMPXCollectionPath* path = iCollectionUtility1->Collection().PathL();
        CleanupStack::PushL( path );
        
        TMC_ASSERT(path->Levels() == 2, 1);
        TMC_ASSERT(path->Count() == 4, 2);
        TMC_ASSERT(path->Index(0) == KColTestPluginIndex, 3);
        TMC_ASSERT(path->Id(0) == (TInt)KCollectionTestPluginImpId, 4);
        TMC_ASSERT(path->Index(1) == 0, 5);
        TMC_ASSERT(path->Id(1) == 11, 6);
        path->SetToFirst();
        TMC_ASSERT(path->Id() == 11, 7);
        ++(*path);
        TMC_ASSERT(path->Id() == 12, 8);
        ++(*path);
        TMC_ASSERT(path->Id() == 13, 9);
        ++(*path);
        TMC_ASSERT(path->Id() == 990, 9);

        CleanupStack::PopAndDestroy( path );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0058L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        CMPXCollectionPath* path = iCollectionUtility1->Collection().PathL();
        CleanupStack::PushL( path );
        
        TMC_ASSERT(path->Levels() == 3, 1);
        TMC_ASSERT(path->Count() == 2, 2);
        TMC_ASSERT(path->Index(0) == KColTestPluginIndex, 3);
        TMC_ASSERT(path->Id(0) == (TInt)KCollectionTestPluginImpId, 4);
        TMC_ASSERT(path->Index(1) == 2, 5);
        TMC_ASSERT(path->Id(1) == 13, 6);
        TMC_ASSERT(path->Index(2) == 0, 7);
        TMC_ASSERT(path->Id(2) == 231, 8);
        path->SetToFirst();
        TMC_ASSERT(path->Id() == 231, 9);
        ++(*path);
        TMC_ASSERT(path->Id() == 232, 10);
        
        CleanupStack::PopAndDestroy( path );
        
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0059L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        iCollectionUtility1->Collection().OpenL();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0059L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);
    
    TMPXAttribute idAttr(KMPXMediaIdGeneral,EMPXMediaGeneralId);
    RArray<TMPXItemId> ids;
    CleanupClosePushL( ids );
    ids.Append( 11 );
    ids.Append( 12 );
    ids.Append( 13 );
    ids.Append( 990 );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, ids, 
                                              idAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &ids );
    
    TMC_ASSERT(match, 2);
    TMC_ASSERT(aIndex == 2, 3);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0060L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)   // 1st
        {
        iLogger->Log( KTextCollectionChange );
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        iCollectionUtility1->Collection().OpenL();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0060L(const CMPXMedia& aEntries,
                                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    (void)aComplete;
    (void)aIndex;
    TMC_ASSERT(aError == KErrNone, 1);
    
    TMPXAttribute uidAttr(KMPXMediaIdGeneral,EMPXMediaGeneralId);
    RArray<TMPXItemId> impUids;
    CleanupClosePushL( impUids );
    impUids.Append( KMusicCollectionUID );
    impUids.Append( KPodcastCollectionUID );
    impUids.Append( KCollectionTestPluginImpId );
    TBool match = TestUtility::MatchMediaArrayObject<TMPXItemId>(aEntries, impUids, 
                                              uidAttr, CCollectionTests::CompareByTMPXItemId);
    CleanupStack::PopAndDestroy( &impUids );
    
    TMC_ASSERT(match, 2);
    TMC_ASSERT(aIndex == KColTestPluginIndex, 3);
    
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0078L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMcItemInserted &&
       aMessage.Data() == 78)   // 1st
        {
        iLogger->Log( _L("Insertion of item Id 78 received") );
        
        RunNext();
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged &&
            aMessage.Type() == EMcPathChangedByCollectionChange)    // 2nd
        {
        iLogger->Log( _L("PathChangeByCollectionChange received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }

void CCollectionTests::DoHandleCollectionTest0079L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMcItemInserted &&
       aMessage.Data() == 79)   // 1st
        {
        iLogger->Log( _L("Insertion of item Id 79 received") );   
/*         
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged &&
            aMessage.Type() == EMcPathChangedByCollectionChange)    // 2nd
        {
        iLogger->Log( _L("PathChangeByCollectionChange received") );
*/        
        if(--iTempCnt <= 0)
            {
            // 2 sets of callbacks received
            RunNext();
            }
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }

void CCollectionTests::DoHandleCollectionTest0080L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMcItemInserted && //EMcPlaylistInserted &&
       aMessage.Data() == 80)   // 1st
        {
        iLogger->Log( _L("Insertion of playlist Id 80 received") );
        
        RunNext();
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged &&
            aMessage.Type() == EMcPathChangedByCollectionChange)    // 2nd
        {
        iLogger->Log( _L("PathChangeByCollectionChange received") );

        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0084L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsRemovePath &&
       aMessage.Data() == KErrNotReady)
        {
        iLogger->Log( _L("RemoveL error message recieved: KErrNotReady") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0085L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)   // 1st
        {
        iLogger->Log( KTextCollectionChange );
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && 
            aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0086L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsRemovePath &&
       aMessage.Data() == KErrArgument)
        {
        iLogger->Log( _L("RemoveL error message recieved: KErrArgument") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }    
    
void CCollectionTests::DoHandleRemoveLTest0088L(MDesCArray& aUriArray, TInt aError)
    {
    TMC_ASSERT(aError == KErrNone, 1);
    TMC_ASSERT(aUriArray.MdcaCount() == 1, 2);
    TMC_ASSERT(aUriArray.MdcaPoint(0) == _L("\\song3111.ts"), 3);
    RunNext();
    }
    
void CCollectionTests::DoHandleRemoveLTest0089L(MDesCArray& /*aUriArray*/, TInt /*aError*/)
    {
    // Should never receive this callback
    TMC_ASSERT(EFalse, 1);
    RunNext();
    }
    
void CCollectionTests::DoHandleRemoveLTest0090L(MDesCArray& aUriArray, TInt aError)
    {
    TMC_ASSERT(aError == KErrNone, 1);
    TMC_ASSERT(aUriArray.MdcaCount() == 3, 2);
    TMC_ASSERT(aUriArray.MdcaPoint(0) == _L("\\song3121.ts"), 3);
    TMC_ASSERT(aUriArray.MdcaPoint(1) == _L("\\song3122.ts"), 4);
    TMC_ASSERT(aUriArray.MdcaPoint(2) == _L("\\song3123.tp"), 5);
    RunNext();
    }

void CCollectionTests::DoHandleRemoveLTest0091L(MDesCArray& /*aUriArray*/, TInt aError)
    {
    TMC_ASSERT(aError == KErrNotFound, 1);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0091L( const TMPXCollectionMessage& aMessage )
    {
    // TODO: return error through HandleRemoveL/HandleCollectionMessage
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsRemovePath &&
       aMessage.Data() == KErrNotFound)
        {
        iLogger->Log( _L("RemoveL error message recieved: KErrNotFound") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }

void CCollectionTests::DoHandleCollectionTest0099L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMPXItemDeleted &&
       aMessage.Data() == 99)   // 1st
        {
        iLogger->Log( _L("Delete of item Id 99 received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }

void CCollectionTests::DoHandleCollectionTest0100L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMPXItemDeleted &&
       aMessage.Data() == 100)   // 1st
        {
        iLogger->Log( _L("[%d] Delete of item Id 99 received"), iTempCnt);
        if(--iTempCnt <= 0)
            {
            RunNext();
            }
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }

void CCollectionTests::DoHandleCollectionTest0108L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMPXItemModified &&
       aMessage.Data() == 108)   // 1st
        {
        iLogger->Log( _L("Modification of item Id 108 received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }

void CCollectionTests::DoHandleCollectionTest0109L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMPXItemModified &&
       aMessage.Data() == 109)   // 1st
        {
        iLogger->Log( _L("[%d] Modification of item Id 109 received"), iTempCnt);
        if(--iTempCnt <= 0)
            {
            RunNext();
            }
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }
void CCollectionTests::DoHandleCollectionTest0117L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EAsyncOpComplete &&
       aMessage.Type() == EMcsSetMediaAsync &&
       aMessage.Data() == KErrNotReady)   // 1st
        {
        iLogger->Log( _L("EAsyncOpComplete[EMcsSetMediaAsync] message: KErrNotReady received") );
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EError &&
            aMessage.Type() == EMcsSetMediaAsync &&
            aMessage.Data() == KErrNotReady)   // 2nd
        {
        iLogger->Log( _L("EError[EMcsSetMediaAsync] message: KErrNotReady received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }

void CCollectionTests::DoHandleCollectionTest0118L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged)   // 1st
        {
        iLogger->Log( KTextCollectionChange );
        iLogger->Log( KTextPluginImpUid, aMessage.Data());
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && 
            aMessage.Type() == EMcPathChangedByOpen) // 2nd
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();  // temp
        
        /*
        // TODO: continue running will panic the thread
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL( media );
        iCollectionUtility1->Collection().SetL( *media );
        CleanupStack::PopAndDestroy( media );
        */
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EAsyncOpComplete &&
       aMessage.Type() == EMcsSetMediaAsync &&
       aMessage.Data() == KErrArgument)   // 3st
        {
        iLogger->Log( _L("EAsyncOpComplete[EMcsSetMediaAsync] message: KErrNotReady received") );
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EError &&
            aMessage.Type() == EMcsSetMediaAsync &&
            aMessage.Data() == KErrArgument)   // 4nd
        {
        iLogger->Log( _L("EError[EMcsSetMediaAsync] message: KErrNotReady received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0119L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMPXItemModified &&
       aMessage.Data() == 119)   // 1st
        {
        iLogger->Log( _L("Modification of item Id 119 received") );
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EAsyncOpComplete &&
            aMessage.Type() == EMcsSetMediaAsync &&
            aMessage.Data() == KErrNone)   // 2nd
        {
        iLogger->Log( _L("EAsyncOpComplete[EMcsSetMediaAsync] message: KErrNone received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    }    
    
void CCollectionTests::DoHandleCollectionTest0120L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EItemChanged &&
       aMessage.Type() == EMPXItemModified &&
       aMessage.Data() == 120)   // 1st
        {
        iLogger->Log( _L("Modification of item Id 120 received") );
        TMC_ASSERT(iTempCnt > 0, 3);
        iTempCnt--;
        if(iTempCnt == 0)   // temp fix
            {            
            TMC_ASSERT(EFalse, 4);  // remove this when fixed
            //RunNext();      // temp fix
            }
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EAsyncOpComplete &&
            aMessage.Type() == EMcsSetMediaAsync &&
            aMessage.Data() == KErrNone)   // 2nd
        {
        iLogger->Log( _L("EAsyncOpComplete[EMcsSetMediaAsync] message: KErrNone received") );
        iLogger->Log( _L("THIS MESSAGE IS EXPECTED TO BE RECEIVED LAST") ); // temp
        //RunNext();        // temp
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    } 
    
void CCollectionTests::DoHandleFindAllTest0126L(const CMPXMedia& /*aResults*/, TBool aComplete,TInt aError)
    {
    TMC_ASSERT(aComplete == 1, 1);
    TMC_ASSERT(aError == KErrNotSupported, 2);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0126L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsFindAll &&
       aMessage.Data() == KErrNotSupported)
        {
        iLogger->Log( _L("EError[EMcsFindAll] message: KErrNotSupported received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 3);
        }
    }

void CCollectionTests::DoHandleFindAllTest0127L(const CMPXMedia& /*aResults*/, TBool aComplete,TInt aError)
    {
    TMC_ASSERT(aComplete == 1, 1);
    TMC_ASSERT(aError == KErrNotSupported, 2);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0127L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsFindAll &&
       aMessage.Data() == KErrNotSupported)
        {
        iLogger->Log( _L("EError[EMcsFindAll] message: KErrNotSupported received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 3);
        }
    }
    
void CCollectionTests::DoHandleFindAllTest0128L(const CMPXMedia& aResults, TBool aComplete,TInt aError)
    {
    TMC_ASSERT( aComplete, 1 );
    TMC_ASSERT( aError == KErrNone, 2 );
    if( !aResults.IsSupported(KMPXMediaGeneralTitle) )
        {
        TMC_ASSERT(EFalse , 3);
        return;
        }
    const TDesC& title = aResults.ValueText( KMPXMediaGeneralTitle );
    TMC_ASSERT(title == _L("CollectionPluginTest0128"), 4 );
    
    if( !aResults.IsSupported(KMPXMediaGeneralCollectionId) )
        {
        TMC_ASSERT(EFalse , 5);
        return;
        }
    TUid uid = aResults.ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );
    TMC_ASSERT(uid == TUid::Uid(KCollectionTestPluginImpId), 6);
    
    if( !aResults.IsSupported(KMPXMediaGeneralSize) )
        {
        TMC_ASSERT(EFalse , 7);
        return;
        }
    TInt attrSize = aResults.ValueTObjectL<TInt>( KMPXMediaGeneralSize );
    TMC_ASSERT(attrSize == 0, 8);    
    RunNext();
    }

void CCollectionTests::DoHandleFindAllTest0129L(const CMPXMedia& aResults, TBool aComplete,TInt aError)
    {
    TMC_ASSERT( aComplete, 1 );
    TMC_ASSERT( aError == KErrNone, 2 );
    if( !aResults.IsSupported(KMPXMediaGeneralTitle) )
        {
        TMC_ASSERT(EFalse , 3);
        return;
        }
    const TDesC& title = aResults.ValueText( KMPXMediaGeneralTitle );
    TMC_ASSERT(title == _L("CollectionPluginTest0129"), 4 );
    
    if( !aResults.IsSupported(KMPXMediaGeneralCollectionId) )
        {
        TMC_ASSERT(EFalse , 5);
        return;
        }
    TUid uid = aResults.ValueTObjectL<TUid>( KMPXMediaGeneralCollectionId );
    TMC_ASSERT(uid == TUid::Uid(KCollectionTestPluginImpId), 6);
    
    if( !aResults.IsSupported(KMPXMediaGeneralSize) )
        {
        TMC_ASSERT(EFalse , 7);
        return;
        }
    TInt attrSize = aResults.ValueTObjectL<TInt>( KMPXMediaGeneralSize );
    TMC_ASSERT(attrSize == 3, 8);
    RunNext();
    }

void CCollectionTests::DoHandleFindAllTest0130L(const CMPXMedia& aResults, TBool aComplete,TInt aError)
    {
    TMC_ASSERT( aComplete, 1 );
    TMC_ASSERT( aError == KErrArgument, 2 );
    TMC_ASSERT( &aResults != NULL, 3);
    RunNext();
    }
 
 void CCollectionTests::DoHandleCollectionTest0130L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsFindAll &&
       aMessage.Data() == KErrArgument)
        {
        iLogger->Log( _L("EError[EMcsFindAll] message: KErrArgument received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 4);
        }
    }

void CCollectionTests::DoHandleCollectionTest0145L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsMediaByPath &&
       aMessage.Data() == KErrNotSupported)
        {
        iLogger->Log( _L("EError[EMcsMediaByPath] message: KErrNotSupported received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 2);
        }
    
    }

void CCollectionTests::DoHandleMediaTest0145L(const CMPXMedia& aMedia, TInt aError)
    {
    TMC_ASSERT(aError == KErrNotSupported, 2);
    TMC_ASSERT(&aMedia != NULL, 3);
    RunNext();
    }
    
void CCollectionTests::DoHandleMediaTest0146L(const CMPXMedia& aMedia, TInt aError)
    {
    TMC_ASSERT(aError == KErrNone, 2);
    TMC_ASSERT(&aMedia == NULL, 3);
    RunNext();
    }
    
void CCollectionTests::DoHandleMediaTest0147L(const CMPXMedia& aMedia, TInt aError)
    {
    TMC_ASSERT(aError == KErrNone, 2);
    TMC_ASSERT(aMedia.Count() == 0, 3);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0148L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError &&
       aMessage.Type() == EMcsMediaByPath &&
       aMessage.Data() == KErrNotFound)
        {
        iLogger->Log( _L("EError[EMcsFindAll] message: KErrNotFound received") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 4);
        }
    }

void CCollectionTests::DoHandleMediaTest0148L(const CMPXMedia& aMedia, TInt aError)
    {
    //TMC_ASSERT(aError == KErrNotFound, 2);
    TMC_ASSERT(aError == KErrNone, 2); // it is found!
    TMC_ASSERT(&aMedia != NULL, 3);
    RunNext();
    }

void CCollectionTests::DoHandleMediaTest0149L(const CMPXMedia& aMedia, TInt aError)
    {
    TMC_ASSERT(aError == KErrNone, 2);
    TMC_ASSERT(aMedia.Count() == 1, 3);
    TMC_ASSERT(aMedia.IsSupported(KMPXMediaGeneralTitle), 4);
    if( aMedia.IsSupported(KMPXMediaGeneralTitle) )
        {
        const TDesC& title = aMedia.ValueText( KMPXMediaGeneralTitle );
        TMC_ASSERT(title == _L("CollectionPluginTest0149"), 5);
        }
        
    RunNext();
    }
    
void CCollectionTests::DoHandleCollectionTest0162L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged && 
       aMessage.Data() == KCollectionTestPluginImpId)
        {
        iLogger->Log( KTextCollectionChange );
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && 
            aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0163L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EFocusChanged && 
       aMessage.Type() == 2 )     // TODO: should be aMessage.Data() == 2???
        {
        iLogger->Log( KTextFocusChanged );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0171L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError && 
       aMessage.Type() == EMcsCommandExt &&
       aMessage.Data() == KErrNotReady)
        {
        iLogger->Log( _L("CommandL error message recieved: KErrNotReady") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCommandCompleteTest0171L(CMPXCommand* aCommandResult, TInt aError)
    {
    TMC_ASSERT(aCommandResult != NULL, 1);
    TMC_ASSERT(aError == KErrNotReady, 2);
    RunNext();
    }

void CCollectionTests::DoHandleCommandCompleteTest0173L(CMPXCommand* aCommandResult, TInt aError)
    {
    TMC_ASSERT(aCommandResult != NULL, 1);
    TMC_ASSERT(aError == KErrNotSupported, 2);
    RunNext();
    }
    
void CCollectionTests::DoHandleCollectionTest0173L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError && 
       aMessage.Type() == EMcsCommandExt &&
       aMessage.Data() == KErrNotSupported)
        {
        iLogger->Log( _L("CommandL error message recieved: KErrNotSupported") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCommandCompleteTest0176L(CMPXCommand* aCommandResult, TInt aError)
    {
    TMC_ASSERT(aCommandResult == NULL, 1);
    TMC_ASSERT(aError == KErrNone, 2);
    RunNext();
    }

void CCollectionTests::DoHandleCommandCompleteTest0177L(CMPXCommand* aCommandResult, TInt aError)
    {
    TMC_ASSERT(aCommandResult != NULL, 1);
    TMC_ASSERT(aError == KErrGeneral, 2);
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0177L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EError && 
       aMessage.Type() == EMcsCommandExt &&
       aMessage.Data() == KErrGeneral)
        {
        iLogger->Log( _L("CommandL error message recieved: KErrGeneral") );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0179L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged && 
       aMessage.Data() == KCollectionTestPluginImpId)
        {
        iLogger->Log( KTextCollectionChange );
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && 
            aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }        

void CCollectionTests::DoHandleCollectionTest0182L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EFocusChanged && 
       aMessage.Type() == 2 )     // TODO: should be aMessage.Data() == 2???
        {
        iLogger->Log( KTextFocusChanged );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCommandCompleteTest0184L(CMPXCommand* aCommandResult, TInt aError)
    {
    TMC_ASSERT(aError == KErrNone, 1);
    if( !aCommandResult )
        {
        TMC_ASSERT(EFalse, 2);
        }
    else
        {
        TMC_ASSERT(aCommandResult->Count() == 3, 3);
        TMC_ASSERT(aCommandResult->ValueTObjectL<TMPXCommandId>(KMPXCommandGeneralId) == 184, 4);
        }
        
    RunNext();
    }

void CCollectionTests::DoHandleCollectionTest0194L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::ECollectionChanged && 
       aMessage.Data() == KCollectionTestPluginImpId)
        {
        iLogger->Log( KTextCollectionChange );
        }
    else if(aMessage.Event() == TMPXCollectionMessage::EPathChanged && 
            aMessage.Type() == EMcPathChangedByOpen)
        {
        iLogger->Log( KTextPathChangedByOpen );
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0195L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgFormatStart &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgFormatStart; drive=EDriveE"));
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0196L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgFormatEnd &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgFormatEnd; drive=EDriveE"));
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0197L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgDiskRemoved &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgDiskRemoved; drive=EDriveE"));
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
void CCollectionTests::DoHandleCollectionTest0198L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgDiskInserted &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgDiskInserted; drive=EDriveE"));
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }

void CCollectionTests::DoHandleCollectionTest0199L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgUSBMassStorageStart &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgUSBMassStorageStart; drive=EDriveE"));
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
void CCollectionTests::DoHandleCollectionTest0200L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgUSBMassStorageEnd &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgUSBMassStorageEnd; drive=EDriveE"));
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    } 

void CCollectionTests::DoHandleCollectionTest0201L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgRefreshStart &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgRefreshStart; drive=EDriveE"));
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    }
    
void CCollectionTests::DoHandleCollectionTest0202L( const TMPXCollectionMessage& aMessage )
    {
    if(aMessage.Event() == TMPXCollectionMessage::EBroadcastEvent && 
       aMessage.Type() == EMcMsgRefreshEnd &&
       aMessage.Data() == EDriveE )
        {
        iLogger->Log( _L("Broadcast Event message recieved: EMcMsgRefreshEnd; drive=EDriveE"));
                
        RunNext();
        }
    else
        {
        iLogger->Log( KTextImproperMsg );
        TMC_ASSERT(EFalse, 1);
        }
    } 

// ---------------------------------------------------------------------------
// Handle collection message
// ---------------------------------------------------------------------------
//
void CCollectionTests::HandleCollectionMessageL(const CMPXMessage& aMsg)
    {
    iLogger->Log(_L("+CCollectionTests::HandleCollectionMessageL"));
    iLogger->Log( KTextHandleColMsgL2 );
    
    // Is this right???	*AIP*
    if( aMsg.IsSupported(KMPXMessageCollectionId) &&
    	aMsg.IsSupported(KMPXMessageChangeEventType))
    	{
        //TMPXCollectionMessage::TEvent event = 
        //*aMsg.Value<TMPXCollectionMessage::TEvent>( KMPXMessageCollectionId );
        //TInt type = *aMsg.Value<TInt>( KMPXMessageChangeEventType );
        //TInt data = *aMsg.Value<TInt>( KMPXMessageGeneralData );
        
        TMPXCollectionMessage::TEvent event = TMPXCollectionMessage::EItemChanged;
        TInt type = *aMsg.Value<TInt>( KMPXMessageChangeEventType );
        TInt data = *aMsg.Value<TInt>( KMPXMessageMediaGeneralId );
        
        TMPXCollectionMessage message(event, type, data);
        switch( iTestIndex )
        	{
            case 78: //ETCFwCollectTest0078:
            case 81: //ETCFwCollectTest0081:
                DoHandleCollectionTest0078L( message );
                break;
            case 79: //ETCFwCollectTest0079:
                DoHandleCollectionTest0079L( message );
                break;
            case 80: //ETCFwCollectTest0080:
                DoHandleCollectionTest0080L( message );
                break;
            case 99: //ETCFwCollectTest0099:
                DoHandleCollectionTest0099L( message );
                break;
            case 108: //ETCFwCollectTest0108:
                DoHandleCollectionTest0108L( message );
                break;
            case 109: //ETCFwCollectTest0109:
                DoHandleCollectionTest0109L( message );
                break;
            default:
                TestPanic( ETestCommonTestNotSupport );                	
        	};
        	
       	return;
    	}
    	
    if( aMsg.IsSupported(KMPXMessageGeneralEvent) &&
        aMsg.IsSupported(KMPXMessageGeneralType) ) 
        {
        TMPXCollectionMessage::TEvent event = 
        *aMsg.Value<TMPXCollectionMessage::TEvent>( KMPXMessageGeneralEvent );
        TInt type = *aMsg.Value<TInt>( KMPXMessageGeneralType );
        TInt data = *aMsg.Value<TInt>( KMPXMessageGeneralData );
        TMPXCollectionMessage message(event, type, data);
        switch( iTestIndex )
            {
            case 1: //ETCFwCollectTest0001:
    		case 21:
            case 23:
                DoHandleCollectionTest0001L( message );
                break;
            case 11: //ETCFwCollectTest0011:
            case 13: //ETCFwCollectTest0013:
            case 15: //ETCFwCollectTest0015:
            case 17: //ETCFwCollectTest0017:
            case 19: //ETCFwCollectTest0019:
            case 20:
                DoHandleCollectionTest0011L( message );
                break;
            case 22: //ETCFwCollectTest0022:
                DoHandleCollectionTest0022L( message );
                break;
            case 29: //ETCFwCollectTest0029:
                DoHandleCollectionTest0029L( message );
                break;
            case 30: //ETCFwCollectTest0030:
                DoHandleCollectionTest0030L( message );
                break;
            case 32: //ETCFwCollectTest0032:
                DoHandleCollectionTest0032L( message );
                break;
            case 34: //ETCFwCollectTest0034:
                DoHandleCollectionTest0034L( message );
                break;
            case 36: //ETCFwCollectTest0036:
                DoHandleCollectionTest0036L( message );
                break;
            case 37: //ETCFwCollectTest0037:
                DoHandleCollectionTest0037L( message );
                break;
            case 39: //ETCFwCollectTest0039:
                DoHandleCollectionTest0039L( message );
                break;
            case 40: //ETCFwCollectTest0040:
                DoHandleCollectionTest0040L( message );
                break;
            case 43: //ETCFwCollectTest0043:
                DoHandleCollectionTest0043L( message );
                break;
            case 47: //ETCFwCollectTest0047:
                DoHandleCollectionTest0047L( message );
                break;
            case 51: //ETCFwCollectTest0051:
                DoHandleCollectionTest0051L( message );
                break;
            case 56: //ETCFwCollectTest0056:
                DoHandleCollectionTest0056L( message );
                break;
            case 57: //ETCFwCollectTest0057:
                DoHandleCollectionTest0057L( message );
                break;
            case 58: //ETCFwCollectTest0058:
                DoHandleCollectionTest0058L( message );
                break;
            case 59: //ETCFwCollectTest0059:
                DoHandleCollectionTest0059L( message );
                break;
            case 60: //ETCFwCollectTest0060:
                DoHandleCollectionTest0060L( message );
                break;
                /* *AIP*
            case 78: //ETCFwCollectTest0078:
            case 81: //ETCFwCollectTest0081:
                DoHandleCollectionTest0078L( message );
                break;
            case 79: //ETCFwCollectTest0079:
                DoHandleCollectionTest0079L( message );
                break;
            case 80: //ETCFwCollectTest0080:
                DoHandleCollectionTest0080L( message );
                break;
                */
            case 84: //ETCFwCollectTest0084:
                DoHandleCollectionTest0084L( message );
                break;
            case 85: //ETCFwCollectTest0085:
                DoHandleCollectionTest0085L( message );
                break;
            case 86: //ETCFwCollectTest0086:
            case 87: //ETCFwCollectTest0087:
                DoHandleCollectionTest0086L( message );
                break;
                /*                
            case 99: //ETCFwCollectTest0099:
                DoHandleCollectionTest0099L( message );
                break;
                */
            case 100: //ETCFwCollectTest0100:
                DoHandleCollectionTest0100L( message );
                break;
            case 108: //ETCFwCollectTest0108:
                DoHandleCollectionTest0108L( message );
                break;
            case 109: //ETCFwCollectTest0109:
                DoHandleCollectionTest0109L( message );
                break;
            case 117: //ETCFwCollectTest0117:
                DoHandleCollectionTest0117L( message );
                break;
            case 118: //ETCFwCollectTest0118:
                DoHandleCollectionTest0118L( message );
                break;
            case 119: //ETCFwCollectTest0119:
                DoHandleCollectionTest0119L( message );
                break;
            case 120: //ETCFwCollectTest0120:
                DoHandleCollectionTest0120L( message );
                break;
            case 126: //ETCFwCollectTest0126:
                DoHandleCollectionTest0126L( message );
                break;
            case 127: //ETCFwCollectTest0127:
                DoHandleCollectionTest0127L( message );
                break;
            case 130: //ETCFwCollectTest0130:
                DoHandleCollectionTest0130L( message );
                break;
            case 145: //ETCFwCollectTest0145:
                DoHandleCollectionTest0145L( message );
                break;
            case 148: //ETCFwCollectTest0148:
                DoHandleCollectionTest0148L( message );
                break;
            case 162: //ETCFwCollectTest0162:
                DoHandleCollectionTest0162L( message );
                break;
            case 163: //ETCFwCollectTest0163:
                DoHandleCollectionTest0163L( message );
                break;
            case 171: //ETCFwCollectTest0171:
                DoHandleCollectionTest0171L( message );
                break;
            case 173: //ETCFwCollectTest0173:
                DoHandleCollectionTest0173L( message );
                break;
            case 177: //ETCFwCollectTest0177:
                DoHandleCollectionTest0177L( message );
                break;
            case 179: //ETCFwCollectTest0179:
                DoHandleCollectionTest0179L( message );
                break;
            case 182: //ETCFwCollectTest0182:
                DoHandleCollectionTest0182L( message );
                break;
            case 194: //ETCFwCollectTest0194:
                DoHandleCollectionTest0194L( message );
                break;
            case 195: //ETCFwCollectTest0195:
                DoHandleCollectionTest0195L( message );
                break;
            case 196: //ETCFwCollectTest0196:
                DoHandleCollectionTest0196L( message );
                break;
            case 197: //ETCFwCollectTest0197:
                DoHandleCollectionTest0197L( message );
                break;
            case 198: //ETCFwCollectTest0198:
                DoHandleCollectionTest0198L( message );
                break;
            case 199: //ETCFwCollectTest0199:
                DoHandleCollectionTest0199L( message );
                break;
            case 200: //ETCFwCollectTest0200:
                DoHandleCollectionTest0200L( message );
                break;
            case 201: //ETCFwCollectTest0201:
                DoHandleCollectionTest0201L( message );
                break;
            case 202: //ETCFwCollectTest0202:
                DoHandleCollectionTest0202L( message );
                break;        
            default:
                TestPanic( ETestCommonTestNotSupport );
            };
        }
    }

// ---------------------------------------------------------------------------
//  Handles the collection entries being opened. Typically called
//  when client has Open()'d a folder
// ---------------------------------------------------------------------------
//
void CCollectionTests::HandleOpenL(const CMPXMedia& aEntries,
                                  TInt aIndex,TBool aComplete,TInt aError)
    {
    iLogger->Log(_L("+CCollectionTests::HandleOpenL"));    
    iLogger->Log(KTextHandleOpenL1);
    iLogger->Log(KTextHandleOpenLArg, aIndex, aComplete, aError);
        
    switch( iTestIndex )
        {
        case 1: //ETCFwCollectTest0001:
   			RunNext();  // skip the handling of callback   			
            break;
        case 11: //ETCFwCollectTest0011:
        case 13: //ETCFwCollectTest0013:
        case 15: //ETCFwCollectTest0015:
        case 17: //ETCFwCollectTest0017:
        case 19: //ETCFwCollectTest0019:
            DoHandleCollectionTest0012L(aEntries, aIndex, aComplete, aError );
            break;
        case 21: //ETCFwCollectTest0021:
        case 22:
            DoHandleCollectionTest0021L(aEntries, aIndex, aComplete, aError );
            break;
        case 23: //ETCFwCollectTest0023:
        case 24: //ETCFwCollectTest0024:
        case 25: //ETCFwCollectTest0025:
        case 26: //ETCFwCollectTest0026:
        case 27: //ETCFwCollectTest0027:
        case 28: //ETCFwCollectTest0028:
            DoHandleCollectionTest0023L(aEntries, aIndex, aComplete, aError );
            break;
        case 29: //ETCFwCollectTest0029:
            DoHandleCollectionTest0029L(aEntries, aIndex, aComplete, aError );
            break;
        case 31: //ETCFwCollectTest0031:
            DoHandleCollectionTest0031L(aEntries, aIndex, aComplete, aError );
            break;
        case 33: //ETCFwCollectTest0033:
            DoHandleCollectionTest0033L(aEntries, aIndex, aComplete, aError );
            break;
        case 35: //ETCFwCollectTest0035:
            DoHandleCollectionTest0035L(aEntries, aIndex, aComplete, aError );
            break;
        case 36: //ETCFwCollectTest0036:
            DoHandleCollectionTest0036L(aEntries, aIndex, aComplete, aError );
            break;
        case 38: //ETCFwCollectTest0038:
            DoHandleCollectionTest0038L(aEntries, aIndex, aComplete, aError );
            break;
        case 39: //ETCFwCollectTest0039:
            DoHandleCollectionTest0039L(aEntries, aIndex, aComplete, aError );
            break;
        case 41: //ETCFwCollectTest0041:
            DoHandleCollectionTest0041L(aEntries, aIndex, aComplete, aError );
            break;
        case 44: //ETCFwCollectTest0044:
            DoHandleCollectionTest0044L(aEntries, aIndex, aComplete, aError );
            break;
        case 48: //ETCFwCollectTest0048:
            DoHandleCollectionTest0048L(aEntries, aIndex, aComplete, aError );
            break;
        case 52: //ETCFwCollectTest0052:
            DoHandleCollectionTest0052L(aEntries, aIndex, aComplete, aError );
            break;
        case 59: //ETCFwCollectTest0059:
            DoHandleCollectionTest0059L(aEntries, aIndex, aComplete, aError );
            break;
        case 60: //ETCFwCollectTest0060:
            DoHandleCollectionTest0060L(aEntries, aIndex, aComplete, aError );
            break;
        default:
            TestPanic( ETestCommonTestNotSupport );
        };
    }
                     
// ---------------------------------------------------------------------------
// Handles the item being opened. Typically called
// when client has Open()'d an item. Client typically responds by
// 'playing' the item via the playlist
// ---------------------------------------------------------------------------
void CCollectionTests::HandleOpenL(const CMPXCollectionPlaylist& /*aPlaylist*/,TInt aError)
    {
    iLogger->Log(KTextHandleOpenL2);
    iLogger->Log(_L("~HandleOpenL executed with error: %d"), aError);
    switch( iTestIndex )
        {
        default:
            TestPanic( ETestCommonTestNotSupport );
        };
    }

// ---------------------------------------------------------------------------
// Handle completion of a asynchronous command
// ---------------------------------------------------------------------------
void CCollectionTests::HandleCommandComplete(CMPXCommand* aCommandResult, TInt aError)
    {
    iLogger->Log(KTextHandleCommandComplete);
    iLogger->Log(_L("~HandleCommandComplete executed with error: %d; aCmdResult: 0x%x"), aError, aCommandResult);
    switch( iTestIndex )
        {
        case 171: //ETCFwCollectTest0171:
            DoHandleCommandCompleteTest0171L(aCommandResult, aError);
            break;
        case 173: //ETCFwCollectTest0173:
            DoHandleCommandCompleteTest0173L(aCommandResult, aError);
            break;
        case 176: //ETCFwCollectTest0176:
            DoHandleCommandCompleteTest0176L(aCommandResult, aError);
            break;
        case 177: //ETCFwCollectTest0177:
            DoHandleCommandCompleteTest0177L(aCommandResult, aError);
            break;
        case 184: //ETCFwCollectTest0184:
            DoHandleCommandCompleteTest0184L(aCommandResult, aError);
            break;
        default:
            TestPanic( ETestCommonTestNotSupport );
        };
    }
    
// ---------------------------------------------------------------------------
//  Handle extended media properties
// ---------------------------------------------------------------------------
//
void CCollectionTests::HandleCollectionMediaL(const CMPXMedia& aMedia, 
                                             TInt aError)
    {
    iLogger->Log(KTextHandleMediaL);
    iLogger->Log(_L("~HandleCollectionMediaL executed with error: %d"), aError);
    switch( iTestIndex )
        {
        case 145: //ETCFwCollectTest0145:
            DoHandleMediaTest0145L(aMedia, aError);
            break;
        case 146: //ETCFwCollectTest0146:
            DoHandleMediaTest0146L(aMedia, aError);
            break;
        case 147: //ETCFwCollectTest0147:
            DoHandleMediaTest0147L(aMedia, aError);
            break;
        case 148: //ETCFwCollectTest0148:
            DoHandleMediaTest0148L(aMedia, aError);
            break;
        case 149: //ETCFwCollectTest0149:
            DoHandleMediaTest0149L(aMedia, aError);
            break;
        default:
            TestPanic( ETestCommonTestNotSupport );
        };
    }

// ---------------------------------------------------------------------------
//  Handles removing a collection path
// ---------------------------------------------------------------------------
//
void CCollectionTests::HandleRemoveL(MDesCArray& aUriArray, TInt aError)
    {
    // Log parameter
    iLogger->Log( KTextHandleRemoveL );
    iLogger->Log(_L("~HandleRemoveL executed with error: %d"), aError);
    if( !aError )
        {
        for(TInt i = 0; i < aUriArray.MdcaCount(); i++)
            {
            TPtrC pt( aUriArray.MdcaPoint(i) );
            iLogger->Log(_L("~File [%d]\"%S\""), i, &pt );
            }
        }
    switch( iTestIndex )
        {
        case 88: //ETCFwCollectTest0088:
            DoHandleRemoveLTest0088L(aUriArray, aError);
            break;
        case 89: //ETCFwCollectTest0089:
            DoHandleRemoveLTest0089L(aUriArray, aError);
            break;
        case 90: //ETCFwCollectTest0090:
            DoHandleRemoveLTest0090L(aUriArray, aError);
            break;
        case 91: //ETCFwCollectTest0091:
            DoHandleRemoveLTest0091L(aUriArray, aError);
            break;
        default:
            TestPanic( ETestCommonTestNotSupport );
        };
    }

// ---------------------------------------------------------------------------
//  Handle callback for "find" operation
// ---------------------------------------------------------------------------
//
void CCollectionTests::HandleFindAllL(const CMPXMedia& aResults, TBool aComplete,
                                     TInt aError)
    {
    iLogger->Log( KTextHandleFindAllL );
    iLogger->Log( _L("~HandleFindAllL executed with error: %d, complete: %d"), aError, aComplete );
    switch( iTestIndex )
        {
        case 126: //ETCFwCollectTest0126:
            DoHandleFindAllTest0126L(aResults, aComplete, aError);
            break;
        case 127: //ETCFwCollectTest0127:
            DoHandleFindAllTest0127L(aResults, aComplete, aError);
            break;
        case 128: //ETCFwCollectTest0128:
            DoHandleFindAllTest0128L(aResults, aComplete, aError);
            break;
        case 129: //ETCFwCollectTest0129:
            DoHandleFindAllTest0129L(aResults, aComplete, aError);
            break;
        case 130: //ETCFwCollectTest0130:
            DoHandleFindAllTest0130L(aResults, aComplete, aError);
            break;
        default:
            TestPanic( ETestCommonTestNotSupport );
        };
    }
   
// -----------------------------------------------------------------------------
// Assert Utility
// -----------------------------------------------------------------------------	
void CCollectionTests::AssertL(TBool aCondition, TInt aCheckPt)
    {
    if( !aCondition )
        {
        iLogger->Log(KTestModuleCheckptFail, aCheckPt);
        iLatestTestResult = KErrGeneral;
        TestCompleteL(); // stop on failure
        }
    }
    
// -----------------------------------------------------------------------------
// Compare Utility
// -----------------------------------------------------------------------------
TBool CCollectionTests::CompareByTMPXItemId(const TMPXItemId& aFirst, 
                                                const TMPXItemId& aSecond)
    {
    // Compare the id
    return aFirst == (TInt)aSecond;
    }

// -----------------------------------------------------------------------------	
// Initialize new collection
// -----------------------------------------------------------------------------
void CCollectionTests::Collection1_Initialize()
	{
    DELETE_COLLECTION_PTR(iCollectionUtility1);
    
    iCollectionUtility1 = MMPXCollectionUtility::NewL(this, KMcModeDefault);
    }
    	
// -----------------------------------------------------------------------------    	
// Initialize new collection and open with TMPXOpenMode
// -----------------------------------------------------------------------------
void CCollectionTests::Collection2_InitializeAndOpenL(TMPXOpenMode aOpenMode)
    {
    iTempCnt = 0;
    
    DELETE_COLLECTION_PTR(iCollectionUtility2);

    // Prevent iCollectionUtility2 from using same context as iCollectionUtility1
    iCollectionUtility2 = MMPXCollectionUtility::NewL(this, KMcModeIsolated);
    iCollectionUtility2->Collection().OpenL( aOpenMode );
    }
    
