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
* Description: Implementation of CPSetCallDivertingBase class.
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
#include "psetcalldivertingbase.h" 

//  LOCAL CONSTANTS AND MACROS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPSetCallDivertingBase::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPSetCallDivertingBase* CPSetCallDivertingBase::NewL( 
        MPsetDivertObserver& aObserver, 
        RMobilePhone& aPhone,
        CPsetCallDiverting* aDivert )
    {
    CPSetCallDivertingBase* self = new ( ELeave ) CPSetCallDivertingBase( aPhone, aDivert );
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CPSetCallDivertingBase::~CPSetCallDivertingBase
// ---------------------------------------------------------------------------
//
CPSetCallDivertingBase::~CPSetCallDivertingBase()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::~CPSetCallDivertingBase");
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
    delete iCfInterrogator;

    iReqObserver = NULL;

    delete iRepository;
    
    if ( iFeatureManagerInitialized )
        {
        FeatureManager::UnInitializeLib();
        }

    __PHSLOGSTRING("[PHS]<-- CPSetCallDivertingBase::~CPSetCallDivertingBase");
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingBase::CPSetCallDivertingBase
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPSetCallDivertingBase::CPSetCallDivertingBase( RMobilePhone& aPhone,  CPsetCallDiverting* aDivert  ) : 
                    CActive ( EPriorityStandard ),
                    iPhone ( aPhone ),
                    iDivert( aDivert )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::CPSetCallDivertingBase");
    iFeatureManagerInitialized = EFalse;
    __PHSLOGSTRING("[PHS]<-- CPSetCallDivertingBase::CPSetCallDivertingBase");
    }

// -----------------------------------------------------------------------------
// CPSetCallDivertingBase::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPSetCallDivertingBase::ConstructL( MPsetDivertObserver& aObserver )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::ConstructL" );
    iLine = new ( ELeave ) RMobileLine;

    //Copy received information to member variable.
    iObserver = &aObserver;
    
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
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::ConstructL: usedLine: %d", usedLine );

    iRepository = CRepository::NewL( KCRUidCallForwarding );

    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();
    iFeatureManagerInitialized = ETrue;
    
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::ConstructL" );
    }

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::RunL()
    {
    // Empty implementation.
    }
       
// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::DoCancel()
    {
    // Empty implementation.
    }

// ---------------------------------------------------------------------------
// SetRequestObserver
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::SetRequestObserver( MPsetRequestObserver* aObs )
    {
    iReqObserver = aObs;
    }

// ---------------------------------------------------------------------------
// SetDivertingL
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::SetDivertingL( const TCallDivertSetting& /*aDivert*/,  
                            TBasicServiceGroups /*aBsc*/ )
    {
    //Empty implementation.
    }

// ---------------------------------------------------------------------------
// CancelCurrentRequest
// ---------------------------------------------------------------------------
//
TInt CPSetCallDivertingBase::CancelCurrentRequest()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::CancelCurrentRequest" );
    // Set PSUI so that correct observer is used and hide requesting note.
    // When interrupting a note, does not leave.
    // This needs to be done first, since notes must be cleared from screen
    // even though request is not active.
    iObserver->SetEngineContact( iDivert );
    // Does not leave
    TRAPD( err, iObserver->HandleCFRequestingL( EFalse, ETrue ) );

    if ( !IsActive() || err != KErrNone )
        {
        __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::CancelCurrentRequest: KErrGeneral" );
        return KErrGeneral;
        }
    
    if ( iCFStatusCheck )
        {
        iCFStatusCheck = EFalse;
        Cancel();
        }

    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::CancelCurrentRequest: KErrNone" );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Sets new default number the the default numbers list, erases the oldest one.
