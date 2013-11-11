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
* Description:  CTestExtObserver implementation
*
*/
/*
* ============================================================================
*  Name:        ctestextobserver.cpp
* 
*  Description: CTestExtObserver implementation
*
* ============================================================================
*/

// Includes

#include <e32base.h>
#include <e32debug.h>
#include "ctestextobserver.h"

// Forward declarations

class MCCEExtObserver;

// ---------------------------------------------------------------
// CTestExtObserver Constructor & Destructor
// ---------------------------------------------------------------

CTestExtObserver::CTestExtObserver( CEventStorage* aEventStorage /*, CCCETester* aTester*/ )
	{
	iEventStorage = aEventStorage;
	
	//iTester = aTester;
	}

CTestExtObserver::~CTestExtObserver()
	{
	}

// ---------------------------------------------------------------
// CTestExtObserver::NewL()
//
// Allocate CTestExtObserverCombined from heap
// ---------------------------------------------------------------
CTestExtObserver* CTestExtObserver::NewL( CEventStorage* aEventStorage /*, CCCETester* aTester*/ )
	{
	CTestExtObserver* self = new (ELeave) CTestExtObserver( aEventStorage /*, aTester*/ );
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}
	
// ---------------------------------------------------------------
// CTestCallObserver::ConstructL()
// ---------------------------------------------------------------
void CTestExtObserver::ConstructL()
	{
	}
	
/*********************** OBSERVER METHODS ***********************/

void CTestExtObserver::HandleExtensionEvents( TUint32 aServiceId,
											 TInt aEvent,
                                    		 TInt aStatus )
	{
	TEvent newEvent;
	
	newEvent.iCallID = 0; // not a call specific event
	newEvent.iEventType = CEventStorage::EHandleExtensionEvents;
	newEvent.iEventID = aEvent;
	newEvent.iParam1 = aServiceId;
	newEvent.iParam2 = aStatus;

	iEventStorage->StoreEvent( newEvent );
	}


