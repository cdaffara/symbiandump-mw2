/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPsetNetwork selects network and gets network information.
*
*
*/


//  INCLUDE FILES
#include "mpsetnetworkinfoobs.h" 
#include "psetnetwork.h" 
#include "psetsaobserver.h" 
#include "psetpanic.h" 
#include "psetconstants.h" 
#include "mpsetnetworkselect.h" 
#include <etelmm.h>
#include <mmretrieve.h>
#include <rmmcustomapi.h>

#include <e32svr.h>
#include "phonesettingslogger.h" 

//  LOCAL CONSTANTS AND MACROS  
_LIT( KPSNameOfClass, "CPsetNetwork" );

enum TActiveObserver
    {
    ENetSelectObserver,
    ENetModeObserver
    };

//  MEMBER FUNCTIONS
//=============================================================================
// ---------------------------------------------------------------------------
// 
// 1st phase constructor.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetNetwork* CPsetNetwork::NewL( RMobilePhone& aPhone, 
    MPsetNetworkInfoObserver& aObserver )
    {
    CPsetNetwork* self = new ( ELeave ) CPsetNetwork( aPhone );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// 
// 1st phase constructor for net mode observer.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetNetwork* CPsetNetwork::NewL( RMobilePhone& aPhone, 
                       MPsetNetworkModeObserver& aModeObserver )
    {
    CPsetNetwork* self = new ( ELeave ) CPsetNetwork( aPhone );
    CleanupStack::PushL( self );
    self->ConstructL( aModeObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// 
// Destructor.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetNetwork::~CPsetNetwork()
    {
    Cancel();
    iCustomPhone.Close();
    delete iNetworkRetrieve;
    iNetworkRetrieve = NULL;
    delete iSAObserver;
    iSAObserver = NULL;    
    } 

// ---------------------------------------------------------------------------
// 
// Returns available networks
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetNetwork::GetAvailableNetworksL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetNetwork::GetAvailableNetworksL");
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }

    if ( iNetworkRetrieve )
        {
        delete iNetworkRetrieve;
        iNetworkRetrieve = NULL;
        }
    
    // Start to request for network information.
    iNetworkRetrieve = CRetrieveMobilePhoneDetectedNetworks::NewL( iPhone );
    iNetworkRetrieve->StartV2( iStatus );    
    iServiceRequest = MPsetNetworkInfoObserver::EServiceRequestGetNetworkInfo;
    SetActive();
    
    // Show searching note.
    CleanupLeavePushL();
    iObserver->HandleSearchingNetworksL( iServiceRequest );
    CleanupStack::Pop(); // CleanupCancelPushL
    __PHSLOGSTRING("[PHS] <--CPsetNetwork::GetAvailableNetworksL");
    }

// ---------------------------------------------------------------------------
// 
// Returns information of the currently selected network.
// DEPRECATED.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetNetwork::GetCurrentNetworkInfo
                ( MPsetNetworkSelect::TCurrentNetworkInfo& /*aInfo*/ )
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// 
// Retrieve current search mode (automatic or manual)
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetNetwork::GetNetworkSelectMode( TSelectMode& aMode )
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    RMobilePhone::TMobilePhoneNetworkSelectionV1 setMode;
    RMobilePhone::TMobilePhoneNetworkSelectionV1Pckg setModePckg( setMode );
    
    __PHSLOGSTRING("[PHS]--> CPsetNetwork::GetNetworkSelectMode");

    TInt res( iPhone.GetNetworkSelectionSetting( setModePckg ) );
    
    if ( res == KErrNone )
        {
        if ( setMode.iMethod == RMobilePhone::ENetworkSelectionAutomatic )
            {
            aMode = ENetSelectModeAutomatic;
            }
        else
            {
            aMode = ENetSelectModeManual;
            }
        iTempNetInfo.iMode = aMode;
        }
    __PHSLOGSTRING("[PHS] <--CPsetNetwork::GetNetworkSelectMode");
    return res;
    }

