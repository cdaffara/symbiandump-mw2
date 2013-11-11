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
* Description: Implementation of CPSetCallDivertingBasicImpl class.
*
*/


// INCLUDE FILES
#include "psetcalldivertingbasicimpl.h" 

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
_LIT( KPSNameOfClass, "CPSetCallDivertingBasicImpl" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPSetCallDivertingBasicImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPSetCallDivertingBasicImpl* CPSetCallDivertingBasicImpl::NewL( 
        MPsetDivertObserver& aObserver, 
        RMobilePhone& aPhone,
        CPsetCallDiverting* aDivert )
    {
    CPSetCallDivertingBasicImpl* self = new ( ELeave ) CPSetCallDivertingBasicImpl( aPhone, aDivert );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPSetCallDivertingBasicImpl::~CPSetCallDivertingBasicImpl
// ---------------------------------------------------------------------------
//
CPSetCallDivertingBasicImpl::~CPSetCallDivertingBasicImpl()
    {
    Cancel();
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingBasicImpl::CPSetCallDivertingBasicImpl
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPSetCallDivertingBasicImpl::CPSetCallDivertingBasicImpl( 
                    RMobilePhone& aPhone, CPsetCallDiverting* aDivert ) : 
                    CPSetCallDivertingBase( aPhone, aDivert )
    {
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingBasicImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSetCallDivertingBasicImpl::ConstructL( MPsetDivertObserver& aObserver )
    {
    CPSetCallDivertingBase::ConstructL( aObserver );
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingBasicImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSetCallDivertingBasicImpl::SetDivertingL( const TCallDivertSetting& aDivert,  TBasicServiceGroups aBsc )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBasicImpl::SetDivertingL");
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBasicImpl::SetDivertingL: aBsc: %d", aBsc );
   if ( IsActive() )
       {
       User::Leave( KErrInUse );
       }
   iBsc = aBsc;
   iRequestedServiceGroup = aDivert.iServiceGroup;
   
   CPsetTelephony::CheckLineModeL( aDivert.iServiceGroup, &iPhone, iLine );
   if ( !iLine->SubSessionHandle() )
       {
       User::Leave( KErrBadHandle );
       }
       
   SetRequestStatus( EPSetChangeDivert );
   iReason = PSetUtility::GetDivertReason ( aDivert.iCondition );
   iVoiceDivert = SetVoiceDivert( aDivert, aBsc );

   ValidateBsc( aBsc );
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
          /*****************************************************
          *    Series 60 Customer / ETel
          *    Series 60  ETel API
          *****************************************************/
          iChangeInfo.iAction = RMobilePhone::EServiceActionRegister;
          iChangeInfo.iTimeout = aDivert.iNoReplyTimer;
          __PHSLOGSTRING("[PHS]    CPSetCallDivertingBasicImpl::SetDivertingL: ERegisterDiverting" );
          break;
       case EEraseDiverting:
           iChangeInfo.iAction = RMobilePhone::EServiceActionErase;
           __PHSLOGSTRING("[PHS]    CPSetCallDivertingBasicImpl::SetDivertingL: EEraseDiverting" );
           break;
       case EActivateDiverting:
           iChangeInfo.iAction = RMobilePhone::EServiceActionActivate;
           iChangeInfo.iTimeout = aDivert.iNoReplyTimer;
           __PHSLOGSTRING("[PHS]    CPSetCallDivertingBasicImpl::SetDivertingL: EActivateDiverting" );
           break;
       case ECancelDiverting:
           iChangeInfo.iAction = RMobilePhone::EServiceActionDeactivate;
           __PHSLOGSTRING("[PHS]    CPSetCallDivertingBasicImpl::SetDivertingL: ECancelDiverting" );
           break;
       default:
           User::Leave( KErrArgument );
           break;
       }

   //Start requesting for setting Divert.
   iPhone.SetCallForwardingStatus( iStatus, iReason, iChangeInfo );
   StartRequestingL( KPsetRequestAlreadySet );    
   __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBasicImpl::SetDivertingL" );
   }

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBasicImpl::RunL()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBasicImpl::RunL" );           
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
           __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBasicImpl::RunL: EPSetChangeDivert" );
           //Notify Observer            
           iDivertStatus.iStatus = PSetUtility::GetChangeInfoStatus( iChangeInfo.iAction );
           iDivertStatus.iNumber = iChangeInfo.iNumber.iTelNumber;
           iDivertStatus.iCondition = PSetUtility::GetDivertReason(iReason);
           iDivertStatus.iNoReplyTimer = iChangeInfo.iTimeout;
           iDivertStatus.iServiceGroup = iRequestedServiceGroup;
           
            // Check is done because of VOIP notification functionality(PSetNotesUI).
           if ( iVoiceDivert && !iVideoDivert )
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
           __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBasicImpl::RunL: EPSetGetDivertStatus" );

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
                   
                __PHSLOGSTRING1("[PHS] CPSetCallDivertingBasicImpl::RunL: EPSetGetDivertStatus     iBsc: %d", iBsc );
                __PHSLOGSTRING1("[PHS] CPSetCallDivertingBasicImpl::RunL: EPSetGetDivertStatus iStatus: %d", cfEntry.iStatus );
               if ( iBsc == EAllTeleAndBearer || iBsc == EAllTele || iBsc == ETelephony || iBsc == EAltTele ||
                   iBsc == EAllBearer || iBsc == EAllSync || iBsc == ESyncData )
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
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBasicImpl::RunL" );
    }    

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBasicImpl::DoCancel()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBasicImpl::DoCancel" );
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
    iVideoDivert = EFalse;
    __PHSLOGSTRING("[PHS]<-- CPSetCallDivertingBasicImpl::DoCancel" );
    }

// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
TInt CPSetCallDivertingBasicImpl::RunError( TInt aError ) 
    {
    __PHSLOGSTRING1("[PHS]--> CPSetCallDivertingBasicImpl::RunError aError: %d", aError );
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
    iVideoDivert = EFalse;
    __PHSLOGSTRING("[PHS]<-- CPSetCallDivertingBasicImpl::RunError" );
    return aError;  
    }

// ---------------------------------------------------------------------------
//  If unconditional voice divert is activated/cancelled, notifies observer, 
//  so that it can set on/off indicators.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBasicImpl::HandleSANotificationL( 
    TBool aVmbxDivert, TDivertingStatus& aCfStatus )
    {
    __PHSLOGSTRING2("[PHS]--> CPSetCallDivertingBasicImpl::HandleSANotificationL aVmbxDivert: %d, aCfStatus: %d", aVmbxDivert, aCfStatus );
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
    
    TCallDivertNotifySetting setting = CreateDivertNotifySetting(
        status, divertChanged, aVmbxDivert, iBsc );
    
    // Notify observer that Diverts have changed.
    systemObserver->NotifyDivertChange( GetSAAls(), 
        setting,
        -1 );
    
    CleanupStack::PopAndDestroy( systemObserver );

    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBasicImpl::HandleSANotificationL" );
    }

