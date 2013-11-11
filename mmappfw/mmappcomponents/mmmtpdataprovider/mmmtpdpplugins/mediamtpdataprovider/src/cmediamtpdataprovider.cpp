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
* Description:  media mtp data provider
*
*/

#include <mtp/mmtpconnection.h>
#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mmtpstoragemgr.h>
#include <mtp/cmtpobjectmetadata.h>
#include <mtp/mmtpobjectmgr.h>
#include <barsc.h>
#include <barsread.h>
#include <driveinfo.h>

#include "cmediamtpdataprovider.h"
#include "crequestprocessor.h"
#include "mediamtpdataproviderconst.h"
#include "cmediamtpdataproviderenumerator.h"
#include "mmmtpdplogger.h"
#include "tmmmtpdppanic.h"
#include "cmmmtpdpaccesssingleton.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdputility.h"
#include "crenameobject.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpfiledefs.h"
#include "cmediamtpdataproviderpropertysettingutility.h"
#include "cmediamtpdataproviderdescriptionutility.h"

// Class constants.
// Defines the number of MTP Active Processors allowed
static const TInt KMediaMtpDataProviderSessionGranularity = 3;
static const TInt KMediaMtpDpArrayGranularity = 2;

static const TInt KActiveEnumeration = 0;
_LIT( KMediaMtpDataProviderExtension1, "microsoft.com/WMPPD: 11.0" );
_LIT( KMediaMtpDataProviderExtension2, "vodafone.com/omadrmv2: 1.0" );

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::NewL
// Two Phase Construction
// -----------------------------------------------------------------------------
//
TAny* CMediaMtpDataProvider::NewL( TAny* aParams )
    {
    CMediaMtpDataProvider* self = new ( ELeave ) CMediaMtpDataProvider( aParams );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::CMediaMtpDataProvider
// Standard C++ constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProvider::CMediaMtpDataProvider( TAny* aParams ) :
    CMTPDataProviderPlugin( aParams ),
    iActiveProcessors( KMediaMtpDataProviderSessionGranularity ),
    iMediaEnumerator( NULL ),
    iPendingEnumerations ( KMediaMtpDpArrayGranularity ),
    iActiveProcessor( -1 ),
    iRenameObject( NULL ),
    iSupportedFormat( KMediaMtpDpArrayGranularity ),
    iSupportedPropAudio( KMediaMtpDpArrayGranularity ),
    iSupportedPropVideo( KMediaMtpDpArrayGranularity ),
    iSupportedPropAll( KMediaMtpDpArrayGranularity )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::~CMediaMtpDataProvider
// Destructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProvider::~CMediaMtpDataProvider()
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProvider::~CMediaMtpDataProvider" ) );

    CMmMtpDpAccessSingleton::Release();
    delete iMediaEnumerator;
    delete iPropSettingUtility;
    delete iDescriptionUtility;

    iPendingEnumerations.Close();
    TInt count = iActiveProcessors.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        iActiveProcessors[i]->Release();
        }
    iActiveProcessors.Close();

    if ( iRenameObject )
        delete iRenameObject;

    iSupportedFormat.Close();
    iSupportedPropAudio.Close();
    iSupportedPropVideo.Close();
    iSupportedPropAll.Close();

    PRINT( _L( "MM MTP <= CMediaMtpDataProvider::~CMediaMtpDataProvider" ) );
    }

