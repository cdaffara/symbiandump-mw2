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
* Description:  Call barring class. Sets and gets call barring setting.
*
*
*/


//  Include Files
#include <etelmm.h>
#include <rsssettings.h> 
#include "mpsetrequestobs.h" 
#include "psetcallbarring.h" 
#include "psettelephony.h" 
#include "psetpanic.h" 
#include "psetconstants.h" 
#include "psetutility.h" 
#include "phonesettingslogger.h" 

// CONSTANTS    
_LIT( KPSNameOfClass, "CPsetCallBarring" );

//  MEMBER FUNCTIONS
//=============================================================================
// ---------------------------------------------------------------------------
// 
// 1st phase constructor.
// 
// ---------------------------------------------------------------------------
//
CPsetCallBarring* CPsetCallBarring::NewL( MPsetBarringObserver& aObserver, 
                  RMobilePhone& aPhone ) 
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::NewL");
    CPsetCallBarring* self = new( ELeave ) CPsetCallBarring( aPhone );

    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop();

    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::NewL");
    return self;
    }

// ---------------------------------------------------------------------------
// 
// 2nd phase constructor.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::ConstructL( MPsetBarringObserver& aObserver )
    {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::ConstructL");
    iLine = new ( ELeave ) RMobileLine;
    this->SetObserver( aObserver );
    CActiveScheduler::Add( this );
    
    //Check which line is used, or if ALS is not active.
    TInt usedLine = ESSSettingsAlsNotSupported;

    iSsSettings = new (ELeave) RSSSettings;
    User::LeaveIfError( iSsSettings->Open() );
    TRAPD( err, iSsSettings->Register( ESSSettingsAls, *this ) );
    if ( err == KErrNone )
        {
        iSsSettings->Get( ESSSettingsAls, usedLine );
        }

    iAls = static_cast <TSSSettingsAlsValue> ( usedLine );
    
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::ConstructL");
    }

// ---------------------------------------------------------------------------
// Destructor.
// Closes line subsession.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPsetCallBarring::~CPsetCallBarring()
    {
    Cancel();
    if ( iLine )
        {
        iLine->Close();    
        }
    delete iLine;
    
    if ( iSsSettings )
        {
        iSsSettings->Cancel( ESSSettingsAls, *this ); 
        iSsSettings->Close();
        }
    delete iSsSettings;
    
    iReqObserver = NULL;
        
    delete iCbStatusInterrogator;
    iCbStatusInterrogator = NULL;
    }

// ---------------------------------------------------------------------------
// 
// If SsSettings notifies of settings change, copy new value to member variable.
// 
// ---------------------------------------------------------------------------
void CPsetCallBarring::PhoneSettingChanged( 
    TSSSettingsSetting aSetting, TInt aNewValue )
    {
    if ( aSetting == ESSSettingsAls )
        {
        iAls = static_cast <TSSSettingsAlsValue> (aNewValue);
        }
    }

// ---------------------------------------------------------------------------
// 
// Validates used bsc, if EUnknown, updates to real value
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::ValidateBsc( TBasicServiceGroups& aBsc )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::ValidateBsc" );
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
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::ValidateBsc" );
    }

