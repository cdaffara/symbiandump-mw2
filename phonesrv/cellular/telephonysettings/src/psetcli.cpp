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
* Description:  CPsetCli is provides calling line identification SS.
*
*
*/


//INCLUDE FILES
#include "psetcli.h" 
#include "psetpanic.h" 
#include "psetconstants.h" 
#include "mpsetrequestobs.h" 
#include <bldvariant.hrh>
#include <featmgr.h>

// CONSTANTS    
_LIT( KPSNameOfClass, "CPsetCli" );

//  MEMBER FUNCTIONS
// ---------------------------------------------------------------------------
// Symbian OS 1st phase Constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCli* CPsetCli::NewL( RMobilePhone& aPhone, MPsetCliObserver& aObserver )
    {
    CPsetCli* self = new ( ELeave ) CPsetCli( aPhone );
    CleanupStack::PushL( self );
    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();
    CleanupStack::Pop( self );
    self->SetObserver( aObserver );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCli::~CPsetCli()
    {
    // Remove FeatureManager
    FeatureManager::UnInitializeLib();
    Cancel();
    iReqObserver = NULL;
    }

// ---------------------------------------------------------------------------
// C++ Constructor.
// ---------------------------------------------------------------------------
//
CPsetCli::CPsetCli( RMobilePhone& aPhone ) : CActive( EPriorityStandard ),
iPhone( aPhone )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Cancels all requests.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCli::CancelAll()
    {
    // Set PSUI so that correct observer is used and hide requesting note.
    // When interrupting a note, does not leave.
    // This needs to be done first, since notes must be cleared from screen
    // even though request is not active.
    iObserver->SetEngineContact( this );
    TRAPD( ignore, iObserver->HandleCliRequestingL( EFalse, ETrue ) );
    // Line below fixes armv5 warning.
    ignore = ignore;
    
    if ( !IsActive() )
        {
        return;
        }    
    
    Cancel();
    }

// ---------------------------------------------------------------------------
// Requests for COLP mode.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCli::GetColpModeL()
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    IssueRequestL( RMobilePhone::EIdServiceConnectedPresentation );
    }

// ---------------------------------------------------------------------------
// Requests for CLIP mode.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCli::GetClipModeL()
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    IssueRequestL( RMobilePhone::EIdServiceCallerPresentation );
    }

// ---------------------------------------------------------------------------
// Requests for COLP mode.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCli::GetClirModeL()
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    IssueRequestL( RMobilePhone::EIdServiceCallerRestriction );
    }

// ---------------------------------------------------------------------------
// Requests for COLR mode.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCli::GetColrModeL()
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    IssueRequestL( RMobilePhone::EIdServiceConnectedRestriction );
    }

// ---------------------------------------------------------------------------
// Sets request observer.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCli::SetRequestObserver( MPsetRequestObserver* aObs )
    {
    iReqObserver = aObs;
    }

// ---------------------------------------------------------------------------
// Make request to get CNAP status
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCli::GetCnapL()
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdPhoneCnap ) )
        {
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        IssueRequestL( RMobilePhone::EIdServiceCallerName );
        }
    }

//---------------------------------------------------------------------------
// Handles completed request. 
// 
//---------------------------------------------------------------------------
//
void CPsetCli::RunL()
    {
    __ASSERT_ALWAYS( iObserver, Panic( KPSNameOfClass, ECliNoObserver ) );
    TBool commonNotes = ETrue;
    
    //Hide requesting note - does not leave when deleting a note.
    iObserver->SetEngineContact( this );
    iObserver->HandleCliRequestingL( EFalse, EFalse );
    if ( iStatus != KErrNone )
        {
        iObserver->HandleCliErrorL( iStatus.Int() );
        RequestCompleted( iStatus.Int() );        
        return;
        }
    
    TPsuiCli type = EPsuiCliUnknown; 
    
    switch ( iRequestedService )
        {
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        case RMobilePhone::EIdServiceConnectedRestriction:
            switch ( iServiceStatus )
                {
                case RMobilePhone::EIdServiceActivePermanent:
                    type = EPsuiColrOn;
                    break;
                case RMobilePhone::EIdServiceNotProvisioned:
                    type = EPsuiColrOff;
                    break;
                case RMobilePhone::EIdServiceUnknown:
                    type = EPsuiCliUnknown;
                    break;
                default:
                    break;
                }
            break;
        case RMobilePhone::EIdServiceConnectedPresentation:
            switch ( iServiceStatus )
                
                {
                case RMobilePhone::EIdServiceActivePermanent:
                    type = EPsuiColpOn;
                    break;
                case RMobilePhone::EIdServiceNotProvisioned:
                    type = EPsuiColpOff;
                    break;
                case RMobilePhone::EIdServiceUnknown:
                    type = EPsuiCliUnknown;
                    break;
                default:
                    break;
                }
            break;
        case RMobilePhone::EIdServiceCallerRestriction:
            switch ( iServiceStatus )
            
                {
                case RMobilePhone::EIdServiceActivePermanent:                
                case RMobilePhone::EIdServiceActiveDefaultRestricted:
                    type = EPsuiClirOn;
                    break;
                case RMobilePhone::EIdServiceActiveDefaultAllowed:
                case RMobilePhone::EIdServiceNotProvisioned:
                    type = EPsuiClirOff;
                    break;
                case RMobilePhone::EIdServiceUnknown:
                    type = EPsuiCliUnknown;
                    break;
                default:
                    break;
                }
            break;
        case RMobilePhone::EIdServiceCallerPresentation:
            switch ( iServiceStatus )
                {
                case RMobilePhone::EIdServiceActivePermanent:
                    type = EPsuiClipOn;
                    break;
                case RMobilePhone::EIdServiceNotProvisioned:
                    type = EPsuiClipOff;
                    break;
                case RMobilePhone::EIdServiceUnknown:
                    type = EPsuiCliUnknown;
                    break;
                default:
                    break;
                }
            break;
        case RMobilePhone::EIdServiceCallerName:
            commonNotes = EFalse;
            iObserver->HandleCnapStatusL( iServiceStatus );
            break;
        default:
            Panic( KPSNameOfClass, ECliParameter );
            break;
        } 

    if ( commonNotes )
        {
        iObserver->CliInformationL( type );
        }    
    RequestCompleted( KErrNone );
    }

