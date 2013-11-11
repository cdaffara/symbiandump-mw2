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


#include <mtp/mtpprotocolconstants.h>
#include <mtp/tmtptyperequest.h>
#include <mtp/tmtptypeevent.h>
#include <mtp/mmtpconnection.h>
#include <mtp/mmtpobjectmgr.h>

#include "crequestprocessor.h"
#include "crequestchecker.h"
#include "mmmtpdputility.h"
#include "mmmtpdplogger.h"

static const TInt KNullBufferSize = 4096;

// -----------------------------------------------------------------------------
// CRequestProcessor::CRequestProcessor
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRequestProcessor::CRequestProcessor( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    TInt aElementCount,
    const TMTPRequestElementInfo* aElements ) :
        CActive( EPriorityStandard ),
        iFramework( aFramework ),
        iConnection( aConnection ),
        iElementCount( aElementCount ),
        iElements( aElements )
    {
    // Note: It has been moved to specific operation handler
    // Some operations don't need add into active scheduler
    // CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::~CRequestProcessor
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRequestProcessor::~CRequestProcessor()
    {
    // Note: It has been moved to specific operation handler
    // Some operations don't need add into active scheduler
    // Cancel();
    iNullBuffer.Close();
    delete iRequestChecker;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::Release
// Relese (delete) this request processor
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::Release()
    {
    delete this;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::SendResponseL
// Send a response to the initiator
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::SendResponseL( TMTPResponseCode aResponseCode,
    TInt aParameterCount,
    TUint32* aParams )
    {
    PRINT2( _L( "MM MTP <> CRequestProcessor::SendResponseL iTransactionCode = %d, aResponseCode = 0x%x" ),
        iTransactionCode,
        aResponseCode );
    __ASSERT_DEBUG( aParameterCount < TMTPTypeRequest::ENumElements, User::Invariant() );

    iResponse.SetUint16( TMTPTypeResponse::EResponseCode, aResponseCode );
    iResponse.SetUint32( TMTPTypeResponse::EResponseSessionID, iSessionId );
    iResponse.SetUint32( TMTPTypeResponse::EResponseTransactionID, iTransactionCode );

    TInt i = 0;
    for( i = 0; i < aParameterCount; i++ )
        {
        iResponse.SetUint32( TMTPTypeResponse::EResponseParameter1 + i, aParams[i] );
        }

    i += TMTPTypeResponse::EResponseParameter1;
    while( i < TMTPTypeResponse::EResponseParameter5 )
        {
        iResponse.SetUint32( i, 0 );
        i++;
        }

    __ASSERT_DEBUG( iRequest, User::Invariant() );
    iFramework.SendResponseL( iResponse, *iRequest, iConnection );
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::Request
// The current active request
// -----------------------------------------------------------------------------
//
EXPORT_C const TMTPTypeRequest& CRequestProcessor::Request() const
    {
    __ASSERT_DEBUG( iRequest, User::Invariant() );
    return *iRequest;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::Connection
// The connection from which the current request comes
// -----------------------------------------------------------------------------
//
EXPORT_C MMTPConnection& CRequestProcessor::Connection() const
    {
    return iConnection;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::UsbDisconnect
// Rollback when WMP closed and disconnect the USB cable,
// during the transferring file
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::UsbDisconnect()
    {
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::CompleteRequestL
// Signal to the framework that the current request transaction has completed
// -----------------------------------------------------------------------------
//
void CRequestProcessor::CompleteRequestL()
    {
    __ASSERT_DEBUG( iRequest, User::Invariant() );
    iFramework.TransactionCompleteL( *iRequest, iConnection );
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::SendDataL
// Send data to the initiator
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::SendDataL( const MMTPType& aData )
    {
    __ASSERT_DEBUG( iRequest, User::Invariant() );
    iFramework.SendDataL( aData, *iRequest, iConnection );
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::ReceiveDataL
// Receive data from the initiator
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::ReceiveDataL( MMTPType& aData )
    {
    __ASSERT_DEBUG( iRequest, User::Invariant() );
    iFramework.ReceiveDataL( aData, *iRequest, iConnection );
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::HandleRequestL
// Handle the request
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::HandleRequestL( const TMTPTypeRequest& aRequest,
    TMTPTransactionPhase aPhase )
    {
    iRequest = &aRequest;
    __ASSERT_DEBUG( iRequest, User::Invariant() );
    TBool result = EFalse;
    switch( aPhase )
        {
        case ERequestPhase:
            ExtractSessionTransactionId();
            result = DoHandleRequestPhaseL();
            break;

        case EDataIToRPhase:
            result = DoHandleDataIToRPhaseL();
            break;

        case EDataRToIPhase:
            result = DoHandleRToIPhaseL();
            break;

        case EResponsePhase:
            if ( iResponseCode != EMTPRespCodeOK && HasDataphase() )
                {
                SendResponseL( iResponseCode );
                iNullBuffer.Close();
                PRINT1( _L( "MM MTP <> CRequestProcessor::HandleRequestL SendResponse 0x%x" ), iResponseCode );
                }
            else
                {
                result = DoHandleResponsePhaseL();
                }
            break;

        case ECompletingPhase:
            MmMtpDpUtility::SetPSStatus(EMtpPSStatusReadyToSync);
            result = DoHandleCompletingPhaseL();
            break;

        default:
            PRINT( _L( "MM MTP <> CRequestProcessor::HandleRequestL default" ) );
            break;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::HandleEventL
// Handle the event
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::HandleEventL( const TMTPTypeEvent& aEvent )
    {
    TUint16 eventCode = aEvent.Uint16( TMTPTypeEvent::EEventCode );
    iCancelled = ( eventCode == EMTPEventCodeCancelTransaction );
    PRINT1( _L("CRequestProcessor::HandleEventL, eventCode = %d"), eventCode);
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::Match
// Check whether the processor can process the request
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::Match( const TMTPTypeRequest& aRequest,
    MMTPConnection& aConnection ) const
    {
    __ASSERT_DEBUG( iRequest, User::Invariant() );
    TBool result = ( ( &aRequest == iRequest ) && ( &iConnection == &aConnection ) );
    return result;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::Match
// Check whether the processor can process the event
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::Match( const TMTPTypeEvent& aEvent,
    MMTPConnection& aConnection ) const
    {
    TUint32 eventSessionId = aEvent.Uint32( TMTPTypeEvent::EEventSessionID );
    TUint32 eventTransactionCode = aEvent.Uint32( TMTPTypeEvent::EEventTransactionID );

    TBool result = EFalse;
    if ( ( iSessionId == eventSessionId )
        && ( iTransactionCode == eventTransactionCode )
        && ( &iConnection == &aConnection ) )
        {
        result = ETrue;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::DoHandleRequestPhaseL
// Handle the request phase of the current request
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::DoHandleRequestPhaseL()
    {
    TRAPD( err, iResponseCode = CheckRequestL() );
    PRINT2( _L( "MM MTP <> CRequestProcessor::DoHandleRequestPhaseL iResponseCode = 0x%x, CheckRequestL err = %d" ),
            iResponseCode, err );
    if ( ( err != KErrNone ) || ( iResponseCode != EMTPRespCodeOK ) )
        {
        if ( HasDataphase() )
            {
            // If we have a dataphase
            // we need to read in the data and discard it
            iNullBuffer.Close();
            iNullBuffer.CreateMaxL( KNullBufferSize );
            iNull.SetBuffer( iNullBuffer );
            ReceiveDataL( iNull );
            }
        else
            {
            if ( err != KErrNone )
                {
                User::Leave( err );
                }
            SendResponseL( iResponseCode );
            }
        }
    else
        {
        TRAP( err, ServiceL() );
        PRINT1( _L( "MM MTP <> CRequestProcessor::DoHandleRequestPhaseL ServiceL err = %d" ), err );
        if ( err != KErrNone )
            {
            iResponseCode = EMTPRespCodeGeneralError;
            if ( HasDataphase() )
                {
                // If we have a dataphase
                // we need to read in the data and discard it
                iNullBuffer.Close();
                iNullBuffer.CreateMaxL( KNullBufferSize );
                iNull.SetBuffer( iNullBuffer );
                ReceiveDataL( iNull );
                }
            else
                {
                User::Leave( err );
                }
            }
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::DoHandleDataIToRPhaseL
// Handle the receiving data phase of the current request
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::DoHandleDataIToRPhaseL()
    {
    User::Invariant();
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::DoHandleRToIPhaseL
// Handle the sending data phase of the current request
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::DoHandleRToIPhaseL()
     {
    User::Invariant();
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::DoHandleResponsePhaseL
// Handle the response phase of the current request
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::DoHandleResponsePhaseL()
    {
    TMTPResponseCode responseCode = ( iCancelled ? EMTPRespCodeIncompleteTransfer : EMTPRespCodeOK );
    SendResponseL( responseCode );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::DoHandleCompletingPhaseL
// Handle the completing phase of the current request
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRequestProcessor::DoHandleCompletingPhaseL()
    {
    CompleteRequestL();
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::CheckRequestL
// Check the current request
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CRequestProcessor::CheckRequestL()
    {
    if ( !iRequestChecker )
        {
        iRequestChecker = CRequestChecker::NewL( iFramework, iConnection );
        }
    __ASSERT_DEBUG( iRequest, User::Invariant() );
    return iRequestChecker->VerifyRequestL( *iRequest, iElementCount, iElements );
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::RunL
// part of active object framework, provide default implementation
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::RunL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::DoCancel
// part of active object framework, provide default implementation
// -----------------------------------------------------------------------------
//
EXPORT_C void CRequestProcessor::DoCancel()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::RunError
// part of active object framework, provide default implementation
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRequestProcessor::RunError( TInt aError )
    {
    if ( aError != KErrNone )
        PRINT1( _L( "MM MTP <> CRequestProcessor RunError = %d" ), aError );

    TRAP_IGNORE( SendResponseL( EMTPRespCodeGeneralError ) );

    return KErrNone;
    }

EXPORT_C TBool CRequestProcessor::HasDataphase() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CRequestProcessor::ExtractSessionTransactionId
// retrieve the session id and transaction code from the current request
// -----------------------------------------------------------------------------
//
void CRequestProcessor::ExtractSessionTransactionId()
    {
    iSessionId = iRequest->Uint32( TMTPTypeRequest::ERequestSessionID );
    iTransactionCode = iRequest->Uint32( TMTPTypeRequest::ERequestTransactionID );

    PRINT2( _L( "MM MTP <> CRequestProcessor iSessionId = %d, iTransactionCode = %d" ),
        iSessionId,
        iTransactionCode );
    }

EXPORT_C TUint32 CRequestProcessor::SessionId()
    {
    return iSessionId;
    }

// end of file
