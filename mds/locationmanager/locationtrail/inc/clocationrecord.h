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

#ifndef C_CLOCATIONRECORD_H
#define C_CLOCATIONRECORD_H

#include <e32base.h>
#include <e32property.h>
#include <lbs.h>


#include <etel.h>
#include <etelmm.h>
#include <geotagobserver.h>

#include "rlocationtrail.h"
#include "locationdatatype.h"
#include "cnetworkinfo.h"
#include "cpositioninfo.h"
#include "rlocationobjectmanipulator.h"

#include "mdccommon.h"
#include "mdesession.h"
#include "mdenamespacedef.h"
#include "mdeobjectdef.h"
#include "mdepropertydef.h"
#include "mderelation.h"
#include "mdequery.h"
#include "locationremappingao.h"

#ifdef LOC_GEOTAGGING_CELLID	
#include "cgeoconverter.h"
#endif

#ifdef LOC_REVERSEGEOCODE
#include <geotagger.h>
#include "ctagcreator.h"
#include <reversegeocoderplugin.h>
#endif

#include <CProfileChangeNotifyHandler.h>
#include <MProfileChangeObserver.h>

typedef RLocationTrail::TTrailState TLocTrailState;

class CTelephony;
class TPositionSatelliteInfo;

/**
*  An observer interface, which is used for getting notification when the 
*  location trail's state changes.    
*
*  @since S60 3.1
*/
class MLocationTrailObserver
    {
public:    
    /**
     * This method is used to notify about location trail state changes.
     */
    virtual void LocationTrailStateChange() = 0;

	/**
     * This method is used to fetch the current location
     * @param aSatelliteInfo  The satellite information
     * @param aNetworkInfo  The network informatiom
     * @param aError ErrCode if any
     */    
    virtual void CurrentLocation( const TPositionSatelliteInfo& aSatelliteInfo, 
    							  const CTelephony::TNetworkInfoV1& aNetworkInfo,
                                  const TInt aError ) = 0;


	/**
     * This method is used to notify about GPS signal quality changes.
     * @param aSatelliteInfo  The satellite information
     */
    virtual void GPSSignalQualityChanged( const TPositionSatelliteInfo& aSatelliteInfo )  = 0;
    
    /**
     * Callback method to notify observer that during waiting for positioning stop timeout remap is done.
     */
    virtual void RemapedCompleted() = 0;
    
    /**
     * Returns if in ETrialStopping state server waits for positioning stop timeout
     * @returns <code>ETrue</code> if server is waiting for positioning stop timeout
     *          <code>EFalse</code>, otherwise.
     */
    virtual TBool WaitForPositioningStopTimeout() = 0;
    };

/**
* Location trail item class.
*/
class TLocationTrailItem
    {
    public:
    	TLocationData	iLocationData; // Location info & network info
        TTime           iTimeStamp;  // Time stamp.
        TLocTrailState  iTrailState; // Trail state for this item.
    };
    
/**
* Location snap shot item class.
*/
class TLocationSnapshotItem
    {
    public:
        TLocationData   iLocationData; // Location info & network info
        TItemId         iObjectId;     // current media object id.
        TItemId         iLocationId;
        TUint           iFlag;         //indicator for various scenarios
#ifdef LOC_REVERSEGEOCODE
        TItemId         iCountryTagId;
        TItemId         iCityTagId;
		TUint           iReverseGeocodeSuccess;
#endif		
    };
class MLocationAddObserver
	{
public:
	/**
	 * This method is used to notify about new locations added to location trail
	 */
	virtual void LocationAdded( const TLocationTrailItem& aTrailItem, 
								const TPositionSatelliteInfo& aSatellites ) = 0;
	};    

/**
 *  Location trail collects location information periodically and stores them
 *  to an array. Stored locations may be searched by time stamp to get 
 *  a location, which corresponds to certain time.  
 *
 *  @since S60 3.1
 */
class CLocationRecord : public CBase,
                        public MNetworkInfoObserver,
                        public MPositionInfoObserver,
                        public MMdEQueryObserver,
                        public MProfileChangeObserver 
#ifdef LOC_GEOTAGGING_CELLID	
                        ,public MGeoConverterObserver
