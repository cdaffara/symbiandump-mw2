/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A class for recording and storing locations.
*
*/

#include <e32cmn.h> 
#include <lbserrors.h>
#include <lbssatellite.h>
#include <ecom.h>
#include <centralrepository.h>
#include <hwrmpowerstatesdkpskeys.h>
#include <MProfileEngine.h> //profile enum
#ifdef LOC_GEOTAGGING_CELLID
#include <lbslocationinfo.h>
#endif //LOC_GEOTAGGING_CELLID

#include "rlocationtrail.h"
#include "clocationrecord.h"
#include "cnetworkinfo.h"
#include "locationmanagerdebug.h"
#include "locationtraildefs.h"
#include "locationtrailpskeys.h"
#include "mdeconstants.h"



using namespace MdeConstants;

const TUint KNetQueryBit = 1; // 1 bit
const TUint KDownloadMediaFile = 2; // 2  bit
const TUint KSnapMediaFile = 4; // 3 bit
const TUint KLocationQueryInProgress = 1; // 1  bit
#ifdef LOC_REVERSEGEOCODE
const TUint KReverseGeoCodingInProgress = 2; // 2 bit
_LIT ( KCountry, "country:");   // country:india 
_LIT ( KCity, "city:");         // city:bangalore|country:india
#endif
const TUint KSnapGeoConvertInProgress = 4; // 3 bit
#ifdef LOC_GEOTAGGING_CELLID
const TUint KSnapGeoConvertInPendingState = 8; // 4 bit
#endif



// --------------------------------------------------------------------------
// CLocationRecord::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CLocationRecord* CLocationRecord::NewL(MGeoTaggerObserver& aGeoTaggerObserver,
                RMobilePhone& aPhone)
    {
    CLocationRecord* self = new (ELeave) CLocationRecord(aGeoTaggerObserver, aPhone);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// --------------------------------------------------------------------------
// CLocationRecord::CLocationRecord
// --------------------------------------------------------------------------
//  
CLocationRecord::CLocationRecord(MGeoTaggerObserver& aGeoTaggerObserver,
                RMobilePhone& aPhone)
    : iNetworkInfoTimer( NULL ),
    iState( RLocationTrail::ETrailStopped ),
    iTrailCaptureSetting( RLocationTrail::ECaptureAll ),
    iLocationCounter( 0 ),
    iRequestCurrentLoc( EFalse ),
    iTrailStarted( EFalse ),
    iLastGPSFixState( EFalse ),
	iLocationQuery(NULL),
	iNetLocationQuery(NULL),
    iRemapState( ERemapProgressNone),
	iNamespaceDef(NULL),
	iLocationObjectDef(NULL),
	iLatitudeDef(NULL),
	iLongitudeDef(NULL),
	iAltitudeDef(NULL),
    iMediaHandlingFlag(0),
    iPhone(aPhone),
    iGpsDataAvailableFlag(EFalse),
    iGeoTaggerObserver(aGeoTaggerObserver)
#ifdef LOC_GEOTAGGING_CELLID	
    ,iGeoConverter(NULL)
    , iConvertRetry(ETrue)
#endif
#ifdef LOC_REVERSEGEOCODE
    ,iConnectionOption(ESilent)
	,iImageQuery(NULL)
	,iTagQuery(NULL)
    ,iTagCreator( NULL )
    ,iLastReverseGeocodeFails(EFalse)
	,iRevGeocoderPlugin( NULL )
#endif
    ,iProfileHandler(NULL)
    ,iOfflineCheck(ETrue)
    {
    iMaxTrailSize = KMaxTrailLength / KUpdateInterval;
    }

// --------------------------------------------------------------------------
// CLocationRecord::ConstructL
// --------------------------------------------------------------------------
//    
void CLocationRecord::ConstructL()
    {
    LOG( "CLocationRecord::ConstructL(), begin" );
    const TInt KMillion = 1000000;
    TInt err = iProperty.Define( KPSUidLocationTrail, KLocationTrailState, RProperty::EInt );
    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    User::LeaveIfError( iProperty.Set( KPSUidLocationTrail,
        KLocationTrailState, (TInt) RLocationTrail::ETrailStopped ) ); 

    
    iNetworkInfoChangeListener = CNetworkInfo::NewL( this );
    iPositionInfo = CPositionInfo::NewL( this );
	iRemapper = CLocationRemappingAO::NewL();
    iNetworkInfoTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    iProfileHandler = CProfileChangeNotifyHandler::NewL(this); 	

#ifdef LOC_REVERSEGEOCODE
    iTagCreator = CTagCreator::NewL();

	  TInt pluginerr = KErrNone;
    TRAP(pluginerr,iRevGeocoderPlugin = CReverseGeoCoderPlugin::NewL());
    		
   if(pluginerr == KErrNone)
   	{
		iRevGeocoderPlugin->AddObserverL(*this);
	 	}
 #endif

	
    TInt interval( 0 );
    TRAP(err, ReadCenRepValueL(KIntervalKey, interval));
    LOG1("Cenrep interval value:%d", interval);
    
    if (interval == 0 || err != KErrNone )
    	{
        LOG1("Cenrep interval err:%d", err);
    	iInterval = KUpdateInterval;
    	}
    else 
    	{
    	iInterval = interval * KMillion;
    	}

    TRAP(err, ReadCenRepValueL(KLocationDeltaKey, iLocationDelta));
    LOG1("Location delta value:%d", iLocationDelta);
    
    if (iLocationDelta == 0)
    	{
        LOG1("Location delta err:%d", err);
        iLocationDelta = KLocationDelta;
    	}
    iLastMediaItem.iFlag = 0;
    iLastMediaItem.iLocationId = 0;
#ifdef LOC_REVERSEGEOCODE
    iLastMediaItem.iCityTagId= 0;
    iLastMediaItem.iCountryTagId = 0;
    iLastMediaItem.iReverseGeocodeSuccess = 0;

    iLastLocationItem.iFlag = 0;
    iLastLocationItem.iCityTagId= 0;
    iLastLocationItem.iCountryTagId = 0;
    iLastLocationItem.iLocationId = 0;
    iLastLocationItem.iReverseGeocodeSuccess = 0;
#endif
    LOG( "CLocationRecord::ConstructL(), end" );    
    }
    
// --------------------------------------------------------------------------
// CLocationRecord::~CLocationRecord
// --------------------------------------------------------------------------
//    
EXPORT_C CLocationRecord::~CLocationRecord()
    {
    LOG( "CLocationRecord::~CLocationRecord(), begin" );    
    Stop();
    iProperty.Delete( KPSUidLocationTrail, KLocationTrailState );
    iProperty.Close();
    iTrail.ResetAndDestroy();
    iTrail.Close();
    iMediaItems.ResetAndDestroy();
    iMediaItems.Close();
#ifdef LOC_REVERSEGEOCODE
    iLocationItems.ResetAndDestroy();
    iLocationItems.Close();
#endif    
    delete iNetworkInfoChangeListener;
    iNetworkInfoChangeListener = NULL;
    delete iPositionInfo;
    iPositionInfo = NULL;
    delete iNetworkInfoTimer;
    iNetworkInfoTimer = NULL;
#ifdef LOC_GEOTAGGING_CELLID
    if(iGeoConverter)
        {
        delete iGeoConverter;
        iGeoConverter = NULL;
        }
#endif    
    if (iRemapper)
		{
		iRemapper->StopRemapping();
		delete iRemapper;
		iRemapper = NULL;
		}
    if(iLocationQuery)
		{
		iLocationQuery->RemoveObserver(*this);
		iLocationQuery->Cancel();
		delete iLocationQuery;
		iLocationQuery = NULL;
		}

if(iNetLocationQuery)
    {
    iNetLocationQuery->RemoveObserver(*this);
    iNetLocationQuery->Cancel();
    delete iNetLocationQuery;
    iNetLocationQuery = NULL;
    }


#ifdef LOC_REVERSEGEOCODE
    if(iImageQuery)
		{
		iImageQuery->RemoveObserver(*this);
		iImageQuery->Cancel();
		delete iImageQuery;
        iImageQuery = NULL;
		}
    if(iTagQuery)
		{
	    iTagQuery->RemoveObserver(*this);
		iTagQuery->Cancel();
		delete iTagQuery;
        iTagQuery = NULL;
		}	
    if(iTagCreator)
        {
		delete iTagCreator;
        iTagCreator = NULL;
        }
    
    if(iRevGeocoderPlugin)
        {
	    // set the pointer to NULL, ECOM will destroy object.
        delete iRevGeocoderPlugin;
        iRevGeocoderPlugin = NULL;
  	    }
#endif
    if(iProfileHandler)
    	{
        delete iProfileHandler;
        iProfileHandler = NULL;
    	}
    LOG( "CLocationRecord::~CLocationRecord(), end" );	
    }

// --------------------------------------------------------------------------
// CLocationRecord::CurrentState
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::LocationTrailState( TLocTrailState& aState )
    {
    aState = iState;
    }

// --------------------------------------------------------------------------
// CLocationRecord::StartL
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::StartL( RLocationTrail::TTrailCaptureSetting aCaptureSetting )
    {
    LOG( "CLocationRecord::StartL(), begin" );
    iTrailCaptureSetting = aCaptureSetting;
    iGpsDataAvailableFlag = EFalse;
    if ( aCaptureSetting == RLocationTrail::ECaptureAll && !iPositionInfo->IsActive() )
        {
        if( iState == RLocationTrail::ETrailStopped  )
            {
            iTrail.ResetAndDestroy();
            }
    	// Capture at least one location with n/w info else we wouldn't be able to tag the immediate taken photograph
    	// incase LBS didn't provide any valid position data. Without this there may be a leave from GetLocationByTimeL 
    	// method while calling this from LocationSnapshotL method
    	UpdateNetworkInfo( this );
        iPositionInfo->StartL( aCaptureSetting, iInterval );
        }
    else if ( aCaptureSetting == RLocationTrail::ECaptureNetworkInfo )
    	{
    	// Update and store network info in location trail immediately.
    	// Timer will trigger the update again later.
    	UpdateNetworkInfo( this );
    	
        if ( iNetworkInfoTimer && iNetworkInfoTimer->IsActive() )
        	{
        	iNetworkInfoTimer->Cancel();
        	}
        	
        StartTimerL();
    	}
    
     iTrailStarted = ETrue;
     iState = RLocationTrail::ETrailStopped;
     SetCurrentState( RLocationTrail::ETrailStarting );
    
    LOG( "CLocationRecord::StartL(), end" );
    }

// --------------------------------------------------------------------------
// CLocationRecord::Stop
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::Stop()
    {
    LOG( "CLocationRecord::StopL(), begin" );
    iPositionInfo->Stop();
    iGpsDataAvailableFlag = EFalse;
    iTrailStarted = EFalse;
    if ( iNetworkInfoTimer && iNetworkInfoTimer->IsActive() )
    	{
    	iNetworkInfoTimer->Cancel();
    	}

    if ( iRemapper )
    	{
    	iRemapper->ResetQueue();
    	}
    SetCurrentState( RLocationTrail::ETrailStopped );
    LOG( "CLocationRecord::StopL(), end" );
    }

// --------------------------------------------------------------------------
// CLocationRecord::SetStateToStop
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::SetStateToStopping()
	{
	SetCurrentState( RLocationTrail::ETrailStopping );
	}

// --------------------------------------------------------------------------
// CLocationRecord::GetLocationByTimeL
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::GetLocationByTimeL( const TTime aTime, 
												   TLocationData& aLocationData,
                                                   TLocTrailState& aState ) 
    {
    LOG( "CLocationRecord::GetLocationByTimeL(), begin" );
    TInt posFound( EFalse );

#ifdef _DEBUG
    _LIT( KDateTimeFormat, "%Y/%M/%D %H:%T:%S" );
    const TInt DateTimeStrMaxLength = 20;
    LOG1( "CLocationRecord::GetLocationByTimeL - aTime: %Ld", aTime.Int64() );
    TBuf<DateTimeStrMaxLength> str1;
    aTime.FormatL( str1, KDateTimeFormat );
    LOG1( "CLocationRecord::GetLocationByTimeL - aTime: %S", &str1 );
#endif

    TTimeIntervalSeconds interval;
    TTimeIntervalSeconds nextInterval;
    for ( TInt i(iTrail.Count()-1) ; i >= 0 && !posFound ; i-- )
        {
        TInt err = iTrail[i]->iTimeStamp.SecondsFrom( aTime, interval );
        
        TInt timeDiff = Abs( interval.Int() );

#ifdef _DEBUG
        LOG1( "CLocationRecord::GetLocationByTimeL - Trail timestamp: %Ld", iTrail[i]->iTimeStamp.Int64() );
        TBuf<DateTimeStrMaxLength> str;
        iTrail[i]->iTimeStamp.FormatL( str, KDateTimeFormat );
        LOG1( "CLocationRecord::GetLocationByTimeL - Trail timestamp: %S", &str );
        LOG1( "CLocationRecord::GetLocationByTimeL - timeDiff: %d", timeDiff );
#endif

        if ( err == KErrNone && timeDiff <= KMaximumIntervalSeconds )
            {
            // The nearest time is in iTrail[i] or in iTrail[i-1].
            if ( i > 0 )
                {
                iTrail[i-1]->iTimeStamp.SecondsFrom( aTime, nextInterval );
                
                TInt nextDiff = Abs( nextInterval.Int() );
                    
                if ( nextDiff < timeDiff )
                    {
                    aLocationData = iTrail[i-1]->iLocationData;
                    aState = iTrail[i-1]->iTrailState;
                    }
                else
                    {
                    aLocationData = iTrail[i]->iLocationData;
                    aState = iTrail[i]->iTrailState;
                    }                    
                }            
            else
                {
                aLocationData = iTrail[i]->iLocationData;
                aState = iTrail[i]->iTrailState;
                }
            posFound = ETrue;
            }
        }
    if ( !posFound )
        {
        User::Leave( KErrNotFound );
        }
    LOG( "CLocationRecord::GetLocationByTimeL(), end" );
    }
    
// --------------------------------------------------------------------------
// CLocationRecord::RequestLocationL
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::RequestLocationL()
    {
    LOG( "CLocationRecord::RequestLocationL(), begin" );    
    iRequestCurrentLoc = ETrue;
    if ( iTrailCaptureSetting != RLocationTrail::ECaptureNetworkInfo &&
    	!iPositionInfo->IsActive() )
        {
        iPositionInfo->StartL( iTrailCaptureSetting, iInterval );
        SetCurrentState( RLocationTrail::ETrailStarting );
        
        iTrailStarted = ETrue;
        }
    else if ( iTrailCaptureSetting == RLocationTrail::ECaptureNetworkInfo )
    	{
    	TPositionSatelliteInfo posInfo;
    	CTelephony::TNetworkInfoV1 network = CTelephony::TNetworkInfoV1();
    	GetNetworkInfo( network );
       	iObserver->CurrentLocation( posInfo, network, KErrNone );
        iRequestCurrentLoc = EFalse;
    	}
    LOG( "CLocationRecord::RequestLocationL(), end" );	
    }

// --------------------------------------------------------------------------
// CLocationRecord::CancelLocationRequest
// --------------------------------------------------------------------------
//    
EXPORT_C void CLocationRecord::CancelLocationRequest()
    {
    LOG( "CLocationRecord::CancelLocationRequest(), begin" );    
    iRequestCurrentLoc = EFalse;
    if ( !iTrailStarted )
        {
        iGpsDataAvailableFlag = EFalse;
        iPositionInfo->Stop();
        }
    LOG( "CLocationRecord::CancelLocationRequest(), end" );	
    }
        
        
// --------------------------------------------------------------------------
// CLocationRecord::GetNetworkInfo
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::GetNetworkInfo( CTelephony::TNetworkInfoV1& aNetworkInfo ) 
    {
    LOG("CLocationRecord::GetNetworkInfo");

    aNetworkInfo = iNetwork;
    }
    
// --------------------------------------------------------------------------
// CLocationRecord::SetObserver
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::SetObserver( MLocationTrailObserver* aObserver) 
    {
    iObserver = aObserver;
    }

// --------------------------------------------------------------------------
// CLocationRecord::SetAddObserver
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::SetAddObserver( MLocationAddObserver* aObserver)
    {
    iAddObserver = aObserver;
    }

// --------------------------------------------------------------------------
// From MNetworkInfoObserver.
// CLocationRecord::Position
// --------------------------------------------------------------------------
//    
void CLocationRecord::Position( const TPositionInfo& aPositionInfo,
                                const TInt aError  )
    {    
    LOG( "CLocationRecord::Position(), begin" );    
    const TPositionSatelliteInfo& positionSatelliteInfo = 
    	static_cast<const TPositionSatelliteInfo&>(aPositionInfo);

    if ( iRequestCurrentLoc )
        {
        HandleLocationRequest( positionSatelliteInfo, aError );
        }
    
    iGpsDataAvailableFlag = EFalse;
    
    if ( !iTrailStarted || iState == RLocationTrail::ETrailStopped)
        {
    	LOG("CLocationRecord::Position(), trail not started/stopped");
        iPositionInfo->Stop();
        return;
        }
    // all cases store the location..
    StoreLocation( positionSatelliteInfo );
    
    if ( RemappingNeeded() )
        {
        //either network or gps signal is available
        if ( ( iNetwork.iCellId != 0 && 
                iNetwork.iCountryCode.Length() != 0 &&
                iNetwork.iNetworkId.Length() != 0 ) || ( aError == KErrNone ) )
            {
            //no error means gps info available
            TRAP_IGNORE( RemapObjectsL( aError == KErrNone ) );
            }
        }
    switch ( aError )
        {
        case KPositionPartialUpdate: // fall through
        case KPositionQualityLoss: 
            {
            // Location is stored, even if it may not be valid.
            LOG("CLocationRecord::Position(), Partial update");
            if ( iState != RLocationTrail::EWaitingGPSData && 
            	 iState != RLocationTrail::ETrailStopping ) 
                {
                SetCurrentState( RLocationTrail::EWaitingGPSData );
            	LOG("CLocationRecord::Position(), Trail waiting for gps");
                }
            break;
            }
        case KErrNone:
            {
            LOG("CLocationRecord::Position(), Good GPS coordinates");
            iGpsDataAvailableFlag = ETrue;
            if ( iState != RLocationTrail::ETrailStarted ) 
                {
                if ( iRemapper )
                	{
                	LOG("CLocationRecord::Position(), Start remapping");
               		iRemapper->StartRemappingObjects( iNewItem.iLocationData );

                    if( iObserver->WaitForPositioningStopTimeout() && !RemappingNeeded() )                                
               		    {                                                
                        iObserver->RemapedCompleted();
                        iPositionInfo->HandleRemapComplete();
                        return;
               		    }
               		
                	}
                if ( iState != RLocationTrail::ETrailStopping )
                	{
                    SetCurrentState( RLocationTrail::ETrailStarted );
                	LOG("Trail started");
                	}
                }
            break;
            }
        default:
            {
            LOG1("CLocationRecord::Position(), Searching GPS, aError %d", aError );
            if ( iState != RLocationTrail::ESearchingGPS &&
               	 iState != RLocationTrail::ETrailStopping ) 
                {
                SetCurrentState( RLocationTrail::ESearchingGPS );
            	LOG("Trail searching gps");
                }
            break;
            }      
        }
    TBool fixState = CheckGPSFix( positionSatelliteInfo );
    LOG1( "CLocationRecord::Position(), fixState %d", fixState );
    LOG1( "CLocationRecord::Position(), iLastGPSFixState %d", iLastGPSFixState );
    
    if ( iObserver && iLastGPSFixState != fixState )
    	{
    	LOG("CLocationRecord::Position(), Quality changed");
    	iObserver->GPSSignalQualityChanged( positionSatelliteInfo );
    	}
    
   	iLastGPSFixState = fixState;
    
    LOG( "CLocationRecord::Position(), end" );	
    }


// --------------------------------------------------------------------------
// CLocationRecord::RemapObjectsL
// Remaps the location objects when GPS is available
// --------------------------------------------------------------------------
//
void CLocationRecord::RemapObjectsL( TBool aGPSInfoAvailable )
    {
    LOG( "CLocationRecord::RemapObjectsL(), begin" );    
    TBool createLocation = EFalse;

    if (iRemapper)
        {
        createLocation = iRemapper->CheckQueue();
        }

    if ( !createLocation || iRemapper == NULL)
        {
        return;
        }

    
    if ( aGPSInfoAvailable )
        {       
        TItemId locationId = DoCreateLocationL( iNewItem.iLocationData );
        iRemapper->UpdateRelationsL( locationId ); 
#ifdef LOC_REVERSEGEOCODE
        if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress))
            {
            iRemapState = ERemapRevGeoCodeInProgress;
            //Find the address by coordinate, results a call to ReverseGeocodeComplete()
            iMediaHandlingFlag |= KReverseGeoCodingInProgress;

            if(iRevGeocoderPlugin)
            	{
            	iRevGeocoderPlugin->GetAddressByCoordinateL( iNewItem.iLocationData.iPosition, iConnectionOption ); 
            	}
            }
        else
            {
            // make this as pending state so that on rev geo code complete
            // it will take the priority over the next item within the location 
            // item in location queue.
			iRemapState = ERemapRevGeoCodePending;
            }
#endif //LOC_REVERSEGEOCODE
        }
    else
        {
        //call location m/w API to convert cell ID to lat, long
#ifdef LOC_GEOTAGGING_CELLID
        if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0)
            {
            iRemapState = ERemapNwGeoConverterPending;
            }
        else
            {
            // go for cell id based geo coding.
            iRemapState = ERemapNwGeoConverterInProgress;
            if(iGeoConverter == NULL)
               {
               iGeoConverter = CGeoConverter::NewL(*this);
               }
            iGeoConverter->ConvertL(iNewItem.iLocationData.iNetworkInfo);
            iMediaHandlingFlag |= KSnapGeoConvertInProgress;
            }
