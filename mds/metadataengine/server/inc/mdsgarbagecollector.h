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
* Description:  This is garbage colletion class which sends*
*/

#ifndef _MDSGARBAGECOLLECTOR_H_
#define _MDSGARBAGECOLLECTOR_H_

#include <e32std.h>
#include <e32base.h>

class MMdSGarbageCollectorObserver
	{
	public:
		/*
		 * @return if the garbage collection need to be started again
		 */
		virtual TBool StartGarbageCollectionL() = 0;
	};

class CMdSGarbageCollector : public CActive
{
public:
	virtual ~CMdSGarbageCollector();
	/**
	 * Two phase constructor.
	 * @param aObserver an observer which will be notified when the garbage collector starts.
	 * @return an instance of CMdSGarbageCollector
	 */
	static CMdSGarbageCollector* NewL(MMdSGarbageCollectorObserver& aObserver);
	
	/**
	 * Two phase constructor.
	 * @param aObserver an observer which will be notified when the garbage collector starts.
 	 * @return an instance of CMdSGarbageCollector
	 */
	static CMdSGarbageCollector* NewLC(MMdSGarbageCollectorObserver& aObserver);
	
	/**
	 * Starts garbage connection with a delay. The same value will also be used as interval between
	 * following garbage collections if the observer returns ETrue when StartGarbageCollectionL()
	 * is called.
	 * 
	 * @param aDelay delay before garbage collection starts
	 */
	void Start( TInt aDelay );
	
	/**
	 * Pauses garbage collection.
	 */
	void Pause();
	
	/**
	 * Resumes garbage collection.
	 */
	void Resume();

private:
	CMdSGarbageCollector(MMdSGarbageCollectorObserver& aObserver);
	void ConstructL();
	
	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError( TInt aError );
	
private:
	RTimer iTimer;

	TTimeIntervalMicroSeconds32 iDelay;
	TTimeIntervalMicroSeconds32 iNewDelay;

	MMdSGarbageCollectorObserver& iObserver;
};

#endif
