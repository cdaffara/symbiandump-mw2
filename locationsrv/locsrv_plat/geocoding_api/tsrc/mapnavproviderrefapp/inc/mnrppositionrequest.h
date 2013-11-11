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
* Description:  CMnrpPositionRequest class definition
*
*/


#ifndef MNRP_POSITION_REQUEST_H_
#define MNRP_POSITION_REQUEST_H_

#include <e32base.h>
#include <lbs.h>

class TPositionUpdateOptions;

/** Position observer */
class MMnrpPositionObserver
    {
    public:
        virtual void HandlePositionRequestCompletedL( TInt aError ) =0;
    };

/** Provides an interface for retrieving the current location.
 */
class CMnrpPositionRequest: public CActive
    {
    public: 

        /** Creates new instance of request
        * @param aAppName the name of this application
        * @param aObserver observer to callback when locations are retrieved
        */
        IMPORT_C static CMnrpPositionRequest* NewL(
            const TDesC& aRequestorName,
            MMnrpPositionObserver& aObserver);

        IMPORT_C ~CMnrpPositionRequest();

    public: 

        /**
         * Starts the fetching of a location. In the first place the default 
         * proxy is used for retrieving a location. If fetching default location
         * fails the last known location is fetched. If fetching last known 
         * location fails, an error code is returned to the observer.
         *
         * @param aLastKnownLocationAllowed whether last know location is allowed
         */
        IMPORT_C void FetchNewPosition( TBool aLastKnownLocationAllowed = EFalse );

 		/** Returns current location */
        IMPORT_C void GetPosition( TPositionInfo& aPosition );

 		/** Returns current speed info */
        IMPORT_C TReal32 Speed();

 		/** Returns current heading info */
        IMPORT_C TReal32 Heading();

 		/** Returns current magnetic heading info */
        IMPORT_C TReal32 MagneticHeading();

 		/** Returns current course info */
        IMPORT_C TReal32 Course();

 		/** Returns current magnetic course info */
        IMPORT_C TReal32 MagneticCourse();

		/** Modifies options for the next request */
        IMPORT_C void SetOptionsL( const TPositionUpdateOptionsBase& aOptions );

    protected: // from CActive

        void DoCancel();
        void RunL();
        TInt RunError(TInt aError);

    private:

        CMnrpPositionRequest( MMnrpPositionObserver& aObserver );
        void ConstructL( const TDesC& aAppName );

        /** Issues new request for location retrieval. */
        void ExecuteNextStep();

        void InitContainer( HPositionGenericInfo& aGenericInfo );
        void SaveContainer( const HPositionGenericInfo& aGenericInfo );

    private:

        /**
        * TState defines the different states this object may enter.
        */
        enum TState 
            {
            EIdle,
            EAcquiringDefaultLocation,
            EAcquiringLastKnownLocation
            };

    private: 

        //! Contains the location
        HPositionGenericInfo* iGenericPosition;
        
        //! Local storage for last location info
        TPositionInfo iPositionInfo;
        TReal32 iSpeed;
        TReal32 iHeading;
        TReal32 iMagneticHeading;
        TReal32 iCourse;
        TReal32 iMagneticCourse;

        //! The observer to callback when location retieval is ready
        MMnrpPositionObserver& iObserver;

        //! Reference to the Location Server session
        RPositionServer iLocationServer;

        //! Reference to the Location Server subsession
        RPositioner iPositioner;
        
        TBool iLastKnownLocationAllowed;

        //! The current state this object has entered
        TState iState;
    };

#endif // MNRP_POSITION_REQUEST_H_

