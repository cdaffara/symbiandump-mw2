// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_MSV_SYS_PROGRESS_H__
#define __T_MSV_SYS_PROGRESS_H__

#include <msvapi.h>
#include <tmsvsystemprogress.h>

class CSysProgressTest : public CActive, public MMsvSessionObserver
	{
	public:		
		static CSysProgressTest* NewL();
		~CSysProgressTest();			
		void TestSysProgressL();
		void TestBulkCommitProgressL();
	private:
		CSysProgressTest();
		
		//from MMsvSessionObserver
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
		
		//from CActive
		void DoCancel();
		void RunL();
	private:	
		enum TSysProgressStatus 
			{
			ECreate = 0,
			ESysProgress,
			EDone
			};
	private:	
		CMsvOperation* iOperation;
		TSysProgressStatus iState;		
	};

#endif //__T_MSV_SYS_PROGRESS_H__
