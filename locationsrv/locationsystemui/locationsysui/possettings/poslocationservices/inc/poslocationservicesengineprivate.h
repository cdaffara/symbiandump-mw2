/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Class declaration of PosLocationServicesEnginePrivate.  
*
*/
#ifndef POSLOCATIONSERVICESENGINEPRIVATE_H
#define POSLOCATIONSERVICESENGINEPRIVATE_H

#include "poslocationservicescommon.h"
#include <epos_msuplsessionobserver.h>

// Forward declaration
class CSuplSettings;

/**
 * The class defines the interface to get notification on location 
 * service update.The class interested in location service update
 * should inherit from this class.
 */
class PosLocationServiceUpdateObserver
    {
public:
    virtual void handleLocationServiceUpdate() = 0;
    };


/**
 * PosLocationServicesEnginePrivate class implements the engine logic.This 
 * provides wrapper over SUPL settings API
 */
class PosLocationServicesEnginePrivate : public MSuplSessionObserver
    {
public:
    /**
     * Constructor
     */
    PosLocationServicesEnginePrivate( PosLocationServiceUpdateObserver&
                                      aObserver );
    
    /**
     * Destructor
     */
    virtual ~PosLocationServicesEnginePrivate();
    
    /**
     * Retrieves all location services info.
     * 
     * @param[out] locationServiceList List of location 
     *            services info.
     * @return  - KErrNone if location services information retrieved successfully.  
     *          - KErrNotFound if there are no location services. 
     *            otherwise system wide error codes.             
     */
    int GetLocationServices( QList<PosLocationServiceInfo>& 
                             aLocationServiceList );
    
    /**
     * Retrieves all location services info.
     * 
     * @param[out] locationServiceList List of location 
     *            services info.
     * @leaves  - KErrNone if location services information retrieved successfully.  
     *          - KErrNotFound if there are no location services. 
     *            otherwise system wide error codes. 
     */
    void GetLocationServicesL( QList<PosLocationServiceInfo>& 
                             aLocationServiceList );
    /**
     * Removes location service based on the service Id.
     * 
     * @param [in] serviceId Location service Id. 
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if service is not found
     *      otherwise system wide error codes. 
     */
    int RemoveLocationService( int aServiceId );
    
public: // From MSuplSessionObserver
    virtual void HandleSuplSessionChangeL(
            TSuplSessionEventType aEvent,TInt64 aSessionId );
    
private:// Data members
    
    /**
     * Reference to location service update observer.
     */
    PosLocationServiceUpdateObserver& mObserver;
    
    /**
     * Pointer to SUPL settings object.
     * Own
     */
    CSuplSettings* mSuplSettings;
    };

#endif /* POSLOCATIONSERVICESENGINEPRIVATE_H */
