/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*/

#ifndef UNMOUNTHANDLERAO_H
#define UNMOUNTHANDLERAO_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <e32msgqueue.h>

class MUnmountObserver
{
public:
	virtual void HandleUnmount( TUint32 aMediaId ) = 0;
};

class CUnmountHandlerAO : public CActive
{
public:
	// Cancel and destroy
	~CUnmountHandlerAO();

	// Two-phased constructor.
	static CUnmountHandlerAO* NewL( MUnmountObserver& aObserver );

	// Two-phased constructor.
	static CUnmountHandlerAO* NewLC( MUnmountObserver& aObserver );

public: // New functions
	// Function for making the initial request
	void WaitForUnmountL();

private:
	// C++ constructor
	CUnmountHandlerAO( MUnmountObserver& aObserver );
	
	// Second-phase constructor
	void ConstructL();
	
private: // From CActive
	// Handle completion
	void RunL();
	
	// How to cancel me
	void DoCancel();
	
	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError( TInt aError );

private:
	MUnmountObserver* iUnmountObserver;
	RMsgQueue<TUint32> iMsgQueue;
};

#endif // UNMOUNTHANDLERAO_H
