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
* Description:  MCCEExtObserver derived class for handling of 
*				observer events
*
*/
/*
* ===================================================================
*  Name:        ctestextobserver.h
*
*  Description: MCCEExtObserver derived class for handling of 
*				observer events
*
* ===================================================================
*/

#ifndef CTESTEXTOBSERVER_H
#define CTESTEXTOBSERVER_H

// Includes

#include <mcceextensionobserver.h>
#include "ceventstorage.h"

// Forward declarations


NONSHARABLE_CLASS( CTestExtObserver ) : public CBase, 
                         public MCCEExtensionObserver
	{
		
	public:
		
		static CTestExtObserver* NewL( CEventStorage* aEventStorage /*, CCCETester* aTester */);
		
		virtual ~CTestExtObserver();
		
	private:
	
		CTestExtObserver( CEventStorage* aEventStorage /*, CCCETester* aTester*/ );
		
		void ConstructL();
		
		// Observer methods
		
		void HandleExtensionEvents( TUint32 aServiceId,
									TInt aEvent,
                                    TInt aStatus );
		
		// Data members
	
		CEventStorage* iEventStorage;
		
		//CCCETester* iTester;
	
	};
	
#endif //CTESTExtObserver_H