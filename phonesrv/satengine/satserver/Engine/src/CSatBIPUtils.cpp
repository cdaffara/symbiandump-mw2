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
* Description:  Utility class for BIP
*
*/


#include <es_sock.h>
#include <etelqos.h>
#include "SatLog.h"
#include "CSatBIPUtils.h"
#include "CSatBIPGPRSDataChannel.h"
#include "csatbipconnectionobserver.h"
#include "MSatBIPDataChannel.h"
#include "MSatBIPChannelStatusObserver.h"
#include "MSatBIPDataAvailableObserver.h"
//lint -e766 Used inside TRAP macro, lint misfunction.
#include "EnginePanic.h"
#include "TSatChannelIDInfo.h"
#include "MSatUtils.h"
#include "CSatApnHandler.h"
#include "csatsactivewrapper.h"
#include "msatmultimodeapi.h"

const TInt KPDPContextPos( 7 );
const TInt KMaxAvailChannels( 7 );
const TInt8 KProposeQoSParametersPop( 2 );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CSatBIPUtils* CSatBIPUtils::NewL( MSatUtils& aUtils )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::NewL calling" )

    CSatBIPUtils* self = new ( ELeave ) CSatBIPUtils( aUtils );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::NewL exiting" )
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSatBIPUtils::~CSatBIPUtils()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::~CSatBIPUtils calling" )

    // Close all data channels
    if ( iDataChannels )
        {
        iDataChannels->ResetAndDestroy();
        }
    delete iDataChannels;

    if ( iChannelIds )
        {
        iChannelIds->ResetAndDestroy();
        }
    delete iChannelIds;
    
    delete iChannelId;
    
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::~CSatBIPUtils\
            Close connection and subconnection" )
    iSubConnection.Close();
    iConnection.Close();
    iSocketServer.Close();
    iPacketService.Close();
    
    delete iApnHandler;

    if ( iWrapper )
        {
        iWrapper->CancelWrapper();
        delete iWrapper;
        iWrapper = NULL;
        }

    if ( iConnObserver )
        {
        iConnObserver->Cancel();
        delete iConnObserver;
        iConnObserver = NULL;
        }

    iDataAvailableObserver = NULL;
    iChannelStatusObserver = NULL;

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::~CSatBIPUtils exiting" )
    }

// ---------------------------------------------------------------------------
// Handles event
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::Event( TInt aEvent )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::Event calling" )

    // Check event
    if ( MSatUtils::ECommandCancelled == aEvent ||
         MSatUtils::ECancelledUsingEndKey == aEvent )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPUtils::Event check event" )
        if ( iDataChannels )
            {
            const TInt channelCount( iDataChannels->Count() );
            LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::Event channelCount: %d",
                  channelCount )
            // Cancel all channels
            for ( TInt index = 0; index < channelCount; index++ )
                {
                MSatBIPDataChannel* channel = iDataChannels->At( index );
                __ASSERT_ALWAYS( channel,
                    PanicSatEngine( ESatEngineNullPointer ) );
                channel->CancelAll();
                }
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::Event exiting" )
    }

