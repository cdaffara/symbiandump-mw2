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
* Description:  Header file for Location Manager Server.
*
*/

#ifndef C_CLOCATIONMANAGERSERVER_H
#define C_CLOCATIONMANAGERSERVER_H


#include <e32base.h>
#include <etel3rdparty.h>
#include <centralrepository.h>
#include <locationdatatype.h>
#include <locationeventdef.h>
#ifdef LOC_REVERSEGEOCODE
#include <geotagger.h>
#endif //LOC_REVERSEGEOCODE

#include "rlocationtrail.h"
#include "rlocationobjectmanipulator.h"
#include "clocationrecord.h"
#include "ctracklog.h"
#include "mdeconstants.h"
#include "mdsutils.h"
#include "mdesession.h"
#include "mdequery.h"
#include "mderelationquery.h"
#include "locationmanagerdefs.h"
#include "clocationservertimerhandler.h"

class CMdESession;
class CLocationGeoTagTimerAO;
class CGeoTagger;
class REComSession;
class CNwRegistrationStatusHandler;

// Total number of ranges
const TUint KLocationManagerRangeCount = 1;

// Definition of the ranges of IPC numbers
const TInt KLocationManagerRanges[KLocationManagerRangeCount] = 
    {
    0,
    }; 

// Policy to implement for each of the above ranges        
const TUint8 KLocationManagerElementsIndex[KLocationManagerRangeCount] = 
    {
    0,
    };

const CPolicyServer::TPolicyElement KLocationManagerPolicyElements[] = 
    {
    { _INIT_SECURITY_POLICY_C3(ECapabilityReadUserData,
                               ECapabilityWriteUserData,
                               ECapabilityLocation),
                               CPolicyServer::EFailClient }
    };

// Package all the above together into a policy
const CPolicyServer::TPolicy KLocationManagerPolicy =
    {
    CPolicyServer::EAlwaysPass,     // all attempts should pass
    KLocationManagerRangeCount,     // number of ranges
    KLocationManagerRanges,         // ranges array
    KLocationManagerElementsIndex,  // elements<->ranges index
    KLocationManagerPolicyElements, // array of elements
    };
    
class CLocationRecord;
//class CLocationWrite;

