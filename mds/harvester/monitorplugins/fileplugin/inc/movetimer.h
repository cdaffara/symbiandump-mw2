/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/

#ifndef MOVETIMER_H
#define MOVETIMER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

// FORWARD DECLARATION
class CHarvesterData;

class MMoveTimerObserver
	{
public:
	virtual void NotMoveEvent(RPointerArray<CHarvesterData>& aHDArray) = 0;
	};

class CMoveTimer : public CActive
{
public:
	// Cancel and destroy
	~CMoveTimer();

	// Two-phased constructor.
	static CMoveTimer* NewL( MMoveTimerObserver* aObserver = NULL);

	// Two-phased constructor.
	static CMoveTimer* NewLC( MMoveTimerObserver* aObserver = NULL);

public: // New functions
		
	void AddHarvesterDataL( CHarvesterData* aHD );
	
	CHarvesterData* CheckUriL( const TDesC& aUri );

private:
	// C++ constructor
	CMoveTimer( MMoveTimerObserver* aObserver = NULL );
	
	// Second-phase constructor
	void ConstructL();
	
	// Function for making the initial request
	void StartL();
	
private: // From CActive
	// Handle completion
	void RunL();
	
	// How to cancel me
	void DoCancel();
	
	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError( TInt aError );

private:
	RTimer	iTimer;		// Provides async timing service
	MMoveTimerObserver* iObserver;
	RPointerArray<CHarvesterData> iHDArray;
};

#endif // MOVETIMER_H