// ---------------------------------------------------------------------------
// Creates new data channel
// ---------------------------------------------------------------------------
//
MSatBIPDataChannel* CSatBIPUtils::CreateChannelL(
    const TSatBearerType aBearerType, TInt& aErrCode )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CreateChannelL calling" )

    TInt errCode( MSatBIPUtils::ESatBIPNoChannelAvail );
    MSatBIPDataChannel* channel = NULL;

    // Get available channel ID
    TSatChannelIdInfo* channelId( AvailableChannelID() );

    // Check is the channel available
    if ( channelId )
        {
        // Create the right type of data channel
        if ( MSatBIPUtils::ESatGPRS == aBearerType )
            {
            LOG( NORMAL, 
            "SATENGINE: CSatBIPUtils::CreateChannelL ESatGPRS" )
            // New GPRS data channel
            channel = CSatBIPGPRSDataChannel::NewLC( *this, iSocketServer,
                *channelId, iConnection, iSubConnection );
            // Reserve channel ID
            channelId->ReserveChannel();
            // Add data channel to list
            iDataChannels->AppendL( channel );
            // Remove from CleanupStack
            CleanupStack::Pop( /* channel */ );
            // Return code
            errCode = MSatBIPUtils::ESatBIPSuccess;

#ifndef SAT_USE_DUMMY_TSY
            // Start connection observer
            if ( !iConnObserver )
                {
                LOG( NORMAL, " Starting connection observer" )
                iConnObserver = 
                    new ( ELeave ) CSatBIPConnectionObserver( *this );
                iConnObserver->StartObserver();
                }
#endif
            }
        else
            {
            // Others that GPRS are not yet supported
            LOG( NORMAL, 
                "SATENGINE: CSatBIPUtils::CreateChannelL Not supported" )
            errCode = MSatBIPUtils::ESatBIPNoChannelAvail;
            }
        }
    else
        {
        // Channel IDs not available
        LOG( NORMAL, 
            "SATENGINE: CSatBIPUtils::CreateChannelL Not available" )
        errCode = MSatBIPUtils::ESatBIPNoChannelAvail;
        }

    // Assign completion code
    aErrCode = errCode;

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CreateChannelL exiting" )
    return channel;
    }

// ---------------------------------------------------------------------------
// Closes the data channel
// ---------------------------------------------------------------------------
//
TInt CSatBIPUtils::CloseChannel( const TInt aChannelId )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseChannel calling" )

    TInt errCode( MSatBIPUtils::ESatBIPChannelIdNotValid );

    TInt index( 0 );
    TBool channelClosed( EFalse );

    // Loop the list of data channels and close the correct one.
    while ( index < iDataChannels->Count() && !channelClosed )
        {
        MSatBIPDataChannel* channel = iDataChannels->At( index );
        __ASSERT_ALWAYS( channel, PanicSatEngine( ESatEngineNullPointer ) );

        if ( channel->ChannelId() == aChannelId )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseChannel \
            channel->ChannelId() == aChannelId" )
             // Makes next data channel creation possible
            iDataChannelActivated = EFalse;
            // Channel found, close it and return Success.
            channel->CloseChannel();
            // Delete data channel
            iDataChannels->Delete( index );
            delete channel;
            errCode = MSatBIPUtils::ESatBIPSuccess;
            channelClosed = ETrue;
            }
        // Goto next channel
        index++;
        }
    LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::CloseChannel index: %d", index )

    // Free the space left in possible Delete call.
    iDataChannels->Compress();
    
    // Close connections if there're no active channels
    if ( !IsContextActive() )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseChannel\
            Close connection and subconnection" )
        iSubConnection.Close();
        SetSubConnectionActivated( EFalse );
        iConnection.Close();
        SetConnectionActivated( EFalse );
        
        if ( iConnObserver )
            {
            iConnObserver->Cancel();
            delete iConnObserver;
            iConnObserver = NULL;
            }
        LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseChannel\
            Close iPacketService" )
        iPacketService.Close();
        iPacketServiceActive = EFalse;
        }
    
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseChannel exiting" )
    return errCode;
    }

// ---------------------------------------------------------------------------
// Closes all data channels
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::CloseAllChannels()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseAllChannels calling" )
    LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::CloseAllChannels \
          channelcount: %d", iChannelIds->Count() )
    // Loop the list of data channels and close the correct one.
    for ( TInt i = 0; i < iChannelIds->Count(); i++ )
        {
        TSatChannelIdInfo* id = iChannelIds->At( i );
        // If channel is reserved -> Close it
        if ( id->IsReserved() )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseAllChannels \
            id->IsReserved()" )
            CloseChannel( id->ChannelId() );
            }
        }
    
    // Close subsession to socket server
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseAllChannels\
            Close connection and subconnection" )
    iSubConnection.Close();
    SetSubConnectionActivated( EFalse );
    iConnection.Close();
    SetConnectionActivated( EFalse );
    
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::CloseAllChannels exiting" )
    }

