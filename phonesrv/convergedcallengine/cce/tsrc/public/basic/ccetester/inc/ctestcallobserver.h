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
* Description:  MCCECallObserver based class for handling of 
*				observer events
*
*/
/*
* ===================================================================
*  Name:        ctestcallobserver.h
*
*  Description: MCCECallObserver based class for handling of 
*				observer events
*
* ===================================================================
*/

#ifndef CTESTCALLOBSERVER_H
#define CTESTCALLOBSERVER_H

// Includes

#include <mccecallobserver.h>

// Forward declarations

class CEventStorage;

NONSHARABLE_CLASS(CTestCallObserver) : public CBase,
                          public MCCECallObserver 
	{
	
	private:
	
		CTestCallObserver( CEventStorage* aEventStorage, TInt aObserverID );
	
		void ConstructL();
	
	public:
		virtual ~CTestCallObserver();
		static CTestCallObserver* NewL( CEventStorage* aEventStorage, TInt aObserverID );
		
	// Observer methods
	
	private:
	
		void ErrorOccurred( const TCCPError aError );
		
		void CallStateChanged( const CCPCall::TCallState aState );
		
		void CallStateChangedWithInband( const CCPCall::TCallState aState );
		
		void CallEventOccurred( const MCCECallObserver::TCCECallEvent aEvent );
		
		void CallDurationChanged( const TTimeIntervalSeconds aDuration );
		
		void CallCapsChanged( const MCCECallObserver::TCCECallControlCaps aCaps );
		
		void HandleTransfer( const TBool aAttended, const TDesC& aDestination );
		
	// Data members
		
		TInt iObserverID;		// Linked to CallID in OpenNewCall (numbering starts from one)
		
		CEventStorage* iEventStorage;
	
	};
	
#endif //CTESTCALLOBSERVER_H
