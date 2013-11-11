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
* Description:  A Server class for LocationManagerServer.
*
*/

#include <e32debug.h>
#include <w32std.h>
#include <ecom.h>
#include <commsdattypesv1_1.h>
#include <cdblen.h>
#include <commsdat_partner.h>

#include "clocationmanagerserver.h"
#include "clocationmanagersession.h"

#include "locationtraildefs.h"
#include "locationmanagerdebug.h"

#include "mdesession.h"
#include "mdenamespacedef.h"
#include "mdeobjectdef.h"
#include "mdepropertydef.h"
#include "mdcserializationbuffer.h"
#include "clocationgeotagtimerao.h"
#include "nwregistrationstatushandler.h"

using namespace MdeConstants;
using namespace CommsDat;


// --------------------------------------------------------------------------
// RunServerL
// Initialize and run the server.
// --------------------------------------------------------------------------
//
static void RunServerL()
    {
    User::LeaveIfError( RThread().RenameMe( KLocServerName ) );

    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL( scheduler );
    CActiveScheduler::Install( scheduler );
    
    CLocationManagerServer* server = CLocationManagerServer::NewLC();

    RProcess::Rendezvous( KErrNone );
    
    CActiveScheduler::Start();
    
    CleanupStack::PopAndDestroy(server); 
    CleanupStack::PopAndDestroy(scheduler); 
    }

