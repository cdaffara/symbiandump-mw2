/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides an observer interface for backup and restore listener
*
*/


#ifndef M_LBTSBACKUPRESTOREOBSERVER_H
#define M_LBTSBACKUPRESTOREOBSERVER_H

/**
 * System state starter listener provides this to notify
 * when system state is ready for server loading
 *
 * @since S60 v4.0
 */
class MLbtBackupRestoreObserver
	{
	public:
		
		// Enumeration to determine if a backp or restore operation
		// has started or ended		
		enum TBRState
			{
			EStarted, // Backup or Restore operation has started
			EEnded,	// Backup or Restore operation has ended
			EInProgress // Backup or Restore operation is in progress
			};
		/**
		 * Notifies the observer when any backup operation
		 * is going on
		 *
		 * @param[in] aState the state to which the backup or restore
		 * operation has changed to
		 */
		virtual void HandleBackupRestoreOperationL( TBRState aState ) = 0;
		
	};
	
#endif //M_LBTSBACKUPRESTOREOBSERVER_H