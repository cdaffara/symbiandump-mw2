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
* Description:  CPsetCallWaiting is call waiting SS setting abstraction.
*
*
*/


//  Include Files
#include "psetcallwaiting.h" 
#include "mpsetcallwaitingobs.h" 
#include "nwdefs.h"             
#include "psetpanic.h" 
#include "psetconstants.h" 
#include "mpsetrequestobs.h" 
#include "psetutility.h" 
#include "phonesettingslogger.h" 

#include "psetvariationproxy.h" 
#include <settingsinternalcrkeys.h> 

// CONSTANTS    
_LIT( KPSNameOfClass, "CPsetCallWaiting" );

//  MEMBER FUNCTIONS
// ---------------------------------------------------------------------------
// 
// Symbian OS 1st phase constructor.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCallWaiting* CPsetCallWaiting::NewL( 
    RMobilePhone& aPhone, MPsetCallWaitingObserver& aObserver )
    {
    CPsetCallWaiting* self = new ( ELeave ) CPsetCallWaiting( aPhone );  
    CleanupStack::PushL( self );
    self->ConstructL(aObserver);
    CleanupStack::Pop( self );         
    return self;
    }

// ---------------------------------------------------------------------------
// 
// 2nd phase constructor.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::ConstructL(MPsetCallWaitingObserver& aObserver)
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallWaiting::ConstructL" );
    this->SetPsuiObserver( aObserver );
    
    //Check which line is used, or if ALS is not active.
    TInt usedLine = ESSSettingsAlsNotSupported;

    iSsSettings = new (ELeave) RSSSettings;
    User::LeaveIfError( iSsSettings->Open() );
    TRAPD( err, iSsSettings->Register( ESSSettingsAls, *this ) );
    if ( err == KErrNone )
        {
        iSsSettings->Get( ESSSettingsAls, usedLine );
        }
        
    CPSetVariationProxy* variationProxy = 
        CPSetVariationProxy::NewL( KCRUidPhoneSettings, KPSetCallWaiting );
    CleanupStack::PushL( variationProxy );
    iCallWaitingDistinquishEnabled = variationProxy->IsFeatureEnabled( KCallWaitingDistiquishNotProvisioned );
    CleanupStack::PopAndDestroy( variationProxy );

    iAls = static_cast <TSSSettingsAlsValue> ( usedLine );
    __PHSLOGSTRING1("[PHS]    CPsetCallWaiting::ConstructL: usedLine: %d", usedLine );
    __PHSLOGSTRING("[PHS] <--CPsetCallWaiting::ConstructL" );
    }

// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
//
CPsetCallWaiting::CPsetCallWaiting( RMobilePhone& aPhone) 
                : CActive( EPriorityStandard ),
                  iPhone ( aPhone )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// 
// Destructor
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCallWaiting::~CPsetCallWaiting()
    {
    Cancel();
    iReqObserver = NULL;

    if ( iSsSettings )
        {
        iSsSettings->Cancel( ESSSettingsAls, *this ); 
        iSsSettings->Close();
        }
    delete iSsSettings;
    iSsSettings = NULL;
    
    delete iCwInterrogator;
    iCwInterrogator = NULL;
    }
    
// ---------------------------------------------------------------------------
// 
// If SsSettings notifies of settings change, copy new value to member variable.
// 
// ---------------------------------------------------------------------------
void CPsetCallWaiting::PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue )
    {
    __PHSLOGSTRING("[PHS]--> PhoneSettingChanged::ValidateBsc" );
    if ( aSetting == ESSSettingsAls )
        {
        iAls = static_cast <TSSSettingsAlsValue> (aNewValue);
        }
    __PHSLOGSTRING("[PHS] <--PhoneSettingChanged::ValidateBsc" );
    }

// ---------------------------------------------------------------------------
// 
// Validates used bsc, if EUnknown, updates to real value
// 
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::ValidateBsc( TBasicServiceGroups& aBsc )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallWaiting::ValidateBsc" );
    if ( aBsc == EUnknown )
    // if bsc is unknown, update it
        {
        if ( iAls == ESSSettingsAlsAlternate )
            {
            aBsc = EAltTele;
            }
        else
            {
            aBsc = ETelephony;
            }
        }    
    else
    // If no specific line is indicated
        {
        if ( aBsc == EAllTeleAndBearer || aBsc == EAllTele )
            {
            // if ALS is used, use 89 bsc
            if ( iAls == ESSSettingsAlsAlternate )
                {
                aBsc = EAltTele;
                }
            }
        }
    __PHSLOGSTRING("[PHS] <--CPsetCallWaiting::ValidateBsc" );
    }

