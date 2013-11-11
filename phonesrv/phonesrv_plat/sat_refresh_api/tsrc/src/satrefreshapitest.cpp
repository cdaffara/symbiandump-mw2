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
* Description:  Test class for SAT Refresh API 
*
*/


// INCLUDE FILES
#include <Stiftestinterface.h>
#include "satrefreshapitest.h"
#include <stifparser.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CSatRefreshAPI::CSatRefreshAPI( CTestModuleIf& aTestModuleIf ) :
    CScriptBase( aTestModuleIf )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSatRefreshAPI::ConstructL()
    {
    // Constructing static buffer size logger, needed with OOM testing because
    // normally logger allocates memory from heap!
    iLog = CStifLogger::NewL( KSatRefreshAPILogPath, 
                              KSatRefreshAPILogFile,
                              CStifLogger::ETxt,
                              CStifLogger::EFile,
                              EFalse,
                              ETrue,
                              ETrue,
                              EFalse,
                              ETrue,
                              EFalse,
                              100 );
    iLog->Log( _L("CSatRefreshAPI logging starts!") );

    // Create observer    
    iObserver = new (ELeave) CSatRefreshObserver( iLog );
    // Initialize SAT Client API
    iSatSession.ConnectL();
    }

// ---------------------------------------------------------------------------
// NewL creates a new test module
// ---------------------------------------------------------------------------
//
CSatRefreshAPI* CSatRefreshAPI::NewL( CTestModuleIf& aTestModuleIf )
    {
    CSatRefreshAPI* self = new (ELeave) CSatRefreshAPI( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSatRefreshAPI::~CSatRefreshAPI()
    { 
    if ( iLog)
        {
        iLog->Log( _L("###### Closing #######") );
        }
        
    if ( iSatRefresh )
        {
        iSatRefresh->Close();
        delete iSatRefresh;
        iSatRefresh = NULL;
        }
        
    iSatSession.Close();

    delete iObserver;
    delete iLog;
    
    }
// ---------------------------------------------------------------------------
// From class CScriptBase.
// Runs a script line
// ---------------------------------------------------------------------------
//
TInt CSatRefreshAPI::RunMethodL( CStifItemParser& aItem )
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        // REFRESH API (exported methods)
        ENTRY( "OpenL", CSatRefreshAPI::OpenL ),
        ENTRY( "Close", CSatRefreshAPI::Close ),
        ENTRY( "Cancel", CSatRefreshAPI::Cancel ),
        ENTRY( "NotifyFileChangeL", CSatRefreshAPI::NotifyFileChangeL ),
        ENTRY( "RefreshEFRead", CSatRefreshAPI::RefreshEFRead ),
        };
        
    const TInt count = sizeof( KFunctions ) / 
        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }
// ---------------------------------------------------------------------------
// Calls RSatRefresh::OpenL method
// ---------------------------------------------------------------------------
//
TInt CSatRefreshAPI::OpenL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Opening..") );
    TInt error( KErrAlreadyExists );
    if (!iSatRefresh )
        {
        iSatRefresh = new (ELeave) RSatRefresh( *iObserver );
        TRAP( error, iSatRefresh->OpenL( iSatSession ) );
        iLog->Log( _L("Opening done, %d"), error );
        error = KErrNone;
        }
    return error;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatRefresh::Close method
// ---------------------------------------------------------------------------
//
TInt CSatRefreshAPI::Close( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Closing..") );
    TInt error( KErrSessionClosed );
    if ( iSatRefresh )
        {
        iSatRefresh->Close();
        delete iSatRefresh;
        iSatRefresh = NULL;
        error = KErrNone;
        }
    return error;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatRefresh::Cancel method
// ---------------------------------------------------------------------------
//
TInt CSatRefreshAPI::Cancel( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Cancel calling") );
    TInt error( KErrSessionClosed );
    if ( iSatRefresh )
        {
        iSatRefresh->Cancel();
        iLog->Log( _L("Cancel done") );
        error = KErrNone;
        }
 
    return error;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatRefresh::NotifyFileChangeL method with different arguments
// ---------------------------------------------------------------------------
//
TInt CSatRefreshAPI::NotifyFileChangeL( CStifItemParser& aItem )
    {
    iLog->Log( _L("NotifyFileChangeL calling") );
    TInt error( KErrSessionClosed );
    TUint fileId( 0 );
    aItem.GetNextInt( fileId, EHex );
    
    if ( iSatRefresh && !fileId )
        {
        iSatRefresh->NotifyFileChangeL();
        iLog->Log( _L("NotifyFileChangeL done") );
        error = KErrNone;
        }
    else if ( iSatRefresh )
        {
        // Notify only requested EFs
        // E.g. 6F01 6F02 4F20 
        RSat::TRefreshFileList observedFileList;
        observedFileList.Append( fileId ); // first one
        while ( !aItem.GetNextInt( fileId, EHex ) )
            {
            // Append remaining files
            observedFileList.Append( fileId );
            }
        iSatRefresh->NotifyFileChangeL( observedFileList );
        iLog->Log( _L("NotifyFileChangeL with FileList done") );
        error = KErrNone;
        }

    return error;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatRefresh::RefreshEFRead method with different arguments
// 0 means EFalse(Default), 1 ETrue
// ---------------------------------------------------------------------------
//
TInt CSatRefreshAPI::RefreshEFRead( CStifItemParser& aItem )
    {
    iLog->Log( _L("RefreshEFRead calling") );
    TBool result( EFalse ); // default
    
    aItem.GetInt( _L("RefreshEFRead"), result );
    TInt error( KErrSessionClosed );
    if ( iSatRefresh )
        {
        iSatRefresh->RefreshEFRead( result );
        iLog->Log( _L("RefreshEFRead done - value %d"), result );
        error = KErrNone;
        }

    return error;
    }    
// ======== GLOBAL FUNCTIONS ========

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CScriptBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    return CSatRefreshAPI::NewL( aTestModuleIf );
    }