#endif
        }    
#ifdef LOC_REVERSEGEOCODE
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0 || 
        (iMediaHandlingFlag & KReverseGeoCodingInProgress) > 0)
#else
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0 )
#endif //LOC_REVERSEGEOCODE
        {
        // stop n/w info change listener, since device may connect to n/w 
        // and local trail will receive so many call backs on current n/w info change.
        iNetworkInfoChangeListener->StopNwInfoChangeNotifier();
        }
    else
        {
        iNetworkInfoChangeListener->StartNwInfoChangeNotifier();
        }
    LOG( "CLocationRecord::RemapObjectsL(), end" );

    }

TBool CLocationRecord::CheckGPSFix( const TPositionSatelliteInfo& aSatelliteInfo )
	{
	LOG("CLocationRecord::CheckGPSFix, begin");
	TPosition position;
	aSatelliteInfo.GetPosition( position );
	LOG1( "latitude %f", position.Latitude() );
	LOG1( "longitude %f", position.Longitude() );
	TBool ret = ( Math::IsNaN(position.Latitude()) || Math::IsNaN(position.Longitude()) ) 
		? EFalse : ETrue;
    LOG1("CLocationRecord::CheckGPSFix, end. Ret - %d", ret);
   	return ret;
	}
    
// --------------------------------------------------------------------------
// From MPositionerObserver.
// CLocationRecord::NetworkInfo
// --------------------------------------------------------------------------
//    
void CLocationRecord::NetworkInfo( const CTelephony::TNetworkInfoV1 &aNetworkInfo, 
		const TInt aError )
    {
    LOG("CLocationRecord::NetworkInfo, begin");
    if ( aError == KErrNone && iOfflineCheck)
        {
        LOG("CLocationRecord::NetworkInfo - KErrNone");
        iNetwork = aNetworkInfo;
        if (iNetwork.iAccess == CTelephony::ENetworkAccessUtran)
        	{
        	iNetwork.iLocationAreaCode = 0;
        	}
#ifdef LOC_GEOTAGGING_CELLID
        if ( iState == RLocationTrail::ETrailStarting && iTrailStarted )
        	{
        	SetCurrentState( RLocationTrail::ETrailStarted );
        	}
#endif        
        }
    else
        {
        LOG1("CLocationRecord::NetworkInfo - %d", aError );
        iNetwork = CTelephony::TNetworkInfoV1();
        iNetwork.iAreaKnown = EFalse;
        iNetwork.iAccess = CTelephony::ENetworkAccessUnknown;
        iNetwork.iCellId = 0;
        iNetwork.iLocationAreaCode = 0;
        iNetwork.iCountryCode.Zero();
        iNetwork.iNetworkId.Zero();
        }
    LOG( "CLocationRecord::NetworkInfo(), end" );	
    }

// --------------------------------------------------------------------------
// CLocationRecord::GetMobilePhone4NwInfo
// --------------------------------------------------------------------------
//    
RMobilePhone& CLocationRecord::GetMobilePhone4NwInfo()
    {
    LOG( "CLocationRecord::GetMobilePhone4NwInfo()" );    
    return iPhone;
    }

// --------------------------------------------------------------------------
// CLocationRecord::StoreLocationL
// --------------------------------------------------------------------------
//    
void CLocationRecord::StoreLocation( const TPositionSatelliteInfo& aSatelliteInfo )
    {
    LOG( "CLocationRecord::StoreLocation(), begin" );    
    aSatelliteInfo.GetPosition( iNewItem.iLocationData.iPosition );
    aSatelliteInfo.GetCourse( iNewItem.iLocationData.iCourse );
    iNewItem.iLocationData.iSatellites = aSatelliteInfo.NumSatellitesUsed();
    iNewItem.iLocationData.iQuality = aSatelliteInfo.HorizontalDoP();
    
    // Network info
    GetNetworkInfo( iNewItem.iLocationData.iNetworkInfo );
    // Get Universal time
    iNewItem.iTimeStamp.UniversalTime();
    iNewItem.iTrailState = iState;
    
    TLocationTrailItem *item = new TLocationTrailItem();
    TInt error = KErrNone;
    if(item != NULL)
        {
        *item = iNewItem;
        error = iTrail.Append( item );
        }
    
    // If appending an item to the trail fails because of OOM, remove oldest trail items
    // until the new item fits or there's only one item left in the trail.
    while ( error == KErrNoMemory && iTrail.Count() > 1 )
		{
		LOG("CLocationRecord::StoreLocation - Out of memory! Shortening trail!");
		iTrail.Remove( 0 );
		error = iTrail.Append( item ); 
		}
    
    if ( iTrail.Count() > iMaxTrailSize )
        {
        item = iTrail[0];
        iTrail.Remove( 0 );
        iTrail.Compress();
        delete item;
        }
    
    if( iAddObserver )
    	{
    	iAddObserver->LocationAdded( iNewItem, aSatelliteInfo );
    	}
    LOG( "CLocationRecord::StoreLocation(), end" );	
    }
    
