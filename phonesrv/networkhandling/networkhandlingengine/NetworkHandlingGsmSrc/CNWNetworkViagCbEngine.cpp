/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CPENetworkViagEngine
*                 class member functions.
*
*/



// INCLUDE FILES
#include    "CNWNetworkViagCbEngine.h"
#include    "CNWNetworkViagBaseEngine.h"
#include    "CNWNetworkViagTimer.h"
#include    "NWPanic.pan"
#include    <networkhandlingdomainpskeys.h>
#include    "NWLogger.h"
#include    <cmcn.h>
#include    <cmcntopicarray.h>

// CONSTANTS
const TUint KNWzero = 0;
const TUint KNWCityZone = 3;
const TInt  KHZCBSMessageDataMaxLength = 2 * KNWViagHzCoordinateLength; // x-coordinate and y-coordinate i.e. '111111222222'


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::CNWNetworkViagCbEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//

       
CNWNetworkViagCbEngine::CNWNetworkViagCbEngine(
    CNWNetworkViagBaseEngine& aBase,
    TNWInfo& aNetworkInfo,
    RMmCustomAPI& aCustomAPI,
    TNWInterInfo& aInterNetworkInfo,
    CNWGsmNetworkCellReselectionHandler& aCellReselectionHandler )
      : 
        iBase ( aBase ),
        iNWNetworkInfo( aNetworkInfo ),
        iCustomAPI( aCustomAPI ),
        iInterNetworkInfo( aInterNetworkInfo ),
        iCellReselectionHandler( aCellReselectionHandler )
    {
    iCBSState = EListeningNone;
    
    NWLOGSTRING(KNWOBJECT, 
        "NW: CNWNetworkViagCbEngine::CNWNetworkViagCbEngine complete");
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::ConstructL()
    {
    NWLOGSTRING(KNWOBJECT, 
           "NW: CNWNetworkViagCbEngine::CNWNetworkViagCbEngine Begin");
    
    iTimer = CNWNetworkViagTimer::NewL();
    iElements = new ( ELeave ) RMmCustomAPI::TViagElements( 1 ); //create array
    iElements->ResizeL( RMmCustomAPI::KViagElementCount );
    iActiveZoneArray = new ( ELeave ) CViagZoneArray( 1 ); //create array
    iActiveZoneArray->ResizeL( RMmCustomAPI::KViagElementCount );
    iCacheHandler = CNWNetworkHZCbCacheHandler::NewL( iCustomAPI );
    
    NWLOGSTRING(KNWOBJECT, 
           "NW: CNWNetworkViagCbEngine::CNWNetworkViagCbEngine End");
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkViagCbEngine* CNWNetworkViagCbEngine::NewL(
                CNWNetworkViagBaseEngine& aBase,
                TNWInfo& aNetworkInfo, 
                RMmCustomAPI& aCustomAPI,
                TNWInterInfo& aInterNetworkInfo,
                CNWGsmNetworkCellReselectionHandler& aCellReselectionHandler )
    {
    NWLOGSTRING(KNWOBJECT, 
           "NW: CNWNetworkViagCbEngine::CNWNetworkViagCbEngine Begin");
    
    CNWNetworkViagCbEngine* self = new (ELeave) CNWNetworkViagCbEngine(
                aBase,
                aNetworkInfo,
                aCustomAPI,
                aInterNetworkInfo,
                aCellReselectionHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    NWLOGSTRING(KNWOBJECT, 
           "NW: CNWNetworkViagCbEngine::CNWNetworkViagCbEngine End");
    
    return self;
    }

    
// ----------------------------------------------------------------------------
// CNWNetworkViagEngine::~CNWNetworkViagCbEngine
// Destructor
// ----------------------------------------------------------------------------
//
CNWNetworkViagCbEngine::~CNWNetworkViagCbEngine()
    {
    NWLOGSTRING(KNWOBJECT, 
        "NW: CNWNetworkViagCbEngine::~CNWNetworkViagCbEngine Begin");
    
    if ( iMcn )
        {
        if( iCBSState == EListeningViag )
            {
            NWLOGSTRING( KNWINT, "NW: CNWnetworkviagengine > imcn.unregister" );
            iMcn->Unregister( this );
            }
        iMcn = NULL; // CNWGsmNetworkCellReselectionHandler has ownership. 
        }

    delete iActiveZoneArray;  //delete array
    delete iElements; //delete array
    delete iTimer; //delete timer
    delete iCacheHandler;
    
    NWLOGSTRING(KNWOBJECT, 
        "NW: CNWNetworkViagCbEngine::~CNWNetworkViagCbEngine() End");
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagEngine::CellInfoChanged
// Gets called when a message of a subscribed topic is received from the network.
// ----------------------------------------------------------------------------
//
TInt CNWNetworkViagCbEngine::CellInfoChanged(
    const TCbsMcnMessage& aMcnMessage ) //MCN message received from the network. 
    {
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagEngine::CellInfoChanged, topic = %d", 
        aMcnMessage.iTopicNumber );
    //HZI message, check that we really are listening it
    if( aMcnMessage.iTopicNumber == KNWHomeZoneMsgId && 
            iCBSState==EListeningViag )
        {
        TInt error = HandleCBSMessages( aMcnMessage );
        if(  error != KErrNone )
            {
            NWLOGSTRING2( KNWERROR, 
                "NW: CNWNetworkViagEngine::CellInfoChanged, HandleCBSMessages \
                error, error code = %d", error );
            }
        else
            {
            iBase.SendMessage( 
                 MNWMessageObserver::ENWMessageCurrentHomeZoneMessage );
            }
        }
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagEngine::CellInfoChanged() End" );
    
    //This is called by MCN client API, so we return always KErrNone
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CNWNetworkViagEngine::StopCbAlgo
// Stops CBS activities
// -----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::StopCbAlgo()
    {
    NWLOGSTRING( KNWINT, "NW: CNWNetworkViagCbEngine::StopCbAlgo() Begin" );
    iTimer->Cancel();
    iState=EIdle;
    if( iCBSState == EListeningViag )
        {
        if ( iMcn )
            {
            NWLOGSTRING( KNWINT, "NW: CNWNetworkViagEngine::StopCbAlgo: iMcn.Unregister" );
            iMcn->Unregister( this );
            }
        iCBSState=EListeningNone;
        }
    
    NWLOGSTRING( KNWINT, "NW: CNWNetworkViagCbEngine::StopCbAlgo() End" );
    }
    
// -----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::CellReselection
// Is called by CNWNetworkCurrentNetworkMonitor in case of cell re-selection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::CellReselection()
    {
    NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::CellReselection() Begin" );
    
    TUint cellId = iInterNetworkInfo.iCellId;
    iCellInfo.iCellId = static_cast<TUint16>( cellId );
    iCellInfo.iLac = static_cast<TUint16>( iInterNetworkInfo.iLac );
    
    // StartIndicatorhandling must be called even if cache reading fails.
    if ( iCacheHandler->InitialisingStatus() == ECBSHZCacheReader_Ready )
        {
        TRAPD( err, StartIndicatorHandlingL( EFalse ) )
        if( err!= KErrNone )
            {
            __ASSERT_DEBUG( EFalse, Panic( ENWPanicNotCompleted ) );
            }
        }
    else
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::CellReselection Cache Reading is Ongoing, Do nothing " );
        }
    
    NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::CellReselection() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::IsActiveZone
// Checks if there are active Viag Zones
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::IsActiveZone( )
    {
    NWLOGSTRING( KNWINT, 
                "NW: CNWNetworkViagCbEngine::CellReselection() Begin" );
    
    //Check that mobile is camping on the Viag network.
    TInt mcc = KNWzero;
    if ( TLex( iNWNetworkInfo.iCountryCode ).Val( mcc ) != KErrNone )
        {
        //No error handling convention to use with this error
        NWLOGSTRING( KNWERROR, "NW: ERROR: CNWNetworkViagCbEngine::ISACTIVEZONE ! TLEX OPERATION FAILED 1");
        }

    TInt mnc = KNWzero;
    if ( TLex( iNWNetworkInfo.iNetworkId ).Val( mnc ) != KErrNone )
        {
        //No error handling convention to use with this error
        NWLOGSTRING( KNWERROR, "NW: ERROR: CNWNetworkViagCbEngine::ISACTIVEZONE ! TLEX OPERATION FAILED 2");
        }

    NWLOGSTRING3( KNWINT, "NW: CNWNetworkViagCbEngine::CellReselection(), \
            mcc = %d, mnc = %d ", mcc, mnc );
    
    if( mcc == KNWViagMcc && mnc == KNWViagMnc )
        {
        TBool home( EFalse ); 
        TBool city( EFalse );
        
        TInt count = iActiveZoneArray->Count();
        
        NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::CellReselection(), \
                   number of active zones = %d", count );
        
        for ( TInt i=count-1; i>=0; i-- )
            {
            TViagZoneInfo info = iActiveZoneArray->At(i);
            if ( info.iActiveFlag == RMmCustomAPI::EHomeZone )
                {
                home=ETrue;
                //error is ignored, since it will always succeed.
                }
            else if ( info.iActiveFlag == RMmCustomAPI::ECityZone )
                {
                city=ETrue;
                //error code is ignored
                }
            }
        if(home && city)
            iIsActiveZones=EBothActive;
        else if(home)
            iIsActiveZones=EHomeActive;
        else if(city)
            iIsActiveZones=ECityActive;
        else
            iIsActiveZones=ENoneActive;
        }
    else    
        iIsActiveZones=ENoneActive;

    NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::IsActiveZone: Active zones(0 none, 1 hz, 2 cz, 3 both): %d", iIsActiveZones );

    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::UpdateViagHzParams
// Updates Viag HomeZone parameters from SIM.
// KErrNone if everything goes allright.
// Some error value if CustomAPI call or Append fails
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkViagCbEngine::UpdateViagHzParams()
    {
    NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::UpdateViagHzParams() Begin" );
    
    //Empty elements without free memory
    TInt count = iElements->Count();
    TInt i;

    NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::UpdateViagHzParams(), \
            iElements->Count() = %d", count );
    
    for ( i = count-1; i>=0; i-- )
        {
        iElements->Delete( i );
        }

    //For TSY requires this to be the array's size
    TRAPD( err, iElements->ResizeL( RMmCustomAPI::KViagElementCount ) )
        if( err != KErrNone )
            {
            //error handling
            NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::\
                UpdateViagHzParams(), elements array resize error: %d", err );
            return err;
            }

    NWLOGSTRING( KNWREQOUT, 
        "NW: CNWNetworkViagCbEngine::UpdateViagHzParams: \
        ReadViagHomeZoneParams()" );
    TInt error = iCustomAPI.ReadViagHomeZoneParams( iParams, *iElements );

    if( error == KErrNone )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::UpdateViagHzParams: params updated" );

        //Empty zoneArray without free memory
        count = iActiveZoneArray->Count();
        NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::\
             UpdateViagHzParams(), number of active zones: %d", count );
        
        for ( i = count-1; i>=0; i-- )
            {
            iActiveZoneArray->Delete( i );
            }

        count = iElements->Count();
        for ( i = KNWzero; i < count; i++ )
            {
            RMmCustomAPI::TViagElement& element = iElements->At( i );
            TViagZoneInfo info;

            info.iX = element.iCoordinates.iX;
            info.iY = element.iCoordinates.iY;
            info.iR2 = element.iCoordinates.iR2;
            info.iZoneId = element.iCoordinates.iZoneId;
            info.iName.Copy( element.iName );
            iBase.SetZoneTag( info.iZoneId-1, info.iName );
            NWLOGSTRING3( KNWINT, 
                "NW: CNWNetworkViagCbEngine::UpdateViagHzParams: \
                Zone Id: %d, element name: %S ", 
                info.iZoneId, &info.iName );
            NWLOGSTRING4( KNWINT, "ViagEngine::UpdateViagHzParams: \
                iX = %d, iY = %d, iR2 = %d", 
                info.iX, info.iY, info.iR2);

            switch( element.iActiveFlag )
                {
                case RMmCustomAPI::ENotActive:
                    info.iActiveFlag = RMmCustomAPI::ENotActive;
                    NWLOGSTRING( KNWINT, 
                        "NW: CNWNetworkViagCbEngine::UpdateViagHzParams \
                        ! ENotActive" );
                    break;
                case RMmCustomAPI::EHomeZone:
                    info.iActiveFlag = RMmCustomAPI::EHomeZone;
                    NWLOGSTRING( KNWINT, 
                        "NW: CNWNetworkViagCbEngine::UpdateViagHzParams !\
                        EHomeZone active" );
                    break;
                case RMmCustomAPI::ECityZone:
                    info.iActiveFlag = RMmCustomAPI::ECityZone;
                    NWLOGSTRING( KNWINT, 
                        "NW: CNWNetworkViagCbEngine::UpdateViagHzParams ! \
                        ECityZone active " );
                    break;
                default:
                    break;
                }

            // If zone is active, add it into iActiveZoneArray
            if ( element.iActiveFlag != RMmCustomAPI::ENotActive )
                {
                TRAPD( err, iActiveZoneArray->AppendL( info ) )//lint !e578
                if( err != KErrNone )
                    {
                    //error handling
                    NWLOGSTRING2( KNWERROR, 
                        "NW: CNWNetworkViagCbEngine::UpdateViagHzParams(),\
                        error code = %d", err );
                    return err;
                    }
                }
            }
        
        NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::\
             UpdateViagHzParams(), after loop running variable i = %d", i );
        }

    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagCbEngine::UpdateViagHzParams: returns %d", error );

    return error;
}

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::RegisterToCBS
// Registers to listening CBS messages
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::RegisterToCBSL(
    TUint16 aTopic )
    {
    NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::RegisterToCBSL() Begin,\
        aTopic = %d ", aTopic );
    
    if( !iMcn )
        {
        iMcn = iCellReselectionHandler.GetMcnPtr();
        }
 
    if( iMcn )
        {
        // Specify topics with CMcnTopicArray.
        CMcnTopicArray* topics = CMcnTopicArray::NewL();
        CleanupStack::PushL( topics );
        topics->AddCbTopicL( aTopic );      // MCN Info (cell info)

        // Register. Parameter observer is an instance of CCbsMcnObserver.
        NWLOGSTRING( KNWINT, "NW: CNWNetworkViagEngine::RegisterToCBSL > imcn.registerL()" );
        iMcn->RegisterL( this, *topics );
        iCBSState = EListeningViag;
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagEngine::RegisterToCBSL ! Registered to Viag messages" );

        // Start CBS message waiting timer
        iTimer->Cancel();
        iTimer->After( KViagTimerSixtySeconds, this );
        iState = ECbsSixtySecondsWaiting;
        NWLOGSTRING( KNWINT, "NW: CNWNetworkViagCbEngine::RegisterToCBSL 60s timer started" );
        CleanupStack::PopAndDestroy( topics );
        }

    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::RegisterToCBSL ! Registered to Viag \
        messages" );
    }   

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::StartIndicatorHandlingL
// Checks and indicates user if current zone is City/HomeZone.
// This method reads params from SIM card and checks if current zone is active 
// zone and if it is in SIM cache. It also starts 60 second timer to listen for 
// CBS messages.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::StartIndicatorHandlingL( TBool aRefreshOccureed )
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::StartIndicatorHandlingL Begin" );
    ResetIndicatorValues();
    IsActiveZone();
    
    NWLOGSTRING3( KNWINT, "NW: CNWNetworkViagCbEngine::StartIndicatorHandlingL(),\
            iIsActiveZones = %d, iInterNetworkInfo.iCellId = %d", iIsActiveZones,
            iInterNetworkInfo.iCellId );
    // Is there any active zones?
    if( iIsActiveZones && iInterNetworkInfo.iCellId != 0 )
        {
        TInt cache = SearchHomeZoneCaches( iCellInfo, iActiveZoneArray );
        if( cache >= EHZ_HomeZoneCache1 )
            {
            // Found from cache. Update values to UI.
            if( cache == EHZ_HomeZoneCache1 ||
                cache == EHZ_HomeZoneCache2 || 
                cache == EHZ_HomeZoneCache3 )
                {
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::StartIndicatorHandlingL ! \
                    Calling writeviagvalues" );  
                iBase.WriteViagValues( ENWViagIndicatorTypeHomeZone,
                                       cache - 1 );
                }
            else if( cache == EHZ_CityZoneCache )
                {
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::StartIndicatorHandlingL ! \
                    Calling writeviagvalues" );
                iBase.WriteViagValues( ENWViagIndicatorTypeCityZone,
                                     KNWCityZone );  // 3 is cityzone 
                }
            }            
       //----------------------------------------------------------- 
       // No cell found from cache 
       //------------------------------------------------------------ 
        else  
            {
            if( aRefreshOccureed )
                {
                if ( iMcn )
                    {
                    // When refresh is occured and we are in same cell the
                    // CBS message is not received so we fetch the old message
                    // from CBS server and use it like received CBS message.
                    NWLOGSTRING( KNWINT, 
                        "NW: CNWNetworkViagCbEngine::StartIndicatorHandlingL\
                        -> Fetch previous CBS Msg from Server" );
                      
                    // Initilize before fetching data from server
                    iCachedHZCBSMessage.iBuffer.Copy( KNullDesC );
                    iCachedHZCBSMessage.iBuffer8.Copy( KNullDesC8 );

                     TInt error = iMcn->GetInfoMessage( 
                             iCachedHZCBSMessage.iBuffer, KNWHomeZoneMsgId );
                    if ( !error )
                        {
                        NWLOGSTRING2( KNWINT, 
                              "NW: CNWNetworkViagCbEngine::StartIndicator\
                              HandlingL 'iCachedHZCBSMessage.iBuffer' = [%S]",
                              &iCachedHZCBSMessage.iBuffer );
                        HandleCBSMessages( iCachedHZCBSMessage );
                        }
                    NWLOGSTRING( KNWINT, 
                        "NW: CNWNetworkViagCbEngine::StartIndicatorHandlingL \
                        -> Simulated CBS message done" );
                    }
                 }
            }
        // Start listening CBS messages.        
        RegisterToCBSL( KNWHomeZoneMsgId );
        }
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::StartIndicatorHandlingL End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::HandleCBSMessages
// Checks and indicates user if current zone is City/HomeZone.
// This method compares the coordinates received by CBS message to zones that we
// are registered to.It also Adds and removes ids from SIM cache, according the 
// result of the comparation.
// Return Values:  KErrNone :Operation was done successfully
//                 KErrArgument :Incorrect message
//                 KErrGeneral :AddHomeZoneCache failed
//                 Possible error code from TLex.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkViagCbEngine::HandleCBSMessages(
    const TCbsMcnMessage& aMcnMessage ) //MCN message received from the network.
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::HandleCBSMessages() Begin" );
    TInt res( KErrNone );

    StopCbAlgo();
    
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagCbEngine::HandleCBSMessages 'CBS MSG' = [%S]",
        &aMcnMessage.iBuffer );

    //Decode Msg
    //7-bit alphabet packing decoding is made in CBs server.
    TPtr msgData =  aMcnMessage.iBuffer.LeftTPtr( KHZCBSMessageDataMaxLength );

    // Validate data and then process it.
    if ( ValidateHZCBSMessage( msgData ) )
        {
        TUint32 mX(0);
        TUint32 mY(0);
    
        TInt length = msgData.Length();

        TInt lexErr = TLex( msgData.Right( KNWViagHzCoordinateLength ) ).Val( 
            mY, EDecimal );

        NWLOGSTRING2( KNWINT, 
            "NW: CNWNetworkViagCbEngine::HandleCBSMessages(),\
            lexErr = %d ", lexErr );
        
        if( lexErr == KErrNone )
            {
            TInt xpos = length - Min( length, KNWViagHzMsgLength );
             
            lexErr = TLex( msgData.Mid( xpos, 
                (length-KNWViagHzCoordinateLength)-xpos ) ).Val( 
                mX, EDecimal );
            }

        if( lexErr == KErrNone )
            {
            TInt zone = CheckCellInHomeZoneArray( mX, mY );
            //If it wasn't in any zone, make sure it's removed from cache
            if( zone==KErrNotFound )
                {
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::HandleCBSMessages ! \
                    Remove from cache" );
                if( RemoveFromHomeZoneCaches( iCellInfo ) )
                    {
                    ResetIndicatorValues();
                    }
                }
            else
                {
                res = DoHandleAddToCBSHZCache( ( 
                        THomeZoneCacheId ) zone, iCellInfo );
                }
            }
        else
            {
            //No error handling convention to use with this error
            NWLOGSTRING( KNWERROR, 
                "NW: HandleCBSMessages ! TLEX OPERATION FAILED");
            }
        }
    else //length != 2*KPhoneViagHzCoordinateLength
        {
        //incorrect message. Nothing to do
        res=KErrArgument;
        }
    
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagCbEngine::HandleCBSMessages() End, \
        res = %d ", res );
    return res; 
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::DoHandleAddToCBSHZCache
// 
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkViagCbEngine::DoHandleAddToCBSHZCache( 
    const THomeZoneCacheId aCache,
    const RMmCustomAPI::TViagCacheRecordContent aCellInfo )
    {
    NWLOGSTRING4( KNWINT, 
        "NW: CNWNetworkViagCbEngine::DoHandleAddToCBSHZCache() Begin, \
        aCache = %d, aCellInfo.iLac = %d, aCellInfo.iCellId = %d ",
        aCache, aCellInfo.iLac, aCellInfo.iCellId );

    ASSERT( aCache >= EHZ_HomeZoneCache1 && aCache <= EHZ_CityZoneCache );
    TInt retVal = KErrNone;
    
    // Step 1: Remove the cell from other caches
    // .. from higher priority ones
    for( TInt cacheId = EHZ_HomeZoneCache1; cacheId < aCache; cacheId++ )
        {
        TInt pos = iCacheHandler->Find( ( 
                THomeZoneCacheId ) cacheId, aCellInfo );
        if( pos != KErrNotFound )
            {
             NWLOGSTRING2( KNWINT, 
                "NW: DoHandleAddToCBSHZCache(), Old item found from cache [%d], remove it",
                cacheId );
           if( iCacheHandler->Remove( ( THomeZoneCacheId ) cacheId, pos ) )
                {
                return KErrGeneral;
                }
            }
        }
    // .. and lower priority ones
    for( TInt cacheId = EHZ_CityZoneCache; cacheId > aCache; cacheId-- )
        {
        TInt pos = iCacheHandler->Find( ( 
                THomeZoneCacheId ) cacheId, aCellInfo );
        if( pos != KErrNotFound )
            {
             NWLOGSTRING2( KNWINT, 
                "NW: DoHandleAddToCBSHZCache(), Old item found from cache \
                [%d], remove it", cacheId );
            if( iCacheHandler->Remove( ( THomeZoneCacheId ) cacheId, pos ) )
                {
                return KErrGeneral;
                }
            }
        }
        
    // Step 2: Update the cache if needed
    // Check if the cell exists already in cache
    TInt pos = iCacheHandler->Find( ( THomeZoneCacheId ) aCache, aCellInfo );
    if( pos == KErrNotFound )
        {
        pos = AddToHomeZoneCache( ( THomeZoneCacheId ) aCache, iCellInfo );
        if( pos < KErrNone )
            {
            retVal = KErrGeneral;
            }
        }
    
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagCbEngine::DoHandleAddToCBSHZCache() End, \
        retVal = %d ", retVal );
    
    return retVal;
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::ValidateHZCBSMessage
// Validates that aMsgData contains valid HomeZone CBS Message data
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TBool CNWNetworkViagCbEngine::ValidateHZCBSMessage( const TDesC & aMsgData )
    {
    NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::ValidateHZCBSMessage() Begin" );
    
    // ASSERT HERE
    TBool retVal ( EFalse );
    TInt msgLength = aMsgData.Length();
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagCbEngine::ValidateHZCBSMessage 'aMsgData' = [%S]",
        &aMsgData );
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagCbEngine::ValidateHZCBSMessage 'msgLength' = [%d]",
        msgLength );

    // Validate the length
    if( msgLength == KHZCBSMessageDataMaxLength )
        {
        TInt pos;
        
        for( pos = 0; pos < msgLength; pos++ )
            {
            // Validate the content. Only digits allowed.
            TChar tmpChar = aMsgData[ pos ];
            if( !tmpChar.IsDigit() )
                {
                break;
                }
            }
        
        NWLOGSTRING2( 
            KNWINT, "NW:CNWNetworkViagCbEngine::ValidateHZCBSMessage,\
                    after loop running variable pos = %d ", pos );
        
        // Check if the loop was run at the end.
        if( pos == msgLength )
            {
            retVal = ETrue;
            }
        }
    
    NWLOGSTRING2( KNWINT, "NW: CNWNetworkViagCbEngine::ValidateHZCBSMessage()\
             End, retVal = %d ", retVal );
    // Contains only numbers
    return retVal;
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::CheckCellInHomeZoneArray
// Checks if current zones coordinates are within some active Home-/CityZone limits.
// Return Values:  HomeZones number that matched the limits
//                 KErrNotFound :No registered Zones were in limits
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkViagCbEngine::CheckCellInHomeZoneArray( TUint32 aX, TUint32 aY )
    {
    NWLOGSTRING3( KNWINT, 
        "NW: CNWNetworkViagCbEngine::CheckCellInHomeZoneArray()\
        Begin, parameter aX = %d, parameter aY = %d", aX, aY );
    
    //Check if current cell is within HomeZone or CityZone
    TInt currentZone = KErrNotFound;
    for ( TInt i = KNWzero; i < iActiveZoneArray->Count(); i++ )
        {
        TViagZoneInfo zoneInfo = iActiveZoneArray->At( i );

        TInt32 dx = Abs( static_cast<TInt32>( aX-zoneInfo.iX ) ); 
        TInt32 dy = Abs( static_cast<TInt32>( aY-zoneInfo.iY ) ); 

        if ( dx > KNWViagHomeZoneMaxRad || dy > KNWViagHomeZoneMaxRad )
            {
            //Checking next homezone
            continue;
            }

        TUint32 d2 = dx*dx + dy*dy;//lint !e732

        if ( d2 < zoneInfo.iR2 )
            {
            //Current cell is inside the home-/cityzone
            currentZone = zoneInfo.iZoneId;
       
            NWLOGSTRING3( KNWINT, 
                "NW: CNWNetworkViagCbEngine::CheckCellInHomeZoneArray ! R2 \
                is %d, matches to zone nbr: %d", d2, currentZone );
            if( zoneInfo.iActiveFlag == RMmCustomAPI::ECityZone ) //This is CityZone
                {
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::CheckCellInHomeZoneArray ! \
                    calling writeviagvalues" );
                
                iBase.WriteViagValues( ENWViagIndicatorTypeCityZone,
                                       zoneInfo.iZoneId - 1 );
                
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::CheckCellInHomeZoneArray ! \
                    cityzoneinfo updated" );
                }
            else    //This is HomeZone
                {
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::CheckCellInHomeZoneArray ! \
                    calling writeviagvalues" );
                iBase.WriteViagValues( ENWViagIndicatorTypeHomeZone,
                                          zoneInfo.iZoneId - 1 );
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagCbEngine::CheckCellInHomeZoneArray ! \
                    homezoneinfo updated" );
                }

            break;
            }
        }
    
    NWLOGSTRING2( KNWINT, 
            "NW: CNWNetworkViagCbEngine::CheckCellInHomeZoneArray()\
            End, currentZone = %d ", currentZone );
    return currentZone;
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::SearchHomeZoneCaches
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkViagCbEngine::SearchHomeZoneCaches( 
    const RMmCustomAPI::TViagCacheRecordContent aCellInfo,
    const CViagZoneArray * aActiveZones  )
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::SearchHomeZoneCaches() <- " );
    NWLOGSTRING2( KNWINT, 
        "NW: SearchHomeZoneCaches(), Item to be found: LAC [%d]",
        aCellInfo.iLac );
    NWLOGSTRING2( KNWINT, 
        "NW: SearchHomeZoneCaches(), Item to be found: CellId [%d]",
        aCellInfo.iCellId );

    TInt retVal = KErrNotFound;
       
    for ( TInt i = 0; i < aActiveZones->Count(); i++ )
        {
        const TViagZoneInfo & zoneInfo = aActiveZones->At( i );
        TInt cacheIndex = zoneInfo.iZoneId;
        
        TInt index = iCacheHandler->Find( ( THomeZoneCacheId ) cacheIndex, 
            aCellInfo );
            
        if( index > KErrNotFound )
            {
            // Found from cache
            // ...set return value
            NWLOGSTRING2( KNWINT, 
                "NW: CNWNetworkViagCbEngine::SearchHomeZoneCaches(): \
                Found from cache: [%d]", cacheIndex );
            retVal = cacheIndex;
            break;
            }
        }

    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::SearchHomeZoneCaches() -> " );

    return retVal;
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::AddToHomeZoneCache
//
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkViagCbEngine::AddToHomeZoneCache( 
    const THomeZoneCacheId aZone, 
    const RMmCustomAPI::TViagCacheRecordContent aCellInfo )
    {
    NWLOGSTRING4( KNWINT, "NW: CNWNetworkViagCbEngine::AddToHomeZoneCache() \
        Begin, aZone = %d, aCellInfo.iLac = %d, aCellInfo.iCellId = %d", 
        aZone, aCellInfo.iLac, aCellInfo.iCellId );

    TInt retVal = KErrNotFound;
    
    // Find a zoneInfo which corresponds to aZone
    TInt zoneIndex = -1;
    TInt activeZones = iActiveZoneArray->Count();
 
    for( zoneIndex = 0; zoneIndex < activeZones; zoneIndex++ )
        {
        TViagZoneInfo& zoneInfo = iActiveZoneArray->At( zoneIndex );
        if( zoneInfo.iZoneId == ( TUint ) aZone )
            {
            // Write updated data in cache
            retVal = iCacheHandler->Insert( ( THomeZoneCacheId ) aZone, 
                aCellInfo );
            NWLOGSTRING2( KNWINT, 
                "NW: CNWNetworkViagEngine::AddToHomeZoneCache, Added in \
                CACHE [%d]", aZone );
            NWLOGSTRING2( KNWINT, 
                "NW: CNWNetworkViagEngine::AddToHomeZoneCache, Added by \
                INDEX [%d]", retVal );
            break;
            }
        }
        
    NWLOGSTRING2( KNWINT, 
            "NW: CNWNetworkViagCbEngine::AddToHomeZoneCache(), after loop \
            running variable zoneIndex = %d ", zoneIndex );
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagCbEngine::AddToHomeZoneCache() End, \
        retVal = %d ", retVal );

    return retVal;
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::HandleTimeOutL
// Handles timeout.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::HandleTimeOut()
    {
    NWLOGSTRING( KNWINT, "NW: CNWNetworkViagCbEngine::HandleTimeOut() Begin ");
    
    switch ( iState )
        {
        case ECbsSixtySecondsWaiting:
            {
            //Unregisters when 60/55sec timer expires
            NWLOGSTRING( KNWINT, 
                "NW: CNWNetworkViagCbEngine::HandleTimeOut ! 60s timer \
                expired" );
            if ( iMcn )
                {
                NWLOGSTRING( KNWINT, 
                    "NW: CNWNetworkViagEngine::HandleTimeOut > imcn.unre\
                    gister" );
                iMcn->Unregister( this );
                }
            iState=EIdle;
            iCBSState=EListeningNone;
            }
            break;
        case EIdle:
        default:
            __ASSERT_DEBUG( EFalse, Panic( ENWPanicNotCompleted ) );
            break;
        }
    
    NWLOGSTRING( KNWINT, "NW: CNWNetworkViagCbEngine::HandleTimeOut() End ");
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::ResetIndicatorValues
// Resets Viag's and MCN display's TPEInfo values.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::ResetIndicatorValues()
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::ResetIndicatorValues() Begin");
    
    iBase.WriteViagValues( ENWViagIndicatorTypeNone, NULL );
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::ResetIndicatorValues() End ");
    }



// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::SatRefreshCompleteNotification
// If homezone files are refreshed this method is called.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::SatRefreshCompleteNotification()
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::SatRefreshCompleteNotification() Begin" );
    StopCbAlgo(); // Stop listening CBS Messages
    TInt error = UpdateViagHzParams(); // Get updated data from SIM
    //If params not found, do nothing
    if( error )
        {
        NWLOGSTRING2( KNWERROR, 
            "NW: ERROR: CNWNetworkViagCbEngine::STARTINDICATORHANDLINGL ! \
            UPDATEVIAGHZPARAMS RETURNED %d", error );
        ResetIndicatorValues();
        }
    else
        {
        ClearHomeZoneCaches(); // Clear cache
        TRAPD( err, StartIndicatorHandlingL( ETrue ) )
        if( err!= KErrNone )
            {
            NWLOGSTRING2( KNWERROR, 
                "NW: ERROR: CNWNetworkViagCbEngine::STARTINDICATORHANDLINGL ! \
                StartIndicatorHandlingL returned %d", err );
            __ASSERT_DEBUG( EFalse, Panic( ENWPanicNotCompleted ) );
            }
        NWLOGSTRING( KNWINT,
            "NW: CNWNetworkViagCbEngine::SatRefreshCompleteNotification() ->" );
        }
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::SatRefreshCompleteNotification() End" );
    }



// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::ClearHomeZoneCaches
// If homezone files are refreshed this method is called.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagCbEngine::ClearHomeZoneCaches()
    { 
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::ClearHomeZoneCaches() Begin " );
    iCacheHandler->Clear( EHZ_HomeZoneCache1 );
    iCacheHandler->Clear( EHZ_HomeZoneCache2 );
    iCacheHandler->Clear( EHZ_HomeZoneCache3 );
    iCacheHandler->Clear( EHZ_CityZoneCache );
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::ClearHomeZoneCaches() End " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::RemoveFromHomeZoneCaches
// 
// ----------------------------------------------------------------------------
//    
TBool CNWNetworkViagCbEngine::RemoveFromHomeZoneCaches(
    const RMmCustomAPI::TViagCacheRecordContent aCellInfo )
    {
    NWLOGSTRING3( KNWINT, 
            "NW: CNWNetworkViagCbEngine::RemoveFromHomeZoneCaches() Begin, \
            aCellInfo.iLac = %d, aCellInfo.iCellId = %d ",
            aCellInfo.iLac, aCellInfo.iCellId );
        
    TInt index = iCacheHandler->Find( EHZ_HomeZoneCache1, aCellInfo );
    if( index != KErrNotFound )
        {
        NWLOGSTRING2( KNWINT, 
            "NW: RemoveFromHomeZoneCaches(), The item found from cache [%d]\
            , remove it", EHZ_HomeZoneCache1 );
        if( iCacheHandler->Remove( EHZ_HomeZoneCache1, index ) )
            {
            return EFalse;
            }
        }
    index = iCacheHandler->Find( EHZ_HomeZoneCache2, aCellInfo );
    if( index != KErrNotFound )
        {
        NWLOGSTRING2( KNWINT, 
            "NW: RemoveFromHomeZoneCaches(), The item found from cache [%d]\
            , remove it",
            EHZ_HomeZoneCache2 );
        if( iCacheHandler->Remove( EHZ_HomeZoneCache2, index ) )
            {
            return EFalse;
            }
        }
    index = iCacheHandler->Find( EHZ_HomeZoneCache3, aCellInfo );
    if( index != KErrNotFound )
        {
        NWLOGSTRING2( KNWINT, 
            "NW: RemoveFromHomeZoneCaches(), The item found from cache [%d]\
            , remove it",
            EHZ_HomeZoneCache3 );
        if( iCacheHandler->Remove( EHZ_HomeZoneCache3, index ) )
            {
            return EFalse;
            }
        }
    
    index = iCacheHandler->Find( EHZ_CityZoneCache, aCellInfo );
    if( index != KErrNotFound )
        {
        NWLOGSTRING2( KNWINT, 
            "NW: RemoveFromHomeZoneCaches(), The item found from cache [%d]\
            , remove it",
            EHZ_CityZoneCache );
        if( iCacheHandler->Remove( EHZ_CityZoneCache, index ) )
            {
            return EFalse;
            }
        }

    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkViagCbEngine::RemoveFromHomeZoneCaches() End " );
    
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::InitilizeCaches
// Starts cb cache initialization
// ----------------------------------------------------------------------------
//  
void CNWNetworkViagCbEngine::InitilizeCaches()
    {
    NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::InitilizeCaches() Begin " );
    
    iCacheHandler->InitialiseHomeZoneCachesAsync( this );
    
    NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::InitilizeCaches() End " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagCbEngine::NotifyCachesInitialised
// Gets called when Cb cache reading is complete. 
// ----------------------------------------------------------------------------
// 
void CNWNetworkViagCbEngine::NotifyCachesInitialised()
    {
    NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::NotifyCachesInitialised() Begin " );
    
    iBase.CellReselection();
    
    NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkViagCbEngine::NotifyCachesInitialised() End " );
    }

//  End of File
