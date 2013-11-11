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
* Description:  A class responsible for geotagging of media files in background.
*
*/

#ifndef C_CINTERNALGEOTAGGER_H
#define C_CINTERNALGEOTAGGER_H

#include <e32base.h>
#include <e32property.h>
#include <lbs.h>
#include <locationdatatype.h>
#include <geotagger.h>

#include "mdccommon.h"
#include "mdesession.h"
#include "mdenamespacedef.h"
#include "mdeobjectdef.h"
#include "mdepropertydef.h"
#include "mderelation.h"
#include "mdequery.h"

#ifdef LOC_GEOTAGGING_CELLID
#include "cgeoconverter.h"
#endif

#ifdef LOC_REVERSEGEOCODE
#include "ctagcreator.h"
#include <reversegeocoderplugin.h>
#endif


/**
 *  GeoTagger converts a given cell ID(CGI Info) to geo-coordinates.
 *  Does reverse geocoding to get country and city name.
 *  Creates country and city tag for image/video files.  
 *  @since S60 9.2
 */
 
class CInternalGeoTagger : public CGeoTagger,
                   public MMdESessionObserver,
                   public MMdEQueryObserver
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
     * @param aMdeSession An instance of the CMdESession
     * @param aObserver   The observer that is to be notified when geotagging is over
     */
    IMPORT_C static CInternalGeoTagger* NewL( CMdESession* aMdeSession,
        MGeoTaggerObserver* aObserver );

    /**
      * C++ destructor.
      */    
    IMPORT_C virtual ~CInternalGeoTagger();

    
public:    //exported API
    // CGeoTagger
     /**
       * Harvests location tags.
       * @param aObserver, observer for geotagging.
       * @return None.
       */    
    virtual void CreateGeoTagsL( const TConnectionOption = EInteractive  );

     /**
       * Checks if there is any untagged photos with location info and tags them.
       * @param aTagInProgress, boolean value to indicate if geotagging is in progress.
       * @return None.
       */
    virtual void PendingGeoTagsL( TBool aTagInProgress );
    
public:   //observer methods

    // From MMdEQueryObserver

    /**
      * @param aQuery the query type
      * @param aFirstNewItemIndex The new item index
      * @param aNewItemCount   The new item count
      */
      void HandleQueryNewResults(CMdEQuery& aQuery, TInt aFirstNewItemIndex, 
              TInt aNewItemCount);

    /**
      * This method is called on completion of a query
      * @param aQuery the query type
      * @param aError  Error if any
      */    
      void HandleQueryCompleted(CMdEQuery& aQuery, TInt aError);
    

#ifdef LOC_REVERSEGEOCODE
    /**
       * Get location objects, where lat, long not populated.
      * @param  aCountryTagId The country tagId
      * @param aCityTagId  The city tagId
      */        
    void AddressInfoL( const TItemId aCountryTagId, const TItemId aCityTagId );

    
    //MReverseGeocodeObserver
    /**
      * This method is called when reverse geocoding is completed
      *@param  aErrorcode  Error if any
      *@param aAddressInfo   the reverse geocoded address     
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
    
#endif //LOC_REVERSEGEOCODE
    
    // From MMdESessionObserver

    /** 
      * Handles session opening
      *@param  aSession  A reference of the CMdESession
      *@param aError     error if any
      */    
    void HandleSessionOpened(CMdESession& aSession, TInt aError);

    /**
      * Handles any error in session opening
      *@param  aSession  A reference of the CMdESession
      *@param aError     error if any
      */    
    void HandleSessionError(CMdESession& aSession, TInt aError);

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
        
private:
       /**
       * Update media object with GPS info.
       */
    void UpdateGPSInfoL(const TLocality& aPosition);
    
#endif //LOC_GEOTAGGING_CELLID

private:    
    
    /**
     * C++ constructor.
     * @param aMdeSession An instance of the CMdESession
     * @param aObserver   The observer that is to be notified when geotagging is over     
     */  
    CInternalGeoTagger( CMdESession* aMdeSession, MGeoTaggerObserver* aObserver );
    
    /**
      * 2nd phase constructor.
     */
    void ConstructL();
 
    /**
      * Get all relation objects associated with this location
     * @param aLocID The locationId whose relation is to be fetched
     */
    void GetRelationObjectsL(TItemId aLocID);

    /**
      * get tags for this image
     * @param aImageID The imageIs whose tag is to be fetched
     */ 
    void GetTagsL( TItemId aImageID );

    /**
       * Get location objects, where lat, long not populated.
      */    
    void GetAllLocationsL( );

    /**
       * checks if the session is ready
      * @return ETrue If the session is ready
      */        
    TBool IsSessionReady();

    /**
       * Fetches the location information
      */        
    void GetLocationInfoL();

    /**
       * starts geotagging of objects
      */        
    void StartGeoTaggingL();

    /**
       * Checks if Location atg exists for an object
      * @return ETrue if location tag is found
      */        
    TBool LocationTagExists();

    /**
       * Moves to the next location for reverse geocoding
      */        
    void IterateNextLocation();
    
    /**
       * @return ETrue if item had valid lat/lon else EFalse
      */        
    TBool GPSInfoExists();
private:
    /**
     * A session to Metadata Engine for creating and manipulating location objects.
     */
    CMdESession* iMdeSession;

    /**
     * This query object is used to find existing locations
     */
    CMdEObjectQuery* iLocationQuery;
    /** @var iRelationQuery. */
    CMdERelationQuery* iRelationQuery;
    CMdERelationQuery* iTagQuery;
    
    TInt iIndex;
        
    TBool iSessionReady;  
    
    MGeoTaggerObserver* iObserver;
    
    TLocationData iLocationData;
    
    TItemId iLocationId;
    
    TConnectionOption iConnectionOption;
    
    
    /**
     * An active scheduler wait loop for waiting a session to MdE to open.
     */
    CActiveSchedulerWait* iASW;
    TBool iMdeSessionOwnFlag;
    TBool iTagPendingHandlerFlag;
#ifdef LOC_GEOTAGGING_CELLID	
    /*
       * Geo converter
       */
	CGeoConverter* iGeoConverter;
#endif
#ifdef LOC_REVERSEGEOCODE
        CTagCreator* iTagCreator;
        CReverseGeoCoderPlugin* iRevGeocoderPlugin;
#endif //LOC_REVERSEGEOCODE
    };

#endif // C_CGEOTAGGER_H 

// End of file.