// ---------------------------------------------------------------------------
//
TInt CPSetCallDivertingBase::SetNewDefaultNumberL( TDes& aNumber )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::SetNewDefaultNumberL" );    
    TTelNumber oldFirst;
    TTelNumber oldSecond; 

    CDesC16ArrayFlat* defNumberArray = 
        new ( ELeave ) CDesC16ArrayFlat( KPSetDefaultNumberMax );
    CleanupStack::PushL( defNumberArray );

    TInt defNumbers = GetNumbersFromSharedDataL( iAls, *defNumberArray );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::SetNewDefaultNumberL: defNumbers: %d", defNumbers);

    if ( defNumbers > 0 ) //at least one number was fetched
        {
        //save latest divert no.
       oldFirst = defNumberArray->MdcaPoint( 0 );
        }
    if ( defNumbers > 1 ) //at least two numbers were fetched
        {
        //save second latest divert no.
        oldSecond = defNumberArray->MdcaPoint( 1 );
        }
    CleanupStack::PopAndDestroy( defNumberArray );

    if ( iAls != ESSSettingsAlsAlternate )
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBase::SetNewDefaultNumberL: !ESSSettingsAlsAlternate" );
        switch ( defNumbers ) // cases flow through
            {
            case 3:
            case 2://two default numbers
                User::LeaveIfError( 
                    SaveKey( KSettingsCFDefaultNumber3, oldSecond ) );
            case 1://one default number
                User::LeaveIfError( 
                    SaveKey( KSettingsCFDefaultNumber2, oldFirst ) );
            case 0://no default numbers
                User::LeaveIfError( 
                    SaveKey( KSettingsCFDefaultNumber1, aNumber ) );
                break;
            }
        }
    else//aux line
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBase::SetNewDefaultNumberL: Aux line" );
        switch ( defNumbers ) // cases flow through
            {
            case 3:
            case 2://two default numbers
                User::LeaveIfError( 
                    SaveKey( KSettingsCFDefaultNumberAls3, oldSecond ) );
            case 1://one default number
                User::LeaveIfError( 
                    SaveKey( KSettingsCFDefaultNumberAls2, oldFirst ) );
            case 0://no default numbers
                User::LeaveIfError( 
                    SaveKey( KSettingsCFDefaultNumberAls1, aNumber ) );
                break;
            default:
                User::Leave( KErrArgument );
                break;
            }
        } 

    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::SetNewDefaultNumberL" );
    return KErrNone; //return value is not used
    }

// --------------------------------------------------------------------------- 
// Saves a single value to central repository. Make sure that repository
// is open before calling this function.
// ---------------------------------------------------------------------------
//
TInt CPSetCallDivertingBase::SaveKey(
    TUint32 aKeyId,
    const TDesC& aKeyValue )
    {
    TInt error = KErrUnknown;

    error = iRepository->Set( aKeyId, aKeyValue );

    return error;
    }

// ---------------------------------------------------------------------------
// Reads used divert numbers (0-3) from shared data and fills array with them.
// ---------------------------------------------------------------------------
//
TInt CPSetCallDivertingBase::GetNumbersFromSharedDataL( TInt aType, 
                CDesC16ArrayFlat& aArray )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::GetNumbersFromSharedDataL" );

    TInt amount = 0;
    TTelNumber readValue;
    aType = iAls;
    
    RArray<TUint32> keyArray( KPSetDefaultNumberMax ); 
    CleanupClosePushL( keyArray );
    //to simplify reading, append needed values to an array    
    if ( aType != ESSSettingsAlsAlternate )
        {
        keyArray.AppendL( KSettingsCFDefaultNumber1 );
        keyArray.AppendL( KSettingsCFDefaultNumber2 );
        keyArray.AppendL( KSettingsCFDefaultNumber3 );
        }
    else //auxiliary line
        {
        keyArray.AppendL( KSettingsCFDefaultNumberAls1 );
        keyArray.AppendL( KSettingsCFDefaultNumberAls2 );
        keyArray.AppendL( KSettingsCFDefaultNumberAls3 );
        }    

    TInt i = KErrNone;
    TInt loc = KErrNone;
    while ( i == KErrNone && loc < KPSetDefaultNumberMax )
        {
        i = iRepository->Get( keyArray.operator[]( loc ), readValue );
        if ( i == KErrNone )
            {
            HBufC16* string = static_cast <HBufC16*> ( readValue.AllocLC() );
            if ( string->Length() > 0 )
                {
                aArray.AppendL( *string );
                amount++;
                }
            CleanupStack::PopAndDestroy( string );
            }
        loc++;
        }
    CleanupStack::PopAndDestroy(); // keyArray.

    __PHSLOGSTRING1("[PHS] <--CPSetCallDivertingBase::GetNumbersFromSharedDataL: amount: %d", amount );
    return amount;
    }