// ---------------------------------------------------------------------------
// Returns the correct data channel
// ---------------------------------------------------------------------------
//
MSatBIPDataChannel* CSatBIPUtils::DataChannel( const TInt aChannelId,
            TInt& aErrCode )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DataChannel calling" )

    // If the channel cannot be found, returns channel not valid error code
    TInt errCode( MSatBIPUtils::ESatBIPChannelIdNotValid );
    MSatBIPDataChannel* channel = NULL;
    TBool found( EFalse );
    TInt index( 0 );

    // Loop the list of data channels and return the correct one.
    while ( index < iDataChannels->Count() && !found )
        {
        channel = iDataChannels->At( index );
        __ASSERT_ALWAYS( channel, PanicSatEngine( ESatEngineNullPointer ) );
        // Compare channel ID's
        if ( channel->ChannelId() == aChannelId )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DataChannel \
            channel->ChannelId() == aChannelId" )
            // Channel found
            found = ETrue;
            if ( channel->IsContextActive() )
                {
                LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DataChannel \
                IsContextActive" )
                // Channel found and it is active
                errCode = MSatBIPUtils::ESatBIPSuccess;
                }
            else
                {
                // Channel found but it is closed
                errCode = MSatBIPUtils::ESatBIPChannelClosed;
                }
            }
        index++;
        }
    LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::DataChannel \
          index: %d", index )
    // Assign completion code to param
    aErrCode = errCode;

    LOG2( SIMPLE,
        "SATENGINE: CSatBIPUtils::DataChannel exiting with return code %d",
        errCode )
    return channel;
    }

// ---------------------------------------------------------------------------
// Generates TChannelStatus object
// ---------------------------------------------------------------------------
//
RSat::TChannelStatus CSatBIPUtils::GenerateChannelStatusL(
    const TInt aChannelId, TInt aStatus )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::GenerateChannelStatusL calling" )

    // ChannelStatus's first byte contains channel ID and pdp context status,
    // second byte contains information. values can be '00' or '05'
    RSat::TChannelStatus channelStatus;
    TInt err( MSatBIPUtils::ESatBIPNoChannelAvail );

    // Used to check the PDP context state
    MSatBIPDataChannel* dataChannel = DataChannel( aChannelId, err );

    // If channel is not active, status is still needed
    if ( ESatBIPSuccess == err || ESatBIPChannelClosed == err )
        {
        // Generate the first byte
        TUint8 channelId( 0 );   // last 3 bits.
        TUint8 pdpContext( 0 );  // first bit
        TUint8 firstByte( 0 );    // First byte

        // Channel ID. Bits 6 to 8.
        channelId = dataChannel->ChannelStatusChannelId();

        // If PDP Context is active, first bit is 1
        if ( dataChannel->IsContextActive() )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPUtils::GenerateChannelStatusL \
            dataChannel IsContextActive" )
            pdpContext = 1 << KPDPContextPos;
            }

        // We don't need data channel anymore
        dataChannel = NULL;

        // All the information for the first byte is gathered
        firstByte = static_cast<TUint8>( pdpContext ^ channelId );
        channelStatus.Append( firstByte );
        // Second byte should go straigth
        channelStatus.Append( aStatus );
        // LOG values
        LOG2( NORMAL,
            "SATENGINE: CSatBIPUtils::GenerateChannelStatusL First byte: %b",
            firstByte )
        LOG2( NORMAL,
            "SATENGINE: CSatBIPUtils::GenerateChannelStatusL Second byte: %b",
            aStatus )
        }
    else
        {
        LOG2( NORMAL, "SATENGINE: CSatBIPUtils::GenerateChannelStatusL \
            Data channel not found: %i", err )
        User::LeaveIfError( KErrNotFound );
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::GenerateChannelStatusL exiting" )
    return channelStatus;
    }

