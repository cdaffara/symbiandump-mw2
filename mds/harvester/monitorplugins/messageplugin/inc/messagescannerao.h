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
* Description:
*
*/

#ifndef MESSAGESCANNER_H_
#define MESSAGESCANNER_H_

#include <e32property.h>
#include <mdequery.h>

// FORWARD DECLARATION
class CMsvSession;
class CMdESession;
class CMsvFindOperation;
class MMonitorPluginObserver;

class TMessage
	{
	public:
		TInt32 iMsvId;
		TTime iCreationDate;
		TItemId iMdeId;
	};

class CMessageScannerAO : public CActive, public MMdEQueryObserver
	{
	public:
		enum TScannerState {
			EScannerIdle,
			EScannerRunning,
			EScannerFinished
		};
	
		static CMessageScannerAO* NewL( CMsvSession* aMsvSession, CMdESession* aMdeSession, 
				MMonitorPluginObserver* aMonitorPluginObserver );
		~CMessageScannerAO();
		void StartL();
		
		void HandleQueryNewResults( CMdEQuery& aQuery, TInt aFirstNewItemIndex,
				TInt aNewItemCount );

		void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );
		
	private:
		CMessageScannerAO( CMsvSession* aMsvSession, CMdESession* aMdeSession,
				MMonitorPluginObserver* aMonitorPluginObserver );
		void ConstructL();
		void RunL();
		void DoCancel();
		TInt RunError( TInt aError );
		void ScanMessagesL( RArray<TMessage>& aMessageIds );
		void QueryAllMessagesL();
		void ProcessMessagesL( CMdEQuery& aQuery, RArray<TMessage>& aMessages );
		void Unregister();
		void Cleanup();
		
	private:
		CMsvFindOperation* 	iFindOperation;
		CMsvSession* 		iMsvSession;
		CMdESession* 		iMdeSession;
		CMdEObjectQuery* 	iObjectQuery;
		MMonitorPluginObserver* iMonitorPluginObserver;
		RProperty iProperty;
		TScannerState		iState;
	};

#endif /*MESSAGESCANNER_H_*/
