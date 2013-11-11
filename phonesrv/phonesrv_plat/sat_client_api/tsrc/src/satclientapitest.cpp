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
* Description:  Test class for SAT Client API / rsatsession.h
*
*/


// INCLUDE FILES
#include <Stiftestinterface.h>
#include "satclientapitest.h"
#include <stifparser.h>
#include <e32math.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CSATClientAPI::CSATClientAPI( CTestModuleIf& aTestModuleIf ) :
    CScriptBase( aTestModuleIf )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSATClientAPI::ConstructL()
    {
    // Constructing static buffer size logger, needed with OOM testing because
    // normally logger allocates memory from heap!
    iLog = CStifLogger::NewL( KSatClientAPILogPath, 
                              KSatClientAPILogFile,
                              CStifLogger::ETxt,
                              CStifLogger::EFile,
                              EFalse,
                              ETrue,
                              ETrue,
                              EFalse,
                              ETrue,
                              EFalse,
                              100 );
    iLog->Log( _L("CSATClientAPI logging starts!") );
    }

// ---------------------------------------------------------------------------
// NewL creates a new test module
// ---------------------------------------------------------------------------
//
CSATClientAPI* CSATClientAPI::NewL( CTestModuleIf& aTestModuleIf )
    {
    CSATClientAPI* self = new (ELeave) CSATClientAPI( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSATClientAPI::~CSATClientAPI()
    {
    if ( iLog )
        { 
        iLog->Log( _L("###### TEST MODULE CLOSED #######") );
        delete iLog;
        }
    }
// ---------------------------------------------------------------------------
// From class CScriptBase.
// Runs a script line
// ---------------------------------------------------------------------------
//
TInt CSATClientAPI::RunMethodL( CStifItemParser& aItem )
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        // SAT SESSION (exported methods)
        ENTRY( "Connect", CSATClientAPI::Connect ),
        ENTRY( "Close", CSATClientAPI::Close ),
        ENTRY( "Version", CSATClientAPI::Version ),
        };
        
    const TInt count = sizeof( KFunctions ) / 
        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }
// ---------------------------------------------------------------------------
// Calls RSatSession::ConnectL method
// ---------------------------------------------------------------------------
//
TInt CSATClientAPI::Connect( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Connecting..") );
    TRAPD( error, iSatSession.ConnectL() );
    iLog->Log( _L("Connecting done, %d"), error );
    return error;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatSession::Close method
// ---------------------------------------------------------------------------
//
TInt CSATClientAPI::Close( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Closing..") );
    iSatSession.Close();
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatSession::Version method
// ---------------------------------------------------------------------------
//
TInt CSATClientAPI::Version( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Version calling") );
    TInt error( KErrNone );
    TVersion version;    
    TRAP( error,  version = iSatSession.Version() );
    iLog->Log( _L("Major: %d, Minor: %d, Build: %d"), version.iMajor, 
        version.iMinor, version.iBuild );
    iLog->Log( _L("Version done, %d"), error );        
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
    return CSATClientAPI::NewL( aTestModuleIf );
    }