// ---------------------------------------------------------------------------
// Returns the interface reference to APN handler
// ---------------------------------------------------------------------------
//
MSatApnHandler& CSatBIPUtils::ApnHandler()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ApnHandler calling" )

    // Assert, Apn handler cannot be NULL
    __ASSERT_ALWAYS( iApnHandler, PanicSatEngine( ESatEngineNullPointer ) );

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ApnHandler exiting" )
    return *iApnHandler;
    }

// ---------------------------------------------------------------------------
// Returns the state of the PDP Context
// ---------------------------------------------------------------------------
//
TBool CSatBIPUtils::IsContextActive()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::IsContextActive calling" )

    TBool retVal( EFalse );
    TInt index( 0 );
    
    // Loop the list of data channels and end if active channel is found
    while ( index < iDataChannels->Count() && !retVal )
        {
        MSatBIPDataChannel* channel = iDataChannels->At( index );
        __ASSERT_ALWAYS( channel, PanicSatEngine( ESatEngineNullPointer ) );
        retVal = channel->IsContextActive();
        // Goto next channel
        index++;
        }
    LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::IsContextActive \
          index: %d", index )
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::IsContextActive exiting" )
    return retVal;
    }

// ---------------------------------------------------------------------------
// Returns the array of data channels
// ---------------------------------------------------------------------------
//
CArrayPtrFlat<MSatBIPDataChannel>* CSatBIPUtils::DataChannels()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DataChannels calling - exiting" )
    return iDataChannels;
    }

// ---------------------------------------------------------------------------
// Request to notify when data is available
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::NotifyDataAvailable(
    MSatBIPDataAvailableObserver* aObserver )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPUtils::NotifyDataAvailable calling - exiting" )
    iDataAvailableObserver = aObserver;
    }

// ---------------------------------------------------------------------------
// Request to notify when an error occurs in data channel
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::NotifyChannelStatus(
    MSatBIPChannelStatusObserver* aObserver )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPUtils::NotifyChannelStatus calling - exiting" )
    iChannelStatusObserver = aObserver;
    }

// ---------------------------------------------------------------------------
// Cancels the DataAvailable request
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::CancelDataAvailable()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPUtils::CancelDataAvailable calling - exiting" )
    iDataAvailableObserver = NULL;
    }

// ---------------------------------------------------------------------------
// Cancels the ChannelStatus request
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::CancelChannelStatus()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPUtils::CancelChannelStatus calling - exiting" )
    iChannelStatusObserver = NULL;
    }

// ---------------------------------------------------------------------------
// DataAvailable event. Notifies observer.
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::DataAvailable( const TInt aChannelId, const TInt aLength )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DataAvailable calling" )

    // If there is an active request for this event, notify observer.
    if ( iDataAvailableObserver )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DataAvailable \
        iDataAvailableObserver true" )
        iDataAvailableObserver->DataAvailable( aChannelId, aLength );
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::DataAvailable exiting" )
    }

