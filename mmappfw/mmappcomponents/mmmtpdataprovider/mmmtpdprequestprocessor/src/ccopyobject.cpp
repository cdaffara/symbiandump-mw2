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
 * Description:  Implement the operation: CopyObject
 *
 */

#include <bautils.h>

#include <mtp/mmtpobjectmgr.h>
#include <mtp/mmtpreferencemgr.h>
#include <mtp/mmtpstoragemgr.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/cmtptypeobjectproplist.h>

#include "ccopyobject.h"
#include "mmmtpdplogger.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdputility.h"
#include "mmmtpdpconfig.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "cpropertysettingutility.h"

/**
 * Verification data for the CopyObject request
 */
const TMTPRequestElementInfo KMTPCopyObjectPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrFile,
        0,
        0,
        0
        },
        {
        TMTPTypeRequest::ERequestParameter2,
        EMTPElementTypeStorageId,
        EMTPElementAttrWrite,
        0,
        0,
        0
        },
        {
        TMTPTypeRequest::ERequestParameter3,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrDir | EMTPElementAttrWrite,
        1,
        0,
        0
        }
    };

// -----------------------------------------------------------------------------
// CCopyObject::~CCopyObject
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CCopyObject::~CCopyObject()
    {
    Cancel();

    delete iTargetObject;
    delete iDest;

    if ( iPropertyElement )
        delete iPropertyElement;
    delete iPropertyList;
    }

// -----------------------------------------------------------------------------
// CCopyObject::CCopyObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CCopyObject::CCopyObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework,
            aConnection, 
            sizeof( KMTPCopyObjectPolicy ) / sizeof( TMTPRequestElementInfo ),
            KMTPCopyObjectPolicy ),
        iDpConfig( aDpConfig ),
        iSourceObject( NULL ),
        iTargetObject( NULL )
    {
    PRINT( _L( "Operation: CopyObject(0x101A)" ) );
    }

// -----------------------------------------------------------------------------
// CCopyObject::ConstructL
// Second phase constructor
// -----------------------------------------------------------------------------
//
EXPORT_C void CCopyObject::ConstructL()
    {
    iPropertyList = CMTPTypeObjectPropList::NewL();
    }

