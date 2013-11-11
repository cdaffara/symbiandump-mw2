/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Audio stream test component
*
*/


#ifndef __MEDIA_EVENT_H
#define __MEDIA_EVENT_H

#include <e32std.h>
#include <e32base.h>
#include "Parameters.h"
#include "EventTarget.h"
//#include <StifTestModule.h>
#include <StifLogger.h>

class CMediaEvent: public CBase
	{
public:
	static CMediaEvent *NewL(CStifLogger *aLogger, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);
	static CMediaEvent *NewLC(CStifLogger *aLogger, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);

	TInt GetCount() const;

private:
	void ConstructL(TInt aPriority);
	~CMediaEvent();

	static TInt Tick(TAny* aObject);
	TInt DoTick();

protected:
	CMediaEvent(CStifLogger *aLogger, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget , /*TMediaCommand aCommand,*/ CParameters *aParameters);

private:
	//CTestModuleIf &iTestModuleIf;	//Test Module Interface
	//CLogger &iLogger;	//Test Module Logger
	CStifLogger *iLogger;

	TTimeIntervalMicroSeconds32 iDelay;
	TTimeIntervalMicroSeconds32 iInterval;
	MEventTarget *iEventTarget;
	CParameters *iParameters;

	CPeriodic *iPeriodic;
	TInt iCount;
	};

class CImmediateMediaEvent : CActive
	{
public:
	static CImmediateMediaEvent *NewLC(CStifLogger *aLogger, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);
	static CImmediateMediaEvent *NewL(CStifLogger *aLogger, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);

	TBool FireExecute();

private:
	void ConstructL();
	~CImmediateMediaEvent();
	CImmediateMediaEvent(CStifLogger *aLogger, MEventTarget *aTarget , CParameters *aParameters, TInt aPriority);

	void RunL();
	void DoCancel();

private:
	//CTestModuleIf &iTestModuleIf;	//Test Module Interface
	//CLogger &iLogger;	//Test Module Logger
	CStifLogger *iLogger;

	MEventTarget *iEventTarget;
	CParameters *iParameters;

	TInt iCount;
	TBool iEnabled;
	};

#endif	//__MEDIA_EVENT_H
