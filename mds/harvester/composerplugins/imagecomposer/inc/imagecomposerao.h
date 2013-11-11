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
* Description:  Composer image active object
*
*/


#include <e32msgqueue.h>

#include "mdeobject.h"
#include "mdeobjectquery.h"
#include "harvesterexifutil.h"
#include "mdeharvestersession.h"

#ifndef __CIMAGECOMPOSERAO_H__
#define __CIMAGECOMPOSERAO_H__

_LIT( KImageComposerPanic, "CImageComposerAO" );

class CMdERelationQuery;

class CImageComposerAO : public CActive, public MMdEQueryObserver
    {
    public:
    
        enum TRequestType
            {
            ERequestReady = 0,
            ERequestGetObject,
            ERequestCompose
            };

        /**
        * Construction
        */
        static CImageComposerAO* NewL();

        /**
        * Destruction
        */
        virtual ~CImageComposerAO();
        
        /**
         * Adds new item to composing queue.
         * @param aItems  Items to add to the queue.
         * @param aForce  Force composing to items in aItems array
         */
        void AddToQueue( const RArray<TItemId>& aItems, TBool aForce );

        /**
         * Checks if composing is completed.
         * @return ETrue, if complete.
         */
        TBool IsComposingComplete();

        /**
         * Sets mde client session to be used by this composer.
         * @param aSession  Mde client session.
         */
        void SetSession( CMdESession* aSession );
        
        /**
         * Remove MdE client session used by this composer.
         */
        void RemoveSession();
        
        /**
         * Writes location data to Exif.
         * @param aMdeObject  Mde object of the image.
         * @param aLocationObject  Related mde location object.
         */
        void DoWriteExifL( CMdEObject* aMdEObject, CMdEObject* aLocationObject );
        
        
        
        
        
    public: // from MMdEQueryObserver

	    /**
	     * Callback to handle new results received from a query.
	     * From MMdEQueryObserver.
	     */
        virtual void HandleQueryNewResults( CMdEQuery& aQuery, TInt aFirstNewItemIndex,
            TInt aNewItemCount );

	    /**
	     * Callback to handle new results received from a query.
	     * From MMdEQueryObserver.
	     */
        virtual void HandleQueryNewResults( CMdEQuery& aQuery, TInt aNewObjectItemCount, 
            TInt aNewRelationItemCount, TInt aNewEventItemCount );

	    /**
	     * Callback to handle all results of a completed query.
	     * From MMdEQueryObserver.
	     * @param aQuery  Completed query
	     * @param aError  Error code, KErrNone on success.
	     */
        virtual void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );

    protected:

    	/**
    	 * RunL
    	 * From CActive.
    	 */
        virtual void RunL();

    	/**
    	 * DoCancel
    	 * From CActive.
    	 */
        virtual void DoCancel();

    	/**
    	 * RunError. Error handler in case RunL() leaves.
    	 * From CActive.
    	 * @param aError  Leave code from RunL.
    	 * @return  Error code. KErrNone if error was handled.
    	 */
        virtual TInt RunError( TInt aError );

    private:

        /**
        * Private constructor
        */    
        CImageComposerAO();
        
        /**
        * 2nd phase construction
        */    
        void ConstructL(); // second-phase constructor
        
        /**
         * Gets an object from MdE DB.
         * Leave on queue errors.
         * Leaves with KErrNotFound, if object is not found.
         * 
         * @param Returned value of current composing request's object ID. 
         *        Value is changed when item is read from composing queue.
         */
        void GetObjectFromMdeL(TItemId& aMdEObjectId);
        
        /**
         * Writes (=composes) new metadata to a image file.
         */
        void ComposeL();
        
        /**
         * SetNextRequest
         * Sets next state for this active object's RunL.
         * @param aNextRequest  State to set.
         */
        void SetNextRequest( TRequestType aNextRequest );
        
        /**
         * @brief Starts the write of GPS tags to a image file.
         * 
         * Starts a query to search for related location data in mde.
         * If found, HandleQueryCompleted will start adding found info
         * to image file.
         * 
         * @param aObjectId  Object to whichs add GPS tags.
         * 
         * @see HandleQueryCompleted
         * @see StartWritingGPSTagsL
         * @see DoWriteExifL
         */
        void WriteGPSTagsL( TItemId aObjectId );
        
        /**
         * Checks if given mime type is JPEG image.
         * @param aMimeType  a mime type string.
         * @return ETrue if mime type is image/jpeg.
         */
        static TBool IsJpeg( const TDesC& aMimeType );
        
        
        /**
         * Starts writing GPS tags to image file.
         * @param aQuery  Mde query which contains results for related
         *                location object query.
         */
        void StartWritingGPSTagsL( CMdEQuery& aQuery );

        /**
         * Converts a TDateTime object's date to a Exif GPS date string.
         *
         * @param aTime  TDateTime object to extract the date from.
         * @param aTgtDes  Target descriptor for the string.
         */
        void TimeToGpsDateStringL( TTime aTime, TDes& aTgtDes );

        /**
         * Converts a TTime object's time to a Exif GPS time string.
         *
         * @param aTime  TDateTime object to extract the time from.
         * @param aTgtDes  Target descriptor for the string.
         */
        void TimeToGpsTimeStringL( TDateTime aTime, TDes8& aTgtDes );


        /**
         * CompareItemIds
         */
		static TInt CompareTItemIds( const TItemId& aLeft, const TItemId& aRight )
			{
			return aLeft - aRight;
			}

    private:

    	/** Pointer to a mde object */
        CMdEObject* iMdeObject;
        
        /** Pointer to mde session to use */
        CMdESession* iSession;
        
        CMdEHarvesterSession* iMdEHarvesterSession;
        
        /** Next state of this active object's RunL() */
        TRequestType iNextRequest;
        
        /** Queue of items to compose */
        RArray<TItemId> iItemQueue;
        
        /** A relation query used to seach for related location objects */
        CMdERelationQuery* iRelationQuery;
        
        /** Mde default namespace. NOT OWN. */
        CMdENamespaceDef* iDefaultNamespace;
        
        /** Image Object Def. NOT OWN. */
        CMdEObjectDef* iImageObjectDef;

        /** "Base" Object Def. NOT OWN. */
        CMdEObjectDef* iObjectDef;

        /** Location Object Def. NOT OWN. */
        CMdEObjectDef* iLocationObjectDef;
        
        /**
         * Array used to skip new coming id after explicit change of
         * MdE object in composer.
         */
        RArray<TItemId> iNextItemsSkip;
        
        /**
         * Array of item id's which will be forced to be composed
         * No timestamp checking will be done for these objects
         */
        RArray<TItemId> iForceObjectIds;
        
        CHarvesterExifUtil* iExifUtil;
        
        RFs iFs;
        
        TBool iFastModeEnabled;
    };

#endif // __CIMAGECOMPOSERAO_H__
