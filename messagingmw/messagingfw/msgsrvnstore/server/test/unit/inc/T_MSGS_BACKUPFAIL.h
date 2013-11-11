// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __T_BACKUP_H__
#define __T_BACKUP_H__

#include <f32file.h>
#include "smstestutils.h"

class CBackupTest : public CActive, public MMsvSessionObserver
	{
	public:
		CBackupTest();
		~CBackupTest();
		void ConstructL();
		void StartBackupL();
		void StartRestoreL();
		void StopBackupRestoreL();
		void RemoveSMSMessageL();
		void ReplaceMailFolderL();
		void CreateInitialMailFolderL();
		void CopyMailFolderL();
		void WaitForSMSL();
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
		void DoCancel();
		void RunL();
		void CheckForSMSL();
	private:
		CMsvSession* iSession;
		CFileMan* iFileman;
		RFs iFs;
		CBaBackupSessionWrapper* iBackup;
		CSmsTestUtils* iSmsTest;
		CTestUtilsWatcherStarter* iWatchers;
		CMsvEntry* iInboxEntry;
		RTimer iTimer;
		TInt iCount;
	};

#endif
