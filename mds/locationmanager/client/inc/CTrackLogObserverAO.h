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
* Description:
*
*/
#ifndef CTRACKLOGOBSERVERAO_H_
#define CTRACKLOGOBSERVERAO_H_

#include <e32base.h>
#include "rtracklog.h"

NONSHARABLE_CLASS( CTrackLogObserverAO ) : public CActive
{
public:
	/**
	 * C++ constructor.
	 */
	CTrackLogObserverAO( RTrackLog* aClient );
	virtual ~CTrackLogObserverAO();
	
private: // From CActive
	void DoCancel();
	void RunL();
	
public:
	TInt StartObserving();
	
private:	
	/**
    * 2nd phase constructor.
    */
    void ConstructL( RTrackLog* aClient );
    
private: // Data
	RTrackLog* iClient;
};

#endif /*CTRACKLOGOBSERVERAO_H_*/