// ---------------------------------------------------------------------------
// Gets the timer value index related to "cfnry" from shared data
// In error cases, return default value (30).
// ---------------------------------------------------------------------------
//
TInt CPSetCallDivertingBase::GetTimerValueL()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::GetTimerValueL" );
    
    TInt timerValue = 0;
    TInt error = KErrNone;

    if ( iAls == ESSSettingsAlsAlternate )
        {
        error = iRepository->Get( KSettingsCFTimerValueAls, timerValue );
        __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetTimerValueL: KSettingsCFTimerValueAls: %d", timerValue );
        __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetTimerValueL: error: %d", error );
        }
    else
        {
        error = iRepository->Get( KSettingsCFTimerValue, timerValue );

        __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetTimerValueL: KPsetCFTimerValue: %d", timerValue );
        __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetTimerValueL: error: %d", error );
        }
    
    if ( error != KErrNone || !VerifyDelayTime( timerValue ) )
        {
        __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::GetTimerValueL: KPsetDefaultTimerValue" );
        return KPsetDefaultTimerValue;
        }
    else
        {
        __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::GetTimerValueL: timerValue" );
        return timerValue;
        }
    }

// ---------------------------------------------------------------------------
// Sets the default time for "divert when not answered" to .ini file 
// ---------------------------------------------------------------------------
TInt CPSetCallDivertingBase::SetTimerValueL( const TInt& aValue )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::SetTimerValueL" );

    TInt retValue = KErrNone;

    // If delay time is not divisable by five, return.
    if ( !VerifyDelayTime( aValue ) )
        {
        return KErrArgument;
        }

    if ( iAls != ESSSettingsAlsAlternate )
        {
        retValue = iRepository->Set( KSettingsCFTimerValue, aValue );
        __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::SetTimerValueL: KPsetCFTimerValue: %d", aValue );
        }
    else
        {
        retValue = iRepository->Set( KSettingsCFTimerValueAls, aValue );
        __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::SetTimerValueL: KPsetCFTimerValueAls: %d", aValue );
        }

    __PHSLOGSTRING1("[PHS] <--CPSetCallDivertingBase::SetTimerValueL: retValue: %d", retValue );
    return retValue;
    }

// ---------------------------------------------------------------------------
// Verifies that delay time is divisible by five.
// ---------------------------------------------------------------------------
//
TBool CPSetCallDivertingBase::VerifyDelayTime( const TInt& aDelayTime )
    {
    TReal res = 0;
    Math::Mod( res, aDelayTime, KPsetDelayTimeDivider ); 
    return res == 0;
    }

// ---------------------------------------------------------------------------
// Swaps the most recently used number to first in the shared data file.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::SwapDefaultNumberL( const TInt& aLocation )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::SwapDefaultNumberL" );

    if ( aLocation == KPSetFirstDefNumber )
        {
        return; //no need to reorganize, number is already first
        }
    else if ( 
        !( aLocation == KPSetSecondDefNumber || 
           aLocation == KPSetThirdDefNumber ) )
        {
        User::Leave( KErrArgument );
        }

    CDesC16ArrayFlat* defNumbers = 
        new ( ELeave ) CDesC16ArrayFlat( KPSetDefaultNumberMax );
    CleanupStack::PushL( defNumbers );

    GetNumbersFromSharedDataL( iAls, *defNumbers );

    if ( iAls != ESSSettingsAlsAlternate ) 
    //EGetAlsLinePrimary or Not supported or Unknown
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBase::SwapDefaultNumberL: !ESSSettingsAlsAlternate" );
        switch( aLocation )
            {
            case KPSetSecondDefNumber:
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumber2, 
                        defNumbers->MdcaPoint(0) ) );
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumber1, 
                        defNumbers->MdcaPoint(1) ) );
                break;
            case KPSetThirdDefNumber:
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumber2, 
                        defNumbers->MdcaPoint(0) ) );
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumber3, 
                        defNumbers->MdcaPoint(1) ) );
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumber1, 
                        defNumbers->MdcaPoint(2) ) );
                break;
            default:
                break;
            }
        }            
    else //alternate line
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBase::SwapDefaultNumberL: ESSSettingsAlsAlternate" );
        switch( aLocation )
            {
            case KPSetSecondDefNumber:
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumberAls1, 
                        defNumbers->MdcaPoint(0) ) );
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumberAls2, 
                        defNumbers->MdcaPoint(1) ) );
                break;
            case KPSetThirdDefNumber:
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumberAls2, 
                        defNumbers->MdcaPoint(0) ) );
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumberAls3, 
                        defNumbers->MdcaPoint(1) ) );
                User::LeaveIfError( 
                    SaveKey( 
                        KSettingsCFDefaultNumberAls1, 
                        defNumbers->MdcaPoint(2) ) );
                break;
            default:
                break;
            }
        }
    CleanupStack::PopAndDestroy( defNumbers );
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::SwapDefaultNumberL" );
    }

