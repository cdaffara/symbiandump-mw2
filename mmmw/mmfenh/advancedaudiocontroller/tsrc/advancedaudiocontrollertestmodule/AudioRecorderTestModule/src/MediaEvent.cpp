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

#include "MediaEvent.h"
const TInt KMaxInt32 = 2147483647;

CMediaEvent* CMediaEvent::NewL(CTestModuleIf &aTestModuleIf , CStifLogger &aLogger,
											TTimeIntervalMicroSeconds32 aDelay , TTimeIntervalMicroSeconds32 aInterval,
											MEventTarget *aTarget , CParameters *aParameters , TInt aPriority, TBool start)
	{
		CMediaEvent *__self = CMediaEvent::NewLC(aTestModuleIf , aLogger,
											aDelay , aInterval , aTarget,
											aParameters, aPriority, start);
		CleanupStack::Pop();
		return __self;
	}

CMediaEvent* CMediaEvent::NewLC(CTestModuleIf &aTestModuleIf , CStifLogger &aLogger,
											TTimeIntervalMicroSeconds32 aDelay , TTimeIntervalMicroSeconds32 aInterval,
											MEventTarget *aTarget, CParameters *aParameters , TInt aPriority, TBool start)
	{
		CMediaEvent *__self = new(ELeave) CMediaEvent(aTestModuleIf , aLogger , aDelay , aInterval , aTarget , aParameters);
		CleanupStack::PushL(__self);
		__self->ConstructL(aPriority,start);
		//CleanupStack::Pop();
		return __self;
	}

CMediaEvent::CMediaEvent (CTestModuleIf &aTestModuleIf , CStifLogger &aLogger,
						TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval,
						MEventTarget *aTarget , CParameters *aParameters)
					: iTestModuleIf(aTestModuleIf), iLogger(aLogger),
						iDelay(aDelay) , iInterval(aInterval) , iEventTarget(aTarget),
						iParameters(aParameters)
	{
		typeInt32 = true;
		iRoundingDelay = 0;
		iIterations = 1;
		iIterationCounter = 0;
		iCount = 0;
	}

void CMediaEvent::ConstructL(TInt aPriority, TBool start)
	{
	iPeriodic=CPeriodic::NewL(aPriority);
	if (start) Start();
	}

CMediaEvent::~CMediaEvent ()
	{
	iPeriodic->Cancel();
	delete iPeriodic;
	delete iParameters;
	}

TInt CMediaEvent::Tick(TAny *aObject)
	{
	return (static_cast<CMediaEvent*>(aObject) )->DoTick(); // cast, and call non-static function
	}

TInt CMediaEvent::DoTick()
	{
	iIterationCounter++;
	if (iIterations == iIterationCounter)
	{
		User::After(iRoundingDelay);

		TTime time;
		time.UniversalTime();
		//iLogger.Log(_L("Timer finish High: %u, Low: %u"),time.Int64().High(),time.Int64().Low());
		//TTimeIntervalMicroSeconds diff = time.MicroSecondsFrom(startTime);
		//iLogger.Log(_L("Timer real time elapsed High: %u, Low: %u"),diff.Int64().High(),diff.Int64().Low());

		if (! iEventTarget->ExecuteL(iParameters)) {
			iPeriodic->Cancel();
		}
		iIterationCounter = 0;
	}
	return 0;
	}

void CMediaEvent::Start()
{
	if (iPeriodic) iPeriodic->Start(iDelay , iInterval , TCallBack(Tick, this));

	startTime.UniversalTime();
	//iLogger.Log(_L("Timer start High: %u, Low: %u"),startTime.Int64().High(),startTime.Int64().Low());
}


//***************

CMediaEvent* CMediaEvent::NewL(CTestModuleIf &aTestModuleIf , CStifLogger &aLogger,
											TTimeIntervalMicroSeconds aDelay , TTimeIntervalMicroSeconds32 aInterval,
											MEventTarget *aTarget , CParameters *aParameters , TInt aPriority, TBool start)
	{
		CMediaEvent *__self = CMediaEvent::NewLC(aTestModuleIf , aLogger,
											aDelay , aInterval , aTarget,
											aParameters, aPriority,start);
		CleanupStack::Pop();
		return __self;
	}

CMediaEvent* CMediaEvent::NewLC(CTestModuleIf &aTestModuleIf , CStifLogger &aLogger,
											TTimeIntervalMicroSeconds aDelay , TTimeIntervalMicroSeconds32 aInterval,
											MEventTarget *aTarget, CParameters *aParameters , TInt aPriority, TBool start)
	{
		CMediaEvent *__self = new(ELeave) CMediaEvent(aTestModuleIf , aLogger , aDelay , aInterval , aTarget , aParameters);
		CleanupStack::PushL(__self);
		__self->ConstructL64(aPriority,start);
		//CleanupStack::Pop();
		return __self;
	}

CMediaEvent::CMediaEvent (CTestModuleIf &aTestModuleIf , CStifLogger &aLogger,
						TTimeIntervalMicroSeconds aDelay, TTimeIntervalMicroSeconds32 aInterval,
						MEventTarget *aTarget , CParameters *aParameters)
					: iTestModuleIf(aTestModuleIf), iLogger(aLogger),
						iDelay64(aDelay) , iInterval(aInterval) , iEventTarget(aTarget),
						iParameters(aParameters)
	{
		typeInt32 = false;
		iRoundingDelay = 0;
		iIterations = 1;
		iIterationCounter = 0;
		iCount = 0;
	}

void CMediaEvent::ConstructL64(TInt aPriority, TBool start)
	{
		TInt counter = 1;
		TInt64 delay = iDelay64.Int64();

		while (delay > TInt64(KMaxInt32))
		{
			delay = delay/2;
			counter = counter*2;
		}

	//	iRoundingDelay = (iDelay64.Int64() - delay*counter).GetTInt();
		iRoundingDelay = I64INT((iDelay64.Int64() - delay*counter));
		iIterations = counter;
		//iDelay = delay.GetTInt();
		iDelay = I64INT(delay);

		iPeriodic=CPeriodic::NewL(aPriority);
		if (start) Start();
	}

TInt CMediaEvent::GetCount() const
	{
	return iCount;
	}
