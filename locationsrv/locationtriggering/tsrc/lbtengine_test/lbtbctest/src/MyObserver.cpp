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
* Description:    Test module to implement Test cases for BC
*
*/



#include "MyObserver.h"


CMyObserver ::CMyObserver()
:CActive( CActive::EPriorityStandard )
{

}
CMyObserver ::~CMyObserver()
{
	Cancel();
}
/*TLbtTriggerId CMyObserver::CallCreateTrigger(RLbt &lbt,CLbtTriggerEntry **trig,TLbtTriggerId trigid,TBool cond,TRequestStatus aStatus)
{
	
	//lbt.CreateTrigger(*trig,trigid,cond,aStatus);
	//SetActive();
	return trigid;
}*/

void CMyObserver ::MySetActive()
{
//CActiveScheduler::Add(this);
SetActive();	
}
void CMyObserver ::AddAO()
{
CActiveScheduler::Add(this);
}

CMyObserver * CMyObserver ::NewL()
{
	CMyObserver *result = new CMyObserver;
	result->AddAO();
	return result;
	
}
void CMyObserver ::RunL()
{
//	iLog->Log(_L("Session trigger created %d"),iStatus); 
iwait->AsyncStop();
		
}
void CMyObserver ::DoCancel()
{
		
}

void CMyObserver::GiveHandle(CActiveSchedulerWait* await)
{
	iwait=await;
}