/**
*  A server class to initialize server. 
*
*  @since S60 3.1
*/
class CLocationManagerServer : public CPolicyServer,
                               public MLocationTrailObserver,
                               public MMdESessionObserver,
                               public MMdEQueryObserver,
                               public MMdEObjectObserver,
                               public MGpxConversionObserver,
                               public MGeoTaggerObserver,
                               public MLocationServerTimerObserver
    {
private:
	struct TMessageQuery
		{
		CMdERelationQuery* iQuery;
		const RMessage2& iMessage;
		
		TMessageQuery( CMdERelationQuery* aQuery, const RMessage2& aMessage ) 
			:  iQuery ( aQuery ), iMessage( aMessage ) { }
		};
    
public:
    /**
    * 2-phased constructor.
    */
    static CLocationManagerServer* NewLC();
    
    /**
    * C++ destructor.
    */ 
    virtual ~CLocationManagerServer();
  
public:
    /**
    * From CServer2, creates a new session.
    */
    CSession2* NewSessionL( const TVersion& aVersion, 
                            const RMessage2& aMessage ) const;

public: // From MMdESessionObserver
	void HandleSessionOpened(CMdESession& aSession, TInt aError);
    void HandleSessionError(CMdESession& aSession, TInt aError);
    
public: // From MMdEQueryObserver
	void HandleQueryNewResults(CMdEQuery& aQuery, TInt aFirstNewItemIndex, TInt aNewItemCount);
	void HandleQueryCompleted(CMdEQuery& aQuery, TInt aError);

public:
    /**
     * Increase session count.
     * @since S60 3.1
     * @param None.
     * @return None.
     */
    void AddSession();
    
    /**
     * Decrease session count, close server if count is zero.
     * @since S60 3.1
     * @param None.
     * @return None.
     */
    void RemoveSession();

public:
    /**
     * Starts to record locations to the trail.
     * @since S60 3.1
     * @param None.
     * @return None.
     */     
    void StartGPSPositioningL( RLocationTrail::TTrailCaptureSetting aCaptureSetting );
    
    /**
     * Stops trail recording.
     * @since S60 3.1
     * @param None.
     * @return None.
     */     
    void StopGPSPositioningL();
    
    /**
     * Gets the current state of the location trail.
     * @since S60 3.1
     * @param aState, The state of the trail is written to this param.
     * @return None.
     */     
    void GetLocationTrailState( RLocationTrail::TTrailState& aState );
    
    /**
     * Adds client's notification request to the request array.
     * @since S60 3.1
     * @param aNotifReq, asynchonous RMessage is stored to be completed
     *                   when the state of the trail changes.
     * @return None.
     */     
    void AddNotificationRequestL( const RMessage2& aNotifReq );
    
    /**
     * Add client's track log notification request to the request array.
     * @param aNotifReq, asynchonous RMessage is stored to be completed
     *                   when the state of the track log changes.
     * @return None.
     */
    void AddTrackLogNotificationRequestL( const RMessage2& aNotifReq );
    
    /**
     * Cancel notification request from client.
     * @since S60 3.1
     * @param aHandle, A handle of the request to be cancelled.
     * @return None.
     */     
    void CancelNotificationRequest( const TInt aHandle );
    
    /**
     * Get a location by time stamp.
     * @since S60 3.1
     * @param aTimeStamp, A time stamp to get corresponding location.
     * @param aPosition, Location info is written to this param.
     * @return None.
     */     
    void GetLocationByTimeL( const TTime& aTimeStamp, 
    						 TLocationData& aLocationData,
                             /*TLocality& aPosition, 
                             CTelephony::TNetworkInfoV1& aNetworkInfo,*/
                             TLocTrailState& aState );

    /**
     * Get current location.
     * @since S60 3.1
     * @param aCurrLocReq 
     * @return None.
     */     
    void RequestCurrentLocationL( const RMessage2& aCurrLocReq ); 

    /**
     * Cancel location request.
     * @since S60 3.1
     * @param aCurrLocReq 
     * @return None.
     */     
    void CancelLocationRequest( const TInt aHandle ); 
    
    /**
     * Cancel tracklog notification request.
     * @param aHandle
     * @return None.
     */
    void CancelTrackLogNotificationRequest( const TInt aHandle );

    /**
     * Get current network cell id.
     * @since S60 3.1
     * @param aCurrLocReq 
     * @return None.
     */     
    void GetCurrentNetworkInfo( CTelephony::TNetworkInfoV1& aNetworkInfo );
    
    /**
     * Create a location context object in DB and create relationships to objects
     * whose id is given in the array.
     * @param aLocationData
     * @param aObjectId
     */
	void CreateLocationObjectL( const TLocationData& aLocationData, 
    						   	   const TUint& aObjectId );
	
    /**
     * Create a location context object in DB and create relationships to objects
     * whose id is given in the array.
     * Location information is taken from location trail
     * @param aObjectId
     */
	void LocationSnapshotL( const TUint& aObjectId );
	
	/**
	 * Deletes the relationship between an object and 
	 * the location context object associated with it.
	 * @since S60 3.2
	 * @param aObjectId
	 * @return None.
	 */ 
	void RemoveLocationObjectL(TUint& aObjectId);

	/**
	  * Copies the location object
	  * @param aSource source location object
	  *	@param aTargets target location object
	  * @param aQuery message query type

	  */	
	void CopyLocationObjectL( TItemId aSource, const RArray<TItemId>& aTargets, TMessageQuery& aQuery );

	/**
	  * Copies the location object
	  * @param aSource The source location object
	  *	@param aTargets target location object
	  * @param aQuery message query type
	  */
	void CopyLocationObjectL( const TDesC& aSource, const RArray<TPtrC>& aTargets, TMessageQuery& aQuery );

	/**
	  * checks if the session is ready
	  * @return ETrue if Session is ready
	  */	
	TBool IsSessionReady();

	/**
	  * Start track log
	  *
	  * @return The itemId
	  */	
	TItemId StartTrackLogL();

	/** 
	  * Stop track log
	  *
	  */	
	void StopTrackLogL();

	/**
	  * IsTrackLogRecording
	  *
	  * @param aRec
	  */	
	void IsTrackLogRecording( TBool &aRec );
	
	/**
	  * Retrieve track log status
	  *
	  * @param aRecording 
	  * @param aFixQuality
	  */
	TInt GetTrackLogStatus( TBool& aRecording, TPositionSatelliteInfo& aFixQuality);

	/**
	  * Delete track log
	  *
	  * @param aUri 
	  */	
	TInt DeleteTrackLogL(const TDesC& aUri);

	/**
	  * Track log name
	  *
	  * @param aFileName name of file used to track log
	  */	
	TInt TrackLogName(TFileName& aFileName);
	
	/**
	  * Gets the location trail capture settings
	  * @param aCaptureSetting The location trail capture settings
	  */	
	void GetCaptureSetting( RLocationTrail::TTrailCaptureSetting& aCaptureSetting );

	/**
	  * Adds an observer  for Gpx conversion
	  * @param aObserver the observer for Gpx conversion
	  */
	void AddGpxObserver( MGpxConversionObserver* aObserver );
	
	/**
	  * Copy location by Id
	  * @param aMessage
	  */	
	void InitCopyLocationByIdL( const RMessage2& aMessage );
	
	/**
	  * Copy Location by Uri
	  * @param aMessage
	  */
	void InitCopyLocationByURIL( const RMessage2& aMessage );

	/**
	  * Handles tag pending request
	  *
	  * @param aMessage IPC message
	  */	
    void TagPendingRequestL( const RMessage2& aMessage );

	/**
	  * Cancels the tag pending request
	  * @param aMessage IPC message
	  */	
    void CancelTagPendingRequest( const RMessage2& aMessage );

	/**
	  * Cancels the geotagging request
	  * @param aMessage IPC message
	  */
	void CancelGeoTaggingRequest( const RMessage2& aMessage );


	/**
	  * Handle starts geotagging request
	  * @param aMessage IPC message
	  */
    void StartGeoTaggingL( const RMessage2& aMessage );
    
public: // from MLocationTrailObserver.
    /**
     * Callback method to get notification about trail state change.
     * @since S60 3.1
     * @param None.
     * @return None.
     */
    void LocationTrailStateChange();
    
    /**
     * Callback method to return current location.
     * @since S60 3.1
     * @param aSatelliteInfo, includes position and satellite info.
     * @param aNetworkInfo, network and cell info.
     * @param aError.
     * @return None.
     */
    void CurrentLocation( const TPositionSatelliteInfo& aSatelliteInfo,
    		const CTelephony::TNetworkInfoV1& aNetworkInfo, const TInt aError );
    
    /**
     * Callback method to notify observer of changes in GPS signal quality.
     * @param aSatelliteInfo, includes position and satellite info
     * @return None.
     */
    void GPSSignalQualityChanged( const TPositionSatelliteInfo& aSatelliteInfo );
    
    /**
     * Callback method to notify observer that during waiting for positioning stop timeout remap is done.
     */
    void RemapedCompleted();
    
    /**
     * Returns if in ETrialStopping state server waits for positioning stop timeout
     * @returns <code>ETrue</code> if server is waiting for positioning stop timeout
     *          <code>EFalse</code>, otherwise.
     */
    TBool WaitForPositioningStopTimeout();
    
public: // from MMdeObjectObserver
	/**
	 * Called to notify the observer that new objects has been
	 * added/modified/removed in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aType defines if object was added/modified/remove
	 * @param aObjectIdArray IDs of added object
	 */
	void HandleObjectNotification( CMdESession& aSession, 
						TObserverNotificationType aType,
						const RArray<TItemId>& aObjectIdArray );
	
public: // from MGpxConversionObserver

	void GpxFileCreated( const TDesC& aFileName, TItemId aTagId, TReal32 aLength,
			TTime aStart, TTime aEnd );

protected:  //From MGeoTaggerObserver

	/**
	  * This method is called on completion of geotagging
	  * and also completes start geotagging IPC message
	  * @param aError The err code for geotagging if any
	  */
     void GeoTaggingCompleted( const TInt aError );   
     /**
      * This method is used for notifying completion of query for pending geo tags
	  * @param aError The err code for geotagging if any           
      */
     void PendingGeoTagReqComplete( const TInt aError );

    /*
    * Get registrer network country code
    *
    * @return current register n/w info
    */
    RMobilePhone::TMobilePhoneNetworkInfoV2& GetCurrentRegisterNw();


    /*
    * Get home network country code
    * @param aHomeNwInfoAvailableFlag ETrue if home n/w info available else EFalse
    * @return user home n/w info
    */
    const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag);

    
    /*
    * UE is registered to home network?
    *
    * @return ETrue if UE is registered at home network else EFalse
    */
    TBool IsRegisteredAtHomeNetwork();