// --------------------------------------------------------------------------
// CLocationRecord::SetCurrentState
// --------------------------------------------------------------------------
//        
void CLocationRecord::SetCurrentState( TLocTrailState aState )    
    {
    LOG1( "CLocationRecord::SetCurrentState(), begin, state:%d", aState );
    iState = aState;
    if( iTrailStarted )
        {
        // Set the property only when trail is started to avoid icon flickering and wrong icon update in UI
        if( iGpsDataAvailableFlag  
#ifdef LOC_GEOTAGGING_CELLID
            || ( iNetwork.iCellId > 0 && 
            ((iNetwork.iLocationAreaCode == 0 && iNetwork.iAccess == CTelephony::ENetworkAccessUtran) || // 3G
            (iNetwork.iLocationAreaCode > 0 && (iNetwork.iAccess == CTelephony::ENetworkAccessGsm ||  // 2G
            						iNetwork.iAccess == CTelephony::ENetworkAccessGsmCompact)))  &&
            iNetwork.iCountryCode.Length() > 0 &&
            iNetwork.iNetworkId.Length() > 0 )
#endif // LOC_GEOTAGGING_CELLID            
            )
            {
            // set the value 3 to have Geo tag available icon else not available.
            iProperty.Set( KPSUidLocationTrail, KLocationTrailState, 
                (TInt) RLocationTrail::ETrailStarted );
            }
        else
            {
            iProperty.Set( KPSUidLocationTrail, KLocationTrailState, 
                (TInt) RLocationTrail::ETrailStopped );
            }
        }
    else
    	{
    		iProperty.Set( KPSUidLocationTrail, KLocationTrailState, 
             (TInt) RLocationTrail::ETrailStopped );
    	}
    		
    if ( iObserver )
        {
        iObserver->LocationTrailStateChange();
        }
    LOG( "CLocationRecord::SetCurrentState(), end" );
    }
    
// --------------------------------------------------------------------------
// CLocationRecord::HandleLocationRequest
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleLocationRequest( const TPositionSatelliteInfo& aSatelliteInfo, 
                                             const TInt aError )    
    {
    LOG( "CLocationRecord::HandleLocationRequest(), begin" );    
	CTelephony::TNetworkInfoV1 network = CTelephony::TNetworkInfoV1();
    if ( aError == KErrNone )
        {
       	GetNetworkInfo( network );
        iObserver->CurrentLocation( aSatelliteInfo, network, aError );
        iRequestCurrentLoc = EFalse;
        if ( !iTrailStarted )
            {
            iGpsDataAvailableFlag = EFalse;
            iPositionInfo->Stop();
            }
        }
    else
        {
        iLocationCounter++;
        if ( iLocationCounter > KCurrentLocTimeoutCount )
            {
            iObserver->CurrentLocation( aSatelliteInfo, network, KErrTimedOut );
            iRequestCurrentLoc = EFalse;
            iLocationCounter = 0;
            if ( !iTrailStarted )
                {
                iGpsDataAvailableFlag = EFalse;
                iPositionInfo->Stop();
                }
            }       
        }  
    LOG( "CLocationRecord::HandleLocationRequest(), end" );	
    }


// --------------------------------------------------------------------------
// CLocationRecord::UpdateNetworkInfo
// --------------------------------------------------------------------------
//
TInt CLocationRecord::UpdateNetworkInfo( TAny* aAny )
	{
	TPositionSatelliteInfo nullPositionInfo;
	CLocationRecord* self = STATIC_CAST( CLocationRecord*, aAny );
	self->StoreLocation( nullPositionInfo );
	return KErrNone;
	}


EXPORT_C void CLocationRecord::CreateLocationObjectL( const TLocationData& aLocationData,
		const TUint& aObjectId )
	{
    LOG( "CLocationRecord::CreateLocationObjectL(), begin" );	
	TLocationSnapshotItem* newItem = new (ELeave) TLocationSnapshotItem;
	newItem->iObjectId = aObjectId;
	newItem->iLocationData = aLocationData;
	
	// for downloaded files, network informations are not valid.
	// Do location handling only based on lat/lon
	// all all n/w information to invalid value.
    newItem->iLocationData.iNetworkInfo.iCellId = 0;
    newItem->iLocationData.iNetworkInfo.iAccess = CTelephony::ENetworkAccessUnknown;
    newItem->iLocationData.iNetworkInfo.iLocationAreaCode = 0;
    newItem->iLocationData.iNetworkInfo.iCountryCode.Zero();
    newItem->iLocationData.iNetworkInfo.iNetworkId.Zero();
	newItem->iFlag = KDownloadMediaFile;
	
	iMediaItems.Append( newItem );
    FindLocationFromDBL();
    GeoTaggingCompleted();
    LOG( "CLocationRecord::CreateLocationObjectL(), end" );	

	}

// --------------------------------------------------------------------------
// CLocationRecord::LocationSnapshotL
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::LocationSnapshotL( const TUint& aObjectId )
	{
    LOG( "CLocationRecord::LocationSnapshotL(), begin" );	
	TBool previousMatch = EFalse;

	// get locationdata from trail with object time
	TTime timestamp = GetMdeObjectTimeL( aObjectId );
	TLocationData locationData;
	TLocTrailState state;
	TRAPD(err, GetLocationByTimeL( timestamp, locationData, state ));
	if(err != KErrNone && iTrailStarted)
        {
        // Execution shouldn't come over here.
        // Handling error case in worst sceenario..
        StartL(iTrailCaptureSetting);
        // this case may apprear, when cache data is not within the delta limit because someone call stop trail.
        // sceenario, take photograph, then stop taking for sometime, again take photograph.
        // during this time, if there is no change in position (because someone stop trail) and n/w, then cache has old value.
        // go with the n/w based.
        LOG( "Old trail cache. go for n/w based." );
        UpdateNetworkInfo( this );
        // again read the location.
        GetLocationByTimeL( timestamp, locationData, state );
        }
	TLocationSnapshotItem* newItem = new (ELeave) TLocationSnapshotItem;
	newItem->iObjectId = aObjectId;
	newItem->iLocationData = locationData;
	newItem->iFlag = KSnapMediaFile;
	                
	iMediaItems.Append( newItem );

	TItemId lastLocationId = 0;

#ifdef LOC_REVERSEGEOCODE
	if ( ( iLastMediaItem.iReverseGeocodeSuccess == 1  ) && (iLastMediaItem.iFlag & KSnapMediaFile) > 0  )
	    {
        lastLocationId = iLastMediaItem.iLocationId;
	    }
#else
    if ( (iLastMediaItem.iFlag & KSnapMediaFile) > 0  )
        {
        lastLocationId = iLastMediaItem.iLocationId;
        }	
#endif

	CTelephony::TNetworkInfoV1* net = &locationData.iNetworkInfo;
	// capture only network data
	if ( iTrailCaptureSetting == RLocationTrail::ECaptureNetworkInfo )
		{
        if ( net->iCellId == 0 && 
             net->iLocationAreaCode == 0 &&
             net->iCountryCode.Length() == 0 &&
             net->iNetworkId.Length() == 0 )
            {
            // no n/w info... put it into remap.
            // remove the last appended element.
            LOG("CLocationRecord::LocationSnapshotL(), No network info (offline mode + no GPS fix), keep for remapping");
			TRemapItem remapItem;
			remapItem.iObjectId = aObjectId;
			remapItem.iTime = timestamp;
			iRemapper->Append( remapItem );
		
			TLocationSnapshotItem* firstPtr = iMediaItems[iMediaItems.Count() - 1];
			iMediaItems.Remove(iMediaItems.Count() - 1);
			iMediaItems.Compress();
            if( firstPtr == newItem)
                {                
                newItem = NULL;
                }           
            delete firstPtr;            
            firstPtr = NULL;
            }   
        else
            {
            // n/w info available
            NetworkInfoSnapshotL();
            }
		}
	else if ( Math::IsNaN( locationData.iPosition.Latitude() ) && 
			Math::IsNaN( locationData.iPosition.Longitude() ))
		{
        LOG("CLocationRecord::LocationSnapshotL(), coordinates empty with or without cellular info")
	    // coordinates empty, with or without cellular info

		if ( net->iCellId == 0 && 
				net->iLocationAreaCode == 0 &&
				net->iCountryCode.Length() == 0 &&
				net->iNetworkId.Length() == 0 )
			{
            LOG("CLocationRecord::LocationSnapshotL(), No network info (offline mode + no GPS fix), keep for remapping");
			TRemapItem remapItem;
			remapItem.iObjectId = aObjectId;
			remapItem.iTime = timestamp;
			iRemapper->Append( remapItem );
		
			TLocationSnapshotItem* firstPtr = iMediaItems[iMediaItems.Count() - 1];
			iMediaItems.Remove(iMediaItems.Count() - 1);
			iMediaItems.Compress();
            if( firstPtr == newItem)
                {                
                newItem = NULL;
                }           
            delete firstPtr;            
            firstPtr = NULL;
			}
		// check match for last created locationobject
#ifdef LOC_REVERSEGEOCODE
     else if ( (iLastMediaItem.iFlag & KSnapMediaFile) > 0 &&
            iLastMediaItem.iCountryTagId > 0 && ( iLastMediaItem.iReverseGeocodeSuccess == 1 ) )
#else
        else if ( (iLastMediaItem.iFlag & KSnapMediaFile) > 0)
#endif //LOC_REVERSEGEOCODE
			{
            LOG("CLocationRecord::LocationSnapshotL(), last created locationobject match" );
			TLocationData lastLocationData = iLastMediaItem.iLocationData;
			CTelephony::TNetworkInfoV1* lastnet = &lastLocationData.iNetworkInfo;

			// networkinfo changed from last location
			if ( lastnet->iCellId == net->iCellId &&
					lastnet->iLocationAreaCode == net->iLocationAreaCode &&
					lastnet->iCountryCode == net->iCountryCode &&
					lastnet->iNetworkId == net->iNetworkId )
				{
				// same network.
				previousMatch = ETrue;
				CreateRelationL( aObjectId, lastLocationId );
				// attach same tags associated to last location
#ifdef LOC_REVERSEGEOCODE
				if ( iLastMediaItem.iCountryTagId && ( iLastMediaItem.iReverseGeocodeSuccess == 1 ) )   //found from DB last time
				    {
				    iTagCreator->AttachTagsL( aObjectId, 
                                iLastMediaItem.iCountryTagId, iLastMediaItem.iCityTagId );
				    }
#endif //LOC_REVERSEGEOCODE
                // remove the current item.
				TLocationSnapshotItem* firstPtr = iMediaItems[iMediaItems.Count() - 1];
				iMediaItems.Remove(iMediaItems.Count() - 1);
				iMediaItems.Compress();
	            if( firstPtr == newItem)
	                {                
	                newItem = NULL;
	                }           
	            delete firstPtr;            
	            firstPtr = NULL;
				}
			}
		
        if (  newItem && !previousMatch )
		    {
			// go for n/w based
            newItem->iFlag |= KNetQueryBit;
		    }
		}
       
	// valid coordinates found
	else if ( lastLocationId != 0 && 
        ((iLastMediaItem.iFlag & KSnapMediaFile) > 0))
		{
        LOG("CLocationRecord::LocationSnapshotL(), valid coordinates found");
        TLocationData lastLocationData = iLastMediaItem.iLocationData;
		CTelephony::TNetworkInfoV1* lastnet = &lastLocationData.iNetworkInfo;
		
		// first check if networkinfo matches last created location
		if ( lastnet->iCellId == net->iCellId &&
				lastnet->iLocationAreaCode == net->iLocationAreaCode &&
				lastnet->iCountryCode == net->iCountryCode &&
				lastnet->iNetworkId == net->iNetworkId )
			{
			
			// if both locations have valid coordinates, calculate distance between points
			if (
#ifdef LOC_REVERSEGEOCODE
                !iLastReverseGeocodeFails &&
#endif //LOC_REVERSEGEOCODE
                !Math::IsNaN( lastLocationData.iPosition.Latitude() ) && 
					!Math::IsNaN( lastLocationData.iPosition.Longitude() ) && 
					!Math::IsNaN( locationData.iPosition.Latitude() ) && 
					!Math::IsNaN( locationData.iPosition.Longitude() ))
				{
				TReal32 distance;
				TInt err = locationData.iPosition.Distance(lastLocationData.iPosition, distance);
				
				if ( distance < iLocationDelta )
					{
					LOG("CLocationRecord::LocationSnapshotL(), location close to the previous one");
					previousMatch = ETrue;
					CreateRelationL( aObjectId, lastLocationId );
					
#ifdef LOC_REVERSEGEOCODE
					// attach same tags associated to last location
				if ( iLastMediaItem.iCountryTagId && ( iLastMediaItem.iReverseGeocodeSuccess == 1 ) )						
					    {
					    iTagCreator->AttachTagsL( 
					          aObjectId, iLastMediaItem.iCountryTagId, iLastMediaItem.iCityTagId );
                        TLocationSnapshotItem* firstPtr = iMediaItems[iMediaItems.Count() - 1];
                        iMediaItems.Remove(iMediaItems.Count() - 1);
                        iMediaItems.Compress();
                        if( firstPtr == newItem)
                            {                
                            newItem = NULL;
                            }           
                        delete firstPtr;            
                        firstPtr = NULL;
					    }
					//check newItem for validity
					else if (newItem)
					    {
					    // country tag not found.. go for reverse geocoding..
					    newItem->iLocationId = lastLocationId;
					    iLocationItems.Append( newItem );
						iMediaItems.Remove(iMediaItems.Count() - 1);
						iMediaItems.Compress();
					                           
                        if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress))
                           {
                           iMediaHandlingFlag |= KReverseGeoCodingInProgress;
                           if(iRevGeocoderPlugin)
						       {
                               iRevGeocoderPlugin->GetAddressByCoordinateL
							        ( iLocationItems[0]->iLocationData.iPosition,
                                      iConnectionOption);
							   }
                           }
					    }
#else
                    // remove from the queue
                    TLocationSnapshotItem* firstPtr = iMediaItems[iMediaItems.Count() - 1];
                    iMediaItems.Remove(iMediaItems.Count() - 1);
                    iMediaItems.Compress();
                    if( firstPtr == newItem)
                        {                
                        newItem = NULL;
                        }           
                    delete firstPtr;            
                    firstPtr = NULL;

#endif //LOC_REVERSEGEOCODE
					}
				}
			}
		}
	
	// last location did not match, find existing one from DB
	if( !previousMatch )
		{
        FindLocationFromDBL();
		}
    LOG( "CLocationRecord::LocationSnapshotL(), end" );	
	}


