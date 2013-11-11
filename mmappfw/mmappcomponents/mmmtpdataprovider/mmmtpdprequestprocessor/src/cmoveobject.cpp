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
* Description:  Implement the operation: MoveObject
*
*/


#include <bautils.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/mmtpstoragemgr.h>
#include <mtp/cmtptypearray.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/mmtpreferencemgr.h>

#include "cmoveobject.h"
#include "mmmtpdplogger.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdpconfig.h"
#include "cpropertysettingutility.h"

/**
* Verification data for the MoveObject request
*/
const TMTPRequestElementInfo KMTPMoveObjectPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrFile | EMTPElementAttrWrite,
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
// CMoveObject::~CMoveObject
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMoveObject::~CMoveObject()
    {
    Cancel();

    delete iDest;

    if ( iPropertyElement )
        delete iPropertyElement;

    delete iPropertyList;
    }

// -----------------------------------------------------------------------------
// CMoveObject::CMoveObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMoveObject::CMoveObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework,
            aConnection,
            sizeof( KMTPMoveObjectPolicy ) / sizeof( TMTPRequestElementInfo ),
            KMTPMoveObjectPolicy ),
        iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: MoveObject(0x1019)" ) );
    }

// -----------------------------------------------------------------------------
// CMoveObject::ConstructL
// Second phase constructor
// -----------------------------------------------------------------------------
//
EXPORT_C void CMoveObject::ConstructL()
    {
    iPropertyList = CMTPTypeObjectPropList::NewL();
    }

