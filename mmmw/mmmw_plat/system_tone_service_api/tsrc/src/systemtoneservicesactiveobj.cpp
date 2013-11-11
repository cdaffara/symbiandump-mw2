/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains testclass implementation.
*
*/

#include <e32base.h>
#include <systemtoneservice.h>
#include "systemtoneservicesactiveobj.h"


CStsActiveObj::CStsActiveObj() : CActive (CActive::EPriorityStandard)
{
   
}


void CStsActiveObj::ConstructL(MStsPlayAlarmObserver* aObserver, CStifLogger* aLogger) 
{  
	iObserver = aObserver;
    iLog = aLogger;
    CActiveScheduler::Add(this);
}

CStsActiveObj* CStsActiveObj::NewL( MStsPlayAlarmObserver* aTestClass, CStifLogger* aLogger)
{
    CStsActiveObj* self = new ( ELeave ) CStsActiveObj();
    CleanupStack::PushL( self );
    self->ConstructL( aTestClass, aLogger);
    CleanupStack::Pop( self );
    return self;

}

CStsActiveObj::~ CStsActiveObj()
{
		if(IsActive())
    {
        Cancel();
    }
}


void  CStsActiveObj::RunL()
{
	/*(if(IsActive())
    {
        Cancel();
    }
    
    SetActive();*/
}


void  CStsActiveObj::DoCancel()
{
    if(IsActive())
    {
        Cancel();
    }
}


TInt  CStsActiveObj::RunError( TInt aError )
{
    
    iObserver->PlayAlarmComplete(aError);
    return aError;
}