void CMediaMtpDataProvider::Cancel()
    {
    iMediaEnumerator->Cancel();
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::ConstructL
// Second-phase construction
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::ConstructL()
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProvider::ConstructL" ) );

    iMediaEnumerator = CMediaMtpDataProviderEnumerator::NewL( Framework(), *this );

    CMmMtpDpAccessSingleton::CreateL( Framework() );

    GetSupportedFormatL();
    GetSupportedPropL();
    GetAllSupportedPropL();

    iPropSettingUtility = CMediaMtpDataProviderPropertySettingUtility::NewL();
    iDescriptionUtility = CMediaMtpDataProviderDescriptionUtility::NewL();

    PRINT( _L( "MM MTP <= CMediaMtpDataProvider::ConstructL" ) );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::ProcessEventL
// Process event from initiator
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::ProcessEventL( const TMTPTypeEvent& aEvent,
    MMTPConnection& aConnection )
    {
    TInt index = LocateRequestProcessorL( aEvent, aConnection );
    if ( index != KErrNotFound )
        {
        iActiveProcessors[index]->HandleEventL( aEvent );
        }
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::ProcessNotificationL
// Process notification from initiator
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::ProcessNotificationL( TMTPNotification aNotification,
    const TAny* aParams )
    {
    switch ( aNotification )
        {
        case EMTPSessionClosed:
            PRINT( _L( "MM MTP <> CMediaMtpDataProvider::ProcessNotificationL EMTPSessionClosed event recvd" ) );
            SessionClosedL( *reinterpret_cast<const TMTPNotificationParamsSessionChange*> ( aParams ) );
            break;

        case EMTPSessionOpened:
            PRINT( _L( "MM MTP <> CMediaMtpDataProvider::ProcessNotificationL EMTPSessionOpened event recvd" ) );
            SessionOpenedL( *reinterpret_cast<const TMTPNotificationParamsSessionChange*> ( aParams ) );
            break;

        case EMTPStorageAdded:
            PRINT( _L( "MM MTP <> CMediaMtpDataProvider::ProcessNotificationL EMTPStorageAdded event recvd" ) );
            break;

        case EMTPStorageRemoved:
            PRINT( _L( "MM MTP <> CMediaMtpDataProvider::ProcessNotificationL EMTPStorageRemoved event recvd" ) );
            break;

        case EMTPRenameObject:
            PRINT( _L( "MM MTP <> CMediaMtpDataProvider::ProcessNotificationL EMTPRenameObject event recvd" ) );
            RenameObjectL( *reinterpret_cast<const TMTPNotificationParamsHandle*> ( aParams ) );
            break;

        case EMTPObjectAdded:
            PRINT( _L( "MM MTP <> CMediaMtpDataProvider::ProcessNotificationL EMTPObjectAdded event recvd" ) );
            ObjectAddedL(*reinterpret_cast<const TUint32*>(aParams));
            break;

        default:
            PRINT( _L( "MM MTP <> CMediaMtpDataProvider::ProcessNotificationL default" ) );
            // Ignore all other notifications.
            break;
        }
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::ProcessRequestPhaseL
// Process the request from initiator
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::ProcessRequestPhaseL( TMTPTransactionPhase aPhase,
    const TMTPTypeRequest& aRequest,
    MMTPConnection& aConnection )
    {
    TInt index = LocateRequestProcessorL( aRequest, aConnection );

    __ASSERT_DEBUG( index != KErrNotFound, Panic(EMmMTPDpNoMatchingProcessor) );

    MMmRequestProcessor* processor = iActiveProcessors[index];
    iActiveProcessor = index;
    iActiveProcessorRemoved = EFalse;
    TBool result = processor->HandleRequestL( aRequest, aPhase );

    if ( !iIsSessionOpen )
        {
        processor->Release();
        }
    // iActiveProcessorRemoved will be set to ETrue in the above function
    // HandleRequestL(),such as SessionClose()
    else if ( iActiveProcessorRemoved )
        {
        processor->Release(); // destroy the processor
        }
    else if ( result ) // destroy the processor
        {
        processor->Release();
        iActiveProcessors.Remove( index );
        }
    iActiveProcessor = -1;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::SessionClosedL
// Notify the data provider that the session has been closed
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::SessionClosedL( const TMTPNotificationParamsSessionChange& aSession )
    {
    iIsSessionOpen = EFalse;
    TInt count = iActiveProcessors.Count();
    PRINT1( _L( "MM MTP => CMediaMtpDataProvider::SessionClosedL, total processor count = %d" ), count );
    while ( count-- )
        {
        MMmRequestProcessor* processor = iActiveProcessors[count];

        // replaced for the Request() is invalid sometimes
        // TUint32 sessionId( processor->Request().Uint32( TMTPTypeRequest::ERequestSessionID ) );
        TUint32 sessionId = processor->SessionId();

        if ( ( sessionId == aSession.iMTPId )
                && ( processor->Connection().ConnectionId()
                == aSession.iConnection.ConnectionId() ) )
            {
            processor->UsbDisconnect(); // Rollback

            iActiveProcessors.Remove( count );
            if ( count == iActiveProcessor )
                {
                iActiveProcessorRemoved = ETrue;
                iActiveProcessor = -1;  // update iActiveProcessor
                }
            else
                {
                processor->Release();
                }
            }
        }

    // introduce to cleanup DBs at each close session
    iMediaEnumerator->SessionClosedL();
    CMmMtpDpAccessSingleton::CloseSessionL();

    PRINT( _L( "MM MTP <= CMediaMtpDataProvider::SessionClosedL" ) );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::SessionOpenedL
// Open Session operation
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::SessionOpenedL( const TMTPNotificationParamsSessionChange& /*aSession*/)
    {
    CMmMtpDpAccessSingleton::OpenSessionL();
    iIsSessionOpen = ETrue;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::RenameObjectL
// Process the changed folder
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::RenameObjectL( const TMTPNotificationParamsHandle& aObject )
    {
    PRINT2( _L( "MM MTP => CMediaMtpDataProvider::RenameObjectL folder handle=0x%x, modified name=%S" ), aObject.iHandleId, &(aObject.iFileName) );

    if ( !iRenameObject )
        {
        iRenameObject = CRenameObject::NewL( Framework(), GetWrapperL() );
        }

    iRenameObject->StartL( aObject.iHandleId, aObject.iFileName );

    PRINT( _L( "MM MTP <= CMediaMtpDataProvider::RenameObjectL" ) );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::ObjectAddedL
// Process the added object
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::ObjectAddedL( TUint32 aObjectHandle )
    {
    PRINT1( _L( "MM MTP => CMediaMtpDataProvider::ObjectAddedL aHandle=0x%x" ), aObjectHandle );

    CMTPObjectMetaData* object(CMTPObjectMetaData::NewLC());
    Framework().ObjectMgr().ObjectL( aObjectHandle, *object );

    //Since the object's processor is not route to media dp, its format code should be reset
    TUint formatCode = MmMtpDpUtility::FormatFromFilename( object->DesC( CMTPObjectMetaData::ESuid ) );
    object->SetUint( CMTPObjectMetaData::EFormatCode, formatCode );
    GetWrapperL().AddObjectL( *object );
    PRINT2( _L( "MM MTP => CMediaMtpDataProvider::ObjectAddedL formatCode=0x%x Suid=%S" ), formatCode, &(object->DesC( CMTPObjectMetaData::ESuid ) ) );
    CleanupStack::PopAndDestroy( object );
    PRINT( _L( "MM MTP <= CMediaMtpDataProvider::ObjectAddedL" ) );
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::StartObjectEnumerationL
// Start object enumeration
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::StartObjectEnumerationL( TUint32 aStorageId )
    {
    iPendingEnumerations.AppendL( aStorageId );

    if ( iPendingEnumerations.Count() > 0 )
        {
        iMediaEnumerator->StartL( iPendingEnumerations[KActiveEnumeration] );
        }
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::StartStorageEnumerationL
// Start Storage enumeration
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::StartStorageEnumerationL()
    {
    Framework().StorageEnumerationCompleteL();
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::Supported
// Defines the supported operations and formats of the data provider
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::Supported( TMTPSupportCategory aCategory,
    RArray<TUint>& aArray ) const
    {
    switch ( aCategory )
        {
        case EEvents:
            break;

        case EObjectCaptureFormats:
        case EObjectPlaybackFormats:
            {
            TInt count = sizeof( KMediaMtpDataProviderSupportedFormats ) / sizeof( TUint16 );
            for ( TInt i = 0; i < count; i++ )
                {
                aArray.Append( KMediaMtpDataProviderSupportedFormats[i] );
                }
            PRINT1( _L( "MM MTP <> CMediaMtpDataProvider::Supported format count = %d" ), count );
            }
            break;

        case EOperations:
            {
            TInt count = sizeof( KMediaMtpDataProviderSupportedOperations ) / sizeof( TUint16 );

            for ( TInt i = 0; i < count; i++ )
                {
                aArray.Append( KMediaMtpDataProviderSupportedOperations[i] );
                }
            PRINT1( _L( "MM MTP <> CMediaMtpDataProvider::Supported operation count = %d" ), count );
            }
            break;

        case EObjectProperties:
            {
            TInt count = iSupportedPropAll.Count();

            for ( TInt i = 0; i < count; i++ )
                {
                aArray.Append( iSupportedPropAll[i] );
                }
            PRINT1( _L( "MM MTP <> CMediaMtpDataProvider::Supported properties count = %d" ), aArray.Count() );
            }
            break;

        case EStorageSystemTypes:
            {
            aArray.Append( CMTPStorageMetaData::ESystemTypeDefaultFileSystem );
            }
            break;

        default:
            // Unrecognised category, leave aArray unmodified.
            break;
        }
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::SupportedL
// Defines the supported operations and formats of the data provider
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::SupportedL( TMTPSupportCategory aCategory,
    CDesCArray& aStrings ) const
    {
    if ( aCategory == EVendorExtensionSets )
        {
        aStrings.AppendL( KMediaMtpDataProviderExtension1 );
        aStrings.AppendL( KMediaMtpDataProviderExtension2 );
        }
    else if( aCategory == EFormatExtensionSets )
        {
        //EMTPFormatCodeMP3,
        aStrings.AppendL(KFormatExtensionMP3);

        #ifdef __WINDOWS_MEDIA
        //EMTPFormatCodeWMA,
        aStrings.AppendL(KFormatExtensionWMA);
        #endif

        //EMTPFormatCodeMP4Container,
        aStrings.AppendL(KFormatExtensionMP4);
        aStrings.AppendL(KFormatExtensionM4A);

        //EMTPFormatCode3GPContainer,
        aStrings.AppendL(KFormatExtension3GP);
        aStrings.AppendL(KFormatExtension3G2);

        //EMTPFormatCodeAAC,
        aStrings.AppendL(KFormatExtensionAAC);

        //EMTPFormatCodeWAV,
        aStrings.AppendL(KFormatExtensionWAV);
        #ifdef __WINDOWS_MEDIA
        //EMTPFormatCodeWMV,
        aStrings.AppendL(KFormatExtensionWMV);

        //EMTPFormatCodeASF
        aStrings.AppendL(KFormatExtensionASF);
        #endif
        //ODF container
        aStrings.AppendL(KFormatExtensionODFAudio3GPP);
        aStrings.AppendL(KFormatExtensionODFVideo3GPP);

        aStrings.AppendL(KFormatExtensionO4A);
        aStrings.AppendL(KFormatExtensionO4V);

        }
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::NotifyEnumerationCompleteL
// enumeration completed
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProvider::NotifyEnumerationCompleteL( TUint32 /*aStorageId*/,
    TInt /*aError*/ )
    {
    Framework().ObjectEnumerationCompleteL( iPendingEnumerations[KActiveEnumeration] );
    iPendingEnumerations.Remove( KActiveEnumeration );
    if ( iPendingEnumerations.Count() )
        {
        iMediaEnumerator->StartL( iPendingEnumerations[KActiveEnumeration] );
        }
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::LocateRequestProcessorL
// Find or create a request processor that can process the request
// -----------------------------------------------------------------------------
//
TInt CMediaMtpDataProvider::LocateRequestProcessorL( const TMTPTypeRequest& aRequest,
    MMTPConnection& aConnection )
    {
    TInt index = KErrNotFound;
    TInt count = iActiveProcessors.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        if ( iActiveProcessors[i]->Match( aRequest, aConnection ) )
            {
            index = i;
            break;
            }
        }
    if ( index == KErrNotFound )
        {
        MMmRequestProcessor* processor =
            MediaMtpDataProviderProcessor::CreateL( Framework(),
                aRequest,
                aConnection,
                *this );

        CleanupReleasePushL( *processor );
        iActiveProcessors.AppendL( processor );
        CleanupStack::Pop( processor );
        index = count;
        }

    return index;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::LocateRequestProcessorL
// Find or create a request processor that can process the event
// -----------------------------------------------------------------------------
//
TInt CMediaMtpDataProvider::LocateRequestProcessorL( const TMTPTypeEvent& aEvent,
    MMTPConnection& aConnection )
    {
    TInt index = KErrNotFound;
    TInt count = iActiveProcessors.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        if ( iActiveProcessors[i]->Match( aEvent, aConnection ) )
            {
            index = i;
            break;
            }
        }

    return index;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::GetWrapper
// return wrapper references
// -----------------------------------------------------------------------------
//
CMmMtpDpMetadataAccessWrapper& CMediaMtpDataProvider::GetWrapperL()
    {
    return CMmMtpDpAccessSingleton::GetAccessWrapperL();
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::PropSettingUtility
// return The utility to setting properties
// -----------------------------------------------------------------------------
//
CPropertySettingUtility* CMediaMtpDataProvider::PropSettingUtility()
    {
    return iPropSettingUtility;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProvider::DescriptionUtility
// return The utiltiy to setting descriptions
// -----------------------------------------------------------------------------
//
CDescriptionUtility* CMediaMtpDataProvider::DescriptionUtility()
    {
    return iDescriptionUtility;
    }

// ---------------------------------------------------------------------------
// CMediaMtpDataProvider::GetSupportedFormat
//
// ---------------------------------------------------------------------------
//
const RArray<TUint>* CMediaMtpDataProvider::GetSupportedFormat() const
    {
    PRINT1( _L( "MM MTP <> CMediaMtpDataProvider::GetSupportedFormat, count = %d" ), iSupportedFormat.Count() );
    return &iSupportedFormat;
    }

void CMediaMtpDataProvider::GetSupportedFormatL()
    {
    iSupportedFormat.Reset();

    TInt count = sizeof ( KMediaMtpDataProviderSupportedFormats ) / sizeof( TUint16 );
    for ( TInt i = 0; i < count; i++ )
        {
        InsertL( iSupportedFormat, KMediaMtpDataProviderSupportedFormats[i] );
        }
    }

const RArray<TUint>* CMediaMtpDataProvider::GetSupportedPropertiesL( TUint32 aFormatCode ) const
    {
    switch ( aFormatCode )
        {
        case EMTPFormatCodeWMA:
        case EMTPFormatCodeMP3:
        case EMTPFormatCodeAAC:
        case EMTPFormatCodeWAV:
        case EMTPFormatCodeMP4Container:
        case EMTPFormatCode3GPContainer:
            return &iSupportedPropAudio;

        case EMTPFormatCodeWMV:
        case EMTPFormatCodeASF:
            return &iSupportedPropVideo;

        default:
            {
            User::Leave( KErrNotSupported );
            }
        }
    // should never run to this line, just for avoiding warning.
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMediaMtpDataProvider::GetSupportedPropL
//
// ---------------------------------------------------------------------------
//
void CMediaMtpDataProvider::GetSupportedPropL()
    {
    iSupportedPropAudio.Reset();
    iSupportedPropVideo.Reset();

    TInt count = 0, i = 0;
    count = sizeof( KMmMtpDpSupportedPropMandatoryAll ) / sizeof( TUint16 );
    for ( i = 0; i < count; i++ )
        {
        InsertL( iSupportedPropAudio, KMmMtpDpSupportedPropMandatoryAll[i] );
        InsertL( iSupportedPropVideo, KMmMtpDpSupportedPropMandatoryAll[i] );

        }

    count = sizeof( KMmMtpDpSupportedPropAdditionalAll ) / sizeof( TUint16 );
    for ( i = 0; i < count; i++ )
        {
        InsertL( iSupportedPropAudio, KMmMtpDpSupportedPropAdditionalAll[i] );
        InsertL( iSupportedPropVideo, KMmMtpDpSupportedPropAdditionalAll[i] );
        }

    for ( TInt j = 0; j < iSupportedFormat.Count(); j++ )
        {
        switch ( iSupportedFormat[j] )
            {
            case EMTPFormatCodeWMA:
            case EMTPFormatCodeMP3:
            case EMTPFormatCodeAAC:
            case EMTPFormatCodeWAV:
            case EMTPFormatCodeMP4Container:
            case EMTPFormatCode3GPContainer:
                {
                count = sizeof ( KMmMtpDpSupportedPropMandatoryAudio ) / sizeof( TUint16 );
                for ( i = 0; i < count; i++ )
                    {
                    InsertL( iSupportedPropAudio, KMmMtpDpSupportedPropMandatoryAudio[i] );
                    }

                count = sizeof ( KMmMtpDpSupportedPropAdditionalAudio ) / sizeof(TUint16);
                for ( i = 0; i < count; i++ )
                    {
                    InsertL( iSupportedPropAudio, KMmMtpDpSupportedPropAdditionalAudio[i] );
                    }
                }
                break;

            case EMTPFormatCodeWMV:
            case EMTPFormatCodeASF:
                {
                count = sizeof ( KMmMtpDpSupportedPropMandatoryWMV ) / sizeof(TUint16);
                for ( i = 0; i < count; i++ )
                    {
                    InsertL( iSupportedPropVideo, KMmMtpDpSupportedPropMandatoryWMV[i] );
                    }

                count = sizeof ( KMmMtpDpSupportedPropAdditionalWMV ) / sizeof(TUint16);
                for ( i = 0; i < count; i++ )
                    {
                    InsertL( iSupportedPropVideo, KMmMtpDpSupportedPropAdditionalWMV[i] );
                    }
                }
                break;

            default:
                {
                // shouldn't happen
                User::Leave( KErrNotSupported );
                }
                break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CMediaMtpDataProvider::GetAllSupportedProperties
//
// ---------------------------------------------------------------------------
//
const RArray<TUint>* CMediaMtpDataProvider::GetAllSupportedProperties() const
    {
    return &iSupportedPropAll;
    }

void CMediaMtpDataProvider::GetAllSupportedPropL()
    {
    iSupportedPropAll.Reset();

    TInt i = 0;
    TInt count = sizeof( KMmMtpDpSupportedPropMandatoryAll ) / sizeof( TUint16 );
    for ( i = 0; i < count; i++ )
        InsertL( iSupportedPropAll, KMmMtpDpSupportedPropMandatoryAll[i] );

    count = sizeof ( KMmMtpDpSupportedPropAdditionalAll ) / sizeof(TUint16);
    for ( i = 0; i < count; i++ )
        InsertL( iSupportedPropAll, KMmMtpDpSupportedPropAdditionalAll[i] );

    count = sizeof ( KMmMtpDpSupportedPropMandatoryAudio ) / sizeof(TUint16);
    for ( i = 0; i < count; i++ )
        InsertL( iSupportedPropAll, KMmMtpDpSupportedPropMandatoryAudio[i] );

    count = sizeof ( KMmMtpDpSupportedPropAdditionalAudio ) / sizeof(TUint16);
    for ( i = 0; i < count; i++ )
        InsertL( iSupportedPropAll, KMmMtpDpSupportedPropAdditionalAudio[i] );

    count = sizeof ( KMmMtpDpSupportedPropMandatoryWMV ) / sizeof(TUint16);
    for ( i = 0; i < count; i++ )
        InsertL( iSupportedPropAll, KMmMtpDpSupportedPropMandatoryWMV[i] );

    count = sizeof ( KMmMtpDpSupportedPropAdditionalWMV ) / sizeof(TUint16);
    for ( i = 0; i < count; i++ )
        InsertL( iSupportedPropAll, KMmMtpDpSupportedPropAdditionalWMV[i] );
    }

// ---------------------------------------------------------------------------
// CMediaMtpDataProvider::GetDefaultStorageIdL
//
// ---------------------------------------------------------------------------
//
TUint32 CMediaMtpDataProvider::GetDefaultStorageIdL() const
    {
    TInt driveNum = -1;
    TInt err = DriveInfo::GetDefaultDrive( DriveInfo::EDefaultMassStorage, driveNum );
    PRINT2( _L( "MM MTP <> GetDefaultDrive, driveNum = %d, err = %d" ), driveNum, err );

    TDriveInfo driveInfo;
    User::LeaveIfError( Framework().Fs().Drive( driveInfo, driveNum ) );
    PRINT3( _L( "driveInfo.iType = 0x%x, driveInfo.iDriveAtt = 0x%x, driveInfo.iMediaAtt = 0x%x" ),
        driveInfo.iType, driveInfo.iDriveAtt, driveInfo.iMediaAtt );
    if( driveInfo.iType == EMediaNotPresent || driveInfo.iType == EMediaUnknown )
        {
        err = DriveInfo::GetDefaultDrive( DriveInfo::EDefaultRemovableMassStorage, driveNum );
        User::LeaveIfError( Framework().Fs().Drive( driveInfo, driveNum ) );
        if( driveInfo.iType == EMediaNotPresent || driveInfo.iType == EMediaUnknown )
            {
            err = DriveInfo::GetDefaultDrive( DriveInfo::EDefaultPhoneMemory, driveNum );
            PRINT( _L( "MM MTP <> Memory card doesn't exist, set PhoneMemory to default" ) );
            }
        }

    return Framework().StorageMgr().FrameworkStorageId( TDriveNumber( driveNum ) );
    }

// ---------------------------------------------------------------------------
// CMediaMtpDataProvider::Insert
//
// ---------------------------------------------------------------------------
//
void CMediaMtpDataProvider::InsertL( RArray<TUint>& aArray, const TUint aProperCode ) const
    {
    TInt err = KErrNone;
    err = aArray.Find( aProperCode );
    if ( err == KErrNotFound )
        err = aArray.Append( aProperCode );

    User::LeaveIfError( err );
    }

// end of file
