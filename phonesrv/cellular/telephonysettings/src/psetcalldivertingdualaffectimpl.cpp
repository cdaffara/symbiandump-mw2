/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPSetCallDivertingDualAffectImpl class.
*
*/


// INCLUDE FILES
#include "psetcalldivertingdualaffectimpl.h" 
#include "psetsubscriberidcheck.h" 

#include <badesca.h>
#include <etelmm.h>
#include <e32math.h>
#include <e32svr.h>
#include <featmgr.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h> 

#include "psetcalldiverting.h" 
#include "psetcontainer.h" 
#include "mpsetdivertobs.h" 
#include "psettelephony.h" 
#include "psetpanic.h" 
#include "mpsetrequestobs.h" 
#include "psetutility.h" 
#include "phonesettingslogger.h" 
#include "psetsaobserver.h" 

//  LOCAL CONSTANTS AND MACROS
_LIT( KPSetIntNbr, "+" );
_LIT( KPSNameOfClass, "CPsetCallDiverting" );
_LIT( KPhone, "Telephone" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPSetCallDivertingDualAffectImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPSetCallDivertingDualAffectImpl* CPSetCallDivertingDualAffectImpl::NewL( 
        MPsetDivertObserver& aObserver, 
        RMobilePhone& aPhone,
        CPsetCallDiverting* aDivert )
    {
    CPSetCallDivertingDualAffectImpl* self = new ( ELeave ) CPSetCallDivertingDualAffectImpl( aPhone, aDivert );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPSetCallDivertingDualAffectImpl::~CPSetCallDivertingDualAffectImpl
// ---------------------------------------------------------------------------
//
CPSetCallDivertingDualAffectImpl::~CPSetCallDivertingDualAffectImpl()
    {
    delete iIdCheck;
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingDualAffectImpl::CPSetCallDivertingDualAffectImpl
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPSetCallDivertingDualAffectImpl::CPSetCallDivertingDualAffectImpl( 
                    RMobilePhone& aPhone, CPsetCallDiverting* aDivert ) : 
                    CPSetCallDivertingBase( aPhone, aDivert )
    {
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingDualAffectImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSetCallDivertingDualAffectImpl::ConstructL( MPsetDivertObserver& aObserver )
    {
    CPSetCallDivertingBase::ConstructL( aObserver );
    
    iIdCheck = CPSetSubscriberIdCheck::NewL();
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingDualAffectImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSetCallDivertingDualAffectImpl::SetDivertingL( const TCallDivertSetting& aDivert,  TBasicServiceGroups aBsc )
    { __PHSLOGSTRING("[PHS]--> CPSetCallDivertingDualAffectImpl::SetDivertingL");
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingDualAffectImpl::SetDivertingL: aBsc: %d", aBsc );
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        } 
    CPsetTelephony::CheckLineModeL( aDivert.iServiceGroup, &iPhone, iLine );
    if ( !iLine->SubSessionHandle() )
        {
        User::Leave( KErrBadHandle );
        }
        
    SetRequestStatus( EPSetChangeDivert );
    iReason = PSetUtility::GetDivertReason ( aDivert.iCondition );
    iVoiceDivert = SetVoiceDivert( aDivert, aBsc );

    /* In case of SIM support dual activation and * -command has been issued from ALS 
    * line 2 we need to change the SS service 
    * code from SS_GSM_AUX_TELEPHONY to the SS_ALL_TELE_AND_BEARER 
    * at SS_SERVICE_REQ -message.
    */
    if ( ( aBsc == EAllTeleAndBearer ) &&                // Command affects to both lines
       ( iAls == ESSSettingsAlsAlternate ) &&            // Command given at ALS line 2
       ( (RThread().Name()).Compare( KPhone ) == 0 ) &&  // *# -commands are given only from Phone thread
       ( aDivert.iSetting != ECheckStatus ) &&           // Not interrogating
       ( iIdCheck->DoesSIMSupportDualActivation() ) )    // Check SIM/IMSI
        {
        // Set iBsc to same value as aBsc so that correct alsValue 
        // can be determined in GetSAAls() method.
        iBsc = aBsc;
        }
    else
        {
        // Set iBsc to same value as aBsc so that correct alsValue 
        // can be determined in GetSAAls() method.
        iBsc = aBsc;
        ValidateBsc( aBsc );
        }

    iPluralNotes = IsMultiAffectingDivert( aDivert, aBsc );

    //Copy data to member variable
    iChangeInfo.iNumber.iTelNumber.Copy( aDivert.iNumber );
    iChangeInfo.iTimeout = KPSetDefaultDelayTimeValue;
    if ( aBsc == EAltTele )
        {
        iChangeInfo.iServiceGroup = PSetUtility::VerifyAltLineUseL();
        }
    iChangeInfo.iServiceGroup = PSetUtility::ChangeToEtelInternal( aBsc ); 
    iChangeInfo.iNumber.iNumberPlan = RMobilePhone::EIsdnNumberPlan;
    iChangeInfo.iNumber.iTypeOfNumber = RMobilePhone::EUnknownNumber;
    if ( iChangeInfo.iNumber.iTelNumber.Length() > 0 )
        {
        if ( iChangeInfo.iNumber.iTelNumber.Left(1) == KPSetIntNbr )
            {
            iChangeInfo.iNumber.iTypeOfNumber = 
                RMobilePhone::EInternationalNumber;
            }
        }

    switch ( aDivert.iSetting )
        {
        case ERegisterDiverting:
            iChangeInfo.iAction = RMobilePhone::EServiceActionRegister;
            iChangeInfo.iTimeout = aDivert.iNoReplyTimer;
            __PHSLOGSTRING("[PHS]    CPSetCallDivertingDualAffectImpl::SetDivertingL: ERegisterDiverting" );
            break;
        case EEraseDiverting:
            iChangeInfo.iAction = RMobilePhone::EServiceActionErase;
            __PHSLOGSTRING("[PHS]    CPSetCallDivertingDualAffectImpl::SetDivertingL: EEraseDiverting" );
            break;
        case EActivateDiverting:
            iChangeInfo.iAction = RMobilePhone::EServiceActionActivate;
            iChangeInfo.iTimeout = aDivert.iNoReplyTimer;
            __PHSLOGSTRING("[PHS]    CPSetCallDivertingDualAffectImpl::SetDivertingL: EActivateDiverting" );
            break;
        case ECancelDiverting:
            iChangeInfo.iAction = RMobilePhone::EServiceActionDeactivate;
            __PHSLOGSTRING("[PHS]    CPSetCallDivertingDualAffectImpl::SetDivertingL: ECancelDiverting" );
            break;
        default:
            User::Leave( KErrArgument );
            break;
        }

    //Start requesting for setting Divert.
    iPhone.SetCallForwardingStatus( iStatus, iReason, iChangeInfo );
    StartRequestingL( KPsetRequestAlreadySet );    
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingDualAffectImpl::SetDivertingL" );
   }

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingDualAffectImpl::RunL()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingDualAffectImpl::RunL" );
    
    __ASSERT_ALWAYS( 
        iObserver != NULL, Panic( KPSNameOfClass, ECDRequestPanicNoObserver ) );

    //Hide requesting note - does not leave when deleting a note.
    iObserver->SetEngineContact( iDivert );
    iObserver->HandleCFRequestingL( EFalse, EFalse );

    //Handle error case.
    if ( iStatus != KErrNone )
        {
        iObserver->HandleDivertingErrorL( iStatus.Int() );
        RequestCompleted( iStatus.Int() );
        return;
        }
        
    iDivertStatus.Initialize();
    switch ( iCurrentReq )
        {
        case EPSetChangeDivert:
            {
            __PHSLOGSTRING("[PHS]--> CPSetCallDivertingDualAffectImpl::RunL: EPSetChangeDivert" );
            //Notify Observer            
            iDivertStatus.iStatus = PSetUtility::GetChangeInfoStatus( iChangeInfo.iAction );
            
             // Check is done because of VOIP notification functionality(PSetNotesUI).
            if ( iVoiceDivert )
                {
                iDivertStatus.iServiceGroup = EServiceGroupVoice;
                }
            
            iObserver->HandleDivertingChangedL( iDivertStatus, iPluralNotes );
            
            //Notify observer.
            HandleSANotificationL( 
                IsVMBXDivertL( iChangeInfo.iNumber.iTelNumber ), 
                iDivertStatus.iStatus );
            break;
            }
        case EPSetGetDivertStatus:
            {
            __PHSLOGSTRING("[PHS]--> CPSetCallDivertingDualAffectImpl::RunL: EPSetGetDivertStatus" );

            CMobilePhoneCFList* cfList = iCfInterrogator->RetrieveListL();
            CleanupStack::PushL( cfList );
            CMobilePhoneCFList* cleanedList = CMobilePhoneCFList::NewL();            
            CleanupStack::PushL( cleanedList );
            
            TInt numOfItems = cfList->Enumerate();
            TInt index = 0;
            RMobilePhone::TMobilePhoneCFInfoEntryV1 cfEntry;
            RMobilePhone::TMobilePhoneCFInfoEntryV1 copy;
            iDivertStatus.iStatus = EDivertingStatusNotRegistered;
            while( index < numOfItems )
                {
                cfEntry = cfList->GetEntryL( index );
                iBsc = PSetUtility::ChangeToGSM( cfEntry.iServiceGroup );

                if ( cfEntry.iStatus == RMobilePhone::ECallForwardingStatusActive )
                    {
                    copy.iServiceGroup = cfEntry.iServiceGroup;
                    copy.iCondition = cfEntry.iCondition;
                    copy.iStatus = cfEntry.iStatus;
                    copy.iNumber = cfEntry.iNumber;
                    copy.iTimeout = cfEntry.iTimeout;
                    cleanedList->AddEntryL( copy );
                    }
                 __PHSLOGSTRING1("[PHS] CPSetCallDivertingDualAffectImpl::RunL: EPSetGetDivertStatus     iBsc: %d", iBsc );
                 __PHSLOGSTRING1("[PHS] CPSetCallDivertingDualAffectImpl::RunL: EPSetGetDivertStatus iStatus: %d", cfEntry.iStatus );
                 
                /* Basic Service Codes above were removed from "if" statement below because they would 
                * cause divert indicator to appear with Data services. Dual activation SIM doesnt want to show 
                * Divert indication for video service.*/                
                if ( iBsc == EAllTeleAndBearer || iBsc == EAllTele || iBsc == ETelephony || iBsc == EAltTele )
                    {
                    // iVoiceDivert is set to true to ensure that the icons are updated in every case.
                    iVoiceDivert = ETrue;
                    if ( ( cfEntry.iStatus != RMobilePhone::ECallForwardingStatusNotRegistered )
                        && ( cfEntry.iStatus != RMobilePhone::ECallForwardingStatusNotActive )
                        && ( cfEntry.iStatus != RMobilePhone::ECallForwardingStatusNotProvisioned ) )
                        {
                        iDivertStatus.iStatus = EDivertingStatusActive;
                        }
                    else
                        {
                        iDivertStatus.iStatus = EDivertingStatusNotRegistered;
                        }
                    
                    HandleSANotificationL( 
                            IsVMBXDivertL( cfEntry.iNumber.iTelNumber ), 
                            iDivertStatus.iStatus );
                    }
                index++;
                }
            if ( cleanedList->Enumerate() == 0 )
                {
                iObserver->HandleDivertingStatusL( *cfList, iPluralNotes );
                }
            else
                {
                iObserver->HandleDivertingStatusL( *cleanedList, iPluralNotes );
                }
            CleanupStack::PopAndDestroy( 2 ); //cleanedList, cfList
            cfList = NULL;
            cleanedList = NULL;
            iCFStatusCheck = EFalse; 
            break;
            }
        default:
            Panic( KPSNameOfClass, ECDRequestPanicIncorrectRequest );
            break;
        } 
    RequestCompleted( KErrNone );
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingDualAffectImpl::RunL" );
    }    

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingDualAffectImpl::DoCancel()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingDualAffectImpl::DoCancel" );
    //Decide which request to cancel.
    switch( iCurrentReq )
        {
        case EPSetChangeDivert:
            __PHSLOGSTRING("[PHS]   DoCancel - EPSetChangeDivert" );
            iPhone.CancelAsyncRequest( EMobilePhoneSetCallForwardingStatus );
            break;
        case EPSetGetDivertStatus:
            __PHSLOGSTRING("[PHS]   DoCancel - EPSetGetDivertStatus" );
            iCfInterrogator->Cancel();
            delete iCfInterrogator;
            iCfInterrogator = NULL;
            break;
        default:
            break;
        }
    
    //Set current request to none.
    iCurrentReq = EPSetNone;
    iVoiceDivert = EFalse;
    __PHSLOGSTRING("[PHS]<-- CPSetCallDivertingDualAffectImpl::DoCancel" );
    }

// -----------------------------------------------------------------------------
// Called if RunL Leaves
// -----------------------------------------------------------------------------
//
TInt CPSetCallDivertingDualAffectImpl::RunError( TInt aError )
    {
    __PHSLOGSTRING1("[PHS]--> CPSetCallDivertingDualAffectImpl::RunError aError: %d", aError );
    if ( ( iCurrentReq == EPSetGetDivertStatus || iCurrentReq == EPSetChangeDivert)
         && iVoiceDivert )
        {
        __PHSLOGSTRING("[PHS]   RunError - HandleSANotificationL" );
        TRAP_IGNORE( HandleSANotificationL( EFalse, iDivertStatus.iStatus ) );
        }
        
    if ( iCurrentReq == EPSetGetDivertStatus )
        {
        iCFStatusCheck = EFalse;
        }   
    aError = KErrNone;
        
    //Set current request to none.
    iCurrentReq = EPSetNone;    
    iVoiceDivert = EFalse;
    __PHSLOGSTRING("[PHS]<-- CPSetCallDivertingDualAffectImpl::RunError" );
    return aError;  
    }

// ---------------------------------------------------------------------------
//  If unconditional voice divert is activated/cancelled, notifies observer, 
//  so that it can set on/off indicators.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingDualAffectImpl::HandleSANotificationL( 
    TBool aVmbxDivert, TDivertingStatus& aCfStatus )
    {
    __PHSLOGSTRING2("[PHS]--> CPSetCallDivertingDualAffectImpl::HandleSANotificationL aVmbxDivert: %d, aCfStatus: %d", aVmbxDivert, aCfStatus );
    TBool divertChanged = EFalse;
    //If divert is not with voice, do not touch indicators.
    if ( !iVoiceDivert )
        {
        return;
        }

    //Unconditional divert and all diverts
    if ( iReason == RMobilePhone::ECallForwardingUnconditional ||
        iReason == RMobilePhone::ECallForwardingAllCases )
        {
        if ( iCurrentReq == EPSetChangeDivert )
            {
            //show indicator if, activation/registration, else hide it
            //these are probably incorrect....
            divertChanged = 
                ( iChangeInfo.iAction == RMobilePhone::EServiceActionRegister ||
                  iChangeInfo.iAction == RMobilePhone::EServiceActionActivate);
            }
        else
            {
            //show indicator if status is activated, hide for cancelled.
            divertChanged = ( aCfStatus == EDivertingStatusActive );
            }
        }
    else
        {
        //if not unconditional, do not touch indicators.
        return;
        }
        
    CPsetSAObserver* systemObserver = CPsetSAObserver::NewL();
    CleanupStack::PushL( systemObserver );
    
    TUnconditionalCFStatus status(KCFIndicatorUnknown);
    
    User::LeaveIfError( systemObserver->GetCurrentDivertStatus( status ) );
    
    // If used SIM support dual activation and ALS is supported then do the check for
    // current divert information. 
    if ( ( iIdCheck->DoesSIMSupportDualActivation()  ) && 
         ( iAls != ( ENotSupportedLine ) ) ) 
        {
        SetCurrentDivertInformation( status, divertChanged );
        }
    
    TCallDivertNotifySetting setting = CreateDivertNotifySetting(
        status, divertChanged, aVmbxDivert, iBsc );
    
    // Notify observer that Diverts have changed.
    systemObserver->NotifyDivertChange( GetSAAls(), 
        setting,
        -1 );
        
    CleanupStack::PopAndDestroy( systemObserver );
    
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingDualAffectImpl::HandleSANotificationL" );
    }

// ---------------------------------------------------------------------------
// Sets current divert information if used used SIM supports dual activation.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingDualAffectImpl::SetCurrentDivertInformation( 
    TUnconditionalCFStatus& aStatus, 
    const TBool aDivertChanged )
    {
    TUnconditionalCFStatus previousDivertStatus = aStatus;
    TSelectedLine alsLine = GetSAAls();
    
    if (( iChangeInfo.iServiceGroup == RMobilePhone::EAllServices) &&    // - Service Code is All tele and bearer.
        ( iCurrentReq == EPSetChangeDivert) && (                         // - Change divert command issued.
        ( iReason == RMobilePhone::ECallForwardingUnconditional ) ||     // - CallForwardingCondition is: CFU or 
        ( iReason == RMobilePhone::ECallForwardingAllCases ) )  )        // CFA.                                           
        {
        switch ( alsLine )
            {
            case EPrimaryLine:
                {
                if( aDivertChanged )     // ON
                    {
                    if(( previousDivertStatus == KCFNoCallsForwarded ) ||      // None/Line1 -> Line2 -> Both
                       ( previousDivertStatus == ( KCFVoiceForwarded | KCFForwardedOnLine1 ) ))
                        {
                        aStatus = KCFForwardedOnLine2;
                        }
                    }
                else                    // OFF
                    {
                    if(( previousDivertStatus == ( KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 ) ) ||  // Both/Line2 -> Line1 -> None
                       ( previousDivertStatus == KCFForwardedOnLine2 ))
                        {
                        aStatus = static_cast<TUnconditionalCFStatus>( KCFVoiceForwarded | KCFForwardedOnLine1 );
                        }
                    }
                break;
                }

            case EAuxiliaryLine:
                {
                if( aDivertChanged )     // ON
                    {
                    if(( previousDivertStatus == KCFNoCallsForwarded ) ||      // None/Line2 -> Line 1 -> Both
                       ( previousDivertStatus == KCFForwardedOnLine2 ))
                        {
                        aStatus = static_cast<TUnconditionalCFStatus>( KCFVoiceForwarded | KCFForwardedOnLine1 );
                        }
                    }
                else                    // OFF
                    {
                    if(( previousDivertStatus == ( KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 ) ) ||  // Both/Line1 -> Line 2 -> None
                       ( previousDivertStatus == ( KCFVoiceForwarded | KCFForwardedOnLine1 ) ))
                        {
                        aStatus = KCFForwardedOnLine2;
                        }
                    }
                break;
                }

            default:
                break;
            }
        }    
    }