// ---------------------------------------------------------------------------
// Request diverting status from network asynhronously.
// ---------------------------------------------------------------------------
void CPSetCallDivertingBase::GetDivertingStatusL( 
    const TServiceGroup aServiceGroup,
    const TCallDivertingCondition aCondition, 
    TBasicServiceGroups aBsc ) 
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::GetDivertingStatusL" );

    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }
    
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetDivertingStatusL: aServiceGroup: %d", aServiceGroup );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetDivertingStatusL: aCondition: %d", aCondition );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetDivertingStatusL: aBsc: %d", aBsc );

    iCFStatusCheck = ETrue;

    CPsetTelephony::CheckLineModeL( aServiceGroup, &iPhone, iLine );    
    if ( !iLine->SubSessionHandle() )
        {
        User::Leave( KErrBadHandle );
        }

    // in these cases basic service group will not be set
    if ( !( aCondition == EDivertConditionAllCalls || 
            aCondition == EDivertConditionAllConditionalCases ) )
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBase::GetDivertingStatusL: CancelAll should not come here" );
        ValidateBsc( aBsc );
        }

    iReason = PSetUtility::GetDivertReason( aCondition );    

    // Start to request for status.
    if (iCfInterrogator)
        {
        delete iCfInterrogator;
        iCfInterrogator = NULL;
        }
    iCfInterrogator = CRetrieveMobilePhoneCFList::NewL( iPhone );
    iCfInterrogator->Start( 
        iStatus, 
        iReason, 
        PSetUtility::ChangeToEtelInternal( aBsc ) );

    StartRequestingL( EPSetGetDivertStatus );

    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::GetDivertingStatusL" );
    }

// ---------------------------------------------------------------------------
// Creates param list for NotifyDivertChange method.
// ---------------------------------------------------------------------------
//
TCallDivertNotifySetting CPSetCallDivertingBase::CreateDivertNotifySetting( 
    TUnconditionalCFStatus aCurrentDivert, TBool aDivertChanged, 
    TBool aVoiceMbxDivert, TInt aBasic )
    {
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase iVideoDivert    = %d", iVideoDivert );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase iVoiceDivert    = %d", iVoiceDivert );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase aVoiceMbxDivert = %d", aVoiceMbxDivert );
    
    TCallDivertNotifySetting divertNotify;
    divertNotify.Initialize();

    divertNotify.iPreviousCfStatus = aCurrentDivert;
    divertNotify.iCfActivated = aDivertChanged;
    
    divertNotify.iBasicServiceCode = aBasic;

    if ( aVoiceMbxDivert )
        {
        divertNotify.iVmbxDivert = ETrue;
        }
               
    return divertNotify;
    }

// ---------------------------------------------------------------------------
// Gets Als information
// ---------------------------------------------------------------------------
//
TSelectedLine CPSetCallDivertingBase::GetSAAls()
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::GetSAAls" );
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
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBase::GetSAAls - Line 2 divert activation/check from Line 1" );
        alsValue = EAuxiliaryLine;
        }
    else if ( iBsc == ETelephony && iAls == ESSSettingsAlsAlternate )
        {
        __PHSLOGSTRING("[PHS]    CPSetCallDivertingBase::GetSAAls - Line 1 divert activation/check from Line 2" );
        alsValue = EPrimaryLine;
        }
    
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::GetSAAls: alsValue: %d", alsValue );
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::GetSAAls" );
    return alsValue;
    }

// ---------------------------------------------------------------------------
// Validates used bsc, if EUnknown, updates to real value
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::ValidateBsc( TBasicServiceGroups& aBsc )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::ValidateBsc" );
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
        if ( aBsc == EAllTeleAndBearer ||
             aBsc == EAllTele )
            {
            // if ALS is used, use 89 bsc
            if ( iAls == ESSSettingsAlsAlternate )
                {
                aBsc = EAltTele;
                }
            }
        }
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::ValidateBsc" );
    }

// ---------------------------------------------------------------------------
// Starts requesting
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::StartRequestingL( const TInt& aRequest )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::StartRequestingL" );
    if ( aRequest != KPsetRequestAlreadySet )
        {
        SetRequestStatus( static_cast <TPSetDivertRequest> ( aRequest ) );
        }
    
    SetActive();

    iObserver->SetEngineContact( iDivert );

    CleanupLeavePushL();    
    iObserver->HandleCFRequestingL( ETrue, EFalse );
    CleanupStack::Pop(); // CleanupCancelPushL
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::StartRequestingL" );
    }

