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
* Description:  MCCESsObserver derived class for handling of 
*				observer events
*
*/
/*
* ===================================================================
*  Name:        ctestssobserver.h
*
*  Description: MCCESsObserver derived class for handling of 
*				observer events
*
* ===================================================================
*/

#ifndef CTESTSSOBSERVER_H
#define CTESTSSOBSERVER_H

// Includes

#include <mccessobserver.h>
#include "cccetester.h"
#include "ceventstorage.h"

// Forward declarations

NONSHARABLE_CLASS( CTestSsObserver ) : public CBase, 
                        public MCCESsObserver
	{
		
	public:
		
		static CTestSsObserver* NewL( CEventStorage* aEventStorage /*, CCCETester* aTester */);
		
		virtual ~CTestSsObserver();
		
	private:
	
		CTestSsObserver( CEventStorage* aEventStorage /*, CCCETester* aTester*/ );
		
		void ConstructL();
		
		// Observer methods
		
		void BarringEventOccurred( const MCCESsObserver::TCCESsBarringEvent aBarringEvent );
		
		void CLIEventOccurred( const MCCESsObserver::TCCESsCLIEvent aCallLineEvent );
		
		void CallForwardEventOccurred( const MCCESsObserver::TCCESsCallForwardEvent aCallForwardEvent,
		                               const TDesC& aRemoteAddress );
								 	   
		void CallCugEventOccurred( const MCCESsObserver::TCCESsCugEvent aCugEvent );

		void NotifyCurrentActiveALSLine( TInt aLine );
		
		// Data members
	
		CEventStorage* iEventStorage;
	
	};
	
#endif //CTESTSSOBSERVER_H