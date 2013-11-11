/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Client Observer for receiving events from TNE server
*
*/



#include "HXTneClientObserver.h"



CTNEClientObserver::CTNEClientObserver(CHXTNEVideoClipInfoImp *aImp): 
    CActive(EPriorityStandard)
    ,iImp(aImp) 
{
	CActiveScheduler::Add(this);
}

CTNEClientObserver::~CTNEClientObserver()
	{
		if(IsActive())
		Cancel();
	}
	
CTNEClientObserver* CTNEClientObserver::NewL(CHXTNEVideoClipInfoImp *aImp)
	{
	CTNEClientObserver* self = new (ELeave) CTNEClientObserver(aImp);	
	return self;
	}
	
void CTNEClientObserver::RunL()
{    	
    iImp->ServiceCompleted(iStatus.Int());
}

void CTNEClientObserver::DoCancel()
{
	
}

void CTNEClientObserver::Activate()
{
    if (!IsActive())
    {
        SetActive();
    }
}