// -----------------------------------------------------------------------------
// CMoveObject::ServiceL
// MoveObject request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CMoveObject::ServiceL()
    {
    PRINT( _L( "MM MTP => CMoveObject::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);
    MoveObjectL();

    PRINT( _L( "MM MTP <= CMoveObject::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CMoveObject::MoveObjectL
// Move object operation
// -----------------------------------------------------------------------------
//
void CMoveObject::MoveObjectL()
    {
    PRINT( _L( "MM MTP => CMoveObject::MoveObjectL" ) );
    TMTPResponseCode responseCode = EMTPRespCodeOK;

    GetParametersL();

    RBuf newObjectName;
    newObjectName.CreateL( KMaxFileName );
    newObjectName.CleanupClosePushL(); // + newObjectName
    newObjectName = *iDest;

    TPtrC suid( iObjectInfo->DesC( CMTPObjectMetaData::ESuid ) );
    TParsePtrC fileNameParser( suid );
    if ( ( newObjectName.Length() + fileNameParser.NameAndExt().Length() )
        <= newObjectName.MaxLength() )
        {
        newObjectName.Append( fileNameParser.NameAndExt() );
        responseCode = CanMoveObjectL( suid, newObjectName );

        if ( responseCode == EMTPRespCodeOK )
            MoveFileL( newObjectName );
        }
    else
        // Destination is not appropriate for the full path name shouldn't be longer than 255
        responseCode = EMTPRespCodeInvalidDataset;

    SendResponseL( responseCode );

    CleanupStack::PopAndDestroy( &newObjectName ); // - newObjectName

    PRINT1( _L( "MM MTP <= CMoveObject::MoveObjectL responseCode = 0x%x" ), responseCode );
    }

// -----------------------------------------------------------------------------
// CMoveObject::GetParametersL
// Retrieve the parameters of the request
// -----------------------------------------------------------------------------
//
void CMoveObject::GetParametersL()
    {
    PRINT( _L( "MM MTP => CMoveObject::GetParametersL" ) );

    __ASSERT_DEBUG( iRequestChecker, Panic( EMmMTPDpRequestCheckNull ) );

    TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    iStorageId = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    iNewParentHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter3 );
    PRINT3( _L( "MM MTP <> objectHandle = 0x%x, iStorageId = 0x%x, iNewParentHandle = 0x%x" ),
        objectHandle,
        iStorageId,
        iNewParentHandle );

    // not taking owernship
    iObjectInfo = iRequestChecker->GetObjectInfo( objectHandle );
    __ASSERT_DEBUG( iObjectInfo, Panic( EMmMTPDpObjectNull ) );

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
    PRINT( _L( "MM MTP <= CMoveObject::GetParametersL" ) );
    }

// -----------------------------------------------------------------------------
// CMoveObject::SetDefaultParentObjectL
// Get a default parent object, ff the request does not specify a parent object,
// -----------------------------------------------------------------------------
//
void CMoveObject::SetDefaultParentObjectL()
    {
    PRINT( _L( "MM MTP => CMoveObject::SetDefaultParentObjectL" ) );

    delete iDest;
    iDest = NULL;
    iDest = iFramework.StorageMgr().StorageL( iStorageId ).DesC( CMTPStorageMetaData::EStorageSuid ).AllocL();
    iNewParentHandle = KMTPHandleNoParent;

    PRINT1( _L( "MM MTP <= CMoveObject::SetDefaultParentObjectL, iDest = %S" ), iDest );
    }

// -----------------------------------------------------------------------------
// CMoveObject::CanMoveObjectL
// Check if we can move the file to the new location
// -----------------------------------------------------------------------------
//
TMTPResponseCode CMoveObject::CanMoveObjectL( const TDesC& aOldName,
    const TDesC& aNewName ) const
    {
    PRINT2( _L( "MM MTP => CMoveObject::CanMoveObjectL aOldName = %S, aNewName = %S" ),
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
        PRINT1( _L( "MM MTP <> CMoveObject::CanMoveObjectL delErr = %d" ), delErr );
        // delete from the metadata DB
        TRAPD( err, iFramework.ObjectMgr().RemoveObjectL( aNewName ) );
        PRINT1( _L( "MM MTP <> CMoveObject::CanMoveObjectL err = %d" ), err );
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

    PRINT1( _L( "MM MTP <= CMoveObject::CanMoveObjectL result = 0x%x" ), result );
    return result;
    }

// -----------------------------------------------------------------------------
// CMoveObject::MoveFileL
// A helper function of MoveObjectL
// -----------------------------------------------------------------------------
//
void CMoveObject::MoveFileL( const TDesC& aNewFileName )
    {
    HBufC* oldFileName = iObjectInfo->DesC( CMTPObjectMetaData::ESuid ).AllocLC(); // + oldFileName
    PRINT2( _L( "MM MTP => CMoveObject::MoveFileL old name = %S, aNewFileName = %S" ),
        oldFileName,
        &aNewFileName );

    if ( iStorageId == iObjectInfo->Uint( CMTPObjectMetaData::EStorageId ) )
        iSameStorage = ETrue;
    else
        iSameStorage = EFalse;

    // Move the file first no matter if it will fail in Get/SetPreviousPropertiesL
    // Already trapped inside
    GetPreviousPropertiesL( *iObjectInfo );
    TRAP_IGNORE( SetPropertiesL( aNewFileName ) );

    CFileMan* fileMan = CFileMan::NewL( iFramework.Fs() );
    CleanupStack::PushL( fileMan ); // + fileMan
    TInt err = fileMan->Move( *oldFileName, aNewFileName );
    PRINT1( _L( "MM MTP <> CMoveObject::MoveFileL err = %d" ), err );
    User::LeaveIfError( err );
    CleanupStack::PopAndDestroy( fileMan ); // - fileMan
    CleanupStack::PopAndDestroy( oldFileName );     // - oldFileName

    PRINT( _L( "MM MTP <= CMoveObject::MoveFileL" ) );
    }

// -----------------------------------------------------------------------------
// CMoveObject::GetPreviousPropertiesL
// Save the object properties before doing the move
// -----------------------------------------------------------------------------
//
void CMoveObject::GetPreviousPropertiesL( const CMTPObjectMetaData& aObject )
    {
    PRINT( _L( "MM MTP => CMoveObject::GetPreviousPropertiesL" ) );

    // same storage, not necessary to get the properties
    if ( iSameStorage )
        return;

    TUint formatCode = aObject.Uint( CMTPObjectMetaData::EFormatCode );
    const RArray<TUint>* properties = iDpConfig.GetSupportedPropertiesL( formatCode );
    TInt count = properties->Count();
    TInt err = KErrNone;
    TUint16 propCode;
    TUint32 handle = aObject.Uint( CMTPObjectMetaData::EHandle );

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
                    aObject.Uint( CMTPObjectMetaData::ENonConsumable ) );
                break;

            case EMTPObjectPropCodeName:
            case EMTPObjectPropCodeDateAdded:
            case EMTPObjectPropCodeAlbumArtist:
                {
                CMTPTypeString* textData = CMTPTypeString::NewLC(); // + textData

                TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( propCode,
                    *textData,
                    aObject ) );
                PRINT1( _L( "MM MTP <> CMoveObject::GetPreviousPropertiesL err = %d" ), err );

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
                ServiceGetSpecificObjectPropertyL( propCode, handle, aObject );
                }
                break;
            }

        if ( iPropertyElement != NULL )
            {
            iPropertyList->CommitPropElemL( *iPropertyElement );
            iPropertyElement = NULL;
            }
        } // end of for loop

    PRINT1( _L( "MM MTP <= CMoveObject::GetPreviousPropertiesL err = %d" ), err );
    }

// -----------------------------------------------------------------------------
// CMoveObject::SetPreviousPropertiesL
// Set the object properties after doing the move
// -----------------------------------------------------------------------------
//
void CMoveObject::SetPreviousPropertiesL()
    {
    PRINT( _L( "MM MTP => CMoveObject::SetPreviousPropertiesL" ) );

    iPropertyList->ResetCursor();
    const TInt count = iPropertyList->NumberOfElements();
    for ( TInt i = 0; i < count; i++ )
        {
        CMTPTypeObjectPropListElement& element = iPropertyList->GetNextElementL();

        TUint32 handle = element.Uint32L( CMTPTypeObjectPropListElement::EObjectHandle );
        TUint16 propertyCode = element.Uint16L( CMTPTypeObjectPropListElement::EPropertyCode );
        TUint16 dataType = element.Uint16L( CMTPTypeObjectPropListElement::EDatatype );
        PRINT3( _L( "MM MTP <> CMoveObject::SetPreviousPropertiesL = 0x%x, propertyCode = 0x%x, dataType = 0x%x" ),
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
                iObjectInfo->SetUint( CMTPObjectMetaData::ENonConsumable,
                    element.Uint8L( CMTPTypeObjectPropListElement::EValue ) );
                iFramework.ObjectMgr().ModifyObjectL( *iObjectInfo );
                break;

            case EMTPObjectPropCodeName:
            case EMTPObjectPropCodeAlbumArtist:
                {
                CMTPTypeString *stringData = CMTPTypeString::NewLC( element.StringL( CMTPTypeObjectPropListElement::EValue ) ); // + stringData

                iDpConfig.PropSettingUtility()->SetMetaDataToWrapper( iDpConfig,
                    propertyCode,
                    *stringData,
                    *iObjectInfo );

                CleanupStack::PopAndDestroy( stringData ); // - stringData
                }
                break;

            default:
                {
                iDpConfig.PropSettingUtility()->SetSpecificObjectPropertyL( iDpConfig,
                    propertyCode,
                    *iObjectInfo,
                    element );
                }
                break;
            }
        } // end of for loop

    PRINT( _L( "MM MTP <= CMoveObject::SetPreviousPropertiesL" ) );
    }

// -----------------------------------------------------------------------------
// CMoveObject::SetPropertiesL
// Set the object properties in the object property store.
// -----------------------------------------------------------------------------
//
void CMoveObject::SetPropertiesL( const TDesC& aNewFileName )
    {
    PRINT1( _L( "MM MTP => CMoveObject::SetPropertiesL, aNewFileName = %S" ), &aNewFileName );

    TUint32 formatCode = iObjectInfo->Uint( CMTPObjectMetaData::EFormatCode );
    if ( formatCode == EMTPFormatCodeAbstractAudioVideoPlaylist )
        {
        // This is used to keep the same behavior in mass storage and device file manager.
        PRINT( _L( "MM MTP <> CMoveObject::SetPropertiesL Playlist file do not update the MPX DB" ) );
        iDpConfig.GetWrapperL().DeleteDummyFile( iObjectInfo->DesC( CMTPObjectMetaData::ESuid ) );
        iDpConfig.GetWrapperL().AddDummyFileL( aNewFileName );
        }

    if (iSameStorage)
        {
        iDpConfig.GetWrapperL().RenameObjectL( *iObjectInfo, aNewFileName );
        iObjectInfo->SetDesCL( CMTPObjectMetaData::ESuid, aNewFileName );
        iObjectInfo->SetUint( CMTPObjectMetaData::EStorageId, iStorageId );
        iObjectInfo->SetUint( CMTPObjectMetaData::EParentHandle, iNewParentHandle );
        iFramework.ObjectMgr().ModifyObjectL( *iObjectInfo );
        }
    else // if the two object in different storage, we should delete the old one and insert new one
        {
        iDpConfig.GetWrapperL().DeleteObjectL( *iObjectInfo );

        HBufC* oldFileName = iObjectInfo->DesC(CMTPObjectMetaData::ESuid).AllocLC(); // + oldFileName
        iObjectInfo->SetDesCL( CMTPObjectMetaData::ESuid, aNewFileName );
        iObjectInfo->SetUint( CMTPObjectMetaData::EStorageId, iStorageId );
        iObjectInfo->SetUint( CMTPObjectMetaData::EParentHandle, iNewParentHandle );
        iFramework.ObjectMgr().ModifyObjectL(*iObjectInfo);

        iDpConfig.GetWrapperL().AddObjectL( *iObjectInfo );

        if ( formatCode == EMTPFormatCodeAbstractAudioVideoPlaylist
            || formatCode == EMTPFormatCodeAbstractAudioAlbum )
            {
            MMTPReferenceMgr& referenceMgr = iFramework.ReferenceMgr();
            CDesCArray* references = referenceMgr.ReferencesLC( aNewFileName ); // + references
            iDpConfig.GetWrapperL().SetReferenceL( *iObjectInfo, *references );
            CleanupStack::PopAndDestroy( references ); // - references
            }
        CleanupStack::PopAndDestroy( oldFileName );     // - oldFileName

        // Only leave when getting proplist element from data received by fw.
        // It should not happen after ReceiveDataL in which construction of proplist already succeed.
        SetPreviousPropertiesL();
        }

    PRINT( _L( "MM MTP <= CMoveObject::SetPropertiesL" ) );
    }

// end of file