#endif						
#ifdef LOC_REVERSEGEOCODE
                        ,public MReverseGeocodeObserver
#endif
    {
public:  
    /**
     * 2-phased constructor.
     * @param aGeoTaggerObserver The observer that is notified after geotagging
     * @since S60 3.1
     */
     IMPORT_C static CLocationRecord* NewL(MGeoTaggerObserver& aGeoTaggerObserver, RMobilePhone& aPhone);
 
    /**
     * C++ destructor.
     * @since S60 3.1
     */    
    IMPORT_C virtual ~CLocationRecord();

    
public:
    /**
     * Returns the current state of the location trail.
     * @since S60 3.1
     * @param aState, The current state is written to this variable.
     * @return None.
     */    
    IMPORT_C void LocationTrailState( TLocTrailState& aState );
    
    /**
     * Starts collecting locations from Location Acquisition API.
     * @since S60 3.1
     * @param None.
     * @return None.
     */    
    IMPORT_C void StartL( RLocationTrail::TTrailCaptureSetting aCaptureSetting );
    
    /**
     * Stops collecting locations.
     * @since S60 3.1
     * @param None.
     * @return None.
     */    
    IMPORT_C void Stop();    

    /**
     * Returns the location info, which is nearest to the given time.
     * @since S60 3.1
     * @param aTime, A time stamp to get corresponding location.
     * @param aPosition, Location info is written to this param.
     * @param 
     * @return None.
     */
    IMPORT_C void GetLocationByTimeL( const TTime aTime,
    								  TLocationData& aLocationData,
                                      /*TLocality& aPosition,
                                      CTelephony::TNetworkInfoV1& aNetworkInfo,*/
                                      TLocTrailState& aState );

    /**
     * Request location info. The result is returned by calllback method.
     * @since S60 3.1
     * @param None.
     * @return None.
     */
    IMPORT_C void RequestLocationL();

    /**
     * Cancel request for location info. 
     * @since S60 3.1
     * @param None.
     * @return None.
     */    
    IMPORT_C void CancelLocationRequest();
        
    /**
     * Get network cell id.
     * @since S60 3.1
     * @param aCellId, Network cell is written into this param.
     * @return None.
     */
    IMPORT_C void GetNetworkInfo( CTelephony::TNetworkInfoV1& aNetworkInfo );
    
    /**
     * Set observer for notifying state changes.
     * @since S60 3.1
     * @param aObserver, An interface to notify about state changes.
     * @return None.
     */
    IMPORT_C void SetObserver( MLocationTrailObserver* aObserver );
    
    /**
     * Set observer (TrackLog) for notifying new locations in location trail
     * @param aObserver, An observer for location changes
     */
    IMPORT_C void SetAddObserver( MLocationAddObserver* aObserver );    


	/**
	* Update network information
	*
	*/
    static TInt UpdateNetworkInfo( TAny* aAny );

	/*
	* creates a location object from the given location data and objectId
	* @param aLocationData the location data
	* @param aObjectId the object Id
	*/
    
    IMPORT_C void CreateLocationObjectL( const TLocationData& aLocationData,
    		const TUint& aObjectId );


	/**
	* Handle the photos taken by phone camera.
	* @param aObjectId Object id
	*/
    IMPORT_C void LocationSnapshotL( const TUint& aObjectId );

	/**
	* Creates a location object with given location data
	* @param aLocationData The location data
	* @return The locationId
	*/
    TItemId DoCreateLocationL( const TLocationData& aLocationData );

	/**
	* Creates a "contains" relation for the given ObjectId and LocationId and adds it to the iMdeSession
	* @param aObjectId Object id
	* @param aLocationId location id
	* @return item id
	*/	
    TItemId CreateRelationL( const TUint& aObjectId, const TUint& aLocationId );
	/**
	* Sets the CMdESession for tagcreation
	* @param aSession The CMdESession object
	*/    
    IMPORT_C void SetMdeSession( CMdESession* aSession );
    
	/**
	* Sets the current location trail state to Stop
	*/	
    IMPORT_C void SetStateToStopping();
    
	/**
	* returns the timevalue of the CMdEObject corresponding to a given ObjectId
	* @param aObjectId  the objectId
	* @return the timevalue associated with given object
	*/	
    TTime GetMdeObjectTimeL( TItemId aObjectId );

	/**
	* Checks if Remapping is needed
	* @return Boolean to indicate if remapping is needed or not
	*/	
    IMPORT_C TBool RemappingNeeded();

	
    IMPORT_C TBool IsLowBattery();

    /**
       * Checks if geotagging is in progress
       * @return Boolean to indicate if geotagging is in progress
       */  
    IMPORT_C TBool TaggingInProgress();   


    /**
       * Initiates geotagging
       * @param aConnectionOption connection option,silent or not
       * @return Etrue if geotagging is started
       */  
    IMPORT_C TBool StartGeoTagging(const TConnectionOption aConnectionOption);

    /** 
       * Cancels the geotagging
       */  
    IMPORT_C void CancelGeoTagging();

public: // from MNetworkInfoObserver.
    /**
     * 
     * NetworkInfo
     * This method is used for setting the network cell id to the 
     * @since S60 3.1
     * @param  aNetworkInfo N/W info
     * @param  aError       Error code
     */
    void NetworkInfo( const CTelephony::TNetworkInfoV1 &aNetworkInfo, TInt aError );

    /**
     * Get mobile phone object for network info object
     * @return mobile phone object
     */
    RMobilePhone& GetMobilePhone4NwInfo();
    
public: // from MPositionInfoObserver    
    /**
     * This method is used for setting the position info to the 
     * location trail.
     * @since S60 3.1
     * @param aPositionInfo the position information
     * @param aError Captures the errcode if any
     */
    void Position( const TPositionInfo& aPositionInfo, const TInt aError );
    
    
public: // From MMdEQueryObserver

	/**
	* Handle query new results
	* @param aQuery Query object
	* @param aFirstNewItemIndex new item index
	* @param aNewItemCount item count
	*/
	void HandleQueryNewResults(CMdEQuery& aQuery, TInt aFirstNewItemIndex, 
			TInt aNewItemCount);


	/*
	* This method  is called when any of the location/image/tag query is completed
	* @param aQuery The Query type
	* @param aError	Indicates if the query was completed successfully or with any err
	*
	*/
	void HandleQueryCompleted(CMdEQuery& aQuery, TInt aError);

#ifdef LOC_GEOTAGGING_CELLID	
public:     // MGeoConverterObserver
    /**
     * This method is used for notifying completion of geotagging
     * @param aError error code
     * @param aPosition position (lat/lon) for the correcponding n/w info
     */
     void ConversionCompletedL( const TInt aError, TLocality& aPosition );

	/**
	* This method is used to handle the conversion error
	* @param aError the error code
	*/
	 void HandleConversionError(TInt aError);
	
#endif	 
public: 

    /*
    * Get registrer network country code
    *
    * @return current register n/w info
    */
    IMPORT_C RMobilePhone::TMobilePhoneNetworkInfoV2& GetCurrentRegisteredNw();
	
    
private:   
    /**
     * Stores the location info into the array.
     * @param aSatelliteInfo Satellite information
     */
    void StoreLocation( const TPositionSatelliteInfo& aSatelliteInfo );
    
    /**
     * Changes the current state. New state is published in P&S and
     * possible observer is notified.
     * @param aState The location trail state that is to be set
     */    
    void SetCurrentState( TLocTrailState aState );
    
    /**
     * Returns the requested location via callback method, if the location
     * is valid. Otherwise new location value is requested until the value
     * is succesful, or the time out limit has been reached.
     * @param aSatelliteInfo The satelliteInfo object
     * @param aError Indicates any error in handling the location request
     * 
     */
    void HandleLocationRequest( const TPositionSatelliteInfo& aSatelliteInfo /*TLocality& aPosition*/, 
                                const TInt aError );
    /**
     * C++ constructor.
     * @param aGeoTaggerObserver The observer that is to be notified when geotagging completes
     */  
     CLocationRecord(MGeoTaggerObserver& aGeoTaggerObserver, RMobilePhone& aPhone);
 
    /**
     * 2nd phase constructor.
     */
    void ConstructL();
    
    /**
     * Read interval value from Central repository
     * @param aKey, Key to item
     * @param aValue, Read value
     */ 
    void ReadCenRepValueL(TInt aKey, TInt& aValue);

	/**
	* Validates the lat lon values recieved
	* @param aSatelliteInfo The satelliteInfo object
	* @return ETrue if the lat=lon are valid
	*/
    TBool CheckGPSFix( const TPositionSatelliteInfo& aSatelliteInfo );


	/**
	* Starts the network info timer
	*/
    void StartTimerL();


	/**
	* Fetches the location info from the db
	*/
	
    void FindLocationFromDBL();


	/**
	* Find location entry
	* @param aQuery       query type for the tag
	* @param aLocationId  Location Id
	*/
    void FindAnyLocationMatchesL( CMdEQuery& aQuery, TUint& aLocationId );
	
    /**
	* Remaps the location objects when GPS is available
	* @param aGPSInfoAvailable Boolean value to indicate if GPS is available
	*/
    void RemapObjectsL( TBool aGPSInfoAvailable );

    /**
	* Check any location object already exists with same network info
	*/	
    void FindLocationWithSameNetInfoL();

    /**
	* Initialises the location object definitions
	*/	
    void InitialiseL();

    /**
	* handle network location related query on complete
	* @param aQuery the query type
	*/    
    void HandleNetLocationQueryL( CMdEQuery& aQuery );


    /**
	* handle the location query  on location query complete
	* @param aQuery the query type
	*/	
    void HandleLocationQueryL( CMdEQuery& aQuery );

    /**
	* Callback method on geotagging complete
	*/	
    void GeoTaggingCompleted();
    
    /**
     * MProfileChangeObserver pure virtual function
     * @param aProfileEvent Profile event
     * @param aProfileId Active profile id
     */
     void HandleActiveProfileEventL(TProfileEvent aProfileEvent,TInt aProfileId );
     

#ifdef LOC_REVERSEGEOCODE
    /**
     * Get any imagefor this location object
     * @param aLocID, location object Id
     */ 
    void GetRelatedImageL( TItemId aLocID );
    
    /**
     * Get tags for this image
     * @param aImageID, image object Id
     */  
    void GetTagsL( TItemId aImageID );

    /**
	* handle the tag query on complete
	* @param aQuery the query type
	*/	
    void HandleTagQueryL( CMdEQuery& aQuery );


     // MReverseGeocodeObserver
     //from reverse-geocode observer 
     /*
        * Call back method from reverse geo coder with address details like country, city..
        * @param aErrorcode Indicates any error in Reverse geocoding
        * aAddressInfo address info
        */
     void ReverseGeocodeComplete( TInt& aErrorcode, MAddressInfo& aAddressInfo );
    
    /*
    * Get registrer network country code
    *
    * @return current register n/w info
    */
    RMobilePhone::TMobilePhoneNetworkInfoV2& GetCurrentRegisterNw();
	
    /*
    * UE is registered to home network?
    *
    * @return ETrue if UE is registered at home network else EFalse
    */
    TBool IsRegisteredAtHomeNetwork();


    /*
    * Get home network country code
    * @param aHomeNwInfoAvailableFlag ETrue if home n/w info available else EFalse
    * @return user home n/w info
    */
    const RMobilePhone::TMobilePhoneNetworkInfoV1& 
        GetHomeNetworkInfo(TBool& aHomeNwInfoAvailableFlag);

	/**
	* Find country & city tags id
	* @param aQuery         query type for the tag
	* @param aCountryTagId  country tag Id
	* @param aCityTagId     city tag Id
	*/    
    void FindCountryAndCityTagL( CMdEQuery& aQuery, TItemId& aCountryTagId, TItemId& aCityTagId );

#endif // LOC_REVERSEGEOCODE

    /**
	* Handle n/w related information on taken photograph
	* 
	*/	
    void NetworkInfoSnapshotL();

    /**
	* Find location from DB within the non leaving method
	*/	
	void HandleFindLocationFromDB();

    /**
	* handle MDS query failure sceenario
	*/	
	void HandleQueryFailure();

    
private:
    enum TRemapState
        {
        ERemapProgressNone = 0x00,
        ERemapRevGeoCodeInProgress,
        ERemapRevGeoCodePending,
        ERemapNwGeoConverterInProgress,
        ERemapNwGeoConverterPending
        };
	/**
	 * A session to Metadata Engine for creating and manipulating location objects.
	 */
	CMdESession* iMdeSession;
	
    /**
     * An observer interface to notify about state changes.
     * Not own.
     */
    MLocationTrailObserver* iObserver;
    
    /**
     * An observer interface to notify about new locations.
     * Not own.
     */
    MLocationAddObserver* iAddObserver;    

    /**
     * An array to collect location values.
     * Own.
     */
    RPointerArray<TLocationTrailItem> iTrail;

    /**
     * P&S key property.
     * Own.
     */
    RProperty iProperty;

    /**
     * Active class to get network information.
     * Own.
     */
    CNetworkInfo* iNetworkInfoChangeListener;
    
    /**
     * Active class to get position information.
     * Own.
     */
    CPositionInfo* iPositionInfo;
    
	/**
	 * Class which handles database remapping operations
	 */ 
	CLocationRemappingAO* iRemapper;
    
    CActiveSchedulerWait   iWait;
    
    /**
     * Timer for capturing network info only
     */
    CPeriodic*			   iNetworkInfoTimer;

    TLocTrailState         iState;    
    TLocationTrailItem     iNewItem;
    RLocationTrail::TTrailCaptureSetting   iTrailCaptureSetting;
    CTelephony::TNetworkInfoV1		   iNetwork;

    TInt                   iMaxTrailSize;
    TInt                   iLocationCounter;
    
    /*
     * Interval value for location trail
     */
    TInt iInterval;
    
    TBool                  iRequestCurrentLoc;
    TBool                  iTrailStarted;
    
    TBool				   iLastGPSFixState;
    
    TInt                   iLocationDelta;
    TLocationData          iLastLocation;
    
    TItemId iObjectId;               
    TLocationData iLocationData;
    
    /**
     * This query object is used to find existing locations
     */
    CMdEObjectQuery* iLocationQuery;
	

    /** 
     * Net location query
     */
    CMdEObjectQuery* iNetLocationQuery;
    
    
    TRemapState iRemapState;
    
    
    /**
     * Database definitions.
     */
    CMdENamespaceDef* iNamespaceDef;  
    CMdEObjectDef* iLocationObjectDef;
    CMdEPropertyDef* iLatitudeDef;
    CMdEPropertyDef* iLongitudeDef;
    CMdEPropertyDef* iAltitudeDef;
    
    /**
      * An array to collect downloaded object 
      * and its location details.
      */
    RPointerArray<TLocationSnapshotItem> iMediaItems;
        
    TLocationSnapshotItem iLastMediaItem;
        
    TInt                 iMediaHandlingFlag;
	RMobilePhone&	iPhone;	

    /*
    * Flag to track GPS data availability to update camera location icon.
    */
    TBool iGpsDataAvailableFlag;

    MGeoTaggerObserver& iGeoTaggerObserver;

#ifdef LOC_GEOTAGGING_CELLID	
    /*
       * Geo converter
       */
	CGeoConverter* iGeoConverter;
    TBool iConvertRetry;
#endif

#ifdef LOC_REVERSEGEOCODE
    
    TConnectionOption    iConnectionOption;

    /** 
     * query object for getting an image/video object
     */
   	CMdERelationQuery* iImageQuery;
   	/** 
   	 * query object for getting country and city tags
   	 */
    CMdERelationQuery* iTagQuery;	
    TLocationSnapshotItem iLastLocationItem;
    RPointerArray<TLocationSnapshotItem> iLocationItems;
    /** 
     * to create/attach tags
     */
   	CTagCreator *iTagCreator;
    /*
     * Flag to track last reverse geocode failure sceenario
     */
	TBool iLastReverseGeocodeFails;

    /*
     * Reverse geo coder plugin object
     */
	 CReverseGeoCoderPlugin* iRevGeocoderPlugin;

#endif

    /**
     *to listen profile change.
     */
   CProfileChangeNotifyHandler* iProfileHandler; 
   
   /* 
   *To check for offline profile 
   */
   TBool iOfflineCheck;
 
    };

#endif // C_CLOCATIONRECORD_H 

// End of file.
