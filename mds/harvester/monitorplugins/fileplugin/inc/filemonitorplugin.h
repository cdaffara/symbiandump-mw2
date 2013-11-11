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

#ifndef __FILEMONITORPLUGIN_H__
#define __FILEMONITORPLUGIN_H__

#include <e32base.h>

#include "monitorplugin.h"
#include "filemonitorao.h"
#include "processoriginmapperinterface.h"

class CFileMonitorPlugin : public CMonitorPlugin,
                           public MProcessOriginMapperInterface
  	{
	public:
		/**
		* Creates and constructs a new instance of CFileMonitorPlugin.
		*
		* @return A pointer to the new instance of CFileMonitorPlugin
		*/
		static CFileMonitorPlugin* NewL();
		
		/**
		* Destructor
		*/
		virtual ~CFileMonitorPlugin();
		
		/**
		* Starts monitoring file system for file creations, modifications and deletions
		* in the specified path.
		*
		* @param aObserver  All events are notified via the aObserver.
		* @param aMdEClient  A pointer to MdE client.
		* @param aCtxEngine  A pointer to context engine.
		* @param aPluginFactory  A pointer to harvester plugin factory.
		* @return ETrue if success, EFalse if not.
		*/
		TBool StartMonitoring( MMonitorPluginObserver& aObserver, CMdESession* aMdEClient, 
				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory );
		
		/**
		* Stops monitoring.
		*
		* @return ETrue if success, EFalse if not.
		*/
		TBool StopMonitoring();
		
		/**
		* Resumes paused monitoring.
		*
		* @param aObserver  All events are notified via the aObserver.
		* @param aMdEClient  A pointer to MdE client.
		* @param aCtxEngine  A pointer to context engine.
		* @param aPluginFactory  A pointer to harvester plugin factory.
		* @return ETrue if success, EFalse if not.
		*/
		TBool ResumeMonitoring( MMonitorPluginObserver& aObserver, CMdESession* aMdEClient,
				CContextEngine* aCtxEngine, CHarvesterPluginFactory* aHarvesterPluginFactory );
		
		/**
		* Pauses monitoring.
		*
		* @return ETrue if success, EFalse if not.
		*/
		TBool PauseMonitoring();
		
        /**
         * Add a process to the list.
         * From MProcessOriginMapperInterface.
         * @param aProcessUid  Uid of the process to add.
         * @param aOrigin      Origin to which this process will mapped to.
         */
        void RegisterProcessL( const TUid& aProcessId,
                               const TOrigin& aOrigin );
        
        /**
         * Remove a process from the list.
         * From MProcessOriginMapperInterface.
         * @param aProcessId  Uid of the process to remove.
         */
        void UnregisterProcessL( const TUid& aProcessId );

    private:
		 
		/**
		* C++ constructor - not exported;
		* implicitly called from NewL()
		*/
		CFileMonitorPlugin();
		
		/**
		* 2nd phase construction, called by NewL()
		*/
		void ConstructL();
		
	private:
		
		/**
		* A pointer to CFileMonitorAO.
		*/
		CFileMonitorAO* iFileMonitor;
	};

#endif // __FILEMONITORPLUGIN_H__