// --------------------------------------------------------------------------
// CLocationRecord::NetworkInfoSnapshotL
// --------------------------------------------------------------------------
//
void CLocationRecord::NetworkInfoSnapshotL()
    {
    LOG( "CLocationRecord::NetworkInfoSnapshotL(), begin" );    
    // n/w info available.. always act on last element within the queue
    TInt lastItemIndex = iMediaItems.Count() - 1;
    if ( lastItemIndex >= 0 &&
        (iMediaItems[lastItemIndex]->iFlag & KSnapMediaFile) > 0 )
        {
        CTelephony::TNetworkInfoV1* net = &iMediaItems[lastItemIndex]->iLocationData.iNetworkInfo;
        //only for snap item.
        if ( (iLastMediaItem.iFlag & KSnapMediaFile) > 0 )
            {
             CTelephony::TNetworkInfoV1* lastnet = &iLastMediaItem.iLocationData.iNetworkInfo;
            
            // compare to previous network info
            TItemId locationId = iLastMediaItem.iLocationId;
            if ( lastnet->iCellId != net->iCellId ||
                    lastnet->iLocationAreaCode != net->iLocationAreaCode ||
                    lastnet->iCountryCode != net->iCountryCode ||
                    lastnet->iNetworkId != net->iNetworkId )
                {
                // last one is not matching.
                // let's check the database of any existing etry.
                iMediaItems[lastItemIndex]->iFlag |= KNetQueryBit;
                }
            else
                {
                // matching with the last entry. Just create a relation.
                CreateRelationL( iMediaItems[lastItemIndex]->iObjectId, locationId );
                TLocationSnapshotItem* firstPtr = iMediaItems[lastItemIndex];
                iMediaItems.Remove(lastItemIndex);
                delete firstPtr;
                }
            }
        else 
            {
            // let's check the database of any existing etry.
            iMediaItems[lastItemIndex]->iFlag |= KNetQueryBit;
            }
        }    
    LOG( "CLocationRecord::NetworkInfoSnapshotL(), end" );   
    }


// --------------------------------------------------------------------------
// CLocationRecord::FindLocationFromDBL
// --------------------------------------------------------------------------
//
void CLocationRecord::FindLocationFromDBL()
    {
    LOG( "CLocationRecord::FindLocationFromDBL(), begin" );    
    if(((iMediaHandlingFlag & KLocationQueryInProgress)> 0)
	     || (iMediaItems.Count() <= 0))
        {
		// query is in progress or queue is empty
		LOG1( "CLocationRecord::FindLocationFromDBL(), Count - %d", iMediaItems.Count() );
        return;
        }
     if ( (iMediaItems[0]->iFlag & KNetQueryBit) > 0 )
         {
		 // n/w based.
         FindLocationWithSameNetInfoL();
         return;
         }
        
     const TReal64 KMeterInDegrees = 0.000009;
     const TReal64 KPi = 3.14159265358979;
     const TReal32 K180Degrees = 180.0;
            
     TReal64 latitude = iMediaItems[0]->iLocationData.iPosition.Latitude();
     TReal64 longitude = iMediaItems[0]->iLocationData.iPosition.Longitude();
     // calculate distance in degrees
     TReal64 cosine;
     Math::Cos(cosine, latitude * KPi / K180Degrees );
     TReal64 latDelta = iLocationDelta * KMeterInDegrees;
     TReal64 lonDelta = latDelta * cosine;
     
     //get network related defs
     CMdEPropertyDef& cellIdDef = iLocationObjectDef->GetPropertyDefL(
                    Location::KCellIdProperty );
     CMdEPropertyDef& locationCodeDef = iLocationObjectDef->GetPropertyDefL( 
                    Location::KLocationAreaCodeProperty );
     CMdEPropertyDef& countryCodeDef = iLocationObjectDef->GetPropertyDefL( 
                    Location::KCountryCodeProperty );
     CMdEPropertyDef& networkCodeDef = iLocationObjectDef->GetPropertyDefL( 
                    Location::KNetworkCodeProperty );
     if(iLocationQuery)
         {
         iLocationQuery->RemoveObserver(*this);
         iLocationQuery->Cancel();
         delete iLocationQuery;
         iLocationQuery = NULL;
         }
     iLocationQuery = iMdeSession->NewObjectQueryL( *iNamespaceDef, *iLocationObjectDef, this );
                 
     CMdELogicCondition& cond = iLocationQuery->Conditions();
     cond.SetOperator( ELogicConditionOperatorAnd );
            
     LOG1( "CLocationRecord::FindLocationFromDBL(), latitude: %f", latitude);
     LOG1( "CLocationRecord::FindLocationFromDBL(), latdelta: %f", latDelta);
     LOG1( "CLocationRecord::FindLocationFromDBL(), longitude: %f", longitude);
     LOG1( "CLocationRecord::FindLocationFromDBL(), londelta: %f", lonDelta);
            
     cond.AddPropertyConditionL( *iLatitudeDef, 
                    TMdERealBetween( latitude - latDelta, latitude + latDelta ));
     cond.AddPropertyConditionL( *iLongitudeDef, 
                    TMdERealBetween( longitude - lonDelta, longitude + lonDelta ));
     
     if ( iMediaItems[0]->iLocationData.iNetworkInfo.iCellId > 0 )
         {
         cond.AddPropertyConditionL( cellIdDef, 
                    TMdEUintEqual( iMediaItems[0]->iLocationData.iNetworkInfo.iCellId) );
         }
     if ( iMediaItems[0]->iLocationData.iNetworkInfo.iLocationAreaCode > 0 )
         {
         cond.AddPropertyConditionL( locationCodeDef, 
                    TMdEUintEqual( iMediaItems[0]->iLocationData.iNetworkInfo.iLocationAreaCode) );
         }
     if ( iMediaItems[0]->iLocationData.iNetworkInfo.iCountryCode.Length() > 0 )
         {
         cond.AddPropertyConditionL( countryCodeDef, ETextPropertyConditionCompareEquals,
                 iMediaItems[0]->iLocationData.iNetworkInfo.iCountryCode );
         }
     if ( iMediaItems[0]->iLocationData.iNetworkInfo.iNetworkId.Length() > 0 )
         {
         cond.AddPropertyConditionL( networkCodeDef, ETextPropertyConditionCompareEquals,
                 iMediaItems[0]->iLocationData.iNetworkInfo.iNetworkId );
         }
     iMediaHandlingFlag |= KLocationQueryInProgress;
     iLocationQuery->FindL();  

    LOG( "CLocationRecord::FindLocationFromDBL(), end" );     
    }


// --------------------------------------------------------------------------
// CLocationRecord::DoCreateLocationL
// --------------------------------------------------------------------------
//	
TItemId CLocationRecord::DoCreateLocationL( const TLocationData& aLocationData )
	{
    LOG( "CLocationRecord::DoCreateLocationL(), begin" );    
	TItemId locationObjectId;
	
	// required object properties
	CMdEPropertyDef& creationDef = iLocationObjectDef->GetPropertyDefL(
			Object::KCreationDateProperty );
	CMdEPropertyDef& modifiedDef = iLocationObjectDef->GetPropertyDefL(
			Object::KLastModifiedDateProperty );
	CMdEPropertyDef& sizeDef = iLocationObjectDef->GetPropertyDefL(
			Object::KSizeProperty );
	CMdEPropertyDef& itemTypeDef = iLocationObjectDef->GetPropertyDefL(
			Object::KItemTypeProperty );
	CMdEPropertyDef& offSetDef = iLocationObjectDef->GetPropertyDefL( 
			Object::KTimeOffsetProperty );

	// location related properties
	CMdEPropertyDef& cellIdDef = iLocationObjectDef->GetPropertyDefL(
			Location::KCellIdProperty );
	CMdEPropertyDef& directionDef = iLocationObjectDef->GetPropertyDefL(
			Location::KDirectionProperty );
	CMdEPropertyDef& speedDef = iLocationObjectDef->GetPropertyDefL( 
			Location::KSpeedProperty );
	CMdEPropertyDef& locationCodeDef = iLocationObjectDef->GetPropertyDefL( 
			Location::KLocationAreaCodeProperty );
	CMdEPropertyDef& countryCodeDef = iLocationObjectDef->GetPropertyDefL( 
			Location::KCountryCodeProperty );
	CMdEPropertyDef& networkCodeDef = iLocationObjectDef->GetPropertyDefL( 
			Location::KNetworkCodeProperty );
	CMdEPropertyDef& qualityDef = iLocationObjectDef->GetPropertyDefL( 
			Location::KQualityProperty );

	// location object
	CMdEObject* locationObject = NULL;

	locationObject = iMdeSession->NewObjectL( *iLocationObjectDef, Object::KAutomaticUri );
	CleanupStack::PushL( locationObject );

	TTime timestamp( 0 );
	timestamp.UniversalTime();

	TTimeIntervalSeconds timeOffset = User::UTCOffset();
	TTime localTime = timestamp + timeOffset;
	
	// required object properties
	locationObject->AddTimePropertyL( creationDef, localTime );
	locationObject->AddTimePropertyL( modifiedDef, timestamp );
	locationObject->AddUint32PropertyL( sizeDef, 0 ); // always zero size for location objects
	locationObject->AddTextPropertyL( itemTypeDef, Location::KLocationItemType );
	locationObject->AddInt16PropertyL( offSetDef, timeOffset.Int() / 60 );
	
	LOG1( "CLocationRecord::DoCreateLocationL(), Location created with stamp: %Ld", timestamp.Int64() );
	
	// location related properties
	if ( !Math::IsNaN( aLocationData.iPosition.Latitude() ) && 
		 !Math::IsNaN( aLocationData.iPosition.Longitude() ))
		{
        LOG1("CLocationRecord::DoCreateLocationL(), Lan - %f", aLocationData.iPosition.Latitude());
        LOG1("CLocationRecord::DoCreateLocationL(), Lon - %f", aLocationData.iPosition.Longitude());
		locationObject->AddReal64PropertyL( *iLatitudeDef, aLocationData.iPosition.Latitude() );
		locationObject->AddReal64PropertyL( *iLongitudeDef, aLocationData.iPosition.Longitude() );

		}
	if ( !Math::IsNaN( aLocationData.iPosition.Altitude() ) )
		{
		locationObject->AddReal64PropertyL( *iAltitudeDef, aLocationData.iPosition.Altitude() );
		}
	if ( !Math::IsNaN( aLocationData.iCourse.Course() ) )
		{
		locationObject->AddReal32PropertyL( directionDef, aLocationData.iCourse.Course() );
		}
	if ( !Math::IsNaN( aLocationData.iCourse.Speed() ) )
		{
		locationObject->AddReal32PropertyL( speedDef, aLocationData.iCourse.Speed() );
		}
	if ( !Math::IsNaN( aLocationData.iQuality ) )
		{
		locationObject->AddReal32PropertyL( qualityDef, aLocationData.iQuality );
		}

	// network related properties
	if ( aLocationData.iNetworkInfo.iAccess != CTelephony::ENetworkAccessUnknown )
		{
		LOG1("CLocationRecord::DoCreateLocationL(), Cell id - %d", aLocationData.iNetworkInfo.iCellId);
		locationObject->AddUint32PropertyL( cellIdDef, aLocationData.iNetworkInfo.iCellId );
		}
	if ( aLocationData.iNetworkInfo.iAreaKnown && 
        aLocationData.iNetworkInfo.iLocationAreaCode != 0 &&
		aLocationData.iNetworkInfo.iAccess != CTelephony::ENetworkAccessUnknown )
		{
		LOG1("CLocationRecord::DoCreateLocationL(), Areacode - %d", aLocationData.iNetworkInfo.iLocationAreaCode);
		locationObject->AddUint32PropertyL( locationCodeDef, 
				aLocationData.iNetworkInfo.iLocationAreaCode );
		}
#ifdef _DEBUG
	
    if ( aLocationData.iNetworkInfo.iCountryCode.Length() > 0 )
        {
        TLex lexer( aLocationData.iNetworkInfo.iCountryCode );
        TUint countryCode = 0;    
        TRAP_IGNORE(lexer.Val( countryCode, EDecimal));        
        LOG1("CLocationRecord::DoCreateLocationL(), Country code - %d", countryCode);
        }
    else
        {
        LOG("CLocationRecord::DoCreateLocationL(), No Country code");
        }
    
    //Set mobile network code
    if ( aLocationData.iNetworkInfo.iNetworkId.Length() > 0 )
        {
        TLex lexer = aLocationData.iNetworkInfo.iNetworkId;
        TUint networkCode = 0;
        TRAP_IGNORE(lexer.Val( networkCode, EDecimal));
        LOG1("CLocationRecord::DoCreateLocationL(), Network id - %d", networkCode); 
        }
    else
        {
        LOG("CLocationRecord::DoCreateLocationL(), No network code");
        }

#endif
	if ( aLocationData.iNetworkInfo.iCountryCode.Length() > 0 )
		{
		locationObject->AddTextPropertyL( countryCodeDef, 
				aLocationData.iNetworkInfo.iCountryCode );
		}

	if ( aLocationData.iNetworkInfo.iNetworkId.Length() > 0 )
		{
		locationObject->AddTextPropertyL(networkCodeDef, aLocationData.iNetworkInfo.iNetworkId);
		}
	        
	// Add the location object to the database.
	locationObjectId = iMdeSession->AddObjectL( *locationObject );
    LOG1("CLocationRecord::DoCreateLocationL(), Location id - %d", locationObjectId);
	CleanupStack::PopAndDestroy( locationObject );
    LOG( "CLocationRecord::DoCreateLocationL(), end" );

	return locationObjectId;
	}