// ---------------------------------------------------------------------------
// Inform request observer that request status has changed.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::SetRequestStatus( TPSetDivertRequest aStatus )
    {
    iCurrentReq = aStatus;
    if ( iReqObserver )
        {
        iReqObserver->RequestStatusChanged( aStatus );
        }
    }

// ---------------------------------------------------------------------------
// If divert activation/cancellation/etc. affects more than one bsc, 
//  inform PSUI about it.
// ---------------------------------------------------------------------------
TBool CPSetCallDivertingBase::IsMultiAffectingDivert( 
    const TCallDivertSetting& aDivert, TBasicServiceGroups aBsc )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::IsMultiAffectingDivert" );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::IsMultiAffectingDivert: aBsc = %d", aBsc );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::IsMultiAffectingDivert: aDivert.iCondition = %d", aDivert.iCondition );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::IsMultiAffectingDivert: aDivert.iStatus = %d", aDivert.iStatus );
    __PHSLOGSTRING1("[PHS]    CPSetCallDivertingBase::IsMultiAffectingDivert: aDivert.iServiceGroup = %d", aDivert.iServiceGroup );

    if ( iReason == RMobilePhone::ECallForwardingAllCases ||
         iReason == RMobilePhone::ECallForwardingAllConditionalCases ||
         iReason == RMobilePhone::ECallForwardingNotReachable )
        {
        __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::IsMultiAffectingDivert1" );
         return ETrue;
        }
    if ( iReason == RMobilePhone::ECallForwardingUnconditional )
        {
        //it is a MMI string
        if ( ( aBsc != EUnknown && aBsc != ETelephony ) && aDivert.iServiceGroup == EServiceGroupVoice ) 
            {
            __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::IsMultiAffectingDivert2" );
            return ETrue;
            }
        }
    
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::IsMultiAffectingDivert3" );
    return EFalse;
    }

// ---------------------------------------------------------------------------
// Push object into cleanupstack to catch leaving.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::CleanupLeavePushL()
      {
      CleanupStack::PushL( TCleanupItem( DoHandleLeave, this ) );
      }

// ---------------------------------------------------------------------------
// Object has caused a leave.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::DoHandleLeave( TAny* aAny )
      {
      REINTERPRET_CAST( CPSetCallDivertingBasicImpl*, aAny )->HandleLeave();
      }

// ---------------------------------------------------------------------------
// Things to do when leave occurs.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::HandleLeave()
    {
    CancelCurrentRequest();
    }

// ---------------------------------------------------------------------------
// Inform the request observer that request has been completed.
// ---------------------------------------------------------------------------
//
void CPSetCallDivertingBase::RequestCompleted( const TInt& aError )
    {
    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::RequestCompleted" );
    if ( aError != KErrNone )
        {
        SetRequestStatus( static_cast <TPSetDivertRequest> (aError) );
        }
    SetRequestStatus( EPSetNone );    
    iVoiceDivert = EFalse;

    if ( iReqObserver )
        {
        iReqObserver->RequestComplete(); 
        }
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::RequestCompleted" );
    }

// ---------------------------------------------------------------------------
// Verifies whether the divert-to number is to voice mail box.
// ---------------------------------------------------------------------------
//
TBool CPSetCallDivertingBase::IsVMBXDivertL( TDesC& /*aTelNumber*/ )
    {

    __PHSLOGSTRING("[PHS]--> CPSetCallDivertingBase::IsVMBXDivertL" );
    
    TBool result = EFalse;
    __PHSLOGSTRING("[PHS] <--CPSetCallDivertingBase::IsVMBXDivertL" );
    return result;
    }

// ---------------------------------------------------------------------------
// If SsSettings notifies of settings change, copy new value to member variable. 
// ---------------------------------------------------------------------------
void CPSetCallDivertingBase::PhoneSettingChanged( 
    TSSSettingsSetting aSetting, TInt aNewValue )
    {
    __PHSLOGSTRING2("[PHS]--> CPSetCallDivertingBase::PhoneSettingChanged ESSSettingsAls: %d, aNewValue: %d", aSetting, aNewValue );
    if ( aSetting == ESSSettingsAls )
        {
        iAls = static_cast <TSSSettingsAlsValue> (aNewValue);
        }
    __PHSLOGSTRING("[PHS]<-- CPSetCallDivertingBase::PhoneSettingChanged" );
    }

// End of File