// ---------------------------------------------------------------------------
// 
// Sets barring setting on network.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallBarring::SetBarringL( 
    const TCallBarringSetting& aBarring, TBasicServiceGroups aBsc )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::SetBarringL");
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }

    __PHSLOGSTRING1("[PHS]    SetBarringL: Barring Group: %d", aBarring.iServiceGroup);
    //Opens the correct basic service group->contradicting custom tsy.
    CPsetTelephony::CheckLineModeL( aBarring.iServiceGroup, &iPhone, iLine );

    // Copy received information to member and local variables.
    // Note if alternate line selected then iServiceGroup changes in iBarringParameters 
    // and iChangeSetting iSericeGroup has old information.
    iChangeSetting = aBarring;

    __PHSLOGSTRING1("[PHS]    SetBarringL: Password: %S", &aBarring.iPassword);
    __PHSLOGSTRING1("[PHS]    SetBarringL: Settings: %d", aBarring.iSetting);

    ValidateBsc( aBsc );
    
    if ( aBsc == EAltTele )
        {
        iBarringParameters.iServiceGroup = PSetUtility::VerifyAltLineUseL();
        }
                
    iBarringParameters.iPassword = aBarring.iPassword;
    iBarringParameters.iAction = SetBarringAction( aBarring.iSetting );
    iBarringParameters.iServiceGroup = PSetUtility::ChangeToEtelInternal( aBsc );
        
    __PHSLOGSTRING1("[PHS]    SetBarringL: Program: %d", aBarring.iType);

    // Start to request barring change.
    iPhone.SetCallBarringStatus( 
        iStatus, 
        SetBarringProgram( aBarring.iType ), 
        iBarringParameters );

    StartRequestingL( EPSetRequestChangeBarring );
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::SetBarringL");
    }

// ---------------------------------------------------------------------------
// 
// Gets barring status from network. 
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallBarring::GetBarringStatusL( const TServiceGroup aGroup, 
    const TBarringProgram aMode )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::GetBarringStatusL");
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }

    __PHSLOGSTRING1("[PHS]    GetBarringStatusL: Barring Group: %d", aGroup);
    __PHSLOGSTRING1("[PHS]    GetBarringStatusL: Program: %d", aMode);

    CPsetTelephony::CheckLineModeL( aGroup, &iPhone, iLine );

    // Start to request for barring status.
    iCbStatusInterrogator = CRetrieveMobilePhoneCBList::NewL( iPhone );
    iCbStatusInterrogator->Start( iStatus, SetBarringProgram( aMode ) );

    StartRequestingL( EPSetRequestGetStatus );
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::GetBarringStatusL");
    }

// ---------------------------------------------------------------------------
// 
// Cancels current request pending on requester. Return Values: TInt error code 
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPsetCallBarring::CancelCurrentRequest()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::CancelCurrentRequest");
    // Set PSUI so that correct observer is used and hide requesting note.
    // When interrupting a note, does *not leave*.
    // This needs to be done first, since notes must be cleared from screen
    // even though request is not active.
    iObserver->SetEngineContact( this );
    // Does not leave
    TRAPD( err, iObserver->HandleCBRequestingL( EFalse, ETrue ) );
        
    if ( !IsActive() || err != KErrNone )
        {
        return KErrGeneral;
        }
    
    // Request has been already sent to network before this cancel method 
    // will be called. Calling DoCancel in this phase causes that e.g. "Barring 
    // activated" note is not shown to user even barring has been set active. 
    // Cancel can be done only to EPSetRequestGetStatus. It does not cause
    // any harm if status is not shown to user when quit is pressed. 
    if ( iCurrentReq == EPSetRequestGetStatus )
        {
        Cancel();
        }
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::CancelCurrentRequest");

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// 
// C++ constructor.
// 
// ---------------------------------------------------------------------------
//
CPsetCallBarring::CPsetCallBarring
                ( RMobilePhone& aPhone ) : CActive( EPriorityStandard ),
                                           iPhone ( aPhone )
    {
    }

