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

#ifndef __FILEMONITORAO_H__
#define __FILEMONITORAO_H__

#include <e32base.h>

#include "mdsfileserverpluginclient.h"
#include "harvesterdata.h"

// FORWARD DECLARATION
class CMdEHarvesterSession;
class CProcessOriginMapper;
class CHarvesterPluginFactory;
class CFileEventHandlerAO;

class CFileMonitorAO : public CActive
    {
    public:
        /**
        * Creates and constructs a new instance of CFileMonitorAO.
        *
        * @return A pointer to the new instance of CFileMonitorAO
        */
        static CFileMonitorAO* NewL();
        
        /**
        * Destructor
        */
        virtual ~CFileMonitorAO();
        
        /**
        * Starts monitoring file system for file creations, modifications and deletions
        * in the specified path.
        *
        * @param aObserver  All events are notified via aObserver.
        * @param aPath  The path to monitor.
        * @param aHarvesterPluginFactory  A pointer to the harvester plugin factory.
        * @return ETrue if success, EFalse if not
        */
        TBool StartMonitoring( MMonitorPluginObserver& aObserver,
        		CMdESession* aMdeSession, const TDesC& aPath,
        		CHarvesterPluginFactory* aHarvesterPluginFactory );
        
        /**
        * Stops monitoring.
        *
        * @return ETrue if success, EFalse if not
        */
        TBool StopMonitoring();
        
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
         * Set file event caching on/off
         */
        void SetCachingStatus( TBool aCachingStatus );
        
        
    private:
        /**
        * C++ constructor - not exported;
        * implicitly called from NewL()
        */
        CFileMonitorAO();
        
        /**
        * 2nd phase construction, called by NewL()
        */
        void ConstructL();
        
        /**
        * Starts file server to notify us about file changes.
        */
        void StartNotify();
        
        /**
         * 
         */
        void ResetMdsFSPStatus();
        
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
         * @var Flag to signal if iFs is connected.
         */
        TBool iFsConnectOk;

        /**
         * 
         */
        RMdsFSPEngine iEngine;
        
        /**
         * 
         */
        TMdsFSPStatusPckg iStatusPckg;

        /**
         * @var Mde client session pointer.
         */
        CMdEHarvesterSession* iMdeSession;
        
        /**
         * @var Harvester plugin factory pointer.
         */
        CHarvesterPluginFactory* iHarvesterPluginFactory;
        
        CFileEventHandlerAO* iFileEventHandler;
    };

#endif // __FILEMONITORAO_H__
