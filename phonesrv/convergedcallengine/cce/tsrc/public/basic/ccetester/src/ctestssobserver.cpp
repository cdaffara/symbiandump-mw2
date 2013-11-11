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
* Description:  CTestSsObserver implementation
*
*/
/*
* ============================================================================
*  Name:        ctestssobserver.cpp
* 
*  Description: CTestSsObserver implementation
*
* ============================================================================
*/

// Includes

#include <e32base.h>
#include <e32debug.h>
#include "ctestssobserver.h"

// Forward declarations

class MCCESsObserver;

// ---------------------------------------------------------------
// CTestSsObserver Constructor & Destructor
// ---------------------------------------------------------------

CTestSsObserver::CTestSsObserver( CEventStorage* aEventStorage /*, CCCETester* aTester*/ )
	{
	iEventStorage = aEventStorage;
	
	//iTester = aTester;
	}

CTestSsObserver::~CTestSsObserver()
	{
	}

// ---------------------------------------------------------------
// CTestSsObserver::NewL()
//
// Allocate CTestSsObserverCombined from heap
// ---------------------------------------------------------------
CTestSsObserver* CTestSsObserver::NewL( CEventStorage* aEventStorage /*, CCCETester* aTester*/ )
	{
	CTestSsObserver* self = new (ELeave) CTestSsObserver( aEventStorage /*, aTester*/ );
	
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}
	
// ---------------------------------------------------------------
// CTestCallObserver::ConstructL()
// ---------------------------------------------------------------
void CTestSsObserver::ConstructL()
	{
	}
	
/*********************** OBSERVER METHODS ***********************/

void CTestSsObserver::BarringEventOccurred( const MCCESsObserver::TCCESsBarringEvent /*aBarringEvent*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestSsObserver::BarringEventOccurred"));
	}
	
void CTestSsObserver::CLIEventOccurred( const MCCESsObserver::TCCESsCLIEvent /*aCallLineEvent*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestSsObserver::CLIEventOccurred"));
	}
						   
void CTestSsObserver::CallForwardEventOccurred( const MCCESsObserver::TCCESsCallForwardEvent /*aCallForwardEvent*/,
							                    const TDesC& /*aRemoteAddress*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestSsObserver::CallForwardEventOccurred"));
	}

void CTestSsObserver::CallCugEventOccurred( const MCCESsObserver::TCCESsCugEvent /*aCugEvent*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestSsObserver::CallCugEventOccurred"));
	}

void CTestSsObserver::NotifyCurrentActiveALSLine( TInt /*aLine*/ )
	{
	RDebug::Print(_L("[CCETEST] CTestSsObserver::NotifyCurrentActiveALSLine"));
	}
	