// --------------------------------------------------------------------------
// CLocationRecord::CreateRelationL
// --------------------------------------------------------------------------
//
TItemId CLocationRecord::CreateRelationL( const TUint& aObjectId, const TUint& aLocationId )
	{ 
	LOG("CLocationRecord::CreateRelationL - start");
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	
	// "contains" relation definition
	CMdERelationDef& containsRelDef = namespaceDef.GetRelationDefL( 
			Relations::KContainsLocation );

	CMdERelation* relationObject = iMdeSession->NewRelationLC( containsRelDef,
			aObjectId, aLocationId, 0 );
	if ( !relationObject )
		{
		User::Leave( KErrBadHandle );
		}
	TItemId relationId = iMdeSession->AddRelationL( *relationObject );

	CleanupStack::PopAndDestroy( relationObject );
	LOG("CLocationRecord::CreateRelationL - end");
	
	return relationId; 
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::ReadCenRepValueL
// --------------------------------------------------------------------------
//
void CLocationRecord::ReadCenRepValueL(TInt aKey, TInt& aValue)
	{
	LOG( "CLocationRecord::::ReadCenRepValueL(), begin" );
	CRepository* repository;
	repository = CRepository::NewLC( KRepositoryUid );
	User::LeaveIfError(repository->Get( aKey, aValue));
	CleanupStack::PopAndDestroy(repository);
    LOG( "CLocationRecord::::ReadCenRepValueL(), end" );   
	}


// --------------------------------------------------------------------------
// CLocationRecord::HandleQueryNewResults
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleQueryNewResults(CMdEQuery& /*aQuery*/, TInt /*aFirstNewItemIndex*/, 
		TInt /*aNewItemCount*/)
	{
	}


// --------------------------------------------------------------------------
// CLocationRecord::HandleQueryCompleted
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleQueryCompleted(CMdEQuery& aQuery, TInt aError)
    {
    LOG("CLocationRecord::HandleQueryCompleted - start");
    const TInt count = aQuery.Count();
    LOG1("CLocationRecord::HandleQueryCompleted count: %d", count);
     
    if ( aError != KErrNone )
        {
		HandleQueryFailure();
        }
    
    else if ( &aQuery == iLocationQuery )
        {
     
      	TRAPD(err,HandleLocationQueryL( aQuery ));
		if(err != KErrNone)
			{
			// unable to process the first node..
			// remove this and process the next.

			// reset the flag
			HandleQueryFailure();
			}
        
        }
    else if ( &aQuery == iNetLocationQuery )
        {
      	TRAPD(err,HandleNetLocationQueryL( aQuery ));
		 if(err != KErrNone)
			 {
			 // unable to process the first node..
			 // remove this and process the next.
			 HandleQueryFailure();
			 }
        
        }

#ifdef LOC_REVERSEGEOCODE
    else if ( &aQuery == iImageQuery )
        {
        if(aQuery.Count() > 0)
            {
           
            CMdERelation& relation = static_cast<CMdERelation&>( aQuery.ResultItem( 0 ) );
                                
            TItemId imageId = relation.LeftObjectId(); 
                
            TRAPD(err, GetTagsL( imageId ) );
			if(err != KErrNone)
				{
				// unable to process the first node..
				// remove this and process the next.
			
				// reset the flag
				HandleQueryFailure();
				}
            }
        else
            {
			HandleQueryFailure();
            }
        }
    
    else if ( &aQuery == iTagQuery )
        {
        TRAPD(err, HandleTagQueryL( aQuery ) );
        
        GeoTaggingCompleted();    
		if(err != KErrNone)
			{
			// unable to process the first node..
			// remove this and process the next.

			// reset the flag
			HandleQueryFailure();
			}
        }
#endif //LOC_REVERSEGEOCODE
	else
		{
		// execution should not come over here...still take recovery action.
		HandleQueryFailure();
		}
    GeoTaggingCompleted();
    LOG( "CLocationRecord::HandleQueryCompleted(), end" );	
    }

// --------------------------------------------------------------------------
// CLocationRecord::HandleQueryFailure()
// handle MDS query sceenario
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleQueryFailure()
    {
    LOG( "CLocationRecord::HandleQueryFailure(), begin" );    
	iMediaHandlingFlag &= ~KLocationQueryInProgress;
	if ( iMediaItems.Count() > 0 )
		{
		TLocationSnapshotItem* firstPtr = iMediaItems[0];
		iMediaItems.Remove(0);
		delete firstPtr;
		iMediaItems.Compress();
		HandleFindLocationFromDB();
		}
    LOG( "CLocationRecord::HandleQueryFailure(), end" );	
	}

// --------------------------------------------------------------------------
// CLocationRecord::HandleFindLocationFromDB()
// handle find location from DB within the non leaving method
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleFindLocationFromDB()
    {
    LOG( "CLocationRecord::HandleFindLocationFromDB(), begin" );    
	if ( iMediaItems.Count() > 0 )
	   {
		TInt trapErr = KErrNone;
		TRAP(trapErr,FindLocationFromDBL());
		// no memory, don't proceed further
		// other error sceenario, we can move the node to the end and process the next
		if(trapErr != KErrNoMemory && trapErr != KErrNone)
			{
			// other than no memory
			TInt numberOfNodes = iMediaItems.Count();
			LOG1("media count - %d\n", numberOfNodes);
			while(--numberOfNodes >= 0 &&
				trapErr != KErrNoMemory && 
				trapErr != KErrNone)
				{
				// first remove the node
				TLocationSnapshotItem* firstPtr = iMediaItems[0];
				iMediaItems.Remove(0);
				// move this to last
				iMediaItems.Append(firstPtr);
				iMediaItems.Compress();
				trapErr = KErrNone;
				// process for the next till we reached the last node.
				TRAP(trapErr,FindLocationFromDBL());
				}
			}
	   }
    LOG( "CLocationRecord::HandleFindLocationFromDB(), end" );	
	}        
        
        
// --------------------------------------------------------------------------
// CLocationRecord::HandleLocationQuery()
// handle if only gps info available
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleLocationQueryL( CMdEQuery& aQuery )
    {
    LOG( "CLocationRecord::HandleLocationQueryL(), begin" );    
    TInt error = KErrNone;
    TUint locationId( 0 );
    if ( iMediaItems.Count() <= 0 )
        {
        LOG("CLocationRecord::HandleLocationQueryL. No media items to process");
        return;
        }
    
    TLocationData locationData = iMediaItems[0]->iLocationData;
    
    const TInt count = aQuery.Count();
    //find any location matches
    for ( TInt i = 0; i < count; i++ )
        {
        LOG1("CLocationRecord::HandleLocationQueryL check item: %d", i);
        CMdEItem& item = aQuery.ResultItem(i);
        CMdEObject& locationObject = static_cast<CMdEObject&>(item);
        
        CMdEProperty* latProp = NULL;
        CMdEProperty* lonProp = NULL; 
        CMdEProperty* altProp = NULL;
        
        locationObject.Property( *iLatitudeDef, latProp, 0 );
        locationObject.Property( *iLongitudeDef, lonProp, 0 );
        locationObject.Property( *iAltitudeDef, altProp, 0 );
        
        if ( latProp && lonProp )
            {
            TReal32 distance;
            TCoordinate newCoords;
                    
            TReal64 lat = latProp->Real64ValueL();
            TReal64 lon = lonProp->Real64ValueL();
            if ( altProp )
                {
                TReal32 alt = (TReal32)altProp->Real64ValueL();                       
                newCoords = TCoordinate( lat, lon, alt );
                }
            else
                {
                newCoords = TCoordinate( lat, lon );
                }
                            
            locationData.iPosition.Distance(newCoords, distance);
                            
            if ( distance < iLocationDelta )  
                {
                 i = count;
                 locationId = locationObject.Id();
                 iMediaItems[0]->iLocationId = locationId;
                 break;
                }
            }
        }
           
    
    if ( locationId )
        {
        TRAP( error, CreateRelationL( iMediaItems[0]->iObjectId, locationId ) );
        
#ifdef LOC_REVERSEGEOCODE
        if( error == KErrNone)
           {             
           //find out if image for this location is tagged already
           GetRelatedImageL( locationId ); 
           }
#else        
       TLocationSnapshotItem* firstPtr = iMediaItems[0];
       iMediaItems.Remove(0);
       delete firstPtr;
       iMediaItems.Compress();
       iMediaHandlingFlag &= ~KLocationQueryInProgress;
       if ( iMediaItems.Count() > 0 )
          {
          FindLocationFromDBL();
          }
#endif //LOC_REVERSEGEOCODE
        }
    else
        {
        
        if((iMediaItems[0]->iFlag & KDownloadMediaFile) > 0)
            {
            locationData.iNetworkInfo.iCellId = 0;
            locationData.iNetworkInfo.iAccess = CTelephony::ENetworkAccessUnknown;
            locationData.iNetworkInfo.iLocationAreaCode = 0;
            locationData.iNetworkInfo.iCountryCode.Zero();
            locationData.iNetworkInfo.iNetworkId.Zero();
            }
        TRAP( error, locationId = DoCreateLocationL( locationData ) );

        if ( error == KErrNone )
           {
           iMediaItems[0]->iLocationId = locationId;
           TRAP( error, CreateRelationL( iMediaItems[0]->iObjectId, locationId ));
           }
        
        TLocationSnapshotItem* item = iMediaItems[0];
        if((iMediaItems[0]->iFlag & KSnapMediaFile) > 0)
            {
            iLastMediaItem = *(iMediaItems[0]);
            }
        iMediaItems.Remove(0);
        iMediaItems.Compress();
        iMediaHandlingFlag &= ~KLocationQueryInProgress;
        if ( error == KErrNone )
           {
           //Find the address by coordinate, results a call to ReverseGeocodeComplete()
#ifdef LOC_REVERSEGEOCODE
           iLocationItems.Append( item );
           if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress))
               {
               iMediaHandlingFlag |= KReverseGeoCodingInProgress;

               if(iRevGeocoderPlugin)
			       {
                   iRevGeocoderPlugin->GetAddressByCoordinateL
				        ( iLocationItems[0]->iLocationData.iPosition,
                           iConnectionOption);
				   }
               }
#else
            // free resources
            delete item;
#endif //LOC_REVERSEGEOCODE
            }
        else
            {
            // free resources
            delete item;
            }
        
        if ( iMediaItems.Count() > 0 )
           {
           FindLocationFromDBL();
           }
        }
    LOG( "CLocationRecord::HandleLocationQueryL(), end" );

    }