// -----------------------------------------------------------------------------
// CCopyObject::ServiceL
// CopyObject request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CCopyObject::ServiceL()
    {
    PRINT( _L( "MM MTP => CCopyObject::ServiceL" ) );

    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);
    CopyObjectL();

    PRINT( _L( "MM MTP <= CCopyObject::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CCopyObject::CopyObjectL
// Copy object operation
// -----------------------------------------------------------------------------
//
void CCopyObject::CopyObjectL()
    {
    PRINT( _L( "MM MTP => CCopyObject::CopyObjectL" ) );

    TMTPResponseCode responseCode = EMTPRespCodeOK;

    GetParametersL();

    RBuf newObjectName;
    newObjectName.CreateL( KMaxFileName );
    newObjectName.CleanupClosePushL(); // + newObjectName
    newObjectName = *iDest;

    const TDesC& suid( iSourceObject->DesC( CMTPObjectMetaData::ESuid ) );
    TParsePtrC fileNameParser( suid );
    if ( ( newObjectName.Length() + fileNameParser.NameAndExt().Length() )
        <= newObjectName.MaxLength() )
        {
        newObjectName.Append( fileNameParser.NameAndExt() );
        responseCode = CanCopyObjectL( suid, newObjectName );

        TUint32 newHandle = KMTPHandleNone;
        if ( responseCode == EMTPRespCodeOK )
            {
            newHandle = CopyFileL( newObjectName );
            SendResponseL( EMTPRespCodeOK, 1, &newHandle );
            PRINT2( _L( "MM MTP <= CCopyObject::CopyObjectL responseCode = 0x%x, aNewHandle = 0x%x" ),
                responseCode,
                newHandle );
            }
        else
            SendResponseL( responseCode );
        }
    else
        // Destination is not appropriate for the full path name shouldn't be longer than 255
        SendResponseL( EMTPRespCodeInvalidDataset );

    CleanupStack::PopAndDestroy( &newObjectName ); // - newObjectName
    }

// -----------------------------------------------------------------------------
// CCopyObject::GetParametersL
// Retrieve the parameters of the request
// -----------------------------------------------------------------------------
//
void CCopyObject::GetParametersL()
    {
    PRINT( _L( "MM MTP => CCopyObject::GetParametersL" ) );

    __ASSERT_DEBUG( iRequestChecker, Panic( EMmMTPDpRequestCheckNull ) );

    TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    iStorageId = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    iNewParentHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter3 );
    PRINT3( _L( "MM MTP <> CCopyObject::GetParametersL Object Handle = 0x%x, StorageId = 0x%x, Parent Handle = 0x%x" ),
        objectHandle,
        iStorageId,
        iNewParentHandle );

    // not taking owernship
    iSourceObject = iRequestChecker->GetObjectInfo( objectHandle );
    __ASSERT_DEBUG( iSourceObject, Panic( EMmMTPDpObjectNull ) );

    if ( iNewParentHandle == KMTPHandleNone )
        {
        SetDefaultParentObjectL();
        }
    else
        {
        CMTPObjectMetaData* parentObject = iRequestChecker->GetObjectInfo( iNewParentHandle );
        __ASSERT_DEBUG( parentObject, Panic( EMmMTPDpObjectNull ) );

        delete iDest;
        iDest = NULL;
        iDest = parentObject->DesC( CMTPObjectMetaData::ESuid ).AllocL();
        PRINT1( _L( "MM MTP <> CMoveObject::GetParametersL iDest = %S" ), iDest );
        }
    PRINT( _L( "MM MTP <= CCopyObject::GetParametersL" ) );
    }

// -----------------------------------------------------------------------------
// CCopyObject::SetDefaultParentObjectL
// Set a default destination, if the request does not specify a parent object,
// -----------------------------------------------------------------------------
//
void CCopyObject::SetDefaultParentObjectL()
    {
    PRINT( _L( "MM MTP => CCopyObject::SetDefaultParentObjectL" ) );

    delete iDest;
    iDest = NULL;
    iDest = iFramework.StorageMgr().StorageL( iStorageId ).DesC( CMTPStorageMetaData::EStorageSuid ).AllocL();

    iNewParentHandle = KMTPHandleNoParent;

    PRINT1( _L( "MM MTP <= CCopyObject::SetDefaultParentObjectL, iDest = %S" ), iDest );
    }

// -----------------------------------------------------------------------------
// CCopyObject::CanCopyObjectL
// Check if we can copy the file to the new location
// -----------------------------------------------------------------------------
//
TMTPResponseCode CCopyObject::CanCopyObjectL( const TDesC& aOldName,
    const TDesC& aNewName ) const
    {
    PRINT2( _L( "MM MTP => CCopyObject::CanCopyObjectL aOldName = %S, aNewName = %S" ),
        &aOldName,
        &aNewName );
    TMTPResponseCode result = EMTPRespCodeOK;

    TEntry fileEntry;
    User::LeaveIfError( iFramework.Fs().Entry( aOldName, fileEntry ) );
    TInt drive = iFramework.StorageMgr().DriveNumber( iStorageId );
    User::LeaveIfError( drive );
    TVolumeInfo volumeInfo;
    User::LeaveIfError( iFramework.Fs().Volume( volumeInfo, drive ) );

    if ( volumeInfo.iFree < fileEntry.FileSize() )
        {
        result = EMTPRespCodeStoreFull;
        }
    else if ( BaflUtils::FileExists( iFramework.Fs(), aNewName ) )
        {
#ifdef MMMTPDP_REPLACE_EXIST_FILE
        // delete the old one and replace
        TInt delErr = iFramework.Fs().Delete( aNewName );
        PRINT1( _L( "MM MTP <> CCopyObject::CanCopyObjectL delErr = %d" ), delErr );
        // delete from the metadata DB
        TRAPD( err, iFramework.ObjectMgr().RemoveObjectL( aNewName ) );
        PRINT1( _L( "MM MTP <> CCopyObject::CanCopyObjectL err = %d" ), err );
        // delete from video/mpx DB
        CMTPObjectMetaData* objectInfo = CMTPObjectMetaData::NewLC(); // + objectInfo
        if ( iFramework.ObjectMgr().ObjectL( aNewName, *objectInfo ) )
            {
            TRAP( err, iDpConfig.GetWrapperL().DeleteObjectL( aNewName,
                objectInfo->Uint( CMTPObjectMetaData::EFormatCode ) ) );
            }
        CleanupStack::PopAndDestroy( objectInfo ); // - objectInfo

        if ( err )
            {
            // do nothing
            }
#else
        result = EMTPRespCodeInvalidParentObject;
#endif
        }
    // This is used to keep the same behavior in mass storage and device file manager.
    else if ( iSourceObject->Uint( CMTPObjectMetaData::EFormatCode )
        == EMTPFormatCodeAbstractAudioVideoPlaylist )
        {
        PRINT( _L( "MM MTP <> CCopyObject::CanCopyObjectL playlist file can't copy" ) );
        result = EMTPRespCodeAccessDenied;
        }

    PRINT1( _L( "MM MTP <= CCopyObject::CanCopyObjectL result = 0x%x" ), result );
    return result;
    }

// -----------------------------------------------------------------------------
// CCopyObject::CopyFileL
// A helper function of CopyFileL
// -----------------------------------------------------------------------------
//
TUint32 CCopyObject::CopyFileL( const TDesC& aNewFileName )
    {
    PRINT( _L( "MM MTP => CCopyObject::CopyFileL" ) );

    GetPreviousPropertiesL();

    // TODO: Need rollback mechanism for consistant with image dp in fw.
    // Not sure if it should be trap if something wrong with MPX db.
    TPtrC oldFileName( iSourceObject->DesC( CMTPObjectMetaData::ESuid ) );
    TUint32 handle = AddObjectToStoreL( oldFileName, aNewFileName );

    // Only leave when getting proplist element from data received by fw.
    // It should not happen after ReceiveDataL in which construction of proplist already succeed.
    SetPreviousPropertiesL();

    CFileMan* fileMan = CFileMan::NewL( iFramework.Fs() );
    CleanupStack::PushL( fileMan ); // + fileMan
    User::LeaveIfError( fileMan->Copy( oldFileName, aNewFileName ) );
    CleanupStack::PopAndDestroy( fileMan ); // - fileMan

    PRINT( _L( "MM MTP <= CCopyObject::CopyFileL" ) );
    return handle;
    }

// -----------------------------------------------------------------------------
// CCopyObject::GetPreviousPropertiesL
// Save the object properties before doing the copy
// -----------------------------------------------------------------------------
//
void CCopyObject::GetPreviousPropertiesL()
    {
    PRINT( _L( "MM MTP => CCopyObject::GetPreviousPropertiesL" ) );

    TUint formatCode = iSourceObject->Uint( CMTPObjectMetaData::EFormatCode );
    const RArray<TUint>* properties = iDpConfig.GetSupportedPropertiesL( formatCode );
    TInt count = properties->Count();

    TInt err = KErrNone;
    TUint16 propCode;
    TUint32 handle = iSourceObject->Uint( CMTPObjectMetaData::EHandle );

    if ( iPropertyElement != NULL )
        {
        delete iPropertyElement;
        iPropertyElement = NULL;
        }

    for ( TInt i = 0; i < count; i++ )
        {
        propCode = ( *properties )[i];

        switch ( propCode )
            {
            case EMTPObjectPropCodeStorageID:
            case EMTPObjectPropCodeObjectFormat:
            case EMTPObjectPropCodeProtectionStatus:
            case EMTPObjectPropCodeObjectSize:
            case EMTPObjectPropCodeObjectFileName:
            case EMTPObjectPropCodeParentObject:
            case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
            case EMTPObjectPropCodeDateCreated:
            case EMTPObjectPropCodeDateModified:
                break;

            case EMTPObjectPropCodeNonConsumable:
                iPropertyElement = &( iPropertyList->ReservePropElemL( handle, propCode ) );
                iPropertyElement->SetUint8L( CMTPTypeObjectPropListElement::EValue,
                    iSourceObject->Uint( CMTPObjectMetaData::ENonConsumable ) );
                break;

            case EMTPObjectPropCodeName:
            case EMTPObjectPropCodeDateAdded:
            case EMTPObjectPropCodeAlbumArtist:
                {
                CMTPTypeString* textData = CMTPTypeString::NewLC(); // + textData

                TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( propCode,
                    *textData,
                    *iSourceObject ) );

                PRINT1( _L( "MM MTP <> CCopyObject::GetPreviousPropertiesL err = %d" ), err );

                if ( err == KErrNone )
                    {
                    iPropertyElement = &( iPropertyList->ReservePropElemL( handle, propCode ) );
                    iPropertyElement->SetStringL( CMTPTypeObjectPropListElement::EValue,
                        textData->StringChars() );
                    }
                else
                    {
                    iPropertyElement = NULL;
                    }

                CleanupStack::PopAndDestroy( textData ); // - textData
                }
                break;

            default:
                {
                ServiceGetSpecificObjectPropertyL( propCode, handle, *iSourceObject );
                }
                break;
            }

        if ( iPropertyElement != NULL )
            {
            iPropertyList->CommitPropElemL( *iPropertyElement );
            iPropertyElement = NULL;
            }
        } // end of for loop

    PRINT1( _L( "MM MTP <= CCopyObject::GetPreviousPropertiesL err = %d" ), err );
    }