public: //MLocationServerTimerObserver
    /**
        * Timer call back
        * @param aLocationServerTimerType timer type
        * @param aErrorCode error code
        */
    void LocationServerTimerCallBackL
        (const TLocationServerTimerType aLocationServerTimerType, const TInt aErrorCode);
private:    
    /**
    * C++ constructor.
    */
    CLocationManagerServer();
    
    /**
    * 2nd phase constructor.
    */
    void ConstructL();

	/**
	  * Copies the location
	  * @param aQuery The query
	  */	
    void CopyLocationL( CMdEQuery& aQuery );

    /**
     * Go through all messages in lists, complete request
     * with KErrCancel status and remove items from list.
     */ 
    void CancelRequests(RArray<RMessage2>& aMessagesList);
    
    /**
	  * cancels the list of copy requests
	  * @param aMessageList  the list of query 
	  */
    void CancelCopyRequests(RArray<TMessageQuery>& aMessageList);
    
    /**
     * Create a new tracklog tag object in the database.
     * @return TItemId tag ID in the database
     */ 
    TItemId CreateTrackLogTagL();
    
    /**
     * Create a new tracklog object in the database and link tracklog tag to it.
     * @param aTagId, tracklog tag id
     */ 
    void CreateTrackLogL( TItemId aTagId, const TDesC& aUri, TReal32 aLength,
    		TTime aStart, TTime aEnd );
    
    /**
     * Start listening for tracklog tag removals.
     */
    void StartListeningTagRemovalsL();
    
    /**
     * Start listening for media object creations.
     */
    void StartListeningObjectCreationsL();
    
    /**
     * Create a relationship between mediaobject(s) and a tracklog tag.
     * @param aObjectIdArray, array of media object ids
     */
    void LinkObjectToTrackLogTagL( const RArray<TItemId>& aObjectIdArray );
    
    /**
     * Callback function for positioning stop timer.
     * @param aAny, a pointer to CLocationRecord object
     * @return Error code
     */
   void PositioningStopTimeout();
    
    /**
     * Callback function to check if files were added to remapping queue with delay
     * @param aAny, a pointer to CLocationRecord object
     * @return Error code
     */
    void CheckForRemappingCallback();

    
    /**
     * Stops location trail and deletes the positioning stop timer.
     */
    void StopRecording();

	/**
	  * Complete notify request
	  *
	  * @param aEventType, returns the event type
	  * @param aError, return error type for this call.
	  */	
    void CompleteNotifyRequest( TEventTypes aEventType, TInt aError );

    /**
      * Get supported TSY name
      *@param aTsyName - buffer to store the supported tsy name
      */  
    void GetCommDbTSYnameL(TDes& aTsyName);

    /**
      * Initialize etel to access modem parameters
      */  
    void InitialisePhoneL();

    /**
      * Retrieve home network
      */  
    void RetrieveHomeNetwork();
    
    /**
      * Stop the server (if possible)
      */  
    void StopServer();