// ---------------------------------------------------------------------------
// Returns ETrue if divert operation is to voice or to all operations.
// ---------------------------------------------------------------------------
//
TBool CPSetCallDivertingDualAffectImpl::SetVoiceDivert( 
        const TCallDivertSetting& aDivert,
        const TBasicServiceGroups /*aBsc*/ )
    {
    TBool retValue(EFalse);
    if ( aDivert.iServiceGroup == EServiceGroupVoice ||
         aDivert.iServiceGroup == EServiceGroupAllTeleservices)
        {
        retValue = ETrue;
        }
    return retValue;
    }

// ---------------------------------------------------------------------------
// Gets Als information.
// ---------------------------------------------------------------------------
//
TSelectedLine CPSetCallDivertingDualAffectImpl::GetSAAls()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingDualAffectImpl::GetSAAls" );
    __PHSLOGSTRING2("[PHS] iAls: %d iBsc: %d", iAls, iBsc  );
    TSelectedLine alsValue = ENotSupportedLine;
    switch ( iAls )
        {
        case ESSSettingsAlsNotSupported:
            alsValue = ENotSupportedLine;
            break;
        case ESSSettingsAlsPrimary:
            iChangeInfo.iServiceGroup == RMobilePhone::EAuxVoiceService 
                ? ( alsValue = EAuxiliaryLine )  
                : ( alsValue = EPrimaryLine );
            break;
        case ESSSettingsAlsAlternate:
            iChangeInfo.iServiceGroup == RMobilePhone::EAuxVoiceService 
                ? ( alsValue = EAuxiliaryLine )  
                : ( alsValue = EPrimaryLine );
            break;
        default:
            alsValue = ENotSupportedLine;
            break;
        }
    
    // If user checks/activates divert to line which is not selected currently than we have
    // to change returned alsvalue accordinly, if this is not done divert icon status is updated to wrong status.
    // Example case: Line 2 active user activates divert to line 1: **21*phonenumber*11#.
    if ( iBsc == EAltTele && iAls == ESSSettingsAlsPrimary )
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingDualAffectImpl::GetSAAls - Line 2 divert activation/check from Line 1" );
        alsValue = EAuxiliaryLine;
        }
    else if ( iBsc == ETelephony && iAls == ESSSettingsAlsAlternate )
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingDualAffectImpl::GetSAAls - Line 1 divert activation/check from Line 2" );
        alsValue = EPrimaryLine;
        }
    
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingDualAffectImpl::GetSAAls: alsValue: %d", alsValue );
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingDualAffectImpl::GetSAAls" );
    return alsValue;
    }

// End of File
