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
* Description:  MpxCollectionTest class member functions
*
*/

// INCLUDE FILES
#include <e32def.h>
#include <bamdesca.h>
#include <mpxcollectionutility.h>
#include <mpxmedia.h>
#include <mpxcollectionplaylist.h>
#include <mpxharvesterutility.h>
#include <mpxcollectionmessage.h>
#include <mpxcommonframeworkdefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxcollectionpath.h>
#include <mpxattribute.h>
#include <stiflogger.h>
#include "consolemain.h"
#include "mpxcollectiontest.h"
#include "collectiontests.h"

_LIT(KTestFailed, "Failed");
_LIT(KTestPassed, "Passed");


// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMpxCollectionTest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMpxCollectionTest* CMpxCollectionTest::NewL()
    {
    CMpxCollectionTest* self = new (ELeave) CMpxCollectionTest;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CMpxCollectionTest::CMpxCollectionTest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMpxCollectionTest::CMpxCollectionTest()
	: iMainConsole(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CMpxCollectionTest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMpxCollectionTest::ConstructL()
    {
    iScheduler = new(ELeave) CActiveScheduler;
    CActiveScheduler::Install(iScheduler);

    iLogger = CStifLogger::NewL( _L("c:\\"), 
                                 _L("MpxCollectionTest.html"), 
                                 CStifLogger::EHtml,
                                 CStifLogger::EFile,
                                 ETrue,
                                 ETrue,
                                 ETrue,
                                 EFalse,
                                 ETrue,
                                 ETrue);
    
    iLogger->Log(_L("===MpxCollectionTest constructed==="));
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CMpxCollectionTest::~CMpxCollectionTest()
    {
    if(iMainConsole)
        {
    	delete iMainConsole;
    	iMainConsole = NULL;
    	}
    if(iLogger)
    	{
    	delete iLogger;
    	iLogger = NULL;
    	}
    if(iScheduler)
    	{
    	delete iScheduler;
    	iScheduler = NULL;	
    	}
    }
     
// -----------------------------------------------------------------------------
// CMpxCollectionTest::InitL
// InitL is used to initialize the Test Module.
// -----------------------------------------------------------------------------
//
TInt CMpxCollectionTest::InitL( 
    TFileName& /*aIniFile*/, 
    TBool /*aFirstTime*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMpxCollectionTest::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be 
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation. 
// -----------------------------------------------------------------------------
//
TInt CMpxCollectionTest::GetTestCasesL( 
    const TFileName& /*aConfig*/, 
    RPointerArray<TTestCaseInfo>& aTestCases )
    {
    
    // Loop through all test cases and create new
    // TTestCaseInfo items and append items to aTestCase array    
    for( TInt i = 0; Case(i).iMethod != NULL; i++ )
        {
 
        // Allocate new TTestCaseInfo from heap for a testcase definition.
        TTestCaseInfo* newCase = new( ELeave ) TTestCaseInfo();
    
        // PushL TTestCaseInfo to CleanupStack.    
        CleanupStack::PushL( newCase );

        // Set number for the testcase.
        // When the testcase is run, this comes as a parameter to RunTestCaseL.
        newCase->iCaseNumber = i;

        // Set title for the test case. This is shown in UI to user.
        newCase->iTitle.Copy( Case(i).iCaseName );

        // Append TTestCaseInfo to the testcase array. After appended 
        // successfully the TTestCaseInfo object is owned (and freed) 
        // by the TestServer. 
        User::LeaveIfError(aTestCases.Append ( newCase ) );
        
        // Pop TTestCaseInfo from the CleanupStack.
        CleanupStack::Pop( newCase );
        }
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CMpxCollectionTest::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified 
// by aTestCase. Test cases that can be run may be requested from 
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
TInt CMpxCollectionTest::RunTestCaseL( 
    const TInt aCaseNumber,   
    const TFileName& /*aConfig*/,                 
    TTestResult& aResult )
    {
    
    // Return value
    TInt execStatus = KErrNone;

    // Get the pointer to test case function
    TCaseInfo tmp = Case ( aCaseNumber );

    // Check that case number was valid
    if ( tmp.iMethod != NULL )
        {
        iLogger->Log( _L( "Running test for: %S" ), &(tmp.iCaseName) );
        
        // Valid case was found, call it via function pointer
        iMethod = tmp.iMethod;        
        execStatus  = ( this->*iMethod )( aResult );                  
        
        // Write log information
        if ( aResult.iResult == KErrNone )
            {
            iLogger->Log( _L( "Result: %S" ), &KTestPassed );
            iLogger->Log( _L( "" ) );
            }
        else
            {
            iLogger->Log( _L( "Result: %S" ), &KTestFailed );
            iLogger->Log( _L( "Description: %S" ), &(aResult.iResultDes) );
            iLogger->Log( _L( "" ) );
            }
        }
    else
        {        
        // Valid case was not found, return error.
        execStatus = KErrNotFound;
        }

    // Return case execution status (not the result of the case execution)
    return execStatus;
    
    }
    
// -----------------------------------------------------------------------------
// CMpxCollectionTest::Case
// Returns a test case by number.
//
// This function contains an array of all available test cases 
// i.e pair of case name and test function. If case specified by parameter
// aCaseNumber is found from array, then that item is returned.
// 
// The reason for this rather complicated function is to specify all the
// test cases only in one place. It is not necessary to understand how
// function pointers to class member functions works when adding new test
// cases. See function body for instructions how to add new test case.
// -----------------------------------------------------------------------------
//
const TCaseInfo CMpxCollectionTest::Case ( 
    const TInt aCaseNumber ) const 
    {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    MpxCollectionTest.cpp file and to MpxCollectionTest.h 
    *    header file.
    * 2) Add entry to following KCases array either by using FUNCENTRY or 
    *    ENTRY macro. 
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    * FUNCENTRY macro takes only test case function name as a parameter and uses
    * that as a test case name and test case function name.
    */
    
    static TCaseInfoInternal const KCases[] =
        {
        // To add new test cases, add new items to this array
  	    ENTRY( "Test0001-OpenL() Collection 1 - NewL Default, OpenL Default + case 11", Test0001L),
	    //ENTRY( "Test0002-OpenL()   blank", Test0002L),
	    //ENTRY( "Test0003-OpenL()   blank", Test0003L),
	    //ENTRY( "Test0004-OpenL()   blank", Test0004L),
	    //ENTRY( "Test0005-OpenL()   blank", Test0005L),
	    //ENTRY( "Test0006-OpenL()   blank", Test0006L),
	    //ENTRY( "Test0007-OpenL()   blank", Test0007L),
	    //ENTRY( "Test0008-OpenL()   blank", Test0008L),
	    //ENTRY( "Test0009-OpenL()   blank", Test0009L),
	    //ENTRY( "Test0010-OpenL()   blank", Test0010L),
  	    ENTRY( "Test0011-OpenL() Collection 2 - NewL(Isolated), OpenL(Default)", Test0011L),
	    //ENTRY( "Test0012-OpenL() blank", Test0012L),
	    ENTRY( "Test0013-OpenL() NewL(Isolated), OpenL(EMPXOpenGroupOrPlaylist)", Test0013L),
	    //ENTRY( "Test0014-OpenL() blank", Test0014L),
	    ENTRY( "Test0015-OpenL() NewL(Isolated), OpenL(EMPXOpenAllItems)", Test0015L),
	    //ENTRY( "Test0016-OpenL() blank", Test0016L),
	    ENTRY( "Test0017-OpenL() NewL(Isolated), OpenL(EMPXOpenNoPlaylist)", Test0017L),
	    //ENTRY( "Test0018-OpenL() blank", Test0018L),
	    ENTRY( "Test0019-OpenL() Collection 2 - NewL(Isolated), OpenL(EMPXOpenPlaylistOnly)", Test0019L),
	    ENTRY( "Test0020-OpenL() Collection 2 - Open(), Close()", Test0020L),
  	    ENTRY( "Test0021-OpenL() NewL Default, OpenL(EMPXOpenDefault)", Test0021L),
	    //ENTRY( "Test0022-OpenL()   NewL Default, OpenL(KColTestPluginNormIndex,EMPXOpenDefault)", Test0022L),
	    //ENTRY( "Test0023-OpenL()   same as 21", Test0023L),
	    //ENTRY( "Test0024-OpenL()   NewL Default, OpenL(EMPXOpenGroupOrPlaylist)", Test0024L),
	    //ENTRY( "Test0025-OpenL()   NewL Default, OpenL(EMPXOpenAllItems)", Test0025L),
	    //ENTRY( "Test0026-OpenL()   NewL Default, OpenL(EMPXOpenNoPlaylist)", Test0026L),
	    //ENTRY( "Test0027-OpenL()   NewL Default, OpenL(EMPXOpenPlaylistOnly)", Test0027L),
	    //ENTRY( "Test0028-OpenL()   same as 21", Test0028L),
	    //ENTRY( "Test0029-OpenL()   NewL Default, OpenL(0,EMPXOpenDefault)", Test0029L),
	    //ENTRY( "Test0030-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0030L),
  	    //ENTRY( "Test0031-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0031L),
	    //ENTRY( "Test0032-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0032L),
	    //ENTRY( "Test0033-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0033L),
	    //ENTRY( "Test0034-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0034L),
	    //ENTRY( "Test0035-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0035L),
	    ENTRY( "Test0036-OpenL() NewL Default, OpenL(EMPXOpenDefault)", Test0036L),
	    //ENTRY( "Test0037-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0037L),
	    //ENTRY( "Test0038-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0038L),
	    //ENTRY( "Test0039-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0039L),
	    //ENTRY( "Test0040-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0040L),
  	    //ENTRY( "Test0041-OpenL()   NewL Default, OpenL(EMPXOpenDefault)", Test0041L),
	    ENTRY( "Test0042-Filters,Path,UID", Test0042L),
	    //ENTRY( "Test0043-		   Filters,Path,UID", Test0043L),
	    //ENTRY( "Test0044-		   Filters,Path,UID", Test0044L),
	    //ENTRY( "Test0045-		   Filters,Path,UID", Test0045L),
	    //ENTRY( "Test0046-		   Filters,Path,UID", Test0046L),
	    //ENTRY( "Test0047-		   Filters,Path,UID", Test0047L),
	    //ENTRY( "Test0048-		   Filters,Path,UID", Test0048L),
	    //ENTRY( "Test0049-		   Filters,Path,UID", Test0049L),
	    //ENTRY( "Test0050-		   Filters,Path,UID", Test0050L),
  	    //ENTRY( "Test0051-		   Filters,Path,UID", Test0051L),
	    //ENTRY( "Test0052-		   Filters,Path,UID", Test0052L),
	    //ENTRY( "Test0053-		   Filters,Path,UID", Test0053L),
	    ENTRY( "Test0054-Filters,Path,UID", Test0054L),
	    //ENTRY( "Test0055-		   PathL()", Test0055L),
	    //ENTRY( "Test0056-		   PathL()", Test0056L),
	    //ENTRY( "Test0057-		   PathL()", Test0057L),
	    //ENTRY( "Test0058-		   PathL()", Test0058L),
	    //ENTRY( "Test0059-		   Back()", Test0059L),
	    //ENTRY( "Test0060-		   Back()", Test0060L),
  	    //ENTRY( "Test0061-		   Back()", Test0061L),
	    //ENTRY( "Test0062-		   Back()", Test0062L),
	    //ENTRY( "Test0063-		   Back()", Test0063L),
	    //ENTRY( "Test0064-		   Empty", Test0064L),
	    //ENTRY( "Test0065-		   Empty", Test0065L),
	    //ENTRY( "Test0066-		   Empty", Test0066L),
	    //ENTRY( "Test0067-		   Empty", Test0067L),
	    //ENTRY( "Test0068-		   Empty", Test0068L),
	    //ENTRY( "Test0069-		   Empty", Test0069L),
	    ENTRY( "Test0070-AddL()", Test0070L),
  	    //ENTRY( "Test0071-		   AddL()", Test0071L),
	    //ENTRY( "Test0072-		   AddL()", Test0072L),
	    //ENTRY( "Test0073-		   AddL()", Test0073L),
	    //ENTRY( "Test0074-		   AddL()", Test0074L),
	    //ENTRY( "Test0075-		   AddL()", Test0075L),
	    //ENTRY( "Test0076-		   AddL()", Test0076L),
	    //ENTRY( "Test0077-		   AddL()", Test0077L),
	    //ENTRY( "Test0078-		   AddL()", Test0078L),
	    //ENTRY( "Test0079-		   AddL()", Test0079L),
	    //ENTRY( "Test0080-		   AddL()", Test0080L),
  	    //ENTRY( "Test0081-		   AddL()", Test0081L),
	    ENTRY( "Test0082-RemoveL()", Test0082L),
	    //ENTRY( "Test0083-		   RemoveL()", Test0083L),
	    //ENTRY( "Test0084-		   RemoveL()", Test0084L),
	    //ENTRY( "Test0085-		   RemoveL()", Test0085L),
	    //ENTRY( "Test0086-		   RemoveL()", Test0086L),
	    //ENTRY( "Test0087-		   RemoveL()", Test0087L),
	    //ENTRY( "Test0088-		   RemoveL()", Test0088L),
	    //ENTRY( "Test0089-		   RemoveL()", Test0089L),
	    //ENTRY( "Test0090-		   RemoveL()", Test0090L),
  	    //ENTRY( "Test0091-		   RemoveL()", Test0091L),
	    //ENTRY( "Test0092-		   RemoveL()", Test0092L),
	    //ENTRY( "Test0093-		   RemoveL()", Test0093L),
	    //ENTRY( "Test0094-		   RemoveL()", Test0094L),
	    //ENTRY( "Test0095-		   RemoveL()", Test0095L),
	    //ENTRY( "Test0096-		   RemoveL()", Test0096L),
	    //ENTRY( "Test0097-		   RemoveL()", Test0097L),
	    //ENTRY( "Test0098-		   RemoveL()", Test0098L),
	    //ENTRY( "Test0099-		   RemoveL()", Test0099L),
	    //ENTRY( "Test0100-		   RemoveL()", Test0100L),
  	    ENTRY( "Test0101-SetSync()", Test0101L),
	    //ENTRY( "Test0102-		   SetSync()", Test0102L),
	    //ENTRY( "Test0103-		   SetSync()", Test0103L),
	    //ENTRY( "Test0104-		   SetSync()", Test0104L),
	    //ENTRY( "Test0105-		   SetSync()", Test0105L),
	    //ENTRY( "Test0106-		   SetSync()", Test0106L),
	    //ENTRY( "Test0107-		   SetSync()", Test0107L),
	    //ENTRY( "Test0108-		   SetSync()", Test0108L),
	    //ENTRY( "Test0109-		   SetSync()", Test0109L),
	    ENTRY( "Test0110-SetL()", Test0110L),
  	    //ENTRY( "Test0111-		   SetL()", Test0111L),
	    //ENTRY( "Test0112-		   SetL()", Test0112L),
	    //ENTRY( "Test0113-		   SetL()", Test0113L),
	    //ENTRY( "Test0114-		   SetL()", Test0114L),
	    //ENTRY( "Test0115-		   SetL()", Test0115L),
	    //ENTRY( "Test0116-		   SetL()", Test0116L),
	    //ENTRY( "Test0117-		   SetL()", Test0117L),
	    //ENTRY( "Test0118-		   SetL()", Test0118L),
	    //ENTRY( "Test0119-		   SetL()", Test0119L),
	    //ENTRY( "Test0120-		   SetL()", Test0120L),
  	    ENTRY( "Test0121-FindAllL()", Test0121L),
	    //ENTRY( "Test0122-	  	   FindAllL()", Test0122L),
	    //ENTRY( "Test0123-		   FindAllL()", Test0123L),
	    //ENTRY( "Test0124-		   FindAllL()", Test0124L),
	    //ENTRY( "Test0125-		   FindAllL()", Test0125L),
	    //ENTRY( "Test0126-		   FindAllL()", Test0126L),
	    //ENTRY( "Test0127-		   FindAllL()", Test0127L),
	    //ENTRY( "Test0128-		   FindAllL()", Test0128L),
	    //ENTRY( "Test0129-		   FindAllL()", Test0129L),
	    //ENTRY( "Test0130-		   FindAllL()", Test0130L),
  	    //ENTRY( "Test0131-		   FindAllL()", Test0131L),
	    //ENTRY( "Test0132-		   FindAllL()", Test0132L),
	    //ENTRY( "Test0133-		   FindAllL()", Test0133L),
	    //ENTRY( "Test0134-		   FindAllL()", Test0134L),
	    //ENTRY( "Test0135-		   FindAllL()", Test0135L),
	    //ENTRY( "Test0136-		   FindAllL()", Test0136L),
	    //ENTRY( "Test0137-		   FindAllL()", Test0137L),
	    //ENTRY( "Test0138-		   FindAllL()", Test0138L),
	    //ENTRY( "Test0139-		   FindAllL()", Test0139L),
	    //ENTRY( "Test0140-		   FindAllL()", Test0140L),
  	    ENTRY( "Test0141-MediaL()", Test0141L),
	    //ENTRY( "Test0142-		   MediaL()", Test0142L),
	    //ENTRY( "Test0143-		   MediaL()", Test0143L),
	    //ENTRY( "Test0144-		   MediaL()", Test0144L),
	    //ENTRY( "Test0145-		   MediaL()", Test0145L),
	    //ENTRY( "Test0146-		   MediaL()", Test0146L),
	    //ENTRY( "Test0147-		   MediaL()", Test0147L),
	    //ENTRY( "Test0148-		   MediaL()", Test0148L),
	    //ENTRY( "Test0149-		   MediaL()", Test0149L),
	    ENTRY( "Test0150-CommandL()", Test0150L),
  	    //ENTRY( "Test0151-		   CommandL()", Test0151L),
	    //ENTRY( "Test0152-		   CommandL()", Test0152L),
	    //ENTRY( "Test0153-		   CommandL()", Test0153L),
	    //ENTRY( "Test0154-		   CommandL()", Test0154L),
	    //ENTRY( "Test0155-		   CommandL()", Test0155L),
	    //ENTRY( "Test0156-		   CommandL()", Test0156L),
	    //ENTRY( "Test0157-		   CommandL()", Test0157L),
	    //ENTRY( "Test0158-		   CommandL()", Test0158L),
	    //ENTRY( "Test0159-		   CommandL()", Test0159L),
	    //ENTRY( "Test0160-		   CommandL()", Test0160L),
  	    //ENTRY( "Test0161-		   CommandL()", Test0161L),
	    //ENTRY( "Test0162-		   CommandL()", Test0162L),
	    //ENTRY( "Test0163-		   CommandL()", Test0163L),
	    //ENTRY( "Test0164-		   CommandL()", Test0164L),
	    //ENTRY( "Test0165-		   CommandL()", Test0165L),
	    //ENTRY( "Test0166-		   CommandL()", Test0166L),
	    //ENTRY( "Test0167-		   CommandL()", Test0167L),
	    //ENTRY( "Test0168-		   CommandL()", Test0168L),
	    //ENTRY( "Test0169-		   CommandL()", Test0169L),
	    ENTRY( "Test0170-CommandL()", Test0170L),
  	    //ENTRY( "Test0171-		   CommandL()", Test0171L),
	    //ENTRY( "Test0172-		   CommandL()", Test0172L),
	    //ENTRY( "Test0173-		   CommandL()", Test0173L),
	    //ENTRY( "Test0174-		   CommandL()", Test0174L),
	    //ENTRY( "Test0175-		   CommandL()", Test0175L),
	    //ENTRY( "Test0176-		   CommandL()", Test0176L),
	    //ENTRY( "Test0177-		   CommandL()", Test0177L),
	    //ENTRY( "Test0178-		   CommandL()", Test0178L),
	    //ENTRY( "Test0179-		   CommandL()", Test0179L),
	    //ENTRY( "Test0180-		   CommandL()", Test0180L),
  	    //ENTRY( "Test0181-		   CommandL()", Test0181L),
	    //ENTRY( "Test0182-		   CommandL()", Test0182L),
	    //ENTRY( "Test0183-		   CommandL()", Test0183L),
	    //ENTRY( "Test0184-		   CommandL()", Test0184L),
	    //ENTRY( "Test0185-GetSupportedTypesL()", Test0185L),
	    //ENTRY( "Test0186-GetSupportedTypesL()", Test0186L),
	    //ENTRY( "Test0187-GetSupportedCapabilitiesL()", Test0187L),
	    //ENTRY( "Test0188-GetSupportedCapabilitiesL()", Test0188L),
	    //ENTRY( "Test0189-GetSupportedCapabilitiesL()", Test0189L),
	    ENTRY( "Test0190-CollectionIDL()", Test0190L),
  	    ENTRY( "Test0191-CollectionIDL()", Test0191L),
	    ENTRY( "Test0192-CollectionIDL()", Test0192L),
	    ENTRY( "Test0193-CollectionIDL()", Test0193L),
	    ENTRY( "Test0194-NotifyL()", Test0194L),
	    //ENTRY( "Test0195-		   NotifyL()", Test0195L),
	    //ENTRY( "Test0196-		   NotifyL()", Test0196L),
	    //ENTRY( "Test0197-		   NotifyL()", Test0197L),
	    //ENTRY( "Test0198-		   NotifyL()", Test0198L),
	    //ENTRY( "Test0199-		   NotifyL()", Test0199L),
	    //ENTRY( "Test0200-		   NotifyL()", Test0200L),
  	    //ENTRY( "Test0201-		   NotifyL()", Test0201L),
	    //ENTRY( "Test0202-		   NotifyL()", Test0202L)
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / 
                               sizeof( TCaseInfoInternal ) )
        {
        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        return null;
        } 

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[ aCaseNumber ].iCaseName );
    tmp.iMethod = KCases[ aCaseNumber ].iMethod;
    return tmp;
    }

// -----------------------------------------------------------------------------
// Initialize environment before test case
// -----------------------------------------------------------------------------
void CMpxCollectionTest::InitializeL(CActiveScheduler* scheduler)
    {
    iMainConsole = CConsoleMain::NewL(scheduler);
    }

// -----------------------------------------------------------------------------
// Cleanup environment after test
// -----------------------------------------------------------------------------
void CMpxCollectionTest::Cleanup()
    {
    if(iMainConsole)
    	{
    	delete iMainConsole;
    	iMainConsole = NULL;
    	}
    }
    
// ================================= TEST CASES ================================
		
TInt CMpxCollectionTest::CommonTestL(TInt testIndex, TTestResult& aResult) 
	{
	__UHEAP_MARK;
	
	InitializeL(iScheduler);  //Initialize environment

    // create and push
    CCollectionTests* collectionTests = CCollectionTests::NewLC(iMainConsole,
                                            NULL,
                                            KTxtCollectionTests,
                                            iLogger,
                                            testIndex);
                                           
	collectionTests->StartNextTestL();
		
	if(!collectionTests->IsTestSynchronous(testIndex))
		{
		TRAPD(err, iMainConsole->ActivateL(collectionTests));

    	// Test passed/failed    
    	if(KErrNone != err)
    	    {
    		aResult.iResult = err;
     	   	}
     	else
    		{
    		aResult.iResult = collectionTests->GetLatestTestResult();
    		}		
		}
	else
		{
		aResult.iResult = collectionTests->GetLatestTestResult();
		}

    aResult.iResultDes = KTxtCollectionTests;
    
    CleanupStack::PopAndDestroy(collectionTests);      
        	
	Cleanup();  //Cleanup environment
    		
    //iCollectionUtility may take a short time to Close()
    User::After(1000000); // 1 second
    
    __UHEAP_MARKEND;
    iLogger->Log( KTxtCollectionTests);
        
    return aResult.iResult;	
	}

// =============================== HELP FUNCTIONS ==============================


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CTestModuleBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CTestModuleBase* LibEntryL()
    {
    return CMpxCollectionTest::NewL();
    }

// -----------------------------------------------------------------------------
// SetRequirements handles test module parameters(implements evolution
// version 1 for test module's heap and stack sizes configuring).
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt SetRequirements( CTestModuleParam*& /*aTestModuleParam*/, 
                                TUint32& /*aParameterValid*/ )
    {

    /* --------------------------------- NOTE ---------------------------------
    USER PANICS occurs in test thread creation when:
    1) "The panic occurs when the value of the stack size is negative."
    2) "The panic occurs if the minimum heap size specified is less
       than KMinHeapSize".
       KMinHeapSize: "Functions that require a new heap to be allocated will
       either panic, or will reset the required heap size to this value if
       a smaller heap size is specified".
    3) "The panic occurs if the minimum heap size specified is greater than
       the maximum size to which the heap can grow".
    Other:
    1) Make sure that your hardware or Symbian OS is supporting given sizes.
       e.g. Hardware might support only sizes that are divisible by four.
    ------------------------------- NOTE end ------------------------------- */

    // Normally STIF uses default heap and stack sizes for test thread, see:
    // KTestThreadMinHeap, KTestThreadMinHeap and KStackSize.
    // If needed heap and stack sizes can be configured here by user. Remove
    // comments and define sizes.

/*
    aParameterValid = KStifTestModuleParameterChanged;

    CTestModuleParamVer01* param = CTestModuleParamVer01::NewL();
    // Stack size
    param->iTestThreadStackSize= 16384; // 16K stack
    // Heap sizes
    param->iTestThreadMinHeap = 4096;   // 4K heap min
    param->iTestThreadMaxHeap = 1048576;// 1M heap max

    aTestModuleParam = param;
*/
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function
// Returns: KErrNone: No error
// -----------------------------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) // Reason
    {
    return(KErrNone);
    }
#endif
//  End of File
