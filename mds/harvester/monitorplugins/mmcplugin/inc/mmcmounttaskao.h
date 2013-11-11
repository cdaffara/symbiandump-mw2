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
* Description:  Handles mount tasks*
*/

#ifndef __MMCMOUNTTASKAO_H__
#define __MMCMOUNTTASKAO_H__

#include <e32base.h>
#include <e32msgqueue.h>
#include <f32file.h>

#include "monitorplugin.h"
#include "mdeharvestersession.h"
#include "mmcfilelist.h"
#include "harvestereventmanager.h"

class CHarvesterPluginFactory;

class CMMCMountTaskAO : public CActive
  	{
	public:
		enum TRequest
			{
			ERequestStartTask,
			ERequestMount,
			ERequestUnmount,
			ERequestFormat,
			ERequestHandleFileEntry,
			ERequestHandleReharvest,
			ERequestCleanup,
			ERequestIdle
			};

		static CMMCMountTaskAO* NewL();	
		virtual ~CMMCMountTaskAO();
		
		void StartMountL( TMountData& aMountData );
		void StartUnmountL( TMountData& aMountData );
		void SetMonitorObserver( MMonitorPluginObserver& aObserver );
		void SetMdeSession( CMdEHarvesterSession* aSession );
		void SetHarvesterPluginFactory( CHarvesterPluginFactory* aPluginFactory );
		
		/**
		 * Returns internal hard drive's media id if the device has one.
		 */
		TUint32 GetInternalDriveMediaId( TBool& aPresent );
		
        void SetCachingStatus( TBool aCachingStatus );
        
	protected:

		void RunL();
		TInt RunError( TInt aError );
		void DoCancel();
		void SetNextRequest( TRequest aRequest );
		
		void Initialize();
		void Deinitialize();
		void SetNotPresentToMDE();
		void HandleReharvestL( RPointerArray<CPlaceholderData>& aArray );
		void RemoveNotPresentFromMDE();
		void StartNotifyL();
		void StopNotifyL();
		
	private:
	
		CMMCMountTaskAO();		
		void ConstructL();
				
	private:
		
		RFs iFs;
		TRequest iNextRequest;
		TMountData* iMountData;
		RPointerArray<TMountData> iMountDataQueue;
		RPointerArray<CPlaceholderData> iEntryArray;
		RPointerArray<CPlaceholderData> iHarvestEntryArray;
		MMonitorPluginObserver* iObserver;
		CMdEHarvesterSession* iMdeSession;
		CHarvesterPluginFactory* iHarvesterPluginFactory;
		CMmcFileList* iMmcFileList;
		CHarvesterEventManager* iHEM;
        TBool iCacheEvents;
        RPointerArray<CHarvesterData> iHdArray;
		
	};

#endif // __MMCMOUNTTASKAO_H__
