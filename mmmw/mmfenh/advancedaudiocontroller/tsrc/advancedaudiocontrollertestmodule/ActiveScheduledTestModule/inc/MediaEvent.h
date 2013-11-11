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
* Description:  Active Scheduler Test Module
*
*/

/*

  Author: Daniel Kurejwowski, dkurejwowski@dextratech.com
  Version: 3.0

*/

#ifndef __MEDIA_EVENT_H
#define __MEDIA_EVENT_H

#include <e32std.h>
#include <e32base.h>
#include "Parameters.h"
#include "EventTarget.h"
#include <StifTestModule.h>
#include <StifLogger.h>
#include "OutputPrinter.h"

class CMediaEvent: public CBase
	{
public:
	static CMediaEvent *NewL(COutputPrinter *aOutputPrinter, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);
	static CMediaEvent *NewLC(COutputPrinter *aOutputPrinter, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);

	TInt GetCount() const;

private:
	void ConstructL(TInt aPriority);
	~CMediaEvent();

	static TInt Tick(TAny* aObject);
	TInt DoTick();

protected:
	CMediaEvent(COutputPrinter *aOutputPrinter, TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval, MEventTarget *aTarget , /*TMediaCommand aCommand,*/ CParameters *aParameters);

private:
	//CTestModuleIf &iTestModuleIf;	//Test Module Interface
	//CLogger &iLogger;	//Test Module Logger
	COutputPrinter *iOutputPrinter;

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
	static CImmediateMediaEvent *NewLC(COutputPrinter *aOutputPrinter, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);
	static CImmediateMediaEvent *NewL(COutputPrinter *aOutputPrinter, MEventTarget *aTarget, CParameters *aParameters, TInt aPriority=CActive::EPriorityStandard);

	TBool FireExecute();

private:
	void ConstructL();
	~CImmediateMediaEvent();
	CImmediateMediaEvent(COutputPrinter *aOutputPrinter, MEventTarget *aTarget , CParameters *aParameters, TInt aPriority);

	void RunL();
	void DoCancel();

private:
	//CTestModuleIf &iTestModuleIf;	//Test Module Interface
	//CLogger &iLogger;	//Test Module Logger
	COutputPrinter *iOutputPrinter;

	MEventTarget *iEventTarget;
	CParameters *iParameters;

	TInt iCount;
	TBool iEnabled;
	};

#endif	//__MEDIA_EVENT_H