// ---------------------------------------------------------------------------
// ChannelStatus event. Notifies observer.
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::ChannelStatus( const TInt aChannelId, const TInt aStatus )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ChannelStatus calling" )

    // If there is an active request for this event, notify observer.
    if ( iChannelStatusObserver )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ChannelStatus \
        iChannelStatusObserver true" )
        iChannelStatusObserver->ChannelStatus( aChannelId, aStatus );
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ChannelStatus exiting" )
    }

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CSatBIPUtils::CSatBIPUtils( MSatUtils& aUtils ) :
    iChannelId( NULL ),
    iUtils( aUtils )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatBIPUtils::CSatBIPUtils calling-exiting" )
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ConstructL calling" )

    // Create array for the data channels.
    iDataChannels =
        new ( ELeave ) CArrayPtrFlat<MSatBIPDataChannel>( KMaxAvailChannels );

    // Create array for Channel ID infos
    iChannelIds =
        new ( ELeave ) CArrayPtrFlat<TSatChannelIdInfo>( KMaxAvailChannels );

    TUint8 index = 1;
    
    iChannelId = new ( ELeave ) TSatChannelIdInfo( RSat::KChannel1, index++ );
    iChannelIds->AppendL( iChannelId );
    iChannelId = NULL;
    
    iChannelId = new ( ELeave ) TSatChannelIdInfo( RSat::KChannel2, index++ );
    iChannelIds->AppendL( iChannelId );
    iChannelId = NULL;
    
    iChannelId = new ( ELeave ) TSatChannelIdInfo( RSat::KChannel3, index++ );
    iChannelIds->AppendL( iChannelId );
    iChannelId = NULL;
    
    iChannelId = new ( ELeave ) TSatChannelIdInfo( RSat::KChannel4, index++ );
    iChannelIds->AppendL( iChannelId );
    iChannelId = NULL;
    
    iChannelId = new ( ELeave ) TSatChannelIdInfo( RSat::KChannel5, index++ );
    iChannelIds->AppendL( iChannelId );
    iChannelId = NULL;
    
    iChannelId = new ( ELeave ) TSatChannelIdInfo( RSat::KChannel6, index++ );
    iChannelIds->AppendL( iChannelId );
    iChannelId = NULL;
    
    iChannelId = new ( ELeave ) TSatChannelIdInfo( RSat::KChannel7, index++ );
    iChannelIds->AppendL( iChannelId );
    iChannelId = NULL;
    
    // Regiseter to listen events
    iUtils.RegisterL( this, MSatUtils::ECommandCancelled );
    iUtils.RegisterL( this, MSatUtils::ECancelledUsingEndKey );

    // Connect to Socket Server
    User::LeaveIfError( iSocketServer.Connect() );
    iPacketServiceActive = EFalse;
    iApnHandler = CSatApnHandler::NewL( iUtils );

    iWrapper = new ( ELeave ) CSatSActiveWrapper;
    
    SetConnectionActivated( EFalse );
    SetSubConnectionActivated( EFalse );

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ConstructL exiting" )
    }

// ---------------------------------------------------------------------------
// Returns next available channel ID
// ---------------------------------------------------------------------------
//
TSatChannelIdInfo* CSatBIPUtils::AvailableChannelID()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::AvailableChannelID calling" )

    TSatChannelIdInfo* channelId = NULL;

    TInt index( 0 );
    TBool idFound( EFalse );

    // Loop channel IDs
    while ( index < iChannelIds->Count() && !idFound )
        {
        channelId = iChannelIds->At( index );
        // If channel is free, take it
        if ( !channelId->IsReserved() )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPUtils::AvailableChannelID \
            channel free" )
            idFound = ETrue;
            }
         // goto next channel
        index++;
        }
    LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::AvailableChannelID \
          index: %d", index )
    if ( !idFound )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPUtils::AvailableChannelID \
        idFound false" )
        // If not found, return NULL
        channelId = NULL;
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::AvailableChannelID exiting" )
    return channelId;
    }

