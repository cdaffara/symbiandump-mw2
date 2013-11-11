/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Test class for SAT Service API 
*
*/


// INCLUDE FILES
#include <stiftestinterface.h>
#include <stifparser.h>
#include <stiflogger.h>
#include <stiftestmodule.h>
#include <fbs.h>
#include <satinternalpskeys.h>
#include <e32property.h>
#include <tsaticoninfo.h>

#include "satserviceapitest.h"
#include "satserviceapitestmonitor.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CSATServiceAPI::CSATServiceAPI( CTestModuleIf& aTestModuleIf ) :
    CScriptBase( aTestModuleIf )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSATServiceAPI::ConstructL()
    {
    // Constructing static buffer size logger, needed with OOM testing because
    // normally logger allocates memory from heap.
    iLog = CStifLogger::NewL( KSatServiceAPILogPath, 
                              KSatServiceAPILogFile,
                              CStifLogger::ETxt,
                              CStifLogger::EFile,
                              EFalse,
                              ETrue,
                              ETrue,
                              EFalse,
                              ETrue,
                              EFalse,
                              100 );
    iLog->Log( _L("CSATServiceAPI logging starts!") );
    iSatSession.ConnectL();
    }

// ---------------------------------------------------------------------------
// NewL creates a new test module
// ---------------------------------------------------------------------------
//
CSATServiceAPI* CSATServiceAPI::NewL( CTestModuleIf& aTestModuleIf )
    {
    CSATServiceAPI* self = new (ELeave) CSATServiceAPI( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSATServiceAPI::~CSATServiceAPI()
    {
    if ( iLog )
        { 
        iLog->Log( _L("###### TEST MODULE CLOSED #######") );
        delete iLog;
        }
    iSatSession.Close();
    delete iMonitor;
    }
    
// ---------------------------------------------------------------------------
// From class CScriptBase.
// Runs a script line
// ---------------------------------------------------------------------------
//
TInt CSATServiceAPI::RunMethodL( CStifItemParser& aItem )
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        // SAT SERVICE API (exported methods)
        ENTRY( "OpenL", 
               CSATServiceAPI::OpenL ),
        ENTRY( "Close", 
               CSATServiceAPI::Close ),
        ENTRY( "GetIconInfoL", 
               CSATServiceAPI::GetIconInfoL ),
        ENTRY( "GetIconL", 
               CSATServiceAPI::GetIconL ),
        ENTRY( "SetIdleModeTextResponse", 
               CSATServiceAPI::SetIdleModeTextResponse ),
        ENTRY( "NotifySetupIdleModeTextChange", 
               CSATServiceAPI::NotifySetupIdleModeTextChange ),
        ENTRY( "NotifySetupIdleModeTextChangeCancel", 
               CSATServiceAPI::NotifySetupIdleModeTextChangeCancel ),
        ENTRY( "GetSetupIdleModeTextL", 
               CSATServiceAPI::GetSetupIdleModeTextL ),

        // Utility methods
        ENTRY( "SetSetupIdleModeText", 
               CSATServiceAPI::SetSetupIdleModeText ),
        ENTRY( "CheckSetupIdleModeTextL", 
               CSATServiceAPI::CheckSetupIdleModeTextL ),
        };
        
    const TInt count = sizeof( KFunctions ) / 
        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }

// ---------------------------------------------------------------------------
// Calls RSatService::OpenL method
// ---------------------------------------------------------------------------
//
TInt CSATServiceAPI::OpenL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Opening..") );

    TInt error( KErrAlreadyExists );
     if (!iSatService )
        {
        iSatService = new (ELeave) RSatService();
        TRAP( error, iSatService->OpenL( iSatSession ) );
        iLog->Log( _L("Opening done, %d"), error );
        iMonitor = new ( ELeave )CSatServiceAPIMonitor( *iLog, *this );
        }
    return error;
    }
  
// ---------------------------------------------------------------------------
// Calls RSatService::Close method
// ---------------------------------------------------------------------------
//
TInt CSATServiceAPI::Close( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("Closing..") );
    TInt error( KErrSessionClosed );
    if ( iSatService )
        {
        iSatService->Close();
        delete iSatService;
        iSatService = NULL;
        delete iMonitor;
        iMonitor = NULL;
        error = KErrNone;
        }
    return error;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatService::GetIconInfoL method
