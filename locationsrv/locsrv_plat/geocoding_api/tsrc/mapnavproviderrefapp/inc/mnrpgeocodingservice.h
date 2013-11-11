/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpGeocodingService class definition
*
*/


#ifndef MNRP_GEOCODINGSERVICE_H
#define MNRP_GEOCODINGSERVICE_H

#include "mngeocodingservicebase.h"

class CPosLandmark;
class CMnrpEngine;

/** Implementation of geocoding service (KMnAppGeocodingService). 
 *	It uses CIdle to simulate asynchronous processing. All the geocoding
 *	job is done using the CMnrpEngine class.
 */
class CMnrpGeocodingService : public CMnGeocodingServiceBase
    {
    public :
        static CMnrpGeocodingService* NewL( CMnrpEngine& aEngine );
        
    protected :
        CMnrpGeocodingService( CMnrpEngine& aEngine );
        virtual ~CMnrpGeocodingService();

        void ConstructL();

		/** Prevents parallel asynchronous requests */
        void LeaveIfBusyL();
        /** Starts processing of new request by starting CIdle.*/
        void StartProcessingL();
        /** Callback for CIdle. Parameter is the pointer to this. */
        static TInt GeocodingProcessingCallBack( TAny* aPtr );
        /** Non-leaving part of geocoding. Calls FinishProcessingL and if
         *	it leaves, completes client's request with error.*/
        void HandleGeocodingCompleted();
        /** Does the geocoding and completes user's request.*/
        void FinishProcessingL();

        /** Asks user to select one of best Coord to Addr matches */
        CPosLandmark* LetUserSelectBestMatchL( const CPosLandmark& aReference );

    protected: // from CMnGeocodingServiceBase
    
        /** Handles client's request for finding address by given coordinate. */
        void HandleFindAddressByCoordinateL( const CPosLandmark& aLandmark );
        
        /** Handles client's request for finding coordinate by given address. */
        void HandleFindCoordinateByAddressL( const CPosLandmark& aLandmark );

        /** Handles client's request for finding coordinate by given address. */
        void HandleFindCoordinateByAddressL( const TDesC& aAddress );
        
    protected: // from CMnServiceBase

        /** Reports that client cancelled request */
        void DoCancel();

    private:
        CMnrpEngine&            iEngine;
    
        enum TServiceType
            {
            ENone,
            ECoordToAddr,
            EAddrToCoord,
            EPlainToCoord
            };
        
        /** Type of last received request to make correct request to Engine */    
        TServiceType            iCurrentService;
    
    	/** Keeps client's data */
        CPosLandmark*           iLandmark;
    	/** Keeps client's data */
        HBufC*                  iPlainAddress;

        /** Keeps client's option */
        TBool                   iDialogAllowed;

		/** Asynchronous geocoding processor */        
        CIdle*                  iProcessor;
        
    };

#endif // MNRP_GEOCODINGSERVICE_H


