/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include <smcmockclassincludes.h>
#include <etelext.h>
#include <etelmm.h>
#include <ctsy/rmmcustomapi.h>
#include "rmmcustomapi.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMmCustomPtrHolder::CMmCustomPtrHolder
// -----------------------------------------------------------------------------
//
CMmCustomPtrHolder::CMmCustomPtrHolder(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CMmCustomPtrHolder::ConstructL
// -----------------------------------------------------------------------------
//
void CMmCustomPtrHolder::ConstructL( 
        const TInt /*aSizeOfPtrArray*/,
        const TInt /*aSizeOfPtrCArray*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CMmCustomPtrHolder::NewL
// -----------------------------------------------------------------------------
//
CMmCustomPtrHolder * CMmCustomPtrHolder::NewL( 
        const TInt aSizeOfPtrArray,
        const TInt aSizeOfPtrCArray )
    {
    SMC_MOCK_NEWL2( CMmCustomPtrHolder *, const TInt, aSizeOfPtrArray, 
        const TInt, aSizeOfPtrCArray )

    CMmCustomPtrHolder* self = new( ELeave ) CMmCustomPtrHolder;
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CMmCustomPtrHolder::~CMmCustomPtrHolder
// -----------------------------------------------------------------------------
//
CMmCustomPtrHolder::~CMmCustomPtrHolder(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::RMmCustomAPI
// -----------------------------------------------------------------------------
//
RMmCustomAPI::RMmCustomAPI(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ConstructL
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::Open
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::Open( 
        RMobilePhone & aPhone )
    {
    SMC_MOCK_METHOD1( TInt, RMobilePhone &, aPhone )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::Close
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::Close(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetDriveMode
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SetDriveMode( 
        TRequestStatus & aStatus,
        const TSetDriveMode aModeStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TSetDriveMode, aModeStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ActivateSimLock
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::ActivateSimLock( 
        const TSimLockPassword & aPassWord,
        const TLockNumber aNumber ) const
    {
    SMC_MOCK_METHOD2( TInt, const TSimLockPassword &, aPassWord, 
        const TLockNumber, aNumber )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ActivateSimLock
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ActivateSimLock( 
        TRequestStatus & aStatus,
        const TSimLockPassword & aPassWord,
        const TLockNumber aNumber ) const
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        const TSimLockPassword &, aPassWord, 
        const TLockNumber, aNumber )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::DeActivateSimLock
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::DeActivateSimLock( 
        const TSimLockPassword & aPassWord,
        const TLockNumber aNumber ) const
    {
    SMC_MOCK_METHOD2( TInt, const TSimLockPassword &, aPassWord, 
        const TLockNumber, aNumber )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::DeActivateSimLock
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::DeActivateSimLock( 
        TRequestStatus & aStatus,
        const TSimLockPassword & aPassWord,
        const TLockNumber aNumber ) const
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        const TSimLockPassword &, aPassWord, 
        const TLockNumber, aNumber )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ReadViagHomeZoneParams
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::ReadViagHomeZoneParams( 
        TViagParams & aParam,
        TViagElements & aElems ) const
    {
    SMC_MOCK_METHOD2( TInt, TViagParams &, aParam, 
        TViagElements &, aElems )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ReadViagHomeZoneParams
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ReadViagHomeZoneParams( 
        TRequestStatus & aStatus,
        TViagParams & aParam,
        TViagElements & aElems )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        TViagParams &, aParam, 
        TViagElements &, aElems )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ReadViagHomeZoneCache
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::ReadViagHomeZoneCache( 
        TViagCacheRecordId & aId,
        TViagCacheRecordContent & aRecord ) const
    {
    SMC_MOCK_METHOD2( TInt, TViagCacheRecordId &, aId, 
        TViagCacheRecordContent &, aRecord )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ReadViagHomeZoneCache
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ReadViagHomeZoneCache( 
        TRequestStatus & aStatus,
        TViagCacheRecordId & aId,
        TViagCacheRecordContent & aRecord )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        TViagCacheRecordId &, aId, 
        TViagCacheRecordContent &, aRecord )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::WriteViagHomeZoneCache
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::WriteViagHomeZoneCache( 
        const TViagCacheRecordId & aId,
        const TViagCacheRecordContent & aRecord ) const
    {
    SMC_MOCK_METHOD2( TInt, const TViagCacheRecordId &, aId, 
        const TViagCacheRecordContent &, aRecord )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetAlwaysOn
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SetAlwaysOn( 
        TRequestStatus & aStatus,
        TSetAlwaysOnMode & aMode ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TSetAlwaysOnMode &, aMode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyRauEvent
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyRauEvent( 
        TRequestStatus & aStatus,
        TRauEventStatus & aEventStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TRauEventStatus &, aEventStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::WriteViagHomeZoneCache
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::WriteViagHomeZoneCache( 
        TRequestStatus & aStatus,
        const TViagCacheRecordId & aId,
        const TViagCacheRecordContent & aRecord )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        const TViagCacheRecordId &, aId, 
        const TViagCacheRecordContent &, aRecord )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyDtmfEvent
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyDtmfEvent( 
        TRequestStatus & aStatus,
        TDtmfInfo & aInfo )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDtmfInfo &, aInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetDiagnosticInfo
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetDiagnosticInfo( 
        TName & aCallName ) const
    {
    SMC_MOCK_METHOD1( TInt, TName &, aCallName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetAlsBlocked
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::SetAlsBlocked( 
        const TSetAlsBlock aBlockStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, const TSetAlsBlock, aBlockStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetAlsBlocked
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SetAlsBlocked( 
        TRequestStatus & aStatus,
        const TSetAlsBlock aBlockStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TSetAlsBlock, aBlockStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetAlsBlocked
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetAlsBlocked( 
        TGetAlsBlockStatus & aBlockStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, TGetAlsBlockStatus &, aBlockStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetAlsBlocked
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetAlsBlocked( 
        TRequestStatus & aStatus,
        TGetAlsBlockStatus & aBlockStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TGetAlsBlockStatus &, aBlockStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetRemoteAlertingToneStatus
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetRemoteAlertingToneStatus( 
        TRemoteAlertingToneStatus & aToneStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, TRemoteAlertingToneStatus &, aToneStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CallOrigin
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::CallOrigin( 
        TName & aCallName,
        TCallOrigin & aOrigin ) const
    {
    SMC_MOCK_METHOD2( TInt, TName &, aCallName, 
        TCallOrigin &, aOrigin )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::TerminateCall
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::TerminateCall( 
        TName & aCallName ) const
    {
    SMC_MOCK_METHOD1( TInt, TName &, aCallName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::TerminateCall
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::TerminateCall( 
        TRequestStatus & aStatus,
        TName & aCallName )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TName &, aCallName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyAlsBlockedChanged
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyAlsBlockedChanged( 
        TRequestStatus & aStatus,
        TGetAlsBlockStatus & aBlockStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TGetAlsBlockStatus &, aBlockStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetCipheringInfo
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetCipheringInfo( 
        TCipheringInfo & aInfo )
    {
    SMC_MOCK_METHOD1( TInt, TCipheringInfo &, aInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetCipheringInfo
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetCipheringInfo( 
        TRequestStatus & aStatus,
        TCipheringInfo & aInfo )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TCipheringInfo &, aInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyCipheringInfoChange
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyCipheringInfoChange( 
        TRequestStatus & aStatus,
        TCipheringInfo & aInfo )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TCipheringInfo &, aInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyNSPSStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyNSPSStatus( 
        TRequestStatus & aStatus,
        TNspsStatus & aNspsStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TNspsStatus &, aNspsStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NetWakeup
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NetWakeup( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckAlsPpSupport
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::CheckAlsPpSupport( 
        TAlsSupport & aSupport ) const
    {
    SMC_MOCK_METHOD1( TInt, TAlsSupport &, aSupport )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckAlsPpSupport
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::CheckAlsPpSupport( 
        TRequestStatus & aStatus,
        TAlsSupport & aSupport )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TAlsSupport &, aSupport )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetNetworkProviderName
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetNetworkProviderName( 
        TDes & aNetworkProviderName ) const
    {
    SMC_MOCK_METHOD1( TInt, TDes &, aNetworkProviderName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetNetworkProviderName
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetNetworkProviderName( 
        TRequestStatus & aStatus,
        TDes & aNetworkProviderName )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes &, aNetworkProviderName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::IsBlocked
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::IsBlocked( 
        TSecurityCodeType & aType,
        TBool & aIsBlocked )
    {
    SMC_MOCK_METHOD2( TInt, TSecurityCodeType &, aType, 
        TBool &, aIsBlocked )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckSecurityCode
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::CheckSecurityCode( 
        const TSecurityCodeType aCode )
    {
    SMC_MOCK_METHOD1( TInt, const TSecurityCodeType, aCode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckSecurityCode
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::CheckSecurityCode( 
        TRequestStatus & aStatus,
        const TSecurityCodeType aCode )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TSecurityCodeType, aCode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetActivePin
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetActivePin( 
        RMobilePhone::TMobilePhoneSecurityCode & aActivePin )
    {
    SMC_MOCK_METHOD1( void, RMobilePhone::TMobilePhoneSecurityCode &, aActivePin )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetAirTimeDuration
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetAirTimeDuration( 
        TTimeIntervalSeconds & aTime ) const
    {
    SMC_MOCK_METHOD1( TInt, TTimeIntervalSeconds &, aTime )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckEmergencyNumber
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::CheckEmergencyNumber( 
        TEmerNumberCheckMode & aNumMode,
        TBool & aResult ) const
    {
    SMC_MOCK_METHOD2( TInt, TEmerNumberCheckMode &, aNumMode, 
        TBool &, aResult )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckEmergencyNumber
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::CheckEmergencyNumber( 
        TRequestStatus & aStatus,
        TEmerNumberCheckMode & aNumMode,
        TBool & aResult )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        TEmerNumberCheckMode &, aNumMode, 
        TBool &, aResult )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ClearCallBlackList
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ClearCallBlackList( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ClearCallBlackList
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::ClearCallBlackList(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SsAdditionalInfoNotification
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SsAdditionalInfoNotification( 
        TRequestStatus & aStatus,
        TSsAdditionalInfo & aSsAdditionalInfo )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TSsAdditionalInfo &, aSsAdditionalInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SsRequestCompleteNotification
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SsRequestCompleteNotification( 
        TRequestStatus & aStatus,
        TInt & aSsStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TInt &, aSsStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyPndCacheReady
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyPndCacheReady( 
        TRequestStatus & aStatus,
        TName & aPndName )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TName &, aPndName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetPndCacheStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetPndCacheStatus( 
        TRequestStatus & aStatus,
        RMmCustomAPI::TPndCacheStatus & aPndStatus,
        const TName & aPndName ) const
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        RMmCustomAPI::TPndCacheStatus &, aPndStatus, 
        const TName &, aPndName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetOperatorName
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetOperatorName( 
        TRequestStatus & aStatus,
        TOperatorNameInfo & aOperatorNameInfo )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TOperatorNameInfo &, aOperatorNameInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetProgrammableOperatorLogo
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetProgrammableOperatorLogo( 
        TRequestStatus & aStatus,
        TOperatorId & aOperatorId,
        TOperatorLogo & aLogo )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        TOperatorId &, aOperatorId, 
        TOperatorLogo &, aLogo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyProgrammableOperatorLogoChange
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyProgrammableOperatorLogoChange( 
        TRequestStatus & aStatus,
        TOperatorId & aOperatorId )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TOperatorId &, aOperatorId )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SatRefreshCompleteNotification
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SatRefreshCompleteNotification( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifySsNetworkEvent
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifySsNetworkEvent( 
        TRequestStatus & aStatus,
        TSsTypeAndMode & aSsTypeAndMode,
        TSsInfo & aSsInfo )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        TSsTypeAndMode &, aSsTypeAndMode, 
        TSsInfo &, aSsInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CancelUssdSession
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::CancelUssdSession(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CancelUssdSession
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::CancelUssdSession( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckTwoDigitDialSupport
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::CheckTwoDigitDialSupport( 
        TTwoDigitDialSupport & aSupport ) const
    {
    SMC_MOCK_METHOD1( TInt, TTwoDigitDialSupport &, aSupport )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::CheckTwoDigitDialSupport
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::CheckTwoDigitDialSupport( 
        TRequestStatus & aStatus,
        TTwoDigitDialSupport & aSupport )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TTwoDigitDialSupport &, aSupport )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ResetNetServer
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ResetNetServer(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ResetNetServer
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ResetNetServer( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ReleaseFile
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ReleaseFile( 
        TRequestStatus & aStatus,
        const TDesC & aFileName )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TDesC &, aFileName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::RestartFile
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::RestartFile( 
        TRequestStatus & aStatus,
        const TDesC & aFileName )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TDesC &, aFileName )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::StartSimCbTopicBrowsing
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::StartSimCbTopicBrowsing(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetNextSimCbTopic
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetNextSimCbTopic( 
        TSimCbTopic & aSimCbTopic )
    {
    SMC_MOCK_METHOD1( TInt, TSimCbTopic &, aSimCbTopic )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetNextSimCbTopic
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetNextSimCbTopic( 
        TRequestStatus & aStatus,
        TSimCbTopic & aSimCbTopic )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TSimCbTopic &, aSimCbTopic )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::DeleteSimCbTopic
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::DeleteSimCbTopic( 
        const TUint & aSimCbTopicNumber )
    {
    SMC_MOCK_METHOD1( TInt, const TUint &, aSimCbTopicNumber )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::DeleteSimCbTopic
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::DeleteSimCbTopic( 
        TRequestStatus & aStatus,
        const TUint & aSimCbTopicNumber )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TUint &, aSimCbTopicNumber )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyNetworkConnectionFailure
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyNetworkConnectionFailure( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SendAPDUReq
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::SendAPDUReq( 
        TApdu & anAPDUReq )
    {
    SMC_MOCK_METHOD1( TInt, TApdu &, anAPDUReq )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SendAPDUReq
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SendAPDUReq( 
        TRequestStatus & aStatus,
        TApdu & anAPDUReq )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TApdu &, anAPDUReq )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::DisablePhoneLock
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::DisablePhoneLock( 
        TRequestStatus & aStatus,
        RMobilePhone::TMobilePassword & aVerifyCode ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        RMobilePhone::TMobilePassword &, aVerifyCode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyEGprsInfoChange
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyEGprsInfoChange( 
        TRequestStatus & aStatus,
        TDes8 & aGprsInfo ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes8 &, aGprsInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetEGprsInfo
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetEGprsInfo( 
        TRequestStatus & aStatus,
        TDes8 & aGprsInfo ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes8 &, aGprsInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ReadSimFile
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ReadSimFile( 
        TRequestStatus & aStatus,
        TDes8 & aSimFileInfo,
        TDes8 & aResponseBytes ) const
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        TDes8 &, aSimFileInfo, 
        TDes8 &, aResponseBytes )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetLifeTime
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetLifeTime( 
        TDes8 & aLifeTimeInfo ) const
    {
    SMC_MOCK_METHOD1( TInt, TDes8 &, aLifeTimeInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetLifeTime
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetLifeTime( 
        TRequestStatus & aStatus,
        TDes8 & aLifeTimeInfo ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes8 &, aLifeTimeInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::Get3GPBInfo
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::Get3GPBInfo( 
        TRequestStatus & aStatus,
        T3GPBInfo & aInfo ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        T3GPBInfo &, aInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetSystemNetworkModes
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetSystemNetworkModes( 
        TUint32 & aSupportedNetworkModes ) const
    {
    SMC_MOCK_METHOD1( TInt, TUint32 &, aSupportedNetworkModes )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetSystemNetworkModes
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetSystemNetworkModes( 
        TRequestStatus & aStatus,
        TUint32 & aSupportedNetworkModes ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TUint32 &, aSupportedNetworkModes )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetSystemNetworkMode
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::SetSystemNetworkMode( 
        const TNetworkModeCaps aNetworkMode )
    {
    SMC_MOCK_METHOD1( TInt, const TNetworkModeCaps, aNetworkMode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetSystemNetworkMode
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SetSystemNetworkMode( 
        TRequestStatus & aStatus,
        const TNetworkModeCaps aNetworkMode )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TNetworkModeCaps, aNetworkMode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetCurrentSystemNetworkModes
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetCurrentSystemNetworkModes( 
        TUint32 & aCurrentNetworkModes ) const
    {
    SMC_MOCK_METHOD1( TInt, TUint32 &, aCurrentNetworkModes )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetCurrentSystemNetworkModes
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetCurrentSystemNetworkModes( 
        TRequestStatus & aStatus,
        TUint32 & aCurrentNetworkModes ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TUint32 &, aCurrentNetworkModes )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetSystemNetworkBand
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::GetSystemNetworkBand( 
        TBandSelection & aSupportedNetworkBand,
        TNetworkModeCaps & aNetworkMode ) const
    {
    SMC_MOCK_METHOD2( TInt, TBandSelection &, aSupportedNetworkBand, 
        TNetworkModeCaps &, aNetworkMode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetSystemNetworkBand
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetSystemNetworkBand( 
        TRequestStatus & aStatus,
        TBandSelection & aSupportedNetworkBand,
        TNetworkModeCaps & aNetworkMode ) const
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        TBandSelection &, aSupportedNetworkBand, 
        TNetworkModeCaps &, aNetworkMode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetSystemNetworkBand
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::SetSystemNetworkBand( 
        const TBandSelection aNetworkBand,
        const TNetworkModeCaps aNetworkMode )
    {
    SMC_MOCK_METHOD2( TInt, const TBandSelection, aNetworkBand, 
        const TNetworkModeCaps, aNetworkMode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetSystemNetworkBand
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SetSystemNetworkBand( 
        TRequestStatus & aStatus,
        const TBandSelection aNetworkBand,
        const TNetworkModeCaps aNetworkMode )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        const TBandSelection, aNetworkBand, 
        const TNetworkModeCaps, aNetworkMode )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::PowerSimOn
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::PowerSimOn( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::PowerSimOff
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::PowerSimOff( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SimWarmReset
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SimWarmReset( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SendAPDUReq
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SendAPDUReq( 
        TRequestStatus & aStatus,
        TApduParameters & aParams )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TApduParameters &, aParams )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetATR
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetATR( 
        TRequestStatus & aStatus,
        TDes8 & aATR )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes8 &, aATR )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetSimCardReaderStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetSimCardReaderStatus( 
        TRequestStatus & aStatus,
        TSimCardReaderStatus & aCardReaderStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TSimCardReaderStatus &, aCardReaderStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifySimCardStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifySimCardStatus( 
        TRequestStatus & aStatus,
        TSIMCardStatus & aCardStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TSIMCardStatus &, aCardStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetWlanSimAuthenticationData
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetWlanSimAuthenticationData( 
        TRequestStatus & aReqStatus,
        TDes8 & aAuthenticationData ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aReqStatus, 
        TDes8 &, aAuthenticationData )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::SetSimMessageStatusRead
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::SetSimMessageStatusRead( 
        TTime & aTime,
        TInt & aTimezoneDiff ) const
    {
    SMC_MOCK_METHOD2( void, TTime &, aTime, 
        TInt &, aTimezoneDiff )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::WriteViagHomeZoneUHZIUESettings
// -----------------------------------------------------------------------------
//
TInt RMmCustomAPI::WriteViagHomeZoneUHZIUESettings( 
        TViagUHZIUESettings & aSettings ) const
    {
    SMC_MOCK_METHOD1( TInt, TViagUHZIUESettings &, aSettings )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::WriteViagHomeZoneUHZIUESettings
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::WriteViagHomeZoneUHZIUESettings( 
        TRequestStatus & aStatus,
        TViagUHZIUESettings & aSettings ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TViagUHZIUESettings &, aSettings )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::ReadHSxPAStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::ReadHSxPAStatus( 
        TRequestStatus & aStatus,
        THSxPAStatus & aHSxPAStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        THSxPAStatus &, aHSxPAStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::WriteHSxPAStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::WriteHSxPAStatus( 
        TRequestStatus & aStatus,
        THSxPAStatus & aHSxPAStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        THSxPAStatus &, aHSxPAStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyHSxPAStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyHSxPAStatus( 
        TRequestStatus & aStatus,
        THSxPAStatus & aHSxPAStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        THSxPAStatus &, aHSxPAStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetIccCallForwardingIndicatorStatus
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetIccCallForwardingIndicatorStatus( 
        TRequestStatus & aReqStatus,
        TDes8 & aCFIndicators ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aReqStatus, 
        TDes8 &, aCFIndicators )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyIccCallForwardingStatusChange
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyIccCallForwardingStatusChange( 
        TRequestStatus & aReqStatus,
        TDes8 & aCFIndicators )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aReqStatus, 
        TDes8 &, aCFIndicators )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetCellInfo
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetCellInfo( 
        TRequestStatus & aStatus,
        TDes8 & aCellInfo ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes8 &, aCellInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyCellInfoChange
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyCellInfoChange( 
        TRequestStatus & aStatus,
        TDes8 & aCellInfo ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes8 &, aCellInfo )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::GetUSIMServiceSupport
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::GetUSIMServiceSupport( 
        TRequestStatus & aStatus,
        TAppSupport & aParams ) const
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TAppSupport &, aParams )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::NotifyRemoteAlertingToneStatusChange
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::NotifyRemoteAlertingToneStatusChange( 
        TRequestStatus & aStatus,
        TRemoteAlertingToneStatus & aToneStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TRemoteAlertingToneStatus &, aToneStatus )
    }


// -----------------------------------------------------------------------------
// RMmCustomAPI::Destruct
// -----------------------------------------------------------------------------
//
void RMmCustomAPI::Destruct(  )
    {
    SMC_MOCK_METHOD0( void )
    }


