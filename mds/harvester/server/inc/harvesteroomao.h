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
* Description:  Low memory observer for havester server
*
*/

#ifndef __HARVESTEROOMAO_H__
#define __HARVESTEROOMAO_H__


// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32msgqueue.h>

// CLASS DECLARATION

/**
 * MHarvesterPluginObserver
 */
class MHarvesterOomObserver
	{
    public:
    	virtual void MemoryLow() = 0;
    	virtual void MemoryGood() = 0;
	};

/**
*  CMdSOomPlugin
* 
*/
class CHarvesterOomAO : public CActive
	{
public:
	// Constructors and destructor
	static CHarvesterOomAO* NewL( MHarvesterOomObserver& aObserver );
	
	/**
	 * Destructor.
	 */
	virtual ~CHarvesterOomAO();	
	
    /**
     * Standard RunL-method.
     * From CActive.
     */       
    void RunL();

    /**
     * From CActive.
     */       
    void DoCancel();

    /**
     * From CActive.
     * @param aError  Leave error code.
     * @return Always KErrNone.
     */       
    TInt RunError( TInt aError );


private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHarvesterOomAO( MHarvesterOomObserver& aObserver );
	
	/**
	 * default constructor for performing 2nd stage construction
	 */
	void ConstructL();

    
private:
	
	RMsgQueue<TInt> iOomMsgQueue;
	MHarvesterOomObserver* iObserver;
	
	};


#endif /*__HARVESTEROOMAO_H__*/