// ---------------------------------------------------------------------------
// 
// Returns correct type of barring program.
// 
// ---------------------------------------------------------------------------
//
RMobilePhone::TMobilePhoneCBCondition CPsetCallBarring::SetBarringProgram( 
    TBarringProgram aMode )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::SetBarringProgram");
    __PHSLOGSTRING1("[PHS]    SetBarringProgram: Program: %d", aMode);
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    RMobilePhone::TMobilePhoneCBCondition barringType = 
        RMobilePhone::EBarUnspecified;
    switch ( aMode )
        {
        case EBarringTypeAllOutgoingServices:
            barringType = RMobilePhone::EBarAllOutgoingServices;
            break;
        case EBarringTypeAllIncomingServices:
            barringType = RMobilePhone::EBarAllIncomingServices;
            break;           
        case EBarringTypeAllServices:
        case EBarringTypeAllBarrings:
            barringType = RMobilePhone::EBarAllCases;
            break;
        case EBarringTypeAllOutgoing:
            barringType = RMobilePhone::EBarAllOutgoing;
            break;
        case EBarringTypeOutgoingInternational:
            barringType = RMobilePhone::EBarOutgoingInternational;
            break;
        case EBarringTypeOutgoingInternationalExceptToHomeCountry:
            barringType = RMobilePhone::EBarOutgoingInternationalExHC;
            break;
        case EBarringTypeAllIncoming:
            barringType = RMobilePhone::EBarAllIncoming;
            break;
        case EBarringTypeIncomingWhenRoaming:
            barringType = RMobilePhone::EBarIncomingRoaming;
            break;
        default:
            Panic( KPSNameOfClass, ECBUnknownBarringSetting );
            break;
        }

    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::SetBarringProgram");
    return barringType;
    }

// ---------------------------------------------------------------------------
// 
// Changes both barring and common passwords.
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallBarring::ChangePasswordL( 
    RMobilePhone::TMobilePhonePasswordChangeV2& aPwds, TBool aIsBarringPw )
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::ChangePasswordL");
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }
    
    RMobilePhone::TMobilePhonePasswordChangeV2Pckg pwdPckg ( aPwds );

    if ( aIsBarringPw )
        {
        iPhone.SetSSPassword( iStatus, pwdPckg, KPsetBarringService );
        SetRequestStatus( EPSetRequestChangeBarringPwd );
        }
    else
        {
        iPhone.SetSSPassword( iStatus, pwdPckg, KPsetAllSSServices );
        SetRequestStatus( EPSetRequestChangeCommonPwd );
        }

    StartRequestingL( KPsetRequestAlreadySet );
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::ChangePasswordL");
    }
// ---------------------------------------------------------------------------
// 
// Verify and change barring password.
// DEPRECATED - calls the correct API
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallBarring::ChangePasswordL( 
    RMobilePhone::TMobilePhonePasswordChangeV2& aPwds )
    {
    ChangePasswordL( aPwds, ETrue );
    }

// ---------------------------------------------------------------------------
// Sets request observer.
// ---------------------------------------------------------------------------
//
EXPORT_C void CPsetCallBarring::SetRequestObserver( MPsetRequestObserver* aObs )
    {
    iReqObserver = aObs;
    }

// ---------------------------------------------------------------------------
// 
// Sets observer member variable.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::SetObserver( MPsetBarringObserver& aObserver )
    {
    iObserver = &aObserver;    
    }

// ---------------------------------------------------------------------------
// 
// Starts requesting
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::StartRequestingL( const TInt& aRequest )
    {
    if ( aRequest != KPsetRequestAlreadySet )
        {
        SetRequestStatus( static_cast <TPSetBarringRequest> ( aRequest ) );
        }

    SetActive();

    iObserver->SetEngineContact( this );

    CleanupLeavePushL();    
    iObserver->HandleCBRequestingL( ETrue, EFalse );
    CleanupStack::Pop(); // CleanupCancelPushL
    }