// ---------------------------------------------------------------------------
// 
// Starts using the indicated network.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetNetwork::SelectNetworkL( const TNetworkInfo& aInfo )
    {
    __PHSLOGSTRING("[PHS]--> CPsetNetwork::SelectNetworkL");
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }    

    //Save previous network mode.
    TBool previousModeAutomatic = EFalse;
    if ( iTempNetInfo.iMode == ENetSelectModeAutomatic )
        {
        previousModeAutomatic = ETrue;
        }
    iTempNetInfo = aInfo;

    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    if ( aInfo.iMode == ENetSelectModeManual )
        {        
        //Copy data to member variables and make a request.
        iNwInfo.iCountry.Append( aInfo.iId.iCountryCode );
        __PHSLOGSTRING1("[PHS]   CPsetNetwork::SelectNetworkL: Manual - CountryCode: %S", &aInfo.iId.iCountryCode);

        iNwInfo.iNetwork.Append( aInfo.iId.iNetworkCode );        
        __PHSLOGSTRING1("[PHS]   CPsetNetwork::SelectNetworkL: Manual - NetworkCode: %S", &aInfo.iId.iNetworkCode);

        iServiceRequest = MPsetNetworkInfoObserver::EServiceRequestSetNetwork;
        iPhone.SelectNetwork( iStatus, ETrue, iNwInfo );
        }
    else
        {
        iServiceRequest = 
            MPsetNetworkInfoObserver::EServiceRequestSetNetworkAutomatic;
        //If previously automatic, and now automatic, issue request
        // to search networks.
        if ( previousModeAutomatic )
            {
            iNwInfo.iCountry.Append( aInfo.iId.iCountryCode );            
            __PHSLOGSTRING1("[PHS]   CPsetNetwork::SelectNetworkL: Auto - CountryCode: %S", &aInfo.iId.iCountryCode);

            iNwInfo.iNetwork.Append( aInfo.iId.iNetworkCode );
            __PHSLOGSTRING1("[PHS]   CPsetNetwork::SelectNetworkL: Auto - NetworkCode: %S", &aInfo.iId.iNetworkCode);

            iPhone.SelectNetwork( iStatus, EFalse, iNwInfo );
            }
        // make new selection without Network/Country code this way device 
        // connects automatically home network.
        else
            {    
            iNwInfo.iCountry = KNullDesC;
            iNwInfo.iNetwork = KNullDesC;
            __PHSLOGSTRING("[PHS]   CPsetNetwork::SelectNetworkL: Manual>Auto - CountryCode = KNullDesC");
            __PHSLOGSTRING("[PHS]   CPsetNetwork::SelectNetworkL: Manual>Auto - NetworkCode = KNullDesC");
            iPhone.SelectNetwork( iStatus, EFalse, iNwInfo );
            }
        }    
    SetActive();
    
    //Show requesting note.
    CleanupLeavePushL();    
    if ( aInfo.iMode == ENetSelectModeManual )
        {
        __PHSLOGSTRING("[PHS]--> CPsetNetwork::SelectNetworkL: Requesting Selected N/W");
        iObserver->HandleRequestingSelectedNetworkL( ETrue );
        }
    else
        {
        __PHSLOGSTRING("[PHS]--> CPsetNetwork::SelectNetworkL: Searching N/Ws");
        iObserver->HandleSearchingNetworksL( iServiceRequest );
        }    
    CleanupStack::Pop(); // CleanupCancelPushL
    __PHSLOGSTRING("[PHS] <--CPsetNetwork::SelectNetworkL");
    }