// ---------------------------------------------------------------------------
//
TInt CSATServiceAPI::GetIconInfoL( CStifItemParser& aItem )
    {
    iLog->Log( _L("GetIconInfoL calling") );
    TInt ret( KErrSessionClosed );
    TInt recordId( 1 ); // default, records start from 1
    aItem.GetInt( _L("GetIconInfoL"), recordId );
    
    if ( iSatService )
        {
        RIconEf iconInfo;
        iSatService->GetIconInfoL( recordId, iconInfo );
        iLog->Log( _L("GetIconInfoL done") );
        if ( !iconInfo.Count() )
            {
            iLog->Log( _L("no icons found") );
            ret = KErrNotFound;
            }
        else
            {
            ret = KErrNone;
            }
        iconInfo.Close();
        }
    
    iLog->Log( _L("GetIconInfoL exit") );
    return ret;
    }

// ---------------------------------------------------------------------------
// Calls RSatService::GetIconInfoL method - gets an icon syncronously
// first parameter defines record number, seconds which icon from the 
// record is read
// ---------------------------------------------------------------------------
//
TInt CSATServiceAPI::GetIconL( CStifItemParser& aItem )
    {
    iLog->Log( _L("GetIconL calling") );

    TInt ret( KErrSessionClosed );
    TInt recordId( 1 ); // default
    aItem.GetInt( _L("GetIconL"), recordId );
    
    if ( iSatService )
        {
        RIconEf iconInfo;
        iSatService->GetIconInfoL( recordId, iconInfo );
        iLog->Log( _L("IconInfo read") );
        
        if ( iconInfo.Count() )
            {
            iLog->Log( _L("%d icons found from this record"), 
                iconInfo.Count() );
            // get the first icon (default)
            TInt iconId( 0 ); 
            // check if test scipt has defined icon id
            aItem.GetNextInt( iconId );
            CFbsBitmap* icon = NULL;
            
            // Fbs session needed as this module is been ran in non-UI process
            RFbsSession fbsSession;
            ret = fbsSession.Connect();
            TRAPD( trapError, 
                   icon = iSatService->GetIconL( iconInfo[iconId] ) );
            if ( icon && !trapError )
                {
                iLog->Log( _L("success, bitmap exists") );
                ret = KErrNone;
                }
            else
                {
                iLog->Log( _L("failure, error code %d "), trapError );
                ret = trapError;
                }
            delete icon;
            
            // Disconnect FBS session
            fbsSession.Disconnect();
            
            }
        else
            {
            ret = KErrNotFound;
            }
        iconInfo.Close();
        }

    return ret;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatService::SetIdleModeTextResponse method
// ---------------------------------------------------------------------------
//
TInt CSATServiceAPI::SetIdleModeTextResponse( CStifItemParser& aItem )
    {
    iLog->Log( _L("SetIdleModeTextResponse calling") );
    TInt ret( KErrSessionClosed );
    if ( iSatService )
        {
        TUint generalResult( 0 ); //default, if not set in script
        aItem.GetNextInt( generalResult, EHex );
        // send the response
        iSatService->SetIdleModeTextResponse( 
            (RSatService::TSATIdleResult)generalResult );
        ret = KErrNone;
        }
    
    iLog->Log( _L("SetIdleModeTextResponse done") );
    return ret;
    }
    
// ---------------------------------------------------------------------------
// Calls RSatService::NotifySetupIdleModeTextChange method
// ---------------------------------------------------------------------------
//     
TInt CSATServiceAPI::NotifySetupIdleModeTextChange( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("NotifySetupIdleModeTextChange calling") );
    TInt ret( KErrSessionClosed );
    if ( iSatService && iMonitor )
        {               
        ret = iSatService->NotifySetupIdleModeTextChange( iMonitor->Status() );
        iMonitor->Activate();
        }    
    iLog->Log( _L("NotifySetupIdleModeTextChange done") );
    return ret;    
    }
    
    
// ---------------------------------------------------------------------------
// Calls RSatService::NotifySetupIdleModeTextChangeCancel method
// ---------------------------------------------------------------------------
//     
TInt CSATServiceAPI::NotifySetupIdleModeTextChangeCancel( 
    CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("NotifySetupIdleModeTextChangeCancel calling") );
    TInt ret( KErrNone );
    if ( iSatService )
        {
        iSatService->NotifySetupIdleModeTextChangeCancel();    
        }
    
    iLog->Log( _L("NotifySetupIdleModeTextChangeCancel done") );
    return ret;       
    }   
     