// -----------------------------------------------------------------------------
// CCopyObject::SetPreviousPropertiesL
// Set the object properties after doing the copy
// -----------------------------------------------------------------------------
//
void CCopyObject::SetPreviousPropertiesL()
    {
    PRINT( _L( "MM MTP => CCopyObject::SetPreviousPropertiesL" ) );

    iPropertyList->ResetCursor();
    const TInt count = iPropertyList->NumberOfElements();
    for ( TInt i = 0; i < count; i++ )
        {
        CMTPTypeObjectPropListElement& element = iPropertyList->GetNextElementL();

        TUint32 handle = element.Uint32L( CMTPTypeObjectPropListElement::EObjectHandle );
        TUint16 propertyCode = element.Uint16L( CMTPTypeObjectPropListElement::EPropertyCode );
        TUint16 dataType = element.Uint16L( CMTPTypeObjectPropListElement::EDatatype );
        PRINT3( _L( "MM MTP <> CCopyObject::SetPreviousPropertiesL = 0x%x, propertyCode = 0x%x, dataType = 0x%x" ),
            handle,
            propertyCode,
            dataType );

        switch ( propertyCode )
            {
            case EMTPObjectPropCodeStorageID:
            case EMTPObjectPropCodeObjectFormat:
            case EMTPObjectPropCodeProtectionStatus:
            case EMTPObjectPropCodeObjectSize:
            case EMTPObjectPropCodeObjectFileName:
            case EMTPObjectPropCodeParentObject:
            case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
            case EMTPObjectPropCodeDateCreated:
            case EMTPObjectPropCodeDateModified:
            case EMTPObjectPropCodeDateAdded:
                break;

            case EMTPObjectPropCodeNonConsumable:
                iTargetObject->SetUint( CMTPObjectMetaData::ENonConsumable,
                    element.Uint8L( CMTPTypeObjectPropListElement::EValue ) );
                // should wait all property setting finished then insert object, or not?
                // need to investigate if it will affect performance result
                iFramework.ObjectMgr().ModifyObjectL( *iTargetObject );
                break;

            case EMTPObjectPropCodeName:
            case EMTPObjectPropCodeAlbumArtist:
                {
                CMTPTypeString *stringData = CMTPTypeString::NewLC( element.StringL( CMTPTypeObjectPropListElement::EValue ) ); // + stringData

                iDpConfig.PropSettingUtility()->SetMetaDataToWrapper( iDpConfig,
                    propertyCode,
                    *stringData,
                    *iTargetObject );

                CleanupStack::PopAndDestroy( stringData ); // - stringData
                }
                break;

            default:
                {
                iDpConfig.PropSettingUtility()->SetSpecificObjectPropertyL( iDpConfig,
                    propertyCode,
                    *iTargetObject,
                    element );
                }
                break;
            }
        } // end of for loop

    PRINT( _L( "MM MTP <= CCopyObject::SetPreviousPropertiesL" ) );
    }