// ---------------------------------------------------------------------------
// 
// Returns ETrue, if plural notes hould be shown.
// 
// ---------------------------------------------------------------------------
//
TBool CPsetCallBarring::GetPlurality()
    {
    if ( iChangeSetting.iSetting == ECancelBarring && 
        ( iChangeSetting.iType == EBarringTypeAllBarrings || 
          iChangeSetting.iType == EBarringTypeAllIncomingServices || 
          iChangeSetting.iType == EBarringTypeAllOutgoingServices ||
          iChangeSetting.iType == EBarringTypeAllServices ) )
        {
        return ETrue;
        }

    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// 
// Push object into cleanupstack to catch leaving.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::CleanupLeavePushL()
      {
      CleanupStack::PushL( TCleanupItem( DoHandleLeave, this ) );
      }

// ---------------------------------------------------------------------------
// 
// Object has caused a leave.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::DoHandleLeave( TAny* aAny )
      {
      REINTERPRET_CAST( CPsetCallBarring*, aAny )->HandleLeave();
      }

// ---------------------------------------------------------------------------
// 
// Things to do when leave occurs.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::HandleLeave()
    {
    CancelCurrentRequest();
    }

// ---------------------------------------------------------------------------
// Inform the request observer that request has been completed.
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::RequestCompleted( const TInt& aError )
    {
    if ( aError != KErrNone )
        {
        SetRequestStatus( static_cast <TPSetBarringRequest> (aError) );
        }    
    ClearParams();

    if ( iReqObserver )
        {
        iReqObserver->RequestComplete(); 
        }
    }

// ---------------------------------------------------------------------------
// Empties member variables.
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::ClearParams()
    {
    SetRequestStatus( EPSetRequestNone );
    iBarringParameters.iServiceGroup = RMobilePhone::EServiceUnspecified;
    iBarringParameters.iAction = RMobilePhone::EServiceActionUnspecified;
    iBarringParameters.iPassword = KNullDesC;
    }

// ---------------------------------------------------------------------------
// Return barring action.
// ---------------------------------------------------------------------------
//
RMobilePhone::TMobilePhoneServiceAction CPsetCallBarring::SetBarringAction( 
    const TBarringSetting& aSetting )
    {
    if ( aSetting == ECancelBarring )
        {
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
        return RMobilePhone::EServiceActionDeactivate;
        }
    else
        {
        return RMobilePhone::EServiceActionActivate;
        }
    }

// ---------------------------------------------------------------------------
// Handles inquiry results.
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::HandleInquiryResultL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::HandleInquiryResultL");
    TUint8 arrayOfBsc[KPSetNumberOfBsc];
    arrayOfBsc[0] = KPSetUnusedValue;

    CMobilePhoneCBList* cbStatusList = iCbStatusInterrogator->RetrieveListL();
    CleanupStack::PushL( cbStatusList );
    TBarringStatus status = EBarringStatusInactive;
    /*****************************************************
    *    Series 60 Customer / ETel
    *    Series 60  ETel API
    *****************************************************/
    RMobilePhone::TMobilePhoneCBInfoEntryV1 cbInfo;
    TInt entries = cbStatusList->Enumerate();
    __PHSLOGSTRING1("[PHS]    HandleInquiryResultL: Entry count: %d", entries);
    TInt i(0); 
    TInt cbArray(0);
    // If alternate line in use then only als line service information is shown.
    if ( iAls == ESSSettingsAlsAlternate )
        {
        while ( entries > i )
            {
            cbInfo = cbStatusList->GetEntryL( i );

            if ( cbInfo.iStatus == RMobilePhone::ECallBarringStatusActive && cbInfo.iServiceGroup == RMobilePhone::EAuxVoiceService )
                {
                 __PHSLOGSTRING1("[PHS]    ALS: iServiceGroup: %d", cbInfo.iServiceGroup);
                status = EBarringStatusActive;
                arrayOfBsc[cbArray] = static_cast <TUint8> 
                    ( PSetUtility::ChangeToGSM( cbInfo.iServiceGroup ) );
                cbArray++;
                }
            i++;                                                                
            }   
        }
    else    // All other services than alternate line service is allowed to show when primary line is active.
        {
        while ( entries > i )
            {
            cbInfo = cbStatusList->GetEntryL( i );

            if ( cbInfo.iStatus == RMobilePhone::ECallBarringStatusActive && cbInfo.iServiceGroup != RMobilePhone::EAuxVoiceService )
                {
                __PHSLOGSTRING1("[PHS]  iServiceGroup: %d", cbInfo.iServiceGroup);
                status = EBarringStatusActive;             
                arrayOfBsc[cbArray] = static_cast <TUint8> 
                    ( PSetUtility::ChangeToGSM( cbInfo.iServiceGroup ) );
                cbArray++; 
                }
            i++;                       
            }
        }
        
    arrayOfBsc[cbArray] = KPSetUnusedValue;
    CleanupStack::PopAndDestroy( cbStatusList );
    iObserver->HandleBarringModeStatusL( arrayOfBsc, status );    
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::HandleInquiryResultL");
    }

