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
* Description:  Monitors MMC insertions and removals.*
*/

#ifndef __MMCMONITORPLUGIN_H__
#define __MMCMONITORPLUGIN_H__

#include <e32base.h>

#include "mdeharvestersession.h"
#include "mdeobject.h"
#include "monitorplugin.h"
#include "mmcmounttaskao.h"
#include "mmcusbao.h"
#include "mmcscannerao.h"

class CMMCMonitorPlugin : public CMonitorPlugin, public MMMCMonitorObserver
  	{
	public:
		/**
		* Creates and constructs a new instance of CMMCMonitorPlugin.
		*
		* @return A pointer to the new instance of CMMCMonitorPlugin.
		*/
		static CMMCMonitorPlugin* NewL();
		
		/**
		* Destructor
		*/
		virtual ~CMMCMonitorPlugin();
				
		/**
		* 
		*
		* @param aObserver  All events are notified via the aObserver.
		* @param aMdEClient  A pointer to MdE client.
		* @param aCtxEngine  A pointer to context engine.
		* @param aPluginFactory  A pointer to harvester plugin factory.
		* @return ETrue if success, EFalse if not.
		*/
		TBool StartMonitoring( MMonitorPluginObserver& aObserver,
            CMdESession* aMdEClient, CContextEngine* aCtxEngine,
            CHarvesterPluginFactory* aHarvesterPluginFactory );
		
		/**
		* Stops the monitoring.
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
		TBool ResumeMonitoring( MMonitorPluginObserver& aObserver,
            CMdESession* aMdEClient, CContextEngine* aCtxEngine,
            CHarvesterPluginFactory* aHarvesterPluginFactory );
		
		/**
		* Pauses the monitoring.
		*
		* @return ETrue if success, EFalse if not.
		*/
		TBool PauseMonitoring();
		
		/**
		* Inherited from MMMCMonitorObserver. Will be called by CMMCMonitorAO when any MMC is inserted
		* or removed.
		*
		* @param aDriveChar  The drive which is inserted or removed.
		* @param aEventType  EMounted if media is inserted, EDismounted if media is removed.
		*/
		void MountEvent( TChar aDriveChar, TUint32 aMediaID, TMMCEventType aEventType );
		
	private:
		
		/**
		* C++ constructor - not exported;
		* implicitly called from NewL()
		*/
		CMMCMonitorPlugin();
		
		/**
		* 2nd phase construction, called by NewL().
		*/
		void ConstructL();
		
		/**
		* Creates and adds a file monitor for a specified drive.
		*
		* @param aDrive  A drive for file monitor.
		*/
		void AddNotificationPathL( TChar aDrive );
		
		/**
		* Starts monitor all MMCs in the device.
		*/
		void StartMonitoringAllMMCsL( RArray<TMdEMediaInfo>& aMedias );
		
	private: // data
		/**
		* An observer class to notify about file creations, modifications and deletions in any MMCs.
		*/
		MMonitorPluginObserver* iObserver;
		
		/**
		* A pointer to MMC monitor which observes any MMCs insertions and removals.
		*/
		CMMCMonitorAO* iMMCMonitor;
		
		/**
		* A pointer to MdE client.
		*/
		CMdEHarvesterSession* iMdEClient;
		
		CMMCMountTaskAO* iMountTask;
		
		CMMCUsbAO* iUsbMonitor;
		
		CMmcScannerAO* iMmcScanner;
		
		CMmcScannerAO* iHddScanner;
		
		RFs iFs;

	};

#endif // __MMCMONITORPLUGIN_H__
