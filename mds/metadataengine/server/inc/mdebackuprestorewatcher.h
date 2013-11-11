/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class for observing backup/restore events
*
*/

#ifndef __MDEBACKUPRESTOREWATCHER_H__
#define __MDEBACKUPRESTOREWATCHER_H__

#include <e32base.h>
#include <e32cmn.h>
#include <e32property.h>

#include "mdebackuprestoreobserver.h"

class CMdEBackupRestoreWatcherAO : public CActive
	{
	public:
		static CMdEBackupRestoreWatcherAO* NewL(MMdEBackupRestoreObserver& aObserver);

		static CMdEBackupRestoreWatcherAO* NewLC(MMdEBackupRestoreObserver& aObserver);

		virtual ~CMdEBackupRestoreWatcherAO();

	private: // private methods
		CMdEBackupRestoreWatcherAO(MMdEBackupRestoreObserver& aObserver);
		void ConstructL();
		
		/**
	    * RunL from CActive
		*/
		void RunL();
		 
	    /**
		* DoCancel from CActive
		*/
		void DoCancel();

	    /**
		* Run by active scheduler in case of an error.
		* @param aError Code of the occurred error
		* @return Error code after RunError().
		*/
		TInt RunError(TInt aError);

	private: // private data
		MMdEBackupRestoreObserver& iObserver;
		RProperty iProperty;
	};
	
#endif // __MDEBACKUPRESTOREWATCHER_H__