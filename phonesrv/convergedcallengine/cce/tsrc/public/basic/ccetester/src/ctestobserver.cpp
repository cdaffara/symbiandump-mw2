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
* Description:  CTestObserver implementation
*
*/
/*
* ============================================================================
*  Name:        ctestobserver.cpp
* 
*  Description: CTestObserver implementation
*
* ============================================================================
*/

// Includes

#include <e32base.h>
#include <e32debug.h>
#include "mccecall.h"
#include "ctestobserver.h"
#include "ccetesterdefs.h"
#include "ctestcallobserver.h"

// Forward declarations

class MCCECall;

// ---------------------------------------------------------------
// CTestObserver Constructor & Destructor
// ---------------------------------------------------------------

CTestObserver::CTestObserver( CEventStorage* aEventStorage, CCCETester* aTester )
	{
	iEventStorage = aEventStorage;
	
	iTester = aTester;
	}

CTestObserver::~CTestObserver()
	{
	}

// ---------------------------------------------------------------
// CTestObserver::NewL()
//
// Allocate CTestObserverCombined from heap
// ---------------------------------------------------------------
CTestObserver* CTestObserver::NewL( CEventStorage* aEventStorage, CCCETester* aTester )
	{
	CTestObserver* self = new (ELeave) CTestObserver( aEventStorage, aTester );
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}
	
// ---------------------------------------------------------------
// CTestCallObserver::ConstructL()
// ---------------------------------------------------------------
void CTestObserver::ConstructL()
	{
	}
	
/*********************** OBSERVER METHODS ***********************/

	
// ---------------------------------------------------------------
// CTestObserver::IncomingCall()
// ---------------------------------------------------------------
void CTestObserver::IncomingCall( MCCECall& aCall )
	{
	TInt nextCallObserverID = iTester->iCallObserverArray.Count() + 1;
	
	// Create new Call Observer
    CTestCallObserver* newCallObs;
    TRAPD( err, newCallObs = CTestCallObserver::NewL( iEventStorage, nextCallObserverID ) );
    
    __ASSERT_ALWAYS( err == KErrNone, User::Panic( _L("CTestObserver::IncomingCall, could not create new call observer"), -2));
        
    // Store handle of created Call Observer
    iTester->InsertCallObserver( *newCallObs );
	
	iTester->InsertCall( aCall );
	
	TEvent newEvent;
	
	newEvent.iCallID = 0;
	newEvent.iEventType = CEventStorage::EObserverIncomingCall;
	newEvent.iEventID = 0;
	newEvent.iParam1 = 0;
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );
	
	// Set Call Observer
	aCall.SetObserver( (MCCECallObserver&)*newCallObs );
	}

// ---------------------------------------------------------------
// CTestObserver::PluginMOCallCreated()
// ---------------------------------------------------------------
void CTestObserver::MOCallCreated( MCCECall& aCall )
	{
	iTester->iPluginInitiatedCall = &aCall;
	
	TEvent newEvent;
	
	newEvent.iCallID = 0;
	newEvent.iEventType = CEventStorage::EObserverMOCallCreated;
	newEvent.iEventID = 0;
	newEvent.iParam1 = 0;
	newEvent.iParam2 = 0;
	
	iEventStorage->StoreEvent( newEvent );	
	}
	
// ---------------------------------------------------------------
// CTestObserver::ConferenceCallCreated()
// -------------------------------------------------------
void CTestObserver::ConferenceCallCreated( MCCEConferenceCall& aConferenceCall )
    {
    RDebug::Print(_L("[CCETEST] CTestObserver::ConferenceCallCreated"));
    
    TEvent newEvent;
    
    newEvent.iCallID = 0;
    newEvent.iEventType = CEventStorage::EObserverConferenceCallCreated;
    newEvent.iEventID = 0;
    newEvent.iParam1 = 0;
    newEvent.iParam2 = 0;
    
    iEventStorage->StoreEvent( newEvent );  
    }

// ---------------------------------------------------------------
// CTestObserver::DataPortName()
// ---------------------------------------------------------------
void CTestObserver::DataPortName( TName& /* aPortName */ )
	{
	RDebug::Print(_L("[CCETEST] CTestObserver::DataPortName"));
	}
	
