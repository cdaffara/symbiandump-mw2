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
* Description:  AudioRecorder Test
*
*/

#ifndef __MEDIA_EVENT_H
#define __MEDIA_EVENT_H

#include <e32std.h>
#include <e32base.h>
#include "Parameters.h"
#include "EventTarget.h"
#include <StifTestModule.h>
#include <StifLogger.h>


class CMediaEvent: public CBase
	{
public:
	static CMediaEvent *NewL(CTestModuleIf &aTestModuleIf, CStifLogger &aLogger, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard, TBool start = true);
	static CMediaEvent *NewLC(CTestModuleIf &aTestModuleIf, CStifLogger &aLogger, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard, TBool start = true);

	static CMediaEvent *NewL(CTestModuleIf &aTestModuleIf, CStifLogger &aLogger, TTimeIntervalMicroSeconds aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard, TBool start = true);
	static CMediaEvent *NewLC(CTestModuleIf &aTestModuleIf, CStifLogger &aLogger, TTimeIntervalMicroSeconds aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard, TBool start = true);

	TInt GetCount() const;
	void Start();

private:
	void ConstructL(TInt aPriority, TBool start);
	void ConstructL64(TInt aPriority, TBool start);
	~CMediaEvent();

	static TInt Tick(TAny* aObject);
	TInt DoTick();

protected:
	CMediaEvent(CTestModuleIf &aTestModuleIf , CStifLogger &aLogger, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget , CParameters *aParameters);
	CMediaEvent(CTestModuleIf &aTestModuleIf , CStifLogger &aLogger, TTimeIntervalMicroSeconds aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget , CParameters *aParameters);


private:
	CTestModuleIf &iTestModuleIf;	//Test Module Interface
	CStifLogger &iLogger;	//Test Module Logger

	TTimeIntervalMicroSeconds32 iDelay;
	TTimeIntervalMicroSeconds iDelay64;
	TTimeIntervalMicroSeconds32 iInterval;

	TInt64 iIterations;
	TInt64 iIterationCounter;
	TInt iRoundingDelay;

	MEventTarget *iEventTarget;
	CParameters *iParameters;

	CPeriodic *iPeriodic;
	TInt iCount;
	TBool typeInt32;

	TTime startTime;
	};

#endif	//__MEDIA_EVENT_H