// -----------------------------------------------------------------------------
// CCopyObject::UpdateObjectInfoL
// Update object info in the database
// -----------------------------------------------------------------------------
//
TUint32 CCopyObject::AddObjectToStoreL( const TDesC& aOldObjectName,
    const TDesC& aNewObjectName )
    {
    PRINT2( _L( "MM MTP => CCopyObject::AddObjectToStoreL aOldObjectName = %S, aNewObjectName = %S" ),
        &aOldObjectName,
        &aNewObjectName );

    iTargetObject = CMTPObjectMetaData::NewL();

    // 1. Add new object into objectMgr db
    iTargetObject->SetUint( CMTPObjectMetaData::EDataProviderId, iSourceObject->Uint( CMTPObjectMetaData::EDataProviderId ) );
    TUint formatCode = iSourceObject->Uint( CMTPObjectMetaData::EFormatCode );
    iTargetObject->SetUint( CMTPObjectMetaData::EFormatCode, formatCode );
    TUint subFormatCode = iSourceObject->Uint( CMTPObjectMetaData::EFormatSubCode );
    iTargetObject->SetUint( CMTPObjectMetaData::EFormatSubCode, subFormatCode );
    iTargetObject->SetUint( CMTPObjectMetaData::EParentHandle, iNewParentHandle );
    iTargetObject->SetUint( CMTPObjectMetaData::EStorageId, iStorageId );
    iTargetObject->SetDesCL( CMTPObjectMetaData::ESuid, aNewObjectName );
    iFramework.ObjectMgr().InsertObjectL( *iTargetObject );

    // 2. Add new object into MPX db
    iDpConfig.GetWrapperL().AddObjectL( *iTargetObject );

    // 3. Set references into references db
    if ( formatCode == EMTPFormatCodeM3UPlaylist )
        {
        MMTPReferenceMgr& referenceMgr = iFramework.ReferenceMgr();
        CDesCArray* references = referenceMgr.ReferencesLC( aOldObjectName ); // + references
        referenceMgr.SetReferencesL( aNewObjectName, *references );
        CleanupStack::PopAndDestroy( references ); // - references
        }

    TUint32 handle = iTargetObject->Uint( CMTPObjectMetaData::EHandle );

    PRINT1( _L( "MM MTP <= CCopyObject::AddObjectToStoreL handle = 0x%x" ), handle );
    return handle;
    }

// end of file
