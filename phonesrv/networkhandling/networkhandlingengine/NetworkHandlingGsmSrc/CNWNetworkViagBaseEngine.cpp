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
* Description:  This module contains the implementation of CNWNetworkViagBaseEngine
*                class member functions.
*
*/


// INCLUDE FILES
#include    <ctsydomainpskeys.h> 
#include    <e32property.h>
#include    <PSVariables.h>
#include    <startupdomainpskeys.h>

#include    "CNWNetworkViagBaseEngine.h"
#include    "CNWNetworkViagCbEngine.h"
#include    "CNWNetworkViagLcEngine.h"
#include    "NWLogger.h"
#include    "CNWNetworkViagRefresh.h"
#include    "CNWNetworkViagSimReader.h"
#include    "CNWNetworkSystemEventMonitor.h"
#include    "CNWGsmMessageHandler.h"
#include    "NWPanic.pan"
#include    <networkhandlingdomainpskeys.h>
#include    "CNWNetworkViagTimer.h"

// CONSTANTS
const TUint KNWzero = 0;
const TInt KArrayCount = 8;
const TInt KNWTimeOut = 2000000;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::CNWNetworkViagBaseEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkViagBaseEngine::CNWNetworkViagBaseEngine(
        CNWGsmNetworkCellReselectionHandler& aOwner,
        CNWGsmMessageHandler& aMessageHandler,
        RMmCustomAPI& aCustomAPI,
        TNWInfo& aNetworkInfo,
        TNWInterInfo& aInterNetworkInfo
         ):
           iOwner ( aOwner ),
           iMessageHandler( aMessageHandler ),
           iCustomAPI( aCustomAPI ),
           iNWNetworkInfo( aNetworkInfo ),
           iInterNetworkInfo( aInterNetworkInfo ),
           iLcSupported ( EFalse ),
           iCbActivation( EFalse ),
           iSimOk ( EFalse ),
           iActivation( EFalse ),
           iInitReady( ),
           iReadingOnGoing( EFalse )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::CNWNetworkViagBaseEngine() Called." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagBaseEngine::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::ConstructL() Begin." );

    RProperty::TType type( RProperty::EInt );
    TSecurityPolicy readPolicy( ECapability_None );
    TSecurityPolicy writePolicy( ECapabilityWriteDeviceData );
    TInt err = RProperty::Define( KPSUidNetworkInfo, 
                                KNWHomeZoneStatus, 
                                type, 
                                readPolicy, 
                                writePolicy );
    
    //An error of KErrAlready exists should be ignored, as this only
    //indicates that some other code int system is also interested in the 
    //value and has created the property.
    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        NWLOGSTRING2( KNWERROR, "NW: CNWNetworkViagBaseEngine::ConstructL \
                RProperty::Define error: %d .", err );
        User::Leave( err );
        }    
       
    RProperty::Set( KPSUidNetworkInfo, KNWHomeZoneStatus, ENWNone );
    
    iSimStateMonitor = CNWNetworkSystemEventMonitor::NewL( *this,  
                                                        KPSUidStartup, 
                                                        KPSSimStatus ); 
    
    iCallStateMonitor = CNWNetworkSystemEventMonitor::NewL( 
            *this, KPSUidCtsyCallInformation, KCTsyCallState );    

    iGPRSStateMonitor = CNWNetworkSystemEventMonitor::NewL( 
            *this, KUidSystemCategory,KPSUidGprsStatusValue );

    iViagCbEngine = CNWNetworkViagCbEngine::NewL( *this, 
                                                iNWNetworkInfo, 
                                                iCustomAPI, 
                                                iInterNetworkInfo, 
                                                iOwner );

   // Sim status monitor.
    iViagSimReader = CNWNetworkViagSimReader::NewL( *this,  iCustomAPI );

    iViagLcEngine = CNWNetworkViagLcEngine::NewL( *this, 
                                                iNWNetworkInfo, 
                                                iCustomAPI, 
                                                iInterNetworkInfo );
                                                
    iZoneTags = new ( ELeave ) CZoneTagsArray( 1 );
    iZoneTags->ResizeL( KArrayCount );
    iSPNEFRead = ETrue;
    iNPNEFRead = ETrue;
    iProgEFRead = ETrue;
    iViagEFRead = ETrue;
    iTimer = CNWNetworkViagTimer::NewL();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::ConstructL() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkViagBaseEngine* CNWNetworkViagBaseEngine::NewL(
                CNWGsmNetworkCellReselectionHandler& aOwner,
                CNWGsmMessageHandler& aMessageHandler,
                RMmCustomAPI& aCustomAPI,
                TNWInfo& aNetworkInfo,
                TNWInterInfo& aInterNetworkInfo
                )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::NewL() Begin." );
    
    CNWNetworkViagBaseEngine* self = new (ELeave) CNWNetworkViagBaseEngine( 
            aOwner, aMessageHandler, aCustomAPI, aNetworkInfo, 
            aInterNetworkInfo );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::NewL() End." );
    return self;
    }

    
