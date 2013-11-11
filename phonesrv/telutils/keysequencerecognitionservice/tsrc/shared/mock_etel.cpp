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
#include <f32file.h>
#include "etel.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RPhone::RPhone
// -----------------------------------------------------------------------------
//
RPhone::RPhone(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RPhone::ConstructL
// -----------------------------------------------------------------------------
//
void RPhone::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RPhone::Open
// -----------------------------------------------------------------------------
//
TInt RPhone::Open( 
        RTelServer & aSession,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RTelServer &, aSession, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RPhone::Close
// -----------------------------------------------------------------------------
//
void RPhone::Close(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RPhone::Initialise
// -----------------------------------------------------------------------------
//
TInt RPhone::Initialise(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RPhone::Initialise
// -----------------------------------------------------------------------------
//
void RPhone::Initialise( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RPhone::InitialiseCancel
// -----------------------------------------------------------------------------
//
void RPhone::InitialiseCancel(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RPhone::GetCaps
// -----------------------------------------------------------------------------
//
TInt RPhone::GetCaps( 
        TCaps & aCaps ) const
    {
    SMC_MOCK_METHOD1( TInt, TCaps &, aCaps )
    }


// -----------------------------------------------------------------------------
// RPhone::NotifyCapsChange
// -----------------------------------------------------------------------------
//
void RPhone::NotifyCapsChange( 
        TRequestStatus & aStatus,
        TCaps & aCaps )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TCaps &, aCaps )
    }


// -----------------------------------------------------------------------------
// RPhone::NotifyCapsChangeCancel
// -----------------------------------------------------------------------------
//
void RPhone::NotifyCapsChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RPhone::GetStatus
// -----------------------------------------------------------------------------
//
TInt RPhone::GetStatus( 
        TStatus & aStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, TStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RPhone::EnumerateLines
// -----------------------------------------------------------------------------
//
TInt RPhone::EnumerateLines( 
        TInt & aCount ) const
    {
    SMC_MOCK_METHOD1( TInt, TInt &, aCount )
    }


// -----------------------------------------------------------------------------
// RPhone::GetLineInfo
// -----------------------------------------------------------------------------
//
TInt RPhone::GetLineInfo( 
        const TInt aIndex,
        TLineInfo & aLineInfo ) const
    {
    SMC_MOCK_METHOD2( TInt, const TInt, aIndex, 
        TLineInfo &, aLineInfo )
    }


// -----------------------------------------------------------------------------
// RPhone::GetInfo
// -----------------------------------------------------------------------------
//
TInt RPhone::GetInfo( 
        TPhoneInfo & aPhoneInfo ) const
    {
    SMC_MOCK_METHOD1( TInt, TPhoneInfo &, aPhoneInfo )
    }


// -----------------------------------------------------------------------------
// RPhone::NotifyModemDetected
// -----------------------------------------------------------------------------
//
void RPhone::NotifyModemDetected( 
        TRequestStatus & aStatus,
        TModemDetection & aDetection )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TModemDetection &, aDetection )
    }


// -----------------------------------------------------------------------------
// RPhone::NotifyModemDetectedCancel
// -----------------------------------------------------------------------------
//
void RPhone::NotifyModemDetectedCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RPhone::SetEmergencyClient
// -----------------------------------------------------------------------------
//
TInt RPhone::SetEmergencyClient( 
        const TUint32 aEmergencyRequest ) const
    {
    SMC_MOCK_METHOD1( TInt, const TUint32, aEmergencyRequest )
    }


// -----------------------------------------------------------------------------
// RPhone::Destruct
// -----------------------------------------------------------------------------
//
void RPhone::Destruct(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::RCall
// -----------------------------------------------------------------------------
//
RCall::RCall(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RCall::ConstructL
// -----------------------------------------------------------------------------
//
void RCall::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RCall::OpenNewCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenNewCall( 
        RTelServer & aServer,
        const TDesC & aName,
        TDes & aNewName )
    {
    SMC_MOCK_METHOD3( TInt, RTelServer &, aServer, 
        const TDesC &, aName, 
        TDes &, aNewName )
    }


// -----------------------------------------------------------------------------
// RCall::OpenNewCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenNewCall( 
        RTelServer & aServer,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RTelServer &, aServer, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RCall::OpenNewCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenNewCall( 
        RPhone & aPhone,
        const TDesC & aName,
        TDes & aNewName )
    {
    SMC_MOCK_METHOD3( TInt, RPhone &, aPhone, 
        const TDesC &, aName, 
        TDes &, aNewName )
    }


// -----------------------------------------------------------------------------
// RCall::OpenNewCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenNewCall( 
        RPhone & aPhone,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RPhone &, aPhone, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RCall::OpenNewCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenNewCall( 
        RLine & aLine,
        TDes & aNewName )
    {
    SMC_MOCK_METHOD2( TInt, RLine &, aLine, 
        TDes &, aNewName )
    }


// -----------------------------------------------------------------------------
// RCall::OpenNewCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenNewCall( 
        RLine & aLine )
    {
    SMC_MOCK_METHOD1( TInt, RLine &, aLine )
    }


// -----------------------------------------------------------------------------
// RCall::OpenExistingCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenExistingCall( 
        RTelServer & aServer,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RTelServer &, aServer, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RCall::OpenExistingCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenExistingCall( 
        RPhone & aPhone,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RPhone &, aPhone, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RCall::OpenExistingCall
// -----------------------------------------------------------------------------
//
TInt RCall::OpenExistingCall( 
        RLine & aLine,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RLine &, aLine, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RCall::Close
// -----------------------------------------------------------------------------
//
void RCall::Close(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::GetStatus
// -----------------------------------------------------------------------------
//
TInt RCall::GetStatus( 
        TStatus & aStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, TStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RCall::GetCaps
// -----------------------------------------------------------------------------
//
TInt RCall::GetCaps( 
        TCaps & aCaps ) const
    {
    SMC_MOCK_METHOD1( TInt, TCaps &, aCaps )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyCapsChange
// -----------------------------------------------------------------------------
//
void RCall::NotifyCapsChange( 
        TRequestStatus & aStatus,
        TCaps & aCaps )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TCaps &, aCaps )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyCapsChangeCancel
// -----------------------------------------------------------------------------
//
void RCall::NotifyCapsChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::GetBearerServiceInfo
// -----------------------------------------------------------------------------
//
TInt RCall::GetBearerServiceInfo( 
        TBearerService & aBearerService ) const
    {
    SMC_MOCK_METHOD1( TInt, TBearerService &, aBearerService )
    }


// -----------------------------------------------------------------------------
// RCall::GetInfo
// -----------------------------------------------------------------------------
//
TInt RCall::GetInfo( 
        TCallInfo & aCallInfo ) const
    {
    SMC_MOCK_METHOD1( TInt, TCallInfo &, aCallInfo )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyHookChange
// -----------------------------------------------------------------------------
//
void RCall::NotifyHookChange( 
        TRequestStatus & aStatus,
        THookStatus & aHookStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        THookStatus &, aHookStatus )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyHookChangeCancel
// -----------------------------------------------------------------------------
//
void RCall::NotifyHookChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyStatusChange
// -----------------------------------------------------------------------------
//
void RCall::NotifyStatusChange( 
        TRequestStatus & aStatus,
        TStatus & aCallStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TStatus &, aCallStatus )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyStatusChangeCancel
// -----------------------------------------------------------------------------
//
void RCall::NotifyStatusChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyCallDurationChange
// -----------------------------------------------------------------------------
//
void RCall::NotifyCallDurationChange( 
        TRequestStatus & aStatus,
        TTimeIntervalSeconds & aTime )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TTimeIntervalSeconds &, aTime )
    }


// -----------------------------------------------------------------------------
// RCall::NotifyCallDurationChangeCancel
// -----------------------------------------------------------------------------
//
void RCall::NotifyCallDurationChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::GetCallDuration
// -----------------------------------------------------------------------------
//
TInt RCall::GetCallDuration( 
        TTimeIntervalSeconds & aTime ) const
    {
    SMC_MOCK_METHOD1( TInt, TTimeIntervalSeconds &, aTime )
    }


// -----------------------------------------------------------------------------
// RCall::Dial
// -----------------------------------------------------------------------------
//
TInt RCall::Dial( 
        const TTelNumberC & aTelNumber ) const
    {
    SMC_MOCK_METHOD1( TInt, const TTelNumberC &, aTelNumber )
    }


// -----------------------------------------------------------------------------
// RCall::Dial
// -----------------------------------------------------------------------------
//
TInt RCall::Dial( 
        const TDesC8 & aCallParams,
        const TTelNumberC & aTelNumber ) const
    {
    SMC_MOCK_METHOD2( TInt, const TDesC8 &, aCallParams, 
        const TTelNumberC &, aTelNumber )
    }


// -----------------------------------------------------------------------------
// RCall::Dial
// -----------------------------------------------------------------------------
//
void RCall::Dial( 
        TRequestStatus & aStatus,
        const TTelNumberC & aTelNumber )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TTelNumberC &, aTelNumber )
    }


// -----------------------------------------------------------------------------
// RCall::Dial
// -----------------------------------------------------------------------------
//
void RCall::Dial( 
        TRequestStatus & aStatus,
        const TDesC8 & aCallParams,
        const TTelNumberC & aTelNumber )
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aStatus, 
        const TDesC8 &, aCallParams, 
        const TTelNumberC &, aTelNumber )
    }


// -----------------------------------------------------------------------------
// RCall::DialCancel
// -----------------------------------------------------------------------------
//
void RCall::DialCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::Connect
// -----------------------------------------------------------------------------
//
TInt RCall::Connect(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RCall::Connect
// -----------------------------------------------------------------------------
//
TInt RCall::Connect( 
        const TDesC8 & aCallParams ) const
    {
    SMC_MOCK_METHOD1( TInt, const TDesC8 &, aCallParams )
    }


// -----------------------------------------------------------------------------
// RCall::Connect
// -----------------------------------------------------------------------------
//
void RCall::Connect( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RCall::Connect
// -----------------------------------------------------------------------------
//
void RCall::Connect( 
        TRequestStatus & aStatus,
        const TDesC8 & aCallParams )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TDesC8 &, aCallParams )
    }


// -----------------------------------------------------------------------------
// RCall::ConnectCancel
// -----------------------------------------------------------------------------
//
void RCall::ConnectCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::AnswerIncomingCall
// -----------------------------------------------------------------------------
//
TInt RCall::AnswerIncomingCall(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RCall::AnswerIncomingCall
// -----------------------------------------------------------------------------
//
TInt RCall::AnswerIncomingCall( 
        const TDesC8 & aCallParams ) const
    {
    SMC_MOCK_METHOD1( TInt, const TDesC8 &, aCallParams )
    }


// -----------------------------------------------------------------------------
// RCall::AnswerIncomingCall
// -----------------------------------------------------------------------------
//
void RCall::AnswerIncomingCall( 
        TRequestStatus & aStatus )
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RCall::AnswerIncomingCall
// -----------------------------------------------------------------------------
//
void RCall::AnswerIncomingCall( 
        TRequestStatus & aStatus,
        const TDesC8 & aCallParams )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TDesC8 &, aCallParams )
    }


// -----------------------------------------------------------------------------
// RCall::AnswerIncomingCallCancel
// -----------------------------------------------------------------------------
//
void RCall::AnswerIncomingCallCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::HangUp
// -----------------------------------------------------------------------------
//
TInt RCall::HangUp(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RCall::HangUp
// -----------------------------------------------------------------------------
//
void RCall::HangUp( 
        TRequestStatus & aStatus ) const
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RCall::HangUpCancel
// -----------------------------------------------------------------------------
//
void RCall::HangUpCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::LoanDataPort
// -----------------------------------------------------------------------------
//
TInt RCall::LoanDataPort( 
        TCommPort & aDataPort ) const
    {
    SMC_MOCK_METHOD1( TInt, TCommPort &, aDataPort )
    }


// -----------------------------------------------------------------------------
// RCall::LoanDataPort
// -----------------------------------------------------------------------------
//
void RCall::LoanDataPort( 
        TRequestStatus & aStatus,
        TCommPort & aDataPort )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TCommPort &, aDataPort )
    }


// -----------------------------------------------------------------------------
// RCall::LoanDataPortCancel
// -----------------------------------------------------------------------------
//
void RCall::LoanDataPortCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::RecoverDataPort
// -----------------------------------------------------------------------------
//
TInt RCall::RecoverDataPort(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RCall::TransferOwnership
// -----------------------------------------------------------------------------
//
TInt RCall::TransferOwnership(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RCall::AcquireOwnership
// -----------------------------------------------------------------------------
//
void RCall::AcquireOwnership( 
        TRequestStatus & aStatus ) const
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RCall::AcquireOwnershipCancel
// -----------------------------------------------------------------------------
//
void RCall::AcquireOwnershipCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RCall::GetOwnershipStatus
// -----------------------------------------------------------------------------
//
TInt RCall::GetOwnershipStatus( 
        TOwnershipStatus & aOwnershipStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, TOwnershipStatus &, aOwnershipStatus )
    }


// -----------------------------------------------------------------------------
// RCall::GetCallParams
// -----------------------------------------------------------------------------
//
TInt RCall::GetCallParams( 
        TDes8 & aParams ) const
    {
    SMC_MOCK_METHOD1( TInt, TDes8 &, aParams )
    }


// -----------------------------------------------------------------------------
// RCall::GetFaxSettings
// -----------------------------------------------------------------------------
//
TInt RCall::GetFaxSettings( 
        TFaxSessionSettings & aSettings ) const
    {
    SMC_MOCK_METHOD1( TInt, TFaxSessionSettings &, aSettings )
    }


// -----------------------------------------------------------------------------
// RCall::SetFaxSettings
// -----------------------------------------------------------------------------
//
TInt RCall::SetFaxSettings( 
        const TFaxSessionSettings & aSettings ) const
    {
    SMC_MOCK_METHOD1( TInt, const TFaxSessionSettings &, aSettings )
    }


// -----------------------------------------------------------------------------
// RCall::AdoptFaxSharedHeaderFile
// -----------------------------------------------------------------------------
//
TInt RCall::AdoptFaxSharedHeaderFile( 
        const RFile & aSharedFile ) const
    {
    SMC_MOCK_METHOD1( TInt, const RFile &, aSharedFile )
    }


// -----------------------------------------------------------------------------
// RCall::ReferenceCount
// -----------------------------------------------------------------------------
//
TInt RCall::ReferenceCount(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RCall::Destruct
// -----------------------------------------------------------------------------
//
void RCall::Destruct(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RFax::RFax
// -----------------------------------------------------------------------------
//
RFax::RFax(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RFax::ConstructL
// -----------------------------------------------------------------------------
//
void RFax::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RFax::Open
// -----------------------------------------------------------------------------
//
TInt RFax::Open( 
        RCall & aCall )
    {
    SMC_MOCK_METHOD1( TInt, RCall &, aCall )
    }


// -----------------------------------------------------------------------------
// RFax::Close
// -----------------------------------------------------------------------------
//
void RFax::Close(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RFax::Read
// -----------------------------------------------------------------------------
//
void RFax::Read( 
        TRequestStatus & aStatus,
        TDes8 & aDes )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TDes8 &, aDes )
    }


// -----------------------------------------------------------------------------
// RFax::Write
// -----------------------------------------------------------------------------
//
void RFax::Write( 
        TRequestStatus & aStatus,
        const TDesC8 & aDes )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        const TDesC8 &, aDes )
    }


// -----------------------------------------------------------------------------
// RFax::WaitForEndOfPage
// -----------------------------------------------------------------------------
//
void RFax::WaitForEndOfPage( 
        TRequestStatus & aStatus ) const
    {
    SMC_MOCK_METHOD1( void, TRequestStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RFax::TerminateFaxSession
// -----------------------------------------------------------------------------
//
TInt RFax::TerminateFaxSession(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RFax::GetProgress
// -----------------------------------------------------------------------------
//
TInt RFax::GetProgress( 
        TProgress & aProgress )
    {
    SMC_MOCK_METHOD1( TInt, TProgress &, aProgress )
    }


// -----------------------------------------------------------------------------
// RFax::Destruct
// -----------------------------------------------------------------------------
//
void RFax::Destruct(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RLine::RLine
// -----------------------------------------------------------------------------
//
RLine::RLine(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RLine::ConstructL
// -----------------------------------------------------------------------------
//
void RLine::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RLine::Open
// -----------------------------------------------------------------------------
//
TInt RLine::Open( 
        RPhone & aPhone,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RPhone &, aPhone, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RLine::Open
// -----------------------------------------------------------------------------
//
TInt RLine::Open( 
        RTelServer & aServer,
        const TDesC & aName )
    {
    SMC_MOCK_METHOD2( TInt, RTelServer &, aServer, 
        const TDesC &, aName )
    }


// -----------------------------------------------------------------------------
// RLine::Close
// -----------------------------------------------------------------------------
//
void RLine::Close(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RLine::GetCaps
// -----------------------------------------------------------------------------
//
TInt RLine::GetCaps( 
        TCaps & aCaps ) const
    {
    SMC_MOCK_METHOD1( TInt, TCaps &, aCaps )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyCapsChange
// -----------------------------------------------------------------------------
//
void RLine::NotifyCapsChange( 
        TRequestStatus & aStatus,
        TCaps & aCaps )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TCaps &, aCaps )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyCapsChangeCancel
// -----------------------------------------------------------------------------
//
void RLine::NotifyCapsChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RLine::GetStatus
// -----------------------------------------------------------------------------
//
TInt RLine::GetStatus( 
        RCall::TStatus & aStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, RCall::TStatus &, aStatus )
    }


// -----------------------------------------------------------------------------
// RLine::GetHookStatus
// -----------------------------------------------------------------------------
//
TInt RLine::GetHookStatus( 
        RCall::THookStatus & aHookStatus ) const
    {
    SMC_MOCK_METHOD1( TInt, RCall::THookStatus &, aHookStatus )
    }


// -----------------------------------------------------------------------------
// RLine::EnumerateCall
// -----------------------------------------------------------------------------
//
TInt RLine::EnumerateCall( 
        TInt & aCount ) const
    {
    SMC_MOCK_METHOD1( TInt, TInt &, aCount )
    }


// -----------------------------------------------------------------------------
// RLine::GetCallInfo
// -----------------------------------------------------------------------------
//
TInt RLine::GetCallInfo( 
        TInt aIndex,
        TCallInfo & aCallInfo ) const
    {
    SMC_MOCK_METHOD2( TInt, TInt, aIndex, 
        TCallInfo &, aCallInfo )
    }


// -----------------------------------------------------------------------------
// RLine::GetInfo
// -----------------------------------------------------------------------------
//
TInt RLine::GetInfo( 
        TLineInfo & aLineInfo ) const
    {
    SMC_MOCK_METHOD1( TInt, TLineInfo &, aLineInfo )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyIncomingCall
// -----------------------------------------------------------------------------
//
void RLine::NotifyIncomingCall( 
        TRequestStatus & aStatus,
        TName & aName )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TName &, aName )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyIncomingCallCancel
// -----------------------------------------------------------------------------
//
void RLine::NotifyIncomingCallCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyHookChange
// -----------------------------------------------------------------------------
//
void RLine::NotifyHookChange( 
        TRequestStatus & aStatus,
        RCall::THookStatus & aHookStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        RCall::THookStatus &, aHookStatus )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyHookChangeCancel
// -----------------------------------------------------------------------------
//
void RLine::NotifyHookChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyStatusChange
// -----------------------------------------------------------------------------
//
void RLine::NotifyStatusChange( 
        TRequestStatus & aStatus,
        RCall::TStatus & aLineStatus )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        RCall::TStatus &, aLineStatus )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyStatusChangeCancel
// -----------------------------------------------------------------------------
//
void RLine::NotifyStatusChangeCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyCallAdded
// -----------------------------------------------------------------------------
//
void RLine::NotifyCallAdded( 
        TRequestStatus & aStatus,
        TName & aName )
    {
    SMC_MOCK_METHOD2( void, TRequestStatus &, aStatus, 
        TName &, aName )
    }


// -----------------------------------------------------------------------------
// RLine::NotifyCallAddedCancel
// -----------------------------------------------------------------------------
//
void RLine::NotifyCallAddedCancel(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RLine::Destruct
// -----------------------------------------------------------------------------
//
void RLine::Destruct(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::RTelSubSessionBase
// -----------------------------------------------------------------------------
//
RTelSubSessionBase::RTelSubSessionBase(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::CancelAsyncRequest
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::CancelAsyncRequest( 
        TInt aReqToCancel ) const
    {
    SMC_MOCK_METHOD1( void, TInt, aReqToCancel )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Blank
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Blank( 
        const TInt aIpc,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD2( TInt, const TInt, aIpc, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Blank
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Blank( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD3( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Set( 
        const TInt aIpc,
        const TDesC8 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD3( TInt, const TInt, aIpc, 
        const TDesC8 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Set( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC8 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC8 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Get( 
        const TInt aIpc,
        TDes8 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD3( TInt, const TInt, aIpc, 
        TDes8 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Get( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        TDes8 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        TDes8 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Set( 
        const TInt aIpc,
        const TDesC8 & aDes1,
        const TDesC8 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( TInt, const TInt, aIpc, 
        const TDesC8 &, aDes1, 
        const TDesC8 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Set( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC8 & aDes1,
        const TDesC8 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC8 &, aDes1, 
        const TDesC8 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Get( 
        const TInt aIpc,
        TDes8 & aDes1,
        TDes8 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( TInt, const TInt, aIpc, 
        TDes8 &, aDes1, 
        TDes8 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Get( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        TDes8 & aDes1,
        TDes8 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        TDes8 &, aDes1, 
        TDes8 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Set( 
        const TInt aIpc,
        const TDesC16 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD3( TInt, const TInt, aIpc, 
        const TDesC16 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Set( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC16 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC16 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Get( 
        const TInt aIpc,
        TDes16 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD3( TInt, const TInt, aIpc, 
        TDes16 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Get( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        TDes16 & aDes,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        TDes16 &, aDes, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Set( 
        const TInt aIpc,
        const TDesC16 & aDes1,
        const TDesC16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( TInt, const TInt, aIpc, 
        const TDesC16 &, aDes1, 
        const TDesC16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Set( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC16 & aDes1,
        const TDesC16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC16 &, aDes1, 
        const TDesC16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Set( 
        const TInt aIpc,
        const TDesC8 & aDes1,
        const TDesC16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( TInt, const TInt, aIpc, 
        const TDesC8 &, aDes1, 
        const TDesC16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Set( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC8 & aDes1,
        const TDesC16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC8 &, aDes1, 
        const TDesC16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Get( 
        const TInt aIpc,
        TDes16 & aDes1,
        TDes16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( TInt, const TInt, aIpc, 
        TDes16 &, aDes1, 
        TDes16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Get( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        TDes16 & aDes1,
        TDes16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        TDes16 &, aDes1, 
        TDes16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Get( 
        const TInt aIpc,
        TDes8 & aDes1,
        TDes16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD4( TInt, const TInt, aIpc, 
        TDes8 &, aDes1, 
        TDes16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Get
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::Get( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        TDes8 & aDes1,
        TDes16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        TDes8 &, aDes1, 
        TDes16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::SetAndGet
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::SetAndGet( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC8 & aDes1,
        TDes8 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC8 &, aDes1, 
        TDes8 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::SetAndGet
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::SetAndGet( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        TDes8 & aDes1,
        const TDesC16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        TDes8 &, aDes1, 
        const TDesC16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::SetAndGet
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::SetAndGet( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC8 & aDummyValue,
        TDes16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC8&, aDummyValue, 
        TDes16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::SetAndGet
// -----------------------------------------------------------------------------
//
void RTelSubSessionBase::SetAndGet( 
        const TInt aIpc,
        TRequestStatus & aStatus,
        const TDesC16 & aDes1,
        TDes16 & aDes2,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD5( void, const TInt, aIpc, 
        TRequestStatus &, aStatus, 
        const TDesC16 &, aDes1, 
        TDes16 &, aDes2, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::Set
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::Set( 
        const TInt aIpc,
        const RFile & aFile,
        TReqPriorityType aType ) const
    {
    SMC_MOCK_METHOD3( TInt, const TInt, aIpc, 
        const RFile &, aFile, 
        TReqPriorityType, aType )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::CancelReq
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::CancelReq( 
        const TInt aIpc,
        const TInt aIpcToCancel ) const
    {
    SMC_MOCK_METHOD2( TInt, const TInt, aIpc, 
        const TInt, aIpcToCancel )
    }


// -----------------------------------------------------------------------------
// RTelSubSessionBase::CancelSubSession
// -----------------------------------------------------------------------------
//
TInt RTelSubSessionBase::CancelSubSession(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RTelServer::RTelServer
// -----------------------------------------------------------------------------
//
RTelServer::RTelServer(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RTelServer::Connect
// -----------------------------------------------------------------------------
//
TInt RTelServer::Connect( 
        TInt aMessageSlots )
    {
    SMC_MOCK_METHOD1( TInt, TInt, aMessageSlots )
    }


// -----------------------------------------------------------------------------
// RTelServer::EnumeratePhones
// -----------------------------------------------------------------------------
//
TInt RTelServer::EnumeratePhones( 
        TInt & aNoOfPhones ) const
    {
    SMC_MOCK_METHOD1( TInt, TInt &, aNoOfPhones )
    }


// -----------------------------------------------------------------------------
// RTelServer::GetPhoneInfo
// -----------------------------------------------------------------------------
//
TInt RTelServer::GetPhoneInfo( 
        const TInt aIndex,
        TPhoneInfo & aInfo ) const
    {
    SMC_MOCK_METHOD2( TInt, const TInt, aIndex, 
        TPhoneInfo &, aInfo )
    }


// -----------------------------------------------------------------------------
// RTelServer::GetTsyName
// -----------------------------------------------------------------------------
//
TInt RTelServer::GetTsyName( 
        const TInt aIndexOfPhone,
        TDes & aTsyName ) const
    {
    SMC_MOCK_METHOD2( TInt, const TInt, aIndexOfPhone, 
        TDes &, aTsyName )
    }


// -----------------------------------------------------------------------------
// RTelServer::LoadPhoneModule
// -----------------------------------------------------------------------------
//
TInt RTelServer::LoadPhoneModule( 
        const TDesC & aFileName ) const
    {
    SMC_MOCK_METHOD1( TInt, const TDesC &, aFileName )
    }


// -----------------------------------------------------------------------------
// RTelServer::UnloadPhoneModule
// -----------------------------------------------------------------------------
//
TInt RTelServer::UnloadPhoneModule( 
        const TDesC & aFileName ) const
    {
    SMC_MOCK_METHOD1( TInt, const TDesC &, aFileName )
    }


// -----------------------------------------------------------------------------
// RTelServer::IsSupportedByModule
// -----------------------------------------------------------------------------
//
TInt RTelServer::IsSupportedByModule( 
        const TDesC & aTsyName,
        const TInt aMixin,
        TBool & aResult ) const
    {
    SMC_MOCK_METHOD3( TInt, const TDesC &, aTsyName, 
        const TInt, aMixin, 
        TBool &, aResult )
    }


// -----------------------------------------------------------------------------
// RTelServer::GetTsyVersionNumber
// -----------------------------------------------------------------------------
//
TInt RTelServer::GetTsyVersionNumber( 
        const TDesC & aTsyName,
        TVersion & aVersion ) const
    {
    SMC_MOCK_METHOD2( TInt, const TDesC &, aTsyName, 
        TVersion &, aVersion )
    }


// -----------------------------------------------------------------------------
// RTelServer::SetPriorityClient
// -----------------------------------------------------------------------------
//
TInt RTelServer::SetPriorityClient(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RTelServer::SetExtendedErrorGranularity
// -----------------------------------------------------------------------------
//
TInt RTelServer::SetExtendedErrorGranularity( 
        const TErrorGranularity aGranularity ) const
    {
    SMC_MOCK_METHOD1( TInt, const TErrorGranularity, aGranularity )
    }


// -----------------------------------------------------------------------------
// RTelServer::__DbgMarkHeap
// -----------------------------------------------------------------------------
//
TInt RTelServer::__DbgMarkHeap(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RTelServer::__DbgCheckHeap
// -----------------------------------------------------------------------------
//
TInt RTelServer::__DbgCheckHeap( 
        TInt aCount )
    {
    SMC_MOCK_METHOD1( TInt, TInt, aCount )
    }


// -----------------------------------------------------------------------------
// RTelServer::__DbgMarkEnd
// -----------------------------------------------------------------------------
//
TInt RTelServer::__DbgMarkEnd( 
        TInt aCount )
    {
    SMC_MOCK_METHOD1( TInt, TInt, aCount )
    }


// -----------------------------------------------------------------------------
// RTelServer::__DbgFailNext
// -----------------------------------------------------------------------------
//
TInt RTelServer::__DbgFailNext( 
        TInt aCount )
    {
    SMC_MOCK_METHOD1( TInt, TInt, aCount )
    }


// -----------------------------------------------------------------------------
// RTelServer::__DbgFailNextAvailable
// -----------------------------------------------------------------------------
//
TInt RTelServer::__DbgFailNextAvailable( 
        TBool & aResult )
    {
    SMC_MOCK_METHOD1( TInt, TBool &, aResult )
    }


// -----------------------------------------------------------------------------
// RTelServer::SetPriorityClientV2
// -----------------------------------------------------------------------------
//
TInt RTelServer::SetPriorityClientV2(  ) const
    {
    SMC_MOCK_METHOD0( TInt )
    }


