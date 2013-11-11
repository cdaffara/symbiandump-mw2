/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of settings api classes
*
*/



// INCLUDE FILES
#include "sessionobserver.h"

CSessionObserver::CSessionObserver()
    {
    }

// Destructor
CSessionObserver::~CSessionObserver()
    {
    }

// Two-phased constructor
CSessionObserver* CSessionObserver::NewL()
    {
    CSessionObserver* self = new (ELeave) CSessionObserver;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

void CSessionObserver::HandleSuplSessionChangeL (TSuplSessionEventType aEvent,TInt64 aSlpId)
	{
		//	iSettings->RemoveObserver();
		CActiveScheduler::Stop();
		
	}


// EPOC default constructor
void CSessionObserver::ConstructL()
    {
    
    
    }