// ---------------------------------------------------------------------------
// Blocks synchronous requests
// ---------------------------------------------------------------------------
//
RPacketQoS::TQoSGPRSNegotiated CSatBIPUtils::ProposeQoSParametersL(
    const RPacketQoS::TQoSGPRSRequested& aQoSParams )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ProposeQoSParametersL calling" )

    // Number of contexts
    TInt count( ContextCount() ); // Get the number of contexts
    // QoS profile name
    TName profName;
    // Handle to the actual context
    RPacketContext packetContext;
    // QoS session
    RPacketQoS qos;
    // Context info
    RPacketService::TContextInfo info;
    // Error value for asynchronous calls
    TInt statusError( KErrNone );

    if ( count > 0 ) // At least one context found
        {
        // Get the last contexts info
        iPacketService.GetContextInfo( 
            iWrapper->RequestStatus(), count - 1, info );

        statusError = iWrapper->SetActiveAndWait(); // Wait for completion
        LOG2( SIMPLE, "     ContextInfo iActiveStatus: %i", statusError )
        LOG2( SIMPLE, "     ContextInfo CntxStatus: %i", info.iStatus )
        // Leave point
        User::LeaveIfError( statusError );
        }
    else
        {
        // Didn't find any contexts
        User::Leave( KErrNotFound );
        }

    // Get the context by name
    LOG( SIMPLE, "     Open existing context" )
    User::LeaveIfError(
        packetContext.OpenExistingContext( iPacketService, info.iName ) );

    CleanupClosePushL( packetContext );

    // Get the QoS Profile
    LOG( SIMPLE, "     Get QoS Profile name" )
    User::LeaveIfError( packetContext.GetProfileName( profName ) );

    // Get the instance of the QoS profile
    if ( profName.Length() > 0 )
        {
        LOG( SIMPLE, "     Open existing QoS profile" )
        User::LeaveIfError( qos.OpenExistingQoS( packetContext, profName ) );
        }

    CleanupClosePushL( qos );

    TPckg<RPacketQoS::TQoSGPRSRequested> qosPckg( aQoSParams );

    qos.SetProfileParameters( iWrapper->RequestStatus(), qosPckg );

    User::LeaveIfError( iWrapper->SetActiveAndWait() ); // Wait for completion

    // Get negotioted parameters
    RPacketQoS::TQoSGPRSNegotiated negParams;
    TPckg<RPacketQoS::TQoSGPRSNegotiated> negPckg( negParams );
    qos.GetProfileParameters( iWrapper->RequestStatus(), negPckg );

    User::LeaveIfError( iWrapper->SetActiveAndWait() ); // Wait for completion

    LOG2( SIMPLE, "     GetNegQoSParams Status: %i", statusError )
    LOG2( SIMPLE, "     GetNegQoSParams Reliability: %i", 
          negParams.iReliability )
    LOG2( SIMPLE, "     GetNegQoSParams Precedence: %i", 
          negParams.iPrecedence )
    LOG2( SIMPLE, "     GetNegQoSParams Delay: %i", negParams.iDelay )
    LOG2( SIMPLE, "     GetNegQoSParams Peak: %i", negParams.iPeakThroughput )
    LOG2( SIMPLE, "     GetNegQoSParams Mean: %i", negParams.iMeanThroughput )

    // Cleaning and closing: packetContext, qos
    CleanupStack::PopAndDestroy( KProposeQoSParametersPop );

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ProposeQoSParametersL exiting" )
    return negParams;
    }

// ---------------------------------------------------------------------------
// Opens session to GPRS Service
// ---------------------------------------------------------------------------
//
TInt CSatBIPUtils::GprsService()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::GprsService calling" )

    TInt ret( KErrNone );
    if ( !iPacketServiceActive )
        {
        LOG( SIMPLE, "SATENGINE: CSatBIPUtils::GprsService \
        iPacketServiceActive false" )
        ret = iPacketService.Open( *( iUtils.MultiModeApi().Phone() ) );
        iPacketServiceActive = ETrue;
        }

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::GprsService exiting" )
    return ret;
    }

// ---------------------------------------------------------------------------
// Gives reference to RPacketService
// ---------------------------------------------------------------------------
//
RPacketService& CSatBIPUtils::PacketService()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::PacketService calling" )

    GprsService();

    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::PacketService exiting" )
    return iPacketService;
    }

// ---------------------------------------------------------------------------
// CSatBIPUtils::ConnectionStatus
// ---------------------------------------------------------------------------
//
RPacketService::TStatus CSatBIPUtils::ConnectionStatus() const
    {
    LOG( NORMAL,
        "SATENGINE: CSatBIPUtils::ConnectionStatus calling" )

#ifndef SAT_USE_DUMMY_TSY
    RPacketService::TStatus status( RPacketService::EStatusSuspended );
#else
    RPacketService::TStatus status( RPacketService::EStatusActive );
#endif

    if ( iConnObserver )
        {
        LOG( NORMAL,
        "SATENGINE: CSatBIPUtils::ConnectionStatus iConnObserver true" )
        status = iConnObserver->Status();
        }

    LOG( NORMAL,
        "SATENGINE: CSatBIPUtils::ConnectionStatus exiting" )
    return status;
    }

