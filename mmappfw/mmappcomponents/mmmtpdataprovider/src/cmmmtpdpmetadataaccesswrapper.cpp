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
* Description:  Meta data access wrapper
*
*/


#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtpobjectmetadata.h>
#include <bautils.h>
#include <e32math.h>
#include <e32property.h>
#include <mtpprivatepskeys.h>

#include "mmmtpdp_variant.hrh"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "cmmmtpdpmetadatampxaccess.h"
#include "cmmmtpdpmetadatavideoaccess.h"
#include "mmmtpdputility.h"
#include "tobjectdescription.h"
#include "mmmtpdplogger.h"

const TInt KMmMtpDummyFileArrayGranularity = 5;

CMmMtpDpMetadataAccessWrapper* CMmMtpDpMetadataAccessWrapper::NewL( MMTPDataProviderFramework& aFramework )
    {
    CMmMtpDpMetadataAccessWrapper* me = new (ELeave) CMmMtpDpMetadataAccessWrapper( aFramework );
    CleanupStack::PushL( me );
    me->ConstructL();
    CleanupStack::Pop( me );

    return me;
    }

CMmMtpDpMetadataAccessWrapper::CMmMtpDpMetadataAccessWrapper( MMTPDataProviderFramework& aFramework ) :
    iFramework( aFramework ),
    iFs( aFramework.Fs() ),
    iOpenCount( 0 )
    {
    // Do nothing
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::ConstructL
// Second-phase
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataAccessWrapper::ConstructL()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::ConstructL" ) );

    iMmMtpDpMetadataMpxAccess = CMmMtpDpMetadataMpxAccess::NewL( iFs );

    iMmMtpDpMetadataVideoAccess = CMmMtpDpMetadataVideoAccess::NewL( iFs );

    iAbstractMediaArray = new ( ELeave ) CDesCArrayFlat( KMmMtpDummyFileArrayGranularity );

    // Create the PS key to notify subscribers that MTP mode is activated
    _LIT_SECURITY_POLICY_C1(KKeyReadPolicy, ECapabilityReadUserData);
    _LIT_SECURITY_POLICY_C1(KKeyWritePolicy, ECapabilityWriteUserData);
    RProperty::Define( KMtpPSUid,
                       KMtpPSStatus,
                       RProperty::EInt,
                       KKeyReadPolicy,
                       KKeyWritePolicy);

#ifdef MMMTPDP_RESET_STATUS_CLOSESESSION
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusUninitialized);
#else
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusReadyToSync);
#endif // MMMTPDP_RESET_STATUS_CLOSESESSION    
    
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::ConstructL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::~CMmMtpDpMetadataAccessWrapper
// Destructor
// ---------------------------------------------------------------------------
//
CMmMtpDpMetadataAccessWrapper::~CMmMtpDpMetadataAccessWrapper()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::~CMmMtpDpMetadataAccessWrapper" ) );
    RemoveDummyFiles();

    delete iAbstractMediaArray;

    delete iMmMtpDpMetadataVideoAccess;
    delete iMmMtpDpMetadataMpxAccess;

    // unblock MPX
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusUninitialized);
    
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::~CMmMtpDpMetadataAccessWrapper" ) );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::SetReferenceL
// Set abstract media to DB
// -----------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::SetReferenceL( const CMTPObjectMetaData& aObject,
    CDesCArray& aRefFileArray )
    {
    TPtrC refOwner( aObject.DesC( CMTPObjectMetaData::ESuid ) );
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::SetReferenceL reference owner = %S" ),
        &refOwner );

    TMPXGeneralCategory category = Category( aObject );
    if ( category == EMPXPlaylist || category == EMPXAbstractAlbum )
        {
        iMmMtpDpMetadataMpxAccess->SetReferenceL( refOwner, aRefFileArray, category );
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::SetReferenceL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::GetObjectMetadataValueL
// Gets a piece of metadata from the collection
// ---------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::GetObjectMetadataValueL( const TUint16 aPropCode,
    MMTPType& aNewData,
    const CMTPObjectMetaData& aObjectMetaData )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::GetObjectMetadataValueL" ) );

    TMPXGeneralCategory category = Category( aObjectMetaData );
    switch ( category )
        {
        case EMPXAbstractAlbum:
            if ( aPropCode == EMTPObjectPropCodeDateAdded && EMTPTypeString == aNewData.Type() )
                {
                TBuf<KMtpMaxDateTimeStringLength> timeStr;
                MmMtpDpUtility::GetObjectDateModifiedL( iFs,
                    aObjectMetaData.DesC( CMTPObjectMetaData::ESuid ),
                    timeStr );
                PRINT1( _L( "MM MTP <> CGetObjectPropList::ServiceOneObjectPropertyL Date time %S" ), &timeStr );

                ( ( CMTPTypeString & ) aNewData ).SetL( timeStr );
                break;
                }
            // else
            // Fall through on purpoe to get from mpx
        case EMPXPlaylist:
        case EMPXSong:
            {
            iMmMtpDpMetadataMpxAccess->GetObjectMetadataValueL( aPropCode,
                aNewData,
                aObjectMetaData.DesC( CMTPObjectMetaData::ESuid ),
                category );
            }
            break;
        case EMPXVideo:
            if ( aPropCode == EMTPObjectPropCodeDateAdded && EMTPTypeString == aNewData.Type() )
                {
                TBuf<KMtpMaxDateTimeStringLength> timeStr;
                MmMtpDpUtility::GetObjectDateModifiedL( iFs,
                    aObjectMetaData.DesC( CMTPObjectMetaData::ESuid ),
                    timeStr );
                PRINT1( _L( "MM MTP <> CGetObjectPropList::ServiceOneObjectPropertyL Date time %S" ), &timeStr );

                ( ( CMTPTypeString & ) aNewData ).SetL( timeStr );
                }
            else
                {
                iMmMtpDpMetadataVideoAccess->GetObjectMetadataValueL( aPropCode,
                    aNewData,
                    aObjectMetaData );
                }
            break;
        default:
            // do nothing
            break;
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::GetObjectMetadataValueL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::SetObjectMetadataValueL
// Sets a piece of metadata in the collection
// ---------------------------------------------------------------------------
//
void CMmMtpDpMetadataAccessWrapper::SetObjectMetadataValueL( const TUint16 aPropCode,
    const MMTPType& aNewData,
    const CMTPObjectMetaData& aObjectMetaData )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::SetObjectMetadataValueL" ) );

    TMPXGeneralCategory category = Category( aObjectMetaData );
    switch ( category )
        {
        case EMPXAbstractAlbum:
        case EMPXSong:
        case EMPXPlaylist:
            iMmMtpDpMetadataMpxAccess->SetObjectMetadataValueL( aPropCode,
                aNewData,
                aObjectMetaData.DesC( CMTPObjectMetaData::ESuid ),
                category );
            break;
        case EMPXVideo:
            iMmMtpDpMetadataVideoAccess->SetObjectMetadataValueL( aPropCode,
                aNewData,
                aObjectMetaData );
            break;
        default:
            break;
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::SetObjectMetadataValueL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::RenameObjectL
// Renames the file part of a record in the collection database
// ---------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::RenameObjectL( const CMTPObjectMetaData& aOldObject,
    const TDesC& aNewFileName )
    {
    TPtrC oldFileName( aOldObject.DesC( CMTPObjectMetaData::ESuid ) );
    PRINT2( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::RenameObjectL old = %S, new = %S" ),
        &oldFileName,
        &aNewFileName );

    TMPXGeneralCategory category = Category( aOldObject );
    switch ( category )
        {
        case EMPXSong:
        case EMPXAbstractAlbum:
        case EMPXPlaylist:
            {
            iMmMtpDpMetadataMpxAccess->RenameObjectL( oldFileName,
                aNewFileName,
                category );
            }
            break;
        case EMPXVideo:
            iMmMtpDpMetadataVideoAccess->RenameRecordL( oldFileName, aNewFileName );
            break;
        default:
            break;
        }


    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::RenameObjectL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::DeleteObjectL
// Deletes metadata information associated with the object
// ---------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::DeleteObjectL( const CMTPObjectMetaData& aObject )
    {
    TPtrC fileName( aObject.DesC( CMTPObjectMetaData::ESuid ) );
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::DeleteObjectL name = %S" ), &fileName );

    TMPXGeneralCategory category = Category( aObject );

    switch ( category )
        {
        case EMPXSong:
        case EMPXAbstractAlbum:
        case EMPXPlaylist:
            {
            iMmMtpDpMetadataMpxAccess->DeleteObjectL( fileName, category );
            }
            break;

        case EMPXVideo:
            {
            iMmMtpDpMetadataVideoAccess->DeleteRecordL( fileName );
            }
            break;

        default:
            PRINT( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::DeleteObjectL default" ) );
            break;
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::DeleteObjectL" ) );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::SetImageObjPropL
// set image specific properties specific to videos
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataAccessWrapper::SetImageObjPropL( const CMTPObjectMetaData& aObject,
    const TUint32 aWidth,
    const TUint32 aHeight )
    {
    TMPXGeneralCategory category = Category( aObject );
    if ( category == EMPXVideo )
        {
        TPtrC fullFileName( aObject.DesC( CMTPObjectMetaData::ESuid ) );
        iMmMtpDpMetadataVideoAccess->SetStorageRootL( fullFileName );
        iMmMtpDpMetadataVideoAccess->SetImageObjPropL( fullFileName,
            aWidth,
            aHeight );
        }
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataMpxAccess::GetImageObjPropL
// get image specific properties specific to videos
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataAccessWrapper::GetImageObjPropL( const CMTPObjectMetaData& aObject,
    TUint32& aWidth,
    TUint32& aHeight )
    {
    TMPXGeneralCategory category = Category( aObject );
    if ( category == EMPXVideo )
        {
        TPtrC fullFileName( aObject.DesC( CMTPObjectMetaData::ESuid ) );
        iMmMtpDpMetadataVideoAccess->SetStorageRootL( fullFileName );
        iMmMtpDpMetadataVideoAccess->GetImageObjPropL( fullFileName, aWidth, aHeight );
        }
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::OpenSessionL
// Called when the MTP session is initialised
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataAccessWrapper::OpenSessionL()
    {
    iOpenCount++;

#ifdef MMMTPDP_RESET_STATUS_CLOSESESSION
    if (iOpenCount == 1)
        MmMtpDpUtility::SetPSStatus(EMtpPSStatusReadyToSync);
#endif // MMMTPDP_RESET_STATUS_CLOSESESSION
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::CloseSessionL
//
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataAccessWrapper::CloseSessionL()
    {
    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::CloseSessionL count = %d" ), iOpenCount );

    if ( iOpenCount >= 1 )
        {
        iOpenCount--;

        if ( iOpenCount == 0 )
            {
            PRINT( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::CloseSessionL close" ) );
            iMmMtpDpMetadataMpxAccess->CloseSession();    // trigger close and flush CollectionHelper
            iMmMtpDpMetadataVideoAccess->CloseSessionL();
            
#ifdef MMMTPDP_RESET_STATUS_CLOSESESSION
            MmMtpDpUtility::SetPSStatus(EMtpPSStatusUninitialized);
#endif // MMMTPDP_RESET_STATUS_CLOSESESSION
            }
        }
    else
        {
        PRINT( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::CloseSessionL already close" ) );
        }
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::Category
// Get category according format code
// ---------------------------------------------------------------------------
//
TMPXGeneralCategory CMmMtpDpMetadataAccessWrapper::Category( const CMTPObjectMetaData& aObject )
    {
    TMPXGeneralCategory category = EMPXNoCategory;
    TUint formatCode = aObject.Uint( CMTPObjectMetaData::EFormatCode );
    switch ( formatCode )
        {
        case EMTPFormatCodeASF:
        case EMTPFormatCodeMP4Container:
        case EMTPFormatCode3GPContainer:
            {
            TUint aSubFormatCode = aObject.Uint( CMTPObjectMetaData::EFormatSubCode );
            if ( aSubFormatCode == EMTPSubFormatCodeUnknown )
                {
                TRAP_IGNORE( category = ContainerCategoryL( aObject.DesC( CMTPObjectMetaData::ESuid ) ) );
                if ( category == EMPXSong )
                    aSubFormatCode = EMTPSubFormatCodeAudio;
                else if ( category == EMPXVideo )
                    aSubFormatCode = EMTPSubFormatCodeVideo;
                else
                    aSubFormatCode = EMTPSubFormatCodeUndefined;
                const_cast<CMTPObjectMetaData&>(aObject).SetUint( CMTPObjectMetaData::EFormatSubCode, aSubFormatCode );
                // If object doesn't exist, do nothing
                TRAP_IGNORE( iFramework.ObjectMgr().ModifyObjectL( aObject ) );
                }
            else if ( aSubFormatCode == EMTPSubFormatCodeAudio )
                category = EMPXSong;
            else if ( aSubFormatCode == EMTPSubFormatCodeVideo )
                category = EMPXVideo;
            else if( aSubFormatCode == EMTPSubFormatCodeUndefined )
                category = EMPXOther;
            }
            break;

        case EMTPFormatCodeMP3:
        case EMTPFormatCodeWAV:
        case EMTPFormatCodeWMA:
        case EMTPFormatCodeAAC:
            {
            category = EMPXSong;
            }
            break;

        case EMTPFormatCodeM3UPlaylist:
        case EMTPFormatCodeAbstractAudioVideoPlaylist:
            {
            category = EMPXPlaylist;
            }
            break;

        case EMTPFormatCodeAbstractAudioAlbum:
            {
            category = EMPXAbstractAlbum;
            }
            break;

        case EMTPFormatCodeWMV:
            {
            category = EMPXVideo;
            }
            break;

        default:
            break;
        }
    PRINT1( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::Category category = %d" ), category );
    return category;
    }

TMPXGeneralCategory CMmMtpDpMetadataAccessWrapper::ContainerCategoryL( const TDesC& aFullFileName )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::ContainerCategoryL aFullFileName = %S" ), &aFullFileName );

    TMPXGeneralCategory category = EMPXNoCategory;
    TParsePtrC pathParser( aFullFileName );
    TPtrC ext( pathParser.Ext() );

    if ( ext.Length() <= 0 )
        category = EMPXOther;
    else if ( ext.CompareF( KTxtExtensionO4A ) == 0
        || ext.CompareF( KTxtExtensionM4A ) == 0 )
        category = EMPXSong;
    else if ( ext.CompareF( KTxtExtensionO4V ) == 0 )
        category = EMPXVideo;
    else
        {
        HBufC8* mimetype = NULL;

        if ( ext.CompareF( KTxtExtensionMP4 ) == 0
            || ext.CompareF( KTxtExtension3GP ) == 0
            || ext.CompareF( KTxtExtension3G2 ) == 0 )
            mimetype = MmMtpDpUtility::Mp4MimeTypeL( aFullFileName );
        else if ( ext.CompareF( KTxtExtensionODF ) == 0 )
            mimetype = MmMtpDpUtility::OdfMimeTypeL( aFullFileName );
        else if ( ext.CompareF( KTxtExtensionASF ) == 0 )
            mimetype = MmMtpDpUtility::AsfMimeTypeL( aFullFileName );
        if ( mimetype != NULL )
            {
            TMmMtpSubFormatCode subFormatCode =
                MmMtpDpUtility::SubFormatCodeFromMime( *mimetype );

            if ( subFormatCode == EMTPSubFormatCodeVideo )
                category = EMPXVideo;
            else if( subFormatCode == EMTPSubFormatCodeAudio )
                category = EMPXSong;
            else
                category = EMPXOther;
            }
        }

    PRINT1( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::ContainerCategoryL, category = %d" ), category );
    return category;
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::GetAllAbstractMediaL
//
// ---------------------------------------------------------------------------
EXPORT_C void CMmMtpDpMetadataAccessWrapper::GetAllAbstractMediaL( const TDesC& aStoreRoot, CDesCArray& aAbstractMedias, TMPXGeneralCategory aCategory )
    {
    iMmMtpDpMetadataMpxAccess->GetAllAbstractMediaL( aStoreRoot, aAbstractMedias, aCategory );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::GetAllReferenceL
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::GetAllReferenceL( const CMTPObjectMetaData& aRefOwner, CDesCArray& aReferences )
    {
    TMPXGeneralCategory category = Category( aRefOwner );
    if ( category == EMPXPlaylist || category == EMPXAbstractAlbum )
        iMmMtpDpMetadataMpxAccess->GetAllReferenceL( aRefOwner.DesC( CMTPObjectMetaData::ESuid ), category, aReferences );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::AddObjectL
// Add object (music, video, playlist and abstract media) info to DB
// -----------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::AddObjectL( const CMTPObjectMetaData& aObject )
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::AddObjectL" ) );
    TMPXGeneralCategory category = Category( aObject );

    TPtrC fullFileName( aObject.DesC( CMTPObjectMetaData::ESuid ) );
    if ( category == EMPXVideo )
        {
        PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::AddObjectL Addvideo" ) );
        iMmMtpDpMetadataVideoAccess->AddVideoL( fullFileName );
        }
    else if ( category == EMPXPlaylist || category == EMPXAbstractAlbum )
        {
        PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::AddObjectL AddPlaylist/AbstractAlbum" ) );
        iMmMtpDpMetadataMpxAccess->AddAbstractMediaL( fullFileName,
            category );
        }
    else if ( category == EMPXSong )
        {
        PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::AddObjectL AddSong" ) );
        iMmMtpDpMetadataMpxAccess->AddSongL( fullFileName );
        }

    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::AddObjectL" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::GetModifiedContentL
// Get Modified content
// ---------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::GetModifiedContentL( const TDesC& aStorageRoot,
    TInt& arrayCount,
    CDesCArray& aModifiedcontent )
    {
    iMmMtpDpMetadataMpxAccess->SetStorageRootL( aStorageRoot );
    iMmMtpDpMetadataMpxAccess->GetModifiedContentL( arrayCount, aModifiedcontent );
    }

EXPORT_C void CMmMtpDpMetadataAccessWrapper::CleanupDatabaseL()
    {
    iMmMtpDpMetadataVideoAccess->CleanupDatabaseL();
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::IsExistL
// if the playlsit exist in the MPX DB
// ---------------------------------------------------------------------------
//
TBool CMmMtpDpMetadataAccessWrapper::IsExistL( const TDesC& aSuid )
    {
    return iMmMtpDpMetadataMpxAccess->IsExistL( aSuid );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::AddDummyFile
// Add one dummy file to dummy files array
// -----------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::AddDummyFileL( const TDesC& aDummyFileName )
    {
    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::AddDummyFile aDummyFileName(%S)" ), &aDummyFileName );
    iAbstractMediaArray->AppendL( aDummyFileName );
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::DeleteDummyFile
// Delete one dummy file from dummy files array
// -----------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::DeleteDummyFile( const TDesC& aDummyFileName )
    {
    PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::DeleteDummyFile aDummyFileName(%S)" ), &aDummyFileName );
    TInt pos = 0;
    if ( iAbstractMediaArray->Count() > 0 )
        {
        if ( 0 == iAbstractMediaArray->Find( aDummyFileName, pos ) )
            {
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::DeleteDummyFile pos = %d" ), pos );
            iAbstractMediaArray->Delete( pos );
            }
        }
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::CreateDummyFile
// Create a Dummy File from the virtual playlist URI
// -----------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::CreateDummyFile( const TDesC& aPlaylistName )
    {
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::CreateDummyFile, filename = %S" ), &aPlaylistName );

    RFile newfile;
    TInt err = newfile.Replace( iFs, aPlaylistName, EFileWrite );

    if ( err != KErrNone )
        {
        newfile.Close();
        PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::CreateDummyFile err =  %d" ), err );
        }
    else // File created OK
        {
        err = newfile.Flush();
        newfile.Close();
        err = iFs.SetAtt( aPlaylistName, KEntryAttSystem | KEntryAttHidden,
            KEntryAttReadOnly | KEntryAttNormal );
        if ( err != KErrNone )
            PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::CreateDummyFile Dummy Playlist file created. err = %d" ), err );
        }
    }

// -----------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::RemoveDummyFiles
// Remove all dummy file of which format is "pla" and "alb", and leave the "m3u"
// -----------------------------------------------------------------------------
//
void CMmMtpDpMetadataAccessWrapper::RemoveDummyFiles()
    {
    PRINT( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::RemoveDummyFiles" ) );

    TInt count = iAbstractMediaArray->Count();
    PRINT1( _L( "MM MTP => CMmMtpDpMetadataAccessWrapper::RemoveDummyFiles, count = %d" ), count );
    // Check if playlist file is a dummy file or an imported file
    for ( TInt i = 0; i < count; i++ )
        {
        TPtrC fileName( (*iAbstractMediaArray)[i] );
        PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::RemoveDummyFiles, fileName = %S" ), &fileName );
        
        TMTPFormatCode format = MmMtpDpUtility::FormatFromFilename( fileName );
        PRINT1( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::RemoveDummyFiles, format = 0x%x" ), format );
        if ( format == EMTPFormatCodeAbstractAudioAlbum )
            {
            // delete the abstract album if its size is zero
            TEntry entry;
            TInt err = iFs.Entry( fileName, entry );
            if ( err == KErrNone && entry.iSize == 0 )
                {
                TRAP( err, iMmMtpDpMetadataMpxAccess->DeleteObjectL( fileName, EMPXAbstractAlbum ) );
                if( err == KErrNone )
                    {
                    err = iFs.Delete( fileName );
                    }
                }
            PRINT3( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::RemoveDummyFile filename = %S, err %d, entry.iSize = %d" ),
                &fileName, err, entry.iSize );
            }
        else
            {
            TInt err = iFs.Delete( fileName );

            PRINT2( _L( "MM MTP <> CMmMtpDpMetadataAccessWrapper::RemoveDummyFile filename = %S, err %d" ),
                &fileName,
                err );
            }
        }
    PRINT( _L( "MM MTP <= CMmMtpDpMetadataAccessWrapper::RemoveDummyFiles" ) );
    }

// ---------------------------------------------------------------------------
// CMmMtpDpMetadataAccessWrapper::UpdateMusicCollectionL
// Update Music collection
// ---------------------------------------------------------------------------
//
EXPORT_C void CMmMtpDpMetadataAccessWrapper::UpdateMusicCollectionL( const TDesC& aStorageRoot )
    {
    iMmMtpDpMetadataMpxAccess->SetStorageRootL( aStorageRoot );
    iMmMtpDpMetadataMpxAccess->UpdateMusicCollectionL( );
    }

// end of file