// --------------------------------------------------------------------------
// CLocationRecord::HandleNetLocationQuery()
// handle if only network info available
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleNetLocationQueryL( CMdEQuery& aQuery )
    {
    LOG( "CLocationRecord::HandleNetLocationQueryL(), begin" );    
    TInt error = KErrNone;
    TUint locationId( 0 );
    if ( iMediaItems.Count() <= 0 )
        {
        LOG("CLocationRecord::HandleNetLocationQueryL(), End. No media items to process\n");
        return;
        }
    if( aQuery.Count() )
        {
        CMdEItem& item = aQuery.ResultItem(0);
        CMdEObject& locationObject = static_cast<CMdEObject&>(item);
        locationId = locationObject.Id();
        
        iMediaItems[0]->iLocationId = locationId;
        TRAP( error, CreateRelationL( iMediaItems[0]->iObjectId, locationId ) );
#ifdef LOC_REVERSEGEOCODE
        //check if found location object has lat, long
        CMdEProperty* latProp = NULL;
        CMdEProperty* lonProp = NULL; 
        CMdEProperty* cellIdProp = NULL;
        CMdEProperty* areadCodeProp = NULL; 
        CMdEProperty* countryProp = NULL;
        CMdEProperty* networkCodeProp = NULL; 
        if ( error == KErrNone )
            {

            CMdEPropertyDef& cellIdDef = iLocationObjectDef->GetPropertyDefL(
                           Location::KCellIdProperty );
            CMdEPropertyDef& locationAreadCodeDef = iLocationObjectDef->GetPropertyDefL( 
                           Location::KLocationAreaCodeProperty );
            CMdEPropertyDef& countryCodeDef = iLocationObjectDef->GetPropertyDefL( 
                           Location::KCountryCodeProperty );
            CMdEPropertyDef& networkCodeDef = iLocationObjectDef->GetPropertyDefL( 
                           Location::KNetworkCodeProperty );

            locationObject.Property( *iLatitudeDef, latProp, 0 );
            locationObject.Property( *iLongitudeDef, lonProp, 0 );      

            locationObject.Property( cellIdDef, cellIdProp, 0 );
            locationObject.Property( locationAreadCodeDef, areadCodeProp, 0 );      
            locationObject.Property( countryCodeDef, countryProp, 0 );
            locationObject.Property( networkCodeDef, networkCodeProp, 0 );      
            } 
        
        if( (latProp && lonProp)
            || (cellIdProp && areadCodeProp && countryProp && networkCodeProp) )
            {
            //find out if image for this location is tagged already
            GetRelatedImageL( locationId );           
            }
        else
#endif //LOC_REVERSEGEOCODE
            {
            // no geo info.. remove the item and proceed for the next.
            TLocationSnapshotItem* firstPtr = iMediaItems[0];
            iMediaItems.Remove(0);
            delete firstPtr;
            iMediaItems.Compress();
            iMediaHandlingFlag &= ~KLocationQueryInProgress;
            if ( iMediaItems.Count() > 0 )
               {
               FindLocationFromDBL();
               }
            }
        }
    else
        {
#ifdef LOC_GEOTAGGING_CELLID
        LOG1("Media handling flag = %d", iMediaHandlingFlag);
        if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0)
            {
            iMediaHandlingFlag |= KSnapGeoConvertInPendingState;
            }
        else
            {
            // go for cell id based geo coding.
            if(iGeoConverter == NULL)
               {
               iGeoConverter = CGeoConverter::NewL(*this);
               }
            iGeoConverter->ConvertL(iMediaItems[0]->iLocationData.iNetworkInfo);
            iMediaHandlingFlag |= KSnapGeoConvertInProgress;
            iMediaHandlingFlag &= ~KSnapGeoConvertInPendingState;
            }
#else
        // cell id based geo tagging is not supported.. go for remapping.
        locationId = DoCreateLocationL( iMediaItems[0]->iLocationData );
        iMediaItems[0]->iLocationId = locationId;
        TItemId relationId = CreateRelationL( iMediaItems[0]->iObjectId, locationId );
        TLocationSnapshotItem* firstPtr = iMediaItems[0];

        // Go for remapping.. get locationdata from trail with object time
        TTime timestamp = GetMdeObjectTimeL( iMediaItems[0]->iObjectId );
        TRemapItem remapItem;
        remapItem.iObjectId = iMediaItems[0]->iObjectId;
        remapItem.iTime = timestamp;
        remapItem.iLocationId = locationId;
        remapItem.iRelationId = relationId;
        iRemapper->Append( remapItem );
        
        iMediaItems.Remove(0);
        delete firstPtr;
        iMediaItems.Compress();
        iMediaHandlingFlag &= ~KLocationQueryInProgress;
        if ( iMediaItems.Count() > 0 )
            {
            FindLocationFromDBL();
            }
#endif
        }
#ifdef LOC_REVERSEGEOCODE
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0 || 
        (iMediaHandlingFlag & KReverseGeoCodingInProgress) > 0)
#else        
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0)
#endif //LOC_REVERSEGEOCODE
        {
        // stop n/w info change listener, since device may connect to n/w 
        // and local trail will receive so many call backs on current n/w info change.
        iNetworkInfoChangeListener->StopNwInfoChangeNotifier();
        }
    else
        {
        iNetworkInfoChangeListener->StartNwInfoChangeNotifier();
        }
    LOG( "CLocationRecord::HandleNetLocationQueryL(), end" );

    }     


#ifdef LOC_GEOTAGGING_CELLID	
// --------------------------------------------------------------------------
// CLocationRecord::ConversionCompletedL()
// --------------------------------------------------------------------------
//
void CLocationRecord::ConversionCompletedL( const TInt aError, TLocality& aPosition )
    {

    LOG1("CLocationRecord::ConversionCompletedL, begin. Error - %d", aError);
    // reset the flag first
    iMediaHandlingFlag &= ~KSnapGeoConvertInProgress;
    LOG1("iRemapState - %d", iRemapState);
    if(aError == KErrNone)
        {
        iConvertRetry = ETrue;
        LOG("Conversion completed successfully");
        if(iRemapState == ERemapNwGeoConverterInProgress)
            {
            iNewItem.iLocationData.iPosition.SetCoordinate
                ( aPosition.Latitude(), aPosition.Longitude(), aPosition.Altitude());
            iNewItem.iLocationData.iQuality = aPosition.HorizontalAccuracy();
            TItemId locationId = DoCreateLocationL( iNewItem.iLocationData );
            iRemapper->UpdateRelationsL( locationId ); 
#ifdef LOC_REVERSEGEOCODE
            if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress))
                {
                iRemapState = ERemapRevGeoCodeInProgress;
                //Find the address by coordinate, results a call to ReverseGeocodeComplete()
                iMediaHandlingFlag |= KReverseGeoCodingInProgress;

                if(iRevGeocoderPlugin)
                	{
                	iRevGeocoderPlugin->GetAddressByCoordinateL
                	    ( iNewItem.iLocationData.iPosition, iConnectionOption ); 
                	}
                }
            else
                {
                // remap in reverse geocoding pending state.
                iRemapState = ERemapRevGeoCodePending;
                }
#endif //LOC_REVERSEGEOCODE
            }
        else if ( iMediaItems.Count() > 0 )
           {
            
           iMediaItems[0]->iLocationData.iPosition.SetCoordinate
                ( aPosition.Latitude(), aPosition.Longitude(), aPosition.Altitude());
           iMediaItems[0]->iLocationData.iQuality = aPosition.HorizontalAccuracy();
           TLocationSnapshotItem* item = iMediaItems[0];
           TItemId locationId = DoCreateLocationL( iMediaItems[0]->iLocationData );
           iMediaItems[0]->iLocationId = locationId;
           TItemId relationId = CreateRelationL( iMediaItems[0]->iObjectId, locationId );

           // Go for remapping.. get locationdata from trail with object time
           TTime timestamp = GetMdeObjectTimeL( iMediaItems[0]->iObjectId );
           TRemapItem remapItem;
           remapItem.iObjectId = iMediaItems[0]->iObjectId;
           remapItem.iTime = timestamp;
           remapItem.iLocationId = locationId;
           remapItem.iRelationId = relationId;
           iRemapper->Append( remapItem );

           if((iMediaItems[0]->iFlag & KSnapMediaFile) > 0)
               {
               iLastMediaItem = *(iMediaItems[0]);
               }
           iMediaItems.Remove(0);
           iMediaItems.Compress();
           iMediaHandlingFlag &= ~KLocationQueryInProgress;
#ifdef LOC_REVERSEGEOCODE
           iLocationItems.Append( item );
#else
           // free resource
           delete item;
#endif
           }
       }
    else
        {
        LOG1("Conversion error - %d", aError);
        if(iConvertRetry)
            {
            LOG("Retry once");
            iConvertRetry = EFalse;
            if(iRemapState == ERemapNwGeoConverterInProgress)
                {
                iRemapState = ERemapNwGeoConverterPending;
                }
            else if ( iMediaItems.Count() > 0 )
                {
                iMediaHandlingFlag |= KSnapGeoConvertInPendingState;
                }
            }
        else
            {
            if(iRemapState == ERemapNwGeoConverterInProgress)
                {
                iRemapState = ERemapProgressNone;
                }
            else if ( iMediaItems.Count() > 0 )
                {
                TLocationSnapshotItem* item = iMediaItems[0];
                // Fails may be becuase of n/w reason..create location + relation so that we can handle at 3:00 AM.
                TItemId locationId = DoCreateLocationL( iMediaItems[0]->iLocationData );
                iMediaItems[0]->iLocationId = locationId;
                TItemId relationId = CreateRelationL( iMediaItems[0]->iObjectId, locationId );

                // Go for remapping.. get locationdata from trail with object time
                TTime timestamp = GetMdeObjectTimeL( iMediaItems[0]->iObjectId );
                TRemapItem remapItem;
                remapItem.iObjectId = iMediaItems[0]->iObjectId;
                remapItem.iTime = timestamp;
                remapItem.iLocationId = locationId;
                remapItem.iRelationId = relationId;
                iRemapper->Append( remapItem );

                iMediaItems.Remove(0);
                iMediaItems.Compress();
                iMediaHandlingFlag &= ~KLocationQueryInProgress;
                delete item;
                }
            }

        }

    // check the remap item first.
    if(iRemapState == ERemapNwGeoConverterPending)
        {
        // go for cell id based geo coding.
        iRemapState = ERemapNwGeoConverterInProgress;
        iGeoConverter->ConvertL(iNewItem.iLocationData.iNetworkInfo);
        iMediaHandlingFlag |= KSnapGeoConvertInProgress;
        }
    //check for media queue pending request.
    else if((iMediaHandlingFlag & KSnapGeoConvertInPendingState) > 0)
         {
         // previous media queue geo convert is in pending state.
         // go for cell id based geo coding.
         iGeoConverter->ConvertL(iMediaItems[0]->iLocationData.iNetworkInfo);
         iMediaHandlingFlag |= KSnapGeoConvertInProgress;
         iMediaHandlingFlag &= ~KSnapGeoConvertInPendingState;
         }
    // let's not use multiple access point.
#ifdef LOC_REVERSEGEOCODE
    else if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress) &&
        iLocationItems.Count() > 0)
       {
       iMediaHandlingFlag |= KReverseGeoCodingInProgress;

       if(iRevGeocoderPlugin)
           {
           iRevGeocoderPlugin->GetAddressByCoordinateL
                  ( iLocationItems[0]->iLocationData.iPosition,
                    iConnectionOption);
           }
       } 
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0 || 
        (iMediaHandlingFlag & KReverseGeoCodingInProgress) > 0)
#else
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0)
#endif //LOC_REVERSEGEOCODE
        {
        // stop n/w info change listener, since device may connect to n/w 
        // and local trail will receive so many call backs on current n/w info change.
        iNetworkInfoChangeListener->StopNwInfoChangeNotifier();
        }
    else
        {
        iNetworkInfoChangeListener->StartNwInfoChangeNotifier();
        }
    FindLocationFromDBL();
    GeoTaggingCompleted();
    LOG("CLocationRecord::ConversionCompletedL, end");
    }


//------------------------------------------------------------------------
// CLocationRecord::HandleConversionError
//------------------------------------------------------------------------
//
void CLocationRecord::HandleConversionError(TInt aError)
	{
    LOG( "CLocationRecord::HandleConversionError(), begin" );	
    ARG_USED(aError);
    if(iRemapState == ERemapNwGeoConverterInProgress)
        {
        iRemapState = ERemapProgressNone;
        }
    else if ( iMediaItems.Count() > 0 )
        {
        TLocationSnapshotItem* item = iMediaItems[0];
        iMediaItems.Remove(0);
        iMediaItems.Compress();
        iMediaHandlingFlag &= ~KLocationQueryInProgress;
        delete item;
        }
        // let's not use multiple access point.
#ifdef LOC_REVERSEGEOCODE
    if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress) &&
        iLocationItems.Count() > 0)
       {
       iMediaHandlingFlag |= KReverseGeoCodingInProgress;

       if(iRevGeocoderPlugin)
           {
           TRAP_IGNORE(iRevGeocoderPlugin->GetAddressByCoordinateL
                  ( iLocationItems[0]->iLocationData.iPosition,
                    iConnectionOption));
           }
       } 
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0 || 
        (iMediaHandlingFlag & KReverseGeoCodingInProgress) > 0)
