/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitor the set up idle mote text P&S key.
*
*/


#include    <e32base.h>
#include    <etelsat.h>
#include    <SATInternalPSKeys.h>       // For monitor the P&S key

#include    "csatpluginmonitor.h"
#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave
// -----------------------------------------------------------------------------
//
CSatPluginMonitor* CSatPluginMonitor::NewL()
    {
    LOG( SIMPLE, "CSatPluginMonitor::NewL calling" )
    
    CSatPluginMonitor* self = new ( ELeave ) CSatPluginMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "CSatPluginMonitor::NewL exits" )
    return self;    
    }
// -----------------------------------------------------------------------------
// Destructor
// Cancels  and deletes
// all the dynamic content
// -----------------------------------------------------------------------------
//
CSatPluginMonitor::~CSatPluginMonitor()
    {
    LOG( SIMPLE, "CSatPluginMonitor::~CSatPluginMonitor calling" )
    iIdleAppStatus = NULL;
    Cancel();
    iIdleMode.Close();
    LOG( SIMPLE, "CSatPluginMonitor::~CSatPluginMonitor exits" )
    }
    
// -----------------------------------------------------------------------------
// Start monitor the P&S key.
// -----------------------------------------------------------------------------
//
TInt CSatPluginMonitor::Start( TRequestStatus& aStatus )
    {
    LOG( SIMPLE, "CSatPluginMonitor::StartMonitor calling" )
    TInt err ( KErrNotReady );    
        
    if ( !iIdleAppStatus && !IsActive() )
        {
        aStatus = KRequestPending;
        iIdleAppStatus = &aStatus;
        iIdleMode.Subscribe( iStatus );
        SetActive();
        err = KErrNone;
        LOG( DETAILED, "CSatPluginMonitor::StartMonitor subscribe the P&S key" )       
        }     
        
    LOG2( SIMPLE, "CSatPluginMonitor::StartMonitor exits, err: %d", err )   
    return err;     
    }

// -----------------------------------------------------------------------------
// Get idle mode text
// -----------------------------------------------------------------------------
//
TInt CSatPluginMonitor::GetIdleModeText( HBufC*& aText )
    {
    LOG( SIMPLE, "CSatPluginMonitor::GetIdleModeText calling" )    
    TInt err ( KErrNoMemory );
    
    // Make the function no-leave.
    aText = HBufC::New( RSat::KIdleModeTextMaxSize );  
    if( aText )
        {
        TPtr txtPtr( aText->Des() );
        err = RProperty::Get( KPSUidSatServerInternal, 
                              KSatIdleModeText, 
                              txtPtr ); 
        if ( KErrNone != err )                           
            {  
            delete aText;
            aText = NULL;            
            }         
        }
    LOG2( SIMPLE,
          "SATCLIENT::GetSetupIdleModeTextL text exits, error: %i", 
          err )            
    return err;                                      
    }

// -----------------------------------------------------------------------------
// Get idle mode icon qualifier
// -----------------------------------------------------------------------------
//
TInt CSatPluginMonitor::GetIdleModeIconQualifier( TInt& aIconQualifier )
    {
    LOG( SIMPLE, "CSatPluginMonitor::GetIdleModeIconQualifier calling" )       
    TInt err( KErrNone );
    err = RProperty::Get( KPSUidSatServerInternal, 
                          KSatIdleModeTextIconQualifier, 
                          aIconQualifier );  
    LOG2( DETAILED,
          "SATCLIENT::GetSetupIdleModeTextL iconQualifier error: %i", 
          err )                           
    LOG2( SIMPLE,
          "SATCLIENT::GetSetupIdleModeTextL exits, iconQualifier: %i", 
          aIconQualifier )   
    return err;                                      
    }

// -----------------------------------------------------------------------------
// Get idle mode icon id
// -----------------------------------------------------------------------------
//
TInt CSatPluginMonitor::GetIdleModeIconId( TInt& aRecordNumber )
    {
    LOG( SIMPLE, "CSatPluginMonitor::GetIdleModeIconId calling" ) 
    TInt err( KErrNone );
    err = RProperty::Get( KPSUidSatServerInternal, 
                          KSatIdleModeTextIconId, 
                          aRecordNumber );  
    LOG2( DETAILED,
          "SATCLIENT::GetIdleModeIconId icon id error: %i", 
          err )   
    LOG2( SIMPLE,
          "SATCLIENT: RSatService:GetIdleModeIconId exits, id: %i",
          aRecordNumber ) 
           
    return err;                                
    }


// -----------------------------------------------------------------------------
// Handles the request completion.
// -----------------------------------------------------------------------------
//
void CSatPluginMonitor::RunL()
    {
    LOG2( SIMPLE, "SATCLIENT::CSatPluginMonitor::RunL calling, \
          iIdleAppStatus: %i", iIdleAppStatus )
    
    if ( iIdleAppStatus )
        {
        // Complete the request.
        User::RequestComplete( iIdleAppStatus, iStatus.Int() );
        LOG2( DETAILED, "CSatPluginMonitor::RunL error: %i", iStatus.Int() );
        
        iIdleAppStatus = NULL;       
        }

    LOG( SIMPLE, "SATCLIENT::CSatPluginMonitor::RunL exits" )    
    }
    
// -----------------------------------------------------------------------------
// Cancels the pending request
// -----------------------------------------------------------------------------
//
void CSatPluginMonitor::DoCancel()
    {
    LOG( SIMPLE, "CSatPluginMonitor::DoCancel calling" )
    if ( iIdleAppStatus )
        {
        User::RequestComplete( iIdleAppStatus, KErrCancel );
        iIdleMode.Cancel(); 
        
        iIdleAppStatus = NULL;
        }
    LOG( SIMPLE, "CSatPluginMonitor::DoCancel exits" )
    }
   
// -----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL()
// -----------------------------------------------------------------------------
//   
TInt CSatPluginMonitor::RunError( TInt aError )
    {
    LOG2( SIMPLE, "CSatPluginMonitor::RunError calling, error: %i", aError )
    if ( iIdleAppStatus )
        {
        // Complete the request.
        User::RequestComplete( iIdleAppStatus, aError );
        iIdleMode.Cancel(); 
        
        iIdleAppStatus = NULL;       
        } 
    LOG( SIMPLE, "CSatPluginMonitor::RunError exits" )   
    return aError;
    }
// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave
// -----------------------------------------------------------------------------
//
void CSatPluginMonitor::ConstructL()
    {
    LOG( SIMPLE, "CSatPluginMonitor::ConstructL calling" )  
     
    TInt err = iIdleMode.Attach( KPSUidSatServerInternal, 
                                 KSatIdleModeTextIconQualifier );
    LOG2( DETAILED, "CSatPluginMonitor::ConstructL err: %i, exits", err )   
                              
    User::LeaveIfError( err );                                        
    }
    
// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
CSatPluginMonitor::CSatPluginMonitor() : 
    CActive( EPriorityStandard )
    {
    LOG( SIMPLE, "CSatPluginMonitor::ConstructL calling" ) 
    CActiveScheduler::Add( this );
    LOG( SIMPLE, "CSatPluginMonitor::ConstructL exits" ) 
    }
    
//  End of File
