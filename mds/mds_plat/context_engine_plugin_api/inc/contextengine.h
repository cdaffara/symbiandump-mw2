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
* Description:  An engine to collect context related metadata.
*
*/

// Statically linked dll

#ifndef CCONTEXTENGINE_H
#define CCONTEXTENGINE_H 

#include <e32cons.h>
#include <e32cmn.h>

//forward declarations

class CMdEObject; 
class CContextEngineAO;
class CContextSnapshotItem;
class CMdESession;
class CHarvesterData;

/**
* An interface for observer that wants to be informed about
* context engine initialization completion and
* snapshot completion.
*/
class MContextInitializationObserver
    {
    public:
        /**
         * Pure virtual method. Intended to inform about
         * context engine initialization completion.
         *
         * @param aErrorCode  Error code for error occurred during initialization 
         */
        virtual void ContextInitializationStatus( TInt aErrorCode ) = 0;
    };

/**
* An interface for observers that want to be informed about context snapshot completion.
*/
class MContextSnapshotObserver
    {
    public:
        /**
         * Pure virtual method. Intended to inform about context snapshot taking completion.
         *
         * @param aErrorCode  Error code for error occurred while taking the snapshot.
         */
        virtual void ContextSnapshotStatus( CHarvesterData* aHD ) = 0;
    };

/**
* An engine to collect context metadata with available context plugins.
* CContextEngineAO is used to do the actual work.
* This class is implemented as a Singleton.
*/
NONSHARABLE_CLASS( CContextEngine ) : public CBase, public MContextSnapshotObserver
      {
    public:

        /**
         * Destroys the implementation. This is virtual method
         * and this class is not intended for derivation, so not exported.
         */
        virtual ~CContextEngine();

        /**
         * Gets an instance to CContextEngine singleton.
         * A new object is created if needed.
         * If an observer object to notify is given, context plugins are initialized
         * asyncronously and observer is notified when ready.
         *
         * @param aObserver  an interface class for callback or NULL
         * @return A pointer to the context engine implementation
         */ 
        IMPORT_C static CContextEngine* GetInstanceL( MContextInitializationObserver* aObserver = NULL );

        /**
         * Release an instance of this singleton.
         */
        IMPORT_C void ReleaseInstance();

        /**
         * Ignores the reference count and destroys this singleton.
         * THINK BEFORE USING! Should be used only instead of reference counting
         * if clients are not calling ReleaseInstance() (which they should).
         */
        IMPORT_C void Destroy();

        /**
         * Set a pointer to MdESession. The pointer is given forward to context plugins.
         * Session must be set in order to successfully harvest context data.
         *
         * @param aSession Session pointer to set.
         */
        IMPORT_C void SetMdeSession( CMdESession* aSession );

        /**
         * Context snapshot. This takes the snapshot using plug-ins.
         *
         * @param aObserver  an interface class for callback.
         * @param aMetadataObject  MdE object the snapshot is taken to.
         */
        IMPORT_C void ContextSnapshot( MContextSnapshotObserver& aObserver,
            CHarvesterData& aHD );
        
        /**
         * Context snapshot. This takes the snapshot using plug-ins.
         *
         * @param aObserver  an interface class for callback.
         * @param aMetadataObjectArray  An array of MdE objects the snapshot is taken to.
         */
        IMPORT_C void ContextSnapshot( MContextSnapshotObserver& aObserver,
            RPointerArray<CHarvesterData>& aMetadataObjectArray );
        
        /**
         * Method used to clarify amount of plug-ins.
         *
         * @return Amount of plug-ins. Intended for test purposes only.
         */
        IMPORT_C TInt PluginCount();

        /**
         * From MContextEngineObserver.
         * Method is called by CContextEngineAO when a context snapshot is finished
         * or an error has occured.
         *
         * @param aErrorCode  Error code for error occurred during snapshot.
         */
        void ContextSnapshotStatus( CHarvesterData* aHD );

    private:

        /**
         * C++ constructor - not exported;
         * implicitly called from GetInstance()
         *
         */
        CContextEngine();
        
        /**
         * 2nd phase construction, called by GetInstance()
         *
         * @param aObserver object/callback to notify when initialization is ready.
         */
        void ConstructL( MContextInitializationObserver* aObserver );

        /**
         * Add a new observer and metadata items to a queue.
         * These observers need to be informed when a snapshot is ready
         * and items need a context snapshot.
         *
         * @param aItem  An item to add.
         */
        void QueueSnapshotItem( CContextSnapshotItem* aItem );

    private: // data

        /**
         * This active object is used to handle queued snapshot requests.
         */
        CContextEngineAO* iContextEngineAO;

        /**
         * Array of observers and related metadata objects that need
         * a context snapshot.
         * Related observer is informed about snapshot completion.
         */
        RPointerArray<CContextSnapshotItem> iSnapshotQueue;
        
        /**
         * In case array of objects for snapshot were passed,
         * variable to store the amount of items to be processed.
         */
        TInt iArrayCount;
        
        /**
         * In case array of objects for snapshot were passed,
         * variable to store the amount of items that have been processed.
         */
        TInt iProcessedArrayCount;
    };


/**
* A helper class to store this singleton's static data.
*/
NONSHARABLE_CLASS( CContextEngineStaticData ): public CBase
    {
    friend class CContextEngine;
        
    public:

        CContextEngineStaticData( CContextEngine* aContextEngine ) : iContextEngine(aContextEngine)
            {
            iRefCount = 1;
            }

        virtual ~CContextEngineStaticData()
            {
            delete iContextEngine;
            }

    protected:

        CContextEngine* iContextEngine;
        TInt iRefCount;
    };


#endif // CCONTEXTENGINE_H