// ---------------------------------------------------------------------------
// 
// Sets call waiting.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallWaiting::SetCallWaitingL
    ( TSetCallWaiting aSetting, TBasicServiceGroups aBsc )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallWaiting::SetCallWaitingL");
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    RMobilePhone::TMobilePhoneServiceAction cwAction = 
        RMobilePhone::EServiceActionUnspecified;
    
    RMobilePhone::TMobileService cwBasicServiceGroup;

    ValidateBsc( aBsc );

    if ( aBsc == EAltTele )
        {
        cwBasicServiceGroup = PSetUtility::VerifyAltLineUseL();
        }

    cwBasicServiceGroup = PSetUtility::ChangeToEtelInternal( aBsc );

    if ( aSetting == EActivateCallWaiting )
        {
        cwAction = RMobilePhone::EServiceActionActivate;
        }
    else 
        {
        cwAction = RMobilePhone::EServiceActionDeactivate;
        }

    //Copy data to member variables.
    iSetValue = aSetting;

    //Start requesting for setting CW.
    iPhone.SetCallWaitingStatus( iStatus, cwBasicServiceGroup, cwAction);

    SetActive();
    SetRequestStatus( EPSetServiceRequestSetCallWaiting );

    //Set PSUI so that correct observer is used and show requesting note.
    CleanupLeavePushL();
    iObserver->SetEngineContact( *this );
    iObserver->HandleCWRequestingL( ETrue, EFalse );
    CleanupStack::Pop();
    __PHSLOGSTRING("[PHS] <--CPsetCallWaiting::SetCallWaitingL");
    }

// ---------------------------------------------------------------------------
// 
// Requests call waiting status.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallWaiting::GetCallWaitingStatusL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallWaiting::GetCallWaitingStatusL");
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }    

    SetRequestStatus( EPSetServiceRequestGetCallWaitingStatus );

    //Start requesting for CW status.
    iCwInterrogator = CRetrieveMobilePhoneCWList::NewL( iPhone );
    iCwInterrogator->Start( iStatus );
    
    SetActive();

    //Set PSUI so that correct observer is used and show requesting note.
    CleanupLeavePushL();
    iObserver->SetEngineContact( *this );
    iObserver->HandleCWRequestingL( ETrue, EFalse );
    CleanupStack::Pop(); //CleanupLeavePushL
    __PHSLOGSTRING("[PHS] <--CPsetCallWaiting::GetCallWaitingStatusL");
    }

