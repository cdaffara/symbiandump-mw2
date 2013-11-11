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
* Description:  MCCEDtmfObserver based class for handling of 
*				DTMF observer events
*
*/
/*
* ===================================================================
*  Name:        ctestdtmfobserver.h
*
*  Description: MCCEDtmfObserver based class for handling of 
*				DTMF observer events
*
* ===================================================================
*/

#ifndef CTESTDTMFOBSERVER_H
#define CTESTDTMFOBSERVER_H

// Includes

#include <mccedtmfobserver.h>

// Forward declarations

class CEventStorage;

NONSHARABLE_CLASS( CTestDtmfObserver ) : public CBase, 
                          public MCCEDtmfObserver
	{
	public:
	
		static CTestDtmfObserver* NewL(CEventStorage* aEventStorage);
		
		virtual ~CTestDtmfObserver();

	private:
	
		CTestDtmfObserver(CEventStorage* aEventStorage);
	
		void ConstructL();
		
		// Observer methods
	
        void HandleDTMFEvent( const MCCEDtmfObserver::TCCEDtmfEvent aEvent, 
                              const TInt aError, 
                              const TChar aTone );
                          
        // Data members
                          
		CEventStorage* iEventStorage;
	
	};
	
#endif //CTESTDTMFOBSERVER_H