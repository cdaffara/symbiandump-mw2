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
* Description:  An interface to Location Trail.
*
*/

#ifndef R_RLOCATIONTRAIL_H
#define R_RLOCATIONTRAIL_H

#include <e32base.h>
#include <etel3rdparty.h>
#include <lbsposition.h>
#include <rlocationmanager.h>
#include <locationdatatype.h>

typedef TPckg<TLocality> TLocalityPckg;

/**
 *  RLocationManager class is used for creating a Location Manager session.
 *  Location Manager is used to start and stop the location trail, retrieve 
 *  location information and write the location information to images.
 *
 *  @lib LocationManager.lib
 *  @since S60 3.1
 */
NONSHARABLE_CLASS( RLocationTrail ) : public RLocationManager
    {
public:
    /**
     * Location trail states.
     */
    enum TTrailState
        {
        ETrailStopped,
        EWaitingGPSData,
        ESearchingGPS,
        ETrailStarted,
        ETrailStopping,
        ETrailStarting
        };
    enum TTrailCaptureSetting
    	{
    	EOff,
    	ECaptureNetworkInfo,
    	ECaptureAll 
    	};
        
public:
	IMPORT_C RLocationTrail();
	
	IMPORT_C ~RLocationTrail();

    /**
     * Starts recording location information to location trail.
     * @since S60 3.2
     * @param aState, an enumeration of ECaptureNetworkInfo  
     * (only cell ID stored) and ECaptureAll 
     * (GPS coordinates and cell ID stored).
     * @return KErrNone if successful, otherwise one of the other 
     *         system-wide error codes.
     */
    IMPORT_C TInt StartLocationTrail(TTrailCaptureSetting aState);
    
    /**
     * Stops recording location information to location trail.
     * @since S60 3.1
     * @param None.
     * @return KErrNone if successful, otherwise one of the other 
     *         system-wide error codes.
     */
    IMPORT_C TInt StopLocationTrail();
    
    /**
     * Retuns the current state of the location trail.
     * @since S60 3.1
     * @param aState, The state of the trail is returned.
     * @return KErrNone if successful, otherwise one of the other 
     *         system-wide error codes.
     */
    IMPORT_C TInt GetLocationTrailState( TTrailState& aState );
    
    /**
     * Get notification when the location trail state changes.
     * @since S60 3.1
     * @param aStatus, The request status. 
     * @return None.
     */
    IMPORT_C void NotifyLocationTrailStateChange( TRequestStatus& aStatus ); 
    
    /**
     * Cancel notification request for the location trail state change.
     * @since S60 3.1
     * @param None.
     * @return None.
     */
    IMPORT_C void CancelNotificationRequest();    
    
    /**
     * Get location by time stamp. If the TLocality for given time stamp is 
     * found from the trail, it is returned even if the data in it wouldn't 
     * be valid.
     * @since S60 3.1
     * @param aTimeStamp, Time stamp to get corresponding location.
     * @param aPosition, Position value returned.
     * @param aNetworkInfo, Network value returned.
     * @param aState, State of the trail at given time.
     * @return KErrNone if successful, otherwise one of the other 
     *         system-wide error codes.
     */                                
    IMPORT_C TInt RetrieveLocation( const TTime& aTimeStamp, 
    								TLocationData& aLocationData,
                                    TTrailState& aState );

    /**
     * Retrieve current location. If request is completed successfully, the 
     * result is returned via the aLocationInfo and aNetworkInfo arguments. 
     * The function is asynchronous, because it may take 
     * several minutes to get location information. 
     * @since S60 3.2
     * @param aStatus, The request status.
     * @param aLocationData, locationData encapsulates all location data
     * @return None.
     */
    IMPORT_C void CurrentLocation( TRequestStatus& aStatus, 
    							   TLocationData& aLocationData);					
                                   
    /**
     * Cancel asynchronous location request.
     * @since S60 3.1
     * @param None.
     * @return None.
     */
    IMPORT_C void CancelLocationRequest();

    /**
     * Get current trail capture setting.
     * @return EOff, ECaptureNetworkInfo or ECaptureAll
     */
    IMPORT_C TInt GetTrailCaptureSetting( TTrailCaptureSetting& aCaptureSetting );
    
private:
	TPckg<TLocationData>* iCurrentLocData;
    };
    
#endif // R_RLOCATIONTRAIL_H

//End of File
