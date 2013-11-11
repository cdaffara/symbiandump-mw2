/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  header for tests
*
*/


#ifndef STUBOBSERVER_H
#define STUBOBSERVER_H

#include <e32base.h>
#include <mspnotifychangeobserver.h>

class CSPNotifyChange;

class CStubObserver: public CBase, public MSPNotifyChangeObserver
	{
	public:
	
		static CStubObserver* NewL(CActiveSchedulerWait* aWait);
		~CStubObserver();
		
	    void HandleNotifyChange( TServiceId aServiceId );
	    void HandleError( TInt aError );
		
		void NotifyChangeL( RIdArray& aIdArray );
		TInt GetServiceId();
		TInt GetCalledTimes();
		TInt AddNewEntryL();
		TBool GetFound();
		void CancelNotify();
		void LeaveWhenNotified(TBool aLeave);
		
	private:
        CStubObserver(CActiveSchedulerWait* aWait);
        void ConstructL();

	private:
		
		TInt iServiceId;
		TInt iCalledTimes;	
		CActiveSchedulerWait* iWait;
		CSPNotifyChange* iNotifyChange;
		TBool iFound;
		TBool iLeave;
	};

#endif