// ---------------------------------------------------------------------------
// 
// Cancel process.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetCallWaiting::CancelProcess()
    {
    // Set PSUI so that correct observer is used and hide requesting note.
    // When interrupting a note, does not leave.
    // This needs to be done first, since notes must be cleared from screen
    // even though request is not active.
    iObserver->SetEngineContact( *this );
    // Does not leave
    TRAPD( err, iObserver->HandleCWRequestingL( EFalse, ETrue ) );
    
    if ( !IsActive() || err != KErrNone )
        {
        return KErrGeneral;
        }            
    Cancel();

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Sets request observer.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallWaiting::SetRequestObserver( MPsetRequestObserver* aObs )
    {
    iReqObserver = aObs;
    }

// ---------------------------------------------------------------------------
// 
// Run active object.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::RunL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallWaiting::RunL");

    __ASSERT_ALWAYS( iObserver, Panic( KPSNameOfClass, ECWPanicNoObserver ) );

    // Set PSUI so that correct observer is used and hide requesting note.
    // does not leave when deleting a note.
    iObserver->SetEngineContact( *this );
    iObserver->HandleCWRequestingL( EFalse, EFalse );

    if ( iStatus != KErrNone )
        {
        iObserver->HandleCWErrorL( iStatus.Int() );
        RequestCompleted( iStatus.Int() );
        return;
        } 

    TUint8 arrayOfBsc[KPSetNumberOfBsc];
    switch ( iServiceRequest )
        {
        case EPSetServiceRequestGetCallWaitingStatus:
            {
            __PHSLOGSTRING("[PHS]    CPsetCallWaiting::RunL: EPSetServiceRequestGetCallWaitingStatus");
            CMobilePhoneCWList* cwStatusList = iCwInterrogator->RetrieveListL();
            CleanupStack::PushL( cwStatusList );
            TGetCallWaitingStatus status = EStatusUnknown;

            /*****************************************************
            *    Series 60 Customer / ETel
            *    Series 60  ETel API
            *****************************************************/
            RMobilePhone::TMobilePhoneCWInfoEntryV1 cwInfo;
            TInt entries = cwStatusList->Enumerate();
            TInt i = 0;
            while ( entries > i)
                {
                cwInfo = cwStatusList->GetEntryL( i );
                if ( cwInfo.iStatus == RMobilePhone::ECallWaitingStatusActive )
                    {
                    status = TGetCallWaitingStatus( cwInfo.iStatus );
                    }
                    
                if ( iCallWaitingDistinquishEnabled && status == EStatusUnknown 
                    && cwInfo.iStatus != RMobilePhone::ECallWaitingStatusUnknown )
                    {
                    // pass on the status if the current status is still 
                    // unknown and the received status is not unknown
                    switch ( cwInfo.iStatus )
                        {
                        case RMobilePhone::ECallWaitingStatusNotProvisioned:
                            {
                            status = EStatusNotProvisioned;
                            break;
                            }   
                            
                            case RMobilePhone::ECallWaitingStatusNotAvailable:
                            {       
                            status = EStatusNotAvailable;
                            break;
                            }
                            
                            default:
                            {   
                            status = TGetCallWaitingStatus( cwInfo.iStatus );
                            break;      
                            }
                        }
                    }
                    
                arrayOfBsc[i] = static_cast <TUint8> 
                    ( PSetUtility::ChangeToGSM( cwInfo.iServiceGroup ) );
                i++;                                                                
                }
            arrayOfBsc[i++] = KPSetUnusedValue;
            CleanupStack::PopAndDestroy( cwStatusList );
            iObserver->SetEngineContact( *this );
            iObserver->HandleCallWaitingGetStatusL( status, arrayOfBsc );
            }
            break;
        case EPSetServiceRequestSetCallWaiting:
            __PHSLOGSTRING("[PHS]    CPsetCallWaiting::RunL: EPSetServiceRequestSetCallWaiting");
            // Set PSUI so that correct observer is used and show note.
            iObserver->SetEngineContact( *this );
            iObserver->HandleCallWaitingChangedL( iSetValue, iStatus.Int() );
            break;
        default:
            Panic( KPSNameOfClass, ECWPanicInvalidRequestMode );
        }
    RequestCompleted( KErrNone );
    
    __PHSLOGSTRING("[PHS] <--CPsetCallWaiting::RunL");
    }

// ---------------------------------------------------------------------------
// 
// Cancels active object.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::DoCancel()
    {    
    switch ( iServiceRequest )
        {
        case EPSetServiceRequestGetCallWaitingStatus:
            iCwInterrogator->Cancel();
            delete iCwInterrogator;
            iCwInterrogator = NULL;
            break;
        case EPSetServiceRequestSetCallWaiting:
            iPhone.CancelAsyncRequest( EMobilePhoneSetCallWaitingStatus );
            break;
        default:
            break;
        } 
    SetRequestStatus( EPSetServiceRequestNone );
    }

// ---------------------------------------------------------------------------
// 
// Sets observer (member variable)
//  
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::SetPsuiObserver( MPsetCallWaitingObserver& aObserver )
    {
    iObserver = &aObserver;
    iObserver->SetEngineContact( *this );
    }

// ---------------------------------------------------------------------------
// 
// Push object into cleanupstack to catch leaving.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::CleanupLeavePushL()
      {
      CleanupStack::PushL( TCleanupItem( DoHandleLeave, this ) );
      }

// ---------------------------------------------------------------------------
// 
// Object has caused a leave.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::DoHandleLeave( TAny* aAny )
      {
      REINTERPRET_CAST( CPsetCallWaiting*, aAny )->HandleLeave();
      }

// ---------------------------------------------------------------------------
// 
// Things to do when leave occurs.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::HandleLeave()
    {
    CancelProcess();
    }

// ---------------------------------------------------------------------------
// Inform the request observer that request has been completed.
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::RequestCompleted( const TInt& aError )
    {
    if ( aError != KErrNone )
        {
        SetRequestStatus( static_cast <TPSetServiceRequest> (aError) );
        }
    SetRequestStatus( EPSetServiceRequestNone );

    if ( iReqObserver )
        {
        iReqObserver->RequestComplete(); 
        }
    }

// ---------------------------------------------------------------------------
// Update the request status - both internally and to observers.
// ---------------------------------------------------------------------------
//
void CPsetCallWaiting::SetRequestStatus( TPSetServiceRequest aStatus )
    {
    iServiceRequest = aStatus;
    if ( iReqObserver )
        {
        iReqObserver->RequestStatusChanged( aStatus );
        }
    }

// End of file
