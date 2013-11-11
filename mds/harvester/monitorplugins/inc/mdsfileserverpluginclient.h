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


#ifndef __MDSFILESERVERPLUGINCLIENT_H__
#define __MDSFILESERVERPLUGINCLIENT_H__

enum TMdsFSPOperation
	{
	EMdsFSPOpEnable,
	EMdsFSPOpDisable,
	EMdsFSPOpRegisterNotification,
	EMdsFSPOpAddNotificationPath,
	EMdsFSPOpRemoveNotificationPath,
	EMdsFSPOpAddIgnorePath,
	EMdsFSPOpRemoveIgnorePath,
	EMdsFSPOpNotificationCancel,
	};

class TMdsFSPStatus
	{
	public:
		TInt iFileEventType;
		TInt iDriveNumber;
		TFileName iFileName;
		TFileName iNewFileName;
        TUid iProcessId;
        TUint32 iDriveMediaId;
	};

enum TMdsFileEventType
	{
	EMdsFileCreated,
	EMdsFileRenamed,
	EMdsFileModified,
	EMdsFileReplaced,
	EMdsFileDeleted,
	EMdsDriveFormatted,
	EMdsFileUnknown,
	EMdsDirRenamed,
	EMdsFileAttribsChanged
	};

typedef TPckgBuf<TMdsFSPStatus> TMdsFSPStatusPckg;

const TInt KMdsFSPluginPosition = 0x200071CD;

#ifndef __MDSFILESERVERPLUGIN_H__
#include <f32file.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <f32file_private.h>
#endif


/**
 * 
 */
class RMdsFSPEngine : public RPlugin
	{
	public:

		/**
		 * Register...
		 * @param aMdsFSPStatus ...
		 * @param aStat  TRequestStatus for asyncronous operation.
		 */
		inline void RegisterNotification( TMdsFSPStatusPckg& aMdsFSPStatus,
				TRequestStatus& aStat);

		/**
		 * Adds a new path to monitor.
		 * @param aPath  Path to add.
		 */
		inline void AddNotificationPath( const TDesC& aPath );

		/**
		 * Removes a path from list of monitored paths.
		 * @param aPath  Path to remove.
		 */
		inline void RemoveNotificationPath( const TDesC& aPath );

		/**
		 * Adds a new ignored path.
		 * @param aPath Path descriptor.
		 */
		inline void AddIgnorePath( const TDesC& aPath );

		/**
		 * Removes a ignored path from the list.
		 * @param aPath Path descriptor.
		 */
		inline void RemoveIgnorePath( const TDesC& aPath );

		/**
		 * Enable plugin.
		 * @return Error code.
		 */
		inline TInt Enable();

		/**
		 * Disable plugin.
		 * @return Error code.
		 */
		inline TInt Disable();

		/**
		 * Cancel notifications.
		 */
		inline void NotificationCancel();
	};

#include "mdsfileserverpluginclient.inl"  // inlined methods

#endif // __MDSFILESERVERPLUGIN_H__

#endif // __MDSFILESERVERPLUGINCLIENT_H__

