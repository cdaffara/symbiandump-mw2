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
* Description:  CTestCallObserver implementation
*
*/
/*
* ============================================================================
*  Name:        ctestcallobserver.cpp
* 
*  Description: CTestCallObserver implementation
*
* ============================================================================
*/

// Include files

#include <e32base.h>
#include <e32debug.h>
#include "ctestcallobserver.h"
#include "ceventstorage.h"
#include "cccetester.h"

// Forward declarations

// Constants


// ---------------------------------------------------------------
// CTestCallObserver Constructor & Destructor
// ---------------------------------------------------------------

CTestCallObserver::CTestCallObserver( CEventStorage* aEventStorage, TInt aObserverID )
	{
	iObserverID = aObserverID;
	
	iEventStorage = aEventStorage;
	}

CTestCallObserver::~CTestCallObserver()
	{
	}

// ---------------------------------------------------------------
// CTestCallObserver::NewL()
//
// Allocate CTestObserverCombined from heap
// 
// ---------------------------------------------------------------
CTestCallObserver* CTestCallObserver::NewL( CEventStorage* aEventStorage, TInt aObserverID )
	{	
	CTestCallObserver* self = new (ELeave) CTestCallObserver( aEventStorage, aObserverID );
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}
	


// ---------------------------------------------------------------
// CTestCallObserver::ConstructL()
// ---------------------------------------------------------------
void CTestCallObserver::ConstructL()
	{
	}
	
	
/*********************** OBSERVER METHODS ***********************/
	


	
void CTestCallObserver::CallStateChanged( const CCPCall::TCallState aState )
	{
	TEvent newEvent;
	
	newEvent.iCallID = iObserverID;
	newEvent.iEventType = CEventStorage::ECallObserverCallStateChanged;
	newEvent.iEventID = aState;
	newEvent.iParam1 = 0;
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );
	}
	
	
void CTestCallObserver::CallStateChangedWithInband( const CCPCall::TCallState aState )
	{
	TEvent newEvent;
	
	newEvent.iCallID = iObserverID;
	newEvent.iEventType = CEventStorage::ECallObserverCallStateChangedWithInband;
	newEvent.iEventID = aState;
	newEvent.iParam1 = 0;
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );
	}
	

void CTestCallObserver::CallEventOccurred( const MCCECallObserver::TCCECallEvent aEvent )
	{
	TEvent newEvent;
	
	newEvent.iCallID = iObserverID;
	newEvent.iEventType = CEventStorage::ECallObserverCallEventOccurred;
	newEvent.iEventID = aEvent;
	newEvent.iParam1 = 0;
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );
	}


void CTestCallObserver::CallDurationChanged( const TTimeIntervalSeconds aDuration )
	{
	RDebug::Print(_L("[CCETEST] CallDurationChanged call %d: %d"), iObserverID, aDuration );
	}


void CTestCallObserver::CallCapsChanged( const MCCECallObserver::TCCECallControlCaps /*aCaps*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestCallObserver::CallCapsChanged, call %d"), iObserverID );
	}


void CTestCallObserver::ErrorOccurred( const TCCPError aError )
	{
    // Notify only error codes not KErrNone
	if ( aError != KErrNone )
		{
		TEvent newEvent;
		
		newEvent.iCallID = iObserverID;
		newEvent.iEventType = CEventStorage::ECallObserverErrorOccurred;
		newEvent.iEventID = 0;
		
		// Error code is passed as parameter 1
		newEvent.iParam1 = aError;
		newEvent.iParam2 = 0;
		
		iEventStorage->StoreEvent( newEvent );
		}
	}
	

void CTestCallObserver::HandleTransfer( const TBool aAttended, const TDesC& /*aDestination*/ )
	{
	TEvent newEvent;
	
	newEvent.iCallID = iObserverID;
	newEvent.iEventType = CEventStorage::ECallObserverHandleTransfer;
	newEvent.iEventID = 0;
	
	if ( aAttended == EFalse )
		{
		newEvent.iParam1 = 0;	
		}
	else
		{
		newEvent.iParam1 = 1;
		}

	// Destination is not passed
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );
	}
	

