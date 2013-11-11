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
* Description:  MCCEConferenceCallObserver based class for handling
*				of conference call observer events
*
*/
/*
* ===================================================================
*  Name:        ctestconfcallobserver.h
*
*  Description: MCCEConferenceCallObserver based class for handling
*				of conference call observer events
*
* ===================================================================
*/

#ifndef CTESTCONFCALLOBSERVER_H
#define CTESTCONFCALLOBSERVER_H

// Includes

#include <e32def.h> // NULL definition
#include <mcceconferencecallobserver.h>
#include "ceventstorage.h"

class CCCETester;

NONSHARABLE_CLASS(CTestConfCallObserver)
    : public CBase,
      public MCCEConferenceCallObserver
	{
public:	// Functions
	
		static CTestConfCallObserver* NewL( CEventStorage* aEventStorage, CCCETester* aTester );
		
		virtual ~CTestConfCallObserver();

private:// Functions
		
		CTestConfCallObserver( CEventStorage* aEventStorage, CCCETester* aTester );

		void ConstructL();
			
		// Observer methods
		
		void CallEventOccurred( const MCCEConferenceCallObserver::TCCEConferenceCallEvent aEvent,
	                 			MCCECall* aReferredCall );

	    void CallStateChanged( const MCCEConferenceCallObserver::TCCEConferenceCallState aState );

	    void CallCapsChanged( const MCCEConferenceCallObserver::TCCEConferenceCallCaps aCaps );
	     
	    void ErrorOccurred( TCCPConferenceCallError aError );

	    void CallDurationChanged( const TTimeIntervalSeconds aDuration );
	    
private:// Data members
	    
	    CEventStorage* iEventStorage;
	    
public: // Data members
	    
	    CCCETester* iTester;
	
	};
	
#endif //CTESTCONFCALLOBSERVER_H