#else
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0)
#endif //LOC_REVERSEGEOCODE
        {
        // stop n/w info change listener, since device may connect to n/w 
        // and local trail will receive so many call backs on current n/w info change.
        iNetworkInfoChangeListener->StopNwInfoChangeNotifier();
        }
    else
        {
        iNetworkInfoChangeListener->StartNwInfoChangeNotifier();
        }
    TRAP_IGNORE(FindLocationFromDBL());
    GeoTaggingCompleted();
    LOG( "CLocationRecord::HandleConversionError(), end" );	
	}

#endif // LOC_GEOTAGGING_CELLID

// --------------------------------------------------------------------------
// CLocationRecord::SetMdeSession
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::SetMdeSession( CMdESession* aSession )
	{
    LOG( "CLocationRecord::SetMdeSession(), begin" );	
	iMdeSession = aSession;
#ifdef LOC_REVERSEGEOCODE
	iTagCreator->SetSession( aSession );
#endif
	TRAPD(err, iRemapper->InitialiseL( aSession ));
	if( err != KErrNone )
		{
		delete iRemapper;
		iRemapper = NULL;
		}
	TRAP(err, InitialiseL() );
    LOG( "CLocationRecord::SetMdeSession(), end" );	
	}

// --------------------------------------------------------------------------
// CLocationRecord::InitialiseL
// --------------------------------------------------------------------------
//
void CLocationRecord::InitialiseL()
    {
    LOG( "CLocationRecord::InitialiseL(), begin" );    
    // namespace defaults
    iNamespaceDef = &iMdeSession->GetDefaultNamespaceDefL();
    
    // location object definitions
    iLocationObjectDef = &iNamespaceDef->GetObjectDefL( Location::KLocationObject );
    iLatitudeDef = &iLocationObjectDef->GetPropertyDefL( Location::KLatitudeProperty );
    iLongitudeDef = &iLocationObjectDef->GetPropertyDefL( Location::KLongitudeProperty );
    iAltitudeDef = &iLocationObjectDef->GetPropertyDefL( Location::KAltitudeProperty );
    LOG( "CLocationRecord::InitialiseL(), end" );	
    }
      

// --------------------------------------------------------------------------
// CLocationRecord::StartTimerL
// --------------------------------------------------------------------------
//  
void CLocationRecord::StartTimerL()
	{
	LOG("CLocationRecord::StartTimerL,begin");
	
	if( !iNetworkInfoTimer->IsActive() )
	    {
	    iNetworkInfoTimer->Start( iInterval, iInterval, TCallBack( UpdateNetworkInfo, this ) );
	    }
    LOG( "CLocationRecord::StartL(), end" );	
	}


// --------------------------------------------------------------------------
// CLocationRecord::GetMdeObjectTimeL
// --------------------------------------------------------------------------
//
TTime CLocationRecord::GetMdeObjectTimeL( TItemId aObjectId ) 
    {
    LOG( "CLocationRecord::GetMdeObjectTimeL(), begin" );    
    CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();

    CMdEObjectDef& objectDef = namespaceDef.GetObjectDefL( Object::KBaseObject );
    CMdEPropertyDef& timeDef = objectDef.GetPropertyDefL( Object::KLastModifiedDateProperty );

    CMdEObject* object = NULL;
    CMdEProperty* property = NULL;
    
    object = iMdeSession->GetObjectL( aObjectId );
    if( !object )
        {
        User::Leave( KErrNotFound );
        }
    
    CleanupStack::PushL( object );
    object->Property( timeDef, property, 0 );
    if ( !property )
        {
        User::Leave( KErrNotFound );
        }
    
    const TTime timeValue( property->TimeValueL() );
    CleanupStack::PopAndDestroy( object );
    
    LOG( "CLocationRecord::GetMdeObjectTimeL(), end" );
    return timeValue;
        
    }



// --------------------------------------------------------------------------
// CLocationRecord::RemappingNeeded
// --------------------------------------------------------------------------
//
EXPORT_C TBool CLocationRecord::RemappingNeeded()
	{
	return ( iRemapper->ItemsInQueue() 
	    && (iRemapState == ERemapProgressNone));
	}




// --------------------------------------------------------------------------
// CLocationRecord::FindLocationWithSameNetInfoL()
// check any location object already exists with same network info
// --------------------------------------------------------------------------
//
void CLocationRecord::FindLocationWithSameNetInfoL()
    {
    LOG( "CLocationRecord::FindLocationWithSameNetInfoL(), begin" );   
    if(iMediaItems[0]->iLocationData.iNetworkInfo.iCellId > 0 &&
        iMediaItems[0]->iLocationData.iNetworkInfo.iCountryCode.Length() > 0 &&
        iMediaItems[0]->iLocationData.iNetworkInfo.iNetworkId.Length() > 0)
        {
        CMdEPropertyDef& cellIdDef = iLocationObjectDef->GetPropertyDefL(
                    Location::KCellIdProperty );
        CMdEPropertyDef& locationCodeDef = iLocationObjectDef->GetPropertyDefL( 
                    Location::KLocationAreaCodeProperty );
        CMdEPropertyDef& countryCodeDef = iLocationObjectDef->GetPropertyDefL( 
                    Location::KCountryCodeProperty );
        CMdEPropertyDef& networkCodeDef = iLocationObjectDef->GetPropertyDefL( 
                    Location::KNetworkCodeProperty );

        if(iNetLocationQuery)
            {
            iNetLocationQuery->RemoveObserver(*this);
            iNetLocationQuery->Cancel();
            delete iNetLocationQuery;
            iNetLocationQuery = NULL;
            }
        iNetLocationQuery = iMdeSession->NewObjectQueryL( *iNamespaceDef, *iLocationObjectDef, this );
        CMdELogicCondition& cond = iNetLocationQuery->Conditions();
        cond.SetOperator( ELogicConditionOperatorAnd );
            
        cond.AddPropertyConditionL( cellIdDef, 
                    TMdEUintEqual( iMediaItems[0]->iLocationData.iNetworkInfo.iCellId) );
        cond.AddPropertyConditionL( locationCodeDef, 
                    TMdEUintEqual( iMediaItems[0]->iLocationData.iNetworkInfo.iLocationAreaCode) );
        cond.AddPropertyConditionL( countryCodeDef, ETextPropertyConditionCompareEquals,
                iMediaItems[0]->iLocationData.iNetworkInfo.iCountryCode );
        cond.AddPropertyConditionL( networkCodeDef, ETextPropertyConditionCompareEquals,
                iMediaItems[0]->iLocationData.iNetworkInfo.iNetworkId );
            
        iNetLocationQuery->FindL(1, 1); 
        iMediaHandlingFlag |= KLocationQueryInProgress;
        }
    else
        {
        TLocationSnapshotItem* firstPtr = iMediaItems[0];
        iMediaItems.Remove(0);
        delete firstPtr;
        iMediaItems.Compress();
        iMediaHandlingFlag &= ~KLocationQueryInProgress;
        FindLocationFromDBL();
        }
    LOG( "CLocationRecord::FindLocationWithSameNetInfoL(), end" );

   }

EXPORT_C TBool CLocationRecord::IsLowBattery()
    {
    LOG("CLocationRecord::IsLowBattery()");
    RProperty batteryProperty;
    TInt batteryStatus;

    TInt error = batteryProperty.Get(KPSUidHWRMPowerState, KHWRMBatteryStatus, batteryStatus);
    LOG1("CLocationRecord::IsLowBattery() - battery status %d", batteryStatus );
    if( error != KErrNone || batteryStatus == EBatteryStatusOk )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

// --------------------------------------------------------------------------
// CLocationRecord::GeoTaggingCompleted
// --------------------------------------------------------------------------
//	
void CLocationRecord::GeoTaggingCompleted()
    {
    LOG( "CLocationRecord::GeoTaggingCompleted(), begin" );    
	if((iMediaItems.Count() ==  0) 
#ifdef LOC_REVERSEGEOCODE
        && (iLocationItems.Count() == 0) 
#endif //LOC_REVERSEGEOCODE
        )
	    {
        LOG("Geo tagging completed");
	    iGeoTaggerObserver.GeoTaggingCompleted(KErrNone);
		// fallback to silent
#ifdef LOC_REVERSEGEOCODE
		iConnectionOption = ESilent;
#endif //LOC_REVERSEGEOCODE
	    }
    LOG( "CLocationRecord::GeoTaggingCompleted(), end" );	
    }




// --------------------------------------------------------------------------
// CLocationRecord::TaggingInProgress
// --------------------------------------------------------------------------
//
EXPORT_C TBool CLocationRecord::TaggingInProgress()
	{
    LOG( "CLocationRecord::TaggingInProgress(), begin" );	
	TBool retVal = EFalse;
#ifdef LOC_REVERSEGEOCODE
    if( ((iMediaItems.Count() >  0) || (iLocationItems.Count() > 0) )
        && (iRevGeocoderPlugin && iRevGeocoderPlugin->SilentConnectionAllowed()))
#else        
    if( iMediaItems.Count() >  0 )

#endif //LOC_REVERSEGEOCODE
        {
#ifdef LOC_REVERSEGEOCODE
        if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress))
            {
            // start geocoding
           
            if (iLocationItems.Count() > 0 )
                {
                iMediaHandlingFlag |= KReverseGeoCodingInProgress;

            	TRAP_IGNORE( iRevGeocoderPlugin->GetAddressByCoordinateL( 
                            iLocationItems[0]->iLocationData.iPosition,
                            iConnectionOption) );
				
                retVal = ETrue;
                }
            }
        else
            {
            retVal = ETrue;
            }
#endif //LOC_REVERSEGEOCODE
        if(!(iMediaHandlingFlag & KLocationQueryInProgress))
            {
            if(iMediaItems.Count() > 0)
                {
				HandleFindLocationFromDB();
                retVal = ETrue;
                }
            }
        else
            {
            retVal = ETrue;
            }
        }
    else
        {
        // Flash the array to avoid double tagging by photos & localrail.
        }
    LOG( "CLocationRecord::TaggingInProgress(), end" );	
    return retVal;
	}


// ----------------------------------------------------------------------------
// CLocationRecord::GetCurrentRegisteredNw()
// ---------------------------------------------------------------------------- 
EXPORT_C RMobilePhone::TMobilePhoneNetworkInfoV2& CLocationRecord::GetCurrentRegisteredNw()
    {
    LOG( "CLocationRecord::GetCurrentRegisteredNw ,begin" ); 
    return iNetworkInfoChangeListener->GetCurrentRegisterNw();
    }

// --------------------------------------------------------------------------
// CLocationRecord::StartGeoTagging
// --------------------------------------------------------------------------
//
EXPORT_C TBool CLocationRecord::StartGeoTagging(const TConnectionOption aConnectionOption)
	{
    LOG( "CLocationRecord::StartGeoTagging(), begin" );	
	TBool retVal = EFalse;
    ARG_USED(aConnectionOption);
 	if((iMediaItems.Count() >  0) 
#ifdef LOC_REVERSEGEOCODE
        || (iLocationItems.Count() > 0) 
#endif //LOC_REVERSEGEOCODE
        )
        {
#ifdef LOC_REVERSEGEOCODE
        iConnectionOption = aConnectionOption;
        if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress))
            {
            // start geocoding
            if (iLocationItems.Count() > 0 )
                {
                iMediaHandlingFlag |= KReverseGeoCodingInProgress;
                if(iRevGeocoderPlugin)
                	{
               		 TRAP_IGNORE( iRevGeocoderPlugin->GetAddressByCoordinateL( 
                                    iLocationItems[0]->iLocationData.iPosition,
                                    iConnectionOption) );
                	}
                retVal = ETrue;
                }
            }
        else
            {
            retVal = ETrue;
            }
#endif //LOC_REVERSEGEOCODE
        if(!(iMediaHandlingFlag & KLocationQueryInProgress))
            {
            if(iMediaItems.Count() > 0)
                {
				HandleFindLocationFromDB();
                retVal = ETrue;
                }
            }
        else
            {
            retVal = ETrue;
            }
        }
    LOG( "CLocationRecord::StartGeoTagging(), end" );	
    return retVal;
	}


// --------------------------------------------------------------------------
// CLocationRecord::CancelGeoTagging
// --------------------------------------------------------------------------
//
EXPORT_C void CLocationRecord::CancelGeoTagging()
	{
#ifdef LOC_REVERSEGEOCODE
    LOG1( "CLocationRecord::CancelGeoTagging(), Connetion opt - %d", 
	             iConnectionOption );	
	// set this to default connection.
	iConnectionOption = ESilent;
#endif //LOC_REVERSEGEOCODE
    }


#ifdef LOC_REVERSEGEOCODE

// --------------------------------------------------------------------------
// CLocationManagerServer::GetRelatedImages()
// Find any image, already related to this location object
// --------------------------------------------------------------------------
//
void CLocationRecord::GetRelatedImageL(TItemId aLocID)
    {
    LOG( "CLocationRecord::GetRelatedImageL(), begin" );    
    CMdEObjectDef& imageObjDef = iNamespaceDef->GetObjectDefL( Image::KImageObject );
    
	if(iImageQuery)
		{
		iImageQuery->RemoveObserver(*this);
		iImageQuery->Cancel();
		delete iImageQuery;
        iImageQuery = NULL;
		}
    
    iImageQuery = iMdeSession->NewRelationQueryL( *iNamespaceDef, this );
    User::LeaveIfNull( iImageQuery );

    iImageQuery->SetResultMode( EQueryResultModeItem );
    
    // both left and right condition must match
    CMdERelationCondition& filterCond = iImageQuery->Conditions().
            AddRelationConditionL( ERelationConditionSideRight );

    // left one must be any image object.
    filterCond.LeftL().AddObjectConditionL( imageObjDef );

    // right one must be this location object 
    filterCond.RightL().AddObjectConditionL( aLocID );

    iImageQuery->FindL(1, 1); // results to a call to HandleQueryCompleted()
    LOG( "CLocationRecord::GetRelatedImageL(), end" );    
    }