// --------------------------------------------------------------------------
// E32Main
// Server process entry-point.
// --------------------------------------------------------------------------
//
TInt E32Main()
    {   
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TInt ret( KErrNoMemory );
    if( cleanup )
        {
        TRAP( ret, RunServerL() );
        delete cleanup;
        cleanup = NULL;
        }
    return ret;
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::NewLC
// 2-phased constructor.
// --------------------------------------------------------------------------
//
CLocationManagerServer* CLocationManagerServer::NewLC()
    {
    CLocationManagerServer* self = new (ELeave) CLocationManagerServer();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::CLocationManagerServer()
// C++ constructor.
// --------------------------------------------------------------------------
//
CLocationManagerServer::CLocationManagerServer() 
    : CPolicyServer( KLocManagerSessionPriority, 
                     KLocationManagerPolicy, 
                     ESharableSessions ),
                     iASW(NULL),
                     iTimer( NULL ),
			         iSessionReady( EFalse ),
                     iTagId( 0 ),
                     iLocManStopDelay( 0 ),
                     iLocManStopRemapDelay( 0 ),
                     iCaptureSetting( RLocationTrail::EOff ),
                     iRemoveLocation( EFalse ),
                     iWaitForPositioningStopTimeout ( EFalse ),
                     iTelServerIsOpen(EFalse),
                     iPhoneIsOpen(EFalse),
                     iNwRegistrationStatusHandler(NULL),
                     iHomeNwInfoAvailableFlag(EFalse),
                     iGeoTagTimer(NULL)
#ifdef LOC_REVERSEGEOCODE
					 ,iGeoTaggingPendingReqObj(NULL)
					 ,iEcomSession(NULL)
#endif //LOC_REVERSEGEOCODE
    {
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::ConstructL
// 2nd phase constructor.
// --------------------------------------------------------------------------
//
void CLocationManagerServer::ConstructL()
    {
    LOG ("CLocationManagerServer::ConstructL() begin");
    
    StartL( KLocServerName );

    // initialize etel
    InitialisePhoneL();
    
#ifdef LOC_REVERSEGEOCODE
    iEcomSession = &(REComSession::OpenL());
#endif //LOC_REVERSEGEOCODE

    iASW = new (ELeave) CActiveSchedulerWait();
    
    iNwRegistrationStatusHandler = CNwRegistrationStatusHandler::NewL(iPhone);
    
    iLocationRecord = CLocationRecord::NewL(*this, iPhone);
    iTrackLog = CTrackLog::NewL();
    iMdeSession = CMdESession::NewL( *this );   

    iLocationRecord->SetObserver( this );
    
    iLocationRecord->SetAddObserver( iTrackLog );
    
    iTrackLog->AddGpxObserver( this );
    
    CRepository* repository = CRepository::NewLC( KRepositoryUid );
    TInt err = repository->Get( KLocationTrailShutdownTimer, iLocManStopDelay );
    
    LOG1("iLocManStopDelay:%d", iLocManStopDelay);
    
    if ( err != KErrNone )
        {
        LOG1("iLocManStopDelay err:%d", err);
        iLocManStopDelay = KLocationTrailShutdownDelay;
        }

    err = repository->Get( KLocationTrailRemapShutdownTimer, iLocManStopRemapDelay );
    CleanupStack::PopAndDestroy( repository );
    
    LOG1("iLocManStopRemapDelay:%d", iLocManStopRemapDelay);
    
    if ( err != KErrNone )
        {
        LOG1("iLocManStopRemapDelay err:%d", err);
        iLocManStopRemapDelay = KLocationTrailRemapShutdownDelay;
        }

    if( !iSessionReady )
        {
        iASW->Start();      
        }

    delete iASW;
    iASW = NULL;   

    if( iSessionReady )
        {
        LOG("Session is ready to give service");
        //Create the instance of the geotagging timer object
        // Create timer, if n/w or reverse geo code based feature flag is enabled
#if defined(LOC_REVERSEGEOCODE) || defined(LOC_GEOTAGGING_CELLID)
        iGeoTagTimer = CLocationGeoTagTimerAO::NewL(*iMdeSession, *this);
        //Schedule a task for geotagging every day at 3.00 AM
        iGeoTagTimer->StartTimer();
#endif        
        }
    else
        {
        LOG("Unable to open MDE session. Closing..");
        User::Leave( KErrCouldNotConnect );
        }
        
    RProcess process;
    process.SetPriority( EPriorityBackground );
    process.Close();
    
    LOG ("CLocationManagerServer::ConstructL() end");
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::~CLocationManagerServer()
// C++ destructor.
// --------------------------------------------------------------------------
//
CLocationManagerServer::~CLocationManagerServer()
    {
	LOG("CLocationManagerServer::~CLocationManagerServer ,begin");
#if defined(LOC_REVERSEGEOCODE) || defined(LOC_GEOTAGGING_CELLID)
    delete iGeoTagTimer;
    iGeoTagTimer = NULL;
#endif    
#ifdef LOC_REVERSEGEOCODE
    delete iGeoTaggingPendingReqObj;
    iGeoTaggingPendingReqObj = NULL;
#endif //LOC_REVERSEGEOCODE
    
    delete iLocationRecord;    
    iLocationRecord = NULL;
    delete iTrackLog;    
    iTrackLog = NULL;
    delete iTimer;
    iTimer = NULL;
    delete iRelationQuery;
    iRelationQuery = NULL;
    delete iASW;
    iASW = NULL;
    delete iMdeSession;
    iMdeSession = NULL;
    delete iNwRegistrationStatusHandler;
    iNwRegistrationStatusHandler = NULL;
    iTargetObjectIds.Close();
    CancelRequests(iNotifReqs);
    iNotifReqs.Close();
    CancelRequests(iLocationReqs);
    iLocationReqs.Close();
    CancelRequests(iTrackLogNotifyReqs);
    iTrackLogNotifyReqs.Close();
    CancelCopyRequests(iCopyReqs);
    iCopyReqs.Close();
    iSessionCount = 0;
	if(iPhoneIsOpen)
        {   
        iPhoneIsOpen = EFalse; // not required
        iPhone.Close();
        }
    if(iTelServerIsOpen)
        {
        iTelServerIsOpen = EFalse;
        iTelServer.Close();
        }
#ifdef LOC_REVERSEGEOCODE
	if(iEcomSession)
        {
        iEcomSession->Close();
        }
#endif //LOC_REVERSEGEOCODE
	LOG("CLocationManagerServer::~CLocationManagerServer ,end");
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::GetCommDbTSYnameL
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::GetCommDbTSYnameL(TDes& aTsyName)
	{
    LOG( "CLocationManagerServer::GetCommDbTSYnameL(), begin" );
#ifdef SYMBIAN_NON_SEAMLESS_NETWORK_BEARER_MOBILITY
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_2);
#else
	CMDBSession* db = CMDBSession::NewL(KCDVersion1_1);
#endif
	CleanupStack::PushL(db);

	CMDBField<TDesC>* globalSettingField = new(ELeave) CMDBField<TDesC>(KCDTIdBearerAvailabilityCheckTSY);
	CleanupStack::PushL(globalSettingField);
	globalSettingField->SetRecordId(1);
	globalSettingField->SetMaxLengthL(KMaxTextLength);
	globalSettingField->LoadL(*db);
	aTsyName = *globalSettingField;
	CleanupStack::PopAndDestroy(globalSettingField);

	CleanupStack::PopAndDestroy(db);
    LOG( "CLocationManagerServer::GetCommDbTSYnameL(), end" );
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::InitialisePhoneL
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::InitialisePhoneL()
	{
    LOG( "CLocationManagerServer::InitialisePhoneL(), begin" );
	User::LeaveIfError(iTelServer.Connect());
	iTelServerIsOpen = ETrue;
	TBuf<KCommsDbSvrMaxFieldLength> tsyName;
	GetCommDbTSYnameL(tsyName);

	User::LeaveIfError(iTelServer.LoadPhoneModule(tsyName));

	TInt numPhones;
	User::LeaveIfError(iTelServer.EnumeratePhones(numPhones));

	TInt phoneIndx;
	TInt ret = KErrHardwareNotAvailable;

	for(phoneIndx=0; phoneIndx<numPhones; phoneIndx++)
		{
		RTelServer::TPhoneInfo tInfo;
		ret = iTelServer.GetPhoneInfo(phoneIndx, tInfo);
		if(ret != KErrNone)
			{
			continue;
			}
		
		ret = iPhone.Open(iTelServer, tInfo.iName);	
		if(ret != KErrNone)
			{
			continue;
			}

		iPhoneIsOpen = ETrue;

		RPhone::TStatus status;
		User::LeaveIfError(iPhone.GetStatus(status));
		if(status.iModemDetected!=RPhone::EDetectedPresent)
			{
			ret = iPhone.Initialise();
			if(ret != KErrNone)
				{
				iPhone.Close();
				iPhoneIsOpen = EFalse;
				continue;
				}
			}
        // we found the correct phone
        break;
		}

	//
	// ret will be KErrNone if a valid phone was found...
	//
	
    LOG1( "CLocationManagerServer::InitialisePhoneL(), end. Err - %d", ret );
	User::LeaveIfError(ret);
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CancelRequests()
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CancelRequests(RArray<RMessage2>& aMessageList)
	{
	const TInt count = aMessageList.Count();

    for ( TInt i(0) ; i < count; i++ )
        {
        RMessage2& msg = aMessageList[i];
        
        if( !msg.IsNull() )
        	{
        	msg.Complete( KErrCancel );
        	}
        }
    aMessageList.Reset();
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::CancelCopyRequests
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CancelCopyRequests(RArray<TMessageQuery>& aMessageList)
	{
	const TInt count = aMessageList.Count();

    for ( TInt i(0) ; i < count; i++ )
        {
        const RMessage2& msg = aMessageList[i].iMessage;
        
        if( !msg.IsNull() )
        	{
        	msg.Complete( KErrCancel );
        	}
        }
    aMessageList.Reset();
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::HandleSessionOpened
// --------------------------------------------------------------------------
//
void CLocationManagerServer::HandleSessionOpened(CMdESession& /*aSession*/, TInt aError)
	{
	LOG ("CLocationManagerServer::HandleSessionOpened() start");
	if ( iASW && iASW->IsStarted() )
		{
		iASW->AsyncStop();
		}
	
	if ( KErrNone == aError )
		{
		iSessionReady = ETrue;
		TRAP_IGNORE( iTrackLog->StartRecoveryL() );
	    iLocationRecord->SetMdeSession( iMdeSession );
		}
	else
		{
		iSessionReady = EFalse;
		delete iMdeSession;
		iMdeSession = NULL;
		}
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::HandleSessionError
// --------------------------------------------------------------------------
//
void CLocationManagerServer::HandleSessionError(CMdESession& /*aSession*/, TInt aError)
	{
	LOG1 ("CLocationManagerServer::HandleSessionError() start, Error - %d", aError);
    ARG_USED(aError);
	iSessionReady = EFalse;
	delete iMdeSession;
	iMdeSession = NULL;

	if ( iASW && iASW->IsStarted() )
		{
		iASW->AsyncStop();
		}	
	}



// --------------------------------------------------------------------------
// CLocationManagerServer::IsSessionReady
// --------------------------------------------------------------------------
//
TBool CLocationManagerServer::IsSessionReady()
	{
	return iSessionReady;
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::NewSessionL
// from CServer2, creates a new session.
// --------------------------------------------------------------------------
//
CSession2* CLocationManagerServer::NewSessionL( const TVersion& aVersion, 
                                             const RMessage2& /*aMsg*/ ) const
    {
    TBool supported = User::QueryVersionSupported( TVersion( 
                                                KLocationManagerServerMajor,
                                                KLocationManagerServerMinor,
                                                KLocationManagerServerBuild ),
                                                aVersion );
    if( !supported )
        {
        User::Leave( KErrNotSupported );
        }

    return new (ELeave) CLocationManagerSession();  
    }   
    
// --------------------------------------------------------------------------
// CLocationManagerServer::AddSession
// --------------------------------------------------------------------------
//  
void CLocationManagerServer::AddSession()  
    {
    iSessionCount++;
    }
  
// --------------------------------------------------------------------------
// CLocationManagerServer::RemoveSession
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::RemoveSession()
    {
    LOG1( "CLocationManagerServer::RemoveSession. Session count - %d", iSessionCount);
    iSessionCount--;
    StopServer();
    }    

// --------------------------------------------------------------------------
// CLocationManagerServer::StartGPSPositioningL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::StartGPSPositioningL( RLocationTrail::TTrailCaptureSetting aCaptureSetting )
    {
    LOG( "CLocationManagerServer::StartGPSPositioningL" );
    iWaitForPositioningStopTimeout = EFalse;
    
    if ( aCaptureSetting == RLocationTrail::EOff )
    	{
    	return;
    	}
    
    iCaptureSetting = aCaptureSetting;
    
    RLocationTrail::TTrailState state;
    GetLocationTrailState( state );
    if ( state != RLocationTrail::ETrailStopped && state != RLocationTrail::ETrailStopping )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    if ( iTimer )
    	{
    	delete iTimer;
    	iTimer = NULL;
    	}
    
    iLocationRecord->StartL( aCaptureSetting );
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::StopGPSPositioning
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::StopGPSPositioningL()
    {
    LOG( "CLocationManagerServer::StopGPSPositioningL()" );
    iCaptureSetting = RLocationTrail::EOff;
    iWaitForPositioningStopTimeout = EFalse;
    
    RLocationTrail::TTrailState state;
    GetLocationTrailState( state );
     
    if( state != RLocationTrail::ETrailStopped && state != RLocationTrail::ETrailStopping )
        {
        if(iTimer == NULL)
            {
            TRAP_IGNORE(iTimer = CLocationServerTimerHandler::NewL(*this));
            }
        if ( iTimer == NULL)
            {
            // If timer can't be created we stop the location trail immediately.
            iLocationRecord->Stop();
            }   
        else
            {
            iLocationRecord->SetStateToStopping();
            iTimer->StartTimer( iLocManStopDelay * 1000000, MLocationServerTimerObserver::EStopRecording);
            }
        }
    
    // Always stop tracklog.
    StopTrackLogL();
    }

// --------------------------------------------------------------------------
// CLocationUtilityServer::StopRecording
// --------------------------------------------------------------------------
//
void CLocationManagerServer::StopRecording()
	{
    LOG( "CLocationManagerServer::StopRecording()" );    
    iWaitForPositioningStopTimeout = EFalse;
    iLocationRecord->Stop();		
	}

// --------------------------------------------------------------------------
// CLocationUtilityServer::LocationServerTimerCallBackL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::LocationServerTimerCallBackL
    (const TLocationServerTimerType aLocationServerTimerType, const TInt /*aErrorCode*/)
	{
    LOG1( "CLocationManagerServer::LocationServerTimerCallBackL, begin, Type - %d",
                aLocationServerTimerType);
	switch(aLocationServerTimerType)
        {
        case MLocationServerTimerObserver::EStopRecording:
            CheckForRemappingCallback();
            break;
        case MLocationServerTimerObserver::EPositioningStopTimeout:
            PositioningStopTimeout();
            break;
        case MLocationServerTimerObserver::ELocGeneralPurpose:
        default:
            // execution shouldn't come over here.
            LOG("Invalid timer type");
            break;
        }
    LOG( "CLocationManagerServer::LocationServerTimerCallBackL, end" );
	}

// --------------------------------------------------------------------------
// CLocationUtilityServer::PositioningStopTimeout
// --------------------------------------------------------------------------
//
void CLocationManagerServer::PositioningStopTimeout()
	{
    LOG( "CLocationManagerServer::PositioningStopTimeout" );
	StopRecording();
	}

// --------------------------------------------------------------------------
// CLocationUtilityServer::CheckForRemappingCallback
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CheckForRemappingCallback()
    {
    LOG( "CLocationManagerServer::CheckForRemappingCallback, begin" );
    if ( iLocationRecord->RemappingNeeded() && !iLocationRecord->IsLowBattery())
        {     
        iTimer->StartTimer( iLocManStopRemapDelay * 1000000, MLocationServerTimerObserver::EPositioningStopTimeout);
        iWaitForPositioningStopTimeout = ETrue;
        }
    else
        {        
        StopRecording();
        }
    LOG( "CLocationManagerServer::CheckForRemappingCallback, end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::GetLocationTrailState
// --------------------------------------------------------------------------
//
void CLocationManagerServer::GetLocationTrailState( RLocationTrail::TTrailState& aState )
    {
    iLocationRecord->LocationTrailState( aState );
    }
    
// --------------------------------------------------------------------------
// CLocationManagerServer::AddNotificationRequestL
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::AddNotificationRequestL( const RMessage2& aNotifReq )
    {
    LOG( "CLocationManagerServer::AddNotificationRequestL(), begin" );
    iNotifReqs.AppendL( aNotifReq );
    LOG( "CLocationManagerServer::AddNotificationRequestL(), end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::AddTrackLogNotificationRequestL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::AddTrackLogNotificationRequestL( const RMessage2& aNotifReq )
	{
	iTrackLogNotifyReqs.AppendL( aNotifReq );
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::CancelNotificationRequest
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::CancelNotificationRequest( const TInt aHandle )
    {
    LOG( "CLocationManagerServer::CancelNotificationRequest(), begin" );
    
    const TInt count = iNotifReqs.Count();
    for ( TInt i = count; --i >= 0; )
        {
        RMessage2& msg = iNotifReqs[i];

        if( msg.IsNull() )
        	{
        	iNotifReqs.Remove(i);
        	continue;
        	}

        if ( msg.Handle() == aHandle )
            {
           	msg.Complete( KErrCancel );
            iNotifReqs.Remove(i);
            break;
            }
        }
    LOG( "CLocationManagerServer::CancelNotificationRequest(), end" );
    }
 
// --------------------------------------------------------------------------
// CLocationManagerServer::GetLocationByTimeL
// --------------------------------------------------------------------------
//   
void CLocationManagerServer::GetLocationByTimeL( const TTime& aTimeStamp, 
												 TLocationData& aLocationData,
                                                 TLocTrailState& aState )
    {
    iLocationRecord->GetLocationByTimeL( aTimeStamp,
    									 aLocationData,
                                         aState );
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::RequestCurrentLocationL
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::RequestCurrentLocationL( const RMessage2& aCurrLocReq )
    {
    iLocationReqs.AppendL( aCurrLocReq );
    iLocationRecord->RequestLocationL();
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::CancelLocationRequest
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::CancelLocationRequest( const TInt aHandle )
    {
    LOG( "CLocationManagerServer::CancelLocationRequest(), begin" );
    
    const TInt count = iLocationReqs.Count();
    for ( TInt i = count; --i >= 0; )
        {
        RMessage2& msg = iLocationReqs[i];
        
        if( msg.IsNull() )
        	{
        	iLocationReqs.Remove(i);
        	continue;
        	}
        
        if ( msg.Handle() == aHandle )
            {
            msg.Complete( KErrCancel );
            iLocationReqs.Remove(i);
            break;
            }
        }
    if ( !iLocationReqs.Count() )
        {
        iLocationRecord->CancelLocationRequest();
        }
    LOG( "CLocationManagerServer::CancelLocationRequest(), end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::GetCurrentNetworkInfo
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::GetCurrentNetworkInfo( CTelephony::TNetworkInfoV1& aNetworkInfo )
    {
    iLocationRecord->GetNetworkInfo( aNetworkInfo );
    }    

// --------------------------------------------------------------------------
// CLocationManagerServer::LocationTrailStateChange
// --------------------------------------------------------------------------
//    
void CLocationManagerServer::LocationTrailStateChange()
    {
    LOG( "CLocationManagerServer::LocationTrailStateChange(), begin" );

    for ( TInt i = iNotifReqs.Count(); --i >= 0; )
        {
        RMessage2& msg = iNotifReqs[i];
        
        if( !msg.IsNull() )
        	{
        	msg.Complete( KErrNone );
        	}
        }
    iNotifReqs.Reset();
    LOG( "CLocationManagerServer::LocationTrailStateChange(), end" );
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::CurrentLocation
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CurrentLocation( const TPositionSatelliteInfo& aSatelliteInfo, 
											  const CTelephony::TNetworkInfoV1& aNetworkInfo,
                                              const TInt aError )
    {
    LOG( "CLocationManagerServer::CurrentLocation(), begin" );
    const TInt KParamLocationData = 0;
    
    TLocationData locationData;
    aSatelliteInfo.GetPosition( locationData.iPosition );
    aSatelliteInfo.GetCourse( locationData.iCourse );
    locationData.iNetworkInfo = aNetworkInfo;
    locationData.iSatellites = aSatelliteInfo.NumSatellitesUsed();
    
    TPckg<TLocationData> wrapLocationData( locationData );

    if ( aError == KErrNone )
    	{
    	for ( TInt i = iLocationReqs.Count(); --i >= 0; )
    		{
    		RMessage2& msg = iLocationReqs[i];

    		if( !msg.IsNull() )
    			{
        		TInt err = msg.Write( KParamLocationData, wrapLocationData );
        		LOG1( "CLocationManagerServer::CurrentLocation() location data written with error:%d", err);
        		msg.Complete( err );
    			}
    		}
    	}
    else
    	{
    	for ( TInt i = iLocationReqs.Count(); --i >= 0; )
    		{
    		RMessage2& msg = iLocationReqs[i];
    		
    		LOG1( "CLocationManagerServer::CurrentLocation() completed with error:%d", aError);
    		
    		if( !msg.IsNull() )
    			{
    			msg.Complete( aError );
    			}
    		}
    	}        

	iLocationReqs.Reset();

    LOG( "CLocationManagerServer::CurrentLocation(), end" );    
    }


// --------------------------------------------------------------------------
// CLocationManagerServer::GPSSignalQualityChanged
// --------------------------------------------------------------------------
//
void CLocationManagerServer::GPSSignalQualityChanged( const TPositionSatelliteInfo& aSatelliteInfo )
	{
	LOG( "CLocationManagerServer::GPSSignalQualityChanged" );
	const TInt KFixParam = 0;
	const TInt KPositionInfoParam = 1;
	const TInt KEventTypeParam = 2;
	TBool fix( ETrue );
	TPosition tmpPosition;
	TEventTypes eventType = ESignalChanged;
	
	TPckg<TPositionSatelliteInfo> wrapSatelliteInfo( aSatelliteInfo );
	TPckg<TBool> wrapFix( fix );
	TPckg<TEventTypes> wrapEventType( eventType );
	
	aSatelliteInfo.GetPosition( tmpPosition );
	if ( Math::IsNaN( tmpPosition.Latitude() ) || Math::IsNaN( tmpPosition.Longitude() ) )
		{
		fix = EFalse;
		LOG( "CLocationManagerServer::GPSSignalQualityChanged - no GPS fix");
		}
	
	TInt error( KErrNone );
	const TInt count = iTrackLogNotifyReqs.Count();
	for ( TInt i( count ); --i >= 0; )
		{
		RMessage2& msg = iTrackLogNotifyReqs[i];
		
		if( !msg.IsNull() )
			{
			LOG1( "CLocationManagerServer::GPSSignalQualityChanged request %d", i );
			error = msg.Write( KFixParam, wrapFix );
			if( KErrNone == error )
				{
				error = msg.Write( KPositionInfoParam, wrapSatelliteInfo );
				if( KErrNone == error )
					{
					error = msg.Write( KEventTypeParam, wrapEventType );
					}
				}
			LOG1( "CLocationManagerServer::GPSSignalQualityChanged error: %d", error );
			msg.Complete( error );
			}
		}
	iTrackLogNotifyReqs.Reset();
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CancelTrackLogNotificationRequest
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CancelTrackLogNotificationRequest( const TInt aHandle )
	{
	LOG( "CLocationManagerServer::CancelTrackLogNotificationRequest(), begin" );
    
    const TInt count = iTrackLogNotifyReqs.Count();
    for ( TInt i(count); --i >= 0; )
        {
        RMessage2& msg = iTrackLogNotifyReqs[i];
        
        if( msg.IsNull() )
        	{
        	iTrackLogNotifyReqs.Remove(i);
        	continue;
        	}
        
        if ( msg.Handle() == aHandle )
            {
            msg.Complete( KErrCancel );
            iTrackLogNotifyReqs.Remove(i);
            break;
            }
        }

    LOG( "CLocationManagerServer::CancelTrackLogNotificationRequest(), end" );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CreateLocationObjectL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CreateLocationObjectL( const TLocationData& aLocationData,
													const TUint& aObjectId )
	{
	if ( !IsSessionReady() )
		{
		User::Leave( KErrNotReady );
		}
	
	iLocationRecord->CreateLocationObjectL( aLocationData, aObjectId );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::LocationSnapshotL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::LocationSnapshotL( const TUint& aObjectId )
	{
    LOG( "CLocationManagerServer::LocationSnapshotL(), begin" );
	if ( !IsSessionReady() )
		{
        LOG( "Session is not ready" );
		User::Leave( KErrNotReady );
		}
	iLocationRecord->LocationSnapshotL( aObjectId );
    LOG( "CLocationManagerServer::LocationSnapshotL(), end" );
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::RemoveLocationObjectL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::RemoveLocationObjectL(TUint& aObjectId)
	{
	if ( !IsSessionReady() )
		{
		User::Leave( KErrNotReady );
		}
	
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	
	iRelationQuery = iMdeSession->NewRelationQueryL( namespaceDef, this );
    iRelationQuery->SetResultMode( EQueryResultModeId );
    iRelationQuery->Conditions().SetOperator( ELogicConditionOperatorAnd );
    
    CMdERelationCondition& filterCondLeft = iRelationQuery->Conditions().AddRelationConditionL( 
    		ERelationConditionSideLeft );

    // The left object in relation must have this ID.
    filterCondLeft.LeftL().AddObjectConditionL( aObjectId );
    
    // Right object in relation must be a location object.
    CMdERelationCondition& filterCondRight = iRelationQuery->Conditions().AddRelationConditionL( 
    		ERelationConditionSideRight );
    CMdEObjectDef& rightObjDef = namespaceDef.GetObjectDefL( Location::KLocationObject );
    filterCondRight.RightL().AddObjectConditionL( rightObjDef );
	
    iRemoveLocation = ETrue;
    iRelationQuery->FindL( 1, 1 );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CopyLocationObjectL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CopyLocationObjectL( TItemId aSource, 
		const RArray<TItemId>& aTargets, TMessageQuery& aMessageQuery )
	{
	if( aTargets.Count() <= 0 )
		{
		aMessageQuery.iMessage.Complete( KErrNotFound );
		return;
		}
	
	if ( !IsSessionReady() )
		{
		User::Leave( KErrNotReady );
		}
	
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	
	TMdEObject obj;
	iMdeSession->CheckObjectL( obj, aSource, &namespaceDef );

    aMessageQuery.iQuery = iMdeSession->NewRelationQueryL( namespaceDef, this );
    aMessageQuery.iQuery->SetResultMode( EQueryResultModeItem );
    aMessageQuery.iQuery->Conditions().SetOperator( ELogicConditionOperatorAnd );
    
    CMdERelationCondition& filterCondLeft = aMessageQuery.iQuery->Conditions()
    	.AddRelationConditionL( ERelationConditionSideLeft );

    // The left object in relation must have this ID.
    filterCondLeft.LeftL().AddObjectConditionL( aSource );
    
    // Right object in relation must be a location object.
    CMdERelationCondition& filterCondRight = aMessageQuery.iQuery->Conditions()
    	.AddRelationConditionL( ERelationConditionSideRight );
    CMdEObjectDef& rightObjDef = namespaceDef.GetObjectDefL( Location::KLocationObject );
    filterCondRight.RightL().AddObjectConditionL( rightObjDef );
	
    if( iTargetObjectIds.Count() <= 0 )
    	{
    	TInt err = 0;
	    for( TInt i = aTargets.Count() - 1; i >=0; i-- )
	    	{
	    	TRAP( err, iMdeSession->CheckObjectL( obj, aTargets[i], &namespaceDef ) );
	    	if ( err == KErrNone )
	    		{
		    	iTargetObjectIds.AppendL( aTargets[i] );
	    		}
	    	}
    	}
    
    iCopyReqs.AppendL( aMessageQuery );
    
    if ( iTargetObjectIds.Count() > 0 )
    	{
        aMessageQuery.iQuery->FindL( 1, 1 );
    	}
    else
    	{
    	aMessageQuery.iMessage.Complete( KErrNotFound );
    	iCopyReqs.Remove( iCopyReqs.Find( aMessageQuery ) );
    	}
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CopyLocationObjectL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CopyLocationObjectL( const TDesC& aSource, 
		const RArray<TPtrC>& aTargets, TMessageQuery& aQuery )
	{
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	TMdEObject obj;
	iMdeSession->CheckObjectL( obj, aSource, &namespaceDef );
	TItemId source = obj.Id();
	const TInt count = aTargets.Count();
	TInt err = 0;
	for( TInt i = 0; i < count; i++ )
		{
		TRAP(err, iMdeSession->CheckObjectL( obj, aTargets[i], &namespaceDef )); 
		if( err == KErrNone )
			{
			iTargetObjectIds.AppendL( obj.Id() );
			}
		}

	CopyLocationObjectL( source, iTargetObjectIds, aQuery );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::HandleQueryNewResults
// --------------------------------------------------------------------------
//
void CLocationManagerServer::HandleQueryNewResults( CMdEQuery& /*aQuery*/, 
		TInt /*aFirstNewItemIndex*/, TInt /*aNewItemCount*/ )
	{
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::HandleQueryCompleted
// --------------------------------------------------------------------------
//
void CLocationManagerServer::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
	{	
	LOG("CLocationManagerServer::HandleQueryCompleted");
	if ( iRemoveLocation )
		{
		if( aQuery.Count() > 0 && aError == KErrNone )
			{
			TRAPD( err, iMdeSession->RemoveRelationL( aQuery.ResultId( 0 ), &aQuery.NamespaceDef() ) );			
			if ( err != KErrNone )
				{
				LOG1( "CLocationManagerServer::HandleQueryCompleted error: %d", err );
				}
			}
		
		iRemoveLocation = EFalse;
		}
	else
		{
		// When results CopyLocationL handles completion of message
		if( aQuery.Count() > 0 && aError == KErrNone )
			{
			TRAP_IGNORE( CopyLocationL( aQuery ) );
			}
		// otherwise find correct message and complete it
		else
			{
			for ( TInt i = iCopyReqs.Count() - 1; i >= 0; --i  )
				{
				if ( iCopyReqs[i].iQuery == &aQuery )
					{
					if( aError == KErrNone )
						{
						aError = KErrNotFound;
						}
					iCopyReqs[i].iMessage.Complete( aError );
					delete iCopyReqs[i].iQuery;
					iCopyReqs[i].iQuery = NULL;
					iCopyReqs.Remove( i );
					break;
					}
				}
			}
		}

	iTargetObjectIds.Reset();
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::NewLC
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CopyLocationL( CMdEQuery& aQuery )
	{
	CMdEObjectDef& locationDef = aQuery.NamespaceDef().GetObjectDefL( Location::KLocationObject );

    CMdERelation& result = static_cast<CMdERelation&>( aQuery.ResultItem( 0 ) );
    TItemId rightId = result.RightObjectId();
    CMdEObject* sourceLocation = iMdeSession->GetObjectL( rightId, locationDef );
    CleanupStack::PushL( sourceLocation );
    
    // "contains" relation definition
    CMdERelationDef& containsRelDef = aQuery.NamespaceDef().GetRelationDefL( 
    		Relations::KContainsLocation );
    
    for( TInt i = iTargetObjectIds.Count() - 1; i >=0; i-- )
    	{
        CMdERelation* relationObject = iMdeSession->NewRelationLC( containsRelDef, iTargetObjectIds[i],
        		rightId, 0 );
        
        iMdeSession->AddRelationL( *relationObject );
        
        CleanupStack::PopAndDestroy( relationObject );
    	}
    
    CleanupStack::PopAndDestroy( sourceLocation );
    
    for ( TInt i = iCopyReqs.Count() - 1; i >= 0; --i  )
    	{
    	if ( iCopyReqs[i].iQuery == &aQuery )
    		{
    		iCopyReqs[i].iMessage.Complete( KErrNone );
    		delete iCopyReqs[i].iQuery;
    		iCopyReqs[i].iQuery = NULL;
    		iCopyReqs.Remove( i );
    		break;
    		}
    	}
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::InitCopyLocationByIdL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::InitCopyLocationByIdL( const RMessage2& aMessage )
	{
	const TInt KParamSourceId = 0;
	const TInt KParamTargetIds = 1;
	TItemId sourceId = 0;
	RArray<TItemId> targetIds;
	CleanupClosePushL(targetIds);
	
	// read TUint& aSourceId from request
	TPckg<TItemId> locSourceId( sourceId );	
	aMessage.ReadL(KParamSourceId, locSourceId);
	
	const TInt KParamTargetIdsLength = aMessage.GetDesLength( KParamTargetIds );
	LOG1("CLocationManagerServer::InitCopyLocationL KParamTargetIdsLength:%d", KParamTargetIdsLength);
	if ( KParamTargetIdsLength > 0 )
	    {
	    HBufC8* paramBuf = HBufC8::NewLC( KParamTargetIdsLength );
	    TPtr8 ptr( paramBuf->Des() );
	    aMessage.ReadL( KParamTargetIds, ptr );
	    
	    DeserializeArrayL( ptr, targetIds );
	    
	    TMessageQuery q( NULL, aMessage );
	    
	    LOG1("CLocationManagerServer::InitCopyLocationL ID count:%d", targetIds.Count());
	    	
	    CopyLocationObjectL( sourceId, targetIds, q );
	    
	    CleanupStack::PopAndDestroy(paramBuf);
	    }
	CleanupStack::PopAndDestroy(&targetIds);
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::InitCopyLocationByURIL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::InitCopyLocationByURIL( const RMessage2& aMessage )
	{
    LOG( "CLocationManagerSession::CopyLocationDataByUriL begin" );
    const TInt KParamSourceUri = 0;
    const TInt KParamTargetUris = 1;

    const TInt KParamSourceLength = aMessage.GetDesLength(KParamSourceUri);
    if (KParamSourceLength > 0)
    	{
    	// read TDesC& aSourceURI from request
	    HBufC* sourceUriBuf = HBufC::NewLC( KParamSourceLength );
	    TPtr ptrSource( sourceUriBuf->Des() );
	    aMessage.ReadL( KParamSourceUri, ptrSource );
	    
	    const TInt KParamTargetUrisLength = aMessage.GetDesLength( KParamTargetUris );
	    LOG1("CLocationManagerSession::CopyLocationDataByUriL KParamTargetUrisLength:%d", KParamTargetUrisLength);
	    if ( KParamTargetUrisLength > 0 )
	        {
		    RArray<TPtrC> targetUris;
		    CleanupClosePushL(targetUris);

		    CMdCSerializationBuffer* uriBuffer = CMdCSerializationBuffer::NewLC( aMessage, KParamTargetUris );
		    
		    TInt32 uriCount = 0;
		    uriBuffer->ReceiveL( uriCount );
		    
		    targetUris.ReserveL( uriCount );

		    // deserialize URIs
		    for( TInt i = uriCount- 1; i >=0; i-- )
		    	{
		    	targetUris.AppendL( uriBuffer->ReceivePtr16L() );
		    	}
		    
	        LOG1("CLocationManagerSession::CopyLocationDataByUriL ID count:%d", targetUris.Count());
	        
	        TMessageQuery q( NULL, aMessage );

	        CopyLocationObjectL( sourceUriBuf->Des(), targetUris, q );

		    CleanupStack::PopAndDestroy( uriBuffer );
	        CleanupStack::PopAndDestroy( &targetUris );
	        }
	    CleanupStack::PopAndDestroy( sourceUriBuf );	    
    	}
    
    LOG( "CLocationManagerSession::CopyLocationDataByUriL end" );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::StartTrackLogL
// --------------------------------------------------------------------------
//
TItemId CLocationManagerServer::StartTrackLogL()
	{
	LOG("CLocationManagerServer::StartTrackLogL");
	if ( iTrackLog->IsRecording() )
		{
		User::Leave( KErrInUse );
		}
	
	iTagId = CreateTrackLogTagL();
	iTrackLog->StartRecordingL( iTagId );
	
	StartListeningObjectCreationsL();	
	StartListeningTagRemovalsL();
	
	CompleteNotifyRequest( EStarted, KErrNone );
	
	return iTagId;
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::StopTrackLogL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::StopTrackLogL()
	{
	LOG("CLocationManagerServer::StopTrackLogL");
	if ( iTrackLog->IsRecording() )
		{
		iTrackLog->StopRecordingL();
		
		CompleteNotifyRequest( EStopped, KErrNone );
		
		// stop observers
		TRAP_IGNORE( iMdeSession->RemoveObjectObserverL( *this, &iMdeSession->GetDefaultNamespaceDefL()) );
		TRAP_IGNORE( iMdeSession->RemoveObjectObserverL( *this, &iMdeSession->GetDefaultNamespaceDefL()) );
		}
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CompleteNotifyRequest
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CompleteNotifyRequest( TEventTypes aEventType, TInt aError )
	{
	LOG("CLocationManagerServer::CompleteNotifyRequest");
	const TInt KEventTypeParam = 2;
	TPckg<TEventTypes> wrapEventType( aEventType );
	
	const TInt count = iTrackLogNotifyReqs.Count();
	for ( TInt i( count ); --i >= 0; )
		{
		RMessage2& msg = iTrackLogNotifyReqs[i];
		
		if( !msg.IsNull() )
			{
			msg.Write( KEventTypeParam, wrapEventType );
			msg.Complete( aError );
			}
		}
	iTrackLogNotifyReqs.Reset();
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::IsTrackLogRecording
// --------------------------------------------------------------------------
//
void CLocationManagerServer::IsTrackLogRecording( TBool &aRec )
	{
	aRec = iTrackLog->IsRecording();
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::GpxFileCreated
// --------------------------------------------------------------------------
//
void CLocationManagerServer::GpxFileCreated( const TDesC& aFileName, TItemId aTagId,
		TReal32 aLength, TTime aStart, TTime aEnd )
	{
	TRAP_IGNORE( CreateTrackLogL( aTagId, aFileName, aLength, aStart, aEnd ) );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CreateTrackLogTagL
// --------------------------------------------------------------------------
//
TItemId CLocationManagerServer::CreateTrackLogTagL()
	{
	LOG("CLocationManagerServer::CreateTrackLogTagL");
	if ( !IsSessionReady() )
		{
		User::Leave( KErrNotReady );
		}
	
	CMdEObjectDef& trackLogTagDef = iMdeSession->GetDefaultNamespaceDefL()
		.GetObjectDefL( Tag::KTagObject );
	
	CMdEObject* trackLogTag = iMdeSession->NewObjectLC( trackLogTagDef, KNullDesC );
	
	// Mandatory parameters for any object.
	CMdEPropertyDef& creationDef = trackLogTagDef.GetPropertyDefL( Object::KCreationDateProperty );
	CMdEPropertyDef& modifiedDef = trackLogTagDef.GetPropertyDefL( Object::KLastModifiedDateProperty );
	CMdEPropertyDef& sizeDef = trackLogTagDef.GetPropertyDefL( Object::KSizeProperty );
	CMdEPropertyDef& itemTypeDef = trackLogTagDef.GetPropertyDefL( Object::KItemTypeProperty );
	
	TTime timestamp( 0 );
	timestamp.UniversalTime();

	// required object properties
	trackLogTag->AddTimePropertyL( creationDef, timestamp );
	trackLogTag->AddTimePropertyL( modifiedDef, timestamp );
	trackLogTag->AddUint32PropertyL( sizeDef, 0 );
	trackLogTag->AddTextPropertyL( itemTypeDef, Tag::KTagItemType );
	
	TItemId tagId = iMdeSession->AddObjectL( *trackLogTag );
	
	CleanupStack::PopAndDestroy( trackLogTag );
	
	return tagId;
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::CreateTrackLogL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CreateTrackLogL( TItemId aTagId, const TDesC& aUri, TReal32 aLength,
		TTime aStart, TTime aEnd )
	{
	LOG("CLocationManagerServer::CreateTrackLogL");
	if ( !IsSessionReady() )
		{
		User::Leave( KErrNotReady );
		}

	CMdEObjectDef& trackLogDef = iMdeSession->GetDefaultNamespaceDefL().GetObjectDefL( 
			TrackLog::KTrackLogObject );

	CMdEObject* trackLog = iMdeSession->NewObjectLC( trackLogDef, aUri );

	// Mandatory parameters for any object.
	CMdEPropertyDef& creationDef = trackLogDef.GetPropertyDefL( Object::KCreationDateProperty );
	CMdEPropertyDef& modifiedDef = trackLogDef.GetPropertyDefL( Object::KLastModifiedDateProperty );
	CMdEPropertyDef& sizeDef = trackLogDef.GetPropertyDefL( Object::KSizeProperty );
	CMdEPropertyDef& itemTypeDef = trackLogDef.GetPropertyDefL( Object::KItemTypeProperty );
	
	// Tracklog specific properties.
	CMdEPropertyDef& lengthDef = trackLogDef.GetPropertyDefL( TrackLog::KLengthProperty );
	CMdEPropertyDef& startTimeDef = trackLogDef.GetPropertyDefL( TrackLog::KStartTimeProperty );
	CMdEPropertyDef& stopTimeDef = trackLogDef.GetPropertyDefL( TrackLog::KStopTimeProperty );

	TTime timestamp( 0 );
	timestamp.UniversalTime();

	trackLog->AddTimePropertyL( creationDef, timestamp );
	trackLog->AddTimePropertyL( modifiedDef, timestamp );
	trackLog->AddUint32PropertyL( sizeDef, 0 );
	trackLog->AddTextPropertyL( itemTypeDef, TrackLog::KTrackLogItemType );
	trackLog->AddUint32PropertyL( lengthDef, TUint32( aLength ));
	trackLog->AddTimePropertyL( startTimeDef, aStart );
	trackLog->AddTimePropertyL( stopTimeDef, aEnd );
	
	TItemId trackLogId = iMdeSession->AddObjectL( *trackLog );
	
	CMdERelationDef& containsRelDef = iMdeSession->GetDefaultNamespaceDefL().GetRelationDefL( 
			Relations::KContains );
    
    CMdERelation* relationObject = iMdeSession->NewRelationLC( containsRelDef, aTagId,
    		trackLogId, 0 );
    
    iMdeSession->AddRelationL( *relationObject );
    
    CleanupStack::PopAndDestroy( relationObject );
    CleanupStack::PopAndDestroy( trackLog );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::GetTrackLogStatus
// --------------------------------------------------------------------------
//
TInt CLocationManagerServer::GetTrackLogStatus( TBool& aRecording, TPositionSatelliteInfo& aFixQuality)
	{
	if ( !iTrackLog )
		{
		return KErrNotFound;
		}
	
	iTrackLog->GetStatus( aRecording, aFixQuality );
	
	return KErrNone;
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::DeleteTrackLogL
// --------------------------------------------------------------------------
//
TInt CLocationManagerServer::DeleteTrackLogL( const TDesC& aUri )
	{
    LOG( "CLocationManagerServer::DeleteTrackLogL enter" );
    
    // remove tracklog mde object 
    CMdEObject* mdeObject = iMdeSession->GetObjectL( aUri );
	if ( mdeObject )
    	{
    	TItemId objectId = mdeObject->Id();
	    delete mdeObject;
	    mdeObject = NULL;
	    
		TTime time( 0 );
		CMdENamespaceDef& nsDef = iMdeSession->GetDefaultNamespaceDefL();
		CMdEEventDef& eventDef = nsDef.GetEventDefL( MdeConstants::Events::KDeleted );

		iMdeSession->RemoveObjectL( aUri, &nsDef );
		time.UniversalTime();
		CMdEEvent* event = iMdeSession->NewEventL( eventDef, objectId, time,NULL,NULL );
		CleanupStack::PushL( event );
		
		iMdeSession->AddEventL( *event );
		CleanupStack::PopAndDestroy( event );
    	}
	
	// remove file from filesystem
	RFs fs;
	TInt err;
	err = fs.Connect();
	if ( err == KErrNone )
		{	
		err = fs.Delete( aUri );
		fs.Close();
		}
	
    LOG( "CLocationManagerServer::DeleteTrackLogL return" );
	
    return err;
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::TrackLogName
// --------------------------------------------------------------------------
//
TInt CLocationManagerServer::TrackLogName( TFileName& aFileName )
	{
	if ( iTrackLog->IsRecording() )
		{
		iTrackLog->GetTrackLogName(aFileName);
		return KErrNone;
		}
	return KErrNotFound;
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::GetCaptureSetting
// --------------------------------------------------------------------------
//
void CLocationManagerServer::GetCaptureSetting( RLocationTrail::TTrailCaptureSetting& aCaptureSetting )
	{
	aCaptureSetting = iCaptureSetting;
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::HandleObjectNotification
// --------------------------------------------------------------------------
//
void CLocationManagerServer::HandleObjectNotification( CMdESession& /*aSession*/,
		TObserverNotificationType aType,
		const RArray<TItemId>& aObjectIdArray )
	{
	// If notification type is remove then someone has deleted a tracklog tag.
	if ( aType == ENotifyRemove )
		{	
		iTrackLog->CancelRecording();
		return;
		}
	
	TRAP_IGNORE( LinkObjectToTrackLogTagL( aObjectIdArray ) );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::StartListeningTagRemovalsL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::StartListeningTagRemovalsL()
	{
	if ( !IsSessionReady() )
		{
		User::Leave( KErrNotReady );
		}
	
	// start listening to mde track log tag removals
    CMdELogicCondition* condition = CMdELogicCondition::NewL( ELogicConditionOperatorAnd );
    CleanupStack::PushL( condition );
    iMdeSession->AddObjectObserverL( *this, condition, ENotifyRemove, 
    		&iMdeSession->GetDefaultNamespaceDefL() );
    CleanupStack::Pop( condition );
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::StartListeningObjectCreationsL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::StartListeningObjectCreationsL()
	{
	if ( !IsSessionReady() )
		{
		User::Leave( KErrNotReady );
		}
	
	CMdELogicCondition* condition = CMdELogicCondition::NewL( ELogicConditionOperatorAnd );
	CleanupStack::PushL( condition );
	
	CMdEObjectDef& objDef = iMdeSession->GetDefaultNamespaceDefL().GetObjectDefL( 
			MediaObject::KMediaObject );

	CMdEPropertyDef& originDef = objDef.GetPropertyDefL( Object::KOriginProperty );
	condition->AddPropertyConditionL( originDef, TMdEUintEqual( Object::ECamera ));
	
	CleanupStack::Pop( condition );
	iMdeSession->AddObjectObserverL( *this, condition, ENotifyAdd | ENotifyModify,
			&iMdeSession->GetDefaultNamespaceDefL() );
	
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::LinkObjectToTrackLogTagL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::LinkObjectToTrackLogTagL( const RArray<TItemId>& aObjectIdArray )
	{
	CMdERelationDef& containsRelDef = iMdeSession->GetDefaultNamespaceDefL()
		.GetRelationDefL( Relations::KContains );

	const TInt count = aObjectIdArray.Count();
	for ( TInt i( 0 ); i < count; i++ )
		{
		CMdERelation* relationObject = iMdeSession->NewRelationLC( containsRelDef, 
				aObjectIdArray[i], iTagId, 0 );

		iMdeSession->AddRelationL( *relationObject );

		CleanupStack::PopAndDestroy( relationObject );
		}
	}


// --------------------------------------------------------------------------
// CLocationManagerServer::AddGpxObserver
// --------------------------------------------------------------------------
//
void CLocationManagerServer::AddGpxObserver( MGpxConversionObserver* aObserver )
	{
	iTrackLog->AddGpxObserver( aObserver );
	}

// --------------------------------------------------------------------------
// CLocationManagerServer::RemapedCompleted()
// --------------------------------------------------------------------------
//
void CLocationManagerServer::RemapedCompleted()
    {
    LOG( "CLocationManagerServer::RemapedCompleted()" );
    StopRecording();
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::WaitForPositioningStopTimeout()
// --------------------------------------------------------------------------
//
TBool CLocationManagerServer::WaitForPositioningStopTimeout()
    {
    LOG( "CLocationManagerServer::WaitForPositioningStopTimeout()" );
    return iWaitForPositioningStopTimeout;
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::GeoTaggingCompleted
// --------------------------------------------------------------------------
//

void CLocationManagerServer::GeoTaggingCompleted(  const TInt aError  )
    {    
    LOG("CLocationManagerServer::GeoTaggingCompleted ,begin");
    ARG_USED(aError);
    // do nothing  because we are only handling pending request for this object.    
#ifdef LOC_REVERSEGEOCODE
    if(!iGeoTaggingMessage.IsNull())
    	{
		LOG("Completing the request");
		iGeoTaggingMessage.Complete(aError);
		iGeoTaggingMessage = RMessage2 ();
    	}
#endif //LOC_REVERSEGEOCODE
    StopServer();
	LOG("CLocationManagerServer::GeoTaggingCompleted ,end");
    }


// --------------------------------------------------------------------------
// CLocationManagerServer::StopServer
// --------------------------------------------------------------------------
//

void CLocationManagerServer::StopServer()
    {    
    LOG("CLocationManagerServer::StopServer ,begin");
    // once geo tagging completed, check whether, we can terminate the server
    // dont't stop this process if
    // 1. when client are connected.
    // 2. 3AM timer is going on.
    // 3. Tagging is in progress.
    if ( !iSessionCount 
#if defined(LOC_REVERSEGEOCODE) || defined(LOC_GEOTAGGING_CELLID)
        && iGeoTagTimer == NULL
#endif        
        && iLocationRecord 
        && !iLocationRecord->TaggingInProgress())
        {
        // Nothing in progress. shutdown the server
        LOG("Stop the schedular");
        CActiveScheduler::Stop();
        }
	LOG("CLocationManagerServer::StopServer ,end");
    }

// --------------------------------------------------------------------------
// CLocationManagerServer::PendingGeoTagReqComplete
// --------------------------------------------------------------------------
//
void CLocationManagerServer::PendingGeoTagReqComplete(  const TInt aError  )
    {
    LOG("CLocationManagerServer::PendingGeoTagReqComplete ,begin");
    ARG_USED(aError);
#ifdef LOC_REVERSEGEOCODE
    if(!iTagPendingMessage.IsNull())
    	{
       
	    TGeoTaggingSatus pendingGeoTagEntry = EGeoTagCmpt;
        switch(aError)
            {
            case KErrNotFound:
                pendingGeoTagEntry = EGeoTaggingPending;
                LOG("Geo tagging pending");
                break;
            case KErrInUse:
                pendingGeoTagEntry = EGeoTaggingGoingOn;
                LOG("Geo tagging going on");
                break;
            default:
                break;
            }
                    
	    TPckg<TGeoTaggingSatus> pendingGeoTagEntryPkg( pendingGeoTagEntry );    
	    TRAPD(err, iTagPendingMessage.WriteL( 0, pendingGeoTagEntryPkg ));
	    iTagPendingMessage.Complete((err == KErrNone) ? KErrNone : err);
	    iTagPendingMessage = RMessage2 ();
    	}
#endif //LOC_REVERSEGEOCODE
	LOG("CLocationManagerServer::PendingGeoTagReqComplete ,end");
    }


// --------------------------------------------------------------------------
// CLocationManagerServer::TagPendingRequestL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::TagPendingRequestL( const RMessage2& aMessage )
    {    
    LOG("CLocationManagerServer::TagPendingRequestL ,begin");
    // Only one request at a time
#ifdef LOC_REVERSEGEOCODE
    if(iTagPendingMessage.IsNull() && iGeoTaggingMessage.IsNull())
        {
        iTagPendingMessage = RMessage2( aMessage );
        // search for pending entry.
        //Create the instance of geotagger class
        TBool tagProgress = iLocationRecord->TaggingInProgress();
        if(tagProgress)
            {
            LOG("Tagging is going on.\n");
            // Let UI to send start geo tagging command.
            PendingGeoTagReqComplete(KErrNotFound);
            }
        else
            {
            LOG("Tagging is not going on.\n");
            if(iGeoTaggingPendingReqObj == NULL)
                {
                iGeoTaggingPendingReqObj = CGeoTagger::NewL( this, NULL );
                }
            iGeoTaggingPendingReqObj->PendingGeoTagsL( tagProgress);
    		}
        }
    else
        {
		LOG("Server busy\n");
        aMessage.Complete(KErrServerBusy);
        }
#else
    aMessage.Complete(KErrNotSupported);
#endif //LOC_REVERSEGEOCODE
	LOG("CLocationManagerServer::TagPendingRequestL ,end");
    }



// --------------------------------------------------------------------------
// CLocationManagerServer::CancelTagPendingRequest
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CancelTagPendingRequest( const RMessage2& aMessage )
    {    
    LOG("CLocationManagerServer::CancelTagPendingRequest ,begin");
    // Only one request at a time
#ifdef LOC_REVERSEGEOCODE
    if(!iTagPendingMessage.IsNull())
        {
        iTagPendingMessage.Complete(KErrCancel);
        iLocationRecord->CancelGeoTagging();
        }
    aMessage.Complete(KErrNone);
#else
    aMessage.Complete(KErrNotSupported);
#endif //LOC_REVERSEGEOCODE
    }


// --------------------------------------------------------------------------
// CLocationManagerServer::CancelGeoTaggingRequest
// --------------------------------------------------------------------------
//
void CLocationManagerServer::CancelGeoTaggingRequest( const RMessage2& aMessage )
    {    
    LOG("CLocationManagerServer::CancelGeoTaggingRequest ,begin");
    // Only one request at a time
#ifdef LOC_REVERSEGEOCODE
    if(!iGeoTaggingMessage.IsNull())
        {
        iGeoTaggingMessage.Complete(KErrCancel);
        iGeoTaggingMessage = RMessage2 ();
        iLocationRecord->CancelGeoTagging();
        }
    aMessage.Complete(KErrNone);
#else
    aMessage.Complete(KErrNotSupported);
#endif //LOC_REVERSEGEOCODE
    }


// --------------------------------------------------------------------------
// CLocationManagerServer::StartGeoTaggingL
// --------------------------------------------------------------------------
//
void CLocationManagerServer::StartGeoTaggingL( const RMessage2& aMessage )
    {
    LOG("CLocationManagerServer::StartGeoTaggingL ,begin");
#ifdef LOC_REVERSEGEOCODE
    if(iGeoTaggingMessage.IsNull() && iTagPendingMessage.IsNull())
        {
        iGeoTaggingMessage = RMessage2( aMessage );
        // search for pending entry.
        //Create the instance of geotagger class
        if(!iLocationRecord->StartGeoTagging(EInteractive))
            {
            if(iGeoTaggingPendingReqObj == NULL)
                {
                iGeoTaggingPendingReqObj = CGeoTagger::NewL( this, NULL );
                }
    		iGeoTaggingPendingReqObj->CreateGeoTagsL((TConnectionOption)(aMessage.Int0()));
    		}
        }
    else
        {
		LOG("Server busy\n");
        aMessage.Complete(KErrServerBusy);
        }
#else
    aMessage.Complete(KErrNotSupported);
#endif //LOC_REVERSEGEOCODE
    }


// ----------------------------------------------------------------------------
// CLocationManagerServer::GetCurrentRegisterNw()
// ---------------------------------------------------------------------------- 
RMobilePhone::TMobilePhoneNetworkInfoV2& CLocationManagerServer::GetCurrentRegisterNw()
    {
    LOG( "CLocationManagerServer::GetCurrentRegisterNw ,begin" );
    return iLocationRecord->GetCurrentRegisteredNw();
    }

// ----------------------------------------------------------------------------
// CLocationManagerServer::RetrieveHomeNetwork()
// ----------------------------------------------------------------------------
void CLocationManagerServer::RetrieveHomeNetwork()
    {
    LOG("CLocationManagerServer::RetrieveHomeNetwork ,begin");
    if(iHomeNwInfoAvailableFlag)
        {
        RMobilePhone::TMobilePhoneNetworkInfoV1Pckg homeNetworkPckg( iHomeNetwork );
        
        TRequestStatus status( KErrNone );
        
        iPhone.GetHomeNetwork(status, homeNetworkPckg);
        User::WaitForRequest( status );
        if(status.Int() == KErrNone)
            {
            iHomeNwInfoAvailableFlag = ETrue;
            }
        }
	LOG("CLocationManagerServer::RetrieveHomeNetwork ,end");
    }

// ----------------------------------------------------------------------------
// CLocationManagerServer::GetHomeNetworkInfo()
// ----------------------------------------------------------------------------
const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        CLocationManagerServer::GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag)
    {
    LOG("CLocationManagerServer::RetrieveHomeNetwork ,begin");
    if(!iHomeNwInfoAvailableFlag)
        {
        RetrieveHomeNetwork();
        }
    aHomeNwInfoAvailableFlag = iHomeNwInfoAvailableFlag;
    return iHomeNetwork;
    }


// ----------------------------------------------------------------------------
// CLocationManagerServer::IsRegisteredAtHomeNetwork()
// ---------------------------------------------------------------------------- 
TBool CLocationManagerServer::IsRegisteredAtHomeNetwork()
    {
    LOG( "CLocationManagerServer::IsRegisteredAtHomeNetwork" );
    return (iNwRegistrationStatusHandler &&
            (iNwRegistrationStatusHandler->GetNetworkRegistrationStatus() 
                == RMobilePhone::ERegisteredOnHomeNetwork));
    }

// End of file 
