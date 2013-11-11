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
* Description:  Implement the operation: SendObjectInfo/SendObjectPropList/SendObject
*
*/


#include <mtp/mmtpobjectmgr.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypeobjectinfo.h>
#include <mtp/cmtptypefile.h>
#include <mtp/mmtpstoragemgr.h>
#include <bautils.h>
#include <mtp/cmtptypeobjectproplist.h>

#include "csendobject.h"
#include "mmmtpdpconfig.h"
#include "mmmtpdputility.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdplogger.h"
#include "cpropertysettingutility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"

const TInt KMtpRollbackFuncCnt = 3;

// Verification data for the SendObjectInfo request
const TMTPRequestElementInfo KMTPSendObjectInfoPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeStorageId,
        EMTPElementAttrWrite,
        1,
        0,
        0
        },

        {
        TMTPTypeRequest::ERequestParameter2,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrDir | EMTPElementAttrWrite,
        2,
        KMTPHandleAll,
        KMTPHandleNone
        }
    };

// -----------------------------------------------------------------------------
// CSendObject::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CSendObject::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CSendObject* self = new ( ELeave ) CSendObject( aFramework, aConnection, aDpConfig );

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CSendObject::~CSendObject
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSendObject::~CSendObject()
    {
    Rollback();
    iRollbackList.Close();

    delete iFileReceived;
    delete iParentSuid;
    delete iObjectInfo;
    delete iObjectPropList;
    delete iDateMod;
    delete iReceivedObjectInfo;

    PRINT( _L( "MM MTP <= CSendObject::~CSendObject" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::CSendObject
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CSendObject::CSendObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework, aConnection, 0, NULL ),
        iFs( iFramework.Fs() ),
        iObjectMgr( iFramework.ObjectMgr() ),
        iDpConfig( aDpConfig ),
        iRollbackList( KMtpRollbackFuncCnt )
    {
    PRINT( _L( "Operation: SendObjectInfo/SendObject/SendObjectPropList(0x100C/0x100D/0x9808)" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::ConstructL
// 2nd Phase Constructor
// -----------------------------------------------------------------------------
//
void CSendObject::ConstructL()
    {
    PRINT( _L( "MM MTP => CSendObject::ConstructL" ) );

    iExpectedSendObjectRequest.SetUint16( TMTPTypeRequest::ERequestOperationCode,
        EMTPOpCodeSendObject );

    iReceivedObjectInfo = CMTPObjectMetaData::NewL();
    iReceivedObjectInfo->SetUint( CMTPObjectMetaData::EDataProviderId,
        iFramework.DataProviderId() );

    PRINT1( _L( "MM MTP <> CSendObject::ConstructL DataProviderId = 0x%x" ), iFramework.DataProviderId());

    PRINT( _L( "MM MTP <= CSendObject::ConstructL" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::Match
// Override to match both the SendObjectInfo and SendObject requests
// @param aRequest    The request to match
// @param aConnection The connection from which the request comes
// @return ETrue if the processor can handle the request, otherwise EFalse
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendObject::Match( const TMTPTypeRequest& aRequest,
    MMTPConnection& aConnection ) const
    {
    TBool result = EFalse;

    TUint16 operationCode = aRequest.Uint16( TMTPTypeRequest::ERequestOperationCode );
    if ( ( operationCode == EMTPOpCodeSendObjectInfo
            || operationCode == EMTPOpCodeSendObject
            || operationCode == EMTPOpCodeSendObjectPropList )
            && ( &iConnection == &aConnection ) )
        {
        result = ETrue;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CSendObject::CheckSendingState
// Helper Functions
// Verify if the SendObject request comes after SendObjectInfo request
// @return EMTPRespCodeOK if SendObject request comes after a valid SendObjectInfo request, otherwise
// EMTPRespCodeNoValidObjectInfo
// -----------------------------------------------------------------------------
//
TMTPResponseCode CSendObject::CheckSendingStateL()
    {
    PRINT( _L( "MM MTP => CSendObject::CheckSendingState" ) );

    TMTPResponseCode result = EMTPRespCodeOK;
    iOperationCode = Request().Uint16( TMTPTypeRequest::ERequestOperationCode );
    PRINT1( _L( "MM MTP <> CSendObject iOperationCode = 0x%x" ), iOperationCode );

    if ( iOperationCode == EMTPOpCodeSendObject )
        {
        // In ParseRouter everytime SendObject gets resolved then will be removed from Registry
        // right away therefore we need reRegister it here again in case possible cancelRequest
        // against this SendObject being raised.
        iExpectedSendObjectRequest.SetUint32( TMTPTypeRequest::ERequestSessionID,
            iSessionId );
        iFramework.RouteRequestRegisterL( iExpectedSendObjectRequest,
            iConnection );
        }

    if ( iProgress == EObjectNone )
        {
        if ( iOperationCode == EMTPOpCodeSendObject )
            {
            PRINT( _L( "MM MTP <> CSendObject::CheckSendingState  EMTPRespCodeNoValidObjectInfo" ) );
            result = EMTPRespCodeNoValidObjectInfo;
            }
        }
    else if ( iProgress == EObjectInfoSucceed )
        {
        if ( iOperationCode == EMTPOpCodeSendObjectInfo )
            {
            delete iObjectInfo;
            iObjectInfo = NULL;
            Rollback();
            iProgress = EObjectNone;
            }
        else if ( iOperationCode == EMTPOpCodeSendObjectPropList )
            {
            delete iObjectPropList;
            iObjectPropList = NULL;
            Rollback();
            iProgress = EObjectNone;
            }
        }
    else
        {
        Panic( EMmMTPDpSendObjectStateInvalid );
        }

    PRINT1( _L( "MM MTP <= CSendObject::CheckSendingState result = 0x%x" ), result );

    return result;
    }

// -----------------------------------------------------------------------------
// CSendObject::CheckRequestL
// Verify the reqeust
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CSendObject::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CSendObject::CheckRequestL" ) );

    TMTPResponseCode responseCode = CheckSendingStateL();

    if ( responseCode != EMTPRespCodeOK )
        {
        return responseCode;
        }

    if ( iProgress == EObjectNone )
    // Only SendObjectInfo/SendObjectPropList's request phase will enter into this function,
    // otherwise, state machine of fw should be wrong.
        {
        iElementCount = sizeof( KMTPSendObjectInfoPolicy ) / sizeof( TMTPRequestElementInfo );
        iElements = KMTPSendObjectInfoPolicy;
        }
    else if ( iProgress == EObjectInfoSucceed )
        {
        iElementCount = 0;
        iElements = NULL;
        }

    if ( iElements != NULL )
        {
        responseCode = CRequestProcessor::CheckRequestL();
        if ( responseCode != EMTPRespCodeOK )
            {
            return responseCode;
            }

        // Reserve storageId and parent into member data variables if they are matched.
        responseCode = MatchStoreAndParentL();
        if ( responseCode != EMTPRespCodeOK )
            {
            return responseCode;
            }

        if ( iOperationCode == EMTPOpCodeSendObjectPropList )
            {
            // check if it is what dp supported
            iObjectFormat = Request().Uint32( TMTPTypeRequest::ERequestParameter3 );
            if ( iObjectFormat != KMTPFormatsAll )
                {
                responseCode = EMTPRespCodeInvalidObjectFormatCode;

                const RArray<TUint>* format = iDpConfig.GetSupportedFormat();
                TInt count = format->Count();

                for ( TInt i = 0; i < count; i++ )
                    {
                    if ( iObjectFormat == ( *format )[i] )
                        {
                        responseCode = EMTPRespCodeOK;
                        break;
                        }
                    }
                if ( responseCode != EMTPRespCodeOK )
                    {
                    return responseCode;
                    }
                } // end of if ( iObjectFormat != KMTPFormatsAll )

            // check object size
            TUint32 objectSizeHigh = Request().Uint32( TMTPTypeRequest::ERequestParameter4 );
            TUint32 objectSizeLow = Request().Uint32( TMTPTypeRequest::ERequestParameter5 );
            iObjectSize = MAKE_TUINT64( objectSizeHigh, objectSizeLow );
            PRINT1( _L( "MM MTP <> CSendObject::CheckRequestL iObjectSize = %Lu" ), iObjectSize );

            if ( IsTooLarge( iObjectSize ) )
                {
                responseCode = EMTPRespCodeObjectTooLarge;
                }
            }
        }

    PRINT1( _L( "MM MTP <= CSendObject::CheckRequestL, responseCode = 0x%x" ), responseCode );
    return responseCode;
    }

// -----------------------------------------------------------------------------
// CSendObject::HasDataphase
// Exception handling. CRequestProcessor will receive data if this operation
// won't by return ETrue.
// @return ETrue if there is data need to be received from initiator
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendObject::HasDataphase() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSendObject::ServiceL
// ServiceL request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendObject::ServiceL()
    {
    PRINT( _L( "MM MTP => CSendObject::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);

    if ( iProgress == EObjectNone )
        {
        // iCancelled could have been set after the last transaction
        // reset the flag here
        iCancelled = EFalse;
        
        if ( iOperationCode == EMTPOpCodeSendObjectInfo )
            {
            ServiceInfoL();
            }
        else
            {
            ServicePropListL();
            }
        }
    else
        {
        PRINT1( _L( "MM MTP <> CSendObject::ServiceL, iProgress = %d" ), iProgress );
        ServiceObjectL();
        }
    PRINT( _L( "MM MTP <= CSendObject::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::ServiceInfoL
// ServiceL - Recieves the objectinfo data
// -----------------------------------------------------------------------------
//
void CSendObject::ServiceInfoL()
    {
    PRINT( _L( "MM MTP => CSendObject::ServiceInfoL" ) );

    iObjectInfo = CMTPTypeObjectInfo::NewL();
    ReceiveDataL( *iObjectInfo );
    iProgress = EObjectInfoInProgress;

    PRINT( _L( "MM MTP <= CSendObject::ServiceInfoL" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::ServicePropListL
// SendObjectPropList
// -----------------------------------------------------------------------------
//
void CSendObject::ServicePropListL()
    {
    PRINT( _L( "MM MTP => CSendObject::ServicePropListL" ) );

    iObjectPropList = CMTPTypeObjectPropList::NewL();
    ReceiveDataL( *iObjectPropList );
    iProgress = EObjectInfoInProgress;

    PRINT( _L( "MM MTP <= CSendObject::ServicePropListL" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::ServiceObjectL
// SendObject
// -----------------------------------------------------------------------------
//
void CSendObject::ServiceObjectL()
    {
    PRINT( _L( "MM MTP => CSendObject::ServiceObjectL" ) );
    ReceiveDataL( *iFileReceived );
    iProgress = ESendObjectInProgress;

    PRINT( _L( "MM MTP <= CSendObject::ServiceObjectL" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::DoHandleResponsePhaseL
// Response Phase Handler
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendObject::DoHandleResponsePhaseL()
    {
    PRINT( _L( "MM MTP => CSendObject::DoHandleResponsePhaseL" ) );

    // check if the sending/receiving data is successful
    TBool successful = !iCancelled;
    if ( iProgress == EObjectInfoInProgress )
        {
        if ( iOperationCode == EMTPOpCodeSendObjectInfo )
            {
            successful = DoHandleResponsePhaseInfoL();
            }
        else if ( iOperationCode == EMTPOpCodeSendObjectPropList )
            {
            successful = DoHandleResponsePhasePropListL();
            }
        iProgress = ( successful ? EObjectInfoSucceed : EObjectInfoFail );
        iPreviousOperation = iOperationCode;
        }
    else if ( iProgress == ESendObjectInProgress )
        {
        successful = DoHandleResponsePhaseObjectL();
        iProgress = ( successful ? ESendObjectSucceed : ESendObjectFail );
        }
    PRINT1( _L( "MM MTP <= CSendObject::DoHandleResponsePhaseL iProgress = %d" ), iProgress );

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSendObject::DoHandleResponsePhaseInfoL
// Handle Response - Checks whether the request was successful
// -----------------------------------------------------------------------------
//
TBool CSendObject::DoHandleResponsePhaseInfoL()
    {
    PRINT( _L( "MM MTP => CSendObject::DoHandleResponsePhaseInfoL" ) );

    TBool result = ETrue;

    // cache the width and height of video file
    iObjectFormat = iObjectInfo->Uint16L( CMTPTypeObjectInfo::EObjectFormat );
    iWidth = iObjectInfo->Uint32L( CMTPTypeObjectInfo::EImagePixWidth );
    iHeight = iObjectInfo->Uint32L( CMTPTypeObjectInfo::EImagePixWidth );
    PRINT3( _L("MM MTP <> CSendObject::DoHandleResponsePhaseInfoL iObjectFormat = 0x%x, iWidth = %d, iHeight = %d"),
        iObjectFormat,
        iWidth,
        iHeight );

    delete iDateMod;
    iDateMod = NULL;
    iDateMod = iObjectInfo->StringCharsL( CMTPTypeObjectInfo::EDateModified ).AllocL();

    // check if storage is full
    iObjectSize = iObjectInfo->Uint32L( CMTPTypeObjectInfo::EObjectCompressedSize );
    PRINT1( _L("MM MTP <> CSendObject::DoHandleResponsePhaseInfoL, iObjectSize = %Lu"), iObjectSize );

    if ( IsTooLarge( iObjectSize ) )
        {
        SendResponseL( EMTPRespCodeObjectTooLarge );
        Rollback();
        result = EFalse;
        }

    if ( result )
        {
        iProtectionStatus = iObjectInfo->Uint16L( CMTPTypeObjectInfo::EProtectionStatus );
        PRINT1( _L( "MM MTP <> CSendObject::DoHandleResponsePhaseInfoL iProtectionStatus = %d" ), iProtectionStatus );
        if ( iProtectionStatus != EMTPProtectionNoProtection
            && iProtectionStatus != EMTPProtectionReadOnly )
            {
            SendResponseL( EMTPRespCodeParameterNotSupported );
            Rollback();
            result = EFalse;
            }
        }

    if ( result )
        {
        result = GetFullPathNameL( iObjectInfo->StringCharsL( CMTPTypeObjectInfo::EFilename ) );
        if ( !result )
            {
            // File and/or parent pathname invalid.
            SendResponseL( EMTPRespCodeInvalidDataset );
            Rollback();
            }
        }

    if ( result )
        {
        if ( ExistsL( iFullPath ) )
            {
            SendResponseL( EMTPRespCodeAccessDenied );
            Rollback();
            result = EFalse;
            }
        }

    if ( result )
        {
        if ( KErrNone != ReserveObjectL() )
            {
            result = EFalse;
            }
        }
    PRINT1( _L( "MM MTP <= CSendObject::DoHandleResponsePhaseInfoL result = %d" ), result );

    return result;
    }

// -----------------------------------------------------------------------------
// CSendObject::DoHandleResponsePhasePropListL
// SendObjectPropList
// -----------------------------------------------------------------------------
//
TBool CSendObject::DoHandleResponsePhasePropListL()
    {
    PRINT( _L("MM MTP => CSendObject::DoHandleResponsePhasePropListL" ) );

    TMTPResponseCode responseCode = EMTPRespCodeOK;

    TInt invalidParameterIndex = KErrNotFound;
    responseCode = VerifyObjectPropListL( invalidParameterIndex );
    TInt err = KErrNone;

    if ( responseCode != EMTPRespCodeOK )
        {
        TUint32 parameters[4];
        parameters[0] = 0;
        parameters[1] = 0;
        parameters[2] = 0;
        parameters[3] = invalidParameterIndex;
        SendResponseL( responseCode, 4, parameters );
        Rollback();
        }
    else if ( ExistsL( iFullPath ) )
        {
        SendResponseL( EMTPRespCodeAccessDenied );
        Rollback();
        }
    else
        err = ReserveObjectL();

    PRINT( _L( "MM MTP <= CSendObject::DoHandleResponsePhasePropListL" ) );
    return ( responseCode == EMTPRespCodeOK && err == KErrNone);
    }

// -----------------------------------------------------------------------------
// CSendObject::DoHandleResponsePhaseObjectL
// SendObject
// -----------------------------------------------------------------------------
//
TBool CSendObject::DoHandleResponsePhaseObjectL()
    {
    PRINT( _L( "MM MTP => CSendObject::DoHandleResponsePhaseObjectL" ) );

    TBool result = ETrue;

    TEntry fileEntry;
    User::LeaveIfError( iFs.Entry( iFullPath, fileEntry ) );
    if ( fileEntry.FileSize() != iObjectSize )
        {
        iFs.Delete( iFullPath );
        iObjectMgr.UnreserveObjectHandleL( *iReceivedObjectInfo );
        TMTPResponseCode responseCode = EMTPRespCodeObjectTooLarge;
        if ( fileEntry.FileSize() < iObjectSize )
            {
            responseCode = EMTPRespCodeIncompleteTransfer;
            }
        SendResponseL( responseCode );
        Rollback();
        result = EFalse;
        }

    // SendObject is cancelled or connection is dropped.
    if ( result && iCancelled )
        {
        iFramework.RouteRequestUnregisterL( iExpectedSendObjectRequest,
            iConnection );
        SendResponseL( EMTPRespCodeTransactionCancelled );
        Rollback();
        }
    else if ( result && !iCancelled )
        {
        if ( iObjectSize > 0 ) // media file
            {
            TRAPD( err, AddMediaToStoreL() );
            PRINT1( _L( "MM MTP <= CSendObject::DoHandleResponsePhaseObjectL err = %d" ), err );

            if ( ( iPreviousOperation == EMTPOpCodeSendObjectPropList )
                && ( err == KErrNone ) )
                {
                // Only leave when getting proplist element from data received by fw.
                // It should not happen after ReceiveDataL in which construction of proplist already succeed.
                SetObjectPropListL();
                }

            // Commits into MTP data object enumeration store the object handle and
            // storage space previously reserved for the specified object.
            iFramework.ObjectMgr().CommitReservedObjectHandleL( *iReceivedObjectInfo );
            iRollbackList.Append( &CSendObject::RemoveObjectFromDbL );
            }

        // Commit object to MTP data store
        iFramework.RouteRequestUnregisterL( iExpectedSendObjectRequest,
            iConnection );

        SendResponseL( EMTPRespCodeOK );
        }

    iCancelled = EFalse;

    PRINT1( _L( "MM MTP <= CSendObject::DoHandleResponsePhaseObjectL result = %d" ), result );

    return result;
    }

// -----------------------------------------------------------------------------
// CSendObject::DoHandleCompletingPhaseL
// Completeing phase Handler
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendObject::DoHandleCompletingPhaseL()
    {
    TBool result = ETrue;

    PRINT( _L( "MM MTP => CSendObject::DoHandleCompletingPhaseL" ) );

    CRequestProcessor::DoHandleCompletingPhaseL();
    //Ensure that, even though the SendObjectInfo was successul, the request responder is not deleted
    if ( iProgress == EObjectInfoSucceed )
        {
        result = EFalse;
        }
    else if ( iProgress == ESendObjectFail )
        {
        //Sending Object failed, but still do not delete request, can try again with current info
        iProgress = EObjectInfoSucceed;
        result = EFalse;
        }

    if ( result )
        iRollbackList.Reset();

    PRINT2( _L( "MM MTP <= CSendObject::DoHandleCompletingPhaseL iProgress= %d, result = %d" ),
        iProgress,
        result );

    return result;
    }

// -----------------------------------------------------------------------------
// Check if the property list is valid and extract necessary properties
// @param aInvalidParameterIndex if invalid, contains the index of the property.
//        Undefined, if it is valid.
// @return if error, one of the error response code; otherwise EMTPRespCodeOK
// -----------------------------------------------------------------------------
TMTPResponseCode CSendObject::VerifyObjectPropListL( TInt& aInvalidParameterIndex )
    {
    PRINT( _L( "MM MTP => CSendObject::VerifyObjectPropListL" ) );

    TMTPResponseCode responseCode( EMTPRespCodeOK );
    const TInt count = iObjectPropList->NumberOfElements();
    iObjectPropList->ResetCursor();
    for ( TInt i = 0; i < count; i++ )
        {
        const CMTPTypeObjectPropListElement& element( iObjectPropList->GetNextElementL() );
        const TUint32 handle( element.Uint32L( CMTPTypeObjectPropListElement::EObjectHandle ) );
        aInvalidParameterIndex = i;
        if ( handle != KMTPHandleNone )
            {
            responseCode = EMTPRespCodeInvalidObjectHandle;
            break;
            }

        responseCode = CheckPropCodeL( element );
        if ( responseCode != EMTPRespCodeOK )
            {
            break;
            }
        responseCode = ExtractPropertyL( element );
        if ( responseCode != EMTPRespCodeOK )
            {
            break;
            }
        }

    PRINT1( _L( "MM MTP <= CSendObject::VerifyObjectPropListL, responseCode = 0x%x" ), responseCode );
    return responseCode;
    }

// -----------------------------------------------------------------------------
// Validates the data type for a given property code.
// @param aElement an object property list element
// @param aPropertyCode MTP property code for the element
// @return EMTPRespCodeOK if the combination is valid, or another MTP response code if not
// -----------------------------------------------------------------------------
TMTPResponseCode CSendObject::CheckPropCodeL( const CMTPTypeObjectPropListElement& aElement )
    {
    PRINT( _L( "MM MTP => CSendObject::CheckPropCodeL" ) );
    TMTPResponseCode responseCode( EMTPRespCodeOK );

    // Checking if the propCode is supported first then check its type
    const RArray<TUint>* properties = iDpConfig.GetSupportedPropertiesL( iObjectFormat );
    TUint16 propCode = aElement.Uint16L( CMTPTypeObjectPropListElement::EPropertyCode );
    TUint16 dataType = aElement.Uint16L( CMTPTypeObjectPropListElement::EDatatype );
    PRINT2( _L( "MM MTP => CSendObject::CheckPropCodeL propCode = 0x%x, dataType = 0x%x" ), propCode, dataType );

    responseCode = EMTPRespCodeInvalidObjectPropCode;
    const TInt count = properties->Count();
    for ( TInt i = 0; i < count; i++ )
        {
        if ( ( *properties )[i] == propCode )
            {
            responseCode = EMTPRespCodeOK;
            break;
            }
        }

    if ( responseCode != EMTPRespCodeOK )
        return responseCode;

    // TODO: abstractmedia and media dp have different supported propCode, need check common prop code and check others in dp derived processors.
    // also need to check if the propCode is supported
    switch ( propCode )
        {
        case EMTPObjectPropCodeStorageID:
            if ( dataType != EMTPTypeUINT32 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            else if ( iStorageId != aElement.Uint32L( CMTPTypeObjectPropListElement::EValue ) )
                {
                responseCode = EMTPRespCodeInvalidDataset;
                }
            break;

        case EMTPObjectPropCodeObjectFormat:
            if ( dataType != EMTPTypeUINT16 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            else if ( iObjectFormat != aElement.Uint16L( CMTPTypeObjectPropListElement::EValue ) )
                {
                responseCode = EMTPRespCodeInvalidDataset;
                }
            break;

        case EMTPObjectPropCodeObjectSize:
            if ( dataType != EMTPTypeUINT64 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            else if ( iObjectSize != aElement.Uint64L( CMTPTypeObjectPropListElement::EValue ) )
                {
                responseCode = EMTPRespCodeInvalidDataset;
                }
            PRINT1(_L("MM MTP => CSendObject::CheckPropCodeL Checking ObjectSize %d"), responseCode );
            break;

        case EMTPObjectPropCodeParentObject:
            if ( dataType != EMTPTypeUINT32 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            else if ( ( iParentHandle != aElement.Uint32L( CMTPTypeObjectPropListElement::EValue ) )
                || ( KMTPHandleNone != aElement.Uint32L( CMTPTypeObjectPropListElement::EValue ) ) )
                // iParentHandle might be changed in CheckRequestL
                {
                responseCode = EMTPRespCodeInvalidDataset;
                }
            break;

        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:    // read-only
            if ( dataType != EMTPTypeUINT128 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            break;

        case EMTPObjectPropCodeProtectionStatus:
            if ( dataType != EMTPTypeUINT16 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            break;

        case EMTPObjectPropCodeDateCreated:
        case EMTPObjectPropCodeDateModified:
        case EMTPObjectPropCodeObjectFileName:
        case EMTPObjectPropCodeName:
            if ( dataType != EMTPTypeString )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            break;

        case EMTPObjectPropCodeNonConsumable:
            if ( dataType != EMTPTypeUINT8 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            break;

        default:
            // check types of DP specific properties
            // TODO: Is there anything except datatype need to be checked?
            responseCode = MmMtpDpUtility::CheckPropType( propCode, dataType );
            if ( responseCode == EMTPRespCodeAccessDenied )
                {
                responseCode = EMTPRespCodeOK;
                }
            break;
        }

    PRINT1( _L( "MM MTP <= CSendObject::CheckPropCode, responseCode = 0x%x" ), responseCode );
    return responseCode;
    }

// -----------------------------------------------------------------------------
// Extracts the file information from the object property list element
// @param aElement an object property list element
// @param aPropertyCode MTP property code for the element
// @return MTP response code
// -----------------------------------------------------------------------------
TMTPResponseCode CSendObject::ExtractPropertyL( const CMTPTypeObjectPropListElement& aElement )
    {
    PRINT( _L ( "MM MTP => CSendObject::ExtractPropertyL" ) );
    TMTPResponseCode responseCode( EMTPRespCodeOK );
    switch ( aElement.Uint16L( CMTPTypeObjectPropListElement::EPropertyCode ) )
        {
        case EMTPObjectPropCodeObjectFileName:
            {
            const TDesC& fileName = aElement.StringL( CMTPTypeObjectPropListElement::EValue );
            if ( !GetFullPathNameL( fileName ) )
                {
                responseCode = EMTPRespCodeInvalidDataset;
                }
            }
            break;

        case EMTPObjectPropCodeProtectionStatus:
            {
            iProtectionStatus = aElement.Uint16L( CMTPTypeObjectPropListElement::EValue );
            if ( iProtectionStatus != EMTPProtectionNoProtection
                && iProtectionStatus != EMTPProtectionReadOnly )
                {
                responseCode = EMTPRespCodeParameterNotSupported;
                }
            }
            break;

        case EMTPObjectPropCodeDateModified:
            delete iDateMod;
            iDateMod = NULL;
            iDateMod = aElement.StringL( CMTPTypeObjectPropListElement::EValue ).AllocL();
            // Cache it for further usage.
            break;

        default:
            // Only extract necessary properties which conform to SendObjectInfo.
            break;
        }

    PRINT1( _L( "MM MTP <= CSendObject::ExtractPropertyL, responseCode = 0x%x" ), responseCode );
    return responseCode;
    }

// -----------------------------------------------------------------------------
// CSendObject::SetObjectPropListL
// Reserve object proplist into database
// -----------------------------------------------------------------------------
//
TMTPResponseCode CSendObject::SetObjectPropListL()
    {
    PRINT( _L( "MM MTP => CSendObject::SetObjectPropListL" ) );

    TMTPResponseCode responseCode( EMTPRespCodeOK );

    const TUint count( iObjectPropList->NumberOfElements() );
    iObjectPropList->ResetCursor();
    for ( TInt i = 0; i < count; i++ )
        {
        const CMTPTypeObjectPropListElement& element( iObjectPropList->GetNextElementL() );

        TUint16 propertyCode = element.Uint16L( CMTPTypeObjectPropListElement::EPropertyCode );
        TUint16 dataType = element.Uint16L( CMTPTypeObjectPropListElement::EDatatype );
        PRINT2( _L( "MM MTP <> SetObjectPropListL propertyCode = 0x%x, dataType = 0x%x" ),
            propertyCode,
            dataType );

        switch ( propertyCode )
            {
            case EMTPObjectPropCodeStorageID:
            case EMTPObjectPropCodeObjectFormat:
            case EMTPObjectPropCodeObjectSize:
            case EMTPObjectPropCodeParentObject:
            case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
                // Do nothing for those properties are already set.
                break;

            case EMTPObjectPropCodeNonConsumable:
                iReceivedObjectInfo->SetUint( CMTPObjectMetaData::ENonConsumable,
                    element.Uint8L( CMTPTypeObjectPropListElement::EValue ) );
                break;

            case EMTPObjectPropCodeDateAdded:
            case EMTPObjectPropCodeDateCreated:
            case EMTPObjectPropCodeObjectFileName:
                // Do nothing for read-only properties
                /* spec:
                 * Object properties that are get-only (0x00 GET)
                 * should accept values during object creation by
                 * way of the SendObjectPropList command.
                 */
                break;

            case EMTPObjectPropCodeDateModified:
            case EMTPObjectPropCodeProtectionStatus:
                // Already done in AddMediaToStore, it's not necessary to set it again.
                break;

            case EMTPObjectPropCodeName:
            case EMTPObjectPropCodeAlbumArtist:
                {
                CMTPTypeString* stringData = CMTPTypeString::NewLC( element.StringL( CMTPTypeObjectPropListElement::EValue ) );// + stringData

                responseCode = iDpConfig.PropSettingUtility()->SetMetaDataToWrapper( iDpConfig,
                    propertyCode,
                    *stringData,
                    *iReceivedObjectInfo );

                CleanupStack::PopAndDestroy( stringData );// - stringData
                }
                break;

            default:
                {
                responseCode = iDpConfig.PropSettingUtility()->SetSpecificObjectPropertyL( iDpConfig,
                    propertyCode,
                    *iReceivedObjectInfo,
                    element );
                }
                break;
            } // end of switch
        } // end of for

    PRINT1( _L( "MM MTP <= CSendObject::SetObjectPropListL responseCode = 0x%x" ), responseCode );
    return responseCode;
    }

// -----------------------------------------------------------------------------
// CSendObject::MatchStoreAndParentL
// -----------------------------------------------------------------------------
//
TMTPResponseCode CSendObject::MatchStoreAndParentL()
    {
    TMTPResponseCode responseCode = EMTPRespCodeOK;

    iStorageId = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    iParentHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    PRINT2( _L( "MM MTP <> CSendObject::MatchStoreAndParentL, iStorageId = 0x%x, iParentHandle = 0x%x" ),
        iStorageId,
        iParentHandle );

    if ( iStorageId == KMTPStorageDefault )
        {
        iStorageId = iDpConfig.GetDefaultStorageIdL();
        PRINT1( _L( "MM MTP <> CSendObject::GetDefaultStorageIdL, iStorageId = 0x%x" ), iStorageId );
        }

    delete iParentSuid;
    iParentSuid = NULL;

    if( iParentHandle == KMTPHandleNone )   // parentHandle is not used by initiator, set it to root
        {
        iParentHandle = KMTPHandleAll;
        }
    if ( iParentHandle == KMTPHandleAll )   // According to spec, KMTPHandleAll means initiator wish to place in the root
        {
        PRINT( _L( "MM MTP <> CSendObject::MatchStoreAndParentL, iParentSuid = KMTPHandleAll" ) );
        iParentSuid = iFramework.StorageMgr().StorageL( iStorageId ).DesC( CMTPStorageMetaData::EStorageSuid ).AllocL();
        PRINT1( _L( "MM MTP <> CSendObject::MatchStoreAndParentL, iParentSuid = %S" ), iParentSuid );
        }
    else    // parentHandle is specified by initiator
        {
        // does not take owernship
        CMTPObjectMetaData* parentObjInfo = iRequestChecker->GetObjectInfo( iParentHandle );
        __ASSERT_DEBUG( parentObjInfo, Panic( EMmMTPDpObjectNull ) );

        if ( parentObjInfo->Uint( CMTPObjectMetaData::EStorageId ) != iStorageId )
            {
            responseCode = EMTPRespCodeInvalidObjectHandle;
            PRINT( _L( "MM MTP <> CSendObject::MatchStoreAndParentL, STORAGEID DOES NOT MATCH WITH PARENTHANDLE!" ) );
            }
        else
            {
            iParentSuid = parentObjInfo->DesC( CMTPObjectMetaData::ESuid ).AllocL();
            }
        }
    PRINT1( _L( "MM MTP <> CSendObject::MatchStoreAndParentL, iParentSuid = %S" ), iParentSuid );

    if ( ( responseCode == EMTPRespCodeOK ) && !BaflUtils::PathExists( iFramework.Fs(), *iParentSuid ) )
        {
        responseCode = EMTPRespCodeInvalidDataset;
        }

    return responseCode;
    }

// -----------------------------------------------------------------------------
// CSendObject::IsTooLarge
// Check if the object is too large
// @return ETrue if yes, otherwise EFalse
// -----------------------------------------------------------------------------
//
TBool CSendObject::IsTooLarge( TUint64 aObjectSize ) const
    {
    const TUint64 KMaxSupportedFileSize = 0xFFFFFFFF; //Maximal file size supported (4GB-1)
    TBool ret = ( aObjectSize > KMaxSupportedFileSize );
    PRINT2( _L( "MM MTP <> CSendObject::IsTooLarge aObjectSize = 0x%Lx, ret = %d" ), aObjectSize, ret );
    return ret;
    }


// -----------------------------------------------------------------------------
// CSendObject::GetFullPathNameL
// Get the full path name of the object to be saved
// @param aFileName, on entry, contains the file name of the object,
// on return, contains the full path name of the object to be saved
// -----------------------------------------------------------------------------
//
TBool CSendObject::GetFullPathNameL( const TDesC& aFileName )
    {
    PRINT1( _L("MM MTP => CSendObject::GetFullPathNameL aFileName = %S"), &aFileName );

    TBool result( EFalse );

    TParsePtrC parser( aFileName );
    if ( aFileName.Length() > 0 )
        {
        iFullPath.Zero();
        iFullPath.Append( *iParentSuid );

        // TODO: need to be done in derived class
        // Only add extension for alb to pass winlogo test cases
        TInt length = iFullPath.Length() + aFileName.Length();

        TBool isAlbWithoutExt =
            ( ( iObjectFormat == EMTPFormatCodeAbstractAudioAlbum ) && ( !parser.ExtPresent() ) );
        if ( isAlbWithoutExt )
            length += KTxtExtensionALB().Length();

        if ( length < KMaxFileName )
            {
            iFullPath.Append( aFileName );
            if ( isAlbWithoutExt )
                iFullPath.Append( KTxtExtensionALB );
            PRINT1( _L( "MM MTP <> CSendObject::GetFullPathNameL iFullPath = %S" ), &iFullPath );
            result = iFramework.Fs().IsValidName( iFullPath );
            }
        }

    if ( result && ( iObjectFormat != MmMtpDpUtility::FormatFromFilename( iFullPath ) ) )
        {
        if ( ( iObjectFormat == EMTPFormatCode3GPContainer ) && ( parser.Ext().CompareF( KTxtExtensionODF ) == 0))
            {
            PRINT( _L( "MM MTP <> might happen if function is called before physical file arrives" ) );
            // might happen if function is called before physical file arrives
            // do nothing
            }
        else if ( iObjectFormat == EMTPFormatCodeASF )
            {
            // happens on some buggy PC implementation, default formatcode to WMA or WMV if extension matches
            PRINT( _L( "MM MTP <> happens on some buggy PC implementation, default formatcode to WMA or WMV if extension matches" ) );
            if ( parser.Ext().CompareF( KTxtExtensionWMA ) == 0 )
                iObjectFormat = EMTPFormatCodeWMA;
            else if ( parser.Ext().CompareF( KTxtExtensionWMV ) == 0 )
                iObjectFormat = EMTPFormatCodeWMV;
            else
                result = EFalse;
            }
        else
            {
            PRINT2( _L( "MM MTP <> %S does not match 0x%x" ), &iFullPath, iObjectFormat );
            result = EFalse;
            }
        }

    PRINT1( _L( "MM MTP <= CSendObject::GetFullPathNameL result = %d" ), result );

    return result;
    }

// -----------------------------------------------------------------------------
// CSendObject::ExistsL
// Check if the file already exists on the storage.
// -----------------------------------------------------------------------------
//
TBool CSendObject::ExistsL( const TDesC& aName ) const
    {
    PRINT1( _L( "MM MTP => CSendObject::Exists aName = %S" ), &aName );
    // This detects both files and folders
    TBool ret( EFalse );
    ret = BaflUtils::FileExists( iFramework.Fs(), aName );

#ifdef MMMTPDP_REPLACE_EXIST_FILE
    if( ret )
        {
        // delete the old one and replace
        TInt delErr = iFramework.Fs().Delete( aName );
        PRINT1( _L( "MM MTP <> CSendObject::Exists delErr = %d" ), delErr );
        // delete from the metadata DB
        TRAPD( err, iFramework.ObjectMgr().RemoveObjectL( aName ) );
        PRINT1( _L( "MM MTP <> CSendObject::Exists err = %d" ), err );
        if( err == KErrNone )
            {
            // do nothing, ignore warning
            }
        // delete from video/mpx DB
        CMTPObjectMetaData* objectInfo = CMTPObjectMetaData::NewLC(); // + objectInfo
        if ( iFramework.ObjectMgr().ObjectL( aName, *objectInfo ) )
            {
            TRAP_IGNORE( iWrapper.DeleteObjectL( aName,
                    objectInfo->Uint( CMTPObjectMetaData::EFormatCode ) ) );
            }
        CleanupStack::PopAndDestroy( objectInfo ); // - objectInfo
        ret = EFalse;
        }
#endif
    PRINT1( _L( "MM MTP <= CSendObject::Exists ret = %d" ), ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// CSendObject::ReserveObjectL
// -----------------------------------------------------------------------------
//
TInt CSendObject::ReserveObjectL()
    {
    PRINT( _L( "MM MTP => CSendObject::ReserveObjectL" ) );
    TInt err = KErrNone;

    iReceivedObjectInfo->SetUint( CMTPObjectMetaData::EStorageId, iStorageId );
    iReceivedObjectInfo->SetUint( CMTPObjectMetaData::EParentHandle,
        iParentHandle );
    iReceivedObjectInfo->SetDesCL( CMTPObjectMetaData::ESuid, iFullPath );
    iReceivedObjectInfo->SetUint( CMTPObjectMetaData::EFormatCode,
        iObjectFormat );

    // Reserves space for and assigns an object handle to the object described
    // by the specified object information record.
    TRAP( err, iObjectMgr.ReserveObjectHandleL( *iReceivedObjectInfo,
        iObjectSize ) );
    iRollbackList.Append( &CSendObject::UnreserveObjectL );

    PRINT2( _L( "MM MTP => CSendObject::ReserveObjectL iObjectsize = %Lu, Operation: 0x%x" ), iObjectSize, iOperationCode );
    if ( err != KErrNone )
        {
        PRINT1( _L( "MM MTP <> ReserveObjectHandleL err = %d" ), err );
        }

    if ( err == KErrNone )
        {
        delete iFileReceived;
        iFileReceived = NULL;
        PRINT2( _L( "MM MTP <> CSendObject::ServiceObjectL, iFullPath is %S, iObjectSize: %Lu" ), &iFullPath, iObjectSize );
        iRollbackList.Append( &CSendObject::RemoveObjectFromFs );
        TRAP( err, iFileReceived = CMTPTypeFile::NewL( iFs, iFullPath, EFileWrite ) );

        PRINT1( _L("MM MTP <> CMTPTypeFile::NewL Leave Code is: %d"), err );
        }

    if ( err == KErrNone )
        {
        TRAP( err, iFileReceived->SetSizeL( iObjectSize ) );
        PRINT1( _L( "MM MTP <> SetSizeL leave code:%d" ), err );
        if ( err == KErrDiskFull )
            {
            SendResponseL( EMTPRespCodeStoreFull );
            Rollback();
            return err;
            }
        }

    if ( err == KErrNone && iObjectSize == 0 )
        {
        // Already trapped inside SaveEmptyFileL.
        SaveEmptyFileL();
        if ( iOperationCode == EMTPOpCodeSendObjectPropList )
            {
            // Only leave when getting proplist element from data received by fw.
            // It should not happen after ReceiveDataL in which construction of proplist already succeed.
            SetObjectPropListL();
            }

        iObjectMgr.CommitReservedObjectHandleL( *iReceivedObjectInfo );
        iRollbackList.Reset();
        }

    if ( err == KErrNone )
        {
        iExpectedSendObjectRequest.SetUint32( TMTPTypeRequest::ERequestSessionID, iSessionId );
        iFramework.RouteRequestRegisterL( iExpectedSendObjectRequest, iConnection );

        TUint32 parameters[3];
        parameters[0] = iStorageId;
        parameters[1] = iParentHandle;
        parameters[2] = iReceivedObjectInfo->Uint( CMTPObjectMetaData::EHandle );
        SendResponseL( EMTPRespCodeOK, 3, parameters );
        }
    else
        {
        SendResponseL( EMTPRespCodeGeneralError );
        Rollback();
        }

    PRINT( _L( "MM MTP <= CSendObject::ReserveObjectL" ) );
    return err;
    }

// -----------------------------------------------------------------------------
// CSendObject::SetFileProperties
// -----------------------------------------------------------------------------
//
void CSendObject::SetFileProperties()
    {
    PRINT1( _L( "MM MTP => CSendObject::SetFileProperties iProtectionStatus = %d" ), iProtectionStatus );

    if ( iFileReceived != NULL )
        {
        if ( iProtectionStatus == EMTPProtectionNoProtection
            || iProtectionStatus == EMTPProtectionReadOnly )
            {
            TInt err = KErrNone;
            if ( iProtectionStatus == EMTPProtectionNoProtection )
                {
                err = iFileReceived->File().SetAtt( KEntryAttNormal, KEntryAttReadOnly );
                }
            else
                {
                err = iFileReceived->File().SetAtt( KEntryAttReadOnly, KEntryAttNormal );
                }
    
            if ( err != KErrNone )
                {
                PRINT1( _L( "MM MTP <> CSendObject::SetFileProperties SetAtt err = %d" ), err );
                }
            }

        if( iDateMod != NULL )
            {
            TTime modTime( 0 );
            TInt err = MmMtpDpUtility::DesToTTime( *iDateMod, modTime );
            if( err == KErrNone )
                {
                err = iFileReceived->File().SetModified( modTime );
                PRINT1( _L( "MM MTP <> CSendObject::SetFileProperties SetModified err = %d" ), err );
                }
            }

        // Close the file after SetFileProperties to make sure other process won't open
        // the file successfully right at the time calling RFile::SetAtt.
        if ( iObjectSize > 0 )
            {
            delete iFileReceived;
            iFileReceived = NULL;
            }
        else
            iFileReceived->File().Close();
        }

    PRINT( _L( "MM MTP <= CSendObject::SetFileProperties" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::SaveEmptyFileL
// -----------------------------------------------------------------------------
//
void CSendObject::SaveEmptyFileL()
    {
    PRINT( _L( "MM MTP => CSendObject::SaveEmptyFileL" ) );

    if ( EMTPFormatCodeAbstractAudioVideoPlaylist == iObjectFormat )
        {
        TInt err = iFileReceived->File().SetAtt( KEntryAttSystem | KEntryAttHidden,
            KEntryAttReadOnly | KEntryAttNormal );
        if ( err != KErrNone )
            PRINT1( _L( "MM MTP <> CSendObject::SaveEmptyFileL err = %d" ), err );
        iDpConfig.GetWrapperL().AddDummyFileL( iFullPath );
        }

    // Set subformat code to avoid MPX query for the first time to GetObjectReference,
    // in which case references has been kept in fw.
    if ( MmMtpDpUtility::HasReference( iObjectFormat ) )
        iReceivedObjectInfo->SetUint( CMTPObjectMetaData::EFormatSubCode, EMTPSubFormatCodeUndefined );

    // add playlist to MPX DB
    TRAPD( err, AddMediaToStoreL() );

    if ( err != KErrNone )
        iRollbackList.Append( &CSendObject::RemoveObjectFromDbL );
    else
        iRollbackList.Reset();

    PRINT1( _L( "MM MTP <= CSendObject::SaveEmptyFileL err = %d" ), err );
    }

// -----------------------------------------------------------------------------
// CSendObject::AddMediaToStoreL()
//
// -----------------------------------------------------------------------------
//
void CSendObject::AddMediaToStoreL()
    {
    PRINT( _L( "MM MTP => CSendObject::AddMediaToStoreL" ) );

    // SetFileProperties here make sure no matter the previous operation is SendObjectInfo
    // or SendObjectPropList
    // Might need to set dateadded for further extension.
    SetFileProperties();

    PRINT1( _L( "MM MTP <> CSendObject::AddMediaToStoreL iFullPath = %S" ), &iFullPath );
    iDpConfig.GetWrapperL().AddObjectL( *iReceivedObjectInfo );

    iDpConfig.GetWrapperL().SetImageObjPropL( *iReceivedObjectInfo, iWidth, iHeight );

    PRINT( _L( "MM MTP <= CSendObject::AddMediaToStoreL" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::UsbDisconnect
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendObject::UsbDisconnect()
    {
    PRINT( _L( "MM MTP => CSendObject::UsbDisconnect" ) );
    Rollback();
    PRINT( _L( "MM MTP <= CSendObject::UsbDisconnect" ) );
    }

// -----------------------------------------------------------------------------
// CSendObject::Rollback()
// delete the file, which transfer incompletely
// -----------------------------------------------------------------------------
//
void CSendObject::Rollback()
    {
    PRINT( _L("MM MTP => CSendObject::Rollback") );

    TInt count = iRollbackList.Count();
    PRINT1( _L("MM MTP => CSendObject::Rollback, iRollbackList.Count() = %d"), iRollbackList.Count() );

    for ( TInt i = 0; i < count; i++ )
        {
        TMmMtpRollbackAction tmp = iRollbackList[i];
        ( this->*((TMmMtpRollbackAction)(iRollbackList[i])))();
        }
    iRollbackList.Reset();

    PRINT( _L("MM MTP <= CSendObject::Rollback") );
    }

void CSendObject::UnreserveObjectL()
    {
    PRINT( _L("MM MTP => CSendObject::UnreserveObjectL") );
    iFramework.ObjectMgr().UnreserveObjectHandleL( *iReceivedObjectInfo );
    PRINT( _L("MM MTP <= CSendObject::UnreserveObjectL") );
    }

void CSendObject::RemoveObjectFromDbL()
    {
    PRINT( _L("MM MTP => CSendObject::RemoveObjectFromDbL") );
    iFramework.ObjectMgr().RemoveObjectL( iReceivedObjectInfo->DesC( CMTPObjectMetaData::ESuid ) );
    iDpConfig.GetWrapperL().DeleteObjectL( *iReceivedObjectInfo );
    PRINT( _L("MM MTP <= CSendObject::RemoveObjectFromDbL") );
    }

void CSendObject::RemoveObjectFromFs()
    {
    PRINT( _L("MM MTP => CSendObject::RemoveObjectFromFs") );

    delete iFileReceived;
    iFileReceived = NULL;

    TInt err = iFramework.Fs().Delete( iFullPath );
    if ( err != KErrNone )
        {
        PRINT1( _L("MM MTP <> CSendObject::RemoveObjectFromFs err = %d"), err );
        }
    PRINT( _L("MM MTP <= CSendObject::RemoveObjectFromFs") );
    }

// end of file