// ---------------------------------------------------------------------------
// Update the request status - both internally and to observers.
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::SetRequestStatus( TPSetBarringRequest aStatus )
    {
    iCurrentReq = aStatus;
    if ( iReqObserver )
        {
        iReqObserver->RequestStatusChanged( aStatus );
        }
    }

// ---------------------------------------------------------------------------
// 
// Cancels pending request.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::DoCancel()
    {    
    switch ( iCurrentReq )
        {
        case EPSetRequestChangeBarring:
            iPhone.CancelAsyncRequest( 
                EMobilePhoneSetCallBarringStatus );
            break;
        case EPSetRequestGetStatus:
            iCbStatusInterrogator->Cancel();
            delete iCbStatusInterrogator;
            iCbStatusInterrogator = NULL;
            break;
        case EPSetRequestChangeBarringPwd:
        case EPSetRequestChangeCommonPwd:
            iPhone.CancelAsyncRequest( EMobilePhoneSetSSPassword );
            break;
        default:
            break;
        }
    ClearParams();    
    }


// ---------------------------------------------------------------------------
// 
// Handles completed request. If error occured, calls error handling
//    function of the observer, otherwise does nothing.
// 
// ---------------------------------------------------------------------------
//
void CPsetCallBarring::RunL()
    {
    __PHSLOGSTRING("[PHS]--> CPsetCallBarring::RunL");
    __ASSERT_ALWAYS( iObserver != NULL, 
        Panic( KPSNameOfClass, ECBNoObserver ) );

    //Hide requesting note - does not leave when deleting a note.
    iObserver->SetEngineContact( this );
    iObserver->HandleCBRequestingL( EFalse, EFalse );

    //Error situations
    if ( iStatus != KErrNone )
        {
        iObserver->HandleBarringErrorL( iStatus.Int() );
        RequestCompleted( iStatus.Int() );      
        return;
        }
  
    switch ( iCurrentReq )
        {
        case EPSetRequestChangeBarring:
            {
            __PHSLOGSTRING("[PHS]     EPSetRequestChangeBarring");
            TBarringStatus status = ( TBarringStatus ) iChangeSetting.iSetting;
            TBool plural = GetPlurality();
            __PHSLOGSTRING1("[PHS]    Barring status: %d", status );
            __PHSLOGSTRING1("[PHS]    Plurality: %d", plural );
            iObserver->HandleBarringModeChangedL( iChangeSetting.iType, 
                                                  status, plural );
            break;
            }            
        case EPSetRequestGetStatus:
            {
            __PHSLOGSTRING("[PHS]     EPSetRequestGetStatus");
            HandleInquiryResultL();
            break;
            }
        case EPSetRequestChangeBarringPwd:
        case EPSetRequestChangeCommonPwd:
            __PHSLOGSTRING("[PHS]     EPSetRequestChange");
            iObserver->CbPasswordChangedL( ETrue );
            break;
        default:
            Panic ( KPSNameOfClass, ECBUnknownRequest );
            break;
        }
    RequestCompleted( KErrNone );
    __PHSLOGSTRING("[PHS] <--CPsetCallBarring::RunL");
    }

// End of File
