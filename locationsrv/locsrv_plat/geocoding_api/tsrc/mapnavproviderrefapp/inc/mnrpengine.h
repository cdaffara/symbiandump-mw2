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
* Description:  CMnrpEngine class definition
*
*/


#ifndef MNRP_ENGINE_H
#define MNRP_ENGINE_H

#include <e32base.h>
#include <f32file.h>

class CPosLandmark;
class TCoordinate;
class RResourceReader;

/** Engine class used both by MnRefProvider and MnRefGeocoder 
 *	components. Provides access to POIs database ( resource file,
 *	some cities ) and geocoding services. 
 */
class CMnrpEngine : public CBase
    {
    public:
    
        struct TDistanceToPoi
            {
            TInt iPoiIndex; // index to KnownPlaces
            TReal32 iDistance; // distance to Poi of iPoiIndex
            };
    
    public:
        IMPORT_C static CMnrpEngine* NewL();
        IMPORT_C ~CMnrpEngine();

		/** Gives access to set of POIs */
        IMPORT_C TArray<CPosLandmark*> KnownPlacesL();
        
        /** Returns coordinate of given address:
		 *	Find a POI, which country or city matches given address
		 *	and returns its coordinates.
		 *	@param aAddress landmark, containing address
		 *	@return landmark, containg coordinates
		 *	@leave KErrNotFound if can't find POI matching given address
 		 */
        IMPORT_C CPosLandmark* AddressToCoordLC( const CPosLandmark& aAddress );

        /** @overload
         *  @return landmark, containg coordinates, or NULL
         */
        IMPORT_C CPosLandmark* AddressToCoordL( const CPosLandmark& aAddress );
        
        /** @overload
		 *	@param aAddress string containing address
		 *	@return landmark, containg coordinates
		 *	@leave KErrNotFound if can't find POI matching given address
 		 */
        IMPORT_C CPosLandmark* AddressToCoordLC( const TDesC& aAddress );

        /** @overload
         *  @param aAddress string containing address
         *  @return landmark, containg coordinates, or NULL
         */
        IMPORT_C CPosLandmark* AddressToCoordL( const TDesC& aAddress );

        /** Returns address for given coordinate:
		 *	Find a closest POI and return its city and country and
 		 *	fill street with "%d[m,Km,tKm] from <city>" string, like "5 km to Tampere"
		 *	@param aLocation coordinate
		 *	@return landmark, containg address
		 *	@leave KErrNotFound if can't find any POI
 		 */
        IMPORT_C CPosLandmark* CoordToAddressLC( const TCoordinate& aLocation );
        
        /** Finds a resource file in the system. 
         *	It looks on all the drives for the file and folder given as parameter
         *	@param[in] aFilePathAndName path and name of the file to search for. Drive
         *							info is ignored
         *	@param[out] aRscFile full name of found resource file
         *	@leave KErrNotFound if file is not found.
         */
        IMPORT_C void FindResourceFileL( 
            const TDesC& aFilePathAndName,
            TFileName& aRscFile );

        /** Returns reference to open file session, given at construction */
        IMPORT_C RFs& Fs();

        /** Calculates distance between two landmarks.
         *  @param aFrom Source landmark
         *  @param aTo Destination landmark
         *  @return Distance, or NaN, if distance cannot be calculated
         */
        IMPORT_C TReal32 DistanceBetweenLandmarks( 
            const CPosLandmark& aFrom, 
            const CPosLandmark& aTo );

        /** Finds POIs closest to given reference landmark. 
         *  @param[in] aReference reference landmark
         *  @param[out] aNeighbourPois List of closest POIs. 
         *  @param[in] aMaxMatches Maximum number of results needed
         */
        IMPORT_C void FindClosestPoisL( 
            const CPosLandmark& aReference,
            RArray<CMnrpEngine::TDistanceToPoi>& aNeighbourPois,
            TInt aMaxMatches );

        /** Finds POIs closest to given location. 
         *  @param[in] aReference reference location
         *  @param[out] aNeighbourPois List of landmarks of closest POIs. 
         *  @param[in] aMaxMatches Maximum number of results needed
         */
        IMPORT_C void BestCoordToAddressMatchesL( 
            const TCoordinate& aLocation,
            RPointerArray<CPosLandmark>& aMatches,
            TInt aMaxMatches );

    protected:

        void ConstructL();
        CMnrpEngine();

		/** Loads POIs from database ( resource file ) */
        void LoadKnownPlacesL();
        /** Reads array of PLACES resource structures */
        void ReadPlacesResourceL( RResourceReader& aReader );
        
        CPosLandmark* CreateCloseLandmarkLC( const CPosLandmark& aReference, TReal32 aDistance );

    private:
        RPointerArray<CPosLandmark> iPlaces;
        RFs                         iFileSession;
    };

#endif // MNRP_ENGINE_H

