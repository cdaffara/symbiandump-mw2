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
* Description:  Implement opeartion SetObjectPropValue
*
*/


#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/mmtpobjectmgr.h>

#include "csetobjectpropvalue.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// Verification data for the SetObjectPropValue request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPSetObjectPropValuePolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrWrite,
        0,
        0,
        0
        },
    };

// -----------------------------------------------------------------------------
// CSetObjectPropValue::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
//EXPORT_C MMmRequestProcessor* CSetObjectPropValue::NewL( MMTPDataProviderFramework& aFramework,
//                                            MMTPConnection& aConnection,
//                                            CMmMtpDpMetadataAccessWrapper& aWrapper )
//    {
//    CSetObjectPropValue* self = new (ELeave) CSetObjectPropValue( aFramework, aConnection, aWrapper );
//
//    return self;
//    }

// -----------------------------------------------------------------------------
// CSetObjectPropValue::CSetObjectPropValue
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSetObjectPropValue::CSetObjectPropValue(
    MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CRequestProcessor( aFramework,
        aConnection,
        sizeof(KMTPSetObjectPropValuePolicy) / sizeof(TMTPRequestElementInfo),
        KMTPSetObjectPropValuePolicy ),
    iObjectMgr( aFramework.ObjectMgr() ),
    iDpConfig( aDpConfig ),
    iFs( aFramework.Fs() )
    {
    PRINT( _L( "Operation: SetObjectPropValue(0x9804)" ) );
    }

// -----------------------------------------------------------------------------
// CSetObjectPropValue::~CSetObjectPropValue
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSetObjectPropValue::~CSetObjectPropValue()
    {
    delete iMTPTypeString;
    delete iMTPTypeArray;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropValue::IsPropCodeReadonly
// A helper function of CheckRequestL.
// To check whether the object property code is readonly.
// -----------------------------------------------------------------------------
//
TBool CSetObjectPropValue::IsPropCodeReadonly( TUint16 aPropCode )
    {
    TBool returnCode = EFalse;

    switch (aPropCode)
        {
        case EMTPObjectPropCodeStorageID:
        case EMTPObjectPropCodeObjectFormat:
        case EMTPObjectPropCodeProtectionStatus:
        case EMTPObjectPropCodeObjectSize:
        case EMTPObjectPropCodeParentObject:
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
        case EMTPObjectPropCodeDateAdded:
        case EMTPObjectPropCodeDateCreated:
        case EMTPObjectPropCodeDateModified:
        //case EMTPObjectPropCodeVideoBitRate:  // move to specific dp
            returnCode = ETrue;
            break;
        case EMTPObjectPropCodeNonConsumable:
            // It's settable, return EFalse here.
            break;

        default:
            returnCode = IsSpecificPropCodeReadOnly(aPropCode);
            break;
        }

    return returnCode;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropValue::CheckRequestL
// Verify the reqeust
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CSetObjectPropValue::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CSetObjectPropValue::CheckRequestL" ) );

    TMTPResponseCode result = CRequestProcessor::CheckRequestL();
    if ( result == EMTPRespCodeObjectWriteProtected )
        {
        // Return AccessDenied for P4S pass rate, instead of EMTPRespCodeObjectWriteProtected
        result = EMTPRespCodeAccessDenied;
        }

    // Check if property is supported
    if ( result == EMTPRespCodeOK )
        {
        iPropCode = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
        PRINT1( _L( "MM MTP <> CSetObjectPropValue::CheckRequestL iPropCode = 0x%x" ), iPropCode );

        TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
        CMTPObjectMetaData* objectInfo = iRequestChecker->GetObjectInfo( objectHandle );

        TUint32 formatCode = objectInfo->Uint( CMTPObjectMetaData::EFormatCode );
        PRINT3( _L( "MM MTP <> CSetObjectPropValue::CheckRequestL, handle = 0x%x, filename = %S, formatCode = 0x%x" ),
            objectHandle,
            &(objectInfo->DesC( CMTPObjectMetaData::ESuid )),
            formatCode );
        const RArray<TUint>* properties = iDpConfig.GetSupportedPropertiesL( formatCode );
        TInt count = properties->Count();

        result = EMTPRespCodeInvalidObjectPropCode;
        for ( TInt i = 0; i < count; i++ )
            {
            // Object property code is supported, but can not be set which is read only.
            if ( ( *properties )[i] == iPropCode && IsPropCodeReadonly( iPropCode ) )
                {
                result = EMTPRespCodeAccessDenied;
                break;
                }
            // Object property code is supported and can be set.
            else if ( iPropCode == ( *properties )[i] )
                {
                result = EMTPRespCodeOK;
                break;
                }
            } // end of for
        }

    PRINT( _L( "MM MTP <= CSetObjectPropValue::CheckRequestL" ) );
    return result;
    }

EXPORT_C TBool CSetObjectPropValue::HasDataphase() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropValue::ServiceL
// SetObjectPropValue request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CSetObjectPropValue::ServiceL()
    {
    PRINT( _L( "MM MTP => CSetObjectPropValue::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus( EMtpPSStatusActive );
    
    __ASSERT_DEBUG( iRequestChecker, Panic( EMmMTPDpRequestCheckNull ) );
    TUint32 handle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    PRINT1( _L( "MM MTP <> CSetObjectPropValue::ServiceL handle = 0x%x" ), handle );

    iObjectInfo = iRequestChecker->GetObjectInfo( handle );

    switch ( iPropCode )
        {
        // Access not allowed on these properties: (so send error)
        case EMTPObjectPropCodeStorageID:
        case EMTPObjectPropCodeObjectFormat:
        case EMTPObjectPropCodeProtectionStatus:
        case EMTPObjectPropCodeObjectSize:
        case EMTPObjectPropCodeParentObject:
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
        case EMTPObjectPropCodeDateAdded:       // 0xDC4E
        case EMTPObjectPropCodeDateCreated:     // Date Created(0xDC08)
        case EMTPObjectPropCodeDateModified:    // Modified Date(0xDC09)
        case EMTPObjectPropCodeVideoBitRate: // 0xDE9C
            {
            SendResponseL( EMTPRespCodeAccessDenied );
            }
            break;

        case EMTPObjectPropCodeNonConsumable:   // Non Consumable(0xDC4F)
            ReceiveDataL( iMTPTypeUint8 );
            break;

        // Get Data for String objects
        case EMTPObjectPropCodeObjectFileName:  // 0xDC07
        case EMTPObjectPropCodeName: // 0xDC44
        case EMTPObjectPropCodeAlbumArtist:
            {
            delete iMTPTypeString;
            iMTPTypeString = NULL;
            iMTPTypeString = CMTPTypeString::NewL();
            ReceiveDataL( *iMTPTypeString );
            }
            break;

        default:
            {
            ReceiveDataSpecificObjectPropertyL( iPropCode );
            }
            break;
        }
    PRINT( _L( "MM MTP <= CSetObjectPropValue::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CSetObjectPropValue::DoHandleResponsePhaseL
// Edit the file to update the prop value
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSetObjectPropValue::DoHandleResponsePhaseL()
    {
    PRINT( _L( "MM MTP => CSetObjectPropValue::DoHandleResponsePhaseL" ) );
    __ASSERT_DEBUG( iObjectInfo, Panic( EMmMTPDpObjectNull ) );
    TMTPResponseCode responseCode = EMTPRespCodeOK;

    switch ( iPropCode )
        {
        case EMTPObjectPropCodeNonConsumable:
            iObjectInfo->SetUint( CMTPObjectMetaData::ENonConsumable, iMTPTypeUint8.Value() );
            iFramework.ObjectMgr().ModifyObjectL( *iObjectInfo );
            break;

        case EMTPObjectPropCodeObjectFileName:
            {
            TPtrC suid( iObjectInfo->DesC( CMTPObjectMetaData::ESuid ) );
            TPtrC ptr ( iMTPTypeString->StringChars() );
            if ( KMaxFileName < ptr.Length() )
                responseCode = EMTPRespCodeInvalidDataset;
            else
                {
                TFileName newSuid( ptr );
                PRINT2( _L( "MM MTP <> old name = %S, new name = %S" ), &suid, &newSuid );
                TInt err = KErrNone;
                err = MmMtpDpUtility::UpdateObjectFileName( iFramework.Fs(),
                    suid,
                    newSuid );
                // TODO: if the new name is the same with old name
                PRINT1( _L( "MM MTP <> Update object file name err = %d" ), err );
                if ( KErrOverflow == err ) // full path name is too long
                    {
                    responseCode = EMTPRespCodeInvalidDataset;
                    }
                else if ( KErrNone == err )
                    {
                    TRAP( err, iDpConfig.GetWrapperL().RenameObjectL( *iObjectInfo, newSuid ) ); //Update MPX DB
                    PRINT1( _L( "MM MTP <> Rename MPX object file name err = %d" ), err );
                    // it is ok if file is not found in DB, following S60 solution
                    if ( KErrNotFound == err )
                        {
                        TRAP( err, iDpConfig.GetWrapperL().AddObjectL( *iObjectInfo ) );
                        PRINT1( _L( "MM MTP <> Add MPX object file name err = %d" ), err );
                        }

                    iObjectInfo->SetDesCL( CMTPObjectMetaData::ESuid, newSuid );
                    iFramework.ObjectMgr().ModifyObjectL( *iObjectInfo );
                    }
                else if ( KErrInUse == err ) // object file is being used by other program
                    {
                    responseCode = EMTPRespCodeDeviceBusy;
                    }
                else
                    {
                    responseCode = EMTPRespCodeGeneralError;
                    }
                }
            }
            break;

        case EMTPObjectPropCodeName: // 0xDC44
        case EMTPObjectPropCodeAlbumArtist:
            {
            responseCode = ServiceMetaDataToWrapperL( iPropCode,
                *iMTPTypeString,
                *iObjectInfo );
            }
            break;

        default:
            {
            responseCode = ServiceSpecificObjectPropertyL( iPropCode );
            }
            break;
        }

    SendResponseL( responseCode );
    PRINT1( _L( "MM MTP <= CSetObjectPropValue::DoHandleResponsePhaseL responseCode = 0x%x" ), responseCode );

    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSetObjectPropValue::ServiceMetaDataToWrapperL
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CSetObjectPropValue::ServiceMetaDataToWrapperL( const TUint16 aPropCode,
    MMTPType& aNewData,
    const CMTPObjectMetaData& aObjectMetaData )
    {
    TMTPResponseCode resCode = EMTPRespCodeOK;

    TRAPD( err, iDpConfig.GetWrapperL().SetObjectMetadataValueL( aPropCode,
        aNewData,
        aObjectMetaData ) );

    PRINT1( _L( "MM MTP <> CSetObjectPropValue::ServiceMetaDataToWrapperL err = %d" ), err);

    if ( err == KErrNone )
        {
        resCode = EMTPRespCodeOK;
        }
    else if ( err == KErrTooBig )
        // according to the codes of S60
        {
        resCode = EMTPRespCodeInvalidDataset;
        }
    else if ( err == KErrPermissionDenied )
        {
        resCode = EMTPRespCodeAccessDenied;
        }
    else if ( err == KErrNotFound )
        {
        if( MmMtpDpUtility::HasMetadata( aObjectMetaData.Uint( CMTPObjectMetaData::EFormatCode ) ) )
            resCode = EMTPRespCodeAccessDenied;
        }
    else
        {
        resCode = EMTPRespCodeGeneralError;
        }

    PRINT1( _L( "MM MTP <> CSetObjectPropValue::ServiceMetaDataToWrapperL resCode = 0x%x" ), resCode );

    return resCode;
    }

// end of file
