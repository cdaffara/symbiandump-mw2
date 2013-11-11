/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Backup/restore observer interface
*
*/

#ifndef __MDEBACKUPRESTOREOBSERVER_H__
#define __MDEBACKUPRESTOREOBSERVER_H__

class MMdEBackupRestoreObserver
	{
	public:
		// Called when backup & restore watcher notices starting back up.
		virtual void HandleBackup() = 0;
		// Called when backup & restore watcher notices starting restore.
		virtual void HandleRestore() = 0;
		// Called when backup or watcher is completed and normal operation continues.
		virtual void ResumeOperation() = 0;
	};
	
#endif