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
* Description:  Monitors file creations, modifications and deletions.*
*/


#ifndef __FILEEVENTHANDLERAO_H__
#define __FILEEVENTHANDLERAO_H__

#include <e32base.h>
#include <f32file.h>

#include "movetimer.h"
#include "mdsfileserverpluginclient.h"
#include <harvesterdata.h>

// FORWARD DECLARATION
class CMdEHarvesterSession;
class CProcessOriginMapper;
class CMdEObject;
class CMdEPropertyDef;
class CFolderRenamer;
class CHarvesterPluginFactory;
class CMdsFSPQueueItem;

class CFileEventHandlerAO : public CActive, public MMoveTimerObserver
    {
    public:
    	enum TRequest
    		{
    		ERequestIdle,
    		ERequestProcessing
    		};
    		
        /**
        * Creates and constructs a new instance of CFileEventHandlerAO.
        *
        * @return A pointer to the new instance of CFileEventHandlerAO
        */
        static CFileEventHandlerAO* NewL( MMonitorPluginObserver& aObserver,
        		CMdEHarvesterSession* aMdeSession,
        		CHarvesterPluginFactory* aHarvesterPluginFactory );
        
        /**
        * Destructor
        */
        virtual ~CFileEventHandlerAO();
        
        /**
        * Inherited from CActive. This method will be called on file server notifying.
        */
        void RunL();
        
        /**
        * Handles a leave occurring in the request completion event handler RunL().
        *
        * @param aError  An error code.
        * @return An error code.
        */
        TInt RunError( TInt aError );
        
        /**
        * Cancels file server notifying. Inherited from CActive.
        */
        void DoCancel();
        
        /**
         * Gets a reference to process / origin mapper, which
         * can be used to register and match which process ids
         * with a mde object's Origin value.
         * @return Handle to File monitor's CProcessOriginMapper.
         */
        CProcessOriginMapper& Mapper();
        
        /**
         * Notifies file monitor about new events
         */
        void HandleNotificationL(TMdsFSPStatus &aEvent);         
        
        /**
         * Notifies file monitor about multiple delete events
         */
        void HandleMultideletionL( CArrayFixSeg< TMdsFSPStatus >* aEvents );     
        
        /**
         * @Returns filemonitors mde session
         * @return mde session object
         */
        CMdEHarvesterSession * MdeHarvesterSession();          
        
        
        void AddToQueueL(TMdsFSPStatus &aEvent);
        
        void SetCachingStatus( TBool aCachingStatus );
        
    private:
        /**
        * C++ constructor - not exported;
        * implicitly called from NewL()
        */
        CFileEventHandlerAO();
        
        /**
        * 2nd phase construction, called by NewL()
        */
        void ConstructL( MMonitorPluginObserver& aObserver,
        		CMdEHarvesterSession* aMdeSession,
        		CHarvesterPluginFactory* aHarvesterPluginFactory );
        
        
        /**
         * 
         */
        void ResetEvent();
        
        void NotMoveEvent(RPointerArray<CHarvesterData>& aHDArray);
        
        /**
         * 
         */
        void RenameToMDEL( const TDesC& aOldUrl, const TDesC& aNewUrl, TOrigin aOrigin );
        
        /**
         * 
         */
        void ReplaceL( const TDesC& aOldUrl, const TDesC& aNewUrl, TOrigin aOrigin, TBool aFastHarvest );
        
        /**
         * 
         */
        void ModifyL( const TDesC& aUrl, TOrigin aOrigin, TBool aFastHarvest );
        
        /**
         * 
         */
        void DeleteFromMDEL( const TDesC& aUrl );
        
        /**
         * 
         */
        void MultiDeleteFromMDEL( const RPointerArray<TDesC>& aUrls );
        
        /**
         * 
         */
        void FormatL( TUint32 aOldMediaId, TBool aSubClose );
        
        /**
         * 
         */
        TOrigin OriginFromMdEObjectL( CMdEObject& aObject );

        /**
         * 
         */
        void OriginToMdEObjectL( CMdEObject& aObject, TOrigin aOrigin );
        
        
        void SetNextRequest( TRequest aRequest );
        
        void ReadCacheSizeFromCenrepL();

        /**
        * Update MDS objects data if needed
        */
       void SetTitleL( CMdEObject* aOldObject, const TDesC& aNewUrl );
       
       void SetModifiedTimeL( CMdEObject* aOldObject, const TDesC& aNewUrl );
       
       void CheckDefaultFolderL( CMdEObject* aOldObject );
        
    private:
        
        /**
         * @var An observer class to notify about file creations, modifications and deletions.
         */
    	MMonitorPluginObserver* iObserver;
        
        /**
         * @var A file server.
         */
        RFs iFs;

        /**
         * 
         */
        TMdsFSPStatusPckg iStatusPckg;

        /**
         * @var Mde client session pointer.
         */
        CMdEHarvesterSession* iMdeSession;
        
        /**
         * Process id / origin mapper.
         */
        CProcessOriginMapper* iMapper;
        
        /**
         * MoveTimer to check if move is coming...
         */
        CMoveTimer* iMoveTimer;
        
        /**
         * Active object which is used to handle folder rename events
         * 
         */
        CFolderRenamer *iFolderRenamer;
        
        /**
         * @var Harvester plugin factory pointer.
         */
        CHarvesterPluginFactory* iHarvesterPluginFactory;
        
        RPointerArray<CMdsFSPQueueItem> iQueue;
        
        TRequest iNextRequest;
        
        TBool iCacheEvents;
        
        TMdsFSPStatus iEvent;
        
        CArrayFixSeg< TMdsFSPStatus >* iEventArray;
        RPointerArray<TDesC> iUriArray;
        
        TInt iCacheSize;
        
        CMdEPropertyDef* iOriginPropertyDef;
        CMdEPropertyDef* iTitlePropertyDef;
        CMdEPropertyDef* iTimePropertyDef;
        CMdEPropertyDef* iDefaultFolderPropertyDef;
        
        HBufC* iPhoneImagesPath;
        HBufC* iMmcImagesPath;
        
        HBufC* iPhoneVideosPath;
        HBufC* iMmcVideosPath;
        
        HBufC* iPhoneSoundsPath;
        HBufC* iMmcSoundsPath;
    };

#endif // __FILEEVENTHANDLERAO_H__