// --------------------------------------------------------------------------
// CLocationRecord::ReverseGeocodeComplete()
// Get address details like country, city..
// Create country and city tags and attach to the current image/video object
// --------------------------------------------------------------------------
//
void CLocationRecord::ReverseGeocodeComplete( TInt& aErrorcode, MAddressInfo& aAddressInfo )
    {
    LOG( "CLocationRecord::ReverseGeocodeComplete(), begin" );    
    TItemId countryTagId(0);
    TItemId cityTagId(0);
    
    iMediaHandlingFlag &= (~KReverseGeoCodingInProgress);
	TLocationSnapshotItem* snapshotItem = NULL;
    LOG1("Error - %d", aErrorcode);
    if( aErrorcode == KErrNone )
        {
        iLastReverseGeocodeFails = EFalse;
        TPtrC countryPtr( aAddressInfo.GetCountryName() );
        TPtrC cityPtr( aAddressInfo.GetCity() );
        TRAP_IGNORE( iTagCreator->CreateLocationTagsL( countryPtr, countryTagId, 
                                                      cityPtr, cityTagId ) );
        if ( iRemapState == ERemapRevGeoCodeInProgress)
            {
            TRAP_IGNORE( iRemapper->AttachGeoTagsL( iTagCreator, countryTagId, cityTagId ) );
            iRemapState = ERemapProgressNone;
            }
        else
            {
            if(iLocationItems.Count() > 0)
                {
                iLocationItems[0]->iCountryTagId = countryTagId;
                iLocationItems[0]->iCityTagId = cityTagId;
				iLocationItems[0]->iReverseGeocodeSuccess = 1 ;
				
                iLastLocationItem = (*iLocationItems[0]);
                
                TRAP_IGNORE( iTagCreator->AttachTagsL( 
                                iLocationItems[0]->iObjectId, countryTagId, cityTagId  ) );
                if ( (iLastMediaItem.iFlag & KSnapMediaFile) > 0 
                    && iLastMediaItem.iLocationId == iLastLocationItem.iLocationId && (iLastLocationItem.iReverseGeocodeSuccess == 1))
                    {
                    LOG("Updating country/city\n");
                    iLastMediaItem.iCountryTagId = countryTagId;
                    iLastMediaItem.iCityTagId = cityTagId;
					iLastMediaItem.iReverseGeocodeSuccess = 1;
                    }
                
                }
            }
        //check other items in the array has same location 
        for ( TInt index = iLocationItems.Count() - 1; index > 0; index--)
            {
            if ( iLocationItems[index]->iLocationId == iLastLocationItem.iLocationId )
                {
                LOG1("Attached tags in for - %d\n", index);
                TRAP_IGNORE( iTagCreator->AttachTagsL( 
                                iLocationItems[index]->iObjectId, countryTagId, cityTagId  ) );
				snapshotItem = iLocationItems[index];
                iLocationItems.Remove(index);
				delete snapshotItem;
                }
            }
        }
    else
        {
        //handle error
        LOG("Reverse geo coding fails");
        iLastReverseGeocodeFails = ETrue;
        }
    
    //irrespective of error or not, remove current(first) item to proceed further
    if ( iLocationItems.Count() > 0 )
        {
    	snapshotItem = iLocationItems[0];
        iLocationItems.Remove(0); 
    	delete snapshotItem;
    	iLocationItems.Compress();
        }
    if (aErrorcode == KErrNone)
        {
        if(iRemapState == ERemapRevGeoCodePending)
            {
            if(iRevGeocoderPlugin)
            	{
            	TRAPD(err, iRevGeocoderPlugin->GetAddressByCoordinateL( iNewItem.iLocationData.iPosition, iConnectionOption );) 
            	if(err == KErrNone)
            		{
            		// Remap geo coding in pending state
            		iRemapState = ERemapRevGeoCodeInProgress;
            		//Find the address by coordinate, results a call to ReverseGeocodeComplete()
            		iMediaHandlingFlag |= KReverseGeoCodingInProgress;
            		}
            	}
            }
        else if(iLocationItems.Count() > 0)
            {
            if(iRevGeocoderPlugin)
            	{
           		TRAPD(err, iRevGeocoderPlugin->GetAddressByCoordinateL( 
                                iLocationItems[0]->iLocationData.iPosition,
                                iConnectionOption) );
            	if(err == KErrNone)
            		{
            		// queue is not empty process the next.
            		iMediaHandlingFlag |= KReverseGeoCodingInProgress;
            		}
            	}
            }
        }

    GeoTaggingCompleted();
    if((iMediaHandlingFlag & KSnapGeoConvertInProgress) > 0 || 
        (iMediaHandlingFlag & KReverseGeoCodingInProgress) > 0)
        {
        // stop n/w info change listener, since device may connect to n/w 
        // and local trail will receive so many call backs on current n/w info change.
        iNetworkInfoChangeListener->StopNwInfoChangeNotifier();
        }
    else
        {
        iNetworkInfoChangeListener->StartNwInfoChangeNotifier();
        }
    LOG( "CLocationRecord::ReverseGeocodeComplete(), end" );	
    }


// ----------------------------------------------------------------------------
// CLocationRecord::IsRegisteredAtHomeNetwork()
// ---------------------------------------------------------------------------- 
TBool CLocationRecord::IsRegisteredAtHomeNetwork()
    {
    LOG( "CLocationRecord::IsRegisteredAtHomeNetwork" );
    return iGeoTaggerObserver.IsRegisteredAtHomeNetwork();
    }

// ----------------------------------------------------------------------------
// CLocationRecord::GetHomeNetworkInfo()
// ----------------------------------------------------------------------------
const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        CLocationRecord::GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag)
    {
    LOG( "CLocationRecord::GetHomeNetworkInfo" );
    return iGeoTaggerObserver.GetHomeNetworkInfo(aHomeNwInfoAvailableFlag);
    }

// ----------------------------------------------------------------------------
// CLocationRecord::GetCurrentRegisterNw()
// ---------------------------------------------------------------------------- 
RMobilePhone::TMobilePhoneNetworkInfoV2& CLocationRecord::GetCurrentRegisterNw()
    {
    LOG( "CLocationRecord::GetCurrentRegisterNw ,begin" ); 
    return GetCurrentRegisteredNw();
    }


// --------------------------------------------------------------------------
// CLocationRecord::HandleTagQuery()
// handle if only gps info available
// --------------------------------------------------------------------------
//
void CLocationRecord::HandleTagQueryL( CMdEQuery& aQuery )
    {
    LOG( "CLocationRecord::HandleTagQueryL(), begin" );    
    TItemId countryTagId = 0;
    TItemId cityTagId = 0;
    TRAPD( error, FindCountryAndCityTagL( aQuery, countryTagId, cityTagId ) );
    if ( error == KErrNone )
        {
        LOG1("Media count - %d\n", iMediaItems.Count());
        if ( !countryTagId && !cityTagId )
            {
             if ( iMediaItems.Count() > 0 )
                {
                iLocationItems.Append( iMediaItems[0] );
                if((iMediaItems[0]->iFlag & KSnapMediaFile) > 0)
                    {
                    iLastMediaItem = *(iMediaItems[0]);
                    }
                iMediaItems.Remove(0);
                iMediaItems.Compress();
                iMediaHandlingFlag &= ~KLocationQueryInProgress;
                }

#ifdef LOC_REVERSEGEOCODE
            if(!(iMediaHandlingFlag & KReverseGeoCodingInProgress) &&
                iLocationItems.Count() > 0)
               {
               iMediaHandlingFlag |= KReverseGeoCodingInProgress;
               if(iRevGeocoderPlugin)
			       {
                   iRevGeocoderPlugin->GetAddressByCoordinateL
				          ( iLocationItems[0]->iLocationData.iPosition,
                            iConnectionOption);
				   }
               } 
#endif //LOC_REVERSEGEOCODE
            }
        else 
            {

            iTagCreator->AttachTagsL( iMediaItems[0]->iObjectId, countryTagId, cityTagId );
            if ( iMediaItems.Count() > 0 )
                {
                iMediaItems[0]->iCountryTagId = countryTagId;
                iMediaItems[0]->iCityTagId = cityTagId;
                TLocationSnapshotItem* item = iMediaItems[0];
                if((iMediaItems[0]->iFlag & KSnapMediaFile) > 0)
                    {
    				iLastMediaItem = *item;
                    }
                iMediaItems.Remove(0);
                iMediaItems.Compress();
                iMediaHandlingFlag &= ~KLocationQueryInProgress;
                delete item;
                }
            }
        
        }
    if ( iMediaItems.Count() > 0 )
        {
        FindLocationFromDBL();
        }
    LOG( "CLocationRecord::HandleTagQueryL(), end" );	
    }


// --------------------------------------------------------------------------
// CLocationRecord::FindCountryAndCityTagL()
// Go through all attached tags to get location tags only
// --------------------------------------------------------------------------
//
void CLocationRecord::FindCountryAndCityTagL( CMdEQuery& aQuery, 
                                             TItemId& aCountryTagId, TItemId& aCityTagId )
    {
    LOG( "CLocationRecord::FindCountryAndCityTagL(), begin" );    
    TItemId tagId = 0;
    CMdEObject* object = NULL;
    TInt error = KErrNone;
    
    const TInt count = aQuery.Count();
                
    for ( TInt i = 0; i < count; i++ )
        {
        CMdERelation& relation =  static_cast<CMdERelation&>( aQuery.ResultItem( i ) );
                    
        tagId = relation.RightObjectId(); 
        TRAP_IGNORE( object = iMdeSession->GetObjectL( tagId ) );
        
        if ( !aCountryTagId )
            {
            error = object->Uri().Find( KCountry );
            //just make sure 'country' appears first in the tag uri
            if ( error == KErrNone )
                {
                aCountryTagId = tagId;
                continue;
                }
            }
                    
        if ( !aCityTagId )
            {
            error = object->Uri().Find( KCity );
            if ( error == KErrNone )
                {
                aCityTagId = tagId;
                }
            }
        
        if ( aCountryTagId && aCityTagId )
            {
            i = count;
            }
        }
    LOG( "CLocationRecord::FindCountryAndCityTagL(), end" );	
                
    }

// --------------------------------------------------------------------------
// CLocationRecord::GetTagsL()
// --------------------------------------------------------------------------
//
void CLocationRecord::GetTagsL( TItemId aImageID )
    {
    LOG( "CLocationRecord::GetTagsL(), begin" );    
    CMdEObjectDef& tagObjectDef = iNamespaceDef->GetObjectDefL( Tag::KTagObject );
        
    if(iTagQuery)
        {
        iTagQuery->RemoveObserver(*this);
        iTagQuery->Cancel();
        delete iTagQuery;
        iTagQuery = NULL;
        }   
        
    iTagQuery = iMdeSession->NewRelationQueryL( *iNamespaceDef, this );
    User::LeaveIfNull( iTagQuery );

    iTagQuery->SetResultMode( EQueryResultModeItem );
        
    // both left and right condition must match
    CMdERelationCondition& filterCond = iTagQuery->Conditions().
    AddRelationConditionL( ERelationConditionSideRight );

    // left one must be this image object.
    filterCond.LeftL().AddObjectConditionL( aImageID );

    // right one must be tag object 
    filterCond.RightL().AddObjectConditionL( tagObjectDef );

    iTagQuery->FindL(); // results to a call to HandleQueryCompleted()
    LOG( "CLocationRecord::GetTagsL(), end" );    
    }
    
#endif //LOC_REVERSEGEOCODE


// --------------------------------------------------------------------------
// CLocationRecord::HandleActiveProfileEventL()
// --------------------------------------------------------------------------
//    
void CLocationRecord::HandleActiveProfileEventL( TProfileEvent aProfileEvent,TInt aProfileId )
    {
     LOG( "CLocationRecord::HandleActiveProfileEventL(), begin" );
	 ARG_USED(aProfileEvent);
	 iOfflineCheck = ETrue;
     
	    
     if ( EProfileOffLineId == aProfileId )
         {
         LOG( "HandleActiveProfileEventL() if condition to stop trail, begin" ); 
         iNetwork.iAreaKnown = EFalse;
         iNetwork.iAccess = CTelephony::ENetworkAccessUnknown;
         iNetwork.iCellId = 0;
         iNetwork.iLocationAreaCode = 0;
         iNetwork.iCountryCode.Zero();
         iNetwork.iNetworkId.Zero();
         iOfflineCheck = EFalse;
         LOG( "HandleActiveProfileEventL() if condition to stop trail, end" );   	
         }
     LOG( "CLocationRecord::HandleActiveProfileEventL(), end" );    
    }

// End of file