// ---------------------------------------------------------------------------
// Cancels pending request.
// 
// ---------------------------------------------------------------------------
//
void CPsetCli::DoCancel()
    {
    if ( !IsActive() )
        {
        return;
        }
    switch ( iRequestedService )
        {
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        case RMobilePhone::EIdServiceConnectedRestriction:
        case RMobilePhone::EIdServiceConnectedPresentation:
        case RMobilePhone::EIdServiceCallerRestriction:
        case RMobilePhone::EIdServiceCallerPresentation:
        case RMobilePhone::EIdServiceCallerName:
            iPhone.CancelAsyncRequest( EMobilePhoneGetIdentityServiceStatus );
            break;
        default:
            break;            
        }
    SetRequestStatus( RMobilePhone::EIdServiceUnspecified );
    }

// ---------------------------------------------------------------------------
// Sets observer.
// ---------------------------------------------------------------------------
//
void CPsetCli::SetObserver( MPsetCliObserver& aObserver )
    {
    iObserver = &aObserver;
    iObserver->SetEngineContact( this );
    }

// ---------------------------------------------------------------------------
// 
// Push object into cleanupstack to catch leaving.
// 
// ---------------------------------------------------------------------------
//
void CPsetCli::CleanupLeavePushL()
    {
    CleanupStack::PushL( TCleanupItem( DoHandleLeave, this ) );
    }

// ---------------------------------------------------------------------------
// 
// Things to do when leave occurs.
// 
// ---------------------------------------------------------------------------
//
void CPsetCli::HandleLeave()
    {
    CancelAll();
    }

// ---------------------------------------------------------------------------
// 
// Object has caused a leave.
// 
// ---------------------------------------------------------------------------
//
void CPsetCli::DoHandleLeave( TAny* aAny )
    {
    REINTERPRET_CAST( CPsetCli*, aAny )->HandleLeave();
    }

// ---------------------------------------------------------------------------
// Inform the request observer that request has been completed.
// ---------------------------------------------------------------------------
//
void CPsetCli::RequestCompleted( const TInt& aError )
    {
    if ( aError != KErrNone )
        {
        SetRequestStatus( 
            static_cast <RMobilePhone::TMobilePhoneIdService> (aError) );
        }
    SetRequestStatus( RMobilePhone::EIdServiceUnspecified );
    
    if ( iReqObserver )
        {
        iReqObserver->RequestComplete(); 
        }
    }

// ---------------------------------------------------------------------------
// Issues pre-defined request.
// ---------------------------------------------------------------------------
//
void CPsetCli::IssueRequestL( RMobilePhone::TMobilePhoneIdService aService ) 
    {    
    if ( IsActive() )
        {
        /*****************************************************
        *    Series 60 Customer / ETel
        *    Series 60  ETel API
        *****************************************************/
        User::Leave( KErrGeneral );
        }    
    SetRequestStatus( aService );
       
    iPhone.GetIdentityServiceStatus( iStatus, aService, iServiceStatus );    
    SetActive();
    
    // Set PSUI so that correct observer is used and show note.
    iObserver->SetEngineContact( this );
    CleanupLeavePushL();    
    iObserver->HandleCliRequestingL( ETrue, EFalse );
    CleanupStack::Pop(); // CleanupCancelPushL
    }

// ---------------------------------------------------------------------------
// Update the request status - both internally and to observers.
// ---------------------------------------------------------------------------
//
void CPsetCli::SetRequestStatus( RMobilePhone::TMobilePhoneIdService aStatus )
    {
    iRequestedService = aStatus;
    if ( iReqObserver )
        {
        iReqObserver->RequestStatusChanged( aStatus );
        }
    }

// End of file
