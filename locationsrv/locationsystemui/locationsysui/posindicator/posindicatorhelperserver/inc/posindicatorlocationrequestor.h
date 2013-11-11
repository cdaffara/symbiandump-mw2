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
* Description: Declaration of location requestor class.
*
*/

#ifndef POSINDICATORLOCATIONREQUESTOR_H
#define POSINDICATORLOCATIONREQUESTOR_H

#include <lbs.h>
#include <lbssatellite.h>
#include <EPos_CPosLandmark.h>
#include "posreversegeocodeinterface.h"

class CPosIndicatorLocationRequestor : public CActive,
                                       public MPosReverseGeocodeObserver
    {
    /**
     * This defines the different states of location requestor
     */
    enum TOperation
        {
        /**
         * Idle state
         */
        ENone,
        
        /**
         * Acquiring last known position
         */
        ELastKnownPosition,
        
        /**
         * During acquiring location
         */
        ECurrentPosition,
        
        /**
         * During acquiring address.
         */
        ECurrentAddress
        };
public: 
    /**
     * Factory function that instantiates an object 
     * of CPosIndicatorLocationRequestor
     */
    static CPosIndicatorLocationRequestor* NewL();

    /**
     * Destructor
     */
    ~CPosIndicatorLocationRequestor();
    
    /**
     * Gets current position
     */
    void GetCurrentPosition( const RMessage2& aMessage );
    
    /**
     * Gets current address info size.
     */
    void GetCurrentAddressInfoSizeL( const RMessage2& aMessage );
    
    /**
     * Gets current address info.
     */
    void GetCurrentAddressInfoL( const RMessage2& aMessage );
    
    /**
     * Cancels outstanding request 
     */
    void CancelRequest( const RMessage2& aMessage );
    

public: // From MPosReverseGeocodeObserver
    virtual void ReverseGeocodeComplete( TInt aErrorcode );
    
protected: // From CActive
    void RunL();
    
    void DoCancel();

private:
    /**
     * Default constructor.
     */
    CPosIndicatorLocationRequestor();
    
    /**
     * 2nd phase constructor for instantiating member variables     
     */
    void ConstructL();
    
    
private: // Data members
    /**
     * Queue of position request
     */
    RArray<RMessage2>   iPositionRequestQueue;
    
    /**
     * Queue of address request
     */
    RArray<RMessage2>   iAddressRequestQueue;
    /**
     * Handle to the location server interface
     */
    RPositionServer     iPosServer;
    
    /**
     * Handle to create a subsession with the location sever
     */
    RPositioner         iPositioner;
    
    /**
     * Pointer to reverse geo-coding plugin object
     * Own
     */
    CPosReverseGeocodeInterface* iReverseGeoCodeInterface;
    
    /**
     * Pointer to landmark object
     * Own
     */
    CPosLandmark* iLandmark;
    
    /**
     * Pointer to landmarks package.
     * Own 
     */
    HBufC8* iLandmarkPackage;
    
    /**
     * Last known position 
     */
    TPositionInfo iLastKnownInfo;
    
    /**
     * Requested satellite info.
     */
    TPositionSatelliteInfo     iPositionInfo;
   
    /**
     * Current operation that is progress.
     */
    TOperation          iOperation;
    };

#endif /* POSINDICATORLOCATIONREQUESTOR_H */
