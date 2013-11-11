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
* Description:  Implement the operation: GetObjectPropValue
*
*/


#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/mmtpobjectmgr.h>
#include <f32file.h>

#include "cgetobjectpropvalue.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"
#include "tobjectdescription.h"


// -----------------------------------------------------------------------------
// Verification data for the GetObjectPropValue request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPGetObjectPropValuePolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrNone,
        0,
        0,
        0
        },
    };

// -----------------------------------------------------------------------------
// CGetObjectPropValue::~CGetObjectPropValue
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectPropValue::~CGetObjectPropValue()
    {
    delete iMTPTypeString;
    delete iMTPTypeArray;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropValue::CGetObjectPropValue
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectPropValue::CGetObjectPropValue( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CRequestProcessor( aFramework,
        aConnection,
        sizeof ( KMTPGetObjectPropValuePolicy ) / sizeof ( TMTPRequestElementInfo ),
        KMTPGetObjectPropValuePolicy ),
    iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: GetObjectPropValue(0x9803)" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropValue::CheckRequestL
// Verify the reqeust
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CGetObjectPropValue::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CGetObjectPropValue::CheckRequestL" ) );
    TMTPResponseCode result = CRequestProcessor::CheckRequestL();

    //Now check to see if we support the property
    if ( result == EMTPRespCodeOK )
        {
        TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
        CMTPObjectMetaData* objectInfo = iRequestChecker->GetObjectInfo( objectHandle );

        if (!objectInfo)
            {
            PRINT(_L("MM MTP <> CGetObjectPropValue::CheckRequestL, objectInfo is NULL"));
            return EMTPRespCodeInvalidObjectHandle;
            }

        TUint32 formatCode = objectInfo->Uint( CMTPObjectMetaData::EFormatCode );

        PRINT3( _L( "MM MTP <> CGetObjectPropValue::CheckRequestL, handle = 0x%x, filename = %S, formatCode = 0x%x" ),
            objectHandle, 
            &( objectInfo->DesC( CMTPObjectMetaData::ESuid ) ),
            formatCode );

        const RArray<TUint>* properties = iDpConfig.GetSupportedPropertiesL( formatCode );
        TInt count = properties->Count();

        TUint32 propCode = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
        PRINT1( _L( "MM MTP <> CGetObjectPropValue::CheckRequestL propCode = 0x%x" ), propCode );

        result = EMTPRespCodeInvalidObjectPropCode;

        for ( TInt i = 0; i < count; i++ )
            {
            if ( (*properties)[i] == propCode )
                {
                result = EMTPRespCodeOK;
                break;
                }
            }
        }

    PRINT1( _L( "MM MTP <= CGetObjectPropValue::CheckRequestL result = 0x%x" ), result );
    return result;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropValue::ServiceL
// GetObjectPropValue request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropValue::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetObjectPropValue::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus( EMtpPSStatusActive );

    //Get the request information
    TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    TUint32 propCode = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    PRINT2( _L( "MM MTP <> CGetObjectPropValue::ServiceL objectHandle = 0x%x, propCode = 0x%x" ),
        objectHandle, propCode );

    // don't have the ownship of the object
    iObjectInfo = iRequestChecker->GetObjectInfo( objectHandle );
    TPtrC suid( iObjectInfo->DesC( CMTPObjectMetaData::ESuid ) );
    PRINT1( _L( "MM MTP <> CGetObjectPropValue::ServiceL object file name is %S" ), &suid );

    if ( iMTPTypeString != NULL )
        {
        delete iMTPTypeString;
        iMTPTypeString = NULL;
        }

    switch ( propCode )
        {
        // Storage ID
        case EMTPObjectPropCodeStorageID:
            {
            iMTPTypeUint32.Set( 0 );
            iMTPTypeUint32 = iObjectInfo->Uint( CMTPObjectMetaData::EStorageId );
            SendDataL( iMTPTypeUint32 );
            }
            break;

        // Format Code
        case EMTPObjectPropCodeObjectFormat:
            {
            iMTPTypeUint16.Set( 0 );
            iMTPTypeUint16 = iObjectInfo->Uint( CMTPObjectMetaData::EFormatCode );
            PRINT1( _L( "MM MTP <> CGetObjectPropValue::ServiceL ObjectFormat = 0x%x" ),
                iMTPTypeUint16.Value() );

            SendDataL( iMTPTypeUint16 );
            }
            break;

        // ProtectionStatus
        case EMTPObjectPropCodeProtectionStatus:
            {
            iMTPTypeUint16.Set( 0 );
            iMTPTypeUint16 = MmMtpDpUtility::GetProtectionStatusL( iFramework.Fs(), suid );
            SendDataL( iMTPTypeUint16 );
            }
            break;

        // Object Size
        case EMTPObjectPropCodeObjectSize:
            {
            iMTPTypeUint64.Set( 0 );
            iMTPTypeUint64 = MmMtpDpUtility::GetObjectSizeL( iFramework.Fs(), suid );
            SendDataL( iMTPTypeUint64 );
            }
            break;

        // Filename
        case EMTPObjectPropCodeObjectFileName:
            {
            TParsePtrC parse( suid );
            iMTPTypeString = CMTPTypeString::NewL( parse.NameAndExt() );
            SendDataL( *iMTPTypeString );
            }
            break;

        // Parent
        case EMTPObjectPropCodeParentObject:
            {
            iMTPTypeUint32.Set( KMTPHandleNone );

            TUint32 parentHandle = iObjectInfo->Uint( CMTPObjectMetaData::EParentHandle );
            // refer to 5.3.1.9 of MTP Spec 1.0
            if ( parentHandle != KMTPHandleNoParent )
                {
                iMTPTypeUint32.Set( parentHandle );
                }

            SendDataL( iMTPTypeUint32 );
            }
            break;

        // Persistent Unique Object Identifier
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
            {
            iMTPTypeUint128.Set( 0, 0 );
            iMTPTypeUint128 = iFramework.ObjectMgr().PuidL( objectHandle );
            SendDataL( iMTPTypeUint128 );
            }
            break;

        // Name and DataAdded (audio only) fall under the same branch while dateadded(video)/modified/created fall under another
        case EMTPObjectPropCodeName: // 0xDC44
        case EMTPObjectPropCodeDateAdded: // 0xDC4E
        case EMTPObjectPropCodeAlbumArtist:
            {
            iMTPTypeString = CMTPTypeString::NewL();
            ServiceMetaDataFromWrapperL( propCode, *iMTPTypeString, *iObjectInfo );
            }
            break;
        //lint -fallthrough
        case EMTPObjectPropCodeDateCreated:
        case EMTPObjectPropCodeDateModified:
            {
            TBuf<KMtpMaxDateTimeStringLength> timeStr;
            MmMtpDpUtility::GetObjectDateModifiedL( iFramework.Fs(), suid, timeStr );

            PRINT1( _L( "MM MTP <> CGetObjectPropValue::ServiceL Date time %S" ), &timeStr );
            iMTPTypeString = CMTPTypeString::NewL( timeStr );
            SendDataL( *iMTPTypeString );
            }
            break;

        // Consumable
        case EMTPObjectPropCodeNonConsumable:
            {
            iMTPTypeUint8.Set( iObjectInfo->Uint( CMTPObjectMetaData::ENonConsumable ) );
            SendDataL( iMTPTypeUint8 );
            }
            break;

        default:
            {
            ServiceSpecificObjectPropertyL( propCode );
            }
            break;
        }
    PRINT( _L( "MM MTP <= CGetObjectPropValue::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropValue::ServiceMetaDataFromWrapperL
// Get MetaData from CMetadataAccessWrapper and send data
// or response to initiator
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropValue::ServiceMetaDataFromWrapperL( const TUint16 aPropCode,
    MMTPType& aNewData,
    const CMTPObjectMetaData& aObjectMetaData )
    {
    TRAPD( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( aPropCode,
        aNewData,
        aObjectMetaData ) );

    PRINT1( _L( "MM MTP <> CGetObjectPropValue::ServiceMetaDataFromWrapperL err = %d" ), err );

    if ( err == KErrNone )
        {
        SendDataL( aNewData );
        }
    // according to the codes of S60
    else if ( err == KErrTooBig )
        {
        SendResponseL( EMTPRespCodeInvalidDataset );
        }
    else if( err == KErrPermissionDenied )
        {
        SendResponseL( EMTPRespCodeAccessDenied );
        }
    else if ( err == KErrNotFound )
        {
        if( MmMtpDpUtility::HasMetadata( aObjectMetaData.Uint( CMTPObjectMetaData::EFormatCode ) ) )
            SendResponseL( EMTPRespCodeAccessDenied );
        else
            SendDataL( aNewData );
        }
    // PTP/MTP error code, according to the codes of S60
    else if ( err > KErrNone )
        {
        SendResponseL( static_cast<TMTPResponseCode>( err ) );
        }
    else
        {
        SendResponseL( EMTPRespCodeGeneralError );
        }
    }

// end of file