// Destructor
CNWNetworkViagBaseEngine::~CNWNetworkViagBaseEngine()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::~CNWNetworkViagBaseEngine() Begin." );
    delete iCharacterSetConverter;
    iFs.Close();
    delete iViagRefresh;
    delete iZoneTags;
    delete iViagLcEngine;
    delete iViagSimReader;
    delete iViagCbEngine;
    delete iGPRSStateMonitor;
    delete iCallStateMonitor;
    delete iSimStateMonitor;

    StopTimer();
    delete iTimer;
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::~CNWNetworkViagBaseEngine() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::CellReselection
// Is called by CNWNetworkCurrentNetworkMonitor in case of cell re-selection.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagBaseEngine::CellReselection()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkViagBaseEngine::CellResection() Begin" );

    // SIM OK
    // VIAG IMSI OK
    // System UP ...

    NWLOGSTRING2( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::CellResection \
                        iSimOk = %d ", iSimOk );
    // Sim is ok and imsi is right
    if ( iSimOk && IsViagImsi() )
        {
        // ... System is ready to proceed with HomeZone algoritmh(s)!!
        if ( iInitReady == EInitReady )
            {
            iViagActiveAlgo = EWNAlgoNone ;
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagBaseEngine::CellReselection StopCbAlgo()" );
            iViagCbEngine->StopCbAlgo();
            //  general activation is in default ETrue and CB activation EFalse   
            iViagLcEngine->GetActivation( iActivation, iCbActivation );
            
            NWLOGSTRING3( KNWOBJECT, 
                "NW: CNWNetworkViagBaseEngine::CellResection \
                iActivation = %d, iCbActivation = %d ", 
                iActivation, iCbActivation );
            
            NWLOGSTRING2( KNWOBJECT, 
                "NW: CNWNetworkViagBaseEngine::CellResection \
                iNetworkMode = %d", iNWNetworkInfo.iNetworkMode );

            if ( iActivation )
                {
                if ( iNWNetworkInfo.iNetworkMode == ENWModeWcdma )  // Network mode is WCDMA AND
                    {
                    if( iLcSupported )
                        {
                        iViagActiveAlgo = EWNAlgoLc ;    // UICC is "new geration" O2 de UICC    
                        }
                    }
                else  // GSM                                                                                // Network mode is GSM 
                    {
                    // Gsm mode + there is NOT gsm cells subscribed in LAC files
                    // Gsm mode + Cb-activation bit is "ON"
                    // Gsm mode + UICC is NOT "new geration" O2 de UICC 
                    iViagActiveAlgo = EWNAlgoCb ;       
                    if ( iViagLcEngine->IsGsmCellSubscribed()  )  // There is subscribed  gsm cells in LAC files.
                        {
                        if ( !iCbActivation )   // AND Cb-activation bit is off.
                            {
                            iViagActiveAlgo = EWNAlgoLc ;           
                            }                   
                        }
                    }
                }
            NWLOGSTRING2( KNWOBJECT, 
                "NW: CNWNetworkViagBaseEngine::CellReselection(),\
                iAreaKnown = %d",iInterNetworkInfo.iAreaKnown );
            // Reset  the indications
            WriteViagValues( ENWViagIndicatorTypeNone, KNWzero ); 
            if ( iNWNetworkInfo.iStatus == ENWStatusCurrent 
                 && iInterNetworkInfo.iAreaKnown )
                {
                if ( iViagActiveAlgo == EWNAlgoLc )
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagBaseEngine::ACTIVE ALGO ***LC***" );
                    iViagLcEngine->CellReselection();
                    }
                else if( iViagActiveAlgo == EWNAlgoCb )
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagBaseEngine::ACTIVE ALGO ***CB***" );
                    iViagCbEngine->CellReselection();
                    }

                else if ( iViagActiveAlgo == EWNAlgoNone )
                    {
                    NWLOGSTRING( KNWOBJECT, 
                        "NW: CNWNetworkViagBaseEngine::ACTIVE ALGO ***NONE***" );
                    }
                }
            }
        // ... System is NOT yet fully initialised
        else if( iInitReady == EInitNotReady && !iCharacterSetConverter ) 
            {
            TRAP_IGNORE( InitializeCharSetConverterL() );
            NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::CellResection \
                    System is NOT yet fully initialised " );
            ViagInitialize(); 
            // Sat server is not up in startup 
            iActiveCall = EFalse;
            }
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::CellResection() End" );
    }

    
// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::SatRefreshCompleteNotification
// Gets called when satrefresh is complete.
// ----------------------------------------------------------------------------
//   
void CNWNetworkViagBaseEngine::SatRefreshCompleteNotification()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::SatRefreshCompleteNo\
        tification() Begin" );
    
    // When the HZ file of caches are refreshed then notify CB-Engine.
    if( iViagCbEngine )
        {
        iViagCbEngine->SatRefreshCompleteNotification(); 
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::SatRefreshCompleteNotification end" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::ViagInitialize
// Initialize viag elements files.
// ----------------------------------------------------------------------------
//   
void CNWNetworkViagBaseEngine::ViagInitialize()
    {               
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::ViagInitialize() Begin" );
    iInitReady = EInitOngoing;
    // In start up read all elementary files that are needed.
    if ( iViagSimReader )
        {
        iViagSimReader->SetElementFile( EFileSettings );
        iViagSimReader->SetElementFile( EFileSubscribedOne );
        iViagSimReader->SetElementFile( EFileSubscribedTwo );
        iViagSimReader->SetElementFile( EFileSubscribedThree );
        iViagSimReader->SetElementFile( EFileSubscribedFour );
        iViagSimReader->SetElementFile( EFileTags );
        iViagSimReader->ReadSimFile();   
        }
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::ViagInitialize() Complete" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::UpdateCache
// Update cache
// ----------------------------------------------------------------------------
//   
TBool CNWNetworkViagBaseEngine::UpdateCache( 
        TNWNetworkElementFile aElementFile, 
        const TDesC8& aResponse )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::UpdateCache() Begin\
        , aElementFile = 0x%x", aElementFile );  
    
    NWLOGSTRING2( KNWINT,
        "NW: CNWNetworkViagBaseEngine::UpdateCache(),\
        iActivation = %d", iActivation ); 
    
    ASSERT( iViagLcEngine );
    ASSERT( iViagCbEngine );

    if( iViagLcEngine )
        {
        if ( aElementFile == EFileSettings ||
                ( aElementFile != EFileSettings && iActivation ) )
            {
            iViagLcEngine->ParseResult( aElementFile ,  aResponse );
            if ( !iActivation )
                {
                iViagLcEngine->GetActivation( iActivation, iCbActivation );
                }
            if ( aElementFile == EFileTags && iInitReady == EInitOngoing )
                {
                iInitReady = EInitReady;
                if( iViagCbEngine )
                    {
                    iViagCbEngine->InitilizeCaches();
                    iViagCbEngine->UpdateViagHzParams();
                    }
                }
            }
        }
        
    NWLOGSTRING2( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::UpdateCache() \
            End, length of aResponse = %d", aResponse.Length() );
    return iActivation;    
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::SetLcNotSupported
// Set LC support to False.
// ----------------------------------------------------------------------------
//   
void  CNWNetworkViagBaseEngine::SetLcSupported( TBool aLcSupported )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::SetLcSupported() \
        Begin, iLcSupported = %d  ", aLcSupported );
    
    ASSERT( iViagLcEngine );
    ASSERT( iViagCbEngine );
    
    iLcSupported = aLcSupported;
    if ( iLcSupported == EFalse )
        {
        iInitReady = EInitReady;
        if ( !iActivation )
            {
            iViagLcEngine->GetActivation( iActivation, iCbActivation );
            }
        if( iViagCbEngine && iActivation )
            {
            iViagCbEngine->InitilizeCaches();
            iViagCbEngine->UpdateViagHzParams();
            // If cell information is allready avaiable call cell reselection.
             if ( iInterNetworkInfo.iCellId != 0 )
                {
                CellReselection();       
                }
            }
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::SetLcSupported() end" );
    }
// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::SendMessage
// Reroutes a message to the Client object.
// ----------------------------------------------------------------------------
// 
void CNWNetworkViagBaseEngine::SendMessage(
        MNWMessageObserver::TNWMessages aMessage ) // send message
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::SendMessage(),\
         aMessage = %d", aMessage );
    
    iMessageHandler.SendMessage( aMessage );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::SendMessage() End" );
    }
    
    
   
// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::WriteViagValues
// Writes Viag-values to networkinfo, shared data and system agent.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkViagBaseEngine::WriteViagValues(
    const  TNWViagIndicatorType aIndicatorType,    //Indicates which zone it is
    const TInt& aZoneId )       // Viag text 
    {

    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::WriteViagValues() \
        Begin, aIndicatorType = %d, aZoneId = %d", 
        aIndicatorType,aZoneId );

    TInt zoneType = ENWNone;

    // Intilise internal datastructure by defaul values (Not in any zone)
    iNWNetworkInfo.iViagIndicatorType = ENWViagIndicatorTypeNone;
    iNWNetworkInfo.iViagTextTag.Zero();

    // Get zone values
    switch( aIndicatorType )
        {
        // HomeZone
        case ENWViagIndicatorTypeHomeZone:
            if( !iActiveCall )
                {
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkViagBaseEngine::WriteViagValues() \
                    iZoneTags->Count = %d", iZoneTags->Count() );
                
                if ( aZoneId < iZoneTags->Count() )
                    {
                    TNWZoneTags zoneTags = iZoneTags->At( aZoneId );
                    iNWNetworkInfo.iViagIndicatorType = ENWViagIndicatorTypeHomeZone;
                    iNWNetworkInfo.iViagTextTag.Copy( zoneTags.iZoneTag ); //Array starts from 0.
                    }
                zoneType = ENWHomeZone;
                }
            break;
         //CityZone and the other known zones
         case ENWViagIndicatorTypeCityZone:
            if( !iActiveCall )
                {
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkViagBaseEngine::WriteViagValues() \
                    iZoneTags->Count = %d", iZoneTags->Count() );
                
                if ( aZoneId < iZoneTags->Count() )
                    {
                    TNWZoneTags zoneTags = iZoneTags->At( aZoneId );
                    iNWNetworkInfo.iViagIndicatorType = ENWViagIndicatorTypeCityZone;
                    iNWNetworkInfo.iViagTextTag.Copy( zoneTags.iZoneTag ); //Array starts from 0.
                    }
                zoneType = ENWCityZone;
                }
             break;
             // NoZone or UnknownZone
         case ENWViagIndicatorTypeNone:
         default:
             break;
         }
               

    // Update the PubSub key value
    TInt err = RProperty::Set( KPSUidNetworkInfo, KNWHomeZoneStatus, zoneType );
    if ( err != KErrNone )
        {
        NWLOGSTRING2( KNWERROR, 
            "NW: CNWNetworkViagEngine::WriteViagValues: RProperty\
            ::Set error: %d", err );
        }

    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::WriteViagValues() Zone Text %S",
        &iNWNetworkInfo.iViagTextTag );
    NWLOGSTRING2( KNWINT,
        "NW: CNWNetworkViagEngine::WriteViagValues: RProperty\
        ::Set, zoneType = %d",
        zoneType );

    // Send upodate messages
    SendMessage( MNWMessageObserver::ENWMessageCurrentHomeZoneMessage );
   
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::WriteViagValues() End" );
    }
    

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::HandleSystemEventL
// Handles system events. 
// ----------------------------------------------------------------------------
//
void CNWNetworkViagBaseEngine::HandleSystemEventL( 
            const TInt aValue,
            const TInt aState )
    {

    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::HandleSystemEventL() -- Begin -- ");
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine ::   State : %d", aState );
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine ::   aValue : %d", aValue );
    
    if ( aValue == KPSSimStatus && aState == ESimUsable )      
        {
        NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine ::1");    
              
        if ( !iSimOk  )
            {
            NWLOGSTRING( KNWOBJECT, 
                "NW: CNWNetworkViagBaseEngine::HandleSystemEventL()\
                 -- SIM OK -- ");
            iSimOk = ETrue;
            CellReselection();
            }
        }
    if ( aValue == KCTsyCallState && iInitReady == EInitReady )
        {
        if ( aState == EPSCTsyCallStateDialling ||
            aState == EPSCTsyCallStateRinging ||
            aState == EPSCTsyCallStateAlerting )
            {
            iActiveCall = ETrue; 
            WriteViagValues( ENWViagIndicatorTypeNone, KNWzero );
            }
   
        else if ( aState == EPSCTsyCallStateNone )
            {
            iActiveCall = EFalse;
            StartTimer();
            }              
        }
        
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::HandleSystemEventL() -- End -- ");
    }


// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::IsViagImsi
// Checks if we got Viag SIM card in ME.
// SubscriberId is checked in the CNWNetworkCommand-class. Viag IMSI has fixed
// prefix which we compare here.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TBool CNWNetworkViagBaseEngine::IsViagImsi( ) const
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::IsViagImsi() Begin" );
    
    TBool ret( EFalse );

    TNWSubscriberId subId( iInterNetworkInfo.iSubscriberId );

    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagBaseEngine::IsViagImsi, subId = %S", &subId );

    if ( subId.Length() >= KNWViagMinImsiLength )
        {
        ret = subId.Left( KNWViagMinImsiLength ).
            Compare( KNWViagImsiIdentity ) == KErrNone;
        //Checks 3g Viag SIM.
        if ( !ret )
            {
            ret = subId.Left( KNWViagMinImsiLength ).
            Compare( KNWViagImsi3gIdentity ) == KErrNone;
            }
        }
    
    NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::IsViagImsi() End, \
            ret = %d", ret );
    
    return ret;
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::StartupReady
// Start Sat Server if it is not started in start up.
// ----------------------------------------------------------------------------
// 
void CNWNetworkViagBaseEngine::StartupReadyL()
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::StartupReadyL() Begin" );
    
    // Sat server is not up in starup 
    if ( !iViagRefresh )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::StartupReady() \
            Creating ViagRefresh  ");
        
        iViagRefresh = CNWNetworkViagRefresh::NewL( 
                *this, iCustomAPI, *iViagSimReader, iReadingOnGoing );
        
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::StartupReady() \
            Creating ViagRefresh  ");
        iOwner.HandleCellReselection();
        }
    
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::StartupReadyL() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::SetZoneTag
// Sets Zone tags to the zonetags Array.
// ----------------------------------------------------------------------------
//   
void CNWNetworkViagBaseEngine::SetZoneTag( 
        TInt aZoneId, const TDesC& aZoneText )
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::SetZoneTag()begin ");
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::SetZoneTag() aZoneId = %d", aZoneId );
    if ( aZoneId < iZoneTags->Count() )
        {
        TNWZoneTags& zoneTag = iZoneTags->At(aZoneId);
        zoneTag.iZoneTag.Copy( aZoneText.Left(KNWZoneTagLength) );
        NWLOGSTRING2( KNWOBJECT,
            "NW: CNWNetworkViagBaseEngine::SetZoneTag() aZoneText = %S",
            &(zoneTag.iZoneTag ));
        }
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::SetZoneTag() end ");
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::CompleteRefresh
// Calls RefreshEFRead Method to complete refresh.
// ----------------------------------------------------------------------------
//   
void CNWNetworkViagBaseEngine::CompleteRefresh()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::CompleteRefresh() Begin "); 
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagBaseEngine::CompleteRefresh(),\
        ReadingOnGoing = %d",iReadingOnGoing ); 
    NWLOGSTRING3( KNWINT, 
        "NW: CNWNetworkViagBaseEngine::CompleteRefresh(), \
        iSPNEFRead = %d, iNPNEFRead = %d", iSPNEFRead, iNPNEFRead );
    NWLOGSTRING3( KNWINT, 
        "NW: CNWNetworkViagBaseEngine::CompleteRefresh(), \
        iProgEFRead = %d, iViagEFRead = %d", iProgEFRead, iViagEFRead );
    
    if ( iSPNEFRead && iNPNEFRead && iProgEFRead && iViagEFRead )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::RefreshEFRead() called "); 
        iViagRefresh->RefreshEFRead();
        if ( iViagActiveAlgo == EWNAlgoLc )
            {
            CellReselection();
            }
        }   
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::CompleteRefresh() End "); 
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::UpdateEFReadingState
// Calls RefreshEFRead Method to complete refresh.
// ----------------------------------------------------------------------------
//     
void CNWNetworkViagBaseEngine::UpdateEFReadingState( 
        const TNWRead& aElementFile, TBool aReadStatus )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::UpdateEFReadingState() Begin ");
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagBaseEngine::UpdateEFReadingState(),\
        aElementFile = %d; SPN = 0, NPN = 1, ProgOPN = 2, ViagHZ = 3",
        ( TInt )aElementFile ); 
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkViagBaseEngine::UpdateEFReadingState(), \
        aReadStatus = %d",aReadStatus ); 
    NWLOGSTRING2( KNWINT,
        "NW: CNWNetworkViagBaseEngine::UpdateEFReadingState(), \
            iReadingOnGoing = %d", iReadingOnGoing ); 
    
    if ( iReadingOnGoing )
        {
        switch ( aElementFile ) 
            {
            case ESPNEFRead:
                {
                iSPNEFRead = aReadStatus;
                break;
                }
            case ENPNEFRead:
                {
                iNPNEFRead = aReadStatus;
                break;
                }
            case EProgEFRead:
                {
                iProgEFRead = aReadStatus;
                break;
                }
            case EViagEFRead:
                {
                iViagEFRead = aReadStatus;
                break;
                }
            case EFullFileChange:
                {
                iSPNEFRead = aReadStatus;
                iNPNEFRead = aReadStatus;
                iProgEFRead = aReadStatus;
                iViagEFRead = aReadStatus;
                break;
                }
            default:
                break;
            }
        if ( aReadStatus )
            {
            CompleteRefresh();
            } 
        }
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::UpdateEFReadingState() End "); 
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::StartTimer
// Starts the timer for waiting Cell change notification.
// ----------------------------------------------------------------------------
//        
void CNWNetworkViagBaseEngine::StartTimer()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::StartTimer() Begin ");
    
    StopTimer();
    if( iTimer )
        {
        iTimer->After( KNWTimeOut, this ); // 2 sec
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::StartTimer() End ");
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::StopTimer
// Stops the timer
// ----------------------------------------------------------------------------
// 
void CNWNetworkViagBaseEngine::StopTimer()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::StopTimer() Begin ");
    
    if( iTimer )
        {
        iTimer->Cancel();
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkViagBaseEngine::StopTimer() End ");
    }

// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::HandleTimeOut
// Calls CellReselection if no notification came.
// ----------------------------------------------------------------------------
//
void CNWNetworkViagBaseEngine::HandleTimeOut()
    {
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::HandleTimeOut() Begin ");    
    CellReselection();  
    NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkViagBaseEngine::HandleTimeOut() End ");
    }    

// -----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::InitializeCharSetConverterL
// Initializes CharacterSetConverter and connects to file server session
// -----------------------------------------------------------------------------
//
void CNWNetworkViagBaseEngine::InitializeCharSetConverterL()
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::InitalizecharSetConverterL() ");
    User::LeaveIfError( iFs.Connect() );
    iCharacterSetConverter = CCnvCharacterSetConverter::NewL();

    CCnvCharacterSetConverter::TAvailability availability = iCharacterSetConverter->PrepareToConvertToOrFromL(
            KCharacterSetIdentifierSms7Bit, iFs );

    if ( availability == CCnvCharacterSetConverter::ENotAvailable )
        {
        delete iCharacterSetConverter;
        iCharacterSetConverter = NULL;
        }
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::InitalizecharSetConverterL() end");
    }

// -----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::DefaultAlphabetDecode
// Decodes 7-bit text representation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CNWNetworkViagBaseEngine::DefaultAlphabetDecode(
  const TDesC8& aTag, TDes& aZoneText )
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::DefaultAlphabetDecode()");
    // Start conversion
    aZoneText.Copy( aTag );
    TBuf16<KNWZoneTagLength> generatedUnicode;
    TInt state( CCnvCharacterSetConverter::KStateDefault ); // has to be ref.
    if ( iCharacterSetConverter )
        {
        TInt returnValue = iCharacterSetConverter->ConvertToUnicode(
                generatedUnicode, aTag, state );

        // returnValue contains the number of characters that were not converted or
        // error value.
        if ( returnValue >= KErrNone )
            {
            aZoneText.Copy( generatedUnicode );
            }
        }
  NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkViagBaseEngine::DefaultAlphabetDecode() end");
  }

//  End of File
