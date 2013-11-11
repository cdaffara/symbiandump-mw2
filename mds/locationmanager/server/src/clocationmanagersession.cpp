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
* Description:  A session class for LocationManagerServer.
*
*/

#include <e32debug.h>
#include <s32mem.h>
#include <lbssatellite.h> 
#include <locationdatatype.h>
#include <locationeventdef.h>

#include "clocationmanagerserver.h"
#include "clocationmanagersession.h"
#include "locationmanagerdefs.h"
#include "locationmanagerdebug.h"
#include "rlocationtrail.h"
#include "rlocationobjectmanipulator.h"
#include "rlocationtrail.h"
#include "mdsutils.h"

// --------------------------------------------------------------------------
// CLocationManagerSession::CLocationManagerSession
// C++ constructor.
// --------------------------------------------------------------------------
//
CLocationManagerSession::CLocationManagerSession() 
    {
    
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::~CLocationManagerSession
// C++ destructor.
// --------------------------------------------------------------------------
//
CLocationManagerSession::~CLocationManagerSession()
    {
    LOG( "CLocationManagerSession::~CLocationManagerSession(), begin" );
    // don't call stop GPS positioning from here...
    // we are expecting the application to start and stop else on server terminate, we will do the same.
    // this is not okay when user takes a photograph and then transfer photo using BT/PC .
    // at this point, it will stop the trail for camera where as camera session is still valid.
    

    Server().CancelNotificationRequest( iNotificationHandle );
    Server().CancelLocationRequest( iLocationHandle );
    Server().CancelTrackLogNotificationRequest( iTrackLogNotificationHandle );
    Server().RemoveSession();
    LOG( "CLocationManagerSession::~CLocationManagerSession(), end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CreateL
// From CSession2, used to increase server's session count.
// --------------------------------------------------------------------------
//
void CLocationManagerSession::CreateL()
    {
    LOG( "CLocationManagerSession::CreateL(), begin" );
    Server().AddSession();
    LOG( "CLocationManagerSession::CreateL(), end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::Server
// Reference to server.
// --------------------------------------------------------------------------
//
CLocationManagerServer& CLocationManagerSession::Server()
    {   
    return *static_cast<CLocationManagerServer*>
           ( const_cast<CServer2*>( CSession2::Server() ) );
    }
    
// --------------------------------------------------------------------------
// CLocationManagerSession::ServiceL
// From CSession2, passes the request forward to DispatchMessageL.
// --------------------------------------------------------------------------
//
void CLocationManagerSession::ServiceL( const RMessage2& aMessage )
    {
    LOG( "CLocationManagerSession::ServiceL, begin" );
    LOG1("Message id - %d", aMessage.Function());
    iMessage = RMessage2( aMessage );
    TRAPD( err, DispatchMessageL( aMessage ) );
    if ( err != KErrNone )
        {
        aMessage.Complete( err );
        }    
    LOG( "CLocationManagerSession::ServiceL, end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::DispatchMessageL
// Handles the request from client.
// --------------------------------------------------------------------------
//
void CLocationManagerSession::DispatchMessageL( const RMessage2& aMessage )
    {
    LOG( "CLocationManagerSession::DispatchMessageL" );
    switch( aMessage.Function() )
        {
        case ELocManStartTrail:
            StartLocationTrailL( aMessage );
            break;        
        case ELocManStopTrail:
            StopLocationTrailL( aMessage );
            break;        
        case ELocManTrailState:
            GetLocationTrailStateL( aMessage );
            break;        
        case ELocManNotifyTrailStateChange:
            NotifyLocationTrailStateChangeL( aMessage );
            break;        
        case ELocManCancelNotification:
            CancelNotificationRequest( aMessage );
            break;
        case ELocManRetrieveLoc:
            RetrieveLocationL( aMessage );
            break;
        case ELocManCurrentLoc:
            CurrentLocationL( aMessage );
            break;
        case ELocManCancelCurrentLoc:
            CancelLocationRequest( aMessage );
            break;
        case ELocManCreateLocationObject:
        	CreateLocationObjectL( aMessage );
        	break;
        case ELocManLocationSnapshot:
        	LocationSnapshotL( aMessage );
        	break;
        case ELocManRemoveLocationObject:
        	RemoveLocationObjectL( aMessage );
        	break;
        case ELocManCopyLocationDataById:
        	CopyLocationDataByIdL( iMessage );
        	break;
        case ELocManCopyLocationDataByUri:
	    	CopyLocationDataByURIL( iMessage );
	    	break;		
        case ELocManStartTrackLog:
        	StartTrackLogL( aMessage );
        	break;
        case ELocManStopTrackLog:
        	StopTrackLogL( aMessage );
        	break;
        case ELocManGetTrackLogStatus:
        	GetTrackLogStatusL( aMessage );
        	break;
        case ELocManDeleteTrackLog:
        	DeleteTrackLogL( aMessage );
        	break;
        case ELocManIsTrackLogRecording:
        	IsTrackLogRecordingL( aMessage );
        	break;
        case ELocManTrackLogName:
        	TrackLogNameL( aMessage );
        	break;
        case ELocManRegObserver:
        	RegisterTrackLogObserver( aMessage );
        	break;
        case ELocManGetCaptureSetting:
        	GetCaptureSettingL( aMessage );
        	break;
        case ELocManCancelTrackLogNotify:
        	CancelTrackLogNotificationRequest( aMessage );
        	break;
        case ELocManTagPending:
        	Server().TagPendingRequestL(aMessage);
        	break;
        case ELocManStartGeoTaging:
            Server().StartGeoTaggingL(aMessage);
            break;
        case ELocManCancelTagPendingReq:
            Server().CancelTagPendingRequest(aMessage);
			break;
		case ELocManCancelGeoTaggingReq:
            Server().CancelGeoTaggingRequest(aMessage);
            break;
        default:
            aMessage.Complete( KErrArgument );
            break;
        }
    }
  
// --------------------------------------------------------------------------
// CLocationManagerSession::StartLocationTrailL
// --------------------------------------------------------------------------
//
void CLocationManagerSession::StartLocationTrailL( const RMessage2& aMessage )
    {
    const TInt KParamSetting = 0;
    
    RLocationTrail::TTrailCaptureSetting setting;
    TPckg<RLocationTrail::TTrailCaptureSetting> locSetting( setting ); 
    aMessage.ReadL(KParamSetting, locSetting);    
    
    LOG1 ("CLocationManagerSession::StartLocationTrailL TTrailCaptureSetting: %d", setting);
    
    Server().StartGPSPositioningL( setting );
    aMessage.Complete( KErrNone );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::StopLocationTrailL
// --------------------------------------------------------------------------
//
void CLocationManagerSession::StopLocationTrailL( const RMessage2& aMessage )
    {
    Server().StopGPSPositioningL();
    if( !aMessage.IsNull() )
        {
        aMessage.Complete( KErrNone );
        }
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::GetLocationTrailStateL
// --------------------------------------------------------------------------
//
void CLocationManagerSession::GetLocationTrailStateL( const RMessage2& aMessage )
    {
    RLocationTrail::TTrailState state;
    Server().GetLocationTrailState( state );
        
    TPckg<RLocationTrail::TTrailState> locState( state );    
    aMessage.WriteL( 0, locState );
    aMessage.Complete( KErrNone );    
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::NotifyLocationTrailStateChangeL
// --------------------------------------------------------------------------
//
void CLocationManagerSession::NotifyLocationTrailStateChangeL( const RMessage2& aMessage )
    {
    iNotificationHandle = aMessage.Handle();
    Server().AddNotificationRequestL( aMessage );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CancelNotificationRequest
// --------------------------------------------------------------------------
//
void CLocationManagerSession::CancelNotificationRequest( const RMessage2& aMessage )
    {
    Server().CancelNotificationRequest( iNotificationHandle );
    aMessage.Complete( KErrNone );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::RetrieveLocationL
// --------------------------------------------------------------------------
//
void CLocationManagerSession::RetrieveLocationL( const RMessage2& aMessage )
    {
    TTime time( 0 );
    TPckg<TTime> timeBuf( time );
    aMessage.ReadL( EArgumentTime, timeBuf );
    
    TLocationData locationData;
    TLocTrailState state;
    Server().GetLocationByTimeL( time, locationData, state );

    TPckg<TLocationData> locDataBuf( locationData );
    aMessage.WriteL( EArgumentLocationData, locDataBuf );
    
    TPckg<TUint> stateBuf( state );
    aMessage.WriteL( EArgumentState, stateBuf );

    aMessage.Complete( KErrNone );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CurrentLocationL
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::CurrentLocationL( const RMessage2& aMessage )
    {
    LOG ("CLocationManagerSession::CurrentLocationL begin");
    iLocationHandle = aMessage.Handle();
    // server creates completion after it has received current location data
    Server().RequestCurrentLocationL( aMessage );
    LOG ("CLocationManagerSession::CurrentLocationL end");
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CancelLocationRequest()
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::CancelLocationRequest( const RMessage2& aMessage )
    {
    Server().CancelLocationRequest( iLocationHandle );
    aMessage.Complete( KErrNone );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CancelTrackLogNotificationRequest()
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::CancelTrackLogNotificationRequest( const RMessage2& aMessage )
    {
    Server().CancelTrackLogNotificationRequest( iTrackLogNotificationHandle );
    aMessage.Complete( KErrNone );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CreateLocationObjectL
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::CreateLocationObjectL( const RMessage2& aMessage )
    {
    LOG( "CLocationManagerSession::CreateLocationObjectL begin" );
    const TInt KParamData = 0;
    const TInt KParamId = 1;
    
    TLocationData locationData;
    TUint objectId;
	
    TPckg<TLocationData> locData( locationData );
    aMessage.ReadL( KParamData, locData );
    
    TPckg<TUint> objId( objectId);
    aMessage.ReadL( KParamId, objId );
    
    Server().CreateLocationObjectL( locationData, objectId );
    
    aMessage.Complete( KErrNone );
    LOG( "CLocationManagerSession::CreateLocationObjectL end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::LocationSnapshotL
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::LocationSnapshotL( const RMessage2& aMessage )
    {
    LOG( "CLocationManagerSession::LocationSnapshotL begin" );
    const TInt KParamId = 0;
    
    TUint objectId;
	
    TPckg<TUint> objId( objectId);
    aMessage.ReadL( KParamId, objId );
    
    Server().LocationSnapshotL( objectId );
    
    aMessage.Complete( KErrNone );
    LOG( "CLocationManagerSession::LocationSnapshotL end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::RemoveLocationObjectL
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::RemoveLocationObjectL( const RMessage2& aMessage )
    {
    LOG( "CLocationManagerSession::RemoveLocationObjectL begin" );
    const TInt KParamId = 0;
    TUint objectId;
    TPckg<TUint> locObjectId( objectId );
    aMessage.ReadL(KParamId, locObjectId);
    Server().RemoveLocationObjectL(objectId);
    aMessage.Complete( KErrNone );
    LOG( "CLocationManagerSession::RemoveLocationObjectL end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CopyLocationDataByIdL
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::CopyLocationDataByIdL( const RMessage2& aMessage )
    {
    Server().InitCopyLocationByIdL( aMessage );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::CopyLocationDataByURIL
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::CopyLocationDataByURIL( const RMessage2& aMessage )
    {
    Server().InitCopyLocationByURIL( aMessage );
    }

// --------------------------------------------------------------------------
// CLocationManagerSession::StartTrackLog
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::StartTrackLogL( const RMessage2& aMessage )
	{
	RLocationTrail::TTrailState state;
	Server().GetLocationTrailState( state );
	
	if ( state == RLocationTrail::ETrailStopped )
		{
		Server().StartGPSPositioningL( RLocationTrail::ECaptureAll );
		}
	
	Server().StartTrackLogL();

	aMessage.Complete( KErrNone );
	}

// --------------------------------------------------------------------------
// CLocationManagerSession::StopTrackLog
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::StopTrackLogL( const RMessage2& aMessage )
	{
	Server().StopTrackLogL();

	aMessage.Complete( KErrNone );
	}

// --------------------------------------------------------------------------
// CLocationManagerSession::GetStatus
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::GetTrackLogStatusL(const RMessage2& aMessage)
	{
	const TInt KParamRec = 0;
	const TInt KParamQuality = 1;
	TBool rec = EFalse;
	TPositionSatelliteInfo fixQuality;
	
	Server().GetTrackLogStatus( rec, fixQuality );
	
	TPckg<TBool> wrapRec( rec );
	aMessage.WriteL( KParamRec, wrapRec );
	
	TPckg<TPositionSatelliteInfo> wrapQuality( fixQuality );
	aMessage.WriteL( KParamQuality, wrapQuality );

	aMessage.Complete( KErrNone );
	}

// --------------------------------------------------------------------------
// CLocationManagerSession::DeleteTrackLog
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::DeleteTrackLogL(const RMessage2& aMessage)
	{
	const TInt KParamUri = 0;
	const TInt KParamUriLength = aMessage.GetDesLength(KParamUri);
	
    TInt err = KErrNone;	
	if (KParamUriLength > 0)
    	{
    	// read TDesC& aURI from request
	    HBufC* uriBuf = HBufC::NewLC( KParamUriLength );
	    TPtr ptrUri( uriBuf->Des() );
	    aMessage.ReadL( KParamUri, ptrUri );  

	    err = Server().DeleteTrackLogL( ptrUri );
	    CleanupStack::PopAndDestroy( uriBuf );
    	}
	aMessage.Complete( err );
	}

// --------------------------------------------------------------------------
// CLocationManagerSession::IsTrackLogRecordingL
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::IsTrackLogRecordingL(const RMessage2& aMessage)
	{	
	const TInt KParamRec = 0;
	TBool rec = EFalse;

	Server().IsTrackLogRecording( rec );
	
	TPckg<TBool> wrapRec( rec );
	aMessage.WriteL(KParamRec, wrapRec);

	aMessage.Complete( KErrNone );
	}

// --------------------------------------------------------------------------
// CLocationManagerSession::TrackLogName
// --------------------------------------------------------------------------
//    
void CLocationManagerSession::TrackLogNameL(const RMessage2& aMessage)
	{	
	const TInt KParamName = 0;
	TFileName fileName;
	TInt err;
	
	err = Server().TrackLogName( fileName );
	aMessage.WriteL(KParamName, fileName);
	aMessage.Complete( err );
	}

// --------------------------------------------------------------------------
// CLocationManagerSession::RegisterTrackLogObserver
// --------------------------------------------------------------------------
//  
void CLocationManagerSession::RegisterTrackLogObserver( const RMessage2& aMessage )
	{
	iTrackLogNotificationHandle = aMessage.Handle();
	TRAP_IGNORE( Server().AddTrackLogNotificationRequestL( aMessage ) );
	}


// --------------------------------------------------------------------------
// CLocationManagerSession::GetCaptureSettingL
// --------------------------------------------------------------------------
//
void CLocationManagerSession::GetCaptureSettingL( const RMessage2& aMessage )
	{
	TInt KParamCaptureSetting = 0;
	RLocationTrail::TTrailCaptureSetting captureSetting = RLocationTrail::EOff;
	
	Server().GetCaptureSetting( captureSetting );
	
	TPckg<RLocationTrail::TTrailCaptureSetting> setting( captureSetting );
	aMessage.WriteL( KParamCaptureSetting, setting );
	aMessage.Complete( KErrNone );
	}
    
// End of file 
