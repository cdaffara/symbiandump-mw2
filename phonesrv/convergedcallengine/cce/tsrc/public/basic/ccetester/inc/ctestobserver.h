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
* Description:  MCCEObserver derived class for handling of 
*				observer events
*
*/
/*
* ===================================================================
*  Name:        ctestobserver.h
*
*  Description: MCCEObserver derived class for handling of 
*				observer events
*
* ===================================================================
*/

#ifndef CTESTOBSERVER_H
#define CTESTOBSERVER_H

// Includes

#include <mcceobserver.h>
#include "cccetester.h"
#include "ceventstorage.h"

// Forward declarations

class CCCETester;

NONSHARABLE_CLASS( CTestObserver ) : public CBase,
                      public MCCEObserver
	{
		
	public:
		
		static CTestObserver* NewL( CEventStorage* aEventStorage, CCCETester* aTester );
		
		virtual ~CTestObserver();
		
	private:
	
		CTestObserver( CEventStorage* aEventStorage, CCCETester* aTester );
		
		void ConstructL();
		
		// Observer methods
		
		void IncomingCall( MCCECall& aCall );
		
		void MOCallCreated( MCCECall& aCall );
		
		void ConferenceCallCreated( MCCEConferenceCall& aConferenceCall );
		
		void DataPortName( TName& aPortName );
		
		// Data members
	
		CEventStorage* iEventStorage;
		
		CCCETester* iTester;
	
	};
	
#endif //CTESTOBSERVER_H