// ---------------------------------------------------------------------------
// Calls RSatService::GetSetupIdleModeTextL method
// ---------------------------------------------------------------------------
//  
TInt CSATServiceAPI::GetSetupIdleModeTextL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log( _L("GetSetupIdleModeTextL calling") );
    TInt ret( KErrNotFound );  
     
    if ( iSatService )
        {
        HBufC* string( NULL );
        RSatService::TSatIconQualifier iconQualifier( 
            RSatService::ESatIconNoIcon );
        TUint8 recordId( 0 );

        if ( iSatService->GetSetupIdleModeTextL( string, 
             iconQualifier, recordId ) )
            {
            ret = KErrNone;
            }
        delete string;        
        }
    iLog->Log( _L("GetSetupIdleModeTextL done") );
    return ret;   
    }
 
// ---------------------------------------------------------------------------
// Publish setupidlemode text and icon to P&S keys
// ---------------------------------------------------------------------------
//  
TInt CSATServiceAPI::SetSetupIdleModeText( CStifItemParser& aItem )
    {
    TInt ret( KErrNone );  
    
    // 1. Read text form config file, set to P&S key 
    // and save the text to iText
    aItem.GetNextString( iText );
    ret = RProperty::Set( KPSUidSatServerInternal, 
                          KSatIdleModeText, 
                          iText );  
    
    // 2. Read record id form config file, set to P&S key 
    // and save id to iRecrodId                        
    TInt recordId( 0 );
    aItem.GetNextInt( recordId );
    ret = RProperty::Set( KPSUidSatServerInternal, 
                          KSatIdleModeTextIconId, 
                          recordId );  
    if ( recordId >= 0 )
        {
        iRecordId = static_cast<TUint8>( recordId );
        }
    else
        {
        iIconQualifier = RSatService::ESatIconNoIcon;
        iRecordId = 0;        
        }        
      
    // 3 Read icon qulifier form config file, set to P&S key 
    // and save it to iIconQulifier
    TInt iconQualifier( 0 ); 
    aItem.GetNextInt( iconQualifier );
    ret = RProperty::Set( KPSUidSatServerInternal, 
                          KSatIdleModeTextIconQualifier, 
                          iconQualifier ); 
                          
    // Form enum TSatIdleModeTextIconQualifierValue defined in 
    // satinternalpskeys.h to enum TSatIconQualifier defined in rsatservice.h
    // NOTE: the value are different.
    switch ( iconQualifier )
        {
        case KSatIdleIconQInit: // go through
        case KSatIdleIconQNoIcon: 
                {
                iIconQualifier = RSatService::ESatIconNoIcon;
                break;
                }
        case KSatIdleIconQSelfExplanatory:
                {
                iIconQualifier = RSatService::ESatIconSelfExplanatory;
                break;
                }        
        case KSatIdleIconQNotSelfExplanatory:
                {
                iIconQualifier = RSatService::ESatIconNotSelfExplanatory;
                break;
                }
        default:
            {
            iIconQualifier = RSatService::ESatIconNoIcon;
            break;            
            }   
        }
    return ret;
    }
   
// ---------------------------------------------------------------------------
// Check setupidlemode text and icon.
// ---------------------------------------------------------------------------
//  
TInt CSATServiceAPI::CheckSetupIdleModeTextL( CStifItemParser& /*aItem*/ ) 
    {
    iLog->Log( _L("CheckSetupIdleModeTextL calling") );
    TInt ret( KErrNotFound );  
     
    if ( iSatService )
        {
        HBufC* string( NULL );
        RSatService::TSatIconQualifier iconQualifier( 
            RSatService::ESatIconNoIcon );
        TUint8 recordId( 0 );

        if ( iSatService->GetSetupIdleModeTextL( string, 
             iconQualifier, recordId ) && string )
            {         
            ret = ( ( !string->Compare( iText ) ) && 
                    ( iIconQualifier == iconQualifier ) &&
                    ( iRecordId == recordId ) );            
            }
        delete string;        
        }
    iLog->Log( _L("CheckSetupIdleModeTextL done") );
    return ret;      
    }
    
// ======== GLOBAL FUNCTIONS ========

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CScriptBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    return CSATServiceAPI::NewL( aTestModuleIf );
    }

