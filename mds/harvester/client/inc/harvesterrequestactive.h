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
* Description:  Active object for an asynchronous thumbnail request
 *
*/


#ifndef HARVESTERREQUESTACTIVE_H
#define HARVESTERREQUESTACTIVE_H

#include <e32base.h>

#include "harvesterclient.h"
#include "harvestercommon.h"

class CHarvesterRequestQueue;

/**
 *  Active object for an asynchronous harvest request.
 *
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CHarvesterRequestActive ): public CActive
    {
public:

    /**
     * Destructor
     *
     * @since S60 v5.0
     */
    virtual ~CHarvesterRequestActive();

    /**
     * Two-phased constructor
     *
     * @since S60 v5.0
     * @param aClient Session used.
     * @param aObserver Observer to receive notifications about completed
     *                  operations.
     * @param aService service to call for
     * @param aUri         Uri of the file to be harvested.
     * @param aAlbumIds  If the harvested file is to be added to some albums, 
     *                               the IDs of these albums..
     * @param aAddLocation   Defines if locationdata is to be added to the harvested item.
     * @param aQueue Queue for harvester calls
     * @return New CHarvesterRequestActive object.
     */
    static CHarvesterRequestActive* NewL( 
            RHarvesterClient& aClient, MHarvestObserver* aObserver,
            TInt aService, const TDesC& aUri, 
            HBufC8* aAlbumIds, TBool aAddLocation,
            CHarvesterRequestQueue* aQueue );

    /**
     * Start harvesting
     *
     * @since S60 v5.0
     */
    void Start();
    
    /**
     * Forces the harvesting request to be sent to server immidiately
     *
     * @since S60 v5.0
     */
    void ForceHarvest();


    /**
     * Is request completed
     *
     * @since S60 v5.0
     */
    TBool RequestCompleted() const;

private:

    /**
     * C++ default constructor
     *
     * @since S60 v5.0
     * @param aClient Session used.
     * @param aObserver Observer to receive notifications about completed
     *                  operations.
     * @param aService service to call for
     * @param aUri         Uri of the file to be harvested.
     * @param aAlbumIds  If the harvested file is to be added to some albums, 
     *                               the IDs of these albums..
     * @param aAddLocation   Defines if locationdata is to be added to the harvested item.
     * @param aQueue Queue for harvester calls
     * @return New CHarvesterRequestActive object.
     */
    CHarvesterRequestActive( RHarvesterClient& aClient,
            MHarvestObserver* aObserver, TInt aService, const TDesC& aUri, 
            HBufC8* aAlbumIds, TBool aAddLocation,
            CHarvesterRequestQueue* aQueue );

    /**
     * Handles an active object's request completion event.
     *
     * @since S60 v5.0
     */
    void RunL();

    /**
     * Handles a leave occurring in the request completion event handler
     * RunL().
     *
     * @since S60 v5.0
     * @param aError The leave code.
     * @return Error code.
     */
    TInt RunError( TInt aError );

    /**
     * Implements cancellation of an outstanding request.
     *
     * @since S60 v5.0
     */
    void DoCancel();

private:
    // data

    friend class CHarvesterRequestQueue;
    
    /**
     * Session. Ref.
     */
    RHarvesterClient iClient;

    /**
     * Observer to receive notifications about completed operations. Not owned.
     */
    MHarvestObserver* iObserver;
    
    TInt iService;
    
    /**
     * Request parameters.
     */
    TFileName iUri;
    
    // Own.
    HBufC8* iAlbumIds;

    TBool iAddLocation;
    
    // not own
    CHarvesterRequestQueue* iRequestQueue;
    
    TPckg<TBool> iLocation;
    
    TIpcArgs iPersistentArgs;
    
    /**
     * Error code.
     */
    TInt iError;

    /**
     * If set, this request object can be deleted.
     */
    TBool iRequestCompleted;
    
    TBool iCancelled;
};

#endif // HARVESTERREQUESTACTIVE_H



// INLINE FUNCTIONS

inline TBool CHarvesterRequestActive::RequestCompleted() const
    {
    return iRequestCompleted;
    }

