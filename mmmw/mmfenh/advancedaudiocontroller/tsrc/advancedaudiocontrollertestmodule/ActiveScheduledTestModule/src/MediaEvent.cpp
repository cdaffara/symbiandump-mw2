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


#include "MediaEvent.h"

CMediaEvent* CMediaEvent::NewL(COutputPrinter *aOutputPrinter,
		TTimeIntervalMicroSeconds32 aDelay , TTimeIntervalMicroSeconds32 aInterval,
		MEventTarget *aTarget , CParameters *aParameters , TInt aPriority)
	{
	CMediaEvent *__self = CMediaEvent::NewLC(aOutputPrinter ,
										aDelay , aInterval , aTarget,
										aParameters, aPriority);
	CleanupStack::Pop(__self);
	return __self;
	}

CMediaEvent* CMediaEvent::NewLC(COutputPrinter *aOutputPrinter,
		TTimeIntervalMicroSeconds32 aDelay , TTimeIntervalMicroSeconds32 aInterval,
		MEventTarget *aTarget, CParameters *aParameters , TInt aPriority)
	{
		CMediaEvent *__self = new(ELeave) CMediaEvent(aOutputPrinter , aDelay , aInterval , aTarget , aParameters);
		CleanupStack::PushL(__self);
		__self->ConstructL(aPriority);
		return __self;
	}

CMediaEvent::CMediaEvent (COutputPrinter *aOutputPrinter,
		TTimeIntervalMicroSeconds32 aDelay, TTimeIntervalMicroSeconds32 aInterval,
		MEventTarget *aTarget , CParameters *aParameters)
	: iOutputPrinter(aOutputPrinter) ,
		iDelay(aDelay) , iInterval(aInterval) , iEventTarget(aTarget),
		iParameters(aParameters) , iCount(0)
	{}

void CMediaEvent::ConstructL(TInt aPriority=CActive::EPriorityStandard)
	{
	iOutputPrinter->Printf(_L("Creating event with priority (%d)") , aPriority);
	iPeriodic=CPeriodic::NewL(aPriority);
	iPeriodic->Start(iDelay , iInterval , TCallBack(Tick, this));
	}

CMediaEvent::~CMediaEvent ()
	{
	iPeriodic->Cancel();
	delete iPeriodic;
	delete iParameters;
	iParameters=NULL;
	}

TInt CMediaEvent::Tick(TAny *aObject)
	{
	return (static_cast<CMediaEvent*>(aObject) )->DoTick(); // cast, and call non-static function
	}

TInt CMediaEvent::DoTick()
	{
	iCount++;
	if (! iEventTarget->ExecuteL(iParameters)) {
		iPeriodic->Cancel();
	}
	return 0;
	}

TInt CMediaEvent::GetCount() const
	{
	return iCount;
	}


////////////////////////////
CImmediateMediaEvent* CImmediateMediaEvent::NewL(COutputPrinter *aOutputPrinter ,
											MEventTarget *aTarget , CParameters *aParameters , TInt aPriority)
	{
		CImmediateMediaEvent *__self = CImmediateMediaEvent::NewLC(aOutputPrinter, aTarget, aParameters, aPriority);

		CleanupStack::Pop(__self);
		return __self;
	}

CImmediateMediaEvent* CImmediateMediaEvent::NewLC(COutputPrinter *aOutputPrinter ,
											MEventTarget *aTarget, CParameters *aParameters , TInt aPriority)
	{
		CImmediateMediaEvent *__self = new(ELeave) CImmediateMediaEvent(aOutputPrinter , aTarget , aParameters, aPriority);
		CleanupStack::PushL(__self);
		__self->ConstructL();
		return __self;
	}

CImmediateMediaEvent::CImmediateMediaEvent (COutputPrinter *aOutputPrinter ,
						MEventTarget *aTarget , CParameters *aParameters, TInt aPriority)
					: CActive(aPriority) , iOutputPrinter(aOutputPrinter) ,
						iEventTarget(aTarget) , iParameters(aParameters) , iEnabled(ETrue)
	{}

void CImmediateMediaEvent::ConstructL(/*TInt aPriority=CActive::EPriorityStandard*/)
	{
	iOutputPrinter->Printf(_L("Creating event with priority (%d)") , Priority() );
	//iPeriodic=CPeriodic::NewL(aPriority);
	//iPeriodic->Start(iDelay , iInterval , TCallBack(Tick, this));
	CActiveScheduler::Add(this);
	}

CImmediateMediaEvent::~CImmediateMediaEvent ()
	{
	delete iParameters;
	iParameters=NULL;
	Cancel();
	}

void CImmediateMediaEvent::RunL()
	{
	if (iEnabled)
		{
		iEventTarget->ExecuteL(iParameters);
		}
	}

void CImmediateMediaEvent::DoCancel()
	{
	iEnabled = EFalse;
	}

TBool CImmediateMediaEvent::FireExecute()
	{
	//__ASSERT_ALWAYS(!IsActive(), User::Panic(_L("Error with the "), KErrGeneral));
	if ( IsActive() ) return EFalse;
	iEnabled = ETrue;
	TRequestStatus *status = &iStatus;
	iStatus = KRequestPending; // service request would be made here and pending set by service provider
	SetActive();
	User::RequestComplete( status , KErrNone);
	return ETrue;
	}