// ---------------------------------------------------------------------------
// 
// Running the active object
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::RunL()
    {
    __PHSLOGSTRING1("[PHS]--> CPsetNetwork::RunL() - iStatus = %d", iStatus.Int());

    MPsetNetworkInfoObserver::TServiceRequest serviceRequest = iServiceRequest;
    MPsetNetworkModeObserver::TServiceRequest modeRequest = iModeRequest;

    // Params have to be cleared here since the observer may initiate another asynchronous operation immediately
    // after they are notified, effectively messing up the state machine which controls the logic in DoCancel()
    // resulting in a possible dead lock when CActive::Cancel()'s User::WaitForRequest( iStatus ) never gets signaled.
    ClearParams();
    
    if ( iActiveObserver == ENetSelectObserver && iObserver )
        {
        __ASSERT_ALWAYS( iObserver, Panic( KPSNameOfClass, ENetNoObserver ) );
        HideRequestNoteL();
        }
    else if ( iActiveObserver == ENetModeObserver && iNetworkModeObserver )
        {        
        __ASSERT_ALWAYS( iNetworkModeObserver, 
                     Panic( KPSNameOfClass, ENetNoNetworkObserver  ) );
        }
        
    if ( iStatus != KErrNone )
        {
        if ( modeRequest == 
              MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode || 
             modeRequest == 
              MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode )
            {
            __PHSLOGSTRING("[PHS]--> CPsetNetwork::RunL: Request is modeRequest");
            //PSetNetworkModeObs
            iNetworkModeObserver->HandleNetworkErrorL( modeRequest, iStatus.Int() );
            }
        else
            {
            __PHSLOGSTRING("[PHS]--> CPsetNetwork::RunL: Request is serviceRequest");
            //PSetNetworkInfoObs
            GetRegistrationStatusL();
            iObserver->HandleNetworkErrorL( serviceRequest, iStatus.Int() );
            }
        } 
    else // Request completed successfully.
        {
        switch ( serviceRequest )
            {
            case MPsetNetworkInfoObserver::EServiceRequestGetNetworkInfo:
                {
                CNetworkInfoArray* array = new ( ELeave ) 
                    CNetworkInfoArray( KPSetNetworkInfoArrayGranularity );
                CleanupStack::PushL( array );

                CMobilePhoneNetworkListV2* results = 
                    iNetworkRetrieve->RetrieveListV2L();
                CleanupStack::PushL( results );
                TInt amount = results->Enumerate();
                __PHSLOGSTRING1("[PHS]   CPsetNetwork::RunL: amount: %d", amount);
                TInt i = 0;
                /*****************************************************
                *    Series 60 Customer / ETel
                *    Series 60  ETel API
                *****************************************************/
                RMobilePhone::TMobilePhoneNetworkInfoV2 nwNames;
                while ( amount > i ) 
                    {
                    nwNames = results->GetEntryL( i );
                    iTempNetInfo.iId.iCountryCode.Copy( nwNames.iCountryCode );
                    __PHSLOGSTRING1("[PHS]   CPsetNetwork::RunL: CountryCode: %S", &iTempNetInfo.iId.iCountryCode);

                    iTempNetInfo.iId.iNetworkCode.Copy( nwNames.iNetworkId );
                    __PHSLOGSTRING1("[PHS]   CPsetNetwork::RunL: NetworkCode: %S", &iTempNetInfo.iId.iNetworkCode);
                    
                    iTempNetInfo.iStatus = (MPsetNetworkSelect::TNetworkStatus)nwNames.iStatus;
                    __PHSLOGSTRING1("[PHS]   CPsetNetwork::RunL: Status: %d", &iTempNetInfo.iStatus);

                    iTempNetInfo.iLongName.Copy( nwNames.iLongName );
                    __PHSLOGSTRING1("[PHS]   CPsetNetwork::RunL: LongName: %S", &iTempNetInfo.iLongName);

                    iTempNetInfo.iShortName.Copy( nwNames.iShortName );
                    __PHSLOGSTRING1("[PHS]   CPsetNetwork::RunL: ShortName: %S", &iTempNetInfo.iShortName);

                    if ( nwNames.iAccess == RMobilePhone::ENetworkAccessUtran )
                        {
                        iTempNetInfo.iAccess = ENetNetworkWCDMA;
                        }
                    else if ( nwNames.iAccess == RMobilePhone::ENetworkAccessGsmAndUtran )
                        {
                        iTempNetInfo.iAccess = ENetNetworkGSMandWCDMA;
                        }
                    else
                        {
                        iTempNetInfo.iAccess = ENetNetworkGSM;
                        }
                    array->AppendL( iTempNetInfo );
                    i++;       
                    }
                CleanupStack::PopAndDestroy( results );
                results = NULL;
                __PHSLOGSTRING1("[PHS]   CPsetNetwork::RunL: Count: %d", array->Count());
                iObserver->HandleNetworkInfoReceivedL( array, KErrNone );
                CleanupStack::PopAndDestroy( array );
                array = NULL;
                break;        
                }
            case MPsetNetworkInfoObserver::EServiceRequestSetNetwork:
                {
                __PHSLOGSTRING("[PHS]   CPsetNetwork::RunL: EServiceRequestSetNetwork");
                GetRegistrationStatusL();
                iObserver->HandleNetworkChangedL( iTempNetInfo,
                    iRegStatus, KErrNone );
                break;
                }
            default:
                break;
            }

        //check if this is correct
        switch ( modeRequest )
            {
            case MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode:
                __PHSLOGSTRING("[PHS]   CPsetNetwork::RunL: EServiceRequestSetSelectedNetworkMode");
                iNetworkModeObserver->HandleNetworkSystemModeEventsL( 
                                                       modeRequest,
                                                       iNetworkModeCaps );
                break;
            case MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode:
                __PHSLOGSTRING("[PHS]   CPsetNetwork::RunL: EServiceRequestGetCurrentNetworkMode");
                iNetworkModeObserver->HandleNetworkSystemModeEventsL( 
                                                       modeRequest,
                                                       iNetworkModeCaps );
                break;            
            default:
                break;
            }
        }
    __PHSLOGSTRING("[PHS] <--CPsetNetwork::RunL");
    }