// ---------------------------------------------------------------------------
// Returns ETrue if divert operation is to voice or to all operations.
// ---------------------------------------------------------------------------
//
TBool CPSetCallDivertingBasicImpl::SetVoiceDivert( const TCallDivertSetting& aDivert,
    const TBasicServiceGroups aBsc )
    {
    iVideoDivert = EFalse;
    if ( aDivert.iServiceGroup == EServiceGroupVoice ||
         aDivert.iServiceGroup == EServiceGroupAllTeleservices)
        {
        return ETrue;
        }
    else if ( ( aDivert.iServiceGroup == EServiceGroupData && ( aBsc == EAllBearer ||
             aBsc == EAllSync || aBsc == ESyncData ) ) )
        {
        iVideoDivert = ETrue;
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// Gets Als information.
// ---------------------------------------------------------------------------
//
TSelectedLine CPSetCallDivertingBasicImpl::GetSAAls()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBasicImpl::GetSAAls" );
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
            alsValue = EAuxiliaryLine;
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
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBasicImpl::GetSAAls - Line 2 divert activation/check from Line 1" );
        alsValue = EAuxiliaryLine;
        }
    else if ( iBsc == ETelephony && iAls == ESSSettingsAlsAlternate )
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBasicImpl::GetSAAls - Line 1 divert activation/check from Line 2" );
        alsValue = EPrimaryLine;
        }
    
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBasicImpl::GetSAAls: alsValue: %d", alsValue );
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBasicImpl::GetSAAls" );
    return alsValue;
    }

// End of File
