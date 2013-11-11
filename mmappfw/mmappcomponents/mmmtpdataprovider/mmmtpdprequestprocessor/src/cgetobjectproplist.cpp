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
* Description:  Implement the operation: GetObjectPropList
*
*/


#include <bautils.h>
#include <e32std.h>

#include <mtp/mmtpobjectmgr.h>
#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>

#include "tobjectdescription.h"
#include "cgetobjectproplist.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"

static const TUint32 KMTPAll = 0xffffffff;


// Verification data for the getObjectPropList request
const TMTPRequestElementInfo KMTPGetObjectPropListPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrNone,
        2,
        KMTPHandleAll,
        KMTPHandleNone
        }
    };

// -----------------------------------------------------------------------------
// CGetObjectPropList::~CGetObjectPropList
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectPropList::~CGetObjectPropList()
    {
    Cancel();

    delete iObject;
    delete iPropertyList;
    delete iPropertyElement;
    iHandles.Close();
    iPropertyArray.Close();

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    delete iPerfLog;
#endif
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::CGetObjectPropList
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectPropList::CGetObjectPropList( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework,
            aConnection,
            sizeof ( KMTPGetObjectPropListPolicy ) / sizeof( TMTPRequestElementInfo ),
            KMTPGetObjectPropListPolicy ),
        iHandles ( KMmMtpRArrayGranularity ),
        iDpConfig( aDpConfig ),
        iPropertyArray( KMmMtpRArrayGranularity )
    {
    PRINT( _L( "Operation: GetObjectPropList(0x9805)" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::ConstructL
// 2nd Phase Constructor
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropList::ConstructL()
    {
    CActiveScheduler::Add( this );

    iPropertyList = CMTPTypeObjectPropList::NewL();

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    iPerfLog = CMmMtpDpPerfLog::NewL( _L( "MediaMtpDataProviderEnumerator" ) );
#endif
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::CheckRequestL
// Verify the reqeust and returns it
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CGetObjectPropList::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CGetObjectPropList::CheckRequestL" ) );

    TMTPResponseCode result = CRequestProcessor::CheckRequestL();

    if ( result == EMTPRespCodeOK )
        {
        result = CheckFormatL();
        }

    if ( result == EMTPRespCodeOK )
        {
        result = CheckDepth();
        }

    if ( result == EMTPRespCodeOK )
        {
        result = CheckPropCodeL();
        }

    PRINT1( _L("MM MTP <= CGetObjectPropList::CheckRequestL, response 0x%x"), result );

    return result;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::ServiceL
// GetObjectPropList request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropList::ServiceL()
    {
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);
    
    GetObjectHandlesL();
    TInt numOfObjects = iHandles.Count();
    PRINT2( _L( "MM MTP <> CGetObjectPropList::ServiceL, numOfObjects = %d, iPropCode = 0x%x" ),
        numOfObjects,
        iPropCode );
    if ( numOfObjects > 0
        && ( iPropCode == KMTPAll || iPropCode == EMTPObjectPropCodeUndefined ) )
        {
        // iFirstUnprocessed = 0;
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, iStatus.Int() );
        SetActive();
        }
    else
        {
        TInt err = KErrNone;
        for ( TInt i = 0; i < numOfObjects; i++ )
            {
            TUint32 handle = iHandles[i];

            if ( iFramework.ObjectMgr().ObjectOwnerId( handle )
                == iFramework.DataProviderId() )
                {
                delete iObject;
                iObject = NULL;
                iObject = CMTPObjectMetaData::NewL();

                if ( iFramework.ObjectMgr().ObjectL( handle, *iObject ) ) // Populate the object meta data
                    {
                    err = ServiceOneObjectPropertyL( handle, iPropCode );
                    if ( err != KErrNone && err != KErrNotSupported )
                        break;
                    }
                }
            }
        PRINT1( _L( "MM MTP <> CGetObjectPropList::ServiceL, one property was queried, Send data to PC! err = %d" ), err );
        if ( err == KErrNone
            || ( err == KErrNotSupported && 0 < iPropertyList->NumberOfElements() ) )
            // Make sure the dataset which is returned to pc is valid
            SendDataL( *iPropertyList );
        else if ( err == KErrNotFound || err == KErrNotSupported )
            // The object entry is not in db
            // or the ONLY required metadata is not in db.
            // The second case is the same with GetObjectPropValue and device has nothing to return.
            SendResponseL( EMTPRespCodeAccessDenied );
        else
            SendResponseL( EMTPRespCodeGeneralError );
        }

    PRINT( _L( "MM MTP <= CGetObjectPropList::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::CheckFormatL
// Ensure the object format operation parameter is valid
// -----------------------------------------------------------------------------
//
TMTPResponseCode CGetObjectPropList::CheckFormatL() const
    {
    TMTPResponseCode response = EMTPRespCodeOK;

    PRINT( _L( "MM MTP => CGetObjectPropList::CheckFormatL" ) );

    TUint32 formatCode = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    iFormatCode = formatCode;

    PRINT1( _L( "MM MTP <> CGetObjectPropList::CheckFormatL formatCode = 0x%x" ), formatCode );

    if ( formatCode != 0 )
        {
        response = EMTPRespCodeInvalidObjectFormatCode;

        const RArray<TUint>* format = iDpConfig.GetSupportedFormat();
        TInt count = format->Count();

        for ( TInt i = 0; i < count; i++ )
            {
            if ( formatCode == (*format)[i] )
                {
                response = EMTPRespCodeOK;
                break;
                }
            }
        }

    PRINT1( _L( "MM MTP <= CGetObjectPropList::CheckFormatL, response 0x%x" ), response );

    return response;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::CheckPropCodeL
// Ensure the object prop code operation parameter is valid
// -----------------------------------------------------------------------------
//
TMTPResponseCode CGetObjectPropList::CheckPropCodeL() const
    {
    PRINT( _L( "MM MTP => CGetObjectPropList::CheckPropCode" ) );

    TMTPResponseCode response = EMTPRespCodeOK;

    TUint32 formatCode = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    iPropCode = Request().Uint32( TMTPTypeRequest::ERequestParameter3 );
    PRINT1( _L( "MM MTP <> CGetObjectPropList::CheckPropCode, iPropCode = 0x%x" ), iPropCode );

    iPropertyArray.Reset();
    if ( iPropCode == EMTPObjectPropCodeUndefined )
        {
        // Get group code
        TUint32 groupCode = Request().Uint32( TMTPTypeRequest::ERequestParameter4 );
        PRINT1( _L( "MM MTP <> CGetObjectPropList::CheckPropCode, Group Code = 0x%x" ), groupCode );

        // check if groupCode is supported
        TInt count = sizeof ( KSupportedGroupCode ) / sizeof ( KSupportedGroupCode[0] );
        TInt i = 0;
        for ( ; i < count; i++ )
            {
            if ( groupCode == KSupportedGroupCode[i] )
                break;
            }
        if ( i == count )
            response = EMTPRespCodeSpecificationByGroupUnsupported;

        }
    else if ( iPropCode != KMTPAll )
        {
        PRINT1( _L( "MM MTP <> CGetObjectPropList::CheckPropCode, Property(0x%x) was queried." ), iPropCode );

        const RArray<TUint>* properties = NULL;
        if ( formatCode == KMTPFormatsAll )
            properties = iDpConfig.GetAllSupportedProperties();
        else
            properties = iDpConfig.GetSupportedPropertiesL( formatCode );

        const TInt count = properties->Count();
        TInt i = 0;
        for ( i = 0; i < count; i++ )
            {
            if ( (*properties)[i] == iPropCode )
                {
                iPropertyArray.Append( iPropCode );
                break;
                }
            }

        if ( iPropertyArray.Count() == 0 )
            {
            response = EMTPRespCodeInvalidObjectPropCode;
            }
        }
    else
        {
        PRINT( _L( "MM MTP <> CGetObjectPropList::CheckPropCode, All properties were queried." ) );
        }

    PRINT1( _L( "MM MTP <= CGetObjectPropList::CheckPropCode, response 0x%x" ), response );

    return response;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::CheckDepth
// Ensure the depth operation parameter is valid
// -----------------------------------------------------------------------------
//
TMTPResponseCode CGetObjectPropList::CheckDepth() const
    {
    TMTPResponseCode response = EMTPRespCodeSpecificationByDepthUnsupported;

    PRINT( _L( "MM MTP => CGetObjectPropList::CheckDepth" ) );

    // We either support no depth at all, or 1 depth or (0xFFFFFFFF) with objecthandle as 0x00000000
    iHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    PRINT1( _L( "MM MTP <> CGetObjectPropList::CheckDepth iHandle = 0x%x" ), iHandle );
    iDepth = Request().Uint32( TMTPTypeRequest::ERequestParameter5 );
    PRINT1( _L( "MM MTP <> CGetObjectPropList::CheckDepth iDepth = 0x%x" ), iDepth );

    if ( ( iDepth == 0 )
        || ( iDepth == 1 )
        || ( iDepth == KMTPAll && iHandle == KMTPHandleNone ) )
        {
        response = EMTPRespCodeOK;
        }

    PRINT1( _L( "MM MTP <= CGetObjectPropList::CheckDepth, response 0x%x" ), response );

    return response;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::GetObjectHandlesL
// Get the handles for the objects that we want the properties for
// -----------------------------------------------------------------------------
//
void CGetObjectPropList::GetObjectHandlesL()
    {
    PRINT( _L( "MM MTP => CGetObjectPropList::GetObjectHandlesL" ) );

    iHandles.Reset();

    if ( iHandle == KMTPHandleAll )
        {
        GetObjectHandlesL( KMTPStorageAll, KMTPHandleNone );
        }
    else if ( iHandle == KMTPHandleNone )
        {
        GetRootObjectHandlesL( iDepth, iFormatCode );
        }
    else
        {
        CMTPObjectMetaData* meta( iRequestChecker->GetObjectInfo( iHandle ) );

        __ASSERT_DEBUG( meta, Panic( EMmMTPDpObjectNull ) );

        if ( ( meta->Uint( CMTPObjectMetaData::EFormatCode ) == EMTPFormatCodeAssociation )
            && ( meta->Uint( CMTPObjectMetaData::EFormatSubCode ) == EMTPAssociationTypeGenericFolder ) )
            {
            GetFolderObjectHandlesL( iDepth, iHandle );
            }
        else
            {
            iHandles.Append( iHandle );
            }
        }

    PRINT( _L( "MM MTP <= CGetObjectPropList::GetObjectHandlesL" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::GetObjectHandlesL
// Get all object handles (for GetObjectHandlesL)
// -----------------------------------------------------------------------------
//
void CGetObjectPropList::GetObjectHandlesL( TUint32 aStorageId,
    TUint32 aParentHandle,
    TUint16 aFormat /*= 0x0000*/ )
    {
    PRINT2( _L( "MM MTP => CGetObjectPropList::GetObjectHandlesL, aStorageId = 0x%x, aParentHandle = 0x%x" ),
        aStorageId,
        aParentHandle );

    RMTPObjectMgrQueryContext context;
    CleanupClosePushL( context ); // + context

    iHandles.Reset();

    if ( aFormat )
        {
        TMTPObjectMgrQueryParams params( aStorageId, aFormat, aParentHandle );

        do
            {
            iFramework.ObjectMgr().GetObjectHandlesL( params, context, iHandles );
            }
        while ( !context.QueryComplete() );
        }
    else
        {
        TMTPObjectMgrQueryParams params( aStorageId,
            KMTPFormatsAll,
            aParentHandle );

        do
            {
            RArray<TUint> handles;
            CleanupClosePushL( handles ); // + handles
            iFramework.ObjectMgr().GetObjectHandlesL( params, context, handles );

            for ( TInt i = 0; i < handles.Count(); i++ )
                {
                if ( iFramework.ObjectMgr().ObjectOwnerId( handles[i ] )
                    == iFramework.DataProviderId() )
                    iHandles.Append( handles[i ] );
                }
            CleanupStack::PopAndDestroy( &handles ); // - handles
            }
        while ( !context.QueryComplete() );
        }

    CleanupStack::PopAndDestroy( &context ); // - context

    PRINT( _L( "MM MTP <= CGetObjectPropList::GetObjectHandlesL" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::GetFolderObjectHandlesL
// Gets a folder object handle
// -----------------------------------------------------------------------------
//
void CGetObjectPropList::GetFolderObjectHandlesL( TUint32 aDepth,
    TUint32 aParentHandle )
    {
    PRINT2( _L( "MM MTP => CGetObjectPropList::GetFolderObjectHandlesL aDepth = 0x%x, aParentHandle = 0x%x" ),
        aDepth,
        aParentHandle );

    if ( aDepth == 0 )
        {
        // Do Nothing!!!
        }
    else //if(aDepth ==1)
        {
        GetObjectHandlesL( KMTPStorageAll, aParentHandle, iFormatCode );
        }

    PRINT( _L( "MM MTP <= CGetObjectPropList::GetFolderObjectHandlesL" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::GetRootObjectHandlesL
// Gets the handles of all objects on the root
// -----------------------------------------------------------------------------
//
void CGetObjectPropList::GetRootObjectHandlesL( TUint32 aDepth, TUint16 aFormat )
    {
    PRINT2( _L( "MM MTP => CGetObjectPropList::GetRootObjectHandlesL aDepth = 0x%x, aFormat = 0x%x" ), aDepth , aFormat);

    if ( aDepth == KMaxTUint )
        {
        GetObjectHandlesL( KMTPStorageAll, KMTPHandleNone, aFormat );
        }
    else if ( aDepth == 0 )
        {
        // Return empty set to PC according to spec 0.96, folder is handled by device dp
        iHandles.Reset();
        }
    else
        {
        GetObjectHandlesL( KMTPStorageAll, KMTPHandleNoParent, aFormat );
        }

    PRINT( _L( "MM MTP <= CGetObjectPropList::GetRootObjectHandlesL" ) );
    }
// -----------------------------------------------------------------------------
// CGetObjectPropList::ServiceAllPropertiesL
//  Gets a list of all the properties supported
// -----------------------------------------------------------------------------
//
TInt CGetObjectPropList::ServiceAllPropertiesL( TUint32 aHandle )
    {
    PRINT1( _L( "MM MTP => CGetObjectPropList::ServiceAllPropertiesL aHandle = 0x%x" ), aHandle );

    // Append the current object info onto our list
    TPtrC fileName( iObject->DesC( CMTPObjectMetaData::ESuid ) );
    TUint32 formatCode = iObject->Uint( CMTPObjectMetaData::EFormatCode );

    PRINT2( _L( "MM MTP <> CGetObjectPropList::ServiceAllPropertiesL, fileName = %S, formatCode = 0x%x" ),
        &fileName,
        formatCode );

    const RArray<TUint>* properties = NULL;
    if ( formatCode == KMTPFormatsAll )
        properties = iDpConfig.GetAllSupportedProperties();
    else
        properties = iDpConfig.GetSupportedPropertiesL( formatCode );

    const TInt count = properties->Count();
    for ( TInt i = 0; i < count; i++ )
        {
        ServiceOneObjectPropertyL( aHandle, (*properties)[i] );
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::ServiceGroupPropertiesL
// Get the grouped object properties for specific object
// -----------------------------------------------------------------------------
//
TInt CGetObjectPropList::ServiceGroupPropertiesL( TUint32 aHandle )
    {
    PRINT1( _L( "MM MTP => CGetObjectPropList::ServiceGroupPropertiesL aHandle = 0x%x" ), aHandle );

    TPtrC fileName( iObject->DesC( CMTPObjectMetaData::ESuid ) );
    TUint32 formatCode = iObject->Uint( CMTPObjectMetaData::EFormatCode );

    PRINT2( _L( "MM MTP <> CGetObjectPropList::ServiceGroupPropertiesL, fileName = %S, formatCode = 0x%x" ),
        &fileName,
        formatCode );

    // Get property codes according to group code, put those into array
    TUint32 groupCode = Request().Uint32( TMTPTypeRequest::ERequestParameter4 );
    iPropertyArray.Reset();
    GetPropertiesL( iPropertyArray, groupCode, formatCode );

    const TInt count = iPropertyArray.Count();

    TInt err = KErrNone;
    for ( TInt i = 0; i < count; i++ )
        {
        err = ServiceOneObjectPropertyL( aHandle, iPropertyArray[i] );
        if ( err == KErrNotSupported || err == KErrNotFound )  // Skip
            err = KErrNone;
        if ( err != KErrNone )
            break;
        }

    PRINT1( _L( "MM MTP <= CGetObjectPropList::ServiceGroupPropertiesL err = %d" ), err );

    return err;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::ServiceOneObjectPropertyL
//  Get the object property information for the required object
// -----------------------------------------------------------------------------
//
TInt CGetObjectPropList::ServiceOneObjectPropertyL( TUint32 aHandle,
    TUint16 aPropCode )
    {
    PRINT2( _L( "MM MTP => CGetObjectPropList::ServiceOneObjectPropertyL aHandle = 0x%x, aPropCode = 0x%x" ),
        aHandle,
        aPropCode );
    CMTPTypeString* textData = NULL;
    TInt err = KErrNone;

    if ( iPropertyElement != NULL )
        {
        delete iPropertyElement;
        iPropertyElement = NULL;
        }

    switch ( aPropCode )
        {
        // Storage ID
        case EMTPObjectPropCodeStorageID:
            {
            TMTPTypeUint32 storageId( iObject->Uint( CMTPObjectMetaData::EStorageId ) );
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint32L( CMTPTypeObjectPropListElement::EValue, storageId.Value() );
            }
            break;

        // Format Code
        case EMTPObjectPropCodeObjectFormat:
            {
            TMTPTypeUint16 objectFormat( iObject->Uint( CMTPObjectMetaData::EFormatCode ) );
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint16L( CMTPTypeObjectPropListElement::EValue, objectFormat.Value() );
            }
            break;

        // Protection Status
        case EMTPObjectPropCodeProtectionStatus:
            {
            TMTPTypeUint16 protectionStatus( MmMtpDpUtility::GetProtectionStatusL( iFramework.Fs(),
                iObject->DesC( CMTPObjectMetaData::ESuid ) ) );
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint16L( CMTPTypeObjectPropListElement::EValue,  protectionStatus.Value() );
            }
            break;

        // Object Size
        case EMTPObjectPropCodeObjectSize:
            {
            TMTPTypeUint64 objectSize( MmMtpDpUtility::GetObjectSizeL( iFramework.Fs(),
                iObject->DesC( CMTPObjectMetaData::ESuid ) ) );
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint64L( CMTPTypeObjectPropListElement::EValue, objectSize.Value() );
            }
            break;

        // Filename
        case EMTPObjectPropCodeObjectFileName:
            {
            TParsePtrC parse( iObject->DesC( CMTPObjectMetaData::ESuid ) );
            PRINT1( _L( "MM MTP <> CGetObjectPropList::ServiceOneObjectPropertyL FileName = %S" ), &parse.FullName() );
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode) );
            iPropertyElement->SetStringL( CMTPTypeObjectPropListElement::EValue, parse.NameAndExt() );
            }
            break;

        // Parent Object
        case EMTPObjectPropCodeParentObject:
            {
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            TUint32 parentHandle = iObject->Uint( CMTPObjectMetaData::EParentHandle );
            // refer to 5.3.1.9 of MTP Spec 1.0
            if ( parentHandle == KMTPHandleNoParent )
                {
                parentHandle = KMTPHandleNone;
                }
            iPropertyElement->SetUint32L( CMTPTypeObjectPropListElement::EValue, parentHandle );
            }
            break;

        // PUID
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
            {
            TMTPTypeUint128 puid = iFramework.ObjectMgr().PuidL( aHandle );
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint128L( CMTPTypeObjectPropListElement::EValue,puid.UpperValue(), puid.LowerValue() );
            }
            break;

        case EMTPObjectPropCodeName:
        case EMTPObjectPropCodeDateAdded:
        case EMTPObjectPropCodeAlbumArtist:
            {
            textData = CMTPTypeString::NewLC(); // + textData

            TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( aPropCode,
                *textData,
                *iObject ) );

            PRINT1( _L( "MM MTP <> CGetObjectPropList::ServiceOneObjectPropertyL err = %d" ), err );

            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetStringL( CMTPTypeObjectPropListElement::EValue, textData->StringChars() );

            CleanupStack::PopAndDestroy( textData ); // - textData
            }
            break;

        case EMTPObjectPropCodeDateCreated:
        case EMTPObjectPropCodeDateModified:
            {
            TBuf<KMtpMaxDateTimeStringLength> timeStr;
            MmMtpDpUtility::GetObjectDateModifiedL( iFramework.Fs(),
                iObject->DesC( CMTPObjectMetaData::ESuid ),
                timeStr );
            PRINT1( _L( "MM MTP <> CGetObjectPropList::ServiceOneObjectPropertyL Date time %S" ), &timeStr );

            CMTPTypeString* datastring = CMTPTypeString::NewLC( timeStr );  // + datastring
            iPropertyElement = &(iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetStringL( CMTPTypeObjectPropListElement::EValue, datastring->StringChars() );
            CleanupStack::PopAndDestroy( datastring );  // - datastring
            }
            break;

        // Consumable Value
        case EMTPObjectPropCodeNonConsumable:
            {
            iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
            iPropertyElement->SetUint8L( CMTPTypeObjectPropListElement::EValue,
                iObject->Uint( CMTPObjectMetaData::ENonConsumable ) );
            }
            break;

        default:
            {
            // "err == KErrNotFound" means the object entry does not exist in DB,
            // "err == KErrNotSupported" means the entry is there but this metadata not.
            err = ServiceSpecificObjectPropertyL( aPropCode, aHandle );
            }
            break;
        }

    if ( iPropertyElement != NULL )
        {
        iPropertyList->CommitPropElemL( *iPropertyElement );
        iPropertyElement = NULL;
        }

    PRINT( _L( "MM MTP <= CGetObjectPropList::ServiceOneObjectPropertyL" ) );

    return err;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::RunL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropList::RunL()
    {
    if ( iHandles.Count() > 0 )
        {
        TInt err = KErrNone;
        TUint32 handle = iHandles[0];
        PRINT1( _L( "MM MTP <> CGetObjectPropList::RunL, handle = 0x%x" ), handle );
        if ( iFramework.ObjectMgr().ObjectOwnerId( handle )
            == iFramework.DataProviderId() )
            {
            delete iObject;
            iObject = NULL;
            iObject = CMTPObjectMetaData::NewL();

            if ( iFramework.ObjectMgr().ObjectL( handle, *iObject ) ) // Populate the object meta data
                {
                if ( iPropCode == KMTPAll )
                    err = ServiceAllPropertiesL( handle );
                else if ( iPropCode == EMTPObjectPropCodeUndefined )
                    {
                    err = ServiceGroupPropertiesL( handle );
                    }
                if ( err == KErrNotFound )
                    SendResponseL( EMTPRespCodeAccessDenied );
                else if ( err != KErrNone )
                    SendResponseL( EMTPRespCodeGeneralError );
                }
            }

        // Complete ourselves with current TRequestStatus
        // Increase index to process next handle on next round
        // iFirstUnprocessed++;
        iHandles.Remove( 0 );

        if ( err == KErrNone )
            {
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, iStatus.Int() );
            SetActive();
            }
        }
    else // all handles processed, can send data
        {
        PRINT1( _L( "MM MTP <> CGetObjectPropList::RunL, Finished, Send data to PC!, iPropertyList->NumberOfElements() = %d" ),
            iPropertyList->NumberOfElements() );
        SendDataL( *iPropertyList );
        }
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::RunError
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CGetObjectPropList::RunError( TInt aError )
    {
    if ( aError != KErrNone )
        PRINT1( _L( "MM MTP <> CGetObjectPropList::RunError aError = %d" ), aError );

    TRAP_IGNORE( SendResponseL( EMTPRespCodeGeneralError ) );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::DoCancel()
// Cancel the process
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropList::DoCancel()
    {
    }

void CGetObjectPropList::GetPropertiesL( RArray<TUint>& aPropArray,
    TUint32 aGroupCode,
    TUint16 aFormatCode ) const
    {
    aPropArray.Reset();

    const RArray<TUint>* properties = NULL;
    if ( aFormatCode == KMTPFormatsAll )
        properties = iDpConfig.GetAllSupportedProperties();
    else
        properties = iDpConfig.GetSupportedPropertiesL( aFormatCode );

    TInt count = sizeof ( KPropGroupMapTable ) / sizeof ( KPropGroupMapTable[0] );
    for ( TInt i = 0; i < properties->Count(); i++ )
        {
        for ( TInt j = 0; j < count; j++ )
            {
            if( KPropGroupMapTable[j].iPropCode == ( *properties )[i]
                && KPropGroupMapTable[j].iGroupCode == aGroupCode )
                {
                aPropArray.Append( (*properties)[i] );
                break;
                }
            }
        }
    }

// end of file
