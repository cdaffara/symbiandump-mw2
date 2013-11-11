// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

#include "TestUtilityServerObserver.h"


/**
CTestUtilityServerObserver()
private constructor
*/
CTestUtilityServerObserver::CTestUtilityServerObserver() 
	{
	}


/**
CTestUtilityServerObserver()
standard destructor 
*/
CTestUtilityServerObserver::~CTestUtilityServerObserver() 
	{
	}



/**
NewLC()
static interface for symbian standard 2 phase constructor, 

@return
CTestUtilityServerObserver pointer or NULL on failure
*/
CTestUtilityServerObserver* CTestUtilityServerObserver::NewLC()
	{
	CTestUtilityServerObserver *self = new (ELeave) CTestUtilityServerObserver;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;	
	}
	

/**
 NewL()
static interface for symbian standard 2 phase constructor, 

@return
CTestUtilityServerObserver pointer or NULL on failure
*/
CTestUtilityServerObserver* CTestUtilityServerObserver::NewL()
	{
	CTestUtilityServerObserver *self = CTestUtilityServerObserver::NewLC();
	CleanupStack::Pop( self );
	return self;
	}


/**
ConstructL()
2nd Part os Symbian 2 phase constructor - contains construction code 
that may leave.
*/
void CTestUtilityServerObserver::ConstructL()
	{
	// add any construction code that may leave here
	
	}
	
	
/**
HandleSessionEventL()
Implementation for MMsvSessionObserver interface. The type of event is indicated by the 
value of aEvent. The interpretation of the TAny arguments depends on this type. For most 
event types, the action that is taken, for example, updating the display, is 
client-specific. All clients though should respond to EMsvCloseSession and 
EMsvServerTerminated events. 

@param TMsvSessionEvent aEvent
Indicates the event type. 
 	
@param TAny* aArg1
Event type-specific argument value 

@param TAny* aArg2
Event type-specific argument value 

@param TAny* aArg3
Event type-specific argument value 
*/
void CTestUtilityServerObserver::HandleSessionEventL( TMsvSessionEvent aEvent
													, TAny* //aArg1
													, TAny* //aArg2
													, TAny* //aArg3
													)
	{
	switch( aEvent )
		{
		case EMsvCloseSession:
			// log server closed message her
			break;
		case EMsvServerTerminated:
			// log server terminated message her
			break;
		default:
			break;
		};
	}

