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
* Description:  Implements queue that the Harvester server reads*
*/

#ifndef __CHARVESTERQUEUE__
#define __CHARVESTERQUEUE__

#include <monitorplugin.h>
#include "harvestermediaidutil.h"
#include "harvestereventmanager.h"

// FORWARD DECLARATION
class CHarvesterAO;
class CHarvesterBlacklist;
class CRestoreWatcher;

class CHarvesterQueue : public CBase, 
                        public MMonitorPluginObserver
    {

    public:
    
        /**
         * Constructs a new harvester queue
         *
         * @return  Harvester server queue implementation
         */
        static CHarvesterQueue* NewL( CHarvesterAO* aHarvesterAO,
        		CHarvesterBlacklist* aBlacklist );

        /**
         * Constructs a new harvester queue implementation and leaves the queue implementation
         * in the cleanup stack.
         *
         * @return  Harvester server queue implementation
         */
        static CHarvesterQueue* NewLC( CHarvesterAO* aHarvesterAO,
        		CHarvesterBlacklist* aBlacklist );

        /**
         * Destructor
         */         
        virtual ~CHarvesterQueue();

        /**
         * ConstructL
         */     
        void ConstructL();
       
        /**
         * ItemsInQueue
         * @return Number of items in the server queue
         */ 
        TInt ItemsInQueue();
        
        /**
         * Gets next item in the Harvester server queue
         * @returns The next item from the queue (FIFO)
         */         
        CHarvesterData* GetNextItem();
        
        /**
         * Adds a item to the end of the queue
         * @param aItem  Item to be added to the queue
         */         
        void Append( CHarvesterData* aItem );
        
        /**
         * Derived from MMonitorPluginObserver - for getting new items to harvest
         * @param aHarvesterData  data type which contains information needed by harvester
         */   
        void MonitorEvent( CHarvesterData* aHarvesterData );
        
        /**
         * Derived from MMonitorPluginObserver - for getting new items to harvest
         * @param aHarvesterData  array of data type which contains information needed by harvester
         */   
        void MonitorEvent( RPointerArray<CHarvesterData>& aHarvesterData );
        
        /**
         * Remove items from harvesting queue
         */
        TUint RemoveItems(TUint32 aMediaId);
        
    private:
        /**
         * Constructor
         */             
        CHarvesterQueue( CHarvesterAO* aHarvesterAO, CHarvesterBlacklist* aBlacklist );
        
    private:
   
        /**
        * Array which holds the details of the available plugins - filled during startup
        */
        RPointerArray<CHarvesterData> iItemQueue;
     
        /** Pointer to harvester active object */
        CHarvesterAO* iHarvesterAO;

        /** Pointer to harvester blacklisting */
        CHarvesterBlacklist* iBlacklist;

        RFs iFs;
        
        TVolumeInfo iVolInfo;
        
        // not own
        CHarvesterMediaIdUtil* iMediaIdUtil;
        
        CHarvesterEventManager* iHEM;
    };

#endif