// ---------------------------------------------------------------------------
// CSatBIPUtils::StopUdpLink
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::StopUdpLink()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::StopUdpLink  calling" )
    if ( iDataChannels )
        {
        const TInt channelCount( iDataChannels->Count() );
        LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::StopUdpLink \
          channelCount: %d", channelCount )
        // Cancel all channels
        for ( TInt index = 0; index < channelCount; index++ )
            {
            MSatBIPDataChannel* channel = iDataChannels->At( index );
            __ASSERT_ALWAYS( channel,
                PanicSatEngine( ESatEngineNullPointer ) );
            channel->StopUdpSocket();
            }
        }
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::StopUdpLink exiting" )
    }

// ---------------------------------------------------------------------------
// Synchronous request complete
// ---------------------------------------------------------------------------
//
TInt CSatBIPUtils::ContextCount()
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ContextCount calling" )

    // Number of contexts
    TInt count( 0 );
    TInt maxAllowed( 0 );

    // Init PacketService
    TInt error( GprsService() );

    if ( KErrNone == error )
        {
        // Enumerate contexts
        iPacketService.EnumerateContexts( 
            iWrapper->RequestStatus(), count, maxAllowed );
        error = iWrapper->SetActiveAndWait(); // Wait for completion
        LOG2( SIMPLE, "     EnumContexts Status: %i", error )
        LOG2( SIMPLE, "     EnumContexts Count: %i", count )
        LOG2( SIMPLE, "     EnumContexts MaxAllowed: %i", maxAllowed )
        // Check status
        if ( KErrNone != error )
            {
            LOG( SIMPLE, "SATENGINE: CSatBIPUtils::ContextCount \
            KErrNone != error" )
            count = 0;
            }
        }

    LOG2( SIMPLE, "SATENGINE: CSatBIPUtils::ContextCount exiting: %i", count )
    return count;
    }

// ---------------------------------------------------------------------------
// CSatBIPUtils::SetConnectionActivated()
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::SetConnectionActivated( TBool aSet )
    {
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::SetConnectionActivated calling" )
    iConnectionActivated = aSet;
    LOG( SIMPLE, "SATENGINE: CSatBIPUtils::SetConnectionActivated exiting" )
    }
    
// ---------------------------------------------------------------------------
// CSatBIPUtils::IsConnectionActivated()
// ---------------------------------------------------------------------------
//
TBool CSatBIPUtils::IsConnectionActivated()
    {
    LOG( SIMPLE, 
        "SATENGINE: CSatBIPUtils::IsConnectionActivated calling-exiting" )
    return iConnectionActivated;
    }

// ---------------------------------------------------------------------------
// CSatBIPUtils::SetSubConnectionActivated()
// ---------------------------------------------------------------------------
//
void CSatBIPUtils::SetSubConnectionActivated( TBool aSet )
    {
    LOG( SIMPLE, 
        "SATENGINE: CSatBIPUtils::SetSubConnectionActivated calling" )
    iSubConnectionActivated = aSet;
    LOG( SIMPLE, 
        "SATENGINE: CSatBIPUtils::SetSubConnectionActivated exiting" )
    }
    
// ---------------------------------------------------------------------------
// CSatBIPUtils::IsSubConnectionActivated()
// ---------------------------------------------------------------------------
//
TBool CSatBIPUtils::IsSubConnectionActivated()
    {
    LOG( SIMPLE, 
        "SATENGINE: CSatBIPUtils::IsSubConnectionActivated calling-exiting" )
    return iSubConnectionActivated;
    }

