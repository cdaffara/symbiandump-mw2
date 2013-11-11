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
* Description:  Implementation of CImageHandlerTests class.
*
*/



// INCLUDE FILES
#include <e32math.h>
#include <cphcltimagehandler.h>
#include <cphcltbaseimageparams.h>
#include <fbs.h>

#include "ImageHandlerTests.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CImageHandlerTests::Case
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
const TCaseInfo CImageHandlerTests::Case ( 
    const TInt aCaseNumber ) const 
     {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    ImageHandlerTests.cpp file and to ImageHandlerTests.h 
    *    header file.
    *
    * 2) Add entry to following KCases array either by using:
    *
    * 2.1: FUNCENTRY or ENTRY macro
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    *
    * FUNCENTRY macro takes only test case function name as a parameter and
    * uses that as a test case name and test case function name.
    *
    * Or
    *
    * 2.2: OOM_FUNCENTRY or OOM_ENTRY macro. Note that these macros are used
    * only with OOM (Out-Of-Memory) testing!
    *
    * OOM_ENTRY macro takes five parameters: test case name, test case 
    * function name, TBool which specifies is method supposed to be run using
    * OOM conditions, TInt value for first heap memory allocation failure and 
    * TInt value for last heap memory allocation failure.
    * 
    * OOM_FUNCENTRY macro takes test case function name as a parameter and uses
    * that as a test case name, TBool which specifies is method supposed to be
    * run using OOM conditions, TInt value for first heap memory allocation 
    * failure and TInt value for last heap memory allocation failure. 
    */ 

    static TCaseInfoInternal const KCases[] =
        {
        // To add new test cases, add new items to this array
        
        // NOTE: When compiled to GCCE, there must be Classname::
        // declaration in front of the method name, e.g. 
        // CImageHandlerTests::PrintTest. Otherwise the compiler
        // gives errors.
        
        FUNCENTRY( CImageHandlerTests::ConstructionTest ),
        FUNCENTRY( CImageHandlerTests::TestImageParamsL ),
        FUNCENTRY( CImageHandlerTests::SaveAndLoadImageTestL ),
        // Example how to use OOM functionality
        //OOM_ENTRY( "Loop test with OOM", CImageHandlerTests::LoopTest, ETrue, 2, 3),
        //OOM_FUNCENTRY( CImageHandlerTests::PrintTest, ETrue, 1, 3 ),
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / 
                               sizeof( TCaseInfoInternal ) )
        {
        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        null.iIsOOMTest = EFalse;
        null.iFirstMemoryAllocation = 0;
        null.iLastMemoryAllocation = 0;
        return null;
        } 

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[ aCaseNumber ].iCaseName );
    tmp.iMethod = KCases[ aCaseNumber ].iMethod;
    tmp.iIsOOMTest = KCases[ aCaseNumber ].iIsOOMTest;
    tmp.iFirstMemoryAllocation = KCases[ aCaseNumber ].iFirstMemoryAllocation;
    tmp.iLastMemoryAllocation = KCases[ aCaseNumber ].iLastMemoryAllocation;
    return tmp;

    }
    
void CImageHandlerTests::CloseFbsSession( TAny* aFbsSession )
    {
    RFbsSession* session = static_cast<RFbsSession*>( aFbsSession );
    session->Disconnect();
    }

// -----------------------------------------------------------------------------
// Construction test
// -----------------------------------------------------------------------------
//
TInt CImageHandlerTests::ConstructionTest( 
    TTestResult& /*aResult*/ )
    {
    CPhCltImageHandler* handler = CPhCltImageHandler::NewL();
    delete handler;
   
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Test image params
// -----------------------------------------------------------------------------
//    
TInt CImageHandlerTests::TestImageParamsL(
    TTestResult& /*aResult*/ )
    {
    CPhCltImageHandler* handler = CPhCltImageHandler::NewL();
    CleanupStack::PushL( handler );
    
    CPhCltImageParams* params = handler->CPhCltBaseImageParamsL( EPhCltTypeOperatorLogo );
    CleanupStack::PushL( params );
    
    TL( params->Type() == EPhCltTypeOperatorLogo );
    
    params->AddImageL( 5 );
    T1L( params->GetImageL( 0 ), 5 );
    T1L( params->Count(), 1 );
    TPckgBuf< TPhCltImageArray > images = params->Images();
    
    
    CleanupStack::PopAndDestroy( 2, handler );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// Test saving image
// -----------------------------------------------------------------------------
//     
TInt CImageHandlerTests::SaveAndLoadImageTestL( 
    TTestResult& /*aResult*/ )
    {
    RFbsSession session;
    User::LeaveIfError( session.Connect() );
    CleanupStack::PushL( TCleanupItem( CloseFbsSession ) );
    
    CPhCltImageHandler* handler = CPhCltImageHandler::NewL();
    CleanupStack::PushL( handler );
    
    // Create bitmap.
    CFbsBitmap* bitmap = new( ELeave )CFbsBitmap();
    CleanupStack::PushL( bitmap );
    User::LeaveIfError( bitmap->Create( TSize( 10, 10 ), EColor4K ) );
    
    // Save image.    
    CPhCltImageParams* params = handler->CPhCltBaseImageParamsL( EPhCltTypeVTStill );
    CleanupStack::PushL( params );
    params->AddImageL( bitmap->Handle() );
    
    T1L( handler->SaveImages( *params ), KErrNone );
    
    // Load image.
    CPhCltImageParams* loaded = handler->CPhCltBaseImageParamsL( EPhCltTypeVTStill );
    CleanupStack::PushL( loaded );
    T1L( handler->LoadImages( loaded ), KErrNone );
    T1L( loaded->Count(), 1 )
    
    // Load default VT image, i.e. operator (or similar) defined. In order this
    // to work you must have placed an image file vtdefaultimage.<extension> into
    // Phone Server private directory 10000850. 
    RFile defaultStillImageFile;
    T1L( handler->OpenDefaultVtImage( defaultStillImageFile), KErrNone );
    defaultStillImageFile.Close();
    
    
    CleanupStack::PopAndDestroy( 5 );
    return KErrNone;
    }

//  End of File

