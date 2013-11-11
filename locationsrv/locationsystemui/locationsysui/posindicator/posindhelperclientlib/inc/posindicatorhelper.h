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
* Description:  Positioning Indicator Helper Client Interface
*
*/



#ifndef POS_INDICATOR_HELPER_H
#define POS_INDICATOR_HELPER_H

#include <e32std.h>
#include <lbssatellite.h>
#include <locationvariationdomaincrkeys.h>
#include <centralrepository.h>

#include "posindicatorserverconst.h"
#include "posindicatorlogger.h"

class CPosIndHelperClientRequester;
class RPosIndicatorHelperServer;
class MPosIndInfoChangeObserver;
class CPosLandmark;

/**
 * A handle to Positioning Indicator Helper Server subsession. This class provides 
 * methods to use Positioning Indicator Helper service from Positioning Indicator Helper
 * Server.
 * 
 * RPosIndicatorHelper is used to create subsession with Positioning Indicator Helper Server. 
 * This class provides mechanisms for retrieving location 
 * [coordinate or reverse geo-code info], actively requesting location.
 * Before using any of these services, a connection to Positioning Indicator Helper
 * Server must first be made.
 *
 * A client can have multiple sessions connected to the Positioning Indicator Helper 
 * Server. There can be multiple subsessions opened from one session. 
 *
 *
 * @see RPosIndicatorHelperServer
 * 
 * @lib posindicatorhelper.lib 	
 *
 */

class MServerRequestCompletetionObserver
    {
public:
    /*
     * It will be used to notify RPosIndicatorHelper client that server has completed
     * the requested operation.
     * 
     * @param[out] aServiceId - Id of the operation which is requested
     * @param[out] aErrCode - Error code if operation is failed
     *                        KErrNone if operation is successful.
     */
    virtual void NotifyServerRequestCompletion(TServiceIds aServiceId, TInt aErrCode) = 0;    
    };

class RPosIndicatorHelper : public RSubSessionBase,
                            public MServerRequestCompletetionObserver
    {
public:
    
    public:    	
        /**
        * default constructior.
        */
         RPosIndicatorHelper();
        
        /**
        * destructior.
        */
         ~RPosIndicatorHelper();
        
       /**
        * Opens a subsession with Positioning Indicator Helper Server.
        *
        * A subsession must be opened before any other service can be used.
        * 
        * @panic PosIndicatorHelper EPosIndicatorHelperServerBadHandle If a session to 
        * Positioning Indicator Helper Server has not been connected.
        *
        * @param[in] aServer Reference to the Positioning Indicator Helper Server 
        * session.
        * @param[in] aPosIndInfoChangeObserver Reference to the Positioning
        * Indicator Info Change observer. Any change in position information
        * is notified by this observer.
        *
        * @return KErrNone if successful. Otherwise, Symbian standard 
        * error code is returned, such as KErrNoMemory, KErrServerBusy, etc.
        */
         TInt Open( RPosIndicatorHelperServer& aServer, 
                MPosIndInfoChangeObserver* aPosIndInfoChangeObserver );
    
       /**
        * Closes the subsession with Positioning Indicator Helper Server.
        *
        * Close() must be called when RPosIndicatorHelper subsession is no longer required. 
        * 
        * Before a subsession is closed, the client application must ensure
        * that all outstanding notification requests have been cancelled. In
        * particular, the application must issue all the appropriate Cancel 
        * requests and then wait for a confirmation that the notification has 
        * been terminated. A failure to do so results in a panic.
        * 
        * @panic PosIndicatorHelper EPosIndicatorHelperRequestsNotCancelled If client application 
        * has requests outstanding with Positioning Indicator Helper Server.
        */
         void Close();    
  
        /*
         *  Requests current position information.
         *  
         *  It will first request for current location. if it is already available
         *  it will request for reverse geo-code address.
         *  
         *  The result of the request is notified to the client through observer.
         */
         void RequestPosInfo();
         
         /**
          * Cancels the current request
          */
         void CancelCurrentRequest();

       /**
        * Symbian 2nd phase construction.
        */
        void ConstructL();

        /*
         * from MServerRequestCompletetionObserver
         */
        void NotifyServerRequestCompletion(TServiceIds aServiceId, TInt aErrCode);   
        
 private:      
         /*
          * Requests size of landmark object containing current position with
          * reverse geo-coded address.
          * 
          * It will request for the reverse geo-coded location giving the 
          * position coordinates as input. 
          * 
          * The size of the landmark object returned from reverse geo-coded
          * plugin is notified to the client through observer.
          * 
          */
         void GetAddressInfoSize();

         /*
          * Requests landmark object containing current position with
          * reverse geo-coded address.
          * 
          * It will request for the reverse geo-coded location
          * 
          * The landmark object returned from reverse geo-coded
          * plugin is notified to the client through observer.
          * 
          */
         void GetAddressL();     

        /*
         * Check Hide Coordinates feature flag.
         * Uses KLocHideCoordinates CenRep key
         */
        void CheckHideCoordinateL();
        
    private:// data     
        
       /**
        * Pointer to client requestor.  
        * Own.
        */
        CPosIndHelperClientRequester* iClientRequester;     
        
        /**
         * It is used to request position details from server
         */
        TPositionSatelliteInfo iPositionInfo;
        
        /**
         * It is used to request size of the landmark object from server
         */
        TInt iSize;
        
        /**
         * It is used to request the landmark object from the server
         * Owns
         */
        HBufC8* iLmkBuf;
        
        /**
         * It is used to sent the landmark object to the observer
         * Owns
         */
        CPosLandmark * iLandmark;
        
        /**
         * It is used to notify PosIndicatorInfoPrivate that new position information
         * new position information is available.
         * 
         * Array of observers are not stored here as multiple client requests are actually
         * handled by signal-slot mechanism in PosIndicatorInfoPrivate
         * Does not own
         */
        MPosIndInfoChangeObserver* iPosIndInfoChangeObserver;
        
        /*
         * Holds value of Hide Coordinates CenRep key
         */
        TInt iCoordinateSettingValue;
    };
#endif // POS_INDICATOR_HELPER_H
