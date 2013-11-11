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
* Description:  MPX Back Stepping Service API 
*
*/


// from AIW and LIW
#include <liwcommon.h>
#include <liwservicehandler.h>

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

#include "mpxbacksteppingutilityimp.h"
#include <mpxlog.h>

// definitions needed by BackStepping service. (ref. bsserviceconstants.h)

// BSS Service and Interface ID
_LIT8( KBSServiceID, "Service.BackStepping" );
_LIT8( KBSInterface, "IBackStepping" );

// BSS commands
_LIT8( KBSCmdInitialize, "Initialize" );
_LIT8( KBSCmdForwardActivationEvent, "ForwardActivationEvent" );
_LIT8( KBSCmdHandleBackCommand, "HandleBackCommand" );

// BSS in param names
_LIT8( KBSInParamAppUid, "AppUid" );
_LIT8( KBSInParamState, "State" );
_LIT8( KBSInParamEnter, "Enter" );

// BSS out param names
_LIT8( KBSOutParamStatusInfo, "StatusInfo" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Private Constructor
// ---------------------------------------------------------------------------
//
CMPXBackSteppingUtilityImp::CMPXBackSteppingUtilityImp()
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXBackSteppingUtilityImp::~CMPXBackSteppingUtilityImp()
    {
    MPX_DEBUG1( "CMPXBackSteppingUtilityImp::~CMPXBackSteppingUtilityImp" );
    if ( iBSInterface )
        {
        iBSInterface->Close();
        }

    delete iServiceHandler;
    }

// ----------------------------------------------------------------------------
// 2nd-Phased Constructor
// ----------------------------------------------------------------------------
//
void CMPXBackSteppingUtilityImp::ConstructL()
    {
    MPX_DEBUG1( "-->CMPXBackSteppingUtilityImp::ConstructL" );
    // create Service Handler and keep as long as access to BS Service needed
    iServiceHandler = CLiwServiceHandler::NewL();
    // for convinience keep pointers to Service Handler param lists
    iInParamList = &iServiceHandler->InParamListL();
    iOutParamList = &iServiceHandler->OutParamListL();

    RCriteriaArray criteriaArray;
    CleanupResetAndDestroyPushL( criteriaArray );

    // create AIW criteria
    CLiwCriteriaItem* criterion = CLiwCriteriaItem::NewLC( 
        KLiwCmdAsStr, KBSInterface, KBSServiceID );
    criterion->SetServiceClass( TUid::Uid( KLiwClassBase ) );

    criteriaArray.AppendL( criterion );
    CleanupStack::Pop( criterion );

    // attach AIW criteria
    iServiceHandler->AttachL( criteriaArray );
    // get BS Service interface
    iServiceHandler->ExecuteServiceCmdL( 
        *criterion, 
        *iInParamList, 
        *iOutParamList );

    CleanupStack::PopAndDestroy( &criteriaArray );
    
    // extract BS interface from output params
    TInt pos( 0 );
    iOutParamList->FindFirst( pos, KBSInterface );
    if( pos != KErrNotFound )
        {
        iBSInterface = (*iOutParamList)[pos].Value().AsInterface(); 
        }
    
    if ( !iBSInterface )
        {
        // handle no BS Service
        User::Leave( KErrNotFound );
        }

    MPX_DEBUG1( "<--CMPXBackSteppingUtilityImp::ConstructL" );
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXBackSteppingUtilityImp* CMPXBackSteppingUtilityImp::NewL()
    {
    CMPXBackSteppingUtilityImp* self = CMPXBackSteppingUtilityImp::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXBackSteppingUtilityImp* CMPXBackSteppingUtilityImp::NewLC()
    {
    CMPXBackSteppingUtilityImp* self = new( ELeave ) CMPXBackSteppingUtilityImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Retrieves view utility from TLS. Creates if it's not there.
// ---------------------------------------------------------------------------
//
MMPXBackSteppingUtility* CMPXBackSteppingUtilityImp::UtilityL()
    {
    MPX_DEBUG1( "-->CMPXBackSteppingUtilityImp::UtilityL" );
    MMPXBackSteppingUtility* utility = 
        reinterpret_cast<MMPXBackSteppingUtility*>( Dll::Tls() );
    if ( !utility )
        {
        utility = CMPXBackSteppingUtilityImp::NewL();
        StoreUtility( utility );
        }

    static_cast<CMPXBackSteppingUtilityImp*>( utility )->iRefCount++;
    MPX_DEBUG1( "<--CMPXBackSteppingUtilityImp::UtilityL" );
    return utility;
    }

// ---------------------------------------------------------------------------
// Store back stepping utility to TLS.
// ---------------------------------------------------------------------------
//
void CMPXBackSteppingUtilityImp::StoreUtility( MMPXBackSteppingUtility* aUtility )
    {
    Dll::SetTls( aUtility );
    }

// ---------------------------------------------------------------------------
// From CMPXBackSteppingUtilityImp
// This method must be called when the client no longer needs this instance. 
// This object will be destroyed when no one is using it.
// ---------------------------------------------------------------------------
//
void CMPXBackSteppingUtilityImp::Close()
    {
    ASSERT( iRefCount > 0 );
    MPX_FUNC_EX( "CMPXBackSteppingUtilityImp::Close" );
    MPX_DEBUG2( "CMPXBackSteppingUtilityImp::Close ref count = %d", iRefCount );
    if ( --iRefCount == 0 )
        {
        MPX_DEBUG1( "CMPXBackSteppingUtilityImp::Close deleting this" );
        // last client released
        StoreUtility( NULL );
        delete this;
        }
    }

// ---------------------------------------------------------------------------
// Forwards activation event
// ---------------------------------------------------------------------------
//
TInt CMPXBackSteppingUtilityImp::ForwardActivationEventL( 
    const TDesC8& aState, 
    const TBool aEnter )
    {
    MPX_DEBUG1( "-->CMPXBackSteppingUtilityImp::ForwardActivationEventL" );
    iInParamList->Reset();
    iOutParamList->Reset();

    TLiwGenericParam state( KBSInParamState, TLiwVariant( aState ) );
    iInParamList->AppendL( state );
    TLiwGenericParam enter( KBSInParamEnter, TLiwVariant( aEnter ) );
    iInParamList->AppendL( enter );
    
    iBSInterface->ExecuteCmdL( 
        KBSCmdForwardActivationEvent, 
        *iInParamList, 
        *iOutParamList );
    
    TInt result = HandleResultL();
    
    iInParamList->Reset();
    iOutParamList->Reset();
    
    MPX_DEBUG1( "<--CMPXBackSteppingUtilityImp::ForwardActivationEventL" );
    return result;
    }

// ---------------------------------------------------------------------------
// Handles back command
// ---------------------------------------------------------------------------
//
TInt CMPXBackSteppingUtilityImp::HandleBackCommandL( 
    const TDesC8& aState )
    {
    MPX_DEBUG1( "-->CMPXBackSteppingUtilityImp::HandleBackCommandL" );
    iInParamList->Reset();
    iOutParamList->Reset();

    TLiwGenericParam state( KBSInParamState, TLiwVariant( aState ) );
    iInParamList->AppendL( state );
    
    iBSInterface->ExecuteCmdL( 
        KBSCmdHandleBackCommand, 
        *iInParamList, 
        *iOutParamList );
    
    TInt result = HandleResultL();
    
    iInParamList->Reset();
    iOutParamList->Reset();
    
    MPX_DEBUG1( "<--CMPXBackSteppingUtilityImp::HandleBackCommandL" );
    return result;
    }

// ---------------------------------------------------------------------------
// Initializes BS Service
// ---------------------------------------------------------------------------
//
void CMPXBackSteppingUtilityImp::InitializeL( const TUid aUid )
    {
    MPX_DEBUG2( "-->CMPXBackSteppingUtilityImp::InitializeL aUid=0x%x", aUid.iUid );
    iInParamList->Reset();
    iOutParamList->Reset();

    TLiwGenericParam appUid( KBSInParamAppUid, TLiwVariant( aUid.iUid ) );
    iInParamList->AppendL( appUid );
    
    iBSInterface->ExecuteCmdL( 
        KBSCmdInitialize, 
        *iInParamList, 
        *iOutParamList );

    if ( HandleResultL() != 1 )
        {
        // Initialize command was not consumed - handle failiure
        User::Leave( KErrArgument );
        }
    
    iInParamList->Reset();
    iOutParamList->Reset();
    MPX_DEBUG1( "<--CMPXBackSteppingUtilityImp::InitializeL" );
    }

// ---------------------------------------------------------------------------
// Handles the result of a LIW command
// ---------------------------------------------------------------------------
//
TInt CMPXBackSteppingUtilityImp::HandleResultL()
    {
    MPX_DEBUG1( "-->CMPXBackSteppingUtilityImp::HandleResultL" );
    TInt statusInfo( KErrNotFound );
    
    TInt posErr( 0 );
    iOutParamList->FindFirst( posErr, LIW::EGenericParamError );
    if( posErr != KErrNotFound )
        {
        // error code found - extract and handle
        TInt errorCode( KErrNone );
        (*iOutParamList)[posErr].Value().Get( errorCode );
        
        User::LeaveIfError( errorCode );
        }

    // if no error returned, carry on
    TInt posStat( 0 );
    iOutParamList->FindFirst( posStat, KBSOutParamStatusInfo );
    if( posStat != KErrNotFound )
        {
        // status info present - extract and return
        (*iOutParamList)[posStat].Value().Get( statusInfo );
        }
    else
        {
        // no return value
        // this should not happen
        User::Leave( KErrNotFound );
        }

    MPX_DEBUG2( "<--CMPXBackSteppingUtilityImp::HandleResultL statusInfo=%d", statusInfo );
    return statusInfo;
    }

// End of file
