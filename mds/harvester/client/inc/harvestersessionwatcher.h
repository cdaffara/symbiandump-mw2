/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#ifndef HARVESTERSESSIONWATCHER_H
#define HARVESTERSESSIONWATCHER_H

#include <e32base.h>
#include <harvesterclient.h>

NONSHARABLE_CLASS( CHarvesterSessionWatcher ) : public CActive
	{
public:

    enum THarvesterWatcherState
        {
        EIdle,
        ERunning,
        ESCancelled
        };

	// Cancel and destroy
	~CHarvesterSessionWatcher();

	// Two-phased constructor.
	static CHarvesterSessionWatcher* NewL( MHarvesterSessionObserver& aObserver );

public:

    // from base class CActive
    void RunL();
    TInt RunError( TInt aError );
    void DoCancel();

private:	

    CHarvesterSessionWatcher( MHarvesterSessionObserver& aObserver );
    
    void ConstructL();
    
    void Start();

private:	
	
    RProcess iProcess;
    
    THarvesterWatcherState iState;
    
    MHarvesterSessionObserver* iObserver;   
	};

#endif // HARVESTERSESSIONWATCHER_H
