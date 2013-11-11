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
* Description:  Harvester client header
*
*/


#ifndef __HARVESTER_CLIENT_H__
#define __HARVESTER_CLIENT_H__

#include <e32base.h>

#include <mdccommon.h>
#include <mdeconstants.h>
#include <harvestereventenum.h>

// forward declarations
class CHarvesterClientAO;
class CHarvesterSessionAsyncHandler;
class CHarvesterEventObserverAO;
class MHarvesterEventObserver;
class CHarvesterRequestActive;
class CHarvesterRequestQueue;
class MHarvesterSessionObserver;
class CHarvesterSessionWatcher;

// default event interval for MHarvesterEventObserver
const TInt KHarvesterEventInterval = 20;

/**
 * Observer interface to inform when fast harvesting is completed
 * with the HarvestingComplete callback.
 *
 * Example of MHarvestObserver and RHarvesterClient::HarvestFile usage:
 *
 * Client application (like Camera) which wants to observe the completion of 
 * harvesting requests (issued with RHarvesterClient::Harvestile method) needs
 * implement the interface MHarvestObserver::HarvestingComplete. Callback 
 * HarvestingComplete provides the name of the harvested file (aURI) and also possible 
 * error code (aError).
 *
 * class CHarvestingObserver : public MHarvestObserver
 *     {
 *     void HarvestingComplete( TDesC& aURI, TInt aError ); // from MHarvestObserver
 *     void IssueHarvestingRequests();
 * 
 *     RHarvesterClient iHClient;	
 *     }
 *
 * void CHarvestObserver::ConstructL()
 *     {
 *     // connecting to Harvester server
 *	   iHClient.Connect();
 *     }
 * 
 * void CHarvestObserver::IssueHarvestingRequests()
 *     {
 *     // new harvesting request data
 * 	   _LIT( KNewFile, "C:\\Data\\ThisIsANewFile1.jpg" );
 *     RArray<TItemId> defaultPhotoAlbums;
 *	   defaultPhotoAlbums.Append( 123 );
 *     defaultPhotoAlbums.Append( 456 );
 *     defaultPhotoAlbums.Append( 789 );
 *
 *     // setting this class as the observer for the fast harvesting calls
 *      iHClient.SetObserver(this);
 *        
 *     // issué new harvesting request and requesting location data to be harvested
 *     iHClient.HarvestFile( KNewFile, defaultPhotoAlbums, ETrue );   
 *     }
 *
 * void CHarvestObserver::HarvestingComplete( TDesC& aURI, TInt aError )
 *     {
 *     _LIT( KExpectedFile, "C:\\Data\\ThisIsANewFile1.jpg" );
 *
 *     // Checking if an error occurred and if this was the expected file
 *     if ((aError == KErrNone) && (aURI.CompareC(KExpectedFile) == 0))
 *         {
 *         // do something
 *         }
 *     }
 *
 */
class MHarvestObserver
	{
public:
    /**
     * Callback to inform when fast harvesting of a file is complete.
     * 
     * @param aURI    URI of the harvested file.
     * @param aError  Error code of the fast harvesting. <code>KErrNone</code> is
     *                expected if fast harvesting succeeded. Otherwise some
     *                system wide error code. 
     *
     */
	virtual void HarvestingComplete( TDesC& aURI, TInt aError ) = 0;
	};