// ---------------------------------------------------------------------------
// 
// Cancels pending request
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::DoCancel()
    {
    __PHSLOGSTRING("[PHS]--> CPsetNetwork::DoCancel");
    switch ( iServiceRequest )
        {
        case MPsetNetworkInfoObserver::EServiceRequestGetNetworkInfo:
            iNetworkRetrieve->Cancel();
            delete iNetworkRetrieve;
            iNetworkRetrieve = NULL;
            break;
        case MPsetNetworkInfoObserver::EServiceRequestSetNetwork:
        case MPsetNetworkInfoObserver::EServiceRequestSetNetworkAutomatic:
            iPhone.CancelAsyncRequest( EMobilePhoneSelectNetwork );
            break;
        default:            
            break;
        }

    switch ( iModeRequest )
        {
        case MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode:
            iCustomPhone.CancelAsyncRequest( ECustomSetSystemNetworkModeIPC );
            break;
        case MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode:
            __PHSLOGSTRING("[PHS]--> CPsetNetwork::EServiceRequestGetCurrentNetworkMode");
            iCustomPhone.CancelAsyncRequest( 
                ECustomGetCurrentSystemNetworkModesIPC );
            __PHSLOGSTRING("[PHS]--> CPsetNetwork::EServiceRequestGetCurrentNetworkMode 2");
            break;
        default:
            break;
        }

    ClearParams();
    __PHSLOGSTRING("[PHS] <--CPsetNetwork::DoCancel");
    }

// ---------------------------------------------------------------------------
// 
// Cancels current request
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetNetwork::CancelProcess()
    {
    if ( !IsActive() )
        {
        //if not active, might mean that network list is shown, 
        // but call is incoming do not leave or raise error.
        }
    
    Cancel();
    }

// ---------------------------------------------------------------------------
// 
// Sets observer (Observer starts to listen again if reset).
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetNetwork::SetNetSAObserver( MPsetNetworkInfoObserver& aObserver )
    {
    __ASSERT_ALWAYS( iSAObserver, Panic( KPSNameOfClass, ENetNoObserver ) );
    iSAObserver->SetNetObserver( aObserver );
    }

