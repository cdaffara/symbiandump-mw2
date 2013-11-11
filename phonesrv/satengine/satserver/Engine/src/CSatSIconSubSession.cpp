/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the icon sub-session IPC.
*
*/


#include    <fbs.h>
#include    <rsatservice.h>
#include    <e32property.h>

#include    "tsatinternaliconinfo.h"
#include    "SATInternalPSKeys.h"
#include    "CSatSIconSubSession.h"
#include    "MSatSIconAPI.h"
#include    "SatSOpcodes.h"
#include    "CSatSSession.h"
#include    "CSatSIconConverter.h"
#include    "EnginePanic.h"
#include    "SatLog.h"
#include    "TSatEventMediator.h"
#include    "MSatUtils.h"

const TUint8 KClutEntrySize = 3;
const TUint KDefaultNumberOfClutEntries = 256;
const TUint8 KColourDepthByte = 4;
const TUint8 KNumberOfCLUTByte = KColourDepthByte + 1;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSIconSubSession::CSatSIconSubSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSIconSubSession::CSatSIconSubSession(
    CSatSSession& aSession,
    TSatEventMediator& aEventMediator ) :
    CSatSSubSession( aSession ),
    iEventMediator( aEventMediator )
    {
    LOG( SIMPLE, "SATENGINE: \
        CSatSIconSubSession::CSatSIconSubSession calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSatSIconSubSession* CSatSIconSubSession::NewL(
    CSatSSession& aSession,
    TSatEventMediator& aEventMediator )
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NewL calling" )

    CSatSIconSubSession* self =
        new( ELeave ) CSatSIconSubSession( aSession, aEventMediator );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( /* self */ );

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NewL exiting" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::ConstructL
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::ConstructL calling" )

    iIconAPI = iSession.CreateIconHandlerL();

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::~CSatSIconSubSession
// -----------------------------------------------------------------------------
//
CSatSIconSubSession::~CSatSIconSubSession()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSIconSubSession::~CSatSIconSubSession calling" )
        
    delete iBitmap;
    delete iIconAPI;
    delete iClut;
    delete iIconData;

    LOG( SIMPLE,
        "SATENGINE: CSatSIconSubSession::~CSatSIconSubSession exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::GetIconInfo
// Gets the icon info from SIM.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::GetIconInfo()
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInfo calling" )

    // Read the recordnumber from client.
    TUint8 recordNumber( 0 );
    TPckg<TUint8> recordPckg( recordNumber );
    TRAPD( err, iMessage.ReadL( 0, recordPckg ) );
    LOG2( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInfo \
          err: %i", err )
    if ( KErrNone == err )
        {
        // Start getting the icon info from SIM.
        iIconEf.SetLength( NULL );
        iIconAPI->GetIconInfo( recordNumber, iIconEf, *this );
        }
    else
        {
        iSession.PanicClient( iMessage, ESatSBadDescriptor );
        }

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInfo exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::NotifyGetIconInfo
// Notification from icon observer
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::NotifyGetIconInfo()
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NotifyGetIconInfo calling" )

    // Send the icon info data got to client.
    TPckg<RSat::TIcon> infoPckg( iIconEf );    
    Write( iMessage, 1, infoPckg );
    iMessage.Complete( KErrNone );

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NotifyGetIconInfo exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::GetIconInstanceL
// Gets the icon from SIM.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::GetIconInstanceL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInstanceL calling" )

    TSatInternalIconInfo info;
    // Read the icon info from client.
    TSatInternalIconInfoPckg infoPckg( info );
    TRAPD( err, iMessage.ReadL( 0, infoPckg ) );

    iInfo.iInstanceNumber = info.iFileIdentifier;
    iInfo.iOffset = info.iOffset;
    iInfo.iLength = info.iLength;

    if ( TSatInternalIconInfo::EBasic == info.iCodingScheme )
        {
        LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInstanceL \
             basic CodingScheme" )
        iInfo.iCoding = RSat::KBasic;
        }
    else
        {
        LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInstanceL \
             colour CodingScheme" )
        iInfo.iCoding = RSat::KColour;
        }

    iInfo.iImgWidth = static_cast<TUint8>( info.iWidth );
    iInfo.iImgHeight = static_cast<TUint8>( info.iHeight );
    iInfo.iFileId = info.iFileIdentifier;
    LOG2( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInstanceL \
          err: %i", err )
    if ( KErrNone == err )
        {
        // Create buffer for the icon data.
        iIconData = HBufC8::NewL( iInfo.iLength );
        TPtr8 iconDataPtr( iIconData->Des() );

        // Start getting the icon data from SIM.
        iIconAPI->GetIconInstance( iInfo, iconDataPtr, *this );
        }
    else
        {
        iSession.PanicClient( iMessage, ESatSBadDescriptor );
        }

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::GetIconInstanceL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::NotifyGetIconInstanceL
// Notification from icon observer
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::NotifyGetIconInstanceL()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSIconSubSession::NotifyGetIconInstanceL calling" )

    LOG2( NORMAL, "SATENGINE: CSatSIconSubSession::NotifyGetIconInstanceL \
          infoLength: %x", iInfo.iLength )

    LOG2( NORMAL, "SATENGINE: CSatSIconSubSession::NotifyGetIconInstanceL \
          dataLength: %x", iIconData->Length() )

    // Get the CLUT and convert the icon data to bitmap.

    // basic icon has empty CLUT
    if ( RSat::KBasic == iInfo.iCoding )
        {
        // For the basi icon the length from the icon info and icon data
        // body should have the same length but for the clore icon the
        // length not always same.
        if ( iInfo.iLength == iIconData->Length() )
            {
            LOG( SIMPLE, "CSatSIconSubSession::NotifyGetIconInstanceL \
             get the basic clut" )
            // Basic icon does not have CLUT.
            iClut = KNullDesC8().AllocL();
            // Complete icon.
            NotifyGetClutL();
            }
        else
            {
            LOG( SIMPLE, "CSatSIconSubSession::NotifyGetIconInstanceL \
                notify failure basic" )
            NotifyFailure( KErrCorrupt );
            }
        }
    else // color icon
        {
        LOG( SIMPLE, "CSatSIconSubSession::NotifyGetIconInstanceL \
             others" )
        TPtr8 iconDataPtr( iIconData->Des() );

        if ( iconDataPtr.Length() >= KNumberOfCLUTByte + 1 )
            {
            // Create buffer for clut. Each clut entry contains
            // intensity of red, green and blue. The value of 0 is
            // interpreted as 256 clut entries. (ETSI TS 131 102 V4.10.0).
            TInt numberOfClutEntries( iconDataPtr[KClutEntrySize] );
            LOG2( SIMPLE, "CSatSIconSubSession::NotifyGetIconInstanceL \
                  numberOfClutEntries: %i", numberOfClutEntries )
            if ( 0 == numberOfClutEntries )
                {
                numberOfClutEntries = KDefaultNumberOfClutEntries;
                }
    
            // Start getting the icon color lookup table from SIM.
            TInt length = numberOfClutEntries * KClutEntrySize;
            // In some situation the length we get from the icon information
            // includes the length of the CLUT and some time it doesn't
            // Both situation a valid.
            if ( ( iInfo.iLength == iIconData->Length() )
                 || ( iInfo.iLength == iIconData->Length() + length ) )  
                {
                iClut = HBufC8::NewL( length );
    
                LOG( SIMPLE, "CSatSIconSubSession::NotifyGetIconInstanceL \
                     get clut" )
                const TUint offset( iconDataPtr[KColourDepthByte] << 8 |
                    iconDataPtr[KNumberOfCLUTByte] );
                TPtr8 clutPtr( iClut->Des() );
                iIconAPI->GetClut(
                    iInfo.iInstanceNumber, offset, clutPtr, *this );
                }
            else
                {
                LOG( SIMPLE, "CSatSIconSubSession::NotifyGetIconInstanceL \
                     others notify failure" )
                NotifyFailure( KErrCorrupt );
                }
            }
        else
            {
            LOG( SIMPLE, "CSatSIconSubSession::NotifyGetIconInstanceL \
                 others notify failure" )
            NotifyFailure( KErrCorrupt );
            }
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSIconSubSession::NotifyGetIconInstanceL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::NotifyGetClutL
// Notification from icon observer
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::NotifyGetClutL()
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NotifyGetClutL calling" )

    // Create icon converter.
    CSatSIconConverter* converter =
        CSatSIconConverter::CreateConverterL(
            static_cast<RSat::TImageCoding>( iInfo.iCoding ) );
    CleanupStack::PushL( converter );

    if ( NULL != iBitmap )
        {
        delete iBitmap; // Delete the bitmap
        iBitmap = NULL;
        }

    iBitmap = converter->ConvertL( *iIconData, *iClut );

    // Send the bitmap handle to client.
    TPckg<TInt> handlePckg( iBitmap->Handle() );
    Write( iMessage, 1, handlePckg );

    CleanupStack::PopAndDestroy( converter );

        delete iClut;
        iClut = NULL;
        
        delete iIconData;
        iIconData = NULL;

    iMessage.Complete( KErrNone );
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NotifyGetClutL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::NotifyFailure
// Notification from icon observer
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::NotifyFailure( TInt errorCode )
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NotifyFailure calling" )

    if ( NULL != iMessage.Handle() )
        {
        LOG(
            SIMPLE,
            "SATENGINE: CSatSIconSubSession::NotifyFailure Message!" )
        iMessage.Complete( errorCode );
        }
    else
        {
        LOG(
            SIMPLE,
            "SATENGINE: CSatSIconSubSession::NotifyFailure No Message!" )
        }

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::NotifyFailure exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::HandleCommandL
// Handles the icon sub-session requests.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::HandleCommandL( const RMessage2& aMessage )
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::HandleCommandL calling" )

    switch ( aMessage.Function() )
        {
        case ESatSGetIconInfo:
            {
            LOG( NORMAL, "SATENGINE:   ESatSGetIconInfo" )
            if ( NULL != iMessage.Handle() )
                {
                LOG( SIMPLE,
                    "SATENGINE:   Request already pending (GetIconInfo)" )
                iSession.PanicClient( aMessage, ESatSBadRequest );
                }
            else
                {
                LOG( SIMPLE,
                    "SATENGINE:   Request handling (GetIconInfo)" )
                iMessage = aMessage;
                GetIconInfo();
                }
            break;
            }

        case ESatSGetIconInstance:
            {
            LOG( NORMAL, "SATENGINE:   ESatSGetIconInstance" )
            TBool mode = IconFetchingMode();
            
            NotifyIconFetchingMode( mode );
                        
            if ( NULL != iMessage.Handle() )
                {
                LOG( SIMPLE,
                    "SATENGINE:   Request already pending (GetIconInstance)" )
                iSession.PanicClient( aMessage, ESatSBadRequest );
                }
            else
                {
                LOG( SIMPLE,
                    "SATENGINE:   Request handling (GetIconInstance)" )
                iMessage = aMessage;
                GetIconInstanceL();
                }
            break;
            }

        // Client issues ESatSIconInstanceGot when the client
        // has duplicated the icon and then the bitmap can be
        // deleted.
        case ESatSIconInstanceGot:
            {
            LOG( NORMAL, "SATENGINE:   ESatSIconInstanceGot" )
            // Free the bitmap.
            delete iBitmap;
            iBitmap = NULL;
            aMessage.Complete( KErrNone );
            break;
            }

        // Response for SetUpIdleModeText
        case ESatSIdleModeResponse:
            {
            LOG( NORMAL, "SATENGINE:   ESatSIdleModeResponse" )
            // Send the response to Proactive SetUpIdleModeText handler
            SendSetUpIdleModeResponse( aMessage.Int0() );
            aMessage.Complete( KErrNone );
            break;
            }

        default:
            {
            LOG( SIMPLE, "SATENGINE:   Bad function - default" )
            iSession.PanicClient( aMessage, ESatSBadRequest );
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::HandleCommandL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::SendSetUpIdleModeResponse
// Notification from icon observer
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::SendSetUpIdleModeResponse( TInt aResponse ) const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSIconSubSession::SendSetUpIdleModeResponse calling" )

    // Send the response through Event handling procedure.
    switch ( aResponse )
        {
        case RSatService::ESATIdleSuccess:
            {
            LOG( NORMAL, "SATENGINE:   Success" )
            iEventMediator.Notify(
                MSatUtils::EIdleModeResponseSuccess );
            break;
            }

        case RSatService::ESATIdleSuccessRequestedIconNotDisplayed:
            {
            LOG( NORMAL, "SATENGINE:   Success no icon" )
            iEventMediator.Notify(
                MSatUtils::EIdleModeResponseSuccessNoIcon );
            break;
            }

        case RSatService::ESATIdleMeUnableToProcessCmd:
            {
            LOG( NORMAL, "SATENGINE:   Unable to process" )
            iEventMediator.Notify(
                MSatUtils::EIdleModeResponseUnableToProcess );
            break;
            }

        case RSatService::ESATIdleCmdBeyondMeCapabilities:
            {
            LOG( NORMAL, "SATENGINE:   Beyond capabilities" )
            iEventMediator.Notify(
                MSatUtils::EIdleModeResponseBeyondCapabilities );
            break;
            }

        default:
            {
            LOG( NORMAL, "SATENGINE:   Unknown response" )
            break;
            }
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSIconSubSession::SendSetUpIdleModeResponse exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::Write
// Writes descriptor to client address space and panics the client if
// writing fails.
// -----------------------------------------------------------------------------
//
void CSatSIconSubSession::Write( const RMessage2& aMessage,
                                 TInt aParam,
                                 const TDesC8& aDes ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::Write calling" )

    TRAPD( err, aMessage.WriteL( aParam, aDes ) );
    LOG2( SIMPLE, "SATENGINE: CSatSIconSubSession::Write \
          err: %i", err )
    if ( KErrNone != err )
        {
        iSession.PanicClient( aMessage, ESatSBadDescriptor );
        }

    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::Write exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconSubSession::CompleteError
// Completion of iMessage for handler
// -----------------------------------------------------------------------------
//
TInt CSatSIconSubSession::CompleteError( TInt aError ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::CompleteError calling" )
    TInt err( KErrCompletion );

    if ( NULL != iMessage.Handle() )
        {
        iMessage.Complete( aError );
        LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::CompleteError iMessage \
            completed" )
        err = KErrNone;
        }

    LOG2( SIMPLE, "SATENGINE: CSatSIconSubSession::CompleteError exiting, \
          err: %i", err )
    return err;
    }
// -----------------------------------------------------------------------------
// Get icon fetching mode which is in synchronous or asynchronous mode.
// -----------------------------------------------------------------------------
TBool CSatSIconSubSession::IconFetchingMode()
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::IconFetchingMode calling" )

    TInt cancelledUsingEndKey( 0 );
    TBool isSyncMode( EFalse );
    
    TInt error = RProperty::Get( KPSUidSatServerInternal,
        KSatAppClosedUsingEndKey, cancelledUsingEndKey );
    
    LOG2( SIMPLE, "SATENGINE: CSatSIconSubSession::IconFetchingMode \
          P&S key: %i", cancelledUsingEndKey )
        
    LOG2( NORMAL,
        "SATENGINE: CSatSIconSubSession::IconFetchingMode \
        subscribe KSatUiCancelledUsingEndKey and return: %d", error )
            
    if ( cancelledUsingEndKey )
        {
        LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::IconFetchingMode \
             cancell UsingEndKey" )
        isSyncMode = ETrue;
        }
    else
        {
        LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::IconFetchingMode \
             don't cancell UsingEndKey" )
        isSyncMode = EFalse;
        }
        
    LOG( SIMPLE, "SATENGINE: CSatSIconSubSession::IconFetchingMode exiting" )

    return isSyncMode;
    }
    
// -----------------------------------------------------------------------------
// Notify CSatIconHandler to fetch icons from NAA synchronously or
// asynchrounously.
// -----------------------------------------------------------------------------
void CSatSIconSubSession::NotifyIconFetchingMode( TBool aMode )
    {
    LOG( SIMPLE, 
        "SATENGINE: CSatSIconSubSession::NotifyIconFetchingMode calling" )

    iIconAPI->SetIconFetchingMode( aMode );

    if ( aMode )
        {
        TInt error = RProperty::Set( KPSUidSatServerInternal,
            KSatAppClosedUsingEndKey, KSatAppTerminatedNormal );
        LOG2( SIMPLE,"CSatSIconSubSession::NotifyIconFetchingMode \
              P&S key: %i", KSatAppTerminatedNormal )
            
        LOG2( NORMAL,
            "SATENGINE: CSatSIconSubSession::NotifyIconFetchingMode \
            publish KSatUiCancelledUsingEndKey and return: %d", error )
        }

    LOG( SIMPLE, 
        "SATENGINE: CSatSIconSubSession::NotifyIconFetchingMode exiting" )
    }
