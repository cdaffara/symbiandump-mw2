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


#include <mtp/mmtpconnection.h>
#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/mmtpstoragemgr.h>

#include "crequestchecker.h"
#include "mmmtpdplogger.h"

static const TInt KMTPRequestCheckerHandleGranularity = 2;

// -----------------------------------------------------------------------------
// CRequestChecker::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
CRequestChecker* CRequestChecker::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection )
    {
    CRequestChecker* self = new (ELeave) CRequestChecker( aFramework, aConnection );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::CRequestChecker
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CRequestChecker::CRequestChecker( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection ) :
    iFramework( aFramework ),
    iConnection( aConnection ),
    iHandles( KMTPRequestCheckerHandleGranularity ),
    iObjectArray( KMTPRequestCheckerHandleGranularity )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRequestChecker::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CRequestChecker::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRequestChecker::~CRequestChecker
// Destructor
// -----------------------------------------------------------------------------
//
CRequestChecker::~CRequestChecker()
    {
    iHandles.Close();
    iObjectArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CRequestChecker::VerifyRequestL
// Verify the request
// -----------------------------------------------------------------------------
//
TMTPResponseCode CRequestChecker::VerifyRequestL( const TMTPTypeRequest& aRequest,
    TInt aCount,
    const TMTPRequestElementInfo* aElementInfo )
    {
    TMTPResponseCode result = EMTPRespCodeOK;
    iHandles.Close();
    iObjectArray.ResetAndDestroy();

    result = CheckRequestHeader( aRequest );

    for ( TInt i = 0; i < aCount && EMTPRespCodeOK == result; i++ )
        {
        TUint32 parameter = aRequest.Uint32( aElementInfo[i].iElementIndex );
        PRINT3( _L( "MM MTP <> CRequestChecker parameter %d/%d = %d" ),
            i + 1,
            aCount, 
            parameter );

        if ( !IsSpecialValue( parameter, aElementInfo[i] ) )
            {
            switch ( aElementInfo[i].iElementType )
                {
                case EMTPElementTypeSessionID:
                    result = VerifySessionId( parameter, aElementInfo[i] );
                    break;

                case EMTPElementTypeObjectHandle:
                    result = VerifyObjectHandleL( parameter, aElementInfo[i] );
                    break;

                case EMTPElementTypeStorageId:
                    result = VerifyStorageIdL( parameter, aElementInfo[i] );
                    break;

                case EMTPElementTypeFormatCode:
                    result = VerifyFormatCode( parameter, aElementInfo[i] );
                    break;

                default:
                    User::Invariant(); // Should never run
                    break;
                }
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::GetObjectInfo
// Return the object info for the handle
// -----------------------------------------------------------------------------
//
CMTPObjectMetaData* CRequestChecker::GetObjectInfo( TUint32 aHandle ) const
    {
    CMTPObjectMetaData* result = NULL;
    TInt count = iHandles.Count();
    for( TInt i = 0; i < count; i++ )
        {
        if ( iHandles[i] == aHandle )
            {
            result = iObjectArray[i];
            break;
            }
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::CheckRequestHeader
// Check the request header portion (session Id and transaction code)
// -----------------------------------------------------------------------------
//
TMTPResponseCode CRequestChecker::CheckRequestHeader( const TMTPTypeRequest& aRequest ) const
    {
    TMTPResponseCode ret = EMTPRespCodeOK;
    TUint16 operationCode = aRequest.Uint16( TMTPTypeRequest::ERequestOperationCode );
    TUint32 sessionId = aRequest.Uint32( TMTPTypeRequest::ERequestSessionID );
    TUint32 transactionCode = aRequest.Uint32( TMTPTypeRequest::ERequestTransactionID );

    if ( operationCode == EMTPOpCodeCloseSession || operationCode == EMTPOpCodeResetDevice )
        {
        if ( sessionId != 0 )
            {
            ret = EMTPRespCodeInvalidParameter;
            }
        }
    else
        {
        // requests that are valid when there's no opened session.
        if ( sessionId == 0 )
            {
            switch ( operationCode )
                {
                case EMTPOpCodeGetDeviceInfo:
                case EMTPOpCodeOpenSession:
                    {
                    // Transaction id must be 0 when called out side an active session.
                    if ( transactionCode != 0 )
                        {
                        ret = EMTPRespCodeInvalidTransactionID;
                        }
                    }
                    break;

                default:
                    {
                    ret = EMTPRespCodeSessionNotOpen;
                    }
                    break;
                }
            }
        else if ( !iConnection.SessionWithMTPIdExists( sessionId ) )
            {
            ret = EMTPRespCodeSessionNotOpen;
            }
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::VerifySessionId
// Check the session id in the request parameter (NOTE the session id is different from the one in the request header),
// this usually only applies to the OpenSession request
// -----------------------------------------------------------------------------
//
TMTPResponseCode CRequestChecker::VerifySessionId( TUint32 aSessionId,
    const TMTPRequestElementInfo& /*aElementInfo*/ ) const
    {
    TMTPResponseCode ret = EMTPRespCodeOK;

    if ( aSessionId != 0 )
        {
        if ( iConnection.SessionWithMTPIdExists( aSessionId ) )
            {
            ret = EMTPRespCodeSessionAlreadyOpen;
            }
        }
    else
        {
        ret = EMTPRespCodeInvalidParameter;
        }

    return ret;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::VerifyObjectHandleL
// Check the object handle in the request parameter, whether the handle is in the object store,
// read/write, file/dir
// -----------------------------------------------------------------------------
//
TMTPResponseCode CRequestChecker::VerifyObjectHandleL( TUint32 aHandle,
    const TMTPRequestElementInfo& aElementInfo )
    {
    PRINT1( _L("MM MTP => CRequestChecker::VerifyObjectHandleL aHandle = 0x%x"), aHandle );
    TMTPResponseCode ret = EMTPRespCodeOK;

    CMTPObjectMetaData* object( CMTPObjectMetaData::NewLC() );
    TBool result( iFramework.ObjectMgr().ObjectL( aHandle, *object ) );
    // Obj handle exists
    if ( result )
        {
        const TDesC& suid( object->DesC( CMTPObjectMetaData::ESuid ) );
        TEntry entry;
        TInt err = iFramework.Fs().Entry( suid, entry );

        if ( err == KErrNotFound )
            {
            iFramework.ObjectMgr().RemoveObjectL( suid );
            // TODO: workaround for abstractalbumart
            // ret = EMTPRespCodeInvalidObjectHandle;
            }
        else if ( err != KErrNone )
            ret = EMTPRespCodeGeneralError;
        else if ( object->Uint( CMTPObjectMetaData::EFormatCode ) != EMTPFormatCodeAssociation )
            {
            if ( iFramework.ObjectMgr().ObjectOwnerId( aHandle ) != iFramework.DataProviderId() )
                ret = EMTPRespCodeInvalidObjectHandle;

            if ( ( aElementInfo.iElementAttr & EMTPElementAttrWrite ) && entry.IsReadOnly() )
                ret = EMTPRespCodeObjectWriteProtected;

            if ( ( EMTPRespCodeOK == ret ) && ( aElementInfo.iElementAttr & EMTPElementAttrFile ) && entry.IsDir() )
                ret = EMTPRespCodeInvalidObjectHandle;

            if ( ( EMTPRespCodeOK == ret ) && ( aElementInfo.iElementAttr & EMTPElementAttrDir ) && !entry.IsDir() )
                ret = EMTPRespCodeInvalidParentObject;
            }
        }

    if ( result )
        {
        iObjectArray.AppendL( object );
        CleanupStack::Pop( object );
        iHandles.AppendL( aHandle );
        }
    else
        {
        CleanupStack::PopAndDestroy( object );
        ret = EMTPRespCodeInvalidObjectHandle;
        }

    PRINT1( _L( "MM MTP <= CRequestChecker::VerifyObjectHandleL ret = 0x%x" ), ret );

    return ret;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::VerifyStorageIdL
// Check the storage id parameter in the request, read/write attributes
// -----------------------------------------------------------------------------
//
TMTPResponseCode CRequestChecker::VerifyStorageIdL( TUint32 aStorageId,
    const TMTPRequestElementInfo& aElementInfo ) const
    {
    MMTPStorageMgr& mgr( iFramework.StorageMgr() );
    TMTPResponseCode ret( EMTPRespCodeOK );
    if ( !mgr.ValidStorageId( aStorageId ) )
        {
        ret = EMTPRespCodeInvalidStorageID;
        }
    else
        {
        if ( !mgr.LogicalStorageId( aStorageId ) )
            {
            ret = EMTPRespCodeStoreNotAvailable;
            }
        else
            {
            TInt drive( mgr.DriveNumber( aStorageId ) );
            // StorageIDs which are not system owned do not correspond to drives.
            if ( drive != KErrNotFound )
                {
                TDriveInfo info;
                User::LeaveIfError( iFramework.Fs().Drive( info, drive ) );
                if ( info.iType == EMediaNotPresent )
                    {
                    /**
                    * Race conditions between media ejection and request processing
                    * may result in a previously valid storage ID no longer being
                    * available.
                    */
                    ret = EMTPRespCodeStoreNotAvailable;
                    }
                else
                    if ( ( aElementInfo.iElementAttr & EMTPElementAttrWrite )
                        && ( ( info.iMediaAtt & KMediaAttWriteProtected )
                        || ( info.iDriveAtt & KDriveAttRom ) ) )
                        {
                        ret = EMTPRespCodeStoreReadOnly;
                        }
                }
            }
       }

    return ret;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::VerifyFormatCode
// Check the format code parameter in the request
// -----------------------------------------------------------------------------
//
TMTPResponseCode CRequestChecker::VerifyFormatCode( TUint32 aFormatCode,
    const TMTPRequestElementInfo& aElementInfo ) const
    {
    PRINT1( _L( "MM MTP => CRequestChecker::VerifyFormatCode aFormatCode = 0x%x" ), aFormatCode );
    TMTPResponseCode ret = EMTPRespCodeInvalidObjectFormatCode;

    if ( aElementInfo.iElementAttr == EMTPElementAttrFormatEnums )
        {
        TUint32* formatArray = (TUint32*)( aElementInfo.iValue1 );
        TUint32 i = 0;
        for ( i = 0; i < aElementInfo.iValue2; i++ )
            {
            if ( aFormatCode == formatArray[i] )
                {
                ret = EMTPRespCodeOK;
                break;
                }
            }
        }
    else
        {
        if ( ( aFormatCode >= EMTPFormatCodePTPStart )
            && ( aFormatCode <= EMTPFormatCodeMTPEnd ) )
            {
            ret = EMTPRespCodeOK;
            }
        }

    PRINT1( _L( "MM MTP => CRequestChecker::VerifyFormatCode ret = 0x%x" ), ret );

    return ret;
    }

// -----------------------------------------------------------------------------
// CRequestChecker::IsSpecialValue
// Check if the parameter is one of the special values
// -----------------------------------------------------------------------------
//
TBool CRequestChecker::IsSpecialValue( TUint32 aParameter,
    const TMTPRequestElementInfo& aElementInfo ) const
    {
    TBool result = EFalse;
    switch ( aElementInfo.iCount )
        {
        case 1:
            result = ( aParameter == aElementInfo.iValue1 );
            break;

        case 2:
            result = ( aParameter == aElementInfo.iValue1
                || aParameter == aElementInfo.iValue2 );
            break;

        default:
            break;
        }

    return result;
    }

// end of file