// ---------------------------------------------------------------------------
// 
// Adds this to scheduler
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::ConstructL( MPsetNetworkInfoObserver& aObserver )
    {
    iObserver = &aObserver;
    iActiveObserver = ENetSelectObserver;
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    iRegStatus = RMobilePhone::ERegistrationUnknown;
    User::LeaveIfError( iCustomPhone.Open( iPhone ) );
    
    iSAObserver = CPsetSAObserver::NewL();
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// 
// Adds this to scheduler
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::ConstructL( MPsetNetworkModeObserver& aModeObserver )
    {
    iNetworkModeObserver = &aModeObserver;
    iActiveObserver = ENetModeObserver;
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    User::LeaveIfError( iCustomPhone.Open( iPhone ) );
    
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
//
CPsetNetwork::CPsetNetwork
                ( RMobilePhone& aPhone ) : CActive( EPriorityStandard ),
                                           iPhone ( aPhone )
    {
    }

// ---------------------------------------------------------------------------
// 
// Gets registration status.
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::GetRegistrationStatusL()
    {
    TRequestStatus status;
    iPhone.GetNetworkRegistrationStatus( status, iRegStatus );
    __PHSLOGSTRING1("[PHS]   CPsetNetwork::GetRegistrationStatusL: iRegStatus: %d", iRegStatus);
    User::WaitForRequest( status );
    User::LeaveIfError(status.Int());
    }

// ---------------------------------------------------------------------------
// 
// Requests SAObserver to check if call is active.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetNetwork::IsCallActive()
    {
    return iSAObserver->IsCallActive();
    }

// ---------------------------------------------------------------------------
// 
// Requests SAObserver to check if gprs connection is active.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetNetwork::IsGPRSConnected()
    {
    return iSAObserver->IsGPRSConnected();
    }

// ---------------------------------------------------------------------------
// 
// Requests ETEL to Reset back to the previously used network.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetNetwork::ResetNetworkSearch()
    {
    __PHSLOGSTRING("[PHS]--> CPsetNetwork::ResetNetworkSearch");
    RMmCustomAPI customApi;
    TInt error = customApi.Open( iPhone );
    if  (error == KErrNone)
        {
        customApi.ResetNetServer();
        }
    customApi.Close();

    __PHSLOGSTRING("[PHS] <--CPsetNetwork::ResetNetworkSearch");
    //for compatibility reasons returns a value
    return error;
    }

// ---------------------------------------------------------------------------
// 
// Hides requesting note (either type). Cannot leave, since only deletes notes.
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::HideRequestNoteL()
    {
    // Hide requesting note, when interrupting a note, does not leave.
    if ( iServiceRequest == 
        MPsetNetworkInfoObserver::EServiceRequestGetNetworkInfo )
        {
        iObserver->HandleSearchingNetworksL( // Does not leave
            MPsetNetworkInfoObserver::EServiceRequestNone ); 
        }
    else //may cause unnecessary call to hide non-existing note
        {
        iObserver->HandleRequestingSelectedNetworkL( EFalse ); // Does not leave
        }
    }

// ---------------------------------------------------------------------------
// 
// Push object into cleanupstack to catch leaving.
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::CleanupLeavePushL()
    {
    CleanupStack::PushL( TCleanupItem( DoHandleLeave, this ) );
    }

// ---------------------------------------------------------------------------
// 
// Object has caused a leave.
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::DoHandleLeave( TAny* aAny )
    {
    REINTERPRET_CAST( CPsetNetwork*, aAny )->HandleLeave();
    }

// ---------------------------------------------------------------------------
// 
// Things to do when leave occurs.
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::HandleLeave()
    {
    CancelProcess();
    }

// ---------------------------------------------------------------------------
// 
// Clears member variables
// 
// ---------------------------------------------------------------------------
//
void CPsetNetwork::ClearParams()
    {
    iNwInfo.iCountry = KNullDesC;
    iNwInfo.iNetwork = KNullDesC;
    iServiceRequest = MPsetNetworkInfoObserver::EServiceRequestNone;
    iModeRequest = MPsetNetworkModeObserver::EServiceRequestNone;
    }


// ---------------------------------------------------------------------------
// 
// Sets the network mode observer for communication.
// This method is called from CGSSettListNetView class.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetNetwork::SetNetworkModeObserver( MPsetNetworkModeObserver& 
                                                    aObserver )
    {
    iNetworkModeObserver = &aObserver;
    }


// ---------------------------------------------------------------------------
// 
// Sets the currently selected network access mode (GSM/UMTS/DualMode).
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetNetwork::SetNetworkModeSelectionL( TUint32& aNetworkMode )
    {
    __PHSLOGSTRING("[PHS]--> CPsetNetwork::SetNetworkModeSelectionL");

    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }

    iModeRequest = 
        MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode;
    iCustomPhone.SetSystemNetworkMode( iStatus, 
                 static_cast<RMmCustomAPI::TNetworkModeCaps>( aNetworkMode ) );
    SetActive();

    __PHSLOGSTRING("[PHS] <--CPsetNetwork::SetNetworkModeSelectionL");
    }


// ---------------------------------------------------------------------------
// 
// Gets the currently selected network access mode (GSM/UMTS/DualMode).
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetNetwork::GetCurrentNetworkModeSelectionL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetNetwork::GetCurrentNetworkModeSelectionL");

    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }

    iModeRequest = 
        MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode;
    iCustomPhone.GetCurrentSystemNetworkModes( iStatus, iNetworkModeCaps );
    SetActive();

    __PHSLOGSTRING("[PHS] <--CPsetNetwork::GetCurrentNetworkModeSelectionL");
    }


// End of file
