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
* Description:  observer file
*
*/


#ifndef MYOBSERVER_H
#define MYOBSERVER_H

//#include "CreateSessiontrigger.h"
#include <lbt.h>
#include <StifLogger.h>

class CMyObserver;

NONSHARABLE_CLASS(CMyObserver) : public CActive
{
protected:
CActiveSchedulerWait* iwait;
public:
	CMyObserver();
	~CMyObserver();
	//virtual TLbtTriggerId CallCreateTrigger(RLbt& ,CLbtTriggerEntry **,TLbtTriggerId,TBool,TRequestStatus);
	virtual void MySetActive();
	static CMyObserver* NewL() ;
	virtual void RunL();
	virtual void DoCancel();
	virtual void AddAO();
	virtual void GiveHandle(CActiveSchedulerWait*);
	
};
#endif