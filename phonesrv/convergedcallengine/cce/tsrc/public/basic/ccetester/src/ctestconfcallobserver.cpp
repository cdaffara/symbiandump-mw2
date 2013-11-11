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
* Description:  CTestConfCallObserver implementation
*
*/
/*
* ============================================================================
*  Name:        ctestconfcallobserver.cpp
* 
*  Description: CTestConfCallObserver implementation
*
* ============================================================================
*/

// Include files

#include <e32debug.h>
#include <mccecall.h>
#include "ctestconfcallobserver.h"
#include "cccetester.h"
#include "ceventstorage.h"

// Forward declarations

// Constants


// ---------------------------------------------------------------
// CTestConfCallObserver constructor & destructor
// ---------------------------------------------------------------

CTestConfCallObserver::CTestConfCallObserver( CEventStorage* aEventStorage, CCCETester* aTester  )
	{
	iEventStorage = aEventStorage;
	
	iTester = aTester;
	}

CTestConfCallObserver::~CTestConfCallObserver()
	{
	}

// ---------------------------------------------------------------
// CTestConfCallObserver::NewL()
//
// Allocate CTestConfCallObserver from heap
// ---------------------------------------------------------------
CTestConfCallObserver* CTestConfCallObserver::NewL( CEventStorage* aEventStorage, CCCETester* aTester )
	{
	CTestConfCallObserver* self = new (ELeave) CTestConfCallObserver( aEventStorage, aTester );
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}
	


// ---------------------------------------------------------------
// CTestConfCallObserver::ConstructL()
// ---------------------------------------------------------------
void CTestConfCallObserver::ConstructL()
	{
	}
	
	
/*********************** OBSERVER METHODS ***********************/

void CTestConfCallObserver::CallEventOccurred( const MCCEConferenceCallObserver::TCCEConferenceCallEvent aEvent,
                                               MCCECall* aReferredCall )
	{
	TEvent newEvent;
	
	newEvent.iCallID = 0; // not a call specific event
	newEvent.iEventType = CEventStorage::EConferenceCallEvent;
	newEvent.iEventID = aEvent;
	
	//newEvent.iParam1 = 0;
	
	if ( aReferredCall )
		{
		TInt referredIndex = iTester->iCallArray.Find( aReferredCall );
		
		__ASSERT_ALWAYS( referredIndex != KErrNotFound, User::Panic( _L("CallEventOccurred, Call index not found"), -1) );
		
		newEvent.iParam1 = referredIndex + 1;
		}
	else
		{
		// No referred call
		newEvent.iParam1 = 0;
		}
		
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );
	}


void CTestConfCallObserver::CallStateChanged( const MCCEConferenceCallObserver::TCCEConferenceCallState aState )
	{
	TEvent newEvent;
	
	newEvent.iCallID = 0; // not a call specific event
	newEvent.iEventType = CEventStorage::EConferenceCallStateChanged;
	newEvent.iEventID = aState;
	newEvent.iParam1 = 0;
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );
	}
	
void CTestConfCallObserver::CallCapsChanged( const MCCEConferenceCallObserver::TCCEConferenceCallCaps /*aCaps*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestConfCallObserver::CallCapsChanged"));
	}
	
void CTestConfCallObserver::ErrorOccurred( TCCPConferenceCallError aError )
	{
	RDebug::Print(_L("[CCETEST] CTestConfCallObserver::ErrorOccurred, aError: %d"), aError );
	
	// if errors are stored
#if 0
	if ( aError != KErrNone )
        {
        TEvent newEvent;
        newEvent.iCallID = iObserverID;
        newEvent.iEventType = CEventStorage::  xxx;
        newEvent.iEventID = 0;
        
        // Error code is passed as parameter 1
        newEvent.iParam1 = aError;
        newEvent.iParam2 = 0;
        
        iEventStorage->StoreEvent( newEvent );
        }
#endif
	}
	

void CTestConfCallObserver::CallDurationChanged( const TTimeIntervalSeconds /*aDuration*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestConfCallObserver::CallDurationChanged"));
	}