/**
 * Observer interface to inform about events that happen inside the observer framework
 *
 * Example of MHarvesterEventObserver, RHarvesterClient::AddHarvesterEventObserver and
 * RHarvesterClient::RemoveHarvesterEventObserver usage:
 *
 * void CHarvestObserver::ConstructL()
 *     {
 *     // iHClient is instance of RHarvesterClient
 *	   iHClient.Connect();
 *
 *     // Listen to placeholder changes to "full" metadata objects - request notification
 *     // after 100 items have changed.
 *     TInt err = iHClient.AddHarvesterEventObserver( *this, EHEObserverTypePlaceholder, 100 );
 *     if (err == KErrNone)
 *         {
 *         // do something now that event observer is setup
 *         }
 *     }
 *
 * void CHarvestObserver::HarvestingUpdated( HarvesterEventObserverType aHEObserverType, 
 *			HarvesterEventState aHarvesterEventState, TInt aItemsLeft )
 *     {
 *     // Check the observer notification type
 *     if (aHEObserverType == EHEObserverTypePlaceholder)
 *         {
 *         // Checking if there are more than 1000 items left to process 
 *         if (aItemsLeft > 1000)
 *             {
 *             // do something since there are some many items still placeholders
 *             }
 *         if (aItemsLeft == 0)
 *             {
	           // all placeholders changed to "full" objects - do something...
 *             }
 *         }   
 *     }
 *
 * CHarvestObserver::~CHarvestObserver()
 *     {
 *     // Removing the observer in destructor
 *     iHClient.RemoveHarvesterEventObserver( *this );   
 *     }
 */


class MHarvesterEventObserver
	{
public:
    /**
     * Callback interface which informs about harvesting changes based on the observers
     * that the end user has setup/registered.
     * 
     * @param aHEObserverType       Defines the observer type to which this events is related 
     * @param aHarvesterEventState  Event that occurred
     * @param aItemsLeft            Number of items left regarding the harvesting procedure (defined
     *                              by aHEObserverType)
     * @see RHarvesterClient::AddHarvesterEventObserver
     * @see RHarvesterClient::RemoveHarvesterEventObserver
     */
	virtual void HarvestingUpdated( 
			HarvesterEventObserverType aHEObserverType, 
			HarvesterEventState aHarvesterEventState,
			TInt aItemsLeft ) = 0;
	};

class MHarvesterSessionObserver
    {
public:

    /**
     * Called to notify the observer that harvester server has been terminated,
     * thus harvester server sessions created are now invalid and new connection
     * needs to be established
     * NOTE! Cliens still have to call Close() on the session to release client side
     * resources.
     */
    virtual void HarvesterServerTerminated() = 0;
    };

/**
 * Harvester client session class which provides also means to:
 * - Pause/resume the Harvesting framework
 * - Fast harvest files and observe when they have been harvested
 * - Setup observers to observe the harvesting progress
 */