private:
    /**
     * A class for recording and storing locations.
     * Own.
     */
    CLocationRecord* iLocationRecord;
    
    /**
     * Pointer to TrackLog
     * Own.
     */
    CTrackLog* iTrackLog;
    
    /**
     * An active scheduler wait loop for waiting a session to MdE to open.
     */
    CActiveSchedulerWait* iASW;
    
    /**
     * An array for asynchronous notification requests.
     * Own.
     */
    RArray<RMessage2> iNotifReqs;
    
    /**
     * An array for asynchronous location requests.
     * Own.
     */
    RArray<RMessage2> iLocationReqs;
    
    /**
     * An array for track log notification requests.
     * Own.
     */
    RArray<RMessage2> iTrackLogNotifyReqs;
    
    /**
     * An array for location copy requests.
     * Own.
     */
    RArray<TMessageQuery> iCopyReqs;
    
    /**
     * A session to Metadata Engine for creating and manipulating location objects.
     */
    CMdESession* iMdeSession;
    
    /** A relation query used to seach for related location objects */
    CMdERelationQuery* iRelationQuery;
    
    /**
     * A timer to stop location trail.
     * Own.
     */
    CLocationServerTimerHandler* iTimer;
    
    TBool iClientSwitch;    
    TInt iSessionCount;
    TBool iSessionReady;  
    
    RArray<TItemId> iTargetObjectIds;
    TItemId iTagId;
    TInt iLocManStopDelay;
    TInt iLocManStopRemapDelay;
    
    RLocationTrail::TTrailCaptureSetting iCaptureSetting;
    TBool iRemoveLocation;    
    
    /**
     * A flag for state of waiting for position stop timeout.
     */
    TBool iWaitForPositioningStopTimeout;
	RTelServer		iTelServer;
	RMobilePhone	iPhone;	
	TBool			iTelServerIsOpen;
	TBool			iPhoneIsOpen;
    CNwRegistrationStatusHandler *iNwRegistrationStatusHandler;
    RMobilePhone::TMobilePhoneNetworkInfoV1 iHomeNetwork;
    TBool iHomeNwInfoAvailableFlag;
  
    CLocationGeoTagTimerAO* iGeoTagTimer;
    
#ifdef LOC_REVERSEGEOCODE
    CGeoTagger* iGeoTaggingPendingReqObj;
    RMessage2 iTagPendingMessage;
    RMessage2 iGeoTaggingMessage;
    REComSession* iEcomSession; 
#endif //LOC_REVERSEGEOCODE
    };


#endif // C_CLOCATIONMANAGERSERVER_H

// End of file.
