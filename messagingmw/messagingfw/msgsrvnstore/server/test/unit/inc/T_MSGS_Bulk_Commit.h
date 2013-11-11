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

#ifndef __T_MSGS_BULK_COMMIT_H__
#define __T_MSGS_BULK_COMMIT_H__

#include <msvapi.h>

class CBulkCommitTest : public CActive, public MMsvSessionObserver
	{
	public:		
		static CBulkCommitTest* NewL();
		~CBulkCommitTest();			
		void TestBulkCommitL();
	private:
		CBulkCommitTest();
		
		//from MMsvSessionObserver
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
		
		//from CActive
		void DoCancel();
		void RunL();
	private:	
		enum TBulkStatus 
			{
			ECreate = 0,
			EDone
			};
	private:	
		CMsvOperation* iOperation;
		TBulkStatus iState;		
	};

#endif //__T_MSGS_BULK_COMMIT_H__