NONSHARABLE_CLASS( RHarvesterClient ) : public RSessionBase
	{
	friend class CHarvesterSessionAsyncHandler;
	friend class CHarvesterEventObserverAO;

	public:
		
        /**
         * Constructor
         */
        IMPORT_C RHarvesterClient();

        /**
         * Public method to connect Harvester server.
         * 
         * @return Symbian OS error code.
         */
        IMPORT_C TInt Connect();
         
        /**
         * Public method to pause the operation
         * of the Harvester framework.
         *
         * @return  Symbian OS error code.
         */
        IMPORT_C TInt Pause();

        /**
         * Public method to resume the operation
         * of the Harvester framework.
         *
         * @return  Symbian OS error code.
         */    
        IMPORT_C TInt Resume();

        /**
         * Public method to close session to Harvester server.
         */
        IMPORT_C void Close();
            
        /**
         * Public method to set observer for fast harvesting.
         * Only one observer is currently supported.
         *
         * @param aObserver  Pointer to the observer
         */     
        IMPORT_C void SetObserver( MHarvestObserver* aObserver);

        /**
         * Public method to remove observer for fast harvesting.
         *
         * @param aObserver  Pointer to the observer
         */     
        IMPORT_C void RemoveObserver( MHarvestObserver* aObserver);
        
        /**
         * Public method for doing fast harvesting. These files
         * are handled first before the ones that are in the harvesting 
         * queue inside the Harvesting framework.
         *
         * @param aURI          Uri of the file to be harvested
         * @param aAlbumIds     Array of album id's that the harvested file 
         *                      will be included in
         * @param aAddLocation  Should location information be added to item
         */ 
    	IMPORT_C void HarvestFile( const TDesC& aURI, RArray<TItemId> &aAlbumIds, TBool aAddLocation );
        
        /**
         * Method for adding observer for harvester framework events
         *
         * @param aHarvesterEventObserver  class to implement MHarvesterEventObserver interface
         * @param aHEObserverType          Harvester event observer type (EPlaceholderState, 
         *                                 EMMCState, EOverallState)
         * @param aEventInterval           Interval of harvester events as item amount
         * 
         * @return  Symbian OS wide error code.
         */
        IMPORT_C TInt AddHarvesterEventObserver( 
        		MHarvesterEventObserver& aHarvesterEventObserver, 
        		TInt aHEObserverType,
        		TInt aEventInterval = KHarvesterEventInterval );
        
        /**
         * Method for removing observer for harvester framework events. Please note that
         * removing any of the event observers will remove all observers which are registered
         * by the given parameter aHarvesterEventObserver (= observer class).
         *
         * @param aHarvesterEventObserver class to implement MHarvesterEventObserver interface
         *
         * @return  Symbian OS wide error code.
         */
        IMPORT_C TInt RemoveHarvesterEventObserver( MHarvesterEventObserver& aHarvesterEventObserver );
        
		/**
         * Public method for doing fast harvesting. These files
         * are handled first before the ones that are in the harvesting 
         * queue. 
         *
         * @param aURI          Uri of the file to be harvested
         * @param aAlbumIds     Array of album id's that the harvested file will be included in
         * @param aAddLocation  Should location information be added to image
		 * @param aUid          Uid of the originating application for the object
         */ 
    	IMPORT_C void HarvestFileWithUID( const TDesC& aURI, 
		                                  RArray<TItemId> &aAlbumIds, 
										  TBool aAddLocation,
										  TUid aUid );

        /**
         * Public method to set observer for notifications about harvester server termination.
         * Only one observer at a time is currently supported.
         *
         * @param aObserver  Pointer to the observer
         */     
    	IMPORT_C void AddSessionObserverL( MHarvesterSessionObserver& aObserver  );

        /**
         * Public method to remove harvester session observer
         */     
        IMPORT_C void RemoveSessionObserver();
    	
        /**
         * Requests a harvest complete event from harvester server.
		 * On return, aURI is the URI of the harvested file.
         *
         * @param aURI  Harvested image uri what server
         * @param aStatus    Reference to a request indicator
         */ 
        void RegisterHarvestComplete( TDes& aURI, TRequestStatus& aStatus );
        
        /**
         * Cancels the harvest complete notification request.
         */ 
        void UnregisterHarvestComplete( );
        
        /**
         * Private method for doing fast harvesting. These files
         * are handled first before the ones that are in the harvesting 
         * queue.
         * 
         * @param aService  Service request which to execute on server side
         * @param aArgs     Parameters for harvesting
         * @param aStatus   Status of the asunchronous call
         */ 
        void HarvestFile( TInt& aService, TIpcArgs& aArgs, TRequestStatus& aStatus  );

        /**
         * Restricted method for doing fast harvesting. These files
         * are handled first before the ones that are in the harvesting 
         * queue. This version forces the file to be harvested immidiately.
         * 
         * @param aService  Service request which to execute on server side
         * @param aArgs     Parameters for harvesting
         */ 
        void ForceHarvestFile( TInt& aService, TIpcArgs& aArgs );
        
	private: 

	    /**
	     * Observer of the class
	     */ 
	     MHarvestObserver* iObserver;
	
        /**
         * Private method for version.
         */      
        TVersion Version() const;

        /**
         * Harvester client active object.
         */    
        CHarvesterClientAO* iHarvesterClientAO;
        
        /** 
         * Pointer to Harvester event observer active object.
         */
        CHarvesterEventObserverAO* iHEO;
        
        /**
         * Request queue processor.
         */
        CHarvesterRequestQueue* iRequestQueue;
        
        /**
         * Harvester session observer AO.
         */
        CHarvesterSessionWatcher* iSessionWatcher;
    	};

#endif // __HARVESTER_CLIENT_H__
