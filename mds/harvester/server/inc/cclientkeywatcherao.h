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
#ifndef CCLIENTKEYWATCHERAO_H_
#define CCLIENTKEYWATCHERAO_H_

#include <e32base.h>
#include <e32cmn.h>
#include <e32property.h>

class CRestoreWatcher;

const TUid KPSRestoreWatcherCategory = { 0x200009F5 };
const TUint KPSRestoreWatcherClientsKey = 0x00000010;

class CClientKeyWatcherAO : public CActive
	{	
	public:
		static CClientKeyWatcherAO* NewL( CRestoreWatcher* aRestoreWatcher );
		virtual ~CClientKeyWatcherAO();
	
	private:
		CClientKeyWatcherAO( CRestoreWatcher* aRestoreWatcher );
		void ConstructL();
		void RunL();
		void DoCancel();
		TInt RunError( TInt );
		
	private:
		// Not owned
		CRestoreWatcher* iRestoreWatcher;

		RProperty iClientsProperty;
		TInt iClients;
	};

#endif /*CCLIENTKEYWATCHERAO_H_*/
