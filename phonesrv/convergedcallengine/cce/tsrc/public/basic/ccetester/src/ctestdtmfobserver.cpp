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
* Description:  CTestDtmfObserver implementation
*
*/
/*
* ============================================================================
*  Name:        ctestdtmfobserver.cpp
* 
*  Description: CTestDtmfObserver implementation
*
* ============================================================================
*/

// Include files

#include <e32debug.h>
#include "ctestdtmfobserver.h"
#include "ceventstorage.h"

// Forward declarations


// ---------------------------------------------------------------
// CTestDtmfObserver Constructor & Destructor
// ---------------------------------------------------------------

CTestDtmfObserver::CTestDtmfObserver( CEventStorage* aEventStorage )
	{
	iEventStorage = aEventStorage;
	}

CTestDtmfObserver::~CTestDtmfObserver()
	{
	}

// ---------------------------------------------------------------
// CTestDtmfObserver::NewL()
//
// Allocate CTestDtmfObserver from heap
// 
// ---------------------------------------------------------------
CTestDtmfObserver* CTestDtmfObserver::NewL( CEventStorage* aEventStorage )
	{
	CTestDtmfObserver* self = new (ELeave) CTestDtmfObserver( aEventStorage );
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}
	


// ---------------------------------------------------------------
// CTestCallObserver::ConstructL()
// ---------------------------------------------------------------
void CTestDtmfObserver::ConstructL()
	{
	}
	
/*********************** OBSERVER METHODS ***********************/

void CTestDtmfObserver::HandleDTMFEvent( const MCCEDtmfObserver::TCCEDtmfEvent aEvent, 
        	                             const TInt aError,
                                         const TChar aTone )
	{
	TEvent newEvent;
	
	newEvent.iCallID = 0; // not a call specific event
	newEvent.iEventType = CEventStorage::EDtmfEvent;
	newEvent.iEventID = aEvent;
	newEvent.iParam1 = aError;
	newEvent.iParam2 = aTone.GetNumericValue();
	
	// If TChar is empty, set param2 to zero
	if ( newEvent.iParam2 == KErrNotFound )
		{
		newEvent.iParam2 = 0;
		}
			
	iEventStorage->StoreEvent( newEvent );
	